// Solution.cxx: implementation of the cxxSolution class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#ifdef USE_MPI
//MPICH seems to require mpi.h to be first
#include <mpi.h>
#endif
#include "Utils.h"				// define first
#include "Solution.h"
#define EXTERNAL extern
#include "global.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include "output.h"
#include <cassert>				// assert
#include <algorithm>			// std::sort


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


cxxSolution::cxxSolution()
	//
	// default constructor for cxxSolution 
	//
:	cxxNumKeyword()
{
	this->tc = 25.0;
	this->ph = 7.0;
	this->pe = 4.0;
	this->mu = 1e-7;
	this->ah2o = 1.0;
	this->total_h = 111.1;
	this->total_o = 55.55;
	this->cb = 0.0;
	this->mass_water = 1.0;
	this->total_alkalinity = 0.0;
	this->totals.type = cxxNameDouble::ND_ELT_MOLES;
	this->master_activity.type = cxxNameDouble::ND_SPECIES_LA;
	this->species_gamma.type = cxxNameDouble::ND_SPECIES_GAMMA;
}

cxxSolution::cxxSolution(struct solution * solution_ptr)
	//
	// constructor for cxxSolution from struct solution
	//
:
cxxNumKeyword(),
totals(solution_ptr->totals),
master_activity(solution_ptr->master_activity,
				solution_ptr->count_master_activity,
				cxxNameDouble::ND_SPECIES_LA),
species_gamma(solution_ptr->species_gamma, solution_ptr->count_species_gamma,
			  cxxNameDouble::ND_SPECIES_GAMMA), isotopes(solution_ptr)
{

	this->set_description(solution_ptr->description);
	this->n_user = solution_ptr->n_user;
	this->n_user_end = solution_ptr->n_user_end;
	this->tc = solution_ptr->tc;
	this->ph = solution_ptr->ph;
	this->pe = solution_ptr->solution_pe;
	this->mu = solution_ptr->mu;
	this->ah2o = solution_ptr->ah2o;
	this->total_h = solution_ptr->total_h;
	this->total_o = solution_ptr->total_o;
	this->cb = solution_ptr->cb;
	this->mass_water = solution_ptr->mass_water;
	this->total_alkalinity = solution_ptr->total_alkalinity;

	// Totals filled in constructor, just save description and moles 

	// Isotopes
	/*
	   for (i = 0; i < solution_ptr->count_isotopes; i++) {
	   cxxSolutionIsotope iso(&solution_ptr->isotopes[i]);
	   isotopes.push_back(iso);
	   }
	 */
	// Master_activity in constructor
	// Species_gamma in constructor
}


cxxSolution::cxxSolution(const std::map < int, cxxSolution > &solutions,
						 cxxMix & mix, int n_user)
//
// constructor for cxxSolution from mixture of solutions
//
	:
cxxNumKeyword()
{

//
//   Zero out solution data
//
	this->zero();
	this->n_user = this->n_user_end = n_user;
//
//   Mix solutions
//
	std::map < int, double >*mixcomps = mix.comps();
	std::map < int, double >::const_iterator it;
	for (it = mixcomps->begin(); it != mixcomps->end(); it++)
	{
		std::map < int, cxxSolution >::const_iterator sol =
			solutions.find(it->first);
		if (sol == solutions.end())
		{
			sprintf(error_string,
					"Solution %d not found in mix_cxxSolutions.", it->first);
			error_msg(error_string, CONTINUE);
			input_error++;
		}
		else
		{
			const cxxSolution *cxxsoln_ptr1 = &(sol->second);
			this->add(*cxxsoln_ptr1, it->second);
		}
	}
}

cxxSolution::cxxSolution(int n_user)
		//
		// constructor for cxxSolution from results of calculation
		// does not work in phast because phast uses only the total molalities
		// of elements in buffer, not individual redox states.
	:
cxxNumKeyword()
{

	//this->set_description none;
	this->n_user = n_user;
	this->n_user_end = n_user;
	this->tc = tc_x;
	this->ph = ph_x;
	this->pe = solution_pe_x;
	this->mu = mu_x;
	this->ah2o = ah2o_x;
	this->total_h = total_h_x;
	this->total_o = total_o_x;
	this->cb = cb_x;
	this->mass_water = mass_water_aq_x;
	this->total_alkalinity =::total_alkalinity;
	this->totals.type = cxxNameDouble::ND_ELT_MOLES;
	this->master_activity.type = cxxNameDouble::ND_SPECIES_LA;
	this->species_gamma.type = cxxNameDouble::ND_SPECIES_GAMMA;
	/*
	 * Add in minor isotopes if initial solution calculation
	 */
	int i;
	if (initial_solution_isotopes == TRUE)
	{
		struct master *master_ptr, *master_i_ptr;
		for (i = 0; i < count_master_isotope; i++)
		{
			if (master_isotope[i]->moles > 0)
			{
				master_i_ptr = master_bsearch(master_isotope[i]->name);
				master_ptr = master_isotope[i]->elt->master;
				if (master_isotope[i]->minor_isotope == TRUE)
				{
					master_i_ptr->total = master_isotope[i]->moles;
					if (master_ptr->total > 0)
					{
						master_i_ptr->s->la =
							master_ptr->s->la +
							log10(master_i_ptr->total / master_ptr->total);
					}
					else
					{
						master_i_ptr->s->la = master_ptr->s->la;
					}
				}
				else if (master_isotope[i]->minor_isotope == FALSE
						 && master_ptr->s != s_hplus
						 && master_ptr->s != s_h2o)
				{
					if (master_ptr->s->secondary != NULL)
					{
						master_ptr->s->secondary->total =
							master_isotope[i]->moles;
					}
					else
					{
						master_ptr->s->primary->total =
							master_isotope[i]->moles;
					}
				}
			}
		}
	}
	/*
	   cxxNameDouble totals;
	   cxxNameDouble master_activity;
	   cxxNameDouble species_gamma;
	   cxxSolutionIsotopeList isotopes;
	 */

// totals and master_activity
	for (i = 0; i < count_master; i++)
	{
		if (master[i]->s->type == EX ||
			master[i]->s->type == SURF || master[i]->s->type == SURF_PSI)
			continue;
		if (master[i]->s == s_hplus)
			continue;
		if (master[i]->s == s_h2o)
			continue;
		if (master[i]->in != FALSE)
		{
			this->master_activity[master[i]->elt->name] = master[i]->s->la;
		}
		if (master[i]->total <= MIN_TOTAL)
		{
			master[i]->total = 0.0;
			master[i]->total_primary = 0.0;
			continue;
		}
		this->totals[master[i]->elt->name] = master[i]->total;
	}

// species_gammas for Pitzer 
	if (pitzer_model == TRUE)
	{
		int j;
		for (j = 0; j < count_s; j++)
		{
			if (s[j]->lg != 0.0)
			{
				this->species_gamma[s[j]->name] = s[j]->lg;
			}
		}
	}

// Save isotope data

	if (count_isotopes_x > 0)
	{
		for (i = 0; i < count_isotopes_x; i++)
		{
			cxxSolutionIsotope cxxiso;
			cxxiso.set_isotope_number(isotopes_x[i].isotope_number);
			cxxiso.set_elt_name(isotopes_x[i].elt_name);
			cxxiso.set_isotope_name(isotopes_x[i].isotope_name);
			cxxiso.set_total(isotopes_x[i].master->total);
			if (isotopes_x[i].master == s_hplus->secondary)
			{
				cxxiso.set_total(2 * mass_water_aq_x / gfw_water);
			}
			if (isotopes_x[i].master == s_h2o->secondary)
			{
				cxxiso.set_total(mass_water_aq_x / gfw_water);
			}
			// cxxiso.ratio                 
			// cxxiso.ratio_uncertainty
			// cxxiso.ratio_uncertainty_defined 
			this->isotopes.push_back(cxxiso);
		}
	}
}

