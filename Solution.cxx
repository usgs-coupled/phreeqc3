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
#include <set>
#include <cassert>				// assert
#include <algorithm>			// std::sort
#include "Utils.h"				// define first
#if !defined(PHREEQC_CLASS)
#define EXTERNAL extern
#include "global.h"
#else
#include "Phreeqc.h"
#endif
#include "Solution.h"
#include "cxxMix.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include "output.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


cxxSolution::cxxSolution(PHRQ_io * io)
	//
	// default constructor for cxxSolution 
	//
:	cxxNumKeyword(io),
isotopes(io)
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

cxxSolution::cxxSolution(struct solution * solution_ptr, PHRQ_io * io)
	//
	// constructor for cxxSolution from struct solution
	//
:
cxxNumKeyword(io),
totals(solution_ptr->totals),
master_activity(solution_ptr->master_activity,
solution_ptr->count_master_activity,
cxxNameDouble::ND_SPECIES_LA),
species_gamma(solution_ptr->species_gamma, solution_ptr->count_species_gamma, cxxNameDouble::ND_SPECIES_GAMMA), 
isotopes(solution_ptr, io)
{

	this->Set_description(solution_ptr->description);
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
						 cxxMix & mix, int l_n_user, PHRQ_io * io)
//
// constructor for cxxSolution from mixture of solutions
//
	:
cxxNumKeyword(io),
isotopes(io)
{

//
//   Zero out solution data
//
	this->zero();
	this->n_user = this->n_user_end = l_n_user;
//
//   Mix solutions
//
	std::map < int, double >&mixcomps = mix.Get_mixComps();
	std::map < int, double >::const_iterator it;
	for (it = mixcomps.begin(); it != mixcomps.end(); it++)
	{
		std::map < int, cxxSolution >::const_iterator sol =
			solutions.find(it->first);
		if (sol == solutions.end())
		{
			std::ostringstream msg;
			msg << "Solution " << it->first << " not found in mix_cxxSolutions.";
			error_msg(msg.str(), CONTINUE);
		}
		else
		{
			const cxxSolution *cxxsoln_ptr1 = &(sol->second);
			this->add(*cxxsoln_ptr1, it->second);
		}
	}
}
cxxSolution::~cxxSolution()
{
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
cxxSolution::dump_raw(std::ostream & s_oss, unsigned int indent, int *n_out) const
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
	int n_user_local = (n_out != NULL) ? *n_out : this->n_user;
	s_oss << "SOLUTION_RAW       " << n_user_local << " " << this->description << std::endl;

	//s_oss << "# Critical values" << std::endl;

	s_oss << indent1;
	s_oss << "-temp              " << this->tc << std::endl;

	// new identifier
	s_oss << indent1;
	s_oss << "-total_h           " << this->total_h << std::endl;

	// new identifier
	s_oss << indent1;
	s_oss << "-total_o           " << this->total_o << std::endl;

	// new identifier
	s_oss << indent1;
	s_oss << "-cb                " << this->cb << std::endl;

	// soln_total conc structures
	s_oss << indent1;
	s_oss << "-totals" << std::endl;
	this->totals.dump_raw(s_oss, indent + 2);

	//s_oss << "# Optional critical values" << std::endl;

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
	/*
	   for (std::map <char *, double, CHARSTAR_LESS>::const_iterator it = totals.begin(); it != totals.end(); ++it) {
	   s_oss << indent2;
	   s_oss << it->first << "   " <<  it->second << std::endl;
	   }
	 */

	//s_oss << "# Non-critical values" << std::endl;

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
	s_oss << "-mass_water        " << this->mass_water << std::endl;

	// new identifier
	s_oss << indent1;
	s_oss << "-total_alkalinity  " << this->total_alkalinity << std::endl;

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

	return;
}

