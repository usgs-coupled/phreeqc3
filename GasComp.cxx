// ExchComp.cxx: implementation of the cxxExchComp class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include <iostream>				// std::cout std::cerr
#include <cassert>				// assert
#include <algorithm>			// std::sort
#include <float.h>

#include "Utils.h"				// define first
#include "Phreeqc.h"
#include "GasComp.h"
#include "phqalloc.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxGasComp::cxxGasComp(PHRQ_io *io)
	//
	// default constructor for cxxExchComp 
	//
	: PHRQ_base(io)
{
	p_read = 0.0;
	moles = 0.0;
	initial_moles = 0.0;
}

cxxGasComp::~cxxGasComp()
{
}

void
cxxGasComp::dump_raw(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing exch_comp message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);
	/*
	std::string phase_name;
	std::string name;
	LDBLE p_read;
	LDBLE moles;
	LDBLE initial_moles;	// Exch_Comp element and attributes
	*/
	s_oss << indent0 << "-phase_name               " << this->phase_name << "\n";
	//s_oss << indent0 << "-name                     " << this->name << "\n";
	s_oss << indent0 << "-p_read                   " << this->p_read << "\n";
	s_oss << indent0 << "-moles                    " << this->moles << "\n";
	s_oss << indent0 << "-initial_moles            " << this->initial_moles << "\n";

	//s_oss << indent1 << "# critical values" << "\n";

}

bool
cxxGasComp::read_raw(CParser & parser, bool check)
{
	std::string str;

	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(10);
		vopts.push_back("phase_name");	// 0 
		vopts.push_back("name");	// 1
		vopts.push_back("p_read");	// 2 
		vopts.push_back("moles");	// 3 
		vopts.push_back("initial_moles");	// 4 
	}
	int errors = parser.get_input_error();

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;

	opt_save = CParser::OPT_ERROR;
	bool phase_name_defined(false);
	bool moles_defined(false);
	int opt;
	for (;;)
	{
		opt = parser.get_option(vopts, next_char);
		//if (opt == CParser::OPT_DEFAULT)
		//{
		//	opt = opt_save;
		//}

		switch (opt)
		{
		case CParser::OPT_EOF:
			break;
		case CParser::OPT_KEYWORD:
			break;
		case CParser::OPT_DEFAULT:
		case CParser::OPT_ERROR:
			opt = CParser::OPT_KEYWORD;
			// Allow return to Exchange for more processing
			//parser.error_msg("Unknown input in EXCH_COMP read.", PHRQ_io::OT_CONTINUE);
			//parser.error_msg(parser.line().c_str(), PHRQ_io::OT_CONTINUE);
			break;

		case 0:				// phase_name
			if (!(parser.get_iss() >> str))
			{
				this->phase_name.clear();
				parser.incr_input_error();
				parser.error_msg("Expected string value for phase_name.",
								 PHRQ_io::OT_CONTINUE);
			}
			else
			{
				this->phase_name = str;
			}
			phase_name_defined = true;
			break;

		case 1:				// name
			if (!(parser.get_iss() >> str))
			{
				this->phase_name.clear();
				parser.incr_input_error();
				parser.error_msg("Expected string value for name.",
								 PHRQ_io::OT_CONTINUE);
			}
			else
			{
				this->phase_name = str;
			}
			break;

		case 2:				// p_read
			if (!(parser.get_iss() >> this->p_read))
			{
				this->p_read = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for initial partial pressure.",
								 PHRQ_io::OT_CONTINUE);
			}
			break;

		case 3:				// moles
			if (!(parser.get_iss() >> this->moles))
			{
				this->moles = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for moles.",
								 PHRQ_io::OT_CONTINUE);
			}
			moles_defined = true;
			break;

		case 4:				// initial_moles
			if (!(parser.get_iss() >> this->initial_moles))
			{
				this->initial_moles = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for initial_moles.",
								 PHRQ_io::OT_CONTINUE);
			}
			break;
		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	if (check)
	{
		// members that must be defined
		if (phase_name_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Phase_name not defined for GasComp input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (moles_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Moles not defined for GasComp input.",
				PHRQ_io::OT_CONTINUE);
		}
	}
	return (parser.get_input_error() == errors);
}
void
cxxGasComp::add(const cxxGasComp & addee, LDBLE extensive)
{
	LDBLE ext1, ext2, f1, f2;
	if (extensive == 0.0)
		return;
	if (addee.phase_name.size() == 0)
		return;

	ext1 = this->moles;
	ext2 = addee.moles * extensive;
	if (ext1 + ext2 != 0)
	{
		f1 = ext1 / (ext1 + ext2);
		f2 = ext2 / (ext1 + ext2);
	}
	else
	{
		f1 = 0.5;
		f2 = 0.5;
	}

	assert(this->phase_name == addee.phase_name);

	//LDBLE moles;
	this->p_read += addee.p_read * extensive;
	this->moles += addee.moles * extensive;
	this->initial_moles += addee.initial_moles * extensive;
}

void
cxxGasComp::multiply(LDBLE extensive)
{
	this->p_read *= extensive;
	this->moles *= extensive;
	this->initial_moles *= extensive;
}

