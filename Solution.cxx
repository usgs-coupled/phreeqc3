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
#include "Phreeqc.h"
#include "Solution.h"
#include "cxxMix.h"
#include "phqalloc.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


cxxSolution::cxxSolution(PHRQ_io * io)
	//
	// default constructor for cxxSolution 
	//
:	cxxNumKeyword(io)
{
	this->io = io;
	this->new_def = false;
	this->patm = 1.0;
	this->tc = 25.0;
	this->ph = 7.0;
	this->pe = 4.0;
	this->mu = 1e-7;
	this->ah2o = 1.0;
	this->total_h = 111.1;
	this->total_o = 55.55;
	this->cb = 0.0;
	this->density = 1.0;
	this->mass_water = 1.0;
	this->total_alkalinity = 0.0;
	this->totals.type = cxxNameDouble::ND_ELT_MOLES;
	this->master_activity.type = cxxNameDouble::ND_SPECIES_LA;
	this->species_gamma.type = cxxNameDouble::ND_SPECIES_GAMMA;
	this->initial_data = NULL;
}
cxxSolution::cxxSolution(const cxxSolution &old_sol)
:	initial_data(NULL)
{
	*this = old_sol;
}
const cxxSolution &
cxxSolution::operator =(const cxxSolution &rhs)
{
	if (this != &rhs)
	{
		this->io                         = rhs.io;
		this->n_user                     = rhs.n_user;
		this->n_user_end                 = rhs.n_user_end;
		this->description                = rhs.description;
		this->new_def                    = rhs.new_def;
		this->patm                       = rhs.patm;
		this->tc                         = rhs.tc;
		this->ph                         = rhs.ph;
		this->pe                         = rhs.pe;
		this->mu                         = rhs.mu;
		this->ah2o                       = rhs.ah2o;
		this->total_h                    = rhs.total_h;
		this->total_o                    = rhs.total_o;
		this->density                    = rhs.density;
		this->cb                         = rhs.cb;
		this->mass_water                 = rhs.mass_water;
		this->total_alkalinity           = rhs.total_alkalinity;
		this->totals		             = rhs.totals;
		this->master_activity            = rhs.master_activity;
		this->species_gamma              = rhs.species_gamma;
		this->isotopes                   = rhs.isotopes;
		if (this->initial_data)
			delete initial_data;
		if (rhs.initial_data != NULL)
			this->initial_data           = new cxxISolution(*rhs.initial_data);
		else
			this->initial_data           = NULL;
	}
	return *this;
}
cxxSolution::cxxSolution(std::map < int, cxxSolution > &solutions,
						 cxxMix & mix, int l_n_user, PHRQ_io * io)
//
// constructor for cxxSolution from mixture of solutions
//
: cxxNumKeyword(io)
{

//
//   Zero out solution data
//
	this->zero();
	this->n_user = this->n_user_end = l_n_user;
//
//   Mix solutions
//
	const std::map < int, LDBLE >&mixcomps = mix.Get_mixComps();
	std::map < int, LDBLE >::const_iterator it;
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
	delete this->initial_data;
}

void
cxxSolution::dump_xml(std::ostream & s_oss, unsigned int indent) const
{
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);

	// Solution element and attributes
	s_oss << indent0;
	s_oss << "<solution " << "\n";

	s_oss << indent1;
	s_oss << "soln_n_user=\"" << this->n_user << "\" " << "\n";

	s_oss << indent1;
	s_oss << "soln_description=\"" << this->description << "\"" << "\n";

	s_oss << indent1;
	s_oss << "soln_tc=\"" << this->tc << "\"" << "\n";

	s_oss << indent1;
	s_oss << "soln_ph=\"" << this->ph << "\"" << "\n";

	s_oss << indent1;
	s_oss << "soln_solution_pe=\"" << this->pe << "\"" << "\n";

	s_oss << indent1;
	s_oss << "soln_mu=\"" << this->mu << "\"" << "\n";

	s_oss << indent1;
	s_oss << "soln_ah2o=\"" << this->ah2o << "\"" << "\n";

	s_oss << indent1;
	s_oss << "soln_total_h=\"" << this->total_h << "\"" << "\n";

	s_oss << indent1;
	s_oss << "soln_total_o=\"" << this->total_o << "\"" << "\n";

	s_oss << indent1;
	s_oss << "soln_cb=\"" << this->cb << "\"" << "\n";

	s_oss << indent1;
	s_oss << "soln_mass_water=\"" << this->mass_water << "\"" << "\n";

	s_oss << indent1;
	s_oss << "soln_total_alkalinity=\"" << this->
		total_alkalinity << "\"" << "\n";

	s_oss << indent1;
	s_oss << "\">" << "\n";

	// soln_total conc structures
	this->totals.dump_xml(s_oss, indent + 1);

	// master_activity map
	this->master_activity.dump_xml(s_oss, indent + 1);

	// species_gamma map
	this->species_gamma.dump_xml(s_oss, indent + 1);

	// End of solution
	s_oss << indent0;
	s_oss << "</solution>" << "\n";

	return;
}

