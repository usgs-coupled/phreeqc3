// Exchange.cxx: implementation of the cxxExchange class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include <iostream>				// std::cout std::cerr
#include <cassert>				// assert
#include <algorithm>			// std::sort

#include "Utils.h" // define first
#if !defined(PHREEQC_CLASS)
#define EXTERNAL extern
#include "global.h"
#else
#include "Phreeqc.h"
#endif
#include "cxxMix.h"
#include "Exchange.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include "output.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxExchange::cxxExchange()
	//
	// default constructor for cxxExchange
	//
:	cxxNumKeyword()
{
	pitzer_exchange_gammas = false;
}

cxxExchange::cxxExchange(struct exchange * exchange_ptr)
	//
	// constructor for cxxExchange from struct exchange
	//
:
cxxNumKeyword()
{
	int i;

	this->set_description(exchange_ptr->description);
	n_user = exchange_ptr->n_user;
	n_user_end = exchange_ptr->n_user_end;
	pitzer_exchange_gammas = (exchange_ptr->pitzer_exchange_gammas == TRUE);
	for (i = 0; i < exchange_ptr->count_comps; i++)
	{
		cxxExchComp ec(&(exchange_ptr->comps[i]));
		std::string str(ec.get_formula());
		exchComps[str] = ec;
	}




}
cxxExchange::cxxExchange(PHREEQC_PTR_ARG_COMMA const std::map < int, cxxExchange > &entities,
						 cxxMix & mix, int l_n_user):
cxxNumKeyword()
{
	this->n_user = this->n_user_end = l_n_user;
	this->pitzer_exchange_gammas = false;
//
//   Mix exchangers
//
	std::map < int, double >*mixcomps = mix.comps();
	std::map < int, double >::const_iterator it;
	for (it = mixcomps->begin(); it != mixcomps->end(); it++)
	{
		if (entities.find(it->first) != entities.end())
		{
			const cxxExchange *entity_ptr =
				&(entities.find(it->first)->second);
			this->add(P_INSTANCE_COMMA *entity_ptr, it->second);
			this->pitzer_exchange_gammas = entity_ptr->pitzer_exchange_gammas;
		}
#ifdef SKIP
		if (entity_ptr == NULL)
		{
			sprintf(error_string, "Exchange %d not found while mixing.",
					it->first);
			error_msg(error_string, CONTINUE);
			input_error++;
			return;
		}
#endif

	}
}

cxxExchange::cxxExchange(PHREEQC_PTR_ARG_COMMA int l_n_user)
	//
	// constructor for cxxExchange from reaction calculation
	// equivalent of xexchange_save
	//        std::list<cxxExchComp> exchComps;
	//        bool pitzer_exchange_gammas;
	//        cxxNameDouble totals;
