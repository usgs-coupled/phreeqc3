// Reaction.cxx: implementation of the cxxReaction class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include <cassert>				// assert
#include <algorithm>			// std::sort

#include "Utils.h"				// define first
#if !defined(PHREEQC_CLASS)
#define EXTERNAL extern
#include "global.h"
#else
#include "Phreeqc.h"
#endif
#include "Reaction.h"
#include "phqalloc.h"
#include "phrqproto.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxReaction::cxxReaction()
	//
	// default constructor for cxxReaction 
	//
:	cxxNumKeyword()
{
	this->set_units("Mol");
	countSteps = 0;
	equalIncrements = false;
	reactantList.type = cxxNameDouble::ND_NAME_COEF;
	elementList.type = cxxNameDouble::ND_ELT_MOLES;
}

cxxReaction::cxxReaction(struct irrev *irrev_ptr)
		//
		// constructor for cxxReaction from struct irrev
		//
	:
cxxNumKeyword(),
reactantList(irrev_ptr->list, irrev_ptr->count_list),
elementList(irrev_ptr->elts)
{
	int i;

	this->set_description(irrev_ptr->description);
	this->n_user = irrev_ptr->n_user;
	this->n_user_end = irrev_ptr->n_user_end;
	this->set_units(irrev_ptr->units);
	// steps
	if (irrev_ptr->count_steps < 0)
	{
		for (i = 0; i < 1; i++)
		{
			this->steps.push_back(irrev_ptr->steps[i]);
		}
		this->countSteps = -irrev_ptr->count_steps;
		this->equalIncrements = true;
	}
	else
	{
		for (i = 0; i < irrev_ptr->count_steps; i++)
		{
			this->steps.push_back(irrev_ptr->steps[i]);
		}
		this->countSteps = irrev_ptr->count_steps;
		this->equalIncrements = false;
	}
}

cxxReaction::~cxxReaction()
{
}


struct irrev *
cxxReaction::cxxReaction2irrev(PHREEQC_PTR_ARG)
		//
		// Builds a irrev structure from instance of cxxReaction 
		//
{
	struct irrev *irrev_ptr;
	irrev_ptr = (struct irrev *) P_INSTANCE_POINTER PHRQ_malloc(sizeof(struct irrev));
	if (irrev_ptr == NULL)
		P_INSTANCE_POINTER malloc_error();

	irrev_ptr->description = P_INSTANCE_POINTER string_duplicate (this->get_description().c_str());
	irrev_ptr->n_user = this->n_user;
	irrev_ptr->n_user_end = this->n_user_end;

	irrev_ptr->list = this->reactantList.name_coef(P_INSTANCE);
	irrev_ptr->count_list = (int) this->reactantList.size();
	if (this->elementList.size() > 0)
	{
		irrev_ptr->elts = this->elementList.elt_list(P_INSTANCE);
	}
	else
	{
		// NULL value causes reaction stoichiometry to be calculated
		irrev_ptr->elts = NULL;
	}
	// steps
	irrev_ptr->steps = NULL;
	if (this->steps.size() > 0)
	{
		irrev_ptr->steps =
			(LDBLE *)
			P_INSTANCE_POINTER PHRQ_malloc((size_t) (this->steps.size() * sizeof(double)));
		if (irrev_ptr->steps == NULL)
			P_INSTANCE_POINTER malloc_error();
		std::copy(this->steps.begin(), this->steps.end(), irrev_ptr->steps);
	}
	if (this->equalIncrements)
	{
		irrev_ptr->count_steps = -this->countSteps;
	}
	else
	{
		irrev_ptr->count_steps = (int) this->steps.size();
	}
	if (this->units.size() == 0)
		irrev_ptr->units = NULL;
	else
		irrev_ptr->units = P_INSTANCE_POINTER string_hsave(this->units.c_str());
	return (irrev_ptr);
}

#ifdef SKIP
void
cxxReaction::dump_xml(std::ostream & s_oss, unsigned int indent) const const
{
	//const char    ERR_MESSAGE[] = "Packing irrev message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// Reaction element and attributes
	s_oss << indent0;
	s_oss << "<irrev " << std::endl;

	s_oss << indent1;
	s_oss << "pitzer_irrev_gammas=\"" << this->
		pitzer_irrev_gammas << "\"" << std::endl;

	// components
	s_oss << indent1;
	s_oss << "<component " << std::endl;
	for (std::list < cxxReactionComp >::const_iterator it =
		 irrevComps.begin(); it != irrevComps.end(); ++it)
	{
		it->dump_xml(s_oss, indent + 2);
	}

	return;
}
#endif

