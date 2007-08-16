// ISolution.cxx: implementation of the cxxSolutionxx class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)   // disable truncation warning (Only used by debugger)
#endif

#include "ISolution.h"
#include "ISolutionComp.h"
#define EXTERNAL extern
#include "global.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include "output.h"

#include <cassert>     // assert
#include <algorithm>   // std::sort 
#include <sstream>
#ifdef ORCHESTRA
extern void ORCH_write_chemistry_species(std::ostream &chemistry_dat);
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//static std::map<int, cxxISolution> ss_map;
//std::map<int, cxxISolution>& cxxISolution::s_map = ss_map;

cxxISolution::cxxISolution()
: 
units("mMol/kgw")
{
        density     = 1.0;
        default_pe  = -1;
	pes = NULL;
}

cxxISolution::cxxISolution(struct solution *solution_ptr)
: cxxSolution(solution_ptr)
        //, pe(cxxPe_Data::alloc())
{
        density     = solution_ptr->density;
        units       = solution_ptr->units;
        // totals
        for (int i = 0; solution_ptr->totals[i].description != NULL; i++) {
                cxxISolutionComp c(&(solution_ptr->totals[i]));
                //comps.insert(solution_ptr->totals[i].description, c);
		comps[solution_ptr->totals[i].description] = c;
        }
        default_pe  = solution_ptr->default_pe;
        // pe_data
        pes = pe_data_dup(solution_ptr->pe);
}

cxxISolution::~cxxISolution()
{
        pe_data_free(this->pes);
}

