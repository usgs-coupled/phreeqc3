// Temperature.cxx: implementation of the cxxTemperature class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include <cassert>				// assert
#include <algorithm>			// std::sort

#include "Utils.h"				// define first
#include "Parser.h"
#if !defined(PHREEQC_CLASS)
#define EXTERNAL extern
#include "global.h"
#else
#include "Phreeqc.h"
#endif
#include "Temperature.h"
#include "phqalloc.h"
#include "phrqproto.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxTemperature::cxxTemperature(PHRQ_io *io)
	//
	// default constructor for cxxTemperature 
	//
:	cxxNumKeyword(io)
{
	countTemps = 0;
	equalIncrements = false;
}

cxxTemperature::cxxTemperature(struct temperature *temperature_ptr, PHRQ_io *io)
		//
		// constructor for cxxTemperature from struct temperature
		//
	:
cxxNumKeyword(io)
{
	int i;

	this->set_description(temperature_ptr->description);
	this->n_user = temperature_ptr->n_user;
	this->n_user_end = temperature_ptr->n_user_end;
	// temps
	if (temperature_ptr->count_t < 0)
	{
		for (i = 0; i < 2; i++)
		{
			this->temps.push_back(temperature_ptr->t[i]);
		}
		this->countTemps = -temperature_ptr->count_t;
		this->equalIncrements = true;
	}
	else
	{
		for (i = 0; i < temperature_ptr->count_t; i++)
		{
			this->temps.push_back(temperature_ptr->t[i]);
		}
		this->countTemps = temperature_ptr->count_t;
		this->equalIncrements = false;
	}
}

cxxTemperature::~cxxTemperature()
{
}

#ifdef MOVE_TO_STRUCTURES
struct temperature *
cxxTemperature::cxxTemperature2temperature(PHREEQC_PTR_ARG)
		//
		// Builds a temperature structure from instance of cxxTemperature 
		//
{
	struct temperature *temperature_ptr;
	temperature_ptr =
		(struct temperature *) P_INSTANCE_POINTER PHRQ_malloc(sizeof(struct temperature));
	if (temperature_ptr == NULL)
		P_INSTANCE_POINTER malloc_error();

	temperature_ptr->description = P_INSTANCE_POINTER string_duplicate (this->get_description().c_str());
	temperature_ptr->n_user = this->n_user;
	temperature_ptr->n_user_end = this->n_user_end;

	// temps
	temperature_ptr->t = NULL;
	if (this->temps.size() > 0)
	{
		temperature_ptr->t =
			(LDBLE *)
			P_INSTANCE_POINTER PHRQ_malloc((size_t) (this->temps.size() * sizeof(double)));
		if (temperature_ptr->t == NULL)
			P_INSTANCE_POINTER malloc_error();
		std::copy(this->temps.begin(), this->temps.end(), temperature_ptr->t);
	}
	if (this->equalIncrements)
	{
		temperature_ptr->count_t = -(int) this->countTemps;
	}
	else
	{
		temperature_ptr->count_t = (int) this->temps.size();
	}
	return (temperature_ptr);
}
#endif

#ifdef SKIP
void
cxxTemperature::dump_xml(std::ostream & s_oss, unsigned int indent) const const
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
	for (std::list < cxxTemperatureComp >::const_iterator it =
		 temperatureComps.begin(); it != temperatureComps.end(); ++it)
	{
		it->dump_xml(s_oss, indent + 2);
	}

	return;
}
#endif

void
cxxTemperature::dump_raw(std::ostream & s_oss, unsigned int indent) const
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
	s_oss << "REACTION_TEMPERATURE_RAW        " << this->n_user << " " << this->description << std::endl;

	s_oss << indent1;
	s_oss << "-count_temps        " << this->countTemps << std::endl;

	s_oss << indent1;
	s_oss << "-equal_increments   " << this->equalIncrements << std::endl;

	// Temperature element and attributes

	s_oss << indent1;
	s_oss << "-temps              " << std::endl;
	{
		int i = 0;
		s_oss << indent2;
		for (std::vector < double >::const_iterator it = this->temps.begin();
			 it != this->temps.end(); it++)
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
cxxTemperature::read_raw(CParser & parser)
{

	double d;
	CParser::TOKEN_TYPE k;
	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(15);
		vopts.push_back("temps");	//0
		vopts.push_back("equal_increments");	//1
		vopts.push_back("count_temps");	//2
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;
	bool useLastLine(false);

	// Read temperature number and description
	this->read_number_description(parser);

	opt_save = CParser::OPT_ERROR;
	bool equalIncrements_defined(false);
	bool countTemps_defined(false);

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
			parser.error_msg("Unknown input in TEMPERATURE_COMP_RAW keyword.",
							 CParser::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
			useLastLine = false;
			break;

		case 0:				// temps
			while ((k =
					parser.copy_token(token, next_char)) == CParser::TT_DIGIT)
			{
				std::istringstream iss(token);
				if (!(iss >> d))
				{
					parser.incr_input_error();
					parser.error_msg("Expected numeric value for temps.",
									 CParser::OT_CONTINUE);
				}
				else
				{
					this->temps.push_back(d);
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
				parser.
					error_msg("Expected boolean value for equalIncrements.",
							  CParser::OT_CONTINUE);
			}
			opt_save = CParser::OPT_DEFAULT;
			useLastLine = false;
			equalIncrements_defined = true;
			break;

		case 2:				// countTemps
			if (!(parser.get_iss() >> this->countTemps))
			{
				this->countTemps = 0;
				parser.incr_input_error();
				parser.error_msg("Expected integer value for countTemps.",
								 CParser::OT_CONTINUE);
			}
			opt_save = CParser::OPT_DEFAULT;
			useLastLine = false;
			countTemps_defined = true;
			break;
		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	// members that must be defined
	if (equalIncrements_defined == false)
	{
		parser.incr_input_error();
		parser.
			error_msg
			("Equal_increments not defined for REACTION_TEMPERATURE_RAW input.",
			 CParser::OT_CONTINUE);
	}
	if (countTemps_defined == false)
	{
		parser.incr_input_error();
		parser.
			error_msg
			("Count_temps not defined for REACTION_TEMPERATURE_RAW input.",
			 CParser::OT_CONTINUE);
	}
}
