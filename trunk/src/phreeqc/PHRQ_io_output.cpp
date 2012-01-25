#include <assert.h>
#include "Phreeqc.h"
#include "phqalloc.h"

/* ---------------------------------------------------------------------- */
int Phreeqc::
warning_msg(const char *err_str)
/* ---------------------------------------------------------------------- */
{
	if (state == TRANSPORT && transport_warnings == FALSE)
		return (OK);
	if (state == ADVECTION && advection_warnings == FALSE)
		return (OK);
	count_warnings++;
	if (pr.warnings >= 0)
	{
		if (count_warnings > pr.warnings)
			return (OK);
	}
	if (phrq_io)
	{
		if (status_on)
		{
			phrq_io->screen_msg("\n");
		}
		std::ostringstream msg;
		msg << "WARNING: " << err_str;
		phrq_io->warning_msg(msg.str().c_str());
	}
	
	return OK;
}

/* ---------------------------------------------------------------------- */
void Phreeqc::
echo_msg(const char *str)
/* ---------------------------------------------------------------------- */
{
	if (pr.echo_input == TRUE)
	{
		if (phrq_io) phrq_io->echo_msg(str);
	}
}

/* ---------------------------------------------------------------------- */
void Phreeqc::
set_forward_output_to_log(int value)
/* ---------------------------------------------------------------------- */
{
	forward_output_to_log = value;
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
get_forward_output_to_log(void)
/* ---------------------------------------------------------------------- */
{
	return forward_output_to_log;
}

#if defined(HDF5_CREATE)
extern void HDFWriteHyperSlabV(const char *name, const char *format,
							   va_list argptr);
#endif

#if defined(USE_MPI) && defined(HDF5_CREATE) && defined(MERGE_FILES)
extern int Merge_fpunchf(const int length, const char *format,
						 va_list argptr);
#endif
void Phreeqc::
fpunchf_heading(const char *name)
{
	if (pr.punch == TRUE && punch.in == TRUE)
	{
		punch_msg(name);
	}
}
void Phreeqc::
fpunchf(const char *name, const char *format, double d)
{
	if (phrq_io) phrq_io->fpunchf(name, format, d);
}
void Phreeqc::
fpunchf(const char *name, const char *format, char * s)
{
	if (phrq_io) phrq_io->fpunchf(name, format, s);
}
void Phreeqc::
fpunchf(const char *name, const char *format, int d)
{
	if (phrq_io) phrq_io->fpunchf(name, format, d);
}

void Phreeqc::
fpunchf_user(int user_index, const char *format, double d)
{
	static int s_warning = 0;
	static char buffer[80];
	const char *name;

	// check headings
	if (user_index < user_punch_count_headings)
	{
		name = user_punch_headings[user_index];
	}
	else
	{
		if (s_warning == 0)
		{
			error_string = sformatf(
					"USER_PUNCH: Headings count doesn't match number of calls to PUNCH.\n");
			warning_msg(error_string);
			s_warning = 1;
		}
		sprintf(buffer, "no_heading_%d",
				(user_index - user_punch_count_headings) + 1);
		name = buffer;
	}
	if (phrq_io) phrq_io->fpunchf(name, format, (double) d);
}

void Phreeqc::
fpunchf_user(int user_index, const char *format, char * d)
{
	static int s_warning = 0;
	static char buffer[80];
	const char *name;

	// check headings
	if (user_index < user_punch_count_headings)
	{
		name = user_punch_headings[user_index];
	}
	else
	{
		if (s_warning == 0)
		{
			error_string = sformatf(
					"USER_PUNCH: Headings count doesn't match number of calls to PUNCH.\n");
			warning_msg(error_string);
			s_warning = 1;
		}
		sprintf(buffer, "no_heading_%d",
				(user_index - user_punch_count_headings) + 1);
		name = buffer;
	}
	if (phrq_io) phrq_io->fpunchf(name, format, d);
}

int Phreeqc::
fpunchf_end_row(const char *format)
{
	if (phrq_io) 
	{
		phrq_io->fpunchf_end_row(format);
	}
	return OK;
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
process_file_names(int argc, char *argv[], std::istream **db_cookie,
				   std::istream **input_cookie, int log)
/* ---------------------------------------------------------------------- */
{
	int l;
	char token[2 * MAX_LENGTH], default_name[2 * MAX_LENGTH];
	char query[2 * MAX_LENGTH];
	char in_file[2 * MAX_LENGTH], out_file[2 * MAX_LENGTH], db_file[2 * MAX_LENGTH];
	char *env_ptr;
	char *ptr;
/*
 *   Prepare error handling
 */
	try {
		if (phrq_io == NULL) 
		{
			std::cerr << "No PHRQ_io output handler defined in process_file_names" << "\n";
		}
/*
 *   Prep for get_line
 */
		max_line = MAX_LINE;
		space((void **) ((void *) &line), INIT, &max_line, sizeof(char));
		space((void **) ((void *) &line_save), INIT, &max_line, sizeof(char));
/*
 *   Open error ostream
 */
		if (argc > 4)
		{
			if (!phrq_io->error_open(argv[4]))
			{
				error_string = sformatf( "Error opening file, %s.", argv[4]);
				warning_msg(error_string);
			}
		}
		else
		{
			phrq_io->error_open(NULL);
		}
/*
 *   For now, set screen ostream to error ostream
 */
		//phrq_io->Set_screen_ostream(phrq_io->Get_error_ostream());
/*
 *   Open user-input file
 */
		strcpy(query, "Name of input file?");
		std::ifstream * local_input_stream = NULL;
		if (argc <= 1)
		{
			default_name[0] = '\0';
			local_input_stream = open_input_stream(query, default_name, std::ios_base::in, false);
		}
		else
		{
			strcpy(default_name, argv[1]);
			local_input_stream = open_input_stream(query, default_name, std::ios_base::in, true);
		}
		screen_msg(sformatf("Input file: %s\n\n", default_name));
		strcpy(in_file, default_name);
/*
 *   Open file for output
 */
		strcpy(query, "Name of output file?");
		ptr = default_name;
		copy_token(token, &ptr, &l);
		strcat(token, ".out");
		std::ofstream * local_output_stream;
		if (argc <= 1)
		{
			local_output_stream = open_output_stream(query, token, std::ios_base::out, false);
		}
		else if (argc == 2)
		{
			local_output_stream = open_output_stream(query, token, std::ios_base::out, true);
		}
		else if (argc >= 3)
		{
			strcpy(token, argv[2]);
			local_output_stream = open_output_stream(query, token, std::ios_base::out, true);
		}
		screen_msg(sformatf("Output file: %s\n\n", token));
		strcpy(out_file, token);
		phrq_io->Set_output_ostream(local_output_stream);
/*
 *   Open log file
 */
		if (log == TRUE)
		{
			if (!phrq_io->log_open("phreeqc.log"))
			{
				error_msg("Can't open log file, phreeqc.log.", STOP);
			}
		}
/*
 *  Read input file for DATABASE keyword
 */
		if (local_input_stream->is_open())
		{
			phrq_io->push_istream(local_input_stream);
			if (get_line() == KEYWORD)
			{
				ptr = line;
				copy_token(token, &ptr, &l);
				if (strcmp_nocase(token, "database") == 0)
				{
#ifdef PHREEQ98
					user_database = string_duplicate(prefix_database_dir(ptr));
#else
					user_database = string_duplicate(ptr);
#endif
					if (string_trim(user_database) == EMPTY)
					{
						warning_msg("DATABASE file name is missing; default database will be used.");
						user_database = (char *) free_check_null(user_database);
					}
				}
			}
			phrq_io->pop_istream();
		}
		else
		{
			delete local_input_stream;
			error_string = sformatf( "Error opening file, %s.", in_file);
			error_msg(error_string, STOP);
		}
		
/*
 *   Open data base
 */
		strcpy(query, "Name of database file?");
		env_ptr = getenv("PHREEQC_DATABASE");
		if (user_database != NULL)
		{
			strcpy(token, user_database);
		}
		else if (env_ptr != NULL)
		{
			strcpy(token, env_ptr);
		}
		else
		{
			strcpy(token, default_data_base);
		}

		std::ifstream * local_database_file;
		if (argc <= 1)
		{
			local_database_file = open_input_stream(query, token, std::ios_base::in, false);
		}
		else if (argc < 4)
		{
			local_database_file = open_input_stream(query, token, std::ios_base::in, true);
		}
		else if (argc >= 4)
		{
			if (user_database == NULL)
			{
				strcpy(token, argv[3]);
			}
			else
			{
#ifndef PHREEQCI_GUI
				warning_msg	("Database file from DATABASE keyword is used; command line argument ignored.");
#endif
			}
			local_database_file = open_input_stream(query, token, std::ios_base::in, true);
		}
		local_database_file->close();
		delete local_database_file;
		//if (local_database_file != NULL)
		//{
		//	if (phrq_io) phrq_io->Set_database_file(local_database_file);
		//}
		screen_msg(sformatf("Database file: %s\n\n", token));
		strcpy(db_file, token);

		output_msg(sformatf("   Input file: %s\n", in_file));
		output_msg(sformatf("  Output file: %s\n", out_file));
		output_msg(sformatf("Database file: %s\n\n", token));
		/*
		*   local cleanup
		*/
		user_database = (char *) free_check_null(user_database);
		line = (char *) free_check_null(line);
		line_save = (char *) free_check_null(line_save);

		*db_cookie = new std::ifstream(db_file, std::ios_base::in);
		*input_cookie = new std::ifstream(in_file, std::ios_base::in);
	}
	catch (PhreeqcStop e)
	{
		return get_input_errors();
	}
	return 0;
}
/* ---------------------------------------------------------------------- */
std::ifstream * Phreeqc::
open_input_stream(char *query, char *default_name, std::ios_base::openmode mode, bool batch)
/* ---------------------------------------------------------------------- */
{
	char name[MAX_LENGTH];
	std::ifstream *new_stream;
	int l;

	//std::ostream * screen_file_save = phrq_io->Get_screen_ostream();
	std::ostream * error_file_save = phrq_io->Get_error_ostream();

	for (;;)
	{
/*
 *   Get file name
 */
		strcpy(name, default_name);
		if (!batch )
		{
			//phrq_io->Set_screen_ostream(&std::cerr);
			phrq_io->Set_error_ostream(&std::cerr);
			screen_msg(sformatf("%s\n", query));;
			if (default_name[0] != '\0')
			{
				screen_msg(sformatf("Default: %s\n", default_name));
			}
			fgets(name, MAX_LENGTH, stdin);
			l = (int) strlen(name);
			name[l - 1] = '\0';
			if (name[0] == '\0')
			{
				strcpy(name, default_name);
			}
		}
/*
 *   Open existing file to read
 */
		new_stream = new std::ifstream(name, mode);
		if (new_stream == NULL || !new_stream->is_open())
		{
			//phrq_io->Set_screen_ostream(&std::cerr);
			phrq_io->Set_error_ostream(&std::cerr);
			error_string = sformatf( "\nERROR: Can't open file, %s.\n", name);
			screen_msg(error_string);
			//screen_flush();
			error_flush();
			batch = FALSE;
			continue;		
		}
		break;
	}
	strncpy(default_name, name, MAX_LENGTH);
	if (!batch )
	{
		//phrq_io->Set_screen_ostream(screen_file_save);
		phrq_io->Set_error_ostream(error_file_save);
	}
	return (new_stream);
}
/* ---------------------------------------------------------------------- */
std::ofstream * Phreeqc::
open_output_stream(char *query, char *default_name, std::ios_base::openmode mode, bool batch)
/* ---------------------------------------------------------------------- */
{
	char name[MAX_LENGTH];
	std::ofstream *new_stream;
	int l;

	//std::ostream * screen_file_save = phrq_io->Get_screen_ostream();
	std::ostream * error_file_save = phrq_io->Get_error_ostream();

	for (;;)
	{
/*
 *   Get file name
 */
		strcpy(name, default_name);
		if (!batch )
		{
			//phrq_io->Set_screen_ostream(&std::cerr);
			phrq_io->Set_error_ostream(&std::cerr);
			screen_msg(sformatf("%s\n", query));;
			if (default_name[0] != '\0')
			{
				screen_msg(sformatf("Default: %s\n", default_name));
			}
			fgets(name, MAX_LENGTH, stdin);
			l = (int) strlen(name);
			name[l - 1] = '\0';
			if (name[0] == '\0')
			{
				strcpy(name, default_name);
			}
		}
/*
 *   Open existing file to read
 */
		new_stream = new std::ofstream(name, mode);
		if (new_stream == NULL || !new_stream->is_open())
		{
			//phrq_io->Set_screen_ostream(&std::cerr);
			phrq_io->Set_error_ostream(&std::cerr);
			error_string = sformatf( "\nERROR: Can't open file, %s.\n", name);
			screen_msg( error_string);
			//screen_flush();
			error_flush();
			batch = FALSE;
			continue;		
		}
		break;
	}
	strncpy(default_name, name, MAX_LENGTH);
	if (!batch )
	{
		//phrq_io->Set_screen_ostream(screen_file_save);
		phrq_io->Set_error_ostream(error_file_save);
	}
	return (new_stream);
}
// ---------------------------------------------------------------------- */
// screen ostream methods
// ---------------------------------------------------------------------- */
#ifdef SKIP
/* ---------------------------------------------------------------------- */
bool Phreeqc::
screen_open(const char *file_name)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io)
	{
		return this->phrq_io->screen_open(file_name);
	}
	return false;
}
/* ---------------------------------------------------------------------- */
void Phreeqc::
screen_flush(void)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io) this->phrq_io->screen_flush();
}
/* ---------------------------------------------------------------------- */
void Phreeqc::
screen_close(void)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io) this->phrq_io->screen_close();
}
#endif
/* ---------------------------------------------------------------------- */
void Phreeqc::
screen_msg(const char *err_str)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io) phrq_io->screen_msg(err_str);
}
// ---------------------------------------------------------------------- */
// dump file methods
// ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
bool Phreeqc::
dump_open(const char *file_name)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io)
		return this->phrq_io->dump_open(file_name);
	return false;
}
/* ---------------------------------------------------------------------- */
void Phreeqc::
dump_flush(void)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io) this->phrq_io->dump_flush();
}
/* ---------------------------------------------------------------------- */
void Phreeqc::
dump_close(void)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io) this->phrq_io->dump_close();
}
///* ---------------------------------------------------------------------- */
//void Phreeqc::
//dump_rewind(void)
///* ---------------------------------------------------------------------- */
//{
//	if (phrq_io) this->phrq_io->dump_rewind();
//}
///* ---------------------------------------------------------------------- */
//bool Phreeqc::
//dump_isopen(void)
///* ---------------------------------------------------------------------- */
//{
//	if (phrq_io)
//		return this->phrq_io->dump_isopen();
//	return false;
//}
/* ---------------------------------------------------------------------- */
void Phreeqc::
dump_msg(const char * str)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io) this->phrq_io->dump_msg(str);
}
// ---------------------------------------------------------------------- */
// error file methods
// ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
bool Phreeqc::
error_open(const char *file_name)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io)
		return this->phrq_io->error_open(file_name);
	return false;
}
/* ---------------------------------------------------------------------- */
void Phreeqc::
error_flush(void)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io) this->phrq_io->error_flush();
}
/* ---------------------------------------------------------------------- */
void Phreeqc::
error_close(void)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io) this->phrq_io->error_close();
}
///* ---------------------------------------------------------------------- */
//void Phreeqc::
//error_rewind(void)
///* ---------------------------------------------------------------------- */
//{
//	if (phrq_io) this->phrq_io->error_rewind();
//}
///* ---------------------------------------------------------------------- */
//bool Phreeqc::
//error_isopen(void)
///* ---------------------------------------------------------------------- */
//{
//	if (phrq_io)
//		return this->phrq_io->error_isopen();
//	return false;
//}
/* ---------------------------------------------------------------------- */
void Phreeqc::
error_msg(const char *err_str, bool stop)
/* ---------------------------------------------------------------------- */
{
	if (get_input_errors() <= 0)
		input_error = 1;
	if (phrq_io)
	{
		std::ostringstream msg;
		msg << "ERROR: " << err_str << "\n";

		phrq_io->output_msg(msg.str().c_str());
		phrq_io->log_msg(msg.str().c_str());

		if (status_on)
		{
			phrq_io->screen_msg("\n");
		}
		phrq_io->error_msg(msg.str().c_str(), stop);
	}

	if (stop)
	{
		throw PhreeqcStop();
	}
}
// ---------------------------------------------------------------------- */
// log file methods
// ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
bool Phreeqc::
log_open(const char *file_name)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io)
		return this->phrq_io->log_open(file_name);
	return false;
}
/* ---------------------------------------------------------------------- */
void Phreeqc::
log_flush(void)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io) this->phrq_io->log_flush();
}
/* ---------------------------------------------------------------------- */
void Phreeqc::
log_close(void)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io) this->phrq_io->log_close();
}
///* ---------------------------------------------------------------------- */
//void Phreeqc::
//log_rewind(void)
///* ---------------------------------------------------------------------- */
//{
//	if (phrq_io) this->phrq_io->log_rewind();
//}
///* ---------------------------------------------------------------------- */
//bool Phreeqc::
//log_isopen(void)
///* ---------------------------------------------------------------------- */
//{
//	if (phrq_io) 
//		return this->phrq_io->log_isopen();
//	return false;
//}
/* ---------------------------------------------------------------------- */
void Phreeqc::
log_msg(const char * str)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io) this->phrq_io->log_msg(str);
}
// ---------------------------------------------------------------------- */
// output_temp file methods
// ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
bool Phreeqc::
output_open(const char *file_name)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io) 
		return this->phrq_io->output_open(file_name);
	return false;
}
/* ---------------------------------------------------------------------- */
void Phreeqc::
output_flush(void)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io) this->phrq_io->output_flush();
}
/* ---------------------------------------------------------------------- */
void Phreeqc::
output_close(void)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io) this->phrq_io->output_close();
}
///* ---------------------------------------------------------------------- */
//void Phreeqc::
//output_rewind(void)
///* ---------------------------------------------------------------------- */
//{
//	if (phrq_io) this->phrq_io->output_rewind();
//}
///* ---------------------------------------------------------------------- */
//bool Phreeqc::
//output_isopen(void)
///* ---------------------------------------------------------------------- */
//{
//	if (phrq_io)
//		return this->phrq_io->output_isopen();
//	return false;
//}
/* ---------------------------------------------------------------------- */
void Phreeqc::
output_msg(const char * str)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io)
	{
		if (get_forward_output_to_log())
		{
			phrq_io->log_msg(str);
		}
		else
		{
			phrq_io->output_msg(str);
		}
	}
}
// ---------------------------------------------------------------------- */
// punch file methods
// ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
bool Phreeqc::
punch_open(const char *file_name)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io)
		return this->phrq_io->punch_open(file_name);
	return false;
}
/* ---------------------------------------------------------------------- */
void Phreeqc::
punch_flush(void)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io) this->phrq_io->punch_flush();
}
/* ---------------------------------------------------------------------- */
void Phreeqc::
punch_close(void)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io) this->phrq_io->punch_close();
}
///* ---------------------------------------------------------------------- */
//void Phreeqc::
//punch_rewind(void)
///* ---------------------------------------------------------------------- */
//{
//	if (phrq_io) this->phrq_io->punch_rewind();
//}
///* ---------------------------------------------------------------------- */
//bool Phreeqc::
//punch_isopen(void)
///* ---------------------------------------------------------------------- */
//{
//	if (phrq_io)
//		return this->phrq_io->punch_isopen();
//	return false;
//}
/* ---------------------------------------------------------------------- */
void Phreeqc::
punch_msg(const char * str)
/* ---------------------------------------------------------------------- */
{
	if (phrq_io) this->phrq_io->punch_msg(str);
}