cxxSolution::~cxxSolution()
{
}
struct solution *
cxxSolution::cxxSolution2solution()
		//
		// Builds a solution structure from instance of cxxSolution 
		//
{

	struct solution *solution_ptr = solution_alloc();

	solution_ptr->description = this->get_description();
	solution_ptr->n_user = this->n_user;
	solution_ptr->n_user_end = this->n_user_end;
	solution_ptr->new_def = FALSE;
	solution_ptr->tc = this->tc;
	solution_ptr->ph = this->ph;
	solution_ptr->solution_pe = this->pe;
	solution_ptr->mu = this->mu;
	solution_ptr->ah2o = this->ah2o;
	solution_ptr->total_h = this->total_h;
	solution_ptr->total_o = this->total_o;
	solution_ptr->cb = this->cb;
	solution_ptr->mass_water = this->mass_water;
	solution_ptr->total_alkalinity = this->total_alkalinity;
	solution_ptr->density = 1.0;
	solution_ptr->units = moles_per_kilogram_string;
	solution_ptr->default_pe = 0;
	// pe_data

	// totals
	solution_ptr->totals =
		(struct conc *) free_check_null(solution_ptr->totals);
	solution_ptr->totals = this->totals.conc();

	// master_activity
	solution_ptr->master_activity =
		(struct master_activity *) free_check_null(solution_ptr->
												   master_activity);
	solution_ptr->master_activity = this->master_activity.master_activity();
	solution_ptr->count_master_activity =
		(int) this->master_activity.size() + 1;

	// species_gamma
	solution_ptr->species_gamma = this->species_gamma.master_activity();
	solution_ptr->count_species_gamma = (int) this->species_gamma.size();

	// isotopes
	solution_ptr->isotopes =
		(struct isotope *) free_check_null(solution_ptr->isotopes);
	//solution_ptr->isotopes = cxxSolutionIsotope::list2isotope(this->isotopes);
	solution_ptr->isotopes = this->isotopes.cxxSolutionIsotopeList2isotope();
	solution_ptr->count_isotopes = (int) this->isotopes.size();

	return (solution_ptr);
}

void
cxxSolution::dump_xml(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing solution message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);

	// Solution element and attributes
	s_oss << indent0;
	s_oss << "<solution " << std::endl;

	//s_oss << indent1;
	//s_oss << "soln_new_def=\"" << this->new_def << "\"" << std::endl;

	s_oss << indent1;
	s_oss << "soln_n_user=\"" << this->n_user << "\" " << std::endl;

	s_oss << indent1;
	s_oss << "soln_description=\"" << this->description << "\"" << std::endl;

	s_oss << indent1;
	s_oss << "soln_tc=\"" << this->tc << "\"" << std::endl;

	s_oss << indent1;
	s_oss << "soln_ph=\"" << this->ph << "\"" << std::endl;

	s_oss << indent1;
	s_oss << "soln_solution_pe=\"" << this->pe << "\"" << std::endl;

	s_oss << indent1;
	s_oss << "soln_mu=\"" << this->mu << "\"" << std::endl;

	s_oss << indent1;
	s_oss << "soln_ah2o=\"" << this->ah2o << "\"" << std::endl;

	s_oss << indent1;
	s_oss << "soln_total_h=\"" << this->total_h << "\"" << std::endl;

	s_oss << indent1;
	s_oss << "soln_total_o=\"" << this->total_o << "\"" << std::endl;

	s_oss << indent1;
	s_oss << "soln_cb=\"" << this->cb << "\"" << std::endl;

	s_oss << indent1;
	s_oss << "soln_mass_water=\"" << this->mass_water << "\"" << std::endl;

	s_oss << indent1;
	s_oss << "soln_total_alkalinity=\"" << this->
		total_alkalinity << "\"" << std::endl;

	s_oss << indent1;
	s_oss << "\">" << std::endl;

	// soln_total conc structures
	this->totals.dump_xml(s_oss, indent + 1);
	/*
	   {
	   for (std::map <char *, double, CHARSTAR_LESS>::const_iterator it = totals.begin(); it != totals.end(); ++it) {
	   s_oss << indent1;
	   s_oss << "<soln_total";
	   s_oss << " conc_desc=\"" << it->first << "\"";
	   s_oss << " conc_moles=\"" << it->second << "\"" ;
	   s_oss << "\">" << std::endl;
	   }
	   }
	 */
	// master_activity map
	this->master_activity.dump_xml(s_oss, indent + 1);
	/*
	   {
	   for (std::map <char *, double>::const_iterator it = master_activity.begin(); it != master_activity.end(); ++it) {
	   s_oss << indent1;
	   s_oss << "<soln_m_a";
	   s_oss << " m_a_desc=\"" << it->first << "\"" ;
	   s_oss << " m_a_la=\"" << it->second << "\"" ;
	   s_oss << "\">" << std::endl;
	   }
	   }
	 */
	// species_gamma map
	this->species_gamma.dump_xml(s_oss, indent + 1);
	/*
	   {
	   for (std::map <char *, double>::const_iterator it = species_gamma.begin(); it != species_gamma.end(); ++it) {
	   s_oss << indent1;
	   s_oss << "<soln_s_g";
	   s_oss << " m_a_desc=\"" << it->first << "\"" ;
	   s_oss << " m_a_la=\"" << it->second << "\"" ;
	   s_oss << "\">" << std::endl;
	   }
	   }
	 */

	for (std::list < cxxSolutionIsotope >::const_iterator it =
		 this->isotopes.begin(); it != isotopes.end(); ++it)
	{
		it->dump_xml(s_oss, indent + 1);
	}

	// End of solution
	s_oss << indent0;
	s_oss << "</solution>" << std::endl;

	return;
}

