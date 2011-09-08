#include <assert.h>
#include "PHRQ_io.h"

#if defined(MERGE_INCLUDE_FILES) 
#include <algorithm>
#include <cctype>
#include <fstream>
#endif /* if defined(MERGE_INCLUDE_FILES) */

PHRQ_io::
PHRQ_io(void)
{
	input_file = NULL;
	database_file = NULL;
	output_file = NULL;	/* OUTPUT_MESSAGE */
	log_file = NULL;	/* OUTPUT_LOG */
	punch_file = NULL;	/* OUTPUT_PUNCH */
	error_file = NULL;	/* OUTPUT_ERROR */
	dump_file = NULL;	/* OUTPUT_DUMP */
	int error_count = 0;
}

PHRQ_io::
~PHRQ_io()
{
}

/* ---------------------------------------------------------------------- */
int PHRQ_io::
phreeqc_handler(const int action, const int type, const char *err_str,
				const bool stop, const char *format,
				va_list args)
/* ---------------------------------------------------------------------- */
{
	int i;

	switch (action)
	{
	case ACTION_OPEN:
		return this->open_handler(type, err_str);
		break;
	case ACTION_OUTPUT:
		return this->output_handler(type, err_str, stop, format, args);
		break;
	case ACTION_FLUSH:
		return this->fileop_handler(type, fflush);
		break;
	case ACTION_REWIND:
		return this->fileop_handler(type, &PHRQ_io::rewind_wrapper);
		break;
	case ACTION_CLOSE:

		i = this->fileop_handler(type, fclose);
		switch (type)
		{
		case OUTPUT_ERROR:
			this->error_file = NULL;
			break;

		case OUTPUT_WARNING:
			break;

		case OUTPUT_MESSAGE:
			this->output_file = NULL;
			break;

		case OUTPUT_PUNCH:
			this->punch_file = NULL;
			break;

		case OUTPUT_SCREEN:
			break;

		case OUTPUT_LOG:
			this->log_file = NULL;
			break;

		case OUTPUT_STDERR:
			break;

		case OUTPUT_DUMP:
			this->dump_file = NULL;
			break;
		}


		return (i);
		break;
	}
	return false;
}

/* ---------------------------------------------------------------------- */
int PHRQ_io::
close_input_files(void)
/* ---------------------------------------------------------------------- */
{
	int i = 0;
	if (database_file)
	{
		i |= fclose(database_file);
	}
	if (input_file)
	{
		i |= fclose(input_file);
	}
	input_file = database_file = NULL;
	return (i);
}

