// GasPhase.cxx: implementation of the cxxGasPhase class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include <cassert>				// assert
#include <algorithm>			// std::sort
#include <float.h>

#include "Utils.h"				// define first
#if !defined(PHREEQC_CLASS)
#define EXTERNAL extern
#include "global.h"
#else
#include "Phreeqc.h"
#endif
#include "GasPhase.h"
#include "cxxMix.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include "output.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxGasPhase::cxxGasPhase()
	//
	// default constructor for cxxGasPhase 
	//
:	cxxNumKeyword()
{
	total_p = 0;
	volume = 0;
	gasPhaseComps.type = cxxNameDouble::ND_NAME_COEF;
}

cxxGasPhase::cxxGasPhase(struct gas_phase *gas_phase_ptr)
		//
		// constructor for cxxGasPhase from struct gas_phase
		//
	:
cxxNumKeyword()
{
	int i;

	this->set_description(gas_phase_ptr->description);
	n_user = gas_phase_ptr->n_user;
	n_user_end = gas_phase_ptr->n_user_end;
	if (gas_phase_ptr->type == PRESSURE)
	{
		type = cxxGasPhase::GP_PRESSURE;
	}
	else
	{
		type = cxxGasPhase::GP_VOLUME;
	}
	total_p = gas_phase_ptr->total_p;
	volume = gas_phase_ptr->volume;

	// gas_phase components
	for (i = 0; i < gas_phase_ptr->count_comps; i++)
	{
		if (gas_phase_ptr->comps[i].name == NULL)
			continue;
		gasPhaseComps[gas_phase_ptr->comps[i].name] =
			gas_phase_ptr->comps[i].moles;
	}
}
cxxGasPhase::cxxGasPhase(PHREEQC_PTR_ARG_COMMA const std::map < int, cxxGasPhase > &entities,
						 cxxMix & mix, int n_user):
cxxNumKeyword()
{
	this->n_user = this->n_user_end = n_user;
	gasPhaseComps.type = cxxNameDouble::ND_NAME_COEF;
	total_p = 0;
	volume = 0;
	bool first = true;
//
//   Mix
//
	//cxxNameDouble gasPhaseComps;
	std::map < int, double >*mixcomps = mix.comps();
	std::map < int, double >::const_iterator it;
	for (it = mixcomps->begin(); it != mixcomps->end(); it++)
	{
		if (entities.find(it->first) != entities.end())
		{
			const cxxGasPhase *entity_ptr =
				&(entities.find(it->first)->second);
			this->gasPhaseComps.add_extensive(entity_ptr->gasPhaseComps,
											  it->second);
			//GP_TYPE type;
			//double total_p;
			//double volume;
			if (first)
			{
				this->type = entity_ptr->type;
				this->total_p = entity_ptr->total_p * it->second;
				this->volume = entity_ptr->volume * it->second;
				first = false;
			}
			else
			{
				if (this->type != entity_ptr->type)
				{
					std::ostringstream oss;
					oss << "Can not mix two gas_phases with differing types.";
					P_INSTANCE_POINTER error_msg(oss.str().c_str(), CONTINUE);
					P_INSTANCE_POINTER input_error++;
					return;
				}

				this->total_p += entity_ptr->total_p * it->second;
				this->volume += entity_ptr->volume * it->second;
			}
		}
	}
}

cxxGasPhase::~cxxGasPhase()
{
}