void
cxxSolution::dump_raw(std::ostream & s_oss, unsigned int indent, int *n_out) const
{
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
	s_oss << "SOLUTION_RAW                 " << n_user_local << " " << this->description << "\n";

	s_oss << indent1;
	s_oss << "-temp                      " << this->tc << "\n";

	// new identifier
	s_oss << indent1;
	s_oss << "-total_h                   " << this->total_h << "\n";

	// new identifier
	s_oss << indent1;
	s_oss << "-total_o                   " << this->total_o << "\n";

	// new identifier
	s_oss << indent1;
	s_oss << "-cb                        " << this->cb << "\n";

	// new identifier
	s_oss << indent1;
	s_oss << "-density                   " << this->density << "\n";

	// soln_total conc structures
	s_oss << indent1;
	s_oss << "-totals" << "\n";
	this->totals.dump_raw(s_oss, indent + 2);

	// Isotopes
	s_oss << indent1;
	{
		for (std::map < std::string, cxxSolutionIsotope >::const_iterator it =
			this->isotopes.begin(); it != isotopes.end(); ++it)
		{
			s_oss << indent1 << "-Isotope" << "\n";
			it->second.dump_raw(s_oss, indent + 2);
		}
	}

	s_oss << indent1;
	s_oss << "-pH                        " << this->ph << "\n";

	s_oss << indent1;
	s_oss << "-pe                        " << this->pe << "\n";

	// new identifier
	s_oss << indent1;
	s_oss << "-mu                        " << this->mu << "\n";

	// new identifier
	s_oss << indent1;
	s_oss << "-ah2o                      " << this->ah2o << "\n";

	// new identifier
	s_oss << indent1;
	s_oss << "-mass_water                " << this->mass_water << "\n";

	// new identifier
	s_oss << indent1;
	s_oss << "-total_alkalinity          " << this->total_alkalinity << "\n";

	// master_activity map
	s_oss << indent1;
	s_oss << "-activities" << "\n";
	this->master_activity.dump_raw(s_oss, indent + 2);

	// species_gamma map
	s_oss << indent1;
	s_oss << "-gammas" << "\n";
	this->species_gamma.dump_raw(s_oss, indent + 2);

	return;
}
void
cxxSolution::read_raw(CParser & parser, bool check)
{

	// Used if it is modify
	cxxNameDouble simple_original_totals = this->totals.Simplify_redox();
	cxxNameDouble original_activities(this->master_activity);

	this->master_activity.clear();

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;

	// Read solution number and description
	this->read_number_description(parser.line());

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
							 PHRQ_io::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), PHRQ_io::OT_CONTINUE);
			continue;

		case 0:				// totals
			{
				cxxNameDouble temp_totals;
				if (temp_totals.read_raw(parser, next_char) !=	CParser::PARSER_OK)
				{
					parser.incr_input_error();
					parser.
						error_msg("Expected element name and moles for totals.",
						PHRQ_io::OT_CONTINUE);
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
					 PHRQ_io::OT_CONTINUE);
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
					 PHRQ_io::OT_CONTINUE);
			}
			opt_save = 2;
			break;

		case 3:				// isotope
			{
				std::string name;
				if (!(parser.get_iss() >> name))
				{
					parser.incr_input_error();
					parser.error_msg("Expected character value for isotope name.",
									 PHRQ_io::OT_CONTINUE);
				}
				else
				{
					cxxSolutionIsotope iso(this->Get_io());
					iso.Set_isotope_name(name.c_str());
					iso.read_raw(parser, check);
					this->isotopes[name] = iso;
				}
			}
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 4:				// temp
		case 5:				// tc_avoid_conflict_with_technetium
		case 6:				// temperature                  
			if (!(parser.get_iss() >> this->tc))
			{
				this->tc = 25.0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for temperature.",
								 PHRQ_io::OT_CONTINUE);
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
								 PHRQ_io::OT_CONTINUE);
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
								 PHRQ_io::OT_CONTINUE);
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
								 PHRQ_io::OT_CONTINUE);
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
							  PHRQ_io::OT_CONTINUE);
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
								 PHRQ_io::OT_CONTINUE);
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
								 PHRQ_io::OT_CONTINUE);
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
								 PHRQ_io::OT_CONTINUE);
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
							  PHRQ_io::OT_CONTINUE);
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
								 PHRQ_io::OT_CONTINUE);
			}
			cb_defined = true;
			opt_save = CParser::OPT_DEFAULT;
			break;
		case 21:				// density
			if (!(parser.get_iss() >> this->density))
			{
				this->density = 1.0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for density.",
								 PHRQ_io::OT_CONTINUE);
			}
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
				PHRQ_io::OT_CONTINUE);
		}
		if (ph_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("pH not defined for SOLUTION_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (pe_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("pe not defined for SOLUTION_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (mu_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Ionic strength not defined for SOLUTION_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (ah2o_defined == false)
		{
			parser.incr_input_error();
			parser.
				error_msg("Activity of water not defined for SOLUTION_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (total_h_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Total hydrogen not defined for SOLUTION_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (total_o_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Total oxygen not defined for SOLUTION_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (cb_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Charge balance not defined for SOLUTION_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (mass_water_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Temp not defined for SOLUTION_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (total_alkalinity_defined == false)
		{
			parser.incr_input_error();
			parser.
				error_msg("Total alkalinity not defined for SOLUTION_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
	}

	// Update activities
	if (original_activities.size() > 0)
	{
		
		cxxNameDouble simple_this_totals = this->totals.Simplify_redox();
		cxxNameDouble::iterator it = simple_original_totals.begin();
		for ( ; it != simple_original_totals.end(); it++)
		{
			cxxNameDouble::iterator jit = simple_this_totals.find(it->first);
			if (jit != simple_this_totals.end())
			{
				if (it->second != 0)
				{
					LDBLE f = jit->second / it->second;
					if (f != 1)
					{
						original_activities.Multiply_activities_redox(it->first, f);
					}
				}
			}
		}
		original_activities.merge_redox(this->master_activity);
		this->master_activity = original_activities;
	}

	return;
}
void
cxxSolution::Update(const cxxNameDouble &const_nd)
{
	if (this->master_activity.size() > 0)
	{
		cxxNameDouble simple_original = this->totals.Simplify_redox();
		cxxNameDouble simple_new = const_nd.Simplify_redox();

		// make factors
		{
			cxxNameDouble::iterator it = simple_new.begin();
			for ( ; it != simple_new.end(); it++)
			{
				cxxNameDouble::iterator jit = simple_original.find(it->first);
				if (jit != simple_original.end())
				{
					if (it->second != 0 && jit->second > 0)
					{
						it->second = log10(jit->second / it->second);
					}
				}
				else
				{
					it->second = 0;
				}
			}
		}

		// simple_new now has factors for master activities
		// Now add factors to activities
		{
			cxxNameDouble::iterator activity_it = this->master_activity.begin();
			cxxNameDouble::iterator total_it = simple_new.begin();
			std::string activity_ename;
			std::basic_string < char >::size_type indexCh;
			while (activity_it != master_activity.end() && total_it != simple_new.end())
			{
				activity_ename = activity_it->first;
				if (activity_ename.size() > 3)
				{
					indexCh = activity_ename.find("(");
					if (indexCh != std::string::npos)
					{
						activity_ename = activity_ename.substr(0, indexCh);
					}
				}
				int j = strcmp(total_it->first.c_str(), activity_ename.c_str());
				if (j < 0)
				{
					total_it++;
				}
				else if (j == 0)
				{
					if (total_it->second > 0)
					{
						activity_it->second += total_it->second;
					}
					activity_it++;
				}
				else 
				{
					activity_it++;
				}
			}
		}
	}
}
#ifdef SKIP
void
cxxSolution::Update(const cxxNameDouble &const_nd)
{
	// const_nd is updated totals
	cxxNameDouble simple_original_totals = this->totals.Simplify_redox();
	cxxNameDouble original_activities(this->master_activity);

	this->master_activity.clear();

	// Update activities
	if (original_activities.size() > 0)
	{
		cxxNameDouble nd = const_nd;
		cxxNameDouble simple_this_totals = nd.Simplify_redox();
		cxxNameDouble::iterator it = simple_original_totals.begin();
		for ( ; it != simple_original_totals.end(); it++)
		{
			cxxNameDouble::iterator jit = simple_this_totals.find(it->first);
			if (jit != simple_this_totals.end())
			{
				if (it->second != 0)
				{
					LDBLE f = jit->second / it->second;
					if (f != 1)
					{
						original_activities.Multiply_activities_redox(it->first, f);
					}
				}
			}
		}
		original_activities.merge_redox(this->master_activity);
		this->master_activity = original_activities;
	}

	return;
}
#endif
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
	this->density = 1.0;
	this->mass_water = 0.0;
	this->total_alkalinity = 0.0;
	this->totals.type = cxxNameDouble::ND_ELT_MOLES;
	this->master_activity.type = cxxNameDouble::ND_SPECIES_LA;
	this->species_gamma.type = cxxNameDouble::ND_SPECIES_GAMMA;
	this->patm = 1.0;
	this->initial_data = NULL;
}

void
cxxSolution::add(const cxxSolution & addee, LDBLE extensive)
		//
		// Add existing solution to "this" solution
		//
{
	if (extensive == 0.0)
		return;
	LDBLE ext1 = this->mass_water;
	LDBLE ext2 = addee.mass_water * extensive;
	LDBLE f1 = ext1 / (ext1 + ext2);
	LDBLE f2 = ext2 / (ext1 + ext2);
	this->tc = f1 * this->tc + f2 * addee.tc;
	this->ph = f1 * this->ph + f2 * addee.ph;
	this->pe = f1 * this->pe + f2 * addee.pe;
	this->mu = f1 * this->mu + f2 * addee.mu;
	this->ah2o = f1 * this->mu + f2 * addee.ah2o;
	this->total_h += addee.total_h * extensive;
	this->total_o += addee.total_o * extensive;
	this->cb += addee.cb * extensive;
	this->density = f1 * this->density + f2 * addee.density;
	this->mass_water += addee.mass_water * extensive;
	this->total_alkalinity += addee.total_alkalinity * extensive;
	this->totals.add_extensive(addee.totals, extensive);
	this->master_activity.add_log_activities(addee.master_activity, f1, f2);
	this->species_gamma.add_intensive(addee.species_gamma, f1, f2);
	this->Add_isotopes(addee.isotopes, f2, extensive);
}

void
cxxSolution::multiply(LDBLE extensive)
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
	this->Multiply_isotopes(extensive);
}

LDBLE
cxxSolution::Get_total(const char *string) const
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
#ifdef SKIP
LDBLE
cxxSolution::Get_total_element(const char *string) const
{
	cxxNameDouble::const_iterator it;
	LDBLE d = 0.0;
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
#endif

void
cxxSolution::Set_total(char *string, LDBLE d)
{
	this->totals[string] = d;
}

LDBLE
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
					  std::vector < LDBLE >&doubles)
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
	   LDBLE double_array[d];
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
cxxSolution::mpi_unpack(int *ints, int *ii, LDBLE *doubles, int *dd)
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
	//LDBLE doubles[MESSAGE_MAX_NUMBERS];
	void *buffer;
	std::vector < int >ints;
	std::vector < LDBLE >doubles;
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
	if (phreeqc_ptr-> input_error > 0)
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
	buffer = phreeqc_ptr-> PHRQ_malloc(max_size);
	if (buffer == NULL)
		malloc_error();
/*
 *   Send message to processor
 */
	position = 0;
	int i = (int) ints.size();
	int *int_array = new int[i];
	int d = (int) doubles.size();
	LDBLE *double_array = new LDBLE[d];
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
	void *buffer = phreeqc_ptr-> PHRQ_malloc(max_size);
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
	LDBLE *doubles = new LDBLE[count_doubles];
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
cxxSolution::Set_master_activity(char *string, LDBLE d)
{
	this->master_activity[string] = d;
}
void
cxxSolution::Add_isotopes(const std::map < std::string, cxxSolutionIsotope > & old, LDBLE intensive, LDBLE extensive)
{
	for (std::map < std::string, cxxSolutionIsotope >::const_iterator itold = old.begin(); itold != old.end(); ++itold)
	{
		std::map < std::string, cxxSolutionIsotope >::iterator it_this;
		it_this = this->isotopes.find(itold->first);
		if (it_this != this->isotopes.end())
		{
			LDBLE t = it_this->second.Get_total();
			t += itold->second.Get_total() * extensive;
			it_this->second.Set_total(t);

			t = it_this->second.Get_ratio();
			t += itold->second.Get_ratio() * intensive;
			it_this->second.Set_ratio(t);

			t = it_this->second.Get_ratio_uncertainty();
			t += itold->second.Get_ratio_uncertainty() * intensive;
			it_this->second.Set_ratio_uncertainty(t);
			it_this->second.Set_ratio_uncertainty_defined(it_this->second.Get_ratio_uncertainty_defined()
												 || itold->second.Get_ratio_uncertainty_defined());
		}
		else
		{
			cxxSolutionIsotope iso(itold->second);
			iso.Set_total(itold->second.Get_total() * extensive);
			this->Get_isotopes()[iso.Get_isotope_name()] = iso;
		}
	}
}
void
cxxSolution::Multiply_isotopes(LDBLE extensive)
{
	std::map < std::string, cxxSolutionIsotope>::iterator it;
	for (it = this->isotopes.begin(); it != this->isotopes.end(); it++)
	{
		LDBLE total = it->second.Get_total();
		total *= extensive;
		it->second.Set_total(total);
	}
}
const std::vector< std::string >::value_type temp_vopts[] = {
	std::vector< std::string >::value_type("totals"),	                            // 0 
	std::vector< std::string >::value_type("activities"),	                        // 1 
	std::vector< std::string >::value_type("gammas"),	                            // 2 
	std::vector< std::string >::value_type("isotopes"),	                            // 3 
	std::vector< std::string >::value_type("temp"),	                                // 4 
	std::vector< std::string >::value_type("tc_avoid_conflict_with_technetium"),	// 5 
	std::vector< std::string >::value_type("temperature"),	                        // 6 
	std::vector< std::string >::value_type("ph"),	                                // 7 
	std::vector< std::string >::value_type("pe"),	                                // 8 
	std::vector< std::string >::value_type("mu"),	                                // 9 
	std::vector< std::string >::value_type("ionic_strength"),	                    // 10
	std::vector< std::string >::value_type("ah2o"),	                                // 11
	std::vector< std::string >::value_type("activity_water"),	                    // 12
	std::vector< std::string >::value_type("total_h"),	                            // 13
	std::vector< std::string >::value_type("total_o"),	                            // 14
	std::vector< std::string >::value_type("mass_water"),	                        // 15
	std::vector< std::string >::value_type("mass_h2o"),	                            // 16
	std::vector< std::string >::value_type("total_alkalinity"),	                    // 17
	std::vector< std::string >::value_type("total_alk"),	                        // 18
	std::vector< std::string >::value_type("cb"),	                                // 19
	std::vector< std::string >::value_type("charge_balance"),	                    // 20
	std::vector< std::string >::value_type("density") 	                            // 21
};									   
const std::vector< std::string > cxxSolution::vopts(temp_vopts, temp_vopts + sizeof temp_vopts / sizeof temp_vopts[0]);	