///* ---------------------------------------------------------------------- */
//int PHRQ_io::
//process_file_names(int argc, char *argv[], void **db_cookie,
//				   void **input_cookie, int log)
///* ---------------------------------------------------------------------- */
//{
//	int l;
//	char token[2 * MAX_LENGTH], default_name[2 * MAX_LENGTH];
//	char query[2 * MAX_LENGTH];
//	char in_file[2 * MAX_LENGTH], out_file[2 * MAX_LENGTH], db_file[2 * MAX_LENGTH];
//	char *env_ptr;
//	char *ptr;
//	int errors;
//	ENTRY item, *found_item;
//
///*
// *   Prepare error handling
// */
//	errors = setjmp(mark);
//	if (errors != 0)
//	{
//		return errors;
//	}
//
///*
// *   Prep for get_line
// */
//	max_line = MAX_LINE;
//	space((void **) ((void *) &line), INIT, &max_line, sizeof(char));
//	space((void **) ((void *) &line_save), INIT, &max_line, sizeof(char));
//	hcreate_multi(5, &strings_hash_table);
//	hcreate_multi(2, &keyword_hash_table);
//
///*
// *   Initialize hash table
// */
//	keyword_hash = (struct key *) PHRQ_malloc(sizeof(struct key));
//	if (keyword_hash == NULL)
//	{
//		malloc_error();
//	}
//	else
//	{
//		keyword_hash->name = string_hsave("database");
//		keyword_hash->keycount = 0;
//		item.key = keyword_hash->name;
//		item.data = keyword_hash;
//		found_item = hsearch_multi(keyword_hash_table, item, ENTER);
//		if (found_item == NULL)
//		{
//			sprintf(error_string,
//					"Hash table error in keyword initialization.");
//			error_msg(error_string, STOP);
//		}
//	}
//
//
///*
// *   Open file for screen output
// */
//	if (argc > 4)
//	{
//		error_file = fopen(argv[4], "w");
//		if (error_file == NULL)
//		{
//#ifdef PHREEQ98
//			error_file = NULL;	/* We can not use stderr in BCB: this causes unexpected behaviour when reopening the input_file... */
//#else
//			error_file = stderr;
//#endif
//			sprintf(error_string, "Error opening file, %s.", argv[4]);
//			warning_msg(error_string);
//		}
//	}
//	else
//	{
//#ifdef PHREEQ98
//		error_file = NULL;		/* We can not use stderr in BCB: this causes unexpected behaviour when reopening the input_file... */
//#else
//		error_file = stderr;
//#endif
//	}
//
///*
// *   Open user-input file
// */
//	strcpy(query, "Name of input file?");
//	if (argc <= 1)
//	{
//		default_name[0] = '\0';
//		input_file = file_open(query, default_name, "r", FALSE);
//	}
//	else
//	{
//		strcpy(default_name, argv[1]);
//		input_file = file_open(query, default_name, "r", TRUE);
//	}
//	output_msg(OUTPUT_SCREEN, "Input file: %s\n\n", default_name);
//	output_msg(OUTPUT_SEND_MESSAGE, "Input file: %s\r\n\r\n", default_name);
//	strcpy(in_file, default_name);
///*
// *   Open file for output
// */
//	strcpy(query, "Name of output file?");
//
//	ptr = default_name;
//	copy_token(token, &ptr, &l);
//	strcat(token, ".out");
//	if (argc <= 1)
//	{
//		output_file = file_open(query, token, "w", FALSE);
//	}
//	else if (argc == 2)
//	{
//		output_file = file_open(query, token, "w", TRUE);
//	}
//	else if (argc >= 3)
//	{
//		strcpy(token, argv[2]);
//		output_file = file_open(query, token, "w", TRUE);
//	}
//	output_msg(OUTPUT_SCREEN, "Output file: %s\n\n", token);
//	output_msg(OUTPUT_SEND_MESSAGE, "Output file: %s\r\n\r\n", token);
//	strcpy(out_file, token);
///*
// *   Open file for errors
// */
//	if (log == TRUE)
//	{
//#ifdef PHREEQ98
//		if ((log_file = fopen(LogFileNameC, "w")) == NULL)
//		{
//#else /* PHREEQ98 */
//		if ((log_file = fopen("phreeqc.log", "w")) == NULL)
//		{
//#endif /* PHREEQ98 */
//			error_msg("Can't open log file, phreeqc.log.", STOP);
//		}
//	}
//	/*
//	 *  Read input file for DATABASE keyword
//	 */
//	std::ifstream * temp_input = new std::ifstream(in_file, std::ifstream::in);
//	this->set_cookie(temp_input);
//	if (get_line(istream_getc, temp_input) == KEYWORD)
//	{
//		ptr = line;
//		copy_token(token, &ptr, &l);
//		if (strcmp_nocase(token, "database") == 0)
//		{
//#ifdef PHREEQ98
//			user_database = string_duplicate(prefix_database_dir(ptr));
//#else
//			user_database = string_duplicate(ptr);
//#endif
//			if (string_trim(user_database) == EMPTY)
//			{
//				warning_msg
//					("DATABASE file name is missing; default database will be used.");
//				user_database = (char *) free_check_null(user_database);
//			}
//		}
//	}
//	fclose(input_file);
//	this->pop_cookie();
//
//	if ((input_file = fopen(in_file, "r")) == NULL)
//	{;
//		error_msg("Can't reopen input file.", STOP);
//	}
///*
// *   Open data base
// */
//	strcpy(query, "Name of database file?");
//	env_ptr = getenv("PHREEQC_DATABASE");
//	if (user_database != NULL)
//	{
//		strcpy(token, user_database);
//	}
//	else if (env_ptr != NULL)
//	{
//		strcpy(token, env_ptr);
//	}
//	else
//	{
//		strcpy(token, default_data_base);
//	}
//	if (argc <= 1)
//	{
//		database_file = file_open(query, token, "r", FALSE);
//	}
//	else if (argc < 4)
//	{
//		database_file = file_open(query, token, "r", TRUE);
//	}
//	else if (argc >= 4)
//	{
//		if (user_database == NULL)
//		{
//			strcpy(token, argv[3]);
//		}
//		else
//		{
//#ifndef PHREEQCI_GUI
//			warning_msg
//				("Database file from DATABASE keyword is used; command line argument ignored.");
//#endif
//		}
//		database_file = file_open(query, token, "r", TRUE);
//	}
//	output_msg(OUTPUT_SCREEN, "Database file: %s\n\n", token);
//	output_msg(OUTPUT_SEND_MESSAGE, "Database file: %s\r\n\r\n", token);
//	strcpy(db_file, token);
//
//	output_msg(OUTPUT_MESSAGE, "   Input file: %s\n", in_file);
//	output_msg(OUTPUT_MESSAGE, "  Output file: %s\n", out_file);
//	output_msg(OUTPUT_MESSAGE, "Database file: %s\n\n", token);
///*
// *   local cleanup
// */
//	user_database = (char *) free_check_null(user_database);
//	line = (char *) free_check_null(line);
//	line_save = (char *) free_check_null(line_save);
//
//	hdestroy_multi(keyword_hash_table);
//	keyword_hash = (struct key *) free_check_null(keyword_hash);
//	keyword_hash_table = NULL;
//
//	free_hash_strings(strings_hash_table);
//	hdestroy_multi(strings_hash_table);
//	strings_hash_table = NULL;
//
//	*db_cookie = database_file;
//	*input_cookie = input_file;
//
//	this->db_stream = new std::ifstream(db_file, std::ifstream::in);
//	this->in_stream = new std::ifstream(in_file, std::ifstream::in);
//	*db_cookie = db_stream;
//	*input_cookie = in_stream;
//
//	return 0;
//}