struct solution *cxxISolution::cxxISolution2solution()
        //
        // Builds a solution structure from instance of cxxISolution 
        //
{
        struct solution *soln_ptr    = this->cxxSolution2solution();
        soln_ptr->new_def            = TRUE;
        soln_ptr->density            = this->density;
        soln_ptr->units              = string_hsave(this->units.c_str());
        soln_ptr->default_pe         = this->default_pe;
        // pe
        soln_ptr->pe = (struct pe_data *) pe_data_free(soln_ptr->pe);
        soln_ptr->pe = pe_data_dup(this->pes);
        // totals
        soln_ptr->totals = (struct conc *) free_check_null(soln_ptr->totals);
        soln_ptr->totals = cxxISolutionComp::cxxISolutionComp2conc(this->comps);
        return(soln_ptr);
}
void cxxISolution::ConvertUnits()
  //
  // Converts from input units to moles per kilogram water
  //
{
  double sum_solutes = 0;
  // foreach conc
  std::map<char *, cxxISolutionComp, CHARSTAR_LESS>::iterator iter = this->comps.begin();
  for(; iter != this->comps.end(); ++iter)
  {
    struct master *master_ptr = master_bsearch (iter->first);
    if (master_ptr != NULL && (master_ptr->minor_isotope == TRUE)) continue;
    if (iter->second.get_description() == "H(1)" || iter->second.get_description() == "E") continue;
    if (iter->second.get_input_conc() <= 0.0) continue;
/*
*   Convert liters to kg solution
*/
    double moles = iter->second.get_input_conc();
    if (this->units.find("/l") != std::string::npos )
    {
	moles /= this->density;
    }
/*
* Convert to moles
*/
    //set gfw for element
    iter->second.set_gfw();
    // convert to moles
    if (iter->second.get_units().find("g/") != std::string::npos)
    {
      if (iter->second.get_gfw() != 0)
      {
	moles /= iter->second.get_gfw();
      }
      else 
      {
	std::ostringstream oss;
	oss << "Could not find gfw, " <<  iter->second.get_description();
	error_msg(oss.str().c_str(), CONTINUE);
	input_error++;
      }
    }
/*
*   Convert milli or micro
*/
    char c = iter->second.get_units().c_str()[0];
    if (c == 'm')
    {
	moles *= 1e-3;
    }
    else if (c == 'u')
    {
	moles *= 1e-6;
    }
    iter->second.set_moles(moles);
/*
*   Sum grams of solute, convert from moles necessary
*/
    sum_solutes += moles * (iter->second.get_gfw());
  }
/*
 *   Convert /kgs to /kgw
 */
  double mass_water;
  if ((this->units.find("kgs") != std::string::npos) ||
    (this->units.find("/l") != std::string::npos))
  {
    mass_water = 1.0 - 1e-3 * sum_solutes;
    for(; iter != this->comps.end(); ++iter)
    {
      iter->second.set_moles(iter->second.get_moles() / mass_water);
    }
  }
/*
 *   Scale by mass of water in solution
 */
  mass_water = this->mass_water;
  for(; iter != this->comps.end(); ++iter)
  {
    iter->second.set_moles(iter->second.get_moles() * mass_water);
  }
}
#ifdef SKIP
cxxISolution& cxxISolution::read(CParser& parser)
{
        static std::vector<std::string> vopts;
        if (vopts.empty()) {
                vopts.reserve(11);
                vopts.push_back("temp");         // 0
                vopts.push_back("temperature");  // 1
                vopts.push_back("dens");         // 2
                vopts.push_back("density");      // 3
                vopts.push_back("units");        // 4
                vopts.push_back("redox");        // 5
                vopts.push_back("ph");           // 6
                vopts.push_back("pe");           // 7
                vopts.push_back("unit");         // 8
                vopts.push_back("isotope");      // 9
                vopts.push_back("water");        // 10
        }
        // const int count_opt_list = vopts.size();

        cxxISolution numkey;

        // Read solution number and description
        numkey.read_number_description(parser);

        // Malloc space for solution data
        //// g_solution_map[numkey.n_user()] = numkey;
        s_map[numkey.n_user()] = numkey;

        std::istream::pos_type ptr;
        std::istream::pos_type next_char;
        std::string token;
        CParser::TOKEN_TYPE j;
        
        cxxISolution& sol = s_map[numkey.n_user()];
        int default_pe = 0;

        for (;;)
        {
                int opt = parser.get_option(vopts, next_char);
                if (opt == CParser::OPTION_DEFAULT)
                {
                        ptr = next_char;
                        if (parser.copy_token(token, ptr) == CParser::TT_DIGIT) {
                                opt = 9;
                        }
                }

                switch (opt)
                {
                case CParser::OPTION_EOF:
                        break;
                case CParser::OPTION_KEYWORD:
                        break;
                case CParser::OPTION_ERROR:
                        opt = CParser::OPTION_EOF;
                        CParser::error_msg("Unknown input in SOLUTION keyword.", CParser::OT_CONTINUE);
                        CParser::error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
                        break;

                case 0: // temp
                case 1: // temperature                  
                        if (!(parser.get_iss() >> sol.tc))
                        {
                                sol.tc = 25;
                        }
                        break;

                case 2: // dens
                case 3: // density
                        parser.get_iss() >> sol.density;
                        break;

                case 4: // units
                case 8: // unit
                        if (parser.copy_token(token, next_char) == CParser::TT_EMPTY) break;
                        if (parser.check_units(token, false, false, sol.units, true) == CParser::OK) {
                                sol.units = token;                              
                        } else {
                                parser.incr_input_error();
                        }
                        break;

                case 5: // redox
                        if (parser.copy_token(token, next_char) == CParser::TT_EMPTY) break;
                        if (parser.parse_couple(token) == CParser::OK) {
                                default_pe = cxxPe_Data::store(sol.pe, token);
                        } else {
                                parser.incr_input_error();
                        }
                        break;

                case 6: // ph
                        {
                                cxxISolutionComp conc;
                                if (conc.read(parser, sol) == cxxISolutionComp::ERROR) {
                                        parser.incr_input_error();
                                        break;
                                }
                                sol.ph = conc.get_input_conc();
                                if (conc.get_equation_name().empty()) {
                                        break;
                                }
                                conc.set_description("H(1)");
                                sol.add(conc);
                        }
                        break;

                case 7: // pe
                        {
                                cxxISolutionComp conc;
                                if (conc.read(parser, sol) == cxxISolutionComp::ERROR) {
                                        parser.incr_input_error();
                                        break;
                                }
                                sol.solution_pe = conc.get_input_conc();
                                if (conc.get_equation_name().empty()) {
                                        break;
                                }
                                conc.set_description("E");
                                sol.add(conc);
                        }
                        break;

                case 9: // isotope
                        {
                                cxxIsotope isotope;
                                if (isotope.read(parser) == cxxIsotope::OK) {
                                        sol.add(isotope);
                                }
                        }
                        break;

                case 10: // water
                        j = parser.copy_token(token, next_char);
                        if (j == CParser::TT_EMPTY) {
                                sol.mass_water = 1.0;
                        } else if (j != CParser::TT_DIGIT) {
                                parser.incr_input_error();
                                parser.error_msg("Expected numeric value for mass of water in solution.", CParser::OT_CONTINUE);
                        } else {
                                std::istringstream(token) >> sol.mass_water;
                        }
                        break;

                case CParser::OPTION_DEFAULT:
                        {
                                //  Read concentration
                                cxxISolutionComp conc;
                                if (conc.read(parser, sol) == cxxISolutionComp::ERROR) {
                                        parser.incr_input_error();
                                } else {
                                        sol.add(conc);
                                }
                        }
                        break;
                }
                if (opt == CParser::OPTION_EOF || opt == CParser::OPTION_KEYWORD) break;
        }
#ifdef SKIP
        //
        // Sort totals by description
        //
        std::sort(sol.totals.begin(), sol.totals.end());
#endif

        //
        // fix up default units and default pe
        //
        std::string token1;
        std::vector<cxxISolutionComp>::iterator iter = sol.totals.begin();
        for (; iter != sol.totals.end(); ++iter)
        {
                token = (*iter).get_description();
                Utilities::str_tolower(token);
                if ((*iter).get_units().empty()) {
                        (*iter).set_units(sol.units);
                } else {
                        bool alk = false;
                        if (token.find("alk") == 0) alk = true;
                        token1 = (*iter).get_units();
                        if (parser.check_units(token1, alk, true, sol.get_units(), true) == CParser::ERROR) {
                                parser.incr_input_error();
                        } else {
                                (*iter).set_units(token1);
                        }
                }
                if ((*iter).get_n_pe() < 0) {
                        (*iter).set_n_pe(default_pe);
                }
        }
        sol.default_pe = default_pe;
        return sol;
}
#endif
#ifdef SKIP
void cxxISolution::dump_xml(std::ostream& os, unsigned int indent)const
{
        unsigned int i;

        for(i = 0; i < indent; ++i) os << Utilities::INDENT;            
        os << "<solution>\n";

        cxxNumKeyword::dump_xml(os, indent);

        for(i = 0; i < indent + 1; ++i) os << Utilities::INDENT;                
        os << "<temp>" << this->get_tc() << "</temp>" << "\n";

        for(i = 0; i < indent + 1; ++i) os << Utilities::INDENT;                
        os << "<pH>" << this->get_ph() << "</pH>" << "\n";

        for(i = 0; i < indent + 1; ++i) os << Utilities::INDENT;                
        os << "<pe>" << this->get_solution_pe() << "</pe>" << "\n";

        assert(this->pe.size() > 0);
        assert(this->default_pe >= 0);
        assert(this->pe.size() > (unsigned int) this->default_pe);
        //this->pe[this->default_pe].dump_xml(os, indent + 1);

        for(i = 0; i < indent + 1; ++i) os << Utilities::INDENT;                
        os << "<units>" << this->get_units() << "</units>" << "\n";

        for(i = 0; i < indent + 1; ++i) os << Utilities::INDENT;                
        os << "<density>" << this->get_density() << "</density>" << "\n";

        // foreach conc
        if (!this->totals.empty())
        {
                for(i = 0; i < indent + 1; ++i) os << Utilities::INDENT;                
                os << "<totals>\n";

                std::vector<cxxISolutionComp>::const_iterator iter = this->totals.begin();
                for(; iter != this->totals.end(); ++iter)
                {
                        (*iter).dump_xml(*this, os, indent + 2);
                }

                for(i = 0; i < indent + 1; ++i) os << Utilities::INDENT;                
                os << "</totals>\n";
        }

        // foreach isotope
        if (!this->isotopes.empty())
        {
                for(i = 0; i < indent + 1; ++i) os << Utilities::INDENT;                
                os << "<isotopes>\n";

                std::list<cxxIsotope>::const_iterator iter = this->isotopes.begin();
                for(; iter != this->isotopes.end(); ++iter)
                {
                        (*iter).dump_xml(os, indent + 2);
                }

                for(i = 0; i < indent + 1; ++i) os << Utilities::INDENT;                
                os << "</isotopes>\n";
        }

        for(i = 0; i < indent + 1; ++i) os << Utilities::INDENT;                
        os << "<water>" << this->get_mass_water() << "</water>" << "\n";

        for(i = 0; i < indent; ++i) os << Utilities::INDENT;            
        os << "</solution>" << "\n";
}
#endif

