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
	error_file = NULL;	
	dump_file = NULL;	
	io_error_count = 0;

	output_file_on = true;
	log_file_on = false;
	punch_file_on = true;
	error_file_on = true;
	dump_file_on = true;
}

PHRQ_io::
~PHRQ_io()
{
}
void 
PHRQ_io::Set_database_file(FILE * in)
{
	safe_close(this->database_file);
	this->database_file = in;
}
void 
PHRQ_io::Set_input_file(FILE * in)
{
	safe_close(this->input_file);
	this->input_file = in;
}
void 
PHRQ_io::Set_output_file(FILE * out)
{
	safe_close(this->output_file);
	this->output_file = out;
}
void 
PHRQ_io::Set_error_file(FILE * out)
{
	safe_close(this->error_file);
	this->error_file = out;
}
void 
PHRQ_io::Set_log_file(FILE * out)
{
	safe_close(this->log_file);
	this->log_file = out;
}
void 
PHRQ_io::Set_punch_file(FILE * out)
{
	safe_close(this->punch_file);
	this->punch_file = out;
}
void 
PHRQ_io::Set_dump_file(FILE * out)
{
	safe_close(this->dump_file);
	this->dump_file = out;
}

// ---------------------------------------------------------------------- */
// output file methods
// ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
bool PHRQ_io::
output_open_temp(const char *file_name)
/* ---------------------------------------------------------------------- */
{
	safe_close(output_file);
	if ((output_file = fopen(file_name, "w")) == NULL)
	{
		return true; // error
	}
	return false;
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
output_fflush_temp(void)
/* ---------------------------------------------------------------------- */
{
	if (output_file && output_file_on)
	{
		fflush(output_file);
	}
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
output_close_temp(void)
/* ---------------------------------------------------------------------- */
{
	safe_close(output_file);
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
output_rewind_temp(void)
/* ---------------------------------------------------------------------- */
{
	if (output_file && output_file_on)
	{
		rewind(output_file);
	}
}
/* ---------------------------------------------------------------------- */
bool PHRQ_io::
output_isopen_temp(void)
/* ---------------------------------------------------------------------- */
{
	if (output_file)
		return true; // open
	return false;
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
output_msg_temp(const char * str)
/* ---------------------------------------------------------------------- */
{
	if (output_file != NULL && output_file_on)
	{
		fprintf(output_file, "%s", str);
	}
}
// ---------------------------------------------------------------------- */
// log file methods
// ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
bool PHRQ_io::
log_open(const char *file_name)
/* ---------------------------------------------------------------------- */
{
	safe_close(log_file);
	if ((log_file = fopen(file_name, "w")) == NULL)
	{
		return true; // error
	}
	return false;
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
log_fflush(void)
/* ---------------------------------------------------------------------- */
{
	if (log_file && log_file_on)
	{
		fflush(log_file);
	}
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
log_close(void)
/* ---------------------------------------------------------------------- */
{
	safe_close(log_file);
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
log_rewind(void)
/* ---------------------------------------------------------------------- */
{
	if (log_file && log_file_on)
	{
		rewind(log_file);
	}
}
/* ---------------------------------------------------------------------- */
bool PHRQ_io::
log_isopen(void)
/* ---------------------------------------------------------------------- */
{
	if (log_file)
		return true; // open
	return false;
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
log_msg(const char * str)
/* ---------------------------------------------------------------------- */
{
	if (log_file != NULL && log_file_on)
	{
		fprintf(log_file, "%s", str);
	}
}
// ---------------------------------------------------------------------- */
// punch file methods
// ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
bool PHRQ_io::
punch_open(const char *file_name)
/* ---------------------------------------------------------------------- */
{
	safe_close(punch_file);
	if ((punch_file = fopen(file_name, "w")) == NULL)
	{
		return true; // error
	}
	return false;
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
punch_fflush(void)
/* ---------------------------------------------------------------------- */
{
	if (punch_file && punch_file_on)
	{
		fflush(punch_file);
	}
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
punch_close(void)
/* ---------------------------------------------------------------------- */
{
	safe_close(punch_file);
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
punch_rewind(void)
/* ---------------------------------------------------------------------- */
{
	if (punch_file && punch_file_on)
	{
		rewind(punch_file);
	}
}
/* ---------------------------------------------------------------------- */
bool PHRQ_io::
punch_isopen(void)
/* ---------------------------------------------------------------------- */
{
	if (punch_file)
		return true; // open
	return false;
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
punch_msg(const char * str)
/* ---------------------------------------------------------------------- */
{
	if (punch_file != NULL && punch_file_on)
	{
		fprintf(punch_file, "%s", str);
	}
}

// ---------------------------------------------------------------------- */
// error file methods
// ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
bool PHRQ_io::
error_open(const char *file_name)
/* ---------------------------------------------------------------------- */
{
	safe_close(error_file);

	if (file_name != NULL)
	{
		if ((error_file = fopen(file_name, "w")) == NULL)
		{
			error_file = stderr;
			return false;
		}
	}
	else
	{
		error_file = stderr;
	}
	return true;
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
error_fflush(void)
/* ---------------------------------------------------------------------- */
{
	if (error_file && error_file_on)
	{
		fflush(error_file);
	}
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
error_close(void)
/* ---------------------------------------------------------------------- */
{
	safe_close(error_file);
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
error_rewind(void)
/* ---------------------------------------------------------------------- */
{
	if (error_file && error_file_on)
	{
		rewind(error_file);
	}
}
/* ---------------------------------------------------------------------- */
bool PHRQ_io::
error_isopen(void)
/* ---------------------------------------------------------------------- */
{
	if (error_file)
		return true; // open
	return false;
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
error_msg(const char *err_str, bool stop)
/* ---------------------------------------------------------------------- */
{

	io_error_count++;
	if (error_file != NULL && error_file_on)
	{
		fprintf(error_file, "\nERROR: %s\n", err_str);
		fflush(error_file);
	}
	if (output_file != NULL && output_file_on)
	{
		fprintf(output_file, "ERROR: %s\n", err_str);
		fflush(output_file);
	}
	if (log_file != NULL && log_file_on)
	{
		fprintf(log_file, "ERROR: %s\n", err_str);
		fflush(log_file);
	}
	if (stop)
	{
		if (error_file != NULL && error_file_on)
		{
			fprintf(error_file, "Stopping.\n");
			fflush(error_file);
		}
		if (output_file != NULL && output_file_on)
		{
			fprintf(output_file, "Stopping.\n");
			fflush(output_file);
		}
		if (log_file != NULL && log_file_on)
		{
			fprintf(log_file, "Stopping.\n");
			fflush(log_file);
		}
	}
}

// ---------------------------------------------------------------------- */
// dump file methods
// ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
bool PHRQ_io::
dump_open(const char *file_name)
/* ---------------------------------------------------------------------- */
{
	safe_close(dump_file);
	if ((dump_file = fopen(file_name, "w")) == NULL)
	{
		return true; // error
	}
	return false;
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
dump_fflush(void)
/* ---------------------------------------------------------------------- */
{
	if (dump_file && dump_file_on)
	{
		fflush(dump_file);
	}
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
dump_close(void)
/* ---------------------------------------------------------------------- */
{
	safe_close(dump_file);
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
dump_rewind(void)
/* ---------------------------------------------------------------------- */
{
	if (dump_file && dump_file_on)
	{
		rewind(dump_file);
	}
}
/* ---------------------------------------------------------------------- */
bool PHRQ_io::
dump_isopen(void)
/* ---------------------------------------------------------------------- */
{
	if (dump_file)
		return true; // open
	return false;
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
dump_msg(const char * str)
/* ---------------------------------------------------------------------- */
{
	if (dump_file != NULL && dump_file_on)
	{
		fprintf(dump_file, "%s", str);
	}
}



void 
PHRQ_io::close_input(void)
{
	safe_close(input_file);
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
void PHRQ_io::
warning_msg(const char *err_str)
/* ---------------------------------------------------------------------- */
{
	if (error_file != NULL && error_file_on)
	{
		fprintf(error_file, "\nWARNING: %s\n", err_str);
		fflush(error_file);
	}	
	if (log_file != NULL && log_file_on)
	{
		fprintf(log_file, "WARNING: %s\n", err_str);
		fflush(log_file);
	}
	if (output_file != NULL && output_file_on)
	{
		fprintf(output_file, "WARNING: %s\n", err_str);
		fflush(output_file);
	}
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
output_msg(int type, const char *format, va_list args)
/* ---------------------------------------------------------------------- */
{
	int flush = 1;

	switch (type)
	{

	//case OUTPUT_ERROR:
	//	io_error_count++;
	//	if (error_file != NULL && error_file_on)
	//	{
	//		fprintf(error_file, "\nERROR: ");
	//		vfprintf(error_file, format, args);
	//		fflush(error_file);
	//	}
	//	if (output_file != NULL && output_file_on)
	//	{
	//		fprintf(output_file, "\nERROR: ");
	//		vfprintf(output_file, format, args);
	//		fflush(output_file);
	//	}
	//	if (log_file != NULL && log_file_on)
	//	{
	//		fprintf(log_file, "\nERROR: ");
	//		vfprintf(log_file, format, args);
	//		fflush(log_file);
	//	}
	//	break;
	case OUTPUT_WARNING:
		if (error_file != NULL && error_file_on)
		{
			fprintf(error_file, "\nWARNING: ");
			vfprintf(error_file, format, args);
			fflush(error_file);
		}
		if (output_file != NULL && output_file_on)
		{
			fprintf(output_file, "\nWARNING: ");
			vfprintf(output_file, format, args);
			fflush(output_file);
		}
		if (log_file != NULL && log_file_on)
		{
			fprintf(log_file, "\nWARNING: ");
			vfprintf(log_file, format, args);
			fflush(log_file);
		}
		break;
	case OUTPUT_CHECKLINE:
		{
			if (output_file != NULL && output_file_on)
			{
				vfprintf(output_file, format, args);
				if (flush)
					fflush(output_file);
			}
		}
		break;
	case OUTPUT_MESSAGE:
		if (output_file != NULL && output_file_on)
		{
			vfprintf(output_file, format, args);
			if (flush)
				fflush(output_file);
		}
		break;
	case OUTPUT_PUNCH:
		if (punch_file != NULL && punch_file_on)
		{
			{
				vfprintf(punch_file, format, args);
				if (flush)
					fflush(punch_file);
			}
		}
		break;
	case OUTPUT_LOG:
		if (log_file != NULL && log_file_on)
		{
			vfprintf(log_file, format, args);
			if (flush)
				fflush(log_file);
		}
		break;
	case OUTPUT_SCREEN:
		if (error_file != NULL && error_file_on)
		{
			vfprintf(error_file, format, args);
			if (flush)
				fflush(error_file);
		}
		break;

	//case OUTPUT_DUMP:
	//	if (dump_file != NULL && dump_file_on)
	//	{
	//		vfprintf(dump_file, format, args);
	//		if (flush)
	//			fflush(dump_file);
	//	}
	//	break;
	default:
		assert(false);
		break;
	}
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
fpunchf(const char *name, const char *format, va_list args)
/* ---------------------------------------------------------------------- */
{
	int flush = 1;

	if (punch_file != NULL && punch_file_on)
	{
		{
			vfprintf(punch_file, format, args);
			if (flush)
				fflush(punch_file);
		}
	}
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
output_string(const int type, std::string str)
/* ---------------------------------------------------------------------- */
{

	switch (type)
	{

	//case OUTPUT_ERROR:
	//	//this->io_error_count++;
	//	if (error_file != NULL && error_file_on)
	//	{
	//		fprintf(error_file, "%s", str.c_str());
	//	}
	//	fflush(error_file);
	//	break;

	case OUTPUT_WARNING:
		if (log_file != NULL && log_file_on)
		{
			fprintf(log_file, "%s", str.c_str());
		}
		if (error_file != NULL && error_file_on)
		{
			fprintf(error_file, "%s", str.c_str());
			fflush(error_file);
		}
		if (output_file != NULL && output_file_on)
		{
			fprintf(output_file, "%s", str.c_str());
		}
		break;
	case OUTPUT_CHECKLINE:
	case OUTPUT_MESSAGE:
		if (output_file != NULL && output_file_on)
		{
			fprintf(output_file, "%s", str.c_str());
		}
		break;
	case OUTPUT_PUNCH:
		if (punch_file != NULL && punch_file_on)
		{
			fprintf(punch_file, "%s", str.c_str());
		}
		break;
	case OUTPUT_LOG:
		if (log_file != NULL && log_file_on)
		{
			fprintf(log_file, "%s", str.c_str());
		}
		break;
	case OUTPUT_SCREEN:
		if (error_file != NULL && error_file_on)
		{
			fprintf(error_file, "%s", str.c_str());
			fflush(error_file);
		}
		break;

	//case OUTPUT_DUMP:
	//	if (dump_file != NULL && dump_file_on)
	//	{
	//		fprintf(dump_file, "%s", str.c_str());
	//	}
	//	break;
	default:
		assert(false);
	}
	return;
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
output_open(int type, const char *file_name)
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
		break;
	case OUTPUT_MESSAGE:
		if (output_file != NULL)
		{
			safe_close(output_file);
			output_file = NULL;
		}
		if ((output_file = fopen(file_name, "w")) == NULL)
		{
			//return ERROR;
			return 0;
		}
		break;
	//case OUTPUT_DUMP:
	//	if (dump_file != NULL)
	//	{
	//		safe_close(dump_file);
	//		dump_file = NULL;
	//	}
	//	if ((dump_file = fopen(file_name, "w")) == NULL)
	//	{
	//		//return ERROR;
	//		return 0;
	//	}
	//	break;
	//case OUTPUT_ERROR:
	//	if (error_file != NULL)
	//	{
	//		safe_close(error_file);
	//		error_file = NULL;
	//	}

	//	if (file_name != NULL)
	//	{
	//		if ((error_file = fopen(file_name, "w")) == NULL)
	//		{
	//			error_file = stderr;
	//			return 0;
	//		}
	//	}
	//	else
	//	{
	//		error_file = stderr;
	//	}
	//	break;
	case OUTPUT_LOG:
		if (log_file != NULL)
		{
			safe_close(log_file);
			log_file = NULL;
		}
		if ((log_file = fopen(file_name, "w")) == NULL)
		{
			return 0;
		}
		break;
	default:
		assert(false);
	}
	return 1;
}

/* ---------------------------------------------------------------------- */
bool PHRQ_io::
output_isopen(const int type)
/* ---------------------------------------------------------------------- */
{
	switch (type)
	{
	//case OUTPUT_ERROR:
	//	return (error_file != NULL);
	//	break;
	case OUTPUT_WARNING:
		return (error_file != NULL || output_file != NULL);
		break;
	case OUTPUT_MESSAGE:
		return (output_file != NULL);
		break;
	case OUTPUT_PUNCH:
		return (punch_file != NULL);
		break;
	case OUTPUT_SCREEN:
		return (error_file != NULL);
		break;
	case OUTPUT_LOG:
		return (log_file != NULL);
		break;
	//case OUTPUT_DUMP:
	//	return (dump_file != NULL);
	//	break;
	default:
		assert(false);
		return (output_file != NULL);
		break;
	}
	return false;
}

/* ---------------------------------------------------------------------- */
void PHRQ_io::
output_fflush(const int type)
/* ---------------------------------------------------------------------- */
{
	switch (type)
	{
	//case OUTPUT_ERROR:
	//	if (error_file)
	//		fflush(error_file);
	//	break;

	case OUTPUT_WARNING:
		if (error_file)
			fflush(error_file);
		if (output_file)
			fflush(output_file);
		break;

	case OUTPUT_MESSAGE:
	case OUTPUT_CHECKLINE:
		if (output_file)
			fflush(output_file);
		break;

	case OUTPUT_PUNCH:
		if (punch_file)
			fflush(punch_file);
		break;

	case OUTPUT_SCREEN:
		if (error_file)
			fflush(error_file);
		break;

	case OUTPUT_LOG:
		if (log_file)
			fflush(log_file);
		break;

	//case OUTPUT_DUMP:
	//	if (dump_file)
	//		fflush(dump_file);
	//	break;
	default:
		assert(false);
		break;
	}
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
output_rewind(const int type)
/* ---------------------------------------------------------------------- */
{
	switch (type)
	{
	//case OUTPUT_ERROR:
	//	if (error_file)
	//		rewind(error_file);
	//	break;

	case OUTPUT_WARNING:
		if (error_file)
			rewind(error_file);
		if (output_file)
			rewind(output_file);
		break;

	case OUTPUT_MESSAGE:
	case OUTPUT_CHECKLINE:
		if (output_file)
			rewind(output_file);
		break;

	case OUTPUT_PUNCH:
		if (punch_file)
			rewind(punch_file);
		break;

	case OUTPUT_SCREEN:
		if (error_file)
			rewind(error_file);
		break;

	case OUTPUT_LOG:
		if (log_file)
			rewind(log_file);
		break;

	//case OUTPUT_DUMP:
	//	if (dump_file)
	//		rewind(dump_file);
	//	break;
	default:
		assert(false);
		break;
	}
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
output_close(const int type)
/* ---------------------------------------------------------------------- */
{
	switch (type)
	{
	//case OUTPUT_ERROR:
	//	safe_close(error_file);
	//	break;

	case OUTPUT_WARNING:
		safe_close(error_file);
		safe_close(output_file);
		break;

	case OUTPUT_MESSAGE:
	case OUTPUT_CHECKLINE:
		safe_close(output_file);
		break;

	case OUTPUT_PUNCH:
		safe_close(punch_file);
		break;

	case OUTPUT_SCREEN:
		safe_close(error_file);
		break;

	case OUTPUT_LOG:
		safe_close(log_file);
		break;

	//case OUTPUT_DUMP:
	//	safe_close(dump_file);
	//	break;

	default:
		assert(false);
		break;
	}
}

//safe_close is static method
/* ---------------------------------------------------------------------- */
void PHRQ_io::
safe_close(FILE * file_ptr)
/* ---------------------------------------------------------------------- */
{
	if (file_ptr != stderr &&
		file_ptr != stdout &&
		file_ptr != stdin &&
		file_ptr != NULL)
	{
		fclose(file_ptr);
		file_ptr = NULL;
	}
}

