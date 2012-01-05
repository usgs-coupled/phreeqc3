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

#include "Phreeqc.h"
#include "cxxMix.h"
#include "Exchange.h"
#include "phqalloc.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxExchange::cxxExchange(PHRQ_io *io)
	//
	// default constructor for cxxExchange
	//
:	cxxNumKeyword(io)
{
	new_def = false;
	solution_equilibria = false;
	n_solution = -999;
	pitzer_exchange_gammas = true;
}
#ifdef SKIP
cxxExchange::cxxExchange(struct exchange * exchange_ptr, PHRQ_io *io)
	//
	// constructor for cxxExchange from struct exchange
	//
:
cxxNumKeyword(io)
{
	int i;

	this->Set_description(exchange_ptr->description);
	n_user = exchange_ptr->n_user;
	n_user_end = exchange_ptr->n_user_end;
	pitzer_exchange_gammas = (exchange_ptr->pitzer_exchange_gammas == TRUE);
	for (i = 0; i < exchange_ptr->count_comps; i++)
	{
		cxxExchComp ec(&(exchange_ptr->comps[i]), this->Get_io());
		std::string str(ec.Get_formula());
		exchComps[str] = ec;
	}




}
#endif
cxxExchange::cxxExchange(const std::map < int, cxxExchange > &entities,
						 cxxMix & mix, int l_n_user, PHRQ_io *io):
cxxNumKeyword(io)
{
	this->n_user = this->n_user_end = l_n_user;
	this->pitzer_exchange_gammas = true;
//
//   Mix exchangers
//
	const std::map < int, double >&mixcomps = mix.Get_mixComps();
	std::map < int, double >::const_iterator it;
	for (it = mixcomps.begin(); it != mixcomps.end(); it++)
	{
		if (entities.find(it->first) != entities.end())
		{
			const cxxExchange *entity_ptr =
				&(entities.find(it->first)->second);
			this->add(*entity_ptr, it->second);
			this->pitzer_exchange_gammas = entity_ptr->pitzer_exchange_gammas;
		}
	}
}


cxxExchange::~cxxExchange()
{
}

bool
cxxExchange::Get_related_phases() const
{
	for (std::map < std::string, cxxExchComp >::const_iterator it =
		 this->exchComps.begin(); it != this->exchComps.end(); ++it)
	{
		if ((*it).second.Get_phase_name().size() == 0)
			continue;
		return (true);
	}
	return (false);
}

bool
cxxExchange::Get_related_rate() const
{
	for (std::map < std::string, cxxExchComp >::const_iterator it =
		 this->exchComps.begin(); it != this->exchComps.end(); ++it)
	{
		if ((*it).second.Get_rate_name().size() == 0)
			continue;
		return (true);
	}
	return (false);
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
	s_oss << "<exchange " << "\n";

	s_oss << indent1;
	s_oss << "pitzer_exchange_gammas=\"" << this->
		pitzer_exchange_gammas << "\"" << "\n";

	// components
	s_oss << indent1;
	s_oss << "<component " << "\n";
	for (std::map < std::string, cxxExchComp >::const_iterator it = exchComps.begin();
		 it != exchComps.end(); ++it)
	{
		(*it).second.dump_xml(s_oss, indent + 2);
	}

	return;
}

void
cxxExchange::dump_raw(std::ostream & s_oss, unsigned int indent, int *n_out) const
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
	int n_user_local = (n_out != NULL) ? *n_out : this->n_user;
	s_oss << "EXCHANGE_RAW       " << n_user_local << " " << this->description << "\n";

	s_oss << indent1;
	s_oss << "-new_def " << this->new_def << "\n";

	s_oss << indent1;
	s_oss << "-exchange_gammas " << this->pitzer_exchange_gammas << "\n";

	s_oss << indent1;
	s_oss << "-solution_equilibria " << this->solution_equilibria << "\n";

	s_oss << indent1;
	s_oss << "-n_solution " << this->n_solution << "\n";

	// exchComps structures
	for (std::map < std::string, cxxExchComp >::const_iterator it = exchComps.begin();
		 it != exchComps.end(); ++it)
	{
		s_oss << indent1;
		s_oss << "-component" << "\n";
		(*it).second.dump_raw(s_oss, indent + 2);
	}

	return;
}