#ifdef ORCHESTRA
void cxxISolution::ORCH_write_chemistry(std::ostream &chemistry_dat)
{
  this->ORCH_write_chemistry_water(chemistry_dat);
  this->ORCH_write_chemistry_primary(chemistry_dat);
  this->ORCH_write_chemistry_total_O_H(chemistry_dat);
  this->ORCH_write_chemistry_alkalinity(chemistry_dat);
  ORCH_write_chemistry_species(chemistry_dat);
  this->ORCH_write_chemistry_minerals(chemistry_dat);
}


void cxxISolution::ORCH_write_chemistry_water(std::ostream &chemistry_dat)
{
  //
  //  Write water entities
  //
  chemistry_dat << std::endl << "//********* The water entities" << std::endl;

  //  e-
  chemistry_dat << "@entity(e-, diss, 0)" << std::endl;
  if (this->comps.find("E") == this->comps.end() || this->comps.find("E")->second.get_equation_name() == NULL) 
  {
    // fixed pe
    chemistry_dat << "@Calc: (1, \"e-.act = 10^-pe\")" << std::endl;

  } else if (this->comps.find("E")->second.get_equation_name() == string_hsave("charge"))
  {
    // charge balance
    chemistry_dat << "@Calc: (1, \"pe = -log10({e-.act})\")" << std::endl;
    chemistry_dat << "@solve (e-.act, 1e-6, log, 1, chargebalance,  1e-14)" << std::endl;
  } else
  {
    // adjust to equilbirium with a phase
    chemistry_dat << "@Calc: (1, \"pe = -log10({e-.act})\")" << std::endl;
    int n;
    struct phase *phase_ptr = phase_bsearch(this->comps.find("E)")->second.get_equation_name(), &n, FALSE);
    assert (phase_ptr != NULL);
    std::string phase_name(phase_ptr->name);
    std::replace(phase_name.begin(), phase_name.end(), '(', '[');
    std::replace(phase_name.begin(), phase_name.end(), ')', ']');
    chemistry_dat << "@solve (e-.act, 1e-6, log, 1, " << phase_name.c_str() << ".si_raw, 1e-9)" << std::endl;
  }
  
  //  H+
  if (this->comps.find("H(1)") == this->comps.end() || this->comps.find("H(1)")->second.get_equation_name() == NULL) 
  {
    // fixed pH
    chemistry_dat << "@Calc: (1, \"H+.act = 10^-pH\")" << std::endl;

  } else if (this->comps.find("H(1)")->second.get_equation_name() == string_hsave("charge"))
  {
    // charge balance
    chemistry_dat << "@Calc: (1, \"pH = -log10({H+.act})\")" << std::endl;
    chemistry_dat << "@solve (H+.act, 1e-6, log, 1, chargebalance,  1e-14)" << std::endl;
  } else
  {
    // adjust to equilbirium with a phase
    chemistry_dat << "@Calc: (1, \"pH = -log10({H+.act})\")" << std::endl;
    int n;
    struct phase *phase_ptr = phase_bsearch(this->comps.find("H(1)")->second.get_equation_name(), &n, FALSE);
    assert (phase_ptr != NULL);
    chemistry_dat << "@solve (H+.act, 1e-6, log, 1, " << phase_ptr->name << ".si_raw, 1e-9)" << std::endl;
  }

  // H2O
  chemistry_dat << "@entity(" << s_h2o->name << ", diss, 55.506)" << std::endl;
  chemistry_dat << std::endl;
}

