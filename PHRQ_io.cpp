#include <assert.h>
#include "PHRQ_io.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>

PHRQ_io::
PHRQ_io(void)
{
	output_ostream = NULL;	
	log_ostream = NULL;	
	punch_ostream = NULL;	
	error_ostream = NULL;	
	dump_ostream = NULL;
	//screen_ostream = NULL;
	io_error_count = 0;

	output_on = true;
	log_on = false;
	punch_on = true;
	error_on = true;
	dump_on = true;
	screen_on = true;
	echo_on = true;
	echo_destination = ECHO_OUTPUT;
}

PHRQ_io::
~PHRQ_io()
{
}
// ---------------------------------------------------------------------- */
// output ostream methods
// ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
bool PHRQ_io::
output_open(const char *file_name, std::ios_base::openmode mode)
/* ---------------------------------------------------------------------- */
{
	if ((output_ostream = new std::ofstream(file_name, mode)) == NULL)
	{
		return false; // error
	}
	return true;
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
output_flush(void)
/* ---------------------------------------------------------------------- */
{
	if (output_ostream)
	{
		output_ostream->flush();
	}
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
output_close(void)
/* ---------------------------------------------------------------------- */
{
	safe_close(&log_ostream);
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
output_msg(const char * str)
/* ---------------------------------------------------------------------- */
{
	if (output_ostream != NULL && output_on)
	{
		(*output_ostream) << str;
	}
}
// ---------------------------------------------------------------------- */
// log ostream methods
// ---------------------------------------------------------------------- */
bool PHRQ_io::
log_open(const char *file_name, std::ios_base::openmode mode)
/* ---------------------------------------------------------------------- */
{
	if ((log_ostream = new std::ofstream(file_name, mode)) == NULL)
	{
		return false; // error
	}
	return true;
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
log_flush(void)
/* ---------------------------------------------------------------------- */
{
	if (log_ostream)
	{
		log_ostream->flush();
	}
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
log_close(void)
/* ---------------------------------------------------------------------- */
{
	safe_close(&log_ostream);
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
log_msg(const char * str)
/* ---------------------------------------------------------------------- */
{
	if (log_ostream != NULL && log_on)
	{
		(*log_ostream) << str;
	}
}
// ---------------------------------------------------------------------- */
// punch ostream methods
// ---------------------------------------------------------------------- */
bool PHRQ_io::
punch_open(const char *file_name, std::ios_base::openmode mode)
/* ---------------------------------------------------------------------- */
{
	if ((punch_ostream = new std::ofstream(file_name, mode)) == NULL)
	{
		return false; // error
	}
	return true;
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
punch_flush(void)
/* ---------------------------------------------------------------------- */
{
	if (punch_ostream)
	{
		punch_ostream->flush();
	}
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
punch_close(void)
/* ---------------------------------------------------------------------- */
{
	safe_close(&punch_ostream);
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
punch_msg(const char * str)
/* ---------------------------------------------------------------------- */
{
	if (punch_ostream != NULL && punch_on)
	{
		(*punch_ostream) << str;
	}
}
// ---------------------------------------------------------------------- */
// error file methods
// ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
bool PHRQ_io::
error_open(const char *file_name, std::ios_base::openmode mode)
/* ---------------------------------------------------------------------- */
{
	if (file_name != NULL)
	{
		if ((error_ostream = new std::ofstream(file_name, mode)) == NULL)
		{
			error_ostream = &std::cerr;
			return false;
		}
	}
	else
	{
		error_ostream = &std::cerr;
	}
	return true;
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
error_flush(void)
/* ---------------------------------------------------------------------- */
{
	if (error_ostream)
	{
		error_ostream->flush();
	}
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
error_close(void)
/* ---------------------------------------------------------------------- */
{
	safe_close(&error_ostream);
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
error_msg(const char *err_str, bool stop)
/* ---------------------------------------------------------------------- */
{

	io_error_count++;
	if (error_ostream != NULL && error_on)
	{
			(*error_ostream) << err_str;
			error_ostream->flush();
	}
	if (stop)
	{
		if (error_ostream != NULL && error_on)
		{
			(*error_ostream) << "Stopping.\n";
			error_ostream->flush();
		}
		output_msg("Stopping.\n");
		log_msg("Stopping.\n");
	}
}
// ---------------------------------------------------------------------- */
// dump ostream methods
// ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
bool PHRQ_io::
dump_open(const char *file_name, std::ios_base::openmode mode)
/* ---------------------------------------------------------------------- */
{
	if ((dump_ostream = new std::ofstream(file_name, mode)) == NULL)
	{
		return false; // error
	}
	return true;
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
dump_flush(void)
/* ---------------------------------------------------------------------- */
{
	if (dump_ostream)
	{
		dump_ostream->flush();
	}
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
dump_close(void)
/* ---------------------------------------------------------------------- */
{
	safe_close(&dump_ostream);
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
dump_msg(const char * str)
/* ---------------------------------------------------------------------- */
{
	if (dump_ostream != NULL && dump_on)
	{
		(*dump_ostream) << str;
	}
}
///* ---------------------------------------------------------------------- */
//void PHRQ_io::
//dump_rewind(void)
///* ---------------------------------------------------------------------- */
//{
//	if (dump_ostream && dump_on)
//	{
//		dump_ostream->seekp(0, std::ios_base::beg);
//	}
//}
///* ---------------------------------------------------------------------- */
//bool PHRQ_io::
//dump_isopen(void)
///* ---------------------------------------------------------------------- */
//{
//	if (dump_ostream)
//	{
//		if (std::ofstream *ofs = dynamic_cast<std::ofstream*>(dump_ostream))
//		{
//			return ofs->is_open();
//		}
//		return true;
//	}
//	return false;
//}
#ifdef SKIP
// ---------------------------------------------------------------------- */
// screen ostream methods
// ---------------------------------------------------------------------- */
/* ---------------------------------------------------------------------- */
bool PHRQ_io::
screen_open(const char *file_name, std::ios_base::openmode mode)
/* ---------------------------------------------------------------------- */
{
	if ((screen_ostream = new std::ofstream(file_name, mode)) == NULL)
	{
		return false; // error
	}
	return true;
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
screen_flush(void)
/* ---------------------------------------------------------------------- */
{
	if (screen_ostream)
	{
		screen_ostream->flush();
	}
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
screen_close(void)
/* ---------------------------------------------------------------------- */
{
	safe_close(&screen_ostream);
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
screen_msg(const char * str)
/* ---------------------------------------------------------------------- */
{
	if (screen_ostream != NULL && screen_on)
	{
		(*screen_ostream) << str;
	}
}
#endif
/* ---------------------------------------------------------------------- */
void PHRQ_io::
screen_msg(const char * str)
/* ---------------------------------------------------------------------- */
{
	if (error_ostream != NULL && screen_on)
	{
		(*error_ostream) << str;
	}
}

int PHRQ_io::
istream_getc(void *cookie)     //istream_getc is *** static ***
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
	std::ostringstream warn_str;
	warn_str << "WARNING: " << err_str << std::endl;

	//screen_msg("\n");
	if (error_ostream != NULL && error_on)
	{
		(*error_ostream) << "\nWARNING: " << err_str << std::endl;
		error_ostream->flush();
	}	
	log_msg(warn_str.str().c_str());
	log_flush();
	output_msg(warn_str.str().c_str());
	output_flush();
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
fpunchf(const char *name, const char *format, double d)
/* ---------------------------------------------------------------------- */
{
	if (punch_ostream != NULL && punch_on)
	{
		{
			char token[256];
			sprintf(token, format, d);
			(*punch_ostream) << token;
		}
	}
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
fpunchf(const char *name, const char *format, char * s)
/* ---------------------------------------------------------------------- */
{
	if (punch_ostream != NULL && punch_on)
	{
		{
			char token[256];
			sprintf(token, format, s);
			(*punch_ostream) << token;
		}
	}
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
fpunchf(const char *name, const char *format, int d)
/* ---------------------------------------------------------------------- */
{
	if (punch_ostream != NULL && punch_on)
	{
		{
			char token[256];
			sprintf(token, format, d);
			(*punch_ostream) << token;
		}
	}
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::fpunchf_end_row(const char *format)
/* ---------------------------------------------------------------------- */
{
	//NOOP for Phreeqc
}
/* ---------------------------------------------------------------------- */
void PHRQ_io::
close_ostreams(void)
/* ---------------------------------------------------------------------- */
{
	std::set<std::ostream *> streams;

	streams.insert(output_ostream);
	streams.insert(log_ostream);
	streams.insert(punch_ostream);
	streams.insert(error_ostream);
	streams.insert(dump_ostream);
	//streams.insert(screen_ostream);

	std::set<std::ostream *>::iterator it = streams.begin();
	for (; it != streams.end(); it++)
	{
		std::ostream * x = *it;
		safe_close(&x);
	}

	output_ostream = NULL;
	log_ostream = NULL;
	punch_ostream = NULL;
	error_ostream = NULL;
	dump_ostream = NULL;
	//screen_ostream = NULL;
}
//safe_close is static method
/* ---------------------------------------------------------------------- */
void PHRQ_io::
safe_close(std::ostream **stream_ptr)
/* ---------------------------------------------------------------------- */
{
	if (*stream_ptr != &std::cerr &&
		*stream_ptr != &std::cout &&
		*stream_ptr != NULL)
	{
		delete *stream_ptr;
		*stream_ptr = NULL;
	}
}

/* ---------------------------------------------------------------------- */
void PHRQ_io::
echo_msg(const char * str)
/* ---------------------------------------------------------------------- */
{
	if (echo_on)
	{
		switch (this->echo_destination)
		{
		case ECHO_LOG:
			log_msg(str);
			break;
		case ECHO_OUTPUT:
			output_msg(str);
			break;
		}
	}
}

std::istream * PHRQ_io::
get_istream()
{
	if (istream_list.size() > 0)
	{
		return istream_list.front();
	}
	else
	{
		return NULL;
	}
}
void PHRQ_io::
push_istream(std::istream * cookie, bool auto_delete)
{
	istream_list.push_front(cookie);
	delete_istream_list.push_front(auto_delete);
}
void PHRQ_io::
clear_istream(void)
{
	while (istream_list.size() > 0)
	{
		pop_istream();
	}
}
void PHRQ_io::
pop_istream()
{
	if (istream_list.size() > 0)
	{
		if (delete_istream_list.front())
		{
			delete istream_list.front();
		}
		istream_list.pop_front();
		delete_istream_list.pop_front();
	}
}