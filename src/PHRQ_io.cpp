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
////#if defined MULTICHART
////			chart_handler.End_timer(PHREEQC_THIS);
////#endif
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
void PHRQ_io::
output_string(const int type, std::string str)
/* ---------------------------------------------------------------------- */
{

	switch (type)
	{

	case OUTPUT_ERROR:
		if (error_file != NULL && error_file_on)
		{
			fprintf(error_file, "%s", str.c_str());
		}
		fflush(error_file);
		break;

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
	case OUTPUT_BASIC:
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
	case OUTPUT_STDERR:
	case OUTPUT_CVODE:
		if (stderr != NULL)
		{
			fprintf(stderr, "%s", str.c_str());
			fflush(stderr);
		}
		break;
	case OUTPUT_DUMP:
		if (dump_file != NULL && dump_file_on)
		{
			fprintf(dump_file, "%s", str.c_str());
		}
		break;
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
/* ---------------------------------------------------------------------- */
void PHRQ_io::
safe_close(FILE * file_ptr)
/* ---------------------------------------------------------------------- */
{
	if (file_ptr != stderr &&
		file_ptr != stdout &&
		file_ptr != stdin)
	{
		fclose(file_ptr);
	}
}

