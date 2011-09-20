#ifndef _PHRQIO_H
#define _PHRQIO_H

#include <sstream>

class PHRQ_io
{
public:
typedef enum
{
	OUTPUT_ERROR,
	OUTPUT_WARNING,
	OUTPUT_MESSAGE,
	OUTPUT_PUNCH,
	OUTPUT_SCREEN,
	OUTPUT_LOG,
	OUTPUT_CHECKLINE,
	OUTPUT_GUI_ERROR,
	OUTPUT_BASIC,
	OUTPUT_CVODE,
	OUTPUT_DUMP,
	OUTPUT_STDERR,
	OUTPUT_SEND_MESSAGE,
	OUTPUT_ECHO,
	OUTPUT_PUNCH_END_ROW
} output_type;

typedef enum
{
	ACTION_OPEN,
	ACTION_OUTPUT,
	ACTION_CLOSE,
	ACTION_REWIND,
	ACTION_FLUSH
} action_type;

	// constructors
	PHRQ_io(void);
	virtual ~ PHRQ_io();

	// methods
	
	int phreeqc_handler(const int action, const int type, const char *err_str, const bool stop, const char *, va_list args);
	int open_handler(const int type, const char *file_name);
	
	int fileop_handler(const int type, int (*PFN) (FILE *));
	int output_handler(const int type, const char *err_str,
						  const bool stop, const char *format,
						  va_list args);
	static int rewind_wrapper(FILE * file_ptr);	
	static void safe_close(FILE * file_ptr);	
	int close_input_files(void);
	int close_output_files(void);
	static int istream_getc(void *cookie);
	void output_string(const int type, std::string str);
	void Set_error_count(int i)
	{
		this->error_count = i;
	}
	int Get_error_count(void)
	{
		return this->error_count;
	}
	void Set_input_file(FILE * in)
	{
		if (this->input_file != NULL &&
			this->input_file != stderr &&
			this->input_file != stdout)
		{
			fclose(this->input_file);
		}
		this->input_file = in;
	}
	void Set_output_file(FILE * out)
	{
		if (this->output_file != NULL &&
			this->output_file != stderr &&
			this->output_file != stdout)
		{
			fclose(this->output_file);
		}
		this->output_file = out;
	}
	void Set_database_file(FILE * in)
	{
		if (this->database_file != NULL &&
			this->database_file != stdin)
		{
			fclose(this->database_file);
		}
		this->database_file = in;
	}
	void close_input(void)
	{
		if (input_file != stdin)
		{
			fclose(input_file);
		}
	}
	void Set_output_file_on(bool tf)
	{
		this->output_file_on = tf;
	}
	void Set_log_file_on(bool tf)
	{
		this->log_file_on = tf;
	}
	void Set_punch_file_on(bool tf)
	{
		this->punch_file_on = tf;
	}
	void Set_error_file_on(bool tf)
	{
		this->error_file_on = tf;
	}
	void Set_dump_file_on(bool tf)
	{
		this->dump_file_on = tf;
	}
	bool Get_output_file_on(void)
	{
		return this->output_file_on;
	}
	bool Get_log_file_on(void)
	{
		return this->log_file_on;
	}
	bool Get_punch_file_on(void)
	{
		return this->punch_file_on;
	}
	bool Get_error_file_on(void)
	{
		return this->error_file_on;
	}
	bool Get_dump_file_on(void)
	{
		return this->dump_file_on;
	}
	// data
private:
	FILE *input_file;
	FILE *database_file;
	FILE *output_file;	/* OUTPUT_MESSAGE */
	FILE *log_file;		/* OUTPUT_LOG */
	FILE *punch_file;	/* OUTPUT_PUNCH */
	FILE *error_file;	/* OUTPUT_ERROR */
	FILE *dump_file;	/* OUTPUT_DUMP */
	int error_count;

	bool output_file_on;
	bool log_file_on;
	bool punch_file_on;
	bool error_file_on;
	bool dump_file_on;
};

#endif /* _PHRQIO_H */
