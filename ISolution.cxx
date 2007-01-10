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