void cxxISolution::ORCH_write_chemistry_primary(std::ostream &chemistry_dat)
{
  chemistry_dat << std::endl << "//********* The primary species" << std::endl;
  //
  //  Write other master species definitions, i.e. primary entities
  //
  std::map<char *, cxxISolutionComp, CHARSTAR_LESS>::iterator iter = this->comps.begin();
  chemistry_dat << "@species(H+, 1)" << std::endl;
  for(; iter != this->comps.end(); ++iter)
  {
    std::string name(iter->second.get_description());
    if (name == "H(1)" || name == "E" || name == "Alkalinity") continue;
    struct element *elt;
    char *element_name = string_hsave(name.c_str());
    elt = element_store(element_name);
    assert(elt != NULL);
    struct species *s_ptr;
    s_ptr = elt->master->s;
    assert(s_ptr != NULL);
    chemistry_dat << "@species(" << s_ptr->name << ", " << s_ptr->z << ")" << std::endl;
    if (iter->second.get_equation_name() == NULL) 
    {
      // regular mole balance
      chemistry_dat << "@primary_entity(" << s_ptr->name << ", 1e-9, diss, 1.0e-9)" << std::endl;
    } else
    {
      std::string eqn(iter->second.get_equation_name());
      if (eqn == "charge") 
      {
	// charge balance
	chemistry_dat << "@solve (" << s_ptr->name << ".act, 1e-6, log, 1, chargebalance, 1e-9)" << std::endl; 
      } else 
      {
	// adjust to phase equilibrium
	int n;
	struct phase *phase_ptr = phase_bsearch(eqn.c_str(), &n, FALSE);
	assert (phase_ptr != NULL);
	std::string phase_name(phase_ptr->name);
	std::replace(phase_name.begin(), phase_name.end(), '(', '[');
	std::replace(phase_name.begin(), phase_name.end(), ')', ']');
	chemistry_dat << "@solve (" << s_ptr->name << ".act, 1e-6, log, 1, " << phase_name << ".si_raw, 1e-9)" << std::endl;
      }
    }
  } 
  chemistry_dat << std::endl;
}