///* ---------------------------------------------------------------------- */
//int PHRQ_io::
//getc_callback(void *cookie)
///* ---------------------------------------------------------------------- */
//{
//	int i;
//	assert(cookie);
//	i = getc((FILE *) cookie);
//
//	return i;
//}

//istream_getc is static ***
int PHRQ_io::
istream_getc(void *cookie)
{
	if (cookie)
	{
		std::istream* is = (std::istream*)cookie;
		int n = is->get();
		if (n == 13 && is->peek() == 10)
		{
			n = is->get();
		}
		return n;
	}
	return EOF;
}

/* ---------------------------------------------------------------------- */
int PHRQ_io::
output_handler(const int type, const char *err_str, const bool stop,
			   const char *format, va_list args)
/* ---------------------------------------------------------------------- */
{
	int flush;
	FILE *save_output = NULL;

	flush = 1;

	//if (get_forward_output_to_log())
	//{
	//	save_output = output_file;
	//	output_file = log_file;
	//}

	switch (type)
	{

	case OUTPUT_ERROR:
//		if (status_on == TRUE)
//		{
//			if (error_file != NULL)
//			{
//				fprintf(error_file, "\n");
//			}
//#ifndef DOS
//			status_on = FALSE;
//#endif
//		}
		if (error_file != NULL)
		{
			fprintf(error_file, "ERROR: %s\n", err_str);
			if (flush)
				fflush(error_file);
		}
#ifdef PHREEQ98
		sprintf(progress_str, "ERROR: %s\n", err_str);
		show_progress(type, progress_str);
#endif /* PHREEQ98 */
		if (output_file != NULL)
		{
			fprintf(output_file, "ERROR: %s\n", err_str);
#ifdef PHREEQ98
			outputlinenr++;
#endif
			if (flush)
				fflush(output_file);
		}
		if (stop)
		{
			if (error_file != NULL)
			{
				fprintf(error_file, "Stopping.\n");
				fflush(error_file);
			}
#ifdef PHREEQ98
			sprintf(progress_str, "Stopping.\n");
			show_progress(type, progress_str);
#endif /* PHREEQ98 */
			if (output_file != NULL)
			{
				fprintf(output_file, "Stopping.\n");
#ifdef PHREEQ98
				outputlinenr++;
#endif
				fflush(output_file);
			}
//#if defined MULTICHART
//			chart_handler.End_timer(PHREEQC_THIS);
//#endif
		}
		break;

	case OUTPUT_WARNING:
		//if (pr.logfile == TRUE && log_file != NULL)
		if (log_file != NULL)
		{
			fprintf(log_file, "WARNING: %s\n", err_str);
			if (flush)
				fflush(log_file);
		}
		//if (state == TRANSPORT && transport_warnings == FALSE)
		//	return (OK);
		//if (state == ADVECTION && advection_warnings == FALSE)
		//	return (OK);
//		if (pr.warnings >= 0)
//		{
//			if (count_warnings > pr.warnings)
//				return (OK);
//		}
//		if (status_on == TRUE)
//		{
//			if (error_file != NULL)
//			{
//				fprintf(error_file, "\n");
//			}
//#ifndef DOS
//			status_on = FALSE;
//#endif
//		}
		if (error_file != NULL)
		{
			fprintf(error_file, "WARNING: %s\n", err_str);
			if (flush)
				fflush(error_file);
		}
#ifdef PHREEQ98
		sprintf(progress_str, "WARNING: %s\n", err_str);
		show_progress(type, progress_str);
#endif /* PHREEQ98 */
		if (output_file != NULL)
		{
			fprintf(output_file, "WARNING: %s\n", err_str);
#ifdef PHREEQ98
			outputlinenr++;
#endif
			if (flush)
				fflush(output_file);
		}
		break;
	case OUTPUT_CHECKLINE:
		//if (pr.echo_input == TRUE)
		{
			if (output_file != NULL)
			{
				vfprintf(output_file, format, args);
#ifdef PHREEQ98
				check_line_breaks(format);
#endif
				if (flush)
					fflush(output_file);
			}
		}
		break;
	case OUTPUT_MESSAGE:
	case OUTPUT_BASIC:
		if (output_file != NULL)
		{
			vfprintf(output_file, format, args);
#ifdef PHREEQ98
			check_line_breaks(format);
#endif
			if (flush)
				fflush(output_file);
		}
		break;
	case OUTPUT_PUNCH:
		if (punch_file != NULL)
		{
			//if (pr.punch == TRUE && punch.in == TRUE)
			{
				vfprintf(punch_file, format, args);
				if (flush)
					fflush(punch_file);
			}
		}
		break;
	case OUTPUT_LOG:
		//if (pr.logfile == TRUE && log_file != NULL)
		if (log_file != NULL)
		{
			vfprintf(log_file, format, args);
			if (flush)
				fflush(log_file);
		}
		break;
	case OUTPUT_SCREEN:
		if (error_file != NULL)
		{
			vfprintf(error_file, format, args);
			if (flush)
				fflush(error_file);
		}
#ifdef PHREEQ98
		vsprintf(progress_str, format, args);
		show_progress(type, progress_str);
#endif /* PHREEQ98 */
		break;
	case OUTPUT_STDERR:
	case OUTPUT_CVODE:
		if (stderr != NULL)
		{
			vfprintf(stderr, format, args);
			fflush(stderr);
		}
		break;
	case OUTPUT_DUMP:
		if (dump_file != NULL)
		{
			vfprintf(dump_file, format, args);
			if (flush)
				fflush(dump_file);
		}
		break;
	}

	//if (get_forward_output_to_log())
	//{
	//	output_file = save_output;
	//}
	//return (OK);
	return 1;
}