void
cxxSolution::read_raw(CParser & parser, bool check)
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
		vopts.push_back("tc_avoid_conflict_with_technetium");	// 5 
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
			//if (this->totals.read_raw(parser, next_char) !=
			//	CParser::PARSER_OK)
			//{
			//	parser.incr_input_error();
			//	parser.
			//		error_msg("Expected element name and moles for totals.",
			//				  CParser::OT_CONTINUE);
			//}
			{
				cxxNameDouble temp_totals;
				if (temp_totals.read_raw(parser, next_char) !=	CParser::PARSER_OK)
				{
					parser.incr_input_error();
					parser.
						error_msg("Expected element name and moles for totals.",
						CParser::OT_CONTINUE);
				}
				else
				{
					this->totals.merge_redox(temp_totals);
				}
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
				cxxSolutionIsotope iso(this->Get_io());
				if (iso.read_raw(parser, next_char) != CParser::PARSER_OK)
				{
					parser.incr_input_error();
					parser.error_msg("Expected data for isotopes.",
									 CParser::OT_CONTINUE);
				}
				else
				{
					if (iso.Get_isotope_name().size() != 0)
					{
						this->isotopes.push_back(iso);
					}
				}
			}
			opt_save = 3;
			break;

		case 4:				// temp
		case 5:				// tc_avoid_conflict_with_technetium
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
	if (check)
	{
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
cxxSolution::Get_total(char *string) const
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
cxxSolution::Get_total_element(const char *string) const
{
	cxxNameDouble::const_iterator it;
	double d = 0.0;
	for (it = this->totals.begin(); it != this->totals.end(); ++it)
	{
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
	}
	return (d);
}

void
cxxSolution::Set_total(char *string, double d)
{
	this->totals[string] = d;
}

double
cxxSolution::Get_master_activity(char *string) const
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
	if (P_INSTANCE_POINTER input_error > 0)
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
	buffer = P_INSTANCE_POINTER PHRQ_malloc(max_size);
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
	void *buffer = P_INSTANCE_POINTER PHRQ_malloc(max_size);
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
cxxSolution::Set_master_activity(char *string, double d)
{
	this->master_activity[string] = d;
}
#ifdef SKIP
void
cxxSolution::modify_activities(PHREEQC_PTR_ARG_COMMA const cxxSolution & original)
//
// Estimate activities after solution_modify
//
{
	// Note: any master_activities in "this" have been read in SOLUTION_MODIFY

	// to standardize, convert element to valence state if needed
	// for original activity list (probably not needed)
	cxxNameDouble orig_master_activity(original.Get_master_activity());
	cxxNameDouble::const_iterator it;
	bool redo=true;
	while (redo)
	{
		redo = false;
		for (it = orig_master_activity.begin(); it != orig_master_activity.end(); it++)
		{
			struct master *master_ptr = P_INSTANCE_POINTER master_bsearch(it->first.c_str());
			if (master_ptr != NULL)
			{
				if (master_ptr->primary == TRUE)
				{
					struct master *master_ptr_secondary = P_INSTANCE_POINTER master_bsearch_secondary(master_ptr->elt->name);

					// redox element erase and replace
					if (master_ptr_secondary != master_ptr) 
					{
						double d = it->second;
						orig_master_activity.erase(orig_master_activity.find(master_ptr->elt->name));
						orig_master_activity[master_ptr_secondary->elt->name] = d;
						redo = true;
						break;
					}
				}
			}
			else
			{
				error_msg("Could not find master species in modify_activities.", STOP);
			}
		}
	}

	// also for modified activity list
	cxxNameDouble mod_master_activity(this->master_activity);
	redo=true;
	while (redo)
	{
		redo = false;
		for (it = mod_master_activity.begin(); it != mod_master_activity.end(); it++)
		{
			struct master *master_ptr = P_INSTANCE_POINTER master_bsearch(it->first.c_str());
			if (master_ptr != NULL)
			{
				if (master_ptr->primary == TRUE)
				{
					struct master *master_ptr_secondary = P_INSTANCE_POINTER master_bsearch_secondary(master_ptr->elt->name);

					// redox element erase and replace
					if (master_ptr_secondary != master_ptr) 
					{
						double d = it->second;
						mod_master_activity.erase(mod_master_activity.find(master_ptr->elt->name));
						mod_master_activity[master_ptr_secondary->elt->name] = d;
						redo = true;
						break;
					}
				}
			}
			else
			{
				error_msg("Could not find master species in modify_activities.", STOP);
			}
		}
	}

	// go through totals
	for (it = this->totals.begin(); it != this->totals.end(); ++it)
	{
		// find element name 
		struct master *master_ptr = P_INSTANCE_POINTER master_bsearch(it->first.c_str());
		struct master *master_primary_ptr = master_ptr->elt->primary;
		char * ename = master_ptr->elt->primary->elt->name;
		char * secondary_name;
		if (master_ptr->primary == TRUE)
		{
			struct master *m_ptr = P_INSTANCE_POINTER master_bsearch_secondary(ename);
			secondary_name = m_ptr->elt->name;
		}
		else
		{
			secondary_name = master_ptr->elt->name;
		}
		if (strcmp(ename, "H") == 0 || strcmp(ename, "O") == 0) continue;

		double d_mod, d_orig;
		d_mod = this->Get_total_element(ename);
		if (d_mod <= 0) continue;

		d_orig = original.Get_total_element(ename);
		if (d_orig <= 0) 
		{
			// add estimate for la based on concentration if not in list
			if (mod_master_activity.find(secondary_name) == mod_master_activity.end())
			{
				mod_master_activity[secondary_name] = log10(d_mod) - 2.0;
			}
			continue;
		}

		// case where total for both orig and modified are greater than 0
		double lratio = log10(d_mod / d_orig);

		int j;
		j = master_primary_ptr->number;
		int j_first = j + 1;
		int j_last = P_INSTANCE_POINTER count_master;

		// non redox element
		if ( (j+1 >= P_INSTANCE_POINTER count_master) || (P_INSTANCE_POINTER master[j+1]->elt->primary != master_primary_ptr) )
		{
			j_first = j;
			j_last = j+1;
		}

		for (j = j_first ; j < j_last; j++)
		{
			if (P_INSTANCE_POINTER master[j]->elt->primary != master_primary_ptr) break;

			if (mod_master_activity.find(P_INSTANCE_POINTER master[j]->elt->name) == mod_master_activity.end())
			{
				// has not been defined in SOLUTION_MODIFY
				cxxNameDouble::const_iterator it1;
				it1 = orig_master_activity.find(P_INSTANCE_POINTER master[j]->elt->name);
				if (it1 != orig_master_activity.end())
				{
					double d = it1->second;
					mod_master_activity[P_INSTANCE_POINTER master[j]->elt->name] = d + lratio;
				}
				else
					// Has total, but no activity, should not happen
				{
					mod_master_activity[P_INSTANCE_POINTER master[j]->elt->name] = log10(d_mod) - 2.0;
				}
			}
		}
	}

	// merge activities
	this->master_activity = orig_master_activity;

	for (it = mod_master_activity.begin(); it != mod_master_activity.end(); it++)
	{
		this->master_activity[it->first] = it->second;
	}
}
#endif
void
cxxSolution::modify_activities(const cxxSolution & original)
//
// Estimate activities after solution_modify
//
{
	// Note: any master_activities in "this" have been read in SOLUTION_MODIFY
	cxxNameDouble factor;
	cxxNameDouble updated_orig_activities(original.Get_master_activity());
	cxxNameDouble::const_iterator it;
	cxxNameDouble::const_iterator orig_it;

	// Calculate a factor of log10(new tot/old tot) for each element or element redox state in current totals
	for (it = this->totals.begin(); it != this->totals.end(); it++)
	{
		orig_it = original.Get_totals().find(it->first);
		if (orig_it != original.Get_totals().end())
		{
			// element or valence state in both
			if (it->second > 0 && orig_it->second > 0)
			{
				factor[it->first] = log10(it->second / orig_it->second);
			}
		}
		else
		{
			
			std::string ename;
			std::basic_string < char >::size_type indexCh;
			indexCh = it->first.find("(");
			if (indexCh != std::string::npos)
			{
				// valence in current and element in original
				ename = it->first.substr(0, indexCh);
				double orig_tot = original.Get_total_element(ename.c_str());
				double tot = this->Get_total_element(ename.c_str());
				if (tot > 0 && orig_tot > 0)
				{
					factor[ename] = log10(tot/orig_tot);
				}
			}
			else
			{
				// element in current and valence in original
				ename = it->first;
				double orig_tot = original.Get_total_element(ename.c_str());
				if (it->second > 0 && orig_tot > 0)
				{
					factor[ename] = log10(it->second/orig_tot);
				}
			}
		}
	}

	// update original master_activities using just computed factors
	for (it = factor.begin(); it != factor.end(); it++)
	{
		orig_it = original.Get_master_activity().find(it->first);
		if (orig_it != original.Get_master_activity().end())
		{
			// Found exact match
			double d = orig_it->second + it->second;
			updated_orig_activities[it->first.c_str()] = d;
		}
		else
		{
			// no exact match, current is element name, need to find all valences
			orig_it = original.Get_master_activity().begin();
			std::string v_name = it->first;
			v_name.append("(");
			for ( ; orig_it != original.Get_master_activity().end(); orig_it++)
			{
				if (strstr(orig_it->first.c_str(), v_name.c_str()) == orig_it->first.c_str())
				{
					double d = orig_it->second + it->second;
					updated_orig_activities[orig_it->first.c_str()] = d;
				}
			}
		}
	}

	// Merge any new master_activities, which overwrites updated originals
	updated_orig_activities.merge_redox(this->Get_master_activity());

	// Set activities to updated, merged activities
	this->master_activity.clear();
	this->master_activity = updated_orig_activities;

}
void 
cxxSolution::Simplify_totals()
{
	// remove individual redox states from totals
	std::set<std::string> list_of_elements;
	cxxNameDouble::iterator it;
	for (it = this->totals.begin(); it != this->totals.end(); ++it)
	{
		std::string current_ename(it->first);
		std::basic_string < char >::size_type indexCh;
		indexCh = current_ename.find("(");
		if (indexCh != std::string::npos)
		{
			current_ename = current_ename.substr(0, indexCh);
		}
		if (current_ename == "H" || current_ename == "O" || current_ename == "Charge")
			continue;
		list_of_elements.insert(current_ename);
	}

	cxxNameDouble new_totals;
	new_totals.type = cxxNameDouble::ND_ELT_MOLES;
	std::set<std::string>::iterator nt_it = list_of_elements.begin();
	for( ; nt_it != list_of_elements.end(); nt_it++)
	{
		new_totals[(*nt_it).c_str()] = this->Get_total_element((*nt_it).c_str());
	}
	this->totals = new_totals;
}
void 
cxxSolution::Update(const cxxNameDouble &nd)
{
	// assumes
	// nd has H, O, Charge, and totals, without any valence states

	// remove individual redox states from solution totals
	this->Simplify_totals();

	// process special cases and calculate factors
	cxxNameDouble factor;	
	cxxNameDouble::const_iterator it;
	for (it = nd.begin(); it != nd.end(); ++it)
	{
		if (it->first == "H")
		{
			this->total_h = it->second;
		}
		else if (it->first == "O")
		{
			this->total_o = it->second;
		}
		else if (it->first == "Charge")
		{
			this->cb = it->second;
		}
		else
		{
			cxxNameDouble::iterator sol_it = this->totals.find(it->first.c_str());
			if (sol_it != this->totals.end())
			{
				if (it->second != 0 && sol_it->second != 0)
				{
					factor[it->first.c_str()] = log10(it->second/sol_it->second);
				}
				this->totals[it->first.c_str()] = it->second;
			}
		}
	}

	// update original master_activities using just computed factors

	cxxNameDouble updated_activities(this->master_activity);
	for (it = factor.begin(); it != factor.end(); it++)
	{
		std::string v_name = it->first;
		v_name.append("(");

		cxxNameDouble::iterator sol_it = this->totals.begin();
		for ( ; sol_it != this->totals.end(); sol_it++)
		{
			// Exact match
			if ( it->first == sol_it->first) 
			{
				double d = sol_it->second + it->second;
				updated_activities[it->first.c_str()] = d;
			}
			else if (strstr(v_name.c_str(), sol_it->first.c_str()) == sol_it->first.c_str())
			{
				double d = sol_it->second + it->second;
				updated_activities[sol_it->first.c_str()] = d;
			}
		}
	}
	// Set activities to updated, merged activities
	this->master_activity.clear();
	this->master_activity = updated_activities;
}

//#include "ISolution.h"
//#include "Exchange.h"
//#include "Surface.h"
//#include "PPassemblage.h"
//#include "cxxKinetics.h"
//#include "SSassemblage.h"
//#include "GasPhase.h"
//#include "Reaction.h"
//#include "Temperature.h"
//#include "StorageBin.h"
//#include "NumKeyword.h"
//#include <iostream>				// std::cout std::cerr
////#include <strstream>
//#include <sstream>
//#include <fstream>
//void
//test_classes(void)
//{
//
//
//
//
//	int i;
//	/*
//	   std::map<int, cxxSolution>      Solutions;
//	   cxxSolution soln(solution[0]);
//	   Solutions[solution[0]->n_user] = soln;
//	   bool b = Utilities::exists(Solutions, 1);
//	 */
//	/*
//	   cxxEntityMap x;
//	   cxxSolution soln(solution[0]);
//	   cxxNumKeyword nk;
//	   x[solution[0]->n_user] = soln;
//	 */
//	std::ostringstream msg;
//	status_on = FALSE;
//	std::cout << std::endl << "TEST CLASSES" << std::endl;
//	for (i = 0; i < count_solution; i++)
//	{
//		if (solution[i]->new_def == TRUE)
//		{
//			std::cout << "Solution new_def " << solution[i]->
//				n_user << std::endl;
//			cxxISolution sol(solution[i]);
//			solution_free(solution[i]);
//			solution[i] = NULL;
//			solution[i] = sol.cxxISolution2solution();
//			struct solution *soln_ptr;
//			soln_ptr = solution[i];
//			soln_ptr = solution[i];
//		}
//		else
//		{
//			std::cout << "Solution " << solution[i]->n_user << std::endl;
//			std::ostringstream oss;
//			cxxSolution sol(solution[i]);
//			solution_free(solution[i]);
//			solution[i] = NULL;
//			sol.dump_raw(oss, 0);
//
//			//std::fstream myfile("t");
//			//CParser cparser(myfile, std::cout, std::cerr);
//			cxxSolution sol1;
//			std::string keyInput = oss.str();
//			std::istringstream iss(keyInput);
//
//			CParser cparser(iss, oss, std::cerr);
//			//For testing, need to read line to get started
//			std::vector < std::string > vopts;
//			std::istream::pos_type next_char;
//			cparser.get_option(vopts, next_char);
//
//
//			sol1.read_raw(cparser);
//
//			solution[i] = sol1.cxxSolution2solution();
//		}
//	}
//	for (i = 0; i < count_exchange; i++)
//	{
//		if (exchange[i].new_def != TRUE)
//		{
//			std::cout << "Exchange " << exchange[i].n_user << std::endl;
//			std::ostringstream oss;
//			cxxExchange ex(&(exchange[i]));
//			ex.dump_raw(oss, 0);
//			//std::cerr << oss.str();
//
//			cxxExchange ex1;
//			std::string keyInput = oss.str();
//			std::istringstream iss(keyInput);
//
//			CParser cparser(iss, oss, std::cerr);
//			//For testing, need to read line to get started
//			std::vector < std::string > vopts;
//			std::istream::pos_type next_char;
//			cparser.get_option(vopts, next_char);
//
//			ex1.read_raw(cparser);
//
//			struct exchange *exchange_ptr = ex1.cxxExchange2exchange();
//			exchange_free(&exchange[i]);
//			exchange_copy(exchange_ptr, &exchange[i], exchange_ptr->n_user);
//			exchange_free(exchange_ptr);
//			free_check_null(exchange_ptr);
//		}
//	}
//	for (i = 0; i < count_surface; i++)
//	{
//		if (surface[i].new_def != TRUE)
//		{
//			std::cout << "Surface " << surface[i].n_user << std::endl;
//			std::ostringstream oss;
//			cxxSurface ex(&(surface[i]));
//			ex.dump_raw(oss, 0);
//			//std::cerr << oss.str();
//
//
//			cxxSurface ex1;
//			std::string keyInput = oss.str();
//			std::istringstream iss(keyInput);
//
//			CParser cparser(iss, oss, std::cerr);
//			//For testing, need to read line to get started
//			std::vector < std::string > vopts;
//			std::istream::pos_type next_char;
//			cparser.get_option(vopts, next_char);
//
//			ex1.read_raw(cparser);
//
//			struct surface *surface_ptr = ex1.cxxSurface2surface();
//			surface_free(&surface[i]);
//			surface_copy(surface_ptr, &surface[i], surface_ptr->n_user);
//			surface_free(surface_ptr);
//			free_check_null(surface_ptr);
//
//		}
//
//	}
//	for (i = 0; i < count_pp_assemblage; i++)
//	{
//		if (pp_assemblage[i].new_def != TRUE)
//		{
//			std::cout << "PPassemblage " << pp_assemblage[i].
//				n_user << std::endl;
//			std::ostringstream oss;
//			cxxPPassemblage ex(&(pp_assemblage[i]));
//			ex.dump_raw(oss, 0);
//			//std::cerr << oss.str();
//
//
//			cxxPPassemblage ex1;
//			std::string keyInput = oss.str();
//			std::istringstream iss(keyInput);
//
//			CParser cparser(iss, oss, std::cerr);
//			//For testing, need to read line to get started
//			std::vector < std::string > vopts;
//			std::istream::pos_type next_char;
//			cparser.get_option(vopts, next_char);
//
//			ex1.read_raw(cparser);
//
//			struct pp_assemblage *pp_assemblage_ptr =
//				ex1.cxxPPassemblage2pp_assemblage();
//			pp_assemblage_free(&pp_assemblage[i]);
//			pp_assemblage_copy(pp_assemblage_ptr, &pp_assemblage[i],
//							   pp_assemblage_ptr->n_user);
//			pp_assemblage_free(pp_assemblage_ptr);
//			free_check_null(pp_assemblage_ptr);
//
//		}
//
//	}
//	for (i = 0; i < count_kinetics; i++)
//	{
//		std::cout << "Kinetics " << kinetics[i].n_user << std::endl;
//		std::ostringstream oss;
//		cxxKinetics ex(&(kinetics[i]));
//		ex.dump_raw(oss, 0);
//		//std::cerr << oss.str();
//
//
//		cxxKinetics ex1;
//		std::string keyInput = oss.str();
//		std::istringstream iss(keyInput);
//
//		CParser cparser(iss, oss, std::cerr);
//		//For testing, need to read line to get started
//		std::vector < std::string > vopts;
//		std::istream::pos_type next_char;
//		cparser.get_option(vopts, next_char);
//
//
//		ex1.read_raw(cparser);
//
//		struct kinetics *kinetics_ptr = ex1.cxxKinetics2kinetics();
//		kinetics_free(&kinetics[i]);
//		kinetics_copy(kinetics_ptr, &kinetics[i], kinetics_ptr->n_user);
//		kinetics_free(kinetics_ptr);
//		free_check_null(kinetics_ptr);
//	}
//	for (i = 0; i < count_s_s_assemblage; i++)
//	{
//		if (s_s_assemblage[i].new_def != TRUE)
//		{
//			std::cout << "Solid solution " << s_s_assemblage[i].
//				n_user << std::endl;
//			std::ostringstream oss;
//			cxxSSassemblage ex(&(s_s_assemblage[i]));
//			ex.dump_raw(oss, 0);
//			//std::cerr << oss.str();
//
//
//			cxxSSassemblage ex1;
//			std::string keyInput = oss.str();
//			std::istringstream iss(keyInput);
//
//			CParser cparser(iss, oss, std::cerr);
//			//For testing, need to read line to get started
//			std::vector < std::string > vopts;
//			std::istream::pos_type next_char;
//			cparser.get_option(vopts, next_char);
//
//			ex1.read_raw(cparser);
//
//			struct s_s_assemblage *s_s_assemblage_ptr =
//				ex1.cxxSSassemblage2s_s_assemblage();
//			s_s_assemblage_free(&s_s_assemblage[i]);
//			s_s_assemblage_copy(s_s_assemblage_ptr, &s_s_assemblage[i],
//								s_s_assemblage_ptr->n_user);
//			s_s_assemblage_free(s_s_assemblage_ptr);
//			free_check_null(s_s_assemblage_ptr);
//
//		}
//
//	}
//	for (i = 0; i < count_gas_phase; i++)
//	{
//		if (gas_phase[i].new_def != TRUE)
//		{
//			std::cout << "Gas phase " << gas_phase[i].n_user << std::endl;
//			std::ostringstream oss;
//			cxxGasPhase ex(&(gas_phase[i]));
//			ex.dump_raw(oss, 0);
//			//std::cerr << oss.str();
//
//
//			cxxGasPhase ex1;
//			std::string keyInput = oss.str();
//			std::istringstream iss(keyInput);
//
//			CParser cparser(iss, oss, std::cerr);
//			//For testing, need to read line to get started
//			std::vector < std::string > vopts;
//			std::istream::pos_type next_char;
//			cparser.get_option(vopts, next_char);
//
//			ex1.read_raw(cparser);
//
//			struct gas_phase *gas_phase_ptr = ex1.cxxGasPhase2gas_phase();
//			gas_phase_free(&gas_phase[i]);
//			gas_phase_copy(gas_phase_ptr, &gas_phase[i],
//						   gas_phase_ptr->n_user);
//			gas_phase_free(gas_phase_ptr);
//			free_check_null(gas_phase_ptr);
//
//		}
//
//	}
//	for (i = 0; i < count_irrev; i++)
//	{
//		std::cout << "Reaction " << irrev[i].n_user << std::endl;
//		std::ostringstream oss;
//		cxxReaction ex(&(irrev[i]));
//		ex.dump_raw(oss, 0);
//		//std::cerr << oss.str();
//
//
//		cxxReaction ex1;
//		std::string keyInput = oss.str();
//		std::istringstream iss(keyInput);
//
//		CParser cparser(iss, oss, std::cerr);
//		//For testing, need to read line to get started
//		std::vector < std::string > vopts;
//		std::istream::pos_type next_char;
//		cparser.get_option(vopts, next_char);
//
//		ex1.read_raw(cparser);
//		struct irrev *irrev_ptr = ex1.cxxReaction2irrev();
//
//		irrev_free(&irrev[i]);
//		irrev_copy(irrev_ptr, &irrev[i], irrev_ptr->n_user);
//
//		irrev_free(irrev_ptr);
//		free_check_null(irrev_ptr);
//
//	}
//	for (i = 0; i < count_mix; i++)
//	{
//		std::cout << "Mix " << mix[i].n_user << std::endl;
//		std::ostringstream oss;
//		cxxMix ex(&(mix[i]));
//		ex.dump_raw(oss, 0);
//		//std::cerr << oss.str();
//
//
//		cxxMix ex1;
//		std::string keyInput = oss.str();
//		std::istringstream iss(keyInput);
//
//		CParser cparser(iss, oss, std::cerr);
//		//For testing, need to read line to get started
//		std::vector < std::string > vopts;
//		std::istream::pos_type next_char;
//		cparser.get_option(vopts, next_char);
//
//		ex1.read_raw(cparser);
//		struct mix *mix_ptr = ex1.cxxMix2mix();
//
//		mix_free(&mix[i]);
//		mix_copy(mix_ptr, &mix[i], mix_ptr->n_user);
//
//		mix_free(mix_ptr);
//		free_check_null(mix_ptr);
//
//	}
//	for (i = 0; i < count_temperature; i++)
//	{
//		std::cout << "Temperature " << temperature[i].n_user << std::endl;
//		std::ostringstream oss;
//		cxxTemperature ex(&(temperature[i]));
//		ex.dump_raw(oss, 0);
//		//std::cerr << oss.str();
//
//
//		cxxTemperature ex1;
//		std::string keyInput = oss.str();
//		std::istringstream iss(keyInput);
//
//		CParser cparser(iss, oss, std::cerr);
//		//For testing, need to read line to get started
//		std::vector < std::string > vopts;
//		std::istream::pos_type next_char;
//		cparser.get_option(vopts, next_char);
//
//		ex1.read_raw(cparser);
//		struct temperature *temperature_ptr =
//			ex1.cxxTemperature2temperature();
//
//		temperature_free(&temperature[i]);
//		temperature_copy(temperature_ptr, &temperature[i],
//						 temperature_ptr->n_user);
//
//		temperature_free(temperature_ptr);
//		free_check_null(temperature_ptr);
//
//	}
//	/*
//	   {
//	   // get all c storage
//	   cxxStorageBin cstorage;
//	   cstorage.import_phreeqc();
//	   //std::ostringstream oss;
//	   //cstorage.dump_raw(oss, 0);
//	   //write it out
//	   std::fstream myfile;
//	   myfile.open("tfile", std::ios_base::out);
//	   cstorage.dump_raw(myfile, 0);
//	   myfile.close();
//	   }
//	   {
//	   // empty storage bin
//	   cxxStorageBin cstorage;
//	   // fstream
//	   std::fstream myfile;
//	   myfile.open("tfile", std::ios_base::in);
//	   // ostream
//	   std::ostringstream oss;
//	   // parser
//	   CParser cparser(myfile, oss, std::cerr);
//	   cstorage.read_raw(cparser);
//	   //std::cerr << oss.str();
//
//	   // read it back
//	   }
//	 */
//}