void cxxISolution::ORCH_write_chemistry_total_O_H(std::ostream &chemistry_dat)
{
  chemistry_dat << std::endl << "//********* Define total hydrogen and oxygen" << std::endl;
  // Define total hydrogen, total oxygen, and difference
  //chemistry_dat << "@var: total_diff 0" << std::endl;
  //chemistry_dat << "@calc: (5, \"total_diff = total_hydrogen - 2*total_oxygen" << "\")" << std::endl;

  // Write H total equation
  chemistry_dat << "@var: total_hydrogen 0" << std::endl;
  chemistry_dat << "@calc: (5, \"total_hydrogen = 0";
  int i;
  for (i = 0; i < count_s_x; i++) 
  {
    // write in terms of orchestra components
    if (s_x[i]->primary != NULL || (s_x[i]->secondary != NULL && s_x[i]->secondary->in == TRUE))
    {
      if (s_x[i]->h != 0) 
      {
	chemistry_dat << "+";
	if (s_x[i]->h != 1)
	{
	  chemistry_dat << s_x[i]->h << "*";
	}
	chemistry_dat << "{" << s_x[i]->name << ".diss}";
      }
    }
  } 
  chemistry_dat << "\")" << std::endl;

  // Write O total equation
  chemistry_dat << "@var: total_oxygen 0" << std::endl;
  chemistry_dat << "@calc: (5, \"total_oxygen = 0";
  for (i = 0; i < count_s_x; i++) 
  {
    if (s_x[i]->o != 0) 
    {
      // write in terms of orchestra components
      if (s_x[i]->primary != NULL || (s_x[i]->secondary != NULL && s_x[i]->secondary->in == TRUE))
      {
	chemistry_dat << "+";
	if (s_x[i]->o != 1)
	{
	  chemistry_dat << s_x[i]->o << "*";
	}
	chemistry_dat << "{" << s_x[i]->name << ".diss}";
      }
    }
  } 
  chemistry_dat << "\")" << std::endl;
  chemistry_dat << std::endl;
}
void cxxISolution::ORCH_write_chemistry_alkalinity(std::ostream &chemistry_dat)
{
  chemistry_dat << std::endl << "//********* Alkalinity definitions" << std::endl;
  //
  // Define alkalinity 
  //
  chemistry_dat << "@Var: Alkalinity 0" << std::endl;
  chemistry_dat << "@calc: (5, \"Alkalinity = 0";
  for (int i = 0; i < count_s_x; i++) 
  {
    if (s_x[i]->alk == 0) continue;
    std::string name(s_x[i]->name);
    std::replace(name.begin(), name.end(), '(', '[');
    std::replace(name.begin(), name.end(), ')', ']');
    if (s_x[i]->alk < 0) 
    {
	if (s_x[i]->alk != -1) 
	{
	  chemistry_dat << s_x[i]->alk << "*{" << name << ".con}";
	} else 
	{
	  chemistry_dat << "-{" << name << ".con}";
	}
    } else if (s_x[i]->alk > 0)
    {
      if (s_x[i]->alk != 1) 
      {
	chemistry_dat << "+" << s_x[i]->alk << "*{" << name << ".con}";
      } else 
      {
	chemistry_dat << "+{" << name << ".con}";
      }
    }
  }
  chemistry_dat << "\")" << std::endl;
  //
  //  Alkalinity (or pH) equation
  //
  std::map<char *, cxxISolutionComp, CHARSTAR_LESS>::iterator iter = this->comps.begin();
  if ((iter = this->comps.find("Alkalinity")) != this->comps.end())
  {
    if ((this->comps.find("C(4)") != this->comps.end()) || (this->comps.find("C") != this->comps.end()))
    {
      if (this->comps.find("H(1)") != this->comps.end() )
      {
	std::ostringstream oss;
	oss << "pH can not be adjusted to charge balance or phase equilibrium when specifying C or C(4) and Alkalinty.";
	error_msg(oss.str().c_str(), CONTINUE);
	input_error++;
      }
      chemistry_dat << "@solve (pH, 1e-6, lin, 1, Alkalinity, 7)" << std::endl;
      chemistry_dat << std::endl;
    } else
    {
      struct master *master_ptr = master_bsearch("Alkalinity");
      if (master_ptr == NULL)
      {
	  std::ostringstream oss;
	  oss << "Could not find Alkalinity definition in database.";
	  error_msg(oss.str().c_str(), CONTINUE);
	  input_error++;
      }
      chemistry_dat << "@species(" << master_ptr->s->name << ", " << master_ptr->s->z << ")" << std::endl;
      chemistry_dat << "@solve (" << master_ptr->s->name << ".act, 1e-6, log, 1, Alkalinity, 1e-9)" << std::endl;
      chemistry_dat << std::endl; 
    }
  }
}