struct gas_comp *
cxxGasPhase::cxxGasPhaseComp2gas_comp(PHREEQC_PTR_ARG)
{
	//struct gas_comp *gas_comp_ptr(NULL);
	struct gas_comp *gas_comp_ptr = NULL;
	if (this->gasPhaseComps.size() > 0)
	{
		int i = 0;
		int n;
		gas_comp_ptr =
			(struct gas_comp *)
			P_INSTANCE_POINTER PHRQ_malloc((size_t)
						(this->gasPhaseComps.size() *
						 sizeof(struct gas_comp)));
		if (gas_comp_ptr == NULL)
			P_INSTANCE_POINTER malloc_error();
		for (cxxNameDouble::iterator it = this->gasPhaseComps.begin();
			 it != this->gasPhaseComps.end(); it++)
		{
			gas_comp_ptr[i].name = P_INSTANCE_POINTER string_hsave(it->first.c_str());
			assert(it->first.size() != 0);
			gas_comp_ptr[i].phase = P_INSTANCE_POINTER phase_bsearch(it->first.c_str(), &n, TRUE);
			gas_comp_ptr[i].p_read = 0;
			gas_comp_ptr[i].moles = it->second;
			gas_comp_ptr[i].initial_moles = 0;
			i++;
		}
	}
	return (gas_comp_ptr);
}

struct gas_phase *
cxxGasPhase::cxxGasPhase2gas_phase(PHREEQC_PTR_ARG)
		//
		// Builds a gas_phase structure from instance of cxxGasPhase 
		//
{
	struct gas_phase *gas_phase_ptr = P_INSTANCE_POINTER gas_phase_alloc();

	gas_phase_ptr->description = P_INSTANCE_POINTER string_duplicate (this->get_description().c_str());
	gas_phase_ptr->n_user = this->n_user;
	gas_phase_ptr->n_user_end = this->n_user_end;
	gas_phase_ptr->new_def = FALSE;
	gas_phase_ptr->solution_equilibria = FALSE;
	gas_phase_ptr->n_solution = -2;
	if (this->type == cxxGasPhase::GP_PRESSURE)
	{
		gas_phase_ptr->type = PRESSURE;
	}
	else
	{
		gas_phase_ptr->type = VOLUME;
	}
	gas_phase_ptr->total_p = this->total_p;
	gas_phase_ptr->volume = this->volume;
	gas_phase_ptr->temperature = 273.15;

	// comps
	gas_phase_ptr->count_comps = (int) this->gasPhaseComps.size();
	gas_phase_ptr->comps =
		(struct gas_comp *) P_INSTANCE_POINTER free_check_null(gas_phase_ptr->comps);
	gas_phase_ptr->comps = this->cxxGasPhaseComp2gas_comp(P_INSTANCE);

	return (gas_phase_ptr);
}

#ifdef SKIP
void
cxxGasPhase::dump_xml(std::ostream & s_oss, unsigned int indent) const const
{
	//const char    ERR_MESSAGE[] = "Packing gas_phase message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// GasPhase element and attributes
	s_oss << indent0;
	s_oss << "<gas_phase " << std::endl;

	s_oss << indent1;
	s_oss << "pitzer_gas_phase_gammas=\"" << this->
		pitzer_gas_phase_gammas << "\"" << std::endl;

	// components
	s_oss << indent1;
	s_oss << "<component " << std::endl;
	for (std::list < cxxGasPhaseComp >::const_iterator it =
		 gas_phaseComps.begin(); it != gas_phaseComps.end(); ++it)
	{
		it->dump_xml(s_oss, indent + 2);
	}

	return;
}
#endif

void
cxxGasPhase::dump_raw(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing gas_phase message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// GasPhase element and attributes
	s_oss << indent0;
	s_oss << "GAS_PHASE_RAW       " << this->n_user << " " << this->
		description << std::endl;

	s_oss << indent1;
	s_oss << "-type               " << this->type << std::endl;

	s_oss << indent1;
	s_oss << "-total_p            " << this->total_p << std::endl;

	s_oss << indent1;
	s_oss << "-volume             " << this->volume << std::endl;

	// gasPhaseComps 
	s_oss << indent1;
	s_oss << "-component" << std::endl;
	this->gasPhaseComps.dump_raw(s_oss, indent + 2);
}

