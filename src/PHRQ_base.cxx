#include "PHRQ_base.h"
#include <iostream>
#include "PHRQ_io.h"
PHRQ_base::
PHRQ_base(void)
{
	this->io = NULL;
	base_error_count = 0;
}

PHRQ_base::
PHRQ_base(PHRQ_io * p_io)
{
	this->io = p_io;
	base_error_count = 0;
}

PHRQ_base::
~PHRQ_base()
{
}

void PHRQ_base::
error_msg(const std::string & stdstr, int stop)
{
	this->base_error_count++;
	if (this->io)
	{
		this->io->phreeqc_handler(PHRQ_io::ACTION_OUTPUT, PHRQ_io::OUTPUT_ERROR, stdstr.c_str(), stop!=0, "", NULL);
		//this->io->output_string(PHRQ_io::OUTPUT_ERROR, stdstr);
	}
	else
	{
		std::cerr << stdstr << std::endl;
		std::cout << stdstr << std::endl;
	}
	if (stop != 0)
	{
		throw "PHRQ_base error message";
	}
}

void PHRQ_base::
warning_msg(const std::string & stdstr)
{
	if (this->io)
	{
		this->io->phreeqc_handler(PHRQ_io::ACTION_OUTPUT, PHRQ_io::OUTPUT_WARNING, stdstr.c_str(), false, "", NULL);
		//this->io->output_string(PHRQ_io::OUTPUT_WARNING, stdstr);
	}
	else
	{
		std::cerr << stdstr << std::endl;
		std::cout << stdstr << std::endl;
	}
}

void PHRQ_base::
output_msg(const std::string & stdstr)
{
	bool stop = false;
	if (this->io)
	{
		//this->io->phreeqc_handler(PHRQ_io::ACTION_OUTPUT, PHRQ_io::OUTPUT_MESSAGE, stdstr.c_str(), stop!=0, "", NULL);
		this->io->output_string(PHRQ_io::OUTPUT_MESSAGE, stdstr);
	}
	else
	{
		std::cout << stdstr << std::endl;
	}
}