void
cxxSolution::dump_raw(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing solution message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// Solution element and attributes
	s_oss << indent0;
	s_oss << "SOLUTION_RAW       " << this->n_user << " " << this->
		description << std::endl;

	s_oss << indent1;
	s_oss << "-temp              " << this->tc << std::endl;

	s_oss << indent1;
	s_oss << "-pH                " << this->ph << std::endl;

	s_oss << indent1;
	s_oss << "-pe                " << this->pe << std::endl;

	// new identifier
	s_oss << indent1;
	s_oss << "-mu                " << this->mu << std::endl;

	// new identifier
	s_oss << indent1;
	s_oss << "-ah2o              " << this->ah2o << std::endl;

	// new identifier
	s_oss << indent1;
	s_oss << "-total_h           " << this->total_h << std::endl;

	// new identifier
	s_oss << indent1;
	s_oss << "-total_o           " << this->total_o << std::endl;

	// new identifier
	s_oss << indent1;
	s_oss << "-cb                " << this->cb << std::endl;

	// new identifier
	s_oss << indent1;
	s_oss << "-mass_water        " << this->mass_water << std::endl;

	// new identifier
	s_oss << indent1;
	s_oss << "-total_alkalinity  " << this->total_alkalinity << std::endl;

	// soln_total conc structures
	s_oss << indent1;
	s_oss << "-totals" << std::endl;
	this->totals.dump_raw(s_oss, indent + 2);
	/*
	   for (std::map <char *, double, CHARSTAR_LESS>::const_iterator it = totals.begin(); it != totals.end(); ++it) {
	   s_oss << indent2;
	   s_oss << it->first << "   " <<  it->second << std::endl;
	   }
	 */

	// master_activity map
	s_oss << indent1;
	s_oss << "-activities" << std::endl;
	this->master_activity.dump_raw(s_oss, indent + 2);
	/*
	   {
	   for (std::map <char *, double>::const_iterator it = master_activity.begin(); it != master_activity.end(); ++it) {
	   s_oss << indent2;
	   s_oss << it->first << "   " << it->second << std::endl;
	   }
	   }
	 */
	// species_gamma map
	s_oss << indent1;
	s_oss << "-gammas" << std::endl;
	this->species_gamma.dump_raw(s_oss, indent + 2);
	/*
	   {
	   {
	   for (std::map <char *, double>::const_iterator it = species_gamma.begin(); it != species_gamma.end(); ++it) {
	   s_oss << indent2;
	   s_oss << it->first << "   " << it->second << std::endl;
	   }
	   }
	   }
	 */
	// Isotopes
	s_oss << indent1;
	s_oss << "-Isotopes" << std::endl;
	{
		for (std::list < cxxSolutionIsotope >::const_iterator it =
			 this->isotopes.begin(); it != isotopes.end(); ++it)
		{
			it->dump_raw(s_oss, indent + 2);
		}
	}

	return;
}

void
cxxSolution::read_raw(CParser & parser)
{
	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(21);
		vopts.push_back("totals");	// 0 
		vopts.push_back("activities");	// 1 
		vopts.push_back("gammas");	// 2 
		vopts.push_back("isotopes");	// 3 
		vopts.push_back("temp");	// 4 
		vopts.push_back("tc");	// 5 
		vopts.push_back("temperature");	// 6 
		vopts.push_back("ph");	// 7 
		vopts.push_back("pe");	// 8 
		vopts.push_back("mu");	// 9 
		vopts.push_back("ionic_strength");	// 10
		vopts.push_back("ah2o");	// 11
		vopts.push_back("activity_water");	// 12
		vopts.push_back("total_h");	// 13
		vopts.push_back("total_o");	// 14
		vopts.push_back("mass_water");	// 15
		vopts.push_back("mass_h2o");	// 16
		vopts.push_back("total_alkalinity");	// 17
		vopts.push_back("total_alk");	// 18
		vopts.push_back("cb");	// 19
		vopts.push_back("charge_balance");	// 20
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;

	// Read solution number and description
	this->read_number_description(parser);

	opt_save = CParser::OPT_ERROR;
	bool tc_defined(false);
	bool ph_defined(false);
	bool pe_defined(false);
	bool mu_defined(false);
	bool ah2o_defined(false);
	bool total_h_defined(false);
	bool total_o_defined(false);
	bool cb_defined(false);
	bool mass_water_defined(false);
	bool total_alkalinity_defined(false);

	for (;;)
	{
		int opt = parser.get_option(vopts, next_char);
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
			parser.error_msg("Unknown input in SOLUTION_RAW keyword.",
							 CParser::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
			continue;

		case 0:				// totals
			if (this->totals.read_raw(parser, next_char) !=
				CParser::PARSER_OK)
			{
				parser.incr_input_error();
				parser.
					error_msg("Expected element name and moles for totals.",
							  CParser::OT_CONTINUE);
			}
			opt_save = 0;
			break;

		case 1:				// activities
			if (this->master_activity.read_raw(parser, next_char) !=
				CParser::PARSER_OK)
			{
				parser.incr_input_error();
				parser.
					error_msg
					("Expected species name and log activity for activities.",
					 CParser::OT_CONTINUE);
			}
			opt_save = 1;
			break;

		case 2:				// gammas
			if (this->species_gamma.read_raw(parser, next_char) !=
				CParser::PARSER_OK)
			{
				parser.incr_input_error();
				parser.
					error_msg
					("Expected species name and activity coefficient for gammas.",
					 CParser::OT_CONTINUE);
			}
			opt_save = 2;
			break;

		case 3:				// isotopes
			{
				cxxSolutionIsotope iso;
				if (iso.read_raw(parser) != CParser::PARSER_OK)
				{
					parser.incr_input_error();
					parser.error_msg("Expected data for isotopes.",
									 CParser::OT_CONTINUE);
				}
				else
				{
					if (iso.get_isotope_name() != NULL)
					{
						this->isotopes.push_back(iso);
					}
				}
			}
			opt_save = 3;
			break;

		case 4:				// temp
		case 5:				// tc
		case 6:				// temperature                  
			if (!(parser.get_iss() >> this->tc))
			{
				this->tc = 25.0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for temperature.",
								 CParser::OT_CONTINUE);
			}
			tc_defined = true;
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 7:				// ph
			if (!(parser.get_iss() >> this->ph))
			{
				this->ph = 7.0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for pH.",
								 CParser::OT_CONTINUE);
			}
			ph_defined = true;
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 8:				// pe
			if (!(parser.get_iss() >> this->pe))
			{
				this->pe = 4.0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for pe.",
								 CParser::OT_CONTINUE);
			}
			pe_defined = true;
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 9:				// mu
		case 10:				// ionic_strength
			if (!(parser.get_iss() >> this->mu))
			{
				this->mu = 1e-7;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for ionic strength.",
								 CParser::OT_CONTINUE);
			}
			mu_defined = true;
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 11:				// ah2o
		case 12:				// activity_water
			if (!(parser.get_iss() >> this->ah2o))
			{
				this->ah2o = 1.0;
				parser.incr_input_error();
				parser.
					error_msg("Expected numeric value for activity of water.",
							  CParser::OT_CONTINUE);
			}
			ah2o_defined = true;
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 13:				// total_h
			if (!(parser.get_iss() >> this->total_h))
			{
				this->total_h = 111.1;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for total hydrogen.",
								 CParser::OT_CONTINUE);
			}
			total_h_defined = true;
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 14:				// total_o
			if (!(parser.get_iss() >> this->total_o))
			{
				this->total_o = 55.55;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for total oxygen.",
								 CParser::OT_CONTINUE);
			}
			total_o_defined = true;
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 15:				// mass_water
		case 16:				// mass_h2o
			if (!(parser.get_iss() >> this->mass_water))
			{
				this->mass_water = 1.0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for mass of water.",
								 CParser::OT_CONTINUE);
			}
			mass_water_defined = true;
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 17:				// total_alkalinity
		case 18:				// total_alk
			if (!(parser.get_iss() >> this->total_alkalinity))
			{
				this->total_alkalinity = 0;
				parser.incr_input_error();
				parser.
					error_msg("Expected numeric value for total_alkalinity.",
							  CParser::OT_CONTINUE);
			}
			total_alkalinity_defined = true;
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 19:				// cb
		case 20:				// charge_balance
			if (!(parser.get_iss() >> this->cb))
			{
				this->cb = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for charge balance.",
								 CParser::OT_CONTINUE);
			}
			cb_defined = true;
			opt_save = CParser::OPT_DEFAULT;
			break;

		}

		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	// all members must be defined
	if (tc_defined == false)
	{
		parser.incr_input_error();
		parser.error_msg("Temp not defined for SOLUTION_RAW input.",
						 CParser::OT_CONTINUE);
	}
	if (ph_defined == false)
	{
		parser.incr_input_error();
		parser.error_msg("pH not defined for SOLUTION_RAW input.",
						 CParser::OT_CONTINUE);
	}
	if (pe_defined == false)
	{
		parser.incr_input_error();
		parser.error_msg("pe not defined for SOLUTION_RAW input.",
						 CParser::OT_CONTINUE);
	}
	if (mu_defined == false)
	{
		parser.incr_input_error();
		parser.error_msg("Ionic strength not defined for SOLUTION_RAW input.",
						 CParser::OT_CONTINUE);
	}
	if (ah2o_defined == false)
	{
		parser.incr_input_error();
		parser.
			error_msg("Activity of water not defined for SOLUTION_RAW input.",
					  CParser::OT_CONTINUE);
	}
	if (total_h_defined == false)
	{
		parser.incr_input_error();
		parser.error_msg("Total hydrogen not defined for SOLUTION_RAW input.",
						 CParser::OT_CONTINUE);
	}
	if (total_o_defined == false)
	{
		parser.incr_input_error();
		parser.error_msg("Total oxygen not defined for SOLUTION_RAW input.",
						 CParser::OT_CONTINUE);
	}
	if (cb_defined == false)
	{
		parser.incr_input_error();
		parser.error_msg("Charge balance not defined for SOLUTION_RAW input.",
						 CParser::OT_CONTINUE);
	}
	if (mass_water_defined == false)
	{
		parser.incr_input_error();
		parser.error_msg("Temp not defined for SOLUTION_RAW input.",
						 CParser::OT_CONTINUE);
	}
	if (total_alkalinity_defined == false)
	{
		parser.incr_input_error();
		parser.
			error_msg("Total alkalinity not defined for SOLUTION_RAW input.",
					  CParser::OT_CONTINUE);
	}
	return;
}