:
cxxNumKeyword()
{
	int i;

	//this->set_description(exchange_ptr->description);
	this->n_user = l_n_user;
	this->n_user_end = l_n_user;
	this->pitzer_exchange_gammas =
		(P_INSTANCE_POINTER use.exchange_ptr->pitzer_exchange_gammas == TRUE);
	this->totals.type = cxxNameDouble::ND_ELT_MOLES;
	for (i = 0; i < P_INSTANCE_POINTER count_unknowns; i++)
	{
		if (P_INSTANCE_POINTER x[i]->type == EXCH)
		{
			cxxExchComp ec;
			//char * formula;
			ec.set_formula(P_INSTANCE_POINTER x[i]->exch_comp->formula);
			//double moles;
			ec.set_moles(0.0);
			//cxxNameDouble formula_totals;
			ec.set_formula_totals(P_INSTANCE_POINTER x[i]->exch_comp->formula_totals);
			//cxxNameDouble totals; see below
			//double la;
			ec.set_la(P_INSTANCE_POINTER x[i]->master[0]->s->la);
			//double charge_balance; see below
			//char   *phase_name;
			ec.set_phase_name(P_INSTANCE_POINTER x[i]->exch_comp->phase_name);
			//double phase_proportion;
			ec.set_phase_proportion(P_INSTANCE_POINTER x[i]->exch_comp->phase_proportion);
			//char   *rate_name;
			ec.set_rate_name(P_INSTANCE_POINTER x[i]->exch_comp->rate_name);
			//double formula_z;
			ec.set_formula_z(P_INSTANCE_POINTER x[i]->exch_comp->formula_z);

			// calculate charge and totals
			P_INSTANCE_POINTER count_elts = 0;
			P_INSTANCE_POINTER paren_count = 0;
			double charge = 0.0;
			int j;
			for (j = 0; j < P_INSTANCE_POINTER count_species_list; j++)
			{
				if (P_INSTANCE_POINTER species_list[j].master_s == P_INSTANCE_POINTER x[i]->master[0]->s)
				{
					P_INSTANCE_POINTER add_elt_list(P_INSTANCE_POINTER species_list[j].s->next_elt,
								 P_INSTANCE_POINTER species_list[j].s->moles);
					charge += P_INSTANCE_POINTER species_list[j].s->moles * P_INSTANCE_POINTER species_list[j].s->z;
				}
			}
			// Keep exchanger related to phase even if none currently in solution
			if (P_INSTANCE_POINTER x[i]->exch_comp->phase_name != NULL && P_INSTANCE_POINTER count_elts == 0)
			{
				P_INSTANCE_POINTER add_elt_list(P_INSTANCE_POINTER x[i]->master[0]->s->next_elt, 1e-20);
			}
			//double charge_balance
			ec.set_charge_balance(charge);
			//cxxNameDouble totals;
			if (P_INSTANCE_POINTER count_elts > 0)
			{
				qsort(P_INSTANCE_POINTER elt_list, (size_t) P_INSTANCE_POINTER count_elts,
					  (size_t) sizeof(struct elt_list), P_INSTANCE_POINTER elt_list_compare);
				P_INSTANCE_POINTER elt_list_combine();
			}
			ec.set_totals(P_INSTANCE_POINTER elt_list, P_INSTANCE_POINTER count_elts);

			// add to comp list
			std::string str(ec.get_formula());
			this->exchComps[str] = ec;
		}
	}
}

cxxExchange::~cxxExchange()
{
}

bool
cxxExchange::get_related_phases()
{
	for (std::map < std::string, cxxExchComp >::const_iterator it =
		 this->exchComps.begin(); it != this->exchComps.end(); ++it)
	{
		if ((*it).second.get_phase_name().size() == 0)
			continue;
		return (true);
	}
	return (false);
}

bool
cxxExchange::get_related_rate()
{
	for (std::map < std::string, cxxExchComp >::const_iterator it =
		 this->exchComps.begin(); it != this->exchComps.end(); ++it)
	{
		if ((*it).second.get_rate_name().size() == 0)
			continue;
		return (true);
	}
	return (false);
}

struct exchange *
cxxExchange::cxxExchange2exchange(PHREEQC_PTR_ARG)
		//
		// Builds a exchange structure from instance of cxxExchange
		//
{
	struct exchange *exchange_ptr = P_INSTANCE_POINTER exchange_alloc();

	exchange_ptr->description = P_INSTANCE_POINTER string_duplicate (this->get_description().c_str());
	exchange_ptr->n_user = this->n_user;
	exchange_ptr->n_user_end = this->n_user_end;
	exchange_ptr->new_def = FALSE;
	exchange_ptr->solution_equilibria = FALSE;
	exchange_ptr->n_solution = -2;
	exchange_ptr->related_phases = (int) this->get_related_phases();
	exchange_ptr->related_rate = (int) this->get_related_rate();
	exchange_ptr->pitzer_exchange_gammas = (int) this->pitzer_exchange_gammas;
	exchange_ptr->count_comps = (int) this->exchComps.size();
	exchange_ptr->comps = (struct exch_comp *) P_INSTANCE_POINTER free_check_null(exchange_ptr->comps);
	exchange_ptr->comps = cxxExchComp::cxxExchComp2exch_comp(P_INSTANCE_COMMA this->exchComps);
	return (exchange_ptr);
}

void
cxxExchange::dump_xml(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing exchange message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// Exchange element and attributes
	s_oss << indent0;
	s_oss << "<exchange " << std::endl;

	s_oss << indent1;
	s_oss << "pitzer_exchange_gammas=\"" << this->
		pitzer_exchange_gammas << "\"" << std::endl;

	// components
	s_oss << indent1;
	s_oss << "<component " << std::endl;
	for (std::map < std::string, cxxExchComp >::const_iterator it = exchComps.begin();
		 it != exchComps.end(); ++it)
	{
		(*it).second.dump_xml(s_oss, indent + 2);
	}

	return;
}