void
cxxExchange::read_raw(CParser & parser, bool check)
{
	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(15);
		vopts.push_back("pitzer_exchange_gammas");	// 0
		vopts.push_back("component");	// 1
		vopts.push_back("exchange_gammas"); // 2
		vopts.push_back("new_def"); // 3
		vopts.push_back("solution_equilibria"); // 4
		vopts.push_back("n_solution"); // 4
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
							 PHRQ_io::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), PHRQ_io::OT_CONTINUE);
			useLastLine = false;
			break;

		case 0:				// pitzer_exchange_gammas
		case 2:				// exchange_gammas
			if (!(parser.get_iss() >> this->pitzer_exchange_gammas))
			{
				this->pitzer_exchange_gammas = false;
				parser.incr_input_error();
				parser.
					error_msg
					("Expected boolean value for pitzer_exchange_gammas.",
					 PHRQ_io::OT_CONTINUE);
			}
			pitzer_exchange_gammas_defined = true;
			useLastLine = false;
			break;
		case 1:				// component
			{
				cxxExchComp ec(this->Get_io());

				// preliminary read
				parser.set_accumulate(true);
				ec.read_raw(parser, false);
				parser.set_accumulate(false);
				std::istringstream is(parser.get_accumulated());
				CParser reread(is, this->Get_io());
				reread.set_echo_file(CParser::EO_NONE);
				reread.set_echo_stream(CParser::EO_NONE);
				if (this->exchComps.find(ec.Get_formula()) != this->exchComps.end())
				{
					cxxExchComp & comp = this->exchComps.find(ec.Get_formula())->second;
					comp.read_raw(reread, false);
				}
				else
				{
					cxxExchComp ec1(this->Get_io());
					ec1.read_raw(reread, false);
					std::string str(ec1.Get_formula());
					this->exchComps[str] = ec1;
				}
			}
			useLastLine = true;
			break;
		case 3:				// new_def
			if (!(parser.get_iss() >> this->new_def))
			{
				this->new_def = false;
				parser.incr_input_error();
				parser.
					error_msg
					("Expected boolean value for new_def.",
					 PHRQ_io::OT_CONTINUE);
			}
			useLastLine = false;
			break;
		case 4:				// solution_equilibria
			if (!(parser.get_iss() >> this->solution_equilibria))
			{
				this->solution_equilibria = false;
				parser.incr_input_error();
				parser.
					error_msg
					("Expected boolean value for solution_equilibria.",
					 PHRQ_io::OT_CONTINUE);
			}
			useLastLine = false;
			break;
		case 5:				// n_solution
			if (!(parser.get_iss() >> this->n_solution))
			{
				this->n_solution = -999;
				parser.incr_input_error();
				parser.
					error_msg
					("Expected integer value for n_solution.",
					 PHRQ_io::OT_CONTINUE);
			}
			useLastLine = false;
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
				PHRQ_io::OT_CONTINUE);
		}
	}
}
void
cxxExchange::add(const cxxExchange & addee, double extensive)
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
			(*it).second.add((*itadd).second, extensive);
		}
		else
		{
			cxxExchComp exc = (*itadd).second;
			exc.multiply(extensive);
			//exc.add(*itadd, extensive);
			this->exchComps[(*itadd).first] = exc;
		}
	}
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
	for (std::map < std::string, cxxExchComp >::iterator it = exchComps.begin();
		it != exchComps.end(); ++it)
	{
		this->totals.add_extensive((*it).second.Get_totals(), 1.0);
		this->totals.add("Charge", (*it).second.Get_charge_balance());
	}
	return;
}
bool 
cxxExchange::Get_pitzer_exchange_gammas() const
{
	return this->pitzer_exchange_gammas;
}
void 
cxxExchange::Set_pitzer_exchange_gammas(bool b)
{
	this->pitzer_exchange_gammas = b;
}

std::map < std::string, cxxExchComp > &
cxxExchange::Get_exchComps(void)
{
	return (this->exchComps);
}

const cxxNameDouble & 
cxxExchange::Get_totals() const
{
	return totals;
}
cxxExchComp *cxxExchange::ExchComp_find(std::string s)
{
	std::map<std::string, cxxExchComp>::iterator it = this->exchComps.begin();
	for ( ; it != this->exchComps.end(); it++)
	{
		cxxNameDouble nd(it->second.Get_totals());
		cxxNameDouble::iterator nd_it;
		for (nd_it = nd.begin(); nd_it != nd.end(); nd_it++)
		{
			if(nd_it->first == s)
			{
				return (&it->second);
			}
		}
	}
	return NULL;
}
std::vector<cxxExchComp *> cxxExchange::Vectorize(void) 
{
	std::vector<cxxExchComp *> vlist;
	std::map<std::string, cxxExchComp>::iterator it = exchComps.begin();
	for ( ; it != this->exchComps.end(); it++)
	{
		vlist.push_back(&it->second);
	}
	return vlist;
}