void
cxxSolution::zero()
{
	this->tc = 0.0;
	this->ph = 0.0;
	this->pe = 0.0;
	this->mu = 0.0;
	this->ah2o = 0.0;
	this->total_h = 0.0;
	this->total_o = 0.0;
	this->cb = 0.0;
	this->mass_water = 0.0;
	this->total_alkalinity = 0.0;
	this->totals.type = cxxNameDouble::ND_ELT_MOLES;
	this->master_activity.type = cxxNameDouble::ND_SPECIES_LA;
	this->species_gamma.type = cxxNameDouble::ND_SPECIES_GAMMA;
}

void
cxxSolution::add(const cxxSolution & addee, double extensive)
		//
		// Add existing solution to "this" solution
		//
{
	if (extensive == 0.0)
		return;
	double ext1 = this->mass_water;
	double ext2 = addee.mass_water * extensive;
	double f1 = ext1 / (ext1 + ext2);
	double f2 = ext2 / (ext1 + ext2);
	this->tc = f1 * this->tc + f2 * addee.tc;
	this->ph = f1 * this->ph + f2 * addee.ph;
	this->pe = f1 * this->pe + f2 * addee.pe;
	this->mu = f1 * this->mu + f2 * addee.mu;
	this->ah2o = f1 * this->mu + f2 * addee.ah2o;
	this->total_h += addee.total_h * extensive;
	this->total_o += addee.total_o * extensive;
	this->cb += addee.cb * extensive;
	this->mass_water += addee.mass_water * extensive;
	this->total_alkalinity += addee.total_alkalinity * extensive;
	this->totals.add_extensive(addee.totals, extensive);
	this->master_activity.add_log_activities(addee.master_activity, f1, f2);
	this->species_gamma.add_intensive(addee.species_gamma, f1, f2);
	this->isotopes.add(addee.isotopes, f2, extensive);
}

void
cxxSolution::multiply(double extensive)
		//
		// Multiply existing solution by extensive
		//
{
	if (extensive == 0.0 || extensive == 1.0)
		return;
	this->total_h *= extensive;
	this->total_o *= extensive;
	this->cb *= extensive;
	this->mass_water *= extensive;
	this->total_alkalinity *= extensive;
	this->totals.multiply(extensive);
	this->isotopes.multiply(extensive);
}

double
cxxSolution::get_total(char *string) const
{
	cxxNameDouble::const_iterator it = this->totals.find(string);
	if (it == this->totals.end())
	{
		return (0.0);
	}
	else
	{
		return (it->second);
	}
}
double
cxxSolution::get_total_element(char *string) const
{
	cxxNameDouble::const_iterator it;
	double d = 0.0;
	for (it = this->totals.begin(); it != this->totals.end(); ++it)
	{
		char token[MAX_LENGTH];
		if (it->first[0] == string[0])
		{
			strcpy(token, it->first);
			replace("(", "\0", token);
			if (strcmp(token, string) == 0)
			{
				d += it->second;
			}
		}
#ifdef SKIP
		// C++ way to do it
		std::string ename(string);
		std::string current_ename(it->first);
		std::basic_string < char >::size_type indexCh;
		indexCh = current_ename.find("(");
		if (indexCh != std::string::npos)
		{
			current_ename = current_ename.substr(0, indexCh);
		}
		if (current_ename == ename)
		{
			d += it->second;
		}
#endif
	}
	return (d);
}

void
cxxSolution::set_total(char *string, double d)
{
	this->totals[string] = d;
}

double
cxxSolution::get_master_activity(char *string) const
{
	cxxNameDouble::const_iterator it = this->master_activity.find(string);
	if (it == this->master_activity.end())
	{
		return (0.0);
	}
	else
	{
		return (it->second);
	}
}