void
cxxExchange::dump_raw(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing exchange message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// Exchange element and attributes
	s_oss << indent0;
	s_oss << "EXCHANGE_RAW       " << this->n_user << " " << this->
		description << std::endl;

	s_oss << indent1;
	s_oss << "-pitzer_exchange_gammas " << this->
		pitzer_exchange_gammas << std::endl;

	// exchComps structures
	for (std::map < std::string, cxxExchComp >::const_iterator it = exchComps.begin();
		 it != exchComps.end(); ++it)
	{
		s_oss << indent1;
		s_oss << "-component" << std::endl;
		(*it).second.dump_raw(s_oss, indent + 2);
	}

	return;
}

void
cxxExchange::read_raw(PHREEQC_PTR_ARG_COMMA CParser & parser, bool check)
{
	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(15);
		vopts.push_back("pitzer_exchange_gammas");	// 0
		vopts.push_back("component");	// 1
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;
	bool useLastLine(false);

	// Read exchange number and description
	this->read_number_description(parser);

	opt_save = CParser::OPT_ERROR;
	bool pitzer_exchange_gammas_defined(false);

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
		switch (opt)
		{
		case CParser::OPT_EOF:
			break;
		case CParser::OPT_KEYWORD:
			break;
		case CParser::OPT_DEFAULT:
		case CParser::OPT_ERROR:
			opt = CParser::OPT_EOF;
			parser.error_msg("Unknown input in EXCH_COMP_RAW keyword.",
							 CParser::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
			useLastLine = false;
			break;

		case 0:				// pitzer_exchange_gammas
			if (!(parser.get_iss() >> this->pitzer_exchange_gammas))
			{
				this->pitzer_exchange_gammas = false;
				parser.incr_input_error();
				parser.
					error_msg
					("Expected boolean value for pitzer_exchange_gammas.",
					 CParser::OT_CONTINUE);
			}
			pitzer_exchange_gammas_defined = true;
			useLastLine = false;
			break;
		case 1:				// component
			{
				cxxExchComp ec;

				// preliminary read
#ifdef SKIP
				std::istream::pos_type pos = parser.tellg();
				CParser::ECHO_OPTION eo = parser.get_echo_file();
				parser.set_echo_file(CParser::EO_NONE);
				CParser::ECHO_OPTION eo_s = parser.get_echo_stream();
				parser.set_echo_stream(CParser::EO_NONE);
				ec.read_raw(parser, false);
				parser.set_echo_file(eo);
				parser.set_echo_stream(eo_s);
				parser.seekg(pos).clear();
				parser.seekg(pos);

				if (this->exchComps.find(ec.get_formula()) != this->exchComps.end())
				{
					cxxExchComp & comp = this->exchComps.find(ec.get_formula())->second;

					comp.read_raw(parser, false);
				}
				else
				{
					cxxExchComp ec1;
					ec1.read_raw(parser, false);
					std::string str(ec1.get_formula());
					this->exchComps[str] = ec1;
				}
#endif
				parser.set_accumulate(true);
				ec.read_raw(P_INSTANCE_COMMA parser, false);
				parser.set_accumulate(false);
				std::istringstream is(parser.get_accumulated());
				CParser reread(P_INSTANCE_COMMA is);
				reread.set_echo_file(CParser::EO_NONE);
				reread.set_echo_stream(CParser::EO_NONE);
				if (this->exchComps.find(ec.get_formula()) != this->exchComps.end())
				{
					cxxExchComp & comp = this->exchComps.find(ec.get_formula())->second;
					comp.read_raw(P_INSTANCE_COMMA reread, false);
				}
				else
				{
					cxxExchComp ec1;
					ec1.read_raw(P_INSTANCE_COMMA reread, false);
					std::string str(ec1.get_formula());
					this->exchComps[str] = ec1;
				}
			}
			useLastLine = true;
			break;
		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	// members that must be defined
	if (check)
	{
		if (pitzer_exchange_gammas_defined == false)
		{
			parser.incr_input_error();
			parser.
				error_msg
				("Pitzer_exchange_gammsa not defined for EXCHANGE_RAW input.",
				CParser::OT_CONTINUE);
		}
	}
}
#ifdef SKIP
void
cxxExchange::add(const cxxExchange & addee, double extensive)
		//
		// Add existing exchange to "this" exchange
		//
{
	//std::list<cxxExchComp> exchComps;
	// exchComps
	if (extensive == 0.0)
		return;
	for (std::list < cxxExchComp >::const_iterator itadd =
		 addee.exchComps.begin(); itadd != addee.exchComps.end(); ++itadd)
	{
		bool found = false;
		for (std::list < cxxExchComp >::iterator it = this->exchComps.begin();
			 it != this->exchComps.end(); ++it)
		{
			if (it->get_formula() == itadd->get_formula())
			{
				it->add((*itadd), extensive);
				found = true;
				break;
			}
		}
		if (!found)
		{
			cxxExchComp exc = *itadd;
			exc.multiply(extensive);
			//exc.add(*itadd, extensive);
			this->exchComps.push_back(exc);
		}
	}
	//bool pitzer_exchange_gammas;
	this->pitzer_exchange_gammas = addee.pitzer_exchange_gammas;
}
#endif
void
cxxExchange::add(PHREEQC_PTR_ARG_COMMA const cxxExchange & addee, double extensive)
		//
		// Add existing exchange to "this" exchange
		//
{
	// exchComps
	if (extensive == 0.0)
		return;
	for (std::map < std::string, cxxExchComp >::const_iterator itadd =
		 addee.exchComps.begin(); itadd != addee.exchComps.end(); ++itadd)
	{
		std::map < std::string, cxxExchComp >::iterator it = this->exchComps.find((*itadd).first);
		if (it != this->exchComps.end())
		{
			(*it).second.add(P_INSTANCE_COMMA (*itadd).second, extensive);
		//bool found = false;
		//for (std::list < cxxExchComp >::iterator it = this->exchComps.begin();
		//	 it != this->exchComps.end(); ++it)
		//{
		//	if (it->get_formula() == itadd->get_formula())
		//	{
		//		it->add((*itadd), extensive);
		//		found = true;
		//		break;
		//	}
		//}
		/*if (!found)*/

		}
		else
		{
			cxxExchComp exc = (*itadd).second;
			exc.multiply(extensive);
			//exc.add(*itadd, extensive);
			this->exchComps[(*itadd).first] = exc;
		}
	}
	//bool pitzer_exchange_gammas;
	this->pitzer_exchange_gammas = addee.pitzer_exchange_gammas;
}
#ifdef USE_MPI
/* ---------------------------------------------------------------------- */
void
cxxExchange::mpi_pack(std::vector < int >&ints,
					  std::vector < double >&doubles)
/* ---------------------------------------------------------------------- */
{
	/* int n_user; */
	ints.push_back(this->n_user);

	ints.push_back((int) this->pitzer_exchange_gammas);
	ints.push_back((int) this->exchComps.size());
	for (std::map < std::string, cxxExchComp >::iterator it = this->exchComps.begin();
		 it != this->exchComps.end(); it++)
	{
		(*it).second.mpi_pack(ints, doubles);
	}
}

/* ---------------------------------------------------------------------- */
void
cxxExchange::mpi_unpack(int *ints, int *ii, double *doubles, int *dd)
/* ---------------------------------------------------------------------- */
{
	int i = *ii;
	int d = *dd;
	/* int n_user; */
	this->n_user = ints[i++];
	this->n_user_end = this->n_user;
	this->description = " ";


	this->pitzer_exchange_gammas = (ints[i++] == TRUE);
	int count = ints[i++];
	this->exchComps.clear();
	for (int n = 0; n < count; n++)
	{
		cxxExchComp ec;
		ec.mpi_unpack(ints, &i, doubles, &d);
		std::string str(ec.get_formula());
		this->exchComps[str] = ec;
	}
	*ii = i;
	*dd = d;
}
#endif
void
cxxExchange::totalize()
{
	this->totals.clear();
	// component structures
	for (std::map < std::string, cxxExchComp >::const_iterator it = exchComps.begin();
		it != exchComps.end(); ++it)
	{
		this->totals.add_extensive((*it).second.get_totals(), 1.0);
		this->totals.add("Charge", (*it).second.get_charge_balance());
	}
	return;
}
bool 
cxxExchange::get_pitzer_exchange_gammas()
{
	return this->pitzer_exchange_gammas;
}
void 
cxxExchange::set_pitzer_exchange_gammas(bool b)
{
	this->pitzer_exchange_gammas = b;
}

std::map < std::string, cxxExchComp > &
cxxExchange::get_exchComps(void)
{
	return (this->exchComps);
}
const cxxNameDouble & 
cxxExchange::get_totals() const
{
	return totals;
}