/* ---------------------------------------------------------------------- */
int PHRQ_io::
close_output_files(void)
/* ---------------------------------------------------------------------- */
{
	int ret = 0;

	if (output_file != NULL)
		ret |= fclose(output_file);
	if (log_file != NULL)
		ret |= fclose(log_file);
	if (punch_file != NULL)
		ret |= fclose(punch_file);
	if (dump_file != NULL)
		ret |= fclose(dump_file);
	if (error_file != NULL)
		ret |= fclose(error_file);
	error_file = NULL;
	output_file = log_file = punch_file = dump_file = NULL;
	return ret;
}

/* ---------------------------------------------------------------------- */
int PHRQ_io::
open_handler(const int type, const char *file_name)
/* ---------------------------------------------------------------------- */
{
	switch (type)
	{

	case OUTPUT_PUNCH:
		if (punch_file != NULL)
		{
			fclose(punch_file);
			punch_file = NULL;
		}
		if ((punch_file = fopen(file_name, "w")) == NULL)
		{
			//return ERROR;
			return 0;
		}
		else
		{
			//free_check_null(selected_output_file_name);
			//selected_output_file_name = string_duplicate(file_name);
		}
		break;

	case OUTPUT_DUMP:
		if (dump_file != NULL)
		{
			fclose(dump_file);
			dump_file = NULL;
		}
		if ((dump_file = fopen(file_name, "w")) == NULL)
		{
			//return ERROR;
			return 0;
		}
		else
		{
			//free_check_null(dump_file_name);
			//dump_file_name = string_duplicate(file_name);
		}
		break;
	case OUTPUT_ERROR:
		if (error_file != NULL && 
			error_file != stderr &&
			error_file != stdout)
		{
			fclose(error_file);
			error_file = NULL;
		}
		if (file_name != NULL)
		{
			if ((error_file = fopen(file_name, "w")) == NULL)
			{
				error_file = stderr;
				return 0;
			}
		}
		else
		{
			error_file = stderr;
		}
		break;
	case OUTPUT_LOG:
		if (log_file != NULL && 
			log_file != stderr &&
			log_file != stdout)
		{
			fclose(log_file);
			log_file = NULL;
		}
		if ((log_file = fopen(file_name, "w")) == NULL)
		{
			return 0;
		}
		break;
	}
	return 1;
}