#ifdef ORCHESTRA
void
cxxSolution::ORCH_write(std::ostream & headings, std::ostream & data) const const
{

	data.precision(DBL_DIG - 1);

	// Solution element and attributes

	//s_oss << "SOLUTION_RAW       " << this->n_user  << " " << this->description << std::endl;

	//s_oss << "-temp              " << this->tc << std::endl;

	//s_oss << "-pH                " << this->ph << std::endl;
	headings << "pH\t";
	data << this->ph << "\t";

	//s_oss << "-pe                " << this->pe << std::endl;
	headings << "pe\t";
	data << this->pe << "\t";

	//s_oss << "-mu                " << this->mu << std::endl;

	//s_oss << "-ah2o              " << this->ah2o << std::endl;
	headings << "H2O.act\t";
	data << 1 << "\t";
	//s_oss << "-total_h           " << this->total_h << std::endl;

	//s_oss << "-total_o           " << this->total_o << std::endl;

	//s_oss << "-cb                " << this->cb << std::endl;

	//s_oss << "-mass_water        " << this->mass_water << std::endl;

	//s_oss << "-total_alkalinity  " << this->total_alkalinity << std::endl;

	// soln_total conc structures
	//this->totals.dump_raw(s_oss, indent + 2);
	//this->totals.write_orchestra(headings, s_oss);
	for (std::map < char *, double, CHARSTAR_LESS >::const_iterator it =
		 totals.begin(); it != totals.end(); ++it)
	{
		std::string master_name;
		struct master *master_ptr;
		master_ptr = master_bsearch(it->first);
		if (master_ptr != NULL)
		{
			//headings << it->first << ".tot" << "\t";
			headings << master_ptr->s->name << ".diss" << "\t";
			data << it->second << "\t";
		}
		else
		{
			assert(false);
		}
	}

	// master_activity map
	//this->master_activity.dump_raw(s_oss, indent + 2);
	/*
	   {
	   for (std::map <char *, double>::const_iterator it = master_activity.begin(); it != master_activity.end(); ++it) {
	   s_oss << indent2;
	   s_oss << it->first << "   " << it->second << std::endl;
	   }
	   }
	 */
	// species_gamma map
	//this->species_gamma.dump_raw(s_oss, indent + 2);
	/*
	   {
	   {
	   for (std::map <char *, double>::const_iterator it = species_gamma.begin(); it != species_gamma.end(); ++it) {
	   s_oss << indent2;
	   s_oss << it->first << "   " << it->second << std::endl;
	   }
	   }
	   }
	 */
	// Isotopes
	//s_oss << "-Isotopes" << std::endl;
	/*
	   {
	   for (std::list<cxxSolutionIsotope>::const_iterator it = this->isotopes.begin(); it != isotopes.end(); ++it) {
	   it->dump_raw(s_oss, indent + 2);
	   }
	   }
	 */

	return;
}

void
cxxSolution::ORCH_read(std::vector < std::pair < std::string,
					   double >>output_vector,
					   std::vector < std::pair < std::string,
					   double >>::iterator & it)
{
	this->tc = it->second;
	it++;
	this->ph = it->second;
	it++;
	this->pe = it->second;
	it++;
	this->mu = it->second;
	it++;
	this->ah2o = it->second;
	it++;
	this->total_h = it->second;
	it++;
	this->total_o = it->second;
	it++;
	this->cb = it->second;
	it++;
	this->mass_water = it->second * gfw_water;
	it++;
	this->mass_water = 1.0;
	this->total_alkalinity = it->second;
	it++;
	it++;						//orch total H+
	it++;						//orch total e-
	it++;						//orch total H2O
	//cxxNameDouble totals;
	char token[MAX_LENGTH];
	while (it->first.compare("end_totals") != 0)
	{
		strcpy(token, it->first.c_str());
		replace(".diss", "", token);
		struct species *s_ptr = s_search(token);
		if (s_ptr == NULL)
			error_msg("Species not found in orchestra read", STOP);
		if (s_ptr->secondary != NULL)
		{
			this->totals[s_ptr->secondary->elt->name] = it->second;
		}
		else if (s_ptr->primary != NULL)
		{
			this->totals[s_ptr->primary->elt->name] = it->second;
		}
		else
		{
			error_msg
				("Species not secondary or primary master species in orchestra read",
				 STOP);
		}
		it++;
	}
	//cxxNameDouble master_activity;
	it++;
	while (it->first.compare("end_master_activities") != 0)
	{
		strcpy(token, it->first.c_str());
		replace(".act", "", token);
		struct species *s_ptr = s_search(token);
		if (s_ptr == NULL)
			error_msg("Species not found in orchestra read", STOP);
		if (s_ptr->secondary != NULL)
		{
			this->master_activity[s_ptr->secondary->elt->name] =
				log10(it->second);
		}
		else if (s_ptr->primary != NULL)
		{
			this->master_activity[s_ptr->primary->elt->name] =
				log10(it->second);
		}
		else
		{
			error_msg
				("Species not secondary or primary master species in orchestra read",
				 STOP);
		}
		it++;
	}
	//cxxNameDouble species_gamma;
	//cxxSolutionIsotopeList isotopes;
	//
	// Also process aqueous species data
	//
	it++;
	while (it->first.compare("end_species") != 0)
	{
		strcpy(token, it->first.c_str());
		replace(".act", "", token);
		while (replace("[", "(", token));
		while (replace("]", ")", token));
		struct species *s_ptr = s_search(token);
		if (s_ptr == NULL)
			error_msg("Species not found in orchestra read", STOP);
		s_ptr->la = log10(it->second);
		it++;
		s_ptr->moles = it->second * this->mass_water;
		s_ptr->lm = log10(it->second);
		it++;
		s_ptr->lg = s_ptr->la - s_ptr->lm;
	}
}

void
cxxSolution::ORCH_store_global(std::map < std::string, double >output_map)
{
	int i;
	tc_x = this->tc;
	mass_water_aq_x = this->mass_water;
	mu_x = this->mu;
	s_h2o->moles = output_map["H2O.con"];
	s_h2o->la = log10(output_map["H2O.act"]);
	s_h2o->lm = s_h2o->la;
	s_h2o->lg = 0;
	for (i = 0; i < count_unknowns; i++)
	{
		residual[i] = 0;
		// MB, ALK, CB, SOLUTION_PHASE_BOUNDARY, MU, AH2O
		switch (x[i]->type)
		{
		case MB:
		case CB:
		case SOLUTION_PHASE_BOUNDARY:
			x[i]->sum = this->totals[x[i]->description] * mass_water_aq_x;
			break;
		case ALK:
			x[i]->f = this->total_alkalinity * mass_water_aq_x;
			break;
		}
	}
}
#endif
#ifdef USE_MPI
/* ---------------------------------------------------------------------- */
void
cxxSolution::mpi_pack(std::vector < int >&ints,
					  std::vector < double >&doubles)
