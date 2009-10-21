// PPassemblage.cxx: implementation of the cxxPPassemblage class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif

#include "Utils.h"				// define first
#include "PPassemblage.h"
#include "PPassemblageComp.h"
#define EXTERNAL extern
#include "global.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include <cassert>				// assert
#include <algorithm>			// std::sort

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxPPassemblage::cxxPPassemblage()
	//
	// default constructor for cxxPPassemblage 
	//
:	cxxNumKeyword()
{
	eltList.type = cxxNameDouble::ND_ELT_MOLES;
}

cxxPPassemblage::cxxPPassemblage(struct pp_assemblage *pp_assemblage_ptr)
		//
		// constructor for cxxPPassemblage from struct PPassemblage
		//
	:
cxxNumKeyword(),
eltList(pp_assemblage_ptr->next_elt)
{
	int i;

	this->set_description(pp_assemblage_ptr->description);
	n_user = pp_assemblage_ptr->n_user;
	n_user_end = pp_assemblage_ptr->n_user_end;
	for (i = 0; i < pp_assemblage_ptr->count_comps; i++)
	{
		cxxPPassemblageComp ppComp(&(pp_assemblage_ptr->pure_phases[i]));
		ppAssemblageComps.push_back(ppComp);
	}
}
cxxPPassemblage::cxxPPassemblage(const std::map < int,
								 cxxPPassemblage > &entities, cxxMix & mix,
								 int n_user):
cxxNumKeyword()
{
	this->n_user = this->n_user_end = n_user;
	eltList.type = cxxNameDouble::ND_ELT_MOLES;
//
//   Mix
//
	std::map < int, double >*mixcomps = mix.comps();
	std::map < int, double >::const_iterator it;
	for (it = mixcomps->begin(); it != mixcomps->end(); it++)
	{
		if (entities.find(it->first) != entities.end())
		{
			const cxxPPassemblage *entity_ptr =
				&(entities.find(it->first)->second);
			this->add(*entity_ptr, it->second);
		}
	}
}

cxxPPassemblage::~cxxPPassemblage()
{
}

struct pp_assemblage *
cxxPPassemblage::cxxPPassemblage2pp_assemblage()
		//
		// Builds a pp_assemblage structure from instance of cxxPPassemblage 
		//
{
	struct pp_assemblage *pp_assemblage_ptr = pp_assemblage_alloc();

	pp_assemblage_ptr->description = this->get_description();
	pp_assemblage_ptr->n_user = this->n_user;
	pp_assemblage_ptr->n_user_end = this->n_user_end;
	pp_assemblage_ptr->new_def = FALSE;
	pp_assemblage_ptr->count_comps = (int) this->ppAssemblageComps.size();
	pp_assemblage_ptr->pure_phases =
		(struct pure_phase *) free_check_null(pp_assemblage_ptr->pure_phases);
	pp_assemblage_ptr->pure_phases =
		cxxPPassemblageComp::cxxPPassemblageComp2pure_phase(this->
															ppAssemblageComps);
	pp_assemblage_ptr->next_elt = this->eltList.elt_list();
	return (pp_assemblage_ptr);
}

void
cxxPPassemblage::dump_xml(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing PPassemblage message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// PPassemblage element and attributes
	s_oss << indent0;
	s_oss << "<EQUILIBRIUM_PHASES " << std::endl;

	// eltList
	this->eltList.dump_xml(s_oss, indent + 1);

	// ppAssemblageComps
	s_oss << indent1;
	s_oss << "<pure_phases " << std::endl;
	for (std::list < cxxPPassemblageComp >::const_iterator it =
		 ppAssemblageComps.begin(); it != ppAssemblageComps.end(); ++it)
	{
		it->dump_xml(s_oss, indent + 2);
	}
}

void
cxxPPassemblage::dump_raw(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing PPassemblage message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// PPassemblage element and attributes
	s_oss << indent0;
	s_oss << "EQUILIBRIUM_PHASES_RAW       " << this->n_user << " " << this->
		description << std::endl;

	// eltList

	s_oss << indent1;
	s_oss << "-eltList       " << std::endl;
	this->eltList.dump_raw(s_oss, indent + 2);

	// ppAssemblagComps
	for (std::list < cxxPPassemblageComp >::const_iterator it =
		 ppAssemblageComps.begin(); it != ppAssemblageComps.end(); ++it)
	{
		s_oss << indent1;
		s_oss << "-component" << std::endl;
		it->dump_raw(s_oss, indent + 2);
	}
}