void cxxISolution::ORCH_write_chemistry_minerals(std::ostream &chemistry_dat)
{
  chemistry_dat << std::endl << "//********* Adjustments to mineral equilibrium" << std::endl;
//
// Write minerals
//
  std::map<char *, cxxISolutionComp, CHARSTAR_LESS>::iterator iter = this->comps.begin();
  for(iter = this->comps.begin(); iter != this->comps.end(); ++iter)
  {
    if (iter->second.get_equation_name() != NULL) 
    {
      std::string name(iter->second.get_equation_name());
      if (name != "charge") 
      {
	struct phase *phase_ptr;
	int n;
	phase_ptr = phase_bsearch(name.c_str(), &n, FALSE);
	assert (phase_ptr != NULL);
	std::string phase_name(phase_ptr->name);
	std::replace(phase_name.begin(), phase_name.end(), '(', '[');
        std::replace(phase_name.begin(), phase_name.end(), ')', ']');
	chemistry_dat << "@si_mineral(" << phase_name << ")" << std::endl;
	chemistry_dat << "@reaction(" << phase_name << ", " << pow(10.0,  -phase_ptr->lk);
	struct rxn_token *next_token = phase_ptr->rxn_x->token;
	next_token++;
	while (next_token->s != NULL || next_token->name != NULL)
	{
	  chemistry_dat << ", " << next_token->coef;
	  if (next_token->s != NULL)
	  {
	    chemistry_dat << ", " << next_token->s->name;
	  } else {
	    chemistry_dat << ", " << next_token->name;
	  }
	  next_token++;
	}
	chemistry_dat << ")" << std::endl;
      }
    }
  }
  //chemistry_dat << "@mineral(Quartz)" << std::endl;
  //chemistry_dat << "@sreaction(Quartz, 10139.1138573668, -2.0, H2O, 1.0, H4SiO4)" << std::endl;
}
void cxxISolution::ORCH_write_input(std::ostream &input_dat)
{


//
//  Write orchestra input file info
//
  std::ostringstream headings, data;
  data.precision(DBL_DIG - 1);
  headings << "var:   ";
  data << "data:  ";


  // Solution element and attributes

  //s_oss << "SOLUTION_RAW       " << this->n_user  << " " << this->description << std::endl;

  //s_oss << "-temp              " << this->tc << std::endl;
  headings << "tempc\t";
  data   << this->tc << "\t";

  //s_oss << "-pH                " << this->ph << std::endl;
  headings << "pH\t";
  data   << this->ph << "\t";

  //s_oss << "-pe                " << this->pe << std::endl;
  headings << "pe\t";
  data   << this->pe << "\t";

  //s_oss << "-mu                " << this->mu << std::endl;

  //s_oss << "-ah2o              " << this->ah2o << std::endl;
  headings << "H2O.act\t";
  data   << 1 << "\t";
  //s_oss << "-total_h           " << this->total_h << std::endl;

  //s_oss << "-total_o           " << this->total_o << std::endl;

  //s_oss << "-cb                " << this->cb << std::endl;

  //s_oss << "-mass_water        " << this->mass_water << std::endl;

  //s_oss << "-total_alkalinity  " << this->total_alkalinity << std::endl;

  // soln_total conc structures
  //this->totals.dump_raw(s_oss, indent + 2);
  //this->totals.write_orchestra(headings, s_oss);

  std::map<char *, cxxISolutionComp, CHARSTAR_LESS>::iterator iter = this->comps.begin();
  for (; iter != this->comps.end(); ++iter) 
  {
    std::string master_name;
    struct master *master_ptr;
    master_ptr = master_bsearch (iter->first);
    assert (master_ptr != NULL);
    std::string ename(iter->first);
    double coef = master_ptr->coef;
    if (master_ptr->coef == 0) 
    {
      coef = 1;
    }
    if (ename != "Alkalinity") 
    {
      ename = master_ptr->s->name;
      ename.append(".diss");
    } 

    if (iter->second.get_equation_name() == NULL) 
    {
      headings << ename  << "\t";
      data << (this->totals.find(iter->first))->second / coef << "\t";
    } else
    {
      std::string name(iter->second.get_equation_name());
      if (name == "charge") 
      {
	headings << ename << "\t";
	data << (this->totals.find(iter->first))->second /coef << "\t";
      } else
      {
	int n;
	struct phase *phase_ptr = phase_bsearch(name.c_str(),&n, TRUE);
	assert(phase_ptr != NULL);
	std::string phase_name(phase_ptr->name);
	std::replace(phase_name.begin(), phase_name.end(), '(', '[');
	std::replace(phase_name.begin(), phase_name.end(), ')', ']');
	headings << phase_name << ".si_raw" << "\t";
	data << iter->second.get_phase_si() << "\t";
      }
    }
    //  activity estimate
    if (ename == "Alkalinity")
    {
      if ((this->comps.find("C") == this->comps.end()) && (this->comps.find("C(4)") == this->comps.end())) 
      {
	headings << master_ptr->s->name << ".act\t";
	data << iter->second.get_moles()*1e-3 << "\t";
      }
    }
    else
    {
      headings << master_ptr->s->name << ".act\t";
      data << iter->second.get_moles()*1e-3 << "\t";
    }
  }
  // Isotopes
  //s_oss << "-Isotopes" << std::endl;
  /*
  {
          for (std::list<cxxSolutionIsotope>::const_iterator it = this->isotopes.begin(); it != isotopes.end(); ++it) {
                  it->dump_raw(s_oss, indent + 2);
          }
  }
  */

  // Write data to string
  input_dat << headings.str() << std::endl;
  input_dat << data.str() << std::endl;

  return;
}