/* ---------------------------------------------------------------------- */
{
/*
 *   Make list of list of ints and doubles from solution structure
 *   This list is not the complete structure, but only enough
 *   for batch-reaction, advection, and transport calculations
 */
	ints.push_back(this->n_user);
	doubles.push_back(this->tc);
	doubles.push_back(this->ph);
	doubles.push_back(this->pe);
	doubles.push_back(this->mu);
	doubles.push_back(this->ah2o);
	doubles.push_back(this->total_h);
	doubles.push_back(this->total_o);
	doubles.push_back(this->cb);
	doubles.push_back(this->mass_water);
	doubles.push_back(this->total_alkalinity);
/*
 *	struct conc *totals;
*/
	this->totals.mpi_pack(ints, doubles);
/*
 *	struct master_activity *master_activity;
 */
	this->master_activity.mpi_pack(ints, doubles);
/*
 *	struct master_activity *species_gamma
 */
	this->species_gamma.mpi_pack(ints, doubles);

	/*
	   position = 0;
	   int i = ints.size();
	   int int_array[i];
	   int d = doubles.size();
	   double double_array[d];
	   for (int j = 0; j < i; j++) {
	   int_array[j] = ints[j];
	   }
	   for (int j = 0; j < d; j++) {
	   double_array[j] = ints[j];
	   }
	   MPI_Send(&max_size, 1, MPI_INT, task_number, 0, MPI_COMM_WORLD);
	   MPI_Pack(&i, 1, MPI_INT, buffer, max_size, &position, MPI_COMM_WORLD);
	   MPI_Pack(&int_array, i, MPI_INT, buffer, max_size, &position, MPI_COMM_WORLD);
	   MPI_Pack(&d, 1, MPI_INT, buffer, max_size, &position, MPI_COMM_WORLD);
	   MPI_Pack(&double_array, d, MPI_DOUBLE, buffer, max_size, &position, MPI_COMM_WORLD);
	   MPI_Send(buffer, position, MPI_PACKED, task_number, 0, MPI_COMM_WORLD);

	   buffer = (void *) free_check_null(buffer);
	 */
}

/* ---------------------------------------------------------------------- */
void
cxxSolution::mpi_unpack(int *ints, int *ii, double *doubles, int *dd)
/* ---------------------------------------------------------------------- */
{
	int i = *ii;
	int d = *dd;
	this->n_user = ints[i++];
	this->n_user_end = this->n_user;
	this->description = "  ";
	this->tc = doubles[d++];
	this->ph = doubles[d++];
	this->pe = doubles[d++];
	this->mu = doubles[d++];
	this->ah2o = doubles[d++];
	this->total_h = doubles[d++];
	this->total_o = doubles[d++];
	this->cb = doubles[d++];
	this->mass_water = doubles[d++];
	this->total_alkalinity = doubles[d++];
/*
 *	struct conc *totals;
*/
	this->totals.mpi_unpack(ints, &i, doubles, &d);
/*
 *	struct master_activity *master_activity;
 */
	this->master_activity.mpi_unpack(ints, &i, doubles, &d);
/*
 *	struct master_activity *species_gamma;
 */
	this->species_gamma.mpi_unpack(ints, &i, doubles, &d);

	*ii = i;
	*dd = d;
}

/* ---------------------------------------------------------------------- */
void
cxxSolution::mpi_send(int task_number)
/* ---------------------------------------------------------------------- */
{
	//int count_totals, count_totals_position, count_activity, count_activity_position;
	int max_size, member_size, position;
	//int ints[MESSAGE_MAX_NUMBERS];
	//double doubles[MESSAGE_MAX_NUMBERS];
	void *buffer;
	std::vector < int >ints;
	std::vector < double >doubles;
/*
 *   Make list of list of ints and doubles from solution structure
 *   This list is not the complete structure, but only enough
 *   for batch-reaction, advection, and transport calculations
 */
	ints.push_back(this->n_user);
	/*  int n_user_end; */
	/*  char *description; */
	doubles.push_back(this->tc);
	doubles.push_back(this->ph);
	doubles.push_back(this->pe);
	doubles.push_back(this->mu);
	doubles.push_back(this->ah2o);
	doubles.push_back(this->total_h);
	doubles.push_back(this->total_o);
	doubles.push_back(this->cb);
	doubles.push_back(this->mass_water);
	doubles.push_back(this->total_alkalinity);
/*
 *	struct conc *totals;
*/
	this->totals.mpi_pack(ints, doubles);
/*
 *	struct master_activity *master_activity;
 */
	this->master_activity.mpi_pack(ints, doubles);
/*
 *	struct master_activity *species_gamma
 */
	this->species_gamma.mpi_pack(ints, doubles);

	/*  int count_isotopes; */
	/*  struct isotope *isotopes; */
	if (input_error > 0)
	{
		std::string errstr("Stopping due to errors\n");
		error_msg(errstr.c_str(), STOP);
	}
/*
 *   Malloc space for a buffer
 */
	max_size = 0;
	//MPI_Pack_size(MESSAGE_MAX_NUMBERS, MPI_INT, MPI_COMM_WORLD, &member_size);
	MPI_Pack_size((int) ints.size(), MPI_INT, MPI_COMM_WORLD, &member_size);
	max_size += member_size;
	//MPI_Pack_size(MESSAGE_MAX_NUMBERS, MPI_DOUBLE, MPI_COMM_WORLD, &member_size);
	MPI_Pack_size((int) doubles.size(), MPI_DOUBLE, MPI_COMM_WORLD,
				  &member_size);
	max_size += member_size + 10;
	buffer = PHRQ_malloc(max_size);
	if (buffer == NULL)
		malloc_error();
/*
 *   Send message to processor
 */
	position = 0;
	int i = (int) ints.size();
	int *int_array = new int[i];
	int d = (int) doubles.size();
	double *double_array = new double[d];
	for (int j = 0; j < i; j++)
	{
		int_array[j] = ints[j];
	}
	for (int j = 0; j < d; j++)
	{
		double_array[j] = doubles[j];
	}

	MPI_Send(&max_size, 1, MPI_INT, task_number, 0, MPI_COMM_WORLD);
	MPI_Pack(&i, 1, MPI_INT, buffer, max_size, &position, MPI_COMM_WORLD);
	MPI_Pack(&int_array, i, MPI_INT, buffer, max_size, &position,
			 MPI_COMM_WORLD);
	MPI_Pack(&d, 1, MPI_INT, buffer, max_size, &position, MPI_COMM_WORLD);
	MPI_Pack(&double_array, d, MPI_DOUBLE, buffer, max_size, &position,
			 MPI_COMM_WORLD);
	MPI_Send(buffer, position, MPI_PACKED, task_number, 0, MPI_COMM_WORLD);

	buffer = (void *) free_check_null(buffer);
	delete[]int_array;
	delete[]double_array;
}

