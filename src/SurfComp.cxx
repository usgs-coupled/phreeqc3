// SurfComp.cxx: implementation of the cxxSurfComp class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)   // disable truncation warning (Only used by debugger)
#endif

#include "Utils.h"   // define first
#include "SurfComp.h"
#define EXTERNAL extern
#include "global.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include "output.h"
#include <cassert>     // assert
#include <algorithm>   // std::sort 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxSurfComp::cxxSurfComp()
        //
        // default constructor for cxxSurfComp 
        //
{
        formula                 = NULL;
        moles                   = 0.0;
        totals.type             = cxxNameDouble::ND_ELT_MOLES;
        la                      = 0.0;
        charge_number           = -99;
        charge_balance          = 0;
        phase_name              = NULL;
        phase_proportion        = 0.0;
        rate_name               = NULL;
}

cxxSurfComp::cxxSurfComp(struct surface_comp *surf_comp_ptr)
        //
        // constructor for cxxSurfComp from struct surface_comp
        //
: 
totals(surf_comp_ptr->totals)
{
        formula                  = surf_comp_ptr->formula;
        moles                    = surf_comp_ptr->moles;
        la                       = surf_comp_ptr->la;
        charge_number            = surf_comp_ptr->charge;
        charge_balance           = surf_comp_ptr->cb;
        phase_name               = surf_comp_ptr->phase_name;
        phase_proportion         = surf_comp_ptr->phase_proportion;
        rate_name                = surf_comp_ptr->rate_name;
}

cxxSurfComp::~cxxSurfComp()
{
}

struct master *cxxSurfComp::get_master()
{       
        struct master *master_ptr = NULL;
        //for (std::map <char *, double, CHARSTAR_LESS>::iterator it = totals.begin(); it != totals.end(); it++) {
        for (cxxNameDouble::iterator it = this->totals.begin(); it != this->totals.end(); it++) {
                /* Find master species */
                char *eltName = it->first;
                struct element *elt_ptr = element_store(eltName);
                if (elt_ptr->master == NULL) {
                        std::ostringstream error_oss;
                        error_oss << "Master species not in data base for " << elt_ptr->name << std::endl;
                        //Utilities::error_msg(error_oss.str(), CONTINUE);
			error_msg(error_oss.str().c_str(), CONTINUE);
                        return(NULL);
                }
                if (elt_ptr->master->type != SURF) continue;
                master_ptr = elt_ptr->master;
                break;
        }
        if (master_ptr == NULL) {
                std::ostringstream error_oss;
                error_oss << "Surface formula does not contain an surface master species, " << this->formula << std::endl;
                //Utilities::error_msg(error_oss.str(), CONTINUE);
		error_msg(error_oss.str().c_str(), CONTINUE);        
	}
        return(master_ptr);
}

struct surface_comp *cxxSurfComp::cxxSurfComp2surface_comp(std::list<cxxSurfComp>& el)
        //
        // Builds surface_comp structure from of cxxSurfComp 
        //
{
        struct surface_comp *surf_comp_ptr = (struct surface_comp *) PHRQ_malloc((size_t) (el.size() * sizeof(struct surface_comp)));
        if (surf_comp_ptr == NULL) malloc_error();

        int i = 0;
        for (std::list<cxxSurfComp>::iterator it = el.begin(); it != el.end(); ++it) {
                surf_comp_ptr[i].formula                =  it->formula;
                surf_comp_ptr[i].moles                  =  it->moles;
                surf_comp_ptr[i].master                 =  it->get_master();
                surf_comp_ptr[i].totals                 =  it->totals.elt_list();
                surf_comp_ptr[i].la                     =  it->la;
                surf_comp_ptr[i].charge                 =  it->charge_number;
                surf_comp_ptr[i].cb                     =  it->charge_balance;
                surf_comp_ptr[i].phase_name             =  it->phase_name;
                surf_comp_ptr[i].phase_proportion       =  it->phase_proportion;
                surf_comp_ptr[i].rate_name              =  it->rate_name;
                surf_comp_ptr[i].master                 =  it->get_master();
                i++;
        }
        return(surf_comp_ptr);
}