void
cxxGasPhase::read_raw(PHREEQC_PTR_ARG_COMMA CParser & parser, bool check)
{

	int i;
	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(15);
		vopts.push_back("type");	//0
		vopts.push_back("total_p");	//1
		vopts.push_back("volume");	//2
		vopts.push_back("component");	//3
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;
	bool useLastLine(false);

	// Read gas_phase number and description
	this->read_number_description(parser);

	opt_save = CParser::OPT_ERROR;
	bool type_defined(false);
	bool total_p_defined(false);
	bool volume_defined(false);

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
			parser.error_msg("Unknown input in GAS_PHASE_COMP_RAW keyword.",
							 CParser::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
			useLastLine = false;
			break;

		case 0:				// type
			if (!(parser.get_iss() >> i))
				//if (!(parser.get_iss() >> (cxxGasPhase::GP_TYPE) this->type))
			{
				this->type = cxxGasPhase::GP_PRESSURE;
				parser.incr_input_error();
				parser.error_msg("Expected enum for type.",
								 CParser::OT_CONTINUE);
			}
			else
			{
				this->type = (cxxGasPhase::GP_TYPE) i;
			}
			type_defined = true;
			useLastLine = false;
			break;

		case 1:				// total_p
			if (!(parser.get_iss() >> this->total_p))
			{
				this->total_p = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for total_p.",
								 CParser::OT_CONTINUE);
			}
			total_p_defined = true;
			useLastLine = false;
			break;

		case 2:				// volume
			if (!(parser.get_iss() >> this->volume))
			{
				this->volume = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for volume.",
								 CParser::OT_CONTINUE);
			}
			volume_defined = true;
			useLastLine = false;
			break;

		case 3:				// component
			if (this->gasPhaseComps.read_raw(P_INSTANCE_COMMA parser, next_char) !=
				CParser::PARSER_OK)
			{
				parser.incr_input_error();
				parser.
					error_msg
					("Expected gas component name and moles for gasPhaseComps.",
					 CParser::OT_CONTINUE);
			}
			opt_save = 3;
			useLastLine = false;
			break;
		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	if (check)
	{
		// members that must be defined
		if (type_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Type not defined for GAS_PHASE_RAW input.",
				CParser::OT_CONTINUE);
		}
		if (total_p_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Total_p not defined for GAS_PHASE_RAW input.",
				CParser::OT_CONTINUE);
		}
		if (volume_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Volume not defined for GAS_PHASE_RAW input.",
				CParser::OT_CONTINUE);
		}
	}
}

#ifdef USE_MPI
void
cxxGasPhase::mpi_pack(std::vector < int >&ints,
					  std::vector < double >&doubles)
{
	ints.push_back(this->n_user);
	this->gasPhaseComps.mpi_pack(ints, doubles);
	if (this->type == cxxGasPhase::GP_PRESSURE)
	{
		ints.push_back(0);
	}
	else
	{
		ints.push_back(1);
	}
	doubles.push_back(this->total_p);
	doubles.push_back(this->volume);
}

void
cxxGasPhase::mpi_unpack(int *ints, int *ii, double *doubles, int *dd)
{
	int i = *ii;
	int d = *dd;
	this->n_user = ints[i++];
	this->n_user_end = this->n_user;
	this->description = " ";
	this->gasPhaseComps.mpi_unpack(ints, &i, doubles, &d);
	int n = ints[i++];
	if (n == 0)
	{
		this->type = cxxGasPhase::GP_PRESSURE;
	}
	else
	{
		this->type = cxxGasPhase::GP_VOLUME;
	}
	this->total_p = doubles[d++];
	this->volume = doubles[d++];
	*ii = i;
	*dd = d;
}
#endif
void
cxxGasPhase::totalize(PHREEQC_PTR_ARG)
{
	this->totals.clear();
	// component structures
	for (cxxNameDouble::const_iterator it = this->gasPhaseComps.begin();
		 it != this->gasPhaseComps.end(); it++)
	{
		struct phase *phase_ptr;
		int l;
		phase_ptr = P_INSTANCE_POINTER phase_bsearch(it->first.c_str(), &l, FALSE);
		if (phase_ptr != NULL)
		{
			cxxNameDouble phase_formula(phase_ptr->next_elt);
			this->totals.add_extensive(phase_formula, it->second);
		}
		else
		{
			assert(false);
		}
	}
	return;
}
