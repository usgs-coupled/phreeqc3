#include "PHRQ_base.h"
#include <iostream>

PHRQ_base::
PHRQ_base(void)
{
	this->io = NULL;
	error_count = 0;
}

PHRQ_base::
PHRQ_base(PHRQ_io * p_io)
{
	this->io = p_io;
	error_count = 0;
}

PHRQ_base::
~PHRQ_base()
{
}

void PHRQ_base::
error_msg(const std::string & stdstr)
{
	this->error_count++;
	if (this->io)
	{
		//this->io->phreeqc_handler(ACTION_OUTPUT, OUTPUT_ERROR, stdstr, stop, "", args);
	}
	else
	{
		std::cerr << stdstr << std::endl;
		std::cout << stdstr << std::endl;
	}
}

void PHRQ_base::
warning_msg(const std::string & stdstr)
{
	if (this->io)
	{
		//this->io->phreeqc_handler(ACTION_OUTPUT, OUTPUT_WARNING, stdstr, stop, "", args);
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
	if (this->io)
	{
		//this->io->phreeqc_handler(ACTION_OUTPUT, OUTPUT_MESSAGE, stdstr, stop, "", args);
	}
	else
	{
		std::cout << stdstr << std::endl;
	}
}