void
cxxPPassemblage::read_raw(CParser & parser, bool check)
{
	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(15);
		vopts.push_back("eltlist");	// 0
		vopts.push_back("component");	// 1
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;
	bool useLastLine(false);

	// Read PPassemblage number and description
	this->read_number_description(parser);

	opt_save = CParser::OPT_ERROR;

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
			parser.
				error_msg("Unknown input in EQUILIBRIUM_PHASES_RAW keyword.",
						  CParser::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
			useLastLine = false;
			break;

		case 0:				// eltList
			if (this->eltList.read_raw(parser, next_char) !=
				CParser::PARSER_OK)
			{
				parser.incr_input_error();
				parser.
					error_msg("Expected element name and moles for totals.",
							  CParser::OT_CONTINUE);
			}
			opt_save = 0;
			break;

		case 1:				// component
			{
				cxxPPassemblageComp ppComp;
				ppComp.read_raw(parser);
				this->ppAssemblageComps.push_back(ppComp);
			}
			useLastLine = true;
			break;
		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	// members that must be defined
}

#ifdef USE_MPI
/* ---------------------------------------------------------------------- */
void
cxxPPassemblage::mpi_pack(std::vector < int >&ints,
						  std::vector < double >&doubles)
/* ---------------------------------------------------------------------- */
{
	/* int n_user; */
	ints.push_back(this->n_user);
	ints.push_back((int) this->ppAssemblageComps.size());
	for (std::list < cxxPPassemblageComp >::iterator it =
		 this->ppAssemblageComps.begin(); it != this->ppAssemblageComps.end();
		 it++)
	{
		it->mpi_pack(ints, doubles);
	}
	this->eltList.mpi_pack(ints, doubles);
}

/* ---------------------------------------------------------------------- */
void
cxxPPassemblage::mpi_unpack(int *ints, int *ii, double *doubles, int *dd)
/* ---------------------------------------------------------------------- */
{
	int i = *ii;
	int d = *dd;
	/* int n_user; */
	this->n_user = ints[i++];
	this->n_user_end = this->n_user;
	this->description = " ";

	int count = ints[i++];
	this->ppAssemblageComps.clear();
	for (int n = 0; n < count; n++)
	{
		cxxPPassemblageComp ppc;
		ppc.mpi_unpack(ints, &i, doubles, &d);
		this->ppAssemblageComps.push_back(ppc);
	}
	this->eltList.mpi_unpack(ints, &i, doubles, &d);
	*ii = i;
	*dd = d;
}
#endif

void
cxxPPassemblage::totalize()
{
	this->totals.clear();
	// component structures
	for (std::list < cxxPPassemblageComp >::iterator it =
		 ppAssemblageComps.begin(); it != ppAssemblageComps.end(); ++it)
	{
		it->totalize();
		this->totals.add_extensive(it->get_totals(), 1.0);
	}
	return;
}

void
cxxPPassemblage::add(const cxxPPassemblage & addee, double extensive)
		//
		// Add to existing ppassemblage to "this" ppassemblage
		//
{
	if (extensive == 0.0)
		return;
	//std::list<cxxPPassemblageComp> ppAssemblageComps;
	for (std::list < cxxPPassemblageComp >::const_iterator itadd =
		 addee.ppAssemblageComps.begin();
		 itadd != addee.ppAssemblageComps.end(); ++itadd)
	{
		bool found = false;
		for (std::list < cxxPPassemblageComp >::iterator it =
			 this->ppAssemblageComps.begin();
			 it != this->ppAssemblageComps.end(); ++it)
		{
			if (it->get_name() == itadd->get_name())
			{
				it->add((*itadd), extensive);
				found = true;
				break;
			}
		}
		if (!found)
		{
			cxxPPassemblageComp entity = *itadd;
			entity.multiply(extensive);
			this->ppAssemblageComps.push_back(entity);
		}
	}
	//cxxNameDouble eltList;
	this->eltList.add_extensive(addee.eltList, extensive);
}

#ifdef ORCHESTRA
void
cxxPPassemblage::ORCH_write_chemistry(std::ostream & chemistry_dat)
{
	chemistry_dat << std::endl << "//********* Mineral equilibria" << std::
		endl;
//
// Write minerals
//
	for (std::list < cxxPPassemblageComp >::iterator it =
		 this->ppAssemblageComps.begin(); it != this->ppAssemblageComps.end();
		 ++it)
	{
		struct phase *phase_ptr;
		int n;
		phase_ptr = phase_bsearch(it->get_name(), &n, FALSE);
		assert(phase_ptr != NULL);
		std::string phase_name(phase_ptr->name);
		std::replace(phase_name.begin(), phase_name.end(), '(', '[');
		std::replace(phase_name.begin(), phase_name.end(), ')', ']');
		chemistry_dat << "@mineral(" << phase_name << ")" << std::endl;
		chemistry_dat << "@reaction(" << phase_name << ", " << pow(10.0,
																   -phase_ptr->
																   lk);
		struct rxn_token *next_token = phase_ptr->rxn_x->token;
		next_token++;
		while (next_token->s != NULL || next_token->name != NULL)
		{
			chemistry_dat << ", " << next_token->coef;
			if (next_token->s != NULL)
			{
				chemistry_dat << ", " << next_token->s->name;
			}
			else
			{
				chemistry_dat << ", " << next_token->name;
			}
			next_token++;
		}
		chemistry_dat << ")" << std::endl;
	}
}

void
cxxPPassemblage::ORCH_write_output_vars(std::ostream & outstream)
{
	//
	//  Write ppassemblage output variables
	//
	outstream << "\tstart_ppassemblage";
	for (std::list < cxxPPassemblageComp >::iterator it =
		 this->ppAssemblageComps.begin(); it != this->ppAssemblageComps.end();
		 ++it)
	{
		outstream << "\t" << it->get_name() << ".min";
	}
	outstream << "\tend_ppassemblage";
}

void
cxxPPassemblage::ORCH_read(std::vector < std::pair < std::string,
						   double >>output_vector,
						   std::vector < std::pair < std::string,
						   double >>::iterator & it)
{
	while (it->first.compare("end_ppassemblage") != 0)
	{
		it++;
	}
}

void
cxxPPassemblage::ORCH_store_global(std::map < std::string, double >output_map)
{
	int i;
	std::map < std::string, double >::iterator it;
	for (i = 0; i < count_unknowns; i++)
	{
		// MB, ALK, CB, SOLUTION_PHASE_BOUNDARY, MU, AH2O
		switch (x[i]->type)
		{
		case PP:
			std::string name(x[i]->phase->name);
			name.append(".min");
			it = output_map.find(name);
			assert(it != output_map.end());
			x[i]->moles = it->second;
			break;
		}
	}
}
#endif
