// Pressure.cxx: implementation of the cxxPressure class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include <cassert>				// assert
#include <algorithm>			// std::sort

#include "Utils.h"				// define first
#include "Parser.h"
#include "Phreeqc.h"
#include "Pressure.h"
#include "phqalloc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxPressure::cxxPressure(PHRQ_io *io)
	//
	// default constructor for cxxPressure 
	//
:	cxxNumKeyword(io)
{
	count = 0;
	equalIncrements = false;
	pressures.push_back(1.0);
}

cxxPressure::~cxxPressure()
{
}
void
cxxPressure::read(CParser & parser)
{
	double d;
	CParser::TOKEN_TYPE k;
	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(5);
		vopts.push_back("pressures");	        //0
		vopts.push_back("equal_increments");	//1
		vopts.push_back("count");	            //2
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;
	bool useLastLine(false);

	// Read reaction_pressure number and description
	this->read_number_description(parser);

	opt_save = CParser::OPT_ERROR;
	bool equalIncrements_defined(false);
	bool count_defined(false);

	for (;;)
	{
		int opt;
		if (useLastLine == false)
		{
			opt = parser.get_option(vopts, next_char);
		}
		else
		{
			opt = parser.getOptionFromLastLine(vopts, next_char);
		}
		if (opt == CParser::OPT_DEFAULT)
		{
			opt = opt_save;
		}
		switch (opt)
		{
		case CParser::OPT_EOF:
			break;
		case CParser::OPT_KEYWORD:
			break;
		case CParser::OPT_DEFAULT:
		case CParser::OPT_ERROR:
			opt = CParser::OPT_EOF;
			parser.error_msg("Unknown input in REACTION_PRESSURE_RAW keyword.",
							 CParser::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
			useLastLine = false;
			break;

		case 0:				// pressures
			while ((k =
					parser.copy_token(token, next_char)) == CParser::TT_DIGIT)
			{
				std::istringstream iss(token);
				if (!(iss >> d))
				{
					parser.incr_input_error();
					parser.error_msg("Expected numeric value for pressures.",
									 CParser::OT_CONTINUE);
				}
				else
				{
					this->pressures.push_back(d);
				}
			}
			opt_save = 0;
			useLastLine = false;
			break;

		case 1:				// equal_increments
			if (!(parser.get_iss() >> this->equalIncrements))
			{
				this->equalIncrements = 0;
				parser.incr_input_error();
				parser.error_msg("Expected boolean value for equalIncrements.", CParser::OT_CONTINUE);
			}
			opt_save = CParser::OPT_DEFAULT;
			useLastLine = false;
			equalIncrements_defined = true;
			break;

		case 2:				// count
			if (!(parser.get_iss() >> this->count))
			{
				this->count = 0;
				parser.incr_input_error();
				parser.error_msg("Expected integer value for count.", CParser::OT_CONTINUE);
			}
			opt_save = CParser::OPT_DEFAULT;
			useLastLine = false;
			count_defined = true;
			break;
		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	// members that must be defined
	if (equalIncrements_defined == false)
	{
		parser.incr_input_error();
		parser.error_msg("Equal_increments not defined for REACTION_PRESSURE_RAW input.", 
			CParser::OT_CONTINUE);
	}
	if (count_defined == false)
	{
		parser.incr_input_error();
		parser.error_msg("Count_temps not defined for REACTION_PRESSURE_RAW input.",
			 CParser::OT_CONTINUE);
	}
}

void
cxxPressure::dump_raw(std::ostream & s_oss, unsigned int indent, int *n_out) const
{
	//const char    ERR_MESSAGE[] = "Packing temperature message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	s_oss << indent0;
	int n_user_local = (n_out != NULL) ? *n_out : this->n_user;
	s_oss << "REACTION_PRESSURE_RAW        " << n_user_local << " " << this->description << std::endl;

	s_oss << indent1;
	s_oss << "-count              " << this->count << std::endl;

	s_oss << indent1;
	s_oss << "-equal_increments   " << this->equalIncrements << std::endl;

	// Temperature element and attributes

	s_oss << indent1;
	s_oss << "-pressures          " << std::endl;
	{
		int i = 0;
		s_oss << indent2;
		for (std::vector < double >::const_iterator it = this->pressures.begin();
			 it != this->pressures.end(); it++)
		{
			if (i++ == 5)
			{
				s_oss << std::endl;
				s_oss << indent2;
				i = 0;
			}
			s_oss << *it << " ";
		}
		s_oss << std::endl;
	}
}

void
cxxPressure::read_raw(CParser & parser)
{
	double d;
	CParser::TOKEN_TYPE k;
	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(5);
		vopts.push_back("pressures");	        //0
		vopts.push_back("equal_increments");	//1
		vopts.push_back("count");	            //2
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;
	bool useLastLine(false);

	// Read reaction_pressure number and description
	this->read_number_description(parser);

	opt_save = CParser::OPT_ERROR;
	bool equalIncrements_defined(false);
	bool count_defined(false);

	for (;;)
	{
		int opt;
		if (useLastLine == false)
		{
			opt = parser.get_option(vopts, next_char);
		}
		else
		{
			opt = parser.getOptionFromLastLine(vopts, next_char);
		}
		if (opt == CParser::OPT_DEFAULT)
		{
			opt = opt_save;
		}
		switch (opt)
		{
		case CParser::OPT_EOF:
			break;
		case CParser::OPT_KEYWORD:
			break;
		case CParser::OPT_DEFAULT:
		case CParser::OPT_ERROR:
			opt = CParser::OPT_EOF;
			parser.error_msg("Unknown input in REACTION_PRESSURE_RAW keyword.",
							 CParser::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
			useLastLine = false;
			break;

		case 0:				// pressures
			while ((k =
					parser.copy_token(token, next_char)) == CParser::TT_DIGIT)
			{
				std::istringstream iss(token);
				if (!(iss >> d))
				{
					parser.incr_input_error();
					parser.error_msg("Expected numeric value for pressures.",
									 CParser::OT_CONTINUE);
				}
				else
				{
					this->pressures.push_back(d);
				}
			}
			opt_save = 0;
			useLastLine = false;
			break;

		case 1:				// equal_increments
			if (!(parser.get_iss() >> this->equalIncrements))
			{
				this->equalIncrements = 0;
				parser.incr_input_error();
				parser.error_msg("Expected boolean value for equalIncrements.", CParser::OT_CONTINUE);
			}
			opt_save = CParser::OPT_DEFAULT;
			useLastLine = false;
			equalIncrements_defined = true;
			break;

		case 2:				// count
			if (!(parser.get_iss() >> this->count))
			{
				this->count = 0;
				parser.incr_input_error();
				parser.error_msg("Expected integer value for count.", CParser::OT_CONTINUE);
			}
			opt_save = CParser::OPT_DEFAULT;
			useLastLine = false;
			count_defined = true;
			break;
		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	// members that must be defined
	if (equalIncrements_defined == false)
	{
		parser.incr_input_error();
		parser.error_msg("Equal_increments not defined for REACTION_PRESSURE_RAW input.", 
			CParser::OT_CONTINUE);
	}
	if (count_defined == false)
	{
		parser.incr_input_error();
		parser.error_msg("Count_temps not defined for REACTION_PRESSURE_RAW input.",
			 CParser::OT_CONTINUE);
	}
}
#ifdef SKIP
void
cxxPressure::dump_xml(std::ostream & s_oss, unsigned int indent) const const
{
	//const char    ERR_MESSAGE[] = "Packing temperature message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// Temperature element and attributes
	s_oss << indent0;
	s_oss << "<temperature " << std::endl;

	s_oss << indent1;
	s_oss << "pitzer_temperature_gammas=\"" << this->
		pitzer_temperature_gammas << "\"" << std::endl;

	// components
	s_oss << indent1;
	s_oss << "<component " << std::endl;
	for (std::list < cxxPressureComp >::const_iterator it =
		 temperatureComps.begin(); it != temperatureComps.end(); ++it)
	{
		it->dump_xml(s_oss, indent + 2);
	}

	return;
}
#endif