/* ---------------------------------------------------------------------- */
void
cxxSolution::mpi_recv(int task_number)
/* ---------------------------------------------------------------------- */
{
	MPI_Status mpi_status;
/*
 *   Malloc space for a buffer
 */
	int max_size;
	// buffer size

	MPI_Recv(&max_size, 1, MPI_INT, task_number, 0, MPI_COMM_WORLD,
			 &mpi_status);
	void *buffer = PHRQ_malloc(max_size);
	if (buffer == NULL)
		malloc_error();
	/*
	 *   Recieve solution
	 */
	MPI_Recv(buffer, max_size, MPI_PACKED, task_number, 0, MPI_COMM_WORLD,
			 &mpi_status);
	int position = 0;
	int msg_size;
	MPI_Get_count(&mpi_status, MPI_PACKED, &msg_size);

	/* Unpack ints */
	int count_ints;
	MPI_Unpack(buffer, msg_size, &position, &count_ints, 1, MPI_INT,
			   MPI_COMM_WORLD);
	int *ints = new int[count_ints];
	MPI_Unpack(buffer, msg_size, &position, ints, count_ints, MPI_INT,
			   MPI_COMM_WORLD);

	/* Unpack doubles */
	int count_doubles;
	MPI_Unpack(buffer, msg_size, &position, &count_doubles, 1, MPI_INT,
			   MPI_COMM_WORLD);
	double *doubles = new double[count_doubles];
	MPI_Unpack(buffer, msg_size, &position, doubles, count_doubles,
			   MPI_DOUBLE, MPI_COMM_WORLD);
	buffer = free_check_null(buffer);
/*
 *   Make list of list of ints and doubles from solution structure
 *   This list is not the complete structure, but only enough
 *   for batch-reaction, advection, and transport calculations
 */
	int i = 0;
	int d = 0;
	/*  int new_def; */
	/* solution_ptr->new_def = FALSE; */
	/*  int n_user; */
	this->n_user = ints[i++];
	/*  int n_user_end; */
	this->n_user_end = this->n_user;

	/*debugging */
	//this->description = (char *) free_check_null(this->description);
	//this->description = string_duplicate(" ");
	this->description = " ";
	this->tc = doubles[d++];
	this->ph = doubles[d++];
	this->pe = doubles[d++];
	this->mu = doubles[d++];
	this->ah2o = doubles[d++];
	this->total_h = doubles[d++];
	this->total_o = doubles[d++];
	this->cb = doubles[d++];
	this->mass_water = doubles[d++];
	this->total_alkalinity = 0;
/*
 *	struct conc *totals;
*/
	this->totals.mpi_unpack(ints, &i, doubles, &d);
/*
 *	struct master_activity *master_activity;
 */
	this->master_activity.mpi_unpack(ints, &i, doubles, &d);
/*
 *	struct master_activity *species_gamma;
 */
	this->species_gamma.mpi_unpack(ints, &i, doubles, &d);
	delete[]ints;
	delete[]doubles;
}
#endif
void
cxxSolution::set_master_activity(char *string, double d)
{
	this->master_activity[string] = d;
}

#include "ISolution.h"
#include "Exchange.h"
#include "Surface.h"
#include "PPassemblage.h"
#include "cxxKinetics.h"
#include "SSassemblage.h"
#include "GasPhase.h"
#include "Reaction.h"
#include "Temperature.h"
#include "StorageBin.h"
#include "NumKeyword.h"
#include <iostream>				// std::cout std::cerr
//#include <strstream>
#include <sstream>
#include <fstream>
void
test_classes(void)
{




	int i;
	/*
	   std::map<int, cxxSolution>      Solutions;
	   cxxSolution soln(solution[0]);
	   Solutions[solution[0]->n_user] = soln;
	   bool b = Utilities::exists(Solutions, 1);
	 */
	/*
	   cxxEntityMap x;
	   cxxSolution soln(solution[0]);
	   cxxNumKeyword nk;
	   x[solution[0]->n_user] = soln;
	 */
	std::ostringstream msg;
	status_on = FALSE;
	std::cout << std::endl << "TEST CLASSES" << std::endl;
	for (i = 0; i < count_solution; i++)
	{
		if (solution[i]->new_def == TRUE)
		{
			std::cout << "Solution new_def " << solution[i]->
				n_user << std::endl;
			cxxISolution sol(solution[i]);
			solution_free(solution[i]);
			solution[i] = NULL;
			solution[i] = sol.cxxISolution2solution();
			struct solution *soln_ptr;
			soln_ptr = solution[i];
			soln_ptr = solution[i];
		}
		else
		{
			std::cout << "Solution " << solution[i]->n_user << std::endl;
			std::ostringstream oss;
			cxxSolution sol(solution[i]);
			solution_free(solution[i]);
			solution[i] = NULL;
			sol.dump_raw(oss, 0);

			//std::fstream myfile("t");
			//CParser cparser(myfile, std::cout, std::cerr);
			cxxSolution sol1;
			std::string keyInput = oss.str();
			std::istringstream iss(keyInput);

			CParser cparser(iss, oss, std::cerr);
			//For testing, need to read line to get started
			std::vector < std::string > vopts;
			std::istream::pos_type next_char;
			cparser.get_option(vopts, next_char);


			sol1.read_raw(cparser);

			solution[i] = sol1.cxxSolution2solution();
		}
	}
	for (i = 0; i < count_exchange; i++)
	{
		if (exchange[i].new_def != TRUE)
		{
			std::cout << "Exchange " << exchange[i].n_user << std::endl;
			std::ostringstream oss;
			cxxExchange ex(&(exchange[i]));
			ex.dump_raw(oss, 0);
			//std::cerr << oss.str();

			cxxExchange ex1;
			std::string keyInput = oss.str();
			std::istringstream iss(keyInput);

			CParser cparser(iss, oss, std::cerr);
			//For testing, need to read line to get started
			std::vector < std::string > vopts;
			std::istream::pos_type next_char;
			cparser.get_option(vopts, next_char);

			ex1.read_raw(cparser);

			struct exchange *exchange_ptr = ex1.cxxExchange2exchange();
			exchange_free(&exchange[i]);
			exchange_copy(exchange_ptr, &exchange[i], exchange_ptr->n_user);
			exchange_free(exchange_ptr);
			free_check_null(exchange_ptr);
		}
	}
	for (i = 0; i < count_surface; i++)
	{
		if (surface[i].new_def != TRUE)
		{
			std::cout << "Surface " << surface[i].n_user << std::endl;
			std::ostringstream oss;
			cxxSurface ex(&(surface[i]));
			ex.dump_raw(oss, 0);
			//std::cerr << oss.str();


			cxxSurface ex1;
			std::string keyInput = oss.str();
			std::istringstream iss(keyInput);

			CParser cparser(iss, oss, std::cerr);
			//For testing, need to read line to get started
			std::vector < std::string > vopts;
			std::istream::pos_type next_char;
			cparser.get_option(vopts, next_char);

			ex1.read_raw(cparser);

			struct surface *surface_ptr = ex1.cxxSurface2surface();
			surface_free(&surface[i]);
			surface_copy(surface_ptr, &surface[i], surface_ptr->n_user);
			surface_free(surface_ptr);
			free_check_null(surface_ptr);

		}

	}
	for (i = 0; i < count_pp_assemblage; i++)
	{
		if (pp_assemblage[i].new_def != TRUE)
		{
			std::cout << "PPassemblage " << pp_assemblage[i].
				n_user << std::endl;
			std::ostringstream oss;
			cxxPPassemblage ex(&(pp_assemblage[i]));
			ex.dump_raw(oss, 0);
			//std::cerr << oss.str();


			cxxPPassemblage ex1;
			std::string keyInput = oss.str();
			std::istringstream iss(keyInput);

			CParser cparser(iss, oss, std::cerr);
			//For testing, need to read line to get started
			std::vector < std::string > vopts;
			std::istream::pos_type next_char;
			cparser.get_option(vopts, next_char);

			ex1.read_raw(cparser);

			struct pp_assemblage *pp_assemblage_ptr =
				ex1.cxxPPassemblage2pp_assemblage();
			pp_assemblage_free(&pp_assemblage[i]);
			pp_assemblage_copy(pp_assemblage_ptr, &pp_assemblage[i],
							   pp_assemblage_ptr->n_user);
			pp_assemblage_free(pp_assemblage_ptr);
			free_check_null(pp_assemblage_ptr);

		}

	}
	for (i = 0; i < count_kinetics; i++)
	{
		std::cout << "Kinetics " << kinetics[i].n_user << std::endl;
		std::ostringstream oss;
		cxxKinetics ex(&(kinetics[i]));
		ex.dump_raw(oss, 0);
		//std::cerr << oss.str();


		cxxKinetics ex1;
		std::string keyInput = oss.str();
		std::istringstream iss(keyInput);

		CParser cparser(iss, oss, std::cerr);
		//For testing, need to read line to get started
		std::vector < std::string > vopts;
		std::istream::pos_type next_char;
		cparser.get_option(vopts, next_char);


		ex1.read_raw(cparser);

		struct kinetics *kinetics_ptr = ex1.cxxKinetics2kinetics();
		kinetics_free(&kinetics[i]);
		kinetics_copy(kinetics_ptr, &kinetics[i], kinetics_ptr->n_user);
		kinetics_free(kinetics_ptr);
		free_check_null(kinetics_ptr);
	}
	for (i = 0; i < count_s_s_assemblage; i++)
	{
		if (s_s_assemblage[i].new_def != TRUE)
		{
			std::cout << "Solid solution " << s_s_assemblage[i].
				n_user << std::endl;
			std::ostringstream oss;
			cxxSSassemblage ex(&(s_s_assemblage[i]));
			ex.dump_raw(oss, 0);
			//std::cerr << oss.str();


			cxxSSassemblage ex1;
			std::string keyInput = oss.str();
			std::istringstream iss(keyInput);

			CParser cparser(iss, oss, std::cerr);
			//For testing, need to read line to get started
			std::vector < std::string > vopts;
			std::istream::pos_type next_char;
			cparser.get_option(vopts, next_char);

			ex1.read_raw(cparser);

			struct s_s_assemblage *s_s_assemblage_ptr =
				ex1.cxxSSassemblage2s_s_assemblage();
			s_s_assemblage_free(&s_s_assemblage[i]);
			s_s_assemblage_copy(s_s_assemblage_ptr, &s_s_assemblage[i],
								s_s_assemblage_ptr->n_user);
			s_s_assemblage_free(s_s_assemblage_ptr);
			free_check_null(s_s_assemblage_ptr);

		}

	}
	for (i = 0; i < count_gas_phase; i++)
	{
		if (gas_phase[i].new_def != TRUE)
		{
			std::cout << "Gas phase " << gas_phase[i].n_user << std::endl;
			std::ostringstream oss;
			cxxGasPhase ex(&(gas_phase[i]));
			ex.dump_raw(oss, 0);
			//std::cerr << oss.str();


			cxxGasPhase ex1;
			std::string keyInput = oss.str();
			std::istringstream iss(keyInput);

			CParser cparser(iss, oss, std::cerr);
			//For testing, need to read line to get started
			std::vector < std::string > vopts;
			std::istream::pos_type next_char;
			cparser.get_option(vopts, next_char);

			ex1.read_raw(cparser);

			struct gas_phase *gas_phase_ptr = ex1.cxxGasPhase2gas_phase();
			gas_phase_free(&gas_phase[i]);
			gas_phase_copy(gas_phase_ptr, &gas_phase[i],
						   gas_phase_ptr->n_user);
			gas_phase_free(gas_phase_ptr);
			free_check_null(gas_phase_ptr);

		}

	}
	for (i = 0; i < count_irrev; i++)
	{
		std::cout << "Reaction " << irrev[i].n_user << std::endl;
		std::ostringstream oss;
		cxxReaction ex(&(irrev[i]));
		ex.dump_raw(oss, 0);
		//std::cerr << oss.str();


		cxxReaction ex1;
		std::string keyInput = oss.str();
		std::istringstream iss(keyInput);

		CParser cparser(iss, oss, std::cerr);
		//For testing, need to read line to get started
		std::vector < std::string > vopts;
		std::istream::pos_type next_char;
		cparser.get_option(vopts, next_char);

		ex1.read_raw(cparser);
		struct irrev *irrev_ptr = ex1.cxxReaction2irrev();

		irrev_free(&irrev[i]);
		irrev_copy(irrev_ptr, &irrev[i], irrev_ptr->n_user);

		irrev_free(irrev_ptr);
		free_check_null(irrev_ptr);

	}
	for (i = 0; i < count_mix; i++)
	{
		std::cout << "Mix " << mix[i].n_user << std::endl;
		std::ostringstream oss;
		cxxMix ex(&(mix[i]));
		ex.dump_raw(oss, 0);
		//std::cerr << oss.str();


		cxxMix ex1;
		std::string keyInput = oss.str();
		std::istringstream iss(keyInput);

		CParser cparser(iss, oss, std::cerr);
		//For testing, need to read line to get started
		std::vector < std::string > vopts;
		std::istream::pos_type next_char;
		cparser.get_option(vopts, next_char);

		ex1.read_raw(cparser);
		struct mix *mix_ptr = ex1.cxxMix2mix();

		mix_free(&mix[i]);
		mix_copy(mix_ptr, &mix[i], mix_ptr->n_user);

		mix_free(mix_ptr);
		free_check_null(mix_ptr);

	}
	for (i = 0; i < count_temperature; i++)
	{
		std::cout << "Temperature " << temperature[i].n_user << std::endl;
		std::ostringstream oss;
		cxxTemperature ex(&(temperature[i]));
		ex.dump_raw(oss, 0);
		//std::cerr << oss.str();


		cxxTemperature ex1;
		std::string keyInput = oss.str();
		std::istringstream iss(keyInput);

		CParser cparser(iss, oss, std::cerr);
		//For testing, need to read line to get started
		std::vector < std::string > vopts;
		std::istream::pos_type next_char;
		cparser.get_option(vopts, next_char);

		ex1.read_raw(cparser);
		struct temperature *temperature_ptr =
			ex1.cxxTemperature2temperature();

		temperature_free(&temperature[i]);
		temperature_copy(temperature_ptr, &temperature[i],
						 temperature_ptr->n_user);

		temperature_free(temperature_ptr);
		free_check_null(temperature_ptr);

	}
	/*
	   {
	   // get all c storage
	   cxxStorageBin cstorage;
	   cstorage.import_phreeqc();
	   //std::ostringstream oss;
	   //cstorage.dump_raw(oss, 0);
	   //write it out
	   std::fstream myfile;
	   myfile.open("tfile", std::ios_base::out);
	   cstorage.dump_raw(myfile, 0);
	   myfile.close();
	   }
	   {
	   // empty storage bin
	   cxxStorageBin cstorage;
	   // fstream
	   std::fstream myfile;
	   myfile.open("tfile", std::ios_base::in);
	   // ostream
	   std::ostringstream oss;
	   // parser
	   CParser cparser(myfile, oss, std::cerr);
	   cstorage.read_raw(cparser);
	   //std::cerr << oss.str();

	   // read it back
	   }
	 */
}