void cxxSurfComp::dump_xml(std::ostream& s_oss, unsigned int indent)const
{
        //const char    ERR_MESSAGE[] = "Packing surf_comp message: %s, element not found\n";
        unsigned int i;
        s_oss.precision(DBL_DIG - 1);
        std::string indent0(""), indent1(""), indent2("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);
        for(i = 0; i < indent + 1; ++i) indent1.append(Utilities::INDENT);
        for(i = 0; i < indent + 2; ++i) indent2.append(Utilities::INDENT);

        // Surf_Comp element and attributes

        s_oss << indent0 << "formula=\"" << this->formula << "\"" << std::endl;
        s_oss << indent0 << "moles=\"" << this->moles  << "\"" << std::endl;
        s_oss << indent0 << "la=\"" << this->la     << "\"" << std::endl;
        s_oss << indent0 << "charge_number=\"" << this->charge_number   << "\"" << std::endl;
        s_oss << indent0 << "charge_balance=\"" << this->charge_balance << "\"" << std::endl;
        if (this->phase_name != NULL) {
                s_oss << indent0 << "phase_name=\"" << this->phase_name << "\"" << std::endl;
        }
        if (this->rate_name != NULL) {
                s_oss << indent0 << "rate_name=\"" << this->rate_name << "\"" << std::endl;
        }
        s_oss << indent0 << "phase_proportion=\"" << this->phase_proportion  << "\"" << std::endl;

        // totals
        s_oss << indent0;
        s_oss << "<totals " << std::endl;
        this->totals.dump_xml(s_oss, indent + 1);

}

void cxxSurfComp::dump_raw(std::ostream& s_oss, unsigned int indent)const
{
        //const char    ERR_MESSAGE[] = "Packing surf_comp message: %s, element not found\n";
        unsigned int i;
        s_oss.precision(DBL_DIG - 1);
        std::string indent0(""), indent1(""), indent2("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);
        for(i = 0; i < indent + 1; ++i) indent1.append(Utilities::INDENT);
        for(i = 0; i < indent + 2; ++i) indent2.append(Utilities::INDENT);

        // Surf_Comp element and attributes

        s_oss << indent0 << "-formula               " << this->formula << std::endl;
        s_oss << indent0 << "-moles                 " << this->moles  << std::endl;
        s_oss << indent0 << "-la                    " << this->la     << std::endl;
        s_oss << indent0 << "-charge_number         " << this->charge_number  << std::endl;
        s_oss << indent0 << "-charge_balance        " << this->charge_balance << std::endl;
        if (this->phase_name != NULL) {
                s_oss << indent0 << "-phase_name            " << this->phase_name << std::endl;
        }
        if (this->rate_name != NULL) {
                s_oss << indent0 << "-rate_name             " << this->rate_name << std::endl;
        }
        s_oss << indent0 << "-phase_proportion      " << this->phase_proportion  << std::endl;

        // totals
        s_oss << indent0;
        s_oss << "-totals" << std::endl;
        this->totals.dump_raw(s_oss, indent + 1);

}

void cxxSurfComp::read_raw(CParser& parser)
{
        std::string str;
        
        static std::vector<std::string> vopts;
        if (vopts.empty()) {
                vopts.reserve(10);
                vopts.push_back("formula");                   // 0 
                vopts.push_back("moles");                     // 1
                vopts.push_back("la");                        // 2 
                vopts.push_back("charge_number");             // 3 
                vopts.push_back("charge_balance");            // 4
                vopts.push_back("phase_name");                // 5 
                vopts.push_back("rate_name");                 // 6 
                vopts.push_back("phase_proportion");          // 7 
                vopts.push_back("totals");                    // 8
        }

        std::istream::pos_type ptr;
        std::istream::pos_type next_char;
        std::string token;
        int opt_save;

        opt_save = CParser::OPT_ERROR;
        bool formula_defined(false); 
        bool moles_defined(false); 
        bool la_defined(false); 
        bool charge_number_defined(false); 
        bool charge_balance_defined(false); 

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
                        opt = CParser::OPT_KEYWORD;
                        // Allow return to Surface for more processing
                        //parser.error_msg("Unknown input in SURF_COMP read.", CParser::OT_CONTINUE);
                        //parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
                        break;

                case 0: // formula
                        if (!(parser.get_iss() >> str))
                        {
                                this->formula = NULL;
                                parser.incr_input_error();
                                parser.error_msg("Expected string value for formula.", CParser::OT_CONTINUE);
                        } else {
                                this->formula = string_hsave(str.c_str());
                        }
                        formula_defined = true;
                        break;

                case 1: // moles
                        if (!(parser.get_iss() >> this->moles))
                        {
                                this->moles = 0;
                                parser.incr_input_error();
                                parser.error_msg("Expected numeric value for moles.", CParser::OT_CONTINUE);
                        }
                        moles_defined = true;
                        break;

                case 2: // la
                        if (!(parser.get_iss() >> this->la))
                        {
                                this->la = 0;
                                parser.incr_input_error();
                                parser.error_msg("Expected numeric value for la.", CParser::OT_CONTINUE);
                        }
                        la_defined = true;
                        break;

                case 3: // charge_number
                        if (!(parser.get_iss() >> this->charge_number))
                        {
                                this->charge_number = 0;
                                parser.incr_input_error();
                                parser.error_msg("Expected integer value for charge_number.", CParser::OT_CONTINUE);
                        }
                        charge_number_defined = true;
                        break;

                case 4: // charge_balance
                        if (!(parser.get_iss() >> this->charge_balance))
                        {
                                this->charge_balance = 0;
                                parser.incr_input_error();
                                parser.error_msg("Expected numeric value for charge_balance.", CParser::OT_CONTINUE);
                        }
                        charge_balance_defined = true;
                        break;

                case 5: // phase_name
                        if (!(parser.get_iss() >> str))
                        {
                                this->phase_name = NULL;
                                parser.incr_input_error();
                                parser.error_msg("Expected string value for phase_name.", CParser::OT_CONTINUE);
                        } else {
                                this->phase_name = string_hsave(str.c_str());
                        }
                        break;

                case 6: // rate_name
                        if (!(parser.get_iss() >> str))
                        {
                                this->rate_name = NULL;
                                parser.incr_input_error();
                                parser.error_msg("Expected string value for rate_name.", CParser::OT_CONTINUE);
                        } else {
                                this->rate_name = string_hsave(str.c_str());
                        }
                        break;

                case 7: // phase_proportion
                        if (!(parser.get_iss() >> this->phase_proportion))
                        {
                                this->phase_proportion = 0;
                                parser.incr_input_error();
                                parser.error_msg("Expected numeric value for phase_proportion.", CParser::OT_CONTINUE);
                        }
                        break;

                case 8: // totals
                        if ( this->totals.read_raw(parser, next_char) != CParser::PARSER_OK) {
                                parser.incr_input_error();
                                parser.error_msg("Expected element name and molality for SurfComp totals.", CParser::OT_CONTINUE);
                        }                               
                        opt_save = 8;
                        break;

                }
                if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD) break;
        }
        // members that must be defined
        if (formula_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Formula not defined for SurfComp input.", CParser::OT_CONTINUE);
        }
        if (moles_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Moles not defined for SurfComp input.", CParser::OT_CONTINUE);
        }
        if (la_defined == false) {
                parser.incr_input_error();
                parser.error_msg("La not defined for SurfComp input.", CParser::OT_CONTINUE);
        }
        if (charge_number_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Charge_number not defined for SurfComp input.", CParser::OT_CONTINUE);
        }
        if (charge_balance_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Charge_balance not defined for SurfComp input.", CParser::OT_CONTINUE);
        }
}