/* ---------------------------------------------------------------------- */
int PHRQ_io::
fileop_handler(const int type, int (*PFN) (FILE *))
/* ---------------------------------------------------------------------- */
{
	FILE *save_output = NULL;

	//int forward = get_forward_output_to_log();
	//if (forward)
	//{
	//	save_output = output_file;
	//	output_file = log_file;
	//}

	switch (type)
	{
	case OUTPUT_ERROR:
		if (error_file)
			PFN(error_file);
		break;

	case OUTPUT_WARNING:
		if (error_file)
			PFN(error_file);
		if (output_file)
			PFN(output_file);
		break;

	case OUTPUT_MESSAGE:
	case OUTPUT_CHECKLINE:
	case OUTPUT_BASIC:
		if (output_file)
			PFN(output_file);
		break;

	case OUTPUT_PUNCH:
		if (punch_file)
			PFN(punch_file);
		break;

	case OUTPUT_SCREEN:
		if (error_file)
			PFN(error_file);
		break;

	case OUTPUT_LOG:
		if (log_file)
			PFN(log_file);
		break;

	case OUTPUT_CVODE:
	case OUTPUT_STDERR:
		if (stderr)
			PFN(stderr);
		break;

	case OUTPUT_DUMP:
		if (dump_file)
			PFN(dump_file);
		break;
	}

	//if (forward)
	//{
	//	output_file = save_output;
	//}
	//return (OK);
	return 1;
}

/* ---------------------------------------------------------------------- */
int PHRQ_io::
rewind_wrapper(FILE * file_ptr)
/* ---------------------------------------------------------------------- */
{
	rewind(file_ptr);
	//return (OK);
	return 1;
}
//#if defined(MERGE_INCLUDE_FILES)
///* ---------------------------------------------------------------------- */
//bool PHRQ_io::
//recursive_include(std::ifstream & input_stream, std::iostream & accumulated_stream)
///* ---------------------------------------------------------------------- */
//{
//	std::string myline;
//
//	// input_file should be opened before calling recursive_include
//	assert(input_stream.is_open());
//
//	while (std::getline(input_stream, myline))
//	{
//		accumulated_stream << myline << std::endl;
//
//		std::string copy_line = myline;
//		// remove leading spaces
//		copy_line.erase(0, copy_line.find_first_not_of(" \t") );
//		// get first 8 characters
//		std::string token = copy_line.substr(0,9);
//		// convert to lower case
//		std::transform(token.begin(), token.end(), token.begin(),
//               (int(*)(int)) std::tolower);
//		// check for #include file_name
//		if (token == "#$include")
//		{
//			// get file name without trailing spaces
//			token = copy_line.substr(10, copy_line.length());
//			token.erase(0, token.find_first_not_of(" \t"));
//			// remove leading, trailing spaces
//			token = token.substr(0, token.find_last_not_of(" \t\n\0") + 1);
//			// open stream
//			std::ifstream next_stream(token.c_str(), std::ifstream::in);
//			if (next_stream.is_open())
//			{
//				recursive_include(next_stream, accumulated_stream);
//				accumulated_stream << "#Done include " << token << std::endl;
//			}
//			else
//			{
//				// error opening file
//				sprintf(error_string, "Could not open include file %s", token.c_str());
//				error_msg(error_string, STOP);
//			}
//		}
//	}
//	return true;
//}
//#endif /* defined(MERGE_INCLUDE_FILES) */