void
cxxReaction::dump_raw(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing irrev message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// Reaction element and attributes
	s_oss << indent0;
	s_oss << "REACTION_RAW        " << this->n_user << " " << this->
		description << std::endl;

	s_oss << indent1;
	s_oss << "-units              " << this->units << std::endl;

	s_oss << indent1;
	s_oss << "-reactant_list      " << std::endl;
	this->reactantList.dump_raw(s_oss, indent + 2);

	s_oss << indent1;
	s_oss << "-element_list       " << std::endl;
	this->elementList.dump_raw(s_oss, indent + 2);

	s_oss << indent1;
	s_oss << "-steps              " << std::endl;
	{
		int i = 0;
		s_oss << indent2;
		for (std::vector < double >::const_iterator it = this->steps.begin();
			 it != this->steps.end(); it++)
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

	s_oss << indent1;
	s_oss << "-equal_increments   " << this->equalIncrements << std::endl;

	s_oss << indent1;
	s_oss << "-count_steps        " << this->countSteps << std::endl;


}

void
cxxReaction::read_raw(PHREEQC_PTR_ARG_COMMA CParser & parser, const bool check)
{

	int j;
	double d;
	CParser::TOKEN_TYPE k;
	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(15);
		vopts.push_back("units");	//0
		vopts.push_back("reactant_list");	//1
		vopts.push_back("element_list");	//2
		vopts.push_back("steps");	//3
		vopts.push_back("equal_increments");	//4
		vopts.push_back("count_steps");	//5
	}

	// clear steps for modify operation, if steps are read
	bool cleared_once = false;

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;
	bool useLastLine(false);

	// Read irrev number and description
	this->read_number_description(parser);

	opt_save = CParser::OPT_ERROR;
	bool units_defined(false);
	bool equalIncrements_defined(false);
	bool countSteps_defined(false);

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
			parser.error_msg("Unknown input in IRREV_COMP_RAW keyword.",
							 CParser::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
			useLastLine = false;
			break;

		case 0:				// units
			j = parser.copy_token(token, next_char);
			if (j == CParser::TT_EMPTY)
				break;
			this->set_units(token.c_str());
			opt_save = CParser::OPT_DEFAULT;
			useLastLine = false;
			units_defined = true;
			break;

		case 1:				// reactant_list
			if (this->reactantList.read_raw(P_INSTANCE_COMMA parser, next_char) !=
				CParser::PARSER_OK)
			{
				parser.incr_input_error();
				parser.error_msg("Expected reactant formula and coefficient.",
								 CParser::OT_CONTINUE);
			}
			opt_save = 1;
			useLastLine = false;
			break;

		case 2:				// element_list
			if (this->elementList.read_raw(P_INSTANCE_COMMA parser, next_char) !=
				CParser::PARSER_OK)
			{
				parser.incr_input_error();
				parser.error_msg("Expected element formula and coefficient.",
								 CParser::OT_CONTINUE);
			}
			opt_save = 2;
			useLastLine = false;
			break;

		case 3:				// steps
			if (!cleared_once) 
			{
				this->steps.clear();
				cleared_once = true;
			}
			while ((k =
					parser.copy_token(token, next_char)) == CParser::TT_DIGIT)
			{
				std::istringstream iss(token);
				if (!(iss >> d))
				{
					parser.incr_input_error();
					parser.error_msg("Expected numeric value for steps.",
									 CParser::OT_CONTINUE);
				}
				else
				{
					this->steps.push_back(d);
				}
			}
			opt_save = 3;
			useLastLine = false;
			break;

		case 4:				// equal_increments
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

		case 5:				// countSteps
			if (!(parser.get_iss() >> this->countSteps))
			{
				this->countSteps = 0;
				parser.incr_input_error();
				parser.error_msg("Expected integer value for countSteps.",
								 CParser::OT_CONTINUE);
			}
			opt_save = CParser::OPT_DEFAULT;
			useLastLine = false;
			countSteps_defined = true;
			break;
		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	if (check)
	{
		// members that must be defined
		if (units_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Units not defined for REACTION_RAW input.",
				CParser::OT_CONTINUE);
		}
		if (equalIncrements_defined == false)
		{
			parser.incr_input_error();
			parser.
				error_msg("Equal_increments not defined for REACTION_RAW input.",
				CParser::OT_CONTINUE);
		}
		if (countSteps_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Count_steps not defined for REACTION_RAW input.",
				CParser::OT_CONTINUE);
		}
	}
}
