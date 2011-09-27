#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include <cassert>
#include <sstream>				// std::ostrstream

#include "Utils.h"
#if !defined(PHREEQC_CLASS)
#define EXTERNAL extern
#include "global.h"
#else
#include "Phreeqc.h"
#endif
#include "SolutionIsotope.h"
#include "phqalloc.h"
#include "phrqproto.h"

cxxSolutionIsotope::cxxSolutionIsotope(PHRQ_io *io)
:
PHRQ_base(io),
isotope_number(0.0)
{
	isotope_number = 0;
	elt_name.clear();
	isotope_name.clear();
	total = 0;
	ratio = -9999.9;
	ratio_uncertainty = 1;
	ratio_uncertainty_defined = false;
}

cxxSolutionIsotope::cxxSolutionIsotope(struct isotope *isotope_ptr, PHRQ_io *io)
:
PHRQ_base(io)
{
	isotope_number = isotope_ptr->isotope_number;
	this->Set_elt_name(isotope_ptr->elt_name);
	std::ostringstream name;
	name << isotope_ptr->isotope_number << isotope_ptr->elt_name;
	this->Set_isotope_name(name.str().c_str());
	total = isotope_ptr->total;
	ratio = isotope_ptr->ratio;
	ratio_uncertainty = isotope_ptr->ratio_uncertainty;
}


cxxSolutionIsotope::~cxxSolutionIsotope(void)
{
}

#ifdef SKIP_OR_MOVE_TO_STRUCTURES
struct isotope *
cxxSolutionIsotope::list2isotope(PHREEQC_PTR_ARG_COMMA std::list < cxxSolutionIsotope > &isolist)
		// takes a std::list of cxxSolutionIsotope structures
		// returns array of isotope structures
{
	struct isotope *iso;
	if (isolist.size() <= 0)
	{
		return NULL;
	}
	else
	{
		iso =
			(struct isotope *)
			P_INSTANCE_POINTER PHRQ_malloc((size_t) ((isolist.size()) * sizeof(struct isotope)));
		if (iso == NULL)
			P_INSTANCE_POINTER malloc_error();
		int i = 0;
		for (std::list < cxxSolutionIsotope >::iterator it = isolist.begin();
			 it != isolist.end(); ++it)
		{
			iso[i].isotope_number = it->isotope_number;
			iso[i].elt_name = P_INSTANCE_POINTER string_hsave(it->elt_name.c_str());
			iso[i].total = it->total;
			iso[i].ratio = it->ratio;
			iso[i].ratio_uncertainty = it->ratio_uncertainty;
			iso[i].master = it->master(P_INSTANCE);
			iso[i].primary = it->primary(P_INSTANCE);
			i++;
		}
	}
	return (iso);
}
#endif

#ifdef SKIP
std::string cxxSolutionIsotope::get_name() constconst
{
	std::ostringstream oss;
	//std::ostrstream oss;
	oss << this->isotope_number << this->elt_name;
	return oss.str();
}
#endif

