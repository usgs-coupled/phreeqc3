// Mix.cxx: implementation of the cxxMix class.
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
#include "cxxMix.h"
#include "phqalloc.h"
#include "phrqproto.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxMix::cxxMix(PHRQ_io *io)
	//
	// default constructor for cxxMix 
	//
:	cxxNumKeyword(io)
{
}

cxxMix::cxxMix(struct mix *mix_ptr, PHRQ_io *io)
		//
		// constructor for cxxMix from struct mix
		//
	:
cxxNumKeyword(io)
{
	int i;

	this->set_description(mix_ptr->description);
	this->n_user = mix_ptr->n_user;
	this->n_user_end = mix_ptr->n_user_end;
	// comps
	if (mix_ptr->count_comps > 0)
	{
		for (i = 0; i < mix_ptr->count_comps; i++)
		{
			this->mixComps[mix_ptr->comps[i].n_solution] =
				mix_ptr->comps[i].fraction;
		}
	}
}

cxxMix::~cxxMix()
{
}


struct mix *
cxxMix::cxxMix2mix(PHREEQC_PTR_ARG)
		//
		// Builds a mix structure from instance of cxxMix 
		//
{
	struct mix *mix_ptr;
	mix_ptr = (struct mix *) P_INSTANCE_POINTER PHRQ_malloc(sizeof(struct mix));
	if (mix_ptr == NULL)
		P_INSTANCE_POINTER malloc_error();

	mix_ptr->description = P_INSTANCE_POINTER string_duplicate (this->get_description().c_str());
	mix_ptr->n_user = this->n_user;
	mix_ptr->n_user_end = this->n_user_end;

	// comps
	mix_ptr->comps = NULL;
	if (this->mixComps.size() > 0)
	{
		int i = 0;
		mix_ptr->comps =
			(struct mix_comp *)
			P_INSTANCE_POINTER PHRQ_malloc((size_t)
						(this->mixComps.size() * sizeof(struct mix_comp)));
		if (mix_ptr->comps == NULL)
			P_INSTANCE_POINTER malloc_error();
		for (std::map < int, double >::iterator it = mixComps.begin();
			 it != mixComps.end(); it++)
		{
			mix_ptr->comps[i].n_solution = it->first;
			mix_ptr->comps[i].fraction = it->second;
			i++;
		}
	}
	mix_ptr->count_comps = (int) this->mixComps.size();
	return (mix_ptr);
}

#ifdef SKIP
void
cxxMix::dump_xml(std::ostream & s_oss, unsigned int indent) const const
{
	//const char    ERR_MESSAGE[] = "Packing mix message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// Mix element and attributes
	s_oss << indent0;
	s_oss << "<mix " << std::endl;

	s_oss << indent1;
	s_oss << "pitzer_mix_gammas=\"" << this->
		pitzer_mix_gammas << "\"" << std::endl;

	// components
	s_oss << indent1;
	s_oss << "<component " << std::endl;
	for (std::list < cxxMixComp >::const_iterator it = mixComps.begin();
		 it != mixComps.end(); ++it)
	{
		it->dump_xml(s_oss, indent + 2);
	}

	return;
}
#endif

void
cxxMix::dump_raw(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing mix message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// Mix element and attributes
	s_oss << indent0;
	s_oss << "MIX        " << this->n_user << " " << this->
		description << std::endl;

	for (std::map < int, double >::const_iterator it = this->mixComps.begin();
		 it != this->mixComps.end(); it++)
	{
		s_oss << indent1 << it->first << "     " << it->second << std::endl;
	}
}

void
cxxMix::read_raw(CParser & parser)
{

	int i;
	double d;
	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(15);
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;
	bool useLastLine(false);

	// Read mix number and description
	this->read_number_description(parser);

	opt_save = CParser::OPT_DEFAULT;

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
		case CParser::OPT_ERROR:
			opt = CParser::OPT_EOF;
			parser.error_msg("Unknown input in MIX_COMP_RAW keyword.",
							 CParser::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
			useLastLine = false;
			break;

		case CParser::OPT_DEFAULT:	// solution number, mix fraction
			if (parser.copy_token(token, next_char) != CParser::TT_EMPTY)
			{
				std::istringstream iss(token);
				if (!(iss >> i))
				{
					parser.incr_input_error();
					parser.
						error_msg
						("Expected integer value for solution number.",
						 CParser::OT_CONTINUE);
					break;
				}
				if (!(parser.get_iss() >> d))
				{
					parser.incr_input_error();
					parser.
						error_msg
						("Expected numeric value for solution fraction.",
						 CParser::OT_CONTINUE);
					break;
				}
				this->mixComps[i] = d;
			}
			opt_save = CParser::OPT_DEFAULT;
			break;
		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	// members that must be defined
}