#ifdef SKIP
cxxSurfComp& cxxSurfComp::read(CParser& parser)
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

        cxxSurfComp numkey;

        // Read surf_comp number and description
        numkey.read_number_description(parser);

        std::istream::pos_type ptr;
        std::istream::pos_type next_char;
        std::string token;
        CParser::TOKEN_TYPE j;
        
        //cxxSurfComp& sol = s_map[numkey.n_user()];
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
                        parser.error_msg("Unknown input in SURF_COMP keyword.", CParser::OT_CONTINUE);
                        parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
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
                                cxxConc conc;
                                if (conc.read(parser, sol) == cxxConc::ERROR) {
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
                                cxxConc conc;
                                if (conc.read(parser, sol) == cxxConc::ERROR) {
                                        parser.incr_input_error();
                                        break;
                                }
                                sol.surf_comp_pe = conc.get_input_conc();
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
                                parser.error_msg("Expected numeric value for mass of water in surf_comp.", CParser::OT_CONTINUE);
                        } else {
                                std::istringstream(token) >> sol.mass_water;
                        }
                        break;

                case CParser::OPTION_DEFAULT:
                        {
                                //  Read concentration
                                cxxConc conc;
                                if (conc.read(parser, sol) == cxxConc::ERROR) {
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
        std::vector<cxxConc>::iterator iter = sol.totals.begin();
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