void
cxxSolutionIsotope::dump_xml(std::ostream & s_oss, unsigned int indent) const
{
	unsigned int i;

	std::string indent0(""), indent1("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);

	s_oss << indent0;
	s_oss << "<soln_isotope=\"" << std::endl;

	s_oss << indent1;
	s_oss << "iso_isotope_number=\"" << this->
		isotope_number << "\"" << std::endl;

	s_oss << indent1;
	s_oss << "iso_elt_name=\"" << this->elt_name << "\"" << std::endl;

	s_oss << indent1;
	s_oss << "iso_isotope_name=\"" << this->isotope_name << "\"" << std::endl;

	s_oss << indent1;
	s_oss << "iso_total=\"" << this->total << "\"" << std::endl;

	s_oss << indent1;
	s_oss << "iso_ratio=\"" << this->ratio << "\"" << std::endl;

	if (this->ratio_uncertainty != NAN)
	{
		s_oss << indent1;
		s_oss << "iso_ratio_uncertainty=\"" << this->
			ratio_uncertainty << "\"" << std::endl;
	}
	s_oss << indent0;
	s_oss << "\">" << std::endl;
}

void
cxxSolutionIsotope::dump_raw(std::ostream & s_oss, unsigned int indent) const
{
	unsigned int i;

	std::string indent0("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);

	s_oss << indent0;
	s_oss << this->isotope_name << " ";
	s_oss << this->isotope_number << " ";
	s_oss << this->elt_name << " ";
	s_oss << this->total << " ";
	s_oss << this->ratio << " ";
	if (this->ratio_uncertainty != NAN)
	{
		s_oss << this->ratio_uncertainty << " ";
	}
	s_oss << std::endl;
}

CParser::STATUS_TYPE cxxSolutionIsotope::read_raw(CParser & parser, std::istream::pos_type next_char )
{
	std::string token;
	CParser::TOKEN_TYPE j;

	// isotope_name
	j = parser.copy_token(token, next_char);

	if (j == CParser::TT_EMPTY)
	{
		this->isotope_name.clear();
		return (CParser::PARSER_OK);
	}
	this->Set_isotope_name(token.c_str());

	// isotope_number
	if (!(parser.get_iss() >> isotope_number))
	{
		return CParser::PARSER_ERROR;
	}

	// elt_name
	if (!(parser.get_iss() >> token))
	{
		return CParser::PARSER_ERROR;
	}
	this->Set_elt_name(token.c_str());

	// total
	if (!(parser.get_iss() >> this->total))
	{
		return CParser::PARSER_ERROR;
	}

	// ratio
	if (!(parser.get_iss() >> this->ratio))
	{
		return CParser::PARSER_ERROR;
	}

	// ratio_uncertainty
	j = parser.peek_token();
	if (j == CParser::TT_EMPTY)
	{
		this->ratio_uncertainty = NAN;
	}
	else if (j != CParser::TT_DIGIT)
	{
		parser.incr_input_error();
		parser.
			error_msg("Expected numeric value for mass of water in solution.",
					  CParser::OT_CONTINUE);
	}
	else
	{
		parser.get_iss() >> this->ratio_uncertainty;
		this->ratio_uncertainty_defined = true;
	}

	return CParser::PARSER_OK;
}

bool
cxxSolutionIsotope::operator<(const cxxSolutionIsotope & isotope) const
{
	int i = Utilities::strcmp_nocase(this->elt_name.c_str(), isotope.elt_name.c_str());
	if (i != 0)
		return (i < 0);
	return (this->isotope_number < isotope.isotope_number);
}
#ifdef SKIP_OR_MOVE_TO_STRUCTURES
struct master *
cxxSolutionIsotope::master(PHREEQC_PTR_ARG)
{
	return (P_INSTANCE_POINTER master_bsearch(this->elt_name.c_str()));
}
#endif
#ifdef SKIP_OR_MOVE_TO_STRUCTURES
struct master *
cxxSolutionIsotope::primary(PHREEQC_PTR_ARG)
{
	char * str = P_INSTANCE_POINTER string_hsave(this->elt_name.c_str());
	return (P_INSTANCE_POINTER master_bsearch_primary(str));
}
#endif
void
cxxSolutionIsotope::add(const cxxSolutionIsotope & isotope_ptr,
						double intensive, double extensive)
{
	if ((this->isotope_number == isotope_ptr.isotope_number) &&
		(this->elt_name == isotope_ptr.elt_name) &&
		(this->isotope_name == isotope_ptr.isotope_name))
	{
		this->total += isotope_ptr.total * extensive;
		this->ratio += isotope_ptr.ratio * intensive;
		this->ratio_uncertainty += isotope_ptr.ratio_uncertainty * intensive;
		this->ratio_uncertainty_defined = (this->ratio_uncertainty_defined
										   || isotope_ptr.
										   ratio_uncertainty_defined);
	}
}
void
cxxSolutionIsotope::multiply(double extensive)
{
	this->total *= extensive;
}

#ifdef SKIP
cxxSolutionIsotope::STATUS cxxSolutionIsotope::read(CParser & parser)
{
	if (!(parser.get_iss() >> this->isotope_number))
	{
		assert(parser.get_iss().fail());
		parser.incr_input_error();
		parser.error_msg("Expected isotope name to"
						 " begin with an isotopic number.",
						 CParser::OT_CONTINUE);
		return ERROR;
	}
	assert(parser.get_iss().good() || parser.get_iss().eof());

	// read and save element name
	std::istringstream::int_type c = parser.get_iss().peek();
	if (c == std::char_traits < char >::eof() || !(::isupper(c)))
	{
		parser.error_msg("Expecting element name.", CParser::OT_CONTINUE);
		parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
		parser.incr_input_error();
		return ERROR;
	}
	assert(parser.get_iss().good() || parser.get_iss().eof());
	if (!(parser.get_iss() >> this->elt_name))
	{
		// should never get here
		return ERROR;
	}
	assert(parser.get_iss().good() || parser.get_iss().eof());
	assert(!this->elt_name.empty() &&::isupper(this->elt_name[0]));

	// read and store isotope ratio
	if (!(parser.get_iss() >> this->ratio))
	{
		assert(parser.get_iss().fail());
		parser.incr_input_error();
		parser.error_msg("Expected numeric value for isotope ratio.",
						 CParser::OT_CONTINUE);
		return ERROR;
	}
	assert(parser.get_iss().good() || parser.get_iss().eof());

	// read and store isotope ratio
	this->ratio_uncertainty_defined = false;
	if (!(parser.get_iss() >> this->ratio_uncertainty))
	{
		if (!parser.get_iss().eof())
		{
			parser.incr_input_error();
			parser.
				error_msg
				("Expected numeric value for uncertainty in isotope ratio.",
				 CParser::OT_CONTINUE);
			return ERROR;
		}
	}
	else
	{
		this->ratio_uncertainty_defined = true;
	}
	assert(parser.get_iss().good() || parser.get_iss().eof());
	return OK;
}


#endif
