#ifndef _PHRQIO_H
#define _PHRQIO_H

#include <sstream>
class PhreeqcStop : std::exception
{
};
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
	OUTPUT_DUMP,
	//OUTPUT_SEND_MESSAGE,
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
	
	static void safe_close(FILE * file_ptr);	
	int close_input_files(void);
	int close_output_files(void);
	static int istream_getc(void *cookie);
	
	int output_open(int type, const char *file_name);
	void error_msg(const char *err_str, bool stop);
	void warning_msg(const char *err_str);
	void output_msg(int type, const char *format, va_list args);
	void output_string(const int type, std::string str);
	void output_fflush(int type);
	void output_rewind(int type);
	void output_close(int type);
	bool output_isopen(int type);
	void fpunchf(const char *name, const char *format, va_list args);

	void Set_io_error_count(int i)  {this->io_error_count = i;};
	int Get_io_error_count(void)    {return this->io_error_count;};
	void Set_database_file(FILE * in);
	void Set_input_file(FILE * in);
	void Set_output_file(FILE * out);
	void Set_error_file(FILE * out);
	void Set_log_file(FILE * out);
	void Set_punch_file(FILE * out);
	void Set_dump_file(FILE * out);

	void PHRQ_io::close_input(void);

	void Set_output_file_on(bool tf) {this->output_file_on = tf;};
	void Set_log_file_on(bool tf)    {this->log_file_on = tf;};
	void Set_punch_file_on(bool tf)  {this->punch_file_on = tf;};
	void Set_error_file_on(bool tf)  {this->error_file_on = tf;};
	void Set_dump_file_on(bool tf)   {this->dump_file_on = tf;};

	bool Get_output_file_on(void)    {return this->output_file_on;};
	bool Get_log_file_on(void)       {return this->log_file_on;};
	bool Get_punch_file_on(void)     {return this->punch_file_on;};
	bool Get_error_file_on(void)     {return this->error_file_on;};
	bool Get_dump_file_on(void)      {return this->dump_file_on;};

	// data
protected:
	FILE *input_file;
	FILE *database_file;
	FILE *output_file;	/* OUTPUT_MESSAGE */
	FILE *log_file;		/* OUTPUT_LOG */
	FILE *punch_file;	/* OUTPUT_PUNCH */
	FILE *error_file;	/* OUTPUT_ERROR */
	FILE *dump_file;	/* OUTPUT_DUMP */
	int io_error_count;

	bool output_file_on;
	bool log_file_on;
	bool punch_file_on;
	bool error_file_on;
	bool dump_file_on;
};

#endif /* _PHRQIO_H */