void cxxISolution::ORCH_write_output_vars(std::ostream &outstream)
{
  outstream << "Var:";
  outstream << "\tnr_iter";
  //
  //  Serialize solution
  //
  outstream << "\tstart_solution";
  //tc
  outstream << "\ttempc";
  //pH
  outstream << "\tpH";
  //pe
  outstream << "\tpe";
  //mu
  outstream << "\tI";
  //ah2o
  outstream << "\tH2O.act";
  //total_h;
  outstream << "\ttotal_hydrogen";
  //total_o;
  outstream << "\ttotal_oxygen";
  //cb
  outstream << "\tchargebalance";
  //mass_water;
  outstream << "\tH2O.con";
  //total_alkalinity;
  outstream << "\tAlkalinity";
  //orchestra master variables
  outstream << "\tH+.diss";
  outstream << "\te-.diss";
  outstream << "\tH2O.diss";
  //totals 
  for(std::map<char *, cxxISolutionComp, CHARSTAR_LESS>::iterator iter = this->comps.begin(); iter != this->comps.end(); ++iter)
  {
    std::string name(iter->second.get_description());
    if (name == "H(1)" || name == "E" || name == "Alkalinity") continue;
    struct element *elt;
    char *element_name = string_hsave(name.c_str());
    elt = element_store(element_name);
    assert(elt != NULL);
    struct species *s_ptr;
    s_ptr = elt->master->s;
    assert(s_ptr != NULL);
    outstream << "\t" << s_ptr->name << ".diss";
  }
  outstream << "\tend_totals";
  for(std::map<char *, cxxISolutionComp, CHARSTAR_LESS>::iterator iter = this->comps.begin(); iter != this->comps.end(); ++iter)
  {
    std::string name(iter->second.get_description());
    if (name == "H(1)" || name == "E" || name == "Alkalinity") continue;
    struct element *elt;
    char *element_name = string_hsave(name.c_str());
    elt = element_store(element_name);
    assert(elt != NULL);
    struct species *s_ptr;
    s_ptr = elt->master->s;
    assert(s_ptr != NULL);
    outstream << "\t" << s_ptr->name << ".act";
  }
  outstream << "\tend_master_activities";
  //
  // Write all species activities and concentrations
  //
  int i;
  for (i = 0; i < count_s_x; i++) {
    std::string name(s_x[i]->name);
    std::replace(name.begin(), name.end(), '(', '[');
    std::replace(name.begin(), name.end(), ')', ']');
    outstream << "\t" << name.c_str() << ".act" << "\t" << name.c_str() << ".con";
  }
  outstream << "\tend_species";
  outstream << std::endl;
  return;
}
void cxxISolution::ORCH_write(std::ostream &chemistry_dat, std::ostream &input_dat, std::ostream &output_dat)
{
  //
  // Write orchestra chemistry file definition
  //
  chemistry_dat << std::endl << "// Write ORCHESTRA chemistry definitions" << std::endl;
  // mark for Orchestra include 
  chemistry_dat << std::endl << "@class: species_reactions () {" << std::endl;
  this->ORCH_write_chemistry(chemistry_dat);
  // end orchestra include block
  chemistry_dat << std::endl << "}" << std::endl;
  //
  // Write orchestra input file definition
  //
  input_dat << std::endl << "@class: input_file_data () {" << std::endl;
  this->ORCH_write_input(input_dat);
  input_dat << std::endl << "}" << std::endl;

  //
  // Write orchestra output file definition
  //
  output_dat << std::endl << "Output_every: 1" << std::endl;
  this->ORCH_write_output_vars(output_dat);

  //write data to stderr
  //std::cerr << chemistry_dat.str() << input_dat.str() << output_dat.str();

} 
#endif
