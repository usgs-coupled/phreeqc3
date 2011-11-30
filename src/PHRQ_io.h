#ifndef _PHRQIO_H
#define _PHRQIO_H

#include <sstream>
class PhreeqcStop : std::exception
{
};
class PHRQ_io
{
public:
	// constructor/destructor
	PHRQ_io(void);
	virtual ~ PHRQ_io();

	// methods
	static int istream_getc(void *cookie);
	static void safe_close(FILE ** file_ptr);

	int close_input_files(void);
	int close_output_files(void);

	// output_file
	bool output_open(const char *file_name);
	void output_fflush(void);
	void output_close(void);
	void output_rewind(void);
	bool output_isopen(void);
	void output_msg(const char * str);

	// log_file
	bool log_open(const char *file_name);
	void log_fflush(void);
	void log_close(void);
	void log_rewind(void);
	bool log_isopen(void);
	void log_msg(const char * str);

	// punch_file
	bool punch_open(const char *file_name);
	void punch_fflush(void);
	void punch_close(void);
	void punch_rewind(void);
	bool punch_isopen(void);
	void punch_msg(const char * str);

	// error_file
	bool error_open(const char *file_name);
	void error_fflush(void);
	void error_close(void);
	void error_rewind(void);
	bool error_isopen(void);
	void error_msg(const char * str, bool stop=false);
	void warning_msg(const char *err_str);

	// dump_file
	bool dump_open(const char *file_name);
	void dump_fflush(void);
	void dump_close(void);
	void dump_rewind(void);
	bool dump_isopen(void);
	void dump_msg(const char * str);

	
	void fpunchf(const char *name, const char *format, double d);
	void fpunchf(const char *name, const char *format, char * d);
	void fpunchf(const char *name, const char *format, int d);

	void Set_io_error_count(int i)  {this->io_error_count = i;};
	int Get_io_error_count(void)    {return this->io_error_count;};
	void Set_database_file(FILE * in);
	void Set_input_file(FILE * in);
	void Set_output_file(FILE * out);
	void Set_error_file(FILE * out);
	void Set_log_file(FILE * out);
	void Set_punch_file(FILE * out);
	void Set_dump_file(FILE * out);

	// close input file
	void close_input(void);

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

	void screen_msg(const char * str);
	void Set_screen_on(bool tf)   {this->screen_on = tf;};
	bool Get_screen_on(void)      {return this->screen_on;};

	enum ECHO_OPTION
	{
		ECHO_LOG,
		ECHO_OUTPUT
	};
	void echo_msg(const char * str);
	void Set_echo_on(bool tf)   {this->echo_on = tf;};
	bool Get_echo_on(void)      {return this->echo_on;};
	void Set_echo_destination(ECHO_OPTION eo)   {this->echo_destination = eo;};
	ECHO_OPTION Get_echo_destination(void)      {return this->echo_destination;};


	// data
protected:
	FILE *input_file;
	FILE *database_file;
	FILE *output_file;	
	FILE *log_file;		
	FILE *punch_file;	
	FILE *error_file;	
	FILE *dump_file;	
	int io_error_count;

	bool output_file_on;
	bool log_file_on;
	bool punch_file_on;
	bool error_file_on;
	bool dump_file_on;
	bool echo_on;
	bool screen_on;
	ECHO_OPTION echo_destination;

};

#endif /* _PHRQIO_H */
