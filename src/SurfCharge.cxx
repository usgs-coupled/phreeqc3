// SurfCharge.cxx: implementation of the cxxSurfCharge class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)   // disable truncation warning (Only used by debugger)
#endif

#include "Utils.h"   // define first
#include "SurfCharge.h"
#define EXTERNAL extern
#include "global.h"
#include "output.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include <cassert>     // assert
#include <algorithm>   // std::sort 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxSurfCharge::cxxSurfCharge()
        //
        // default constructor for cxxSurfCharge 
        //
{
        name                    = NULL;
        specific_area           = 0.0;
        grams                   = 0.0;
        charge_balance          = 0.0;
        mass_water              = 0.0;
        la_psi                  = 0.0;
        diffuse_layer_totals.type = cxxNameDouble::ND_ELT_MOLES;
}

cxxSurfCharge::cxxSurfCharge(struct surface_charge *surf_charge_ptr)
        //
        // constructor for cxxSurfCharge from struct surface_charge
        //
: 
diffuse_layer_totals(surf_charge_ptr->diffuse_layer_totals)
{
        name                     = surf_charge_ptr->name;
        specific_area            = surf_charge_ptr->specific_area;
        grams                    = surf_charge_ptr->grams;
        charge_balance           = surf_charge_ptr->charge_balance;
        mass_water               = surf_charge_ptr->mass_water;
        la_psi                   = surf_charge_ptr->la_psi;
}

cxxSurfCharge::~cxxSurfCharge()
{
}

struct master *cxxSurfCharge::get_psi_master()
{       
        struct master *master_ptr = NULL;
        std::string str = this->name;

        str.append("_psi");
        master_ptr = master_bsearch(str.c_str());
        if (master_ptr == NULL) {
                std::ostringstream error_oss;
                error_oss << "Surface charge psi_master not found." << this->name << std::endl;
                //Utilities::error_msg(error_oss.str(), CONTINUE);
		error_msg(error_oss.str().c_str(), CONTINUE);
        }
        return(master_ptr);
}

struct surface_charge *cxxSurfCharge::cxxSurfCharge2surface_charge(std::list<cxxSurfCharge>& el)
        //
        // Builds surface_charge structure from of cxxSurfCharge 
        //
{
        struct surface_charge *surf_charge_ptr = (struct surface_charge *) PHRQ_malloc((size_t) (el.size() * sizeof(struct surface_charge)));
        if (surf_charge_ptr == NULL) malloc_error();

        int i = 0;
        for (std::list<cxxSurfCharge>::iterator it = el.begin(); it != el.end(); ++it) {
                surf_charge_ptr[i].name                 = it->name;
                surf_charge_ptr[i].specific_area        = it->specific_area;
                surf_charge_ptr[i].grams                = it->grams;
                surf_charge_ptr[i].charge_balance       = it->charge_balance;
                surf_charge_ptr[i].mass_water           = it->mass_water;
                surf_charge_ptr[i].la_psi               = it->la_psi;
                surf_charge_ptr[i].diffuse_layer_totals = it->diffuse_layer_totals.elt_list();
                surf_charge_ptr[i].psi_master           = it->get_psi_master();
                surf_charge_ptr[i].count_g              = 0;
                surf_charge_ptr[i].g                    = NULL;
                i++;
        }
        return(surf_charge_ptr);
}

void cxxSurfCharge::dump_xml(std::ostream& s_oss, unsigned int indent)const
{
        //const char    ERR_MESSAGE[] = "Packing surf_charge message: %s, element not found\n";
        unsigned int i;
        s_oss.precision(DBL_DIG - 1);
        std::string indent0(""), indent1(""), indent2("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);
        for(i = 0; i < indent + 1; ++i) indent1.append(Utilities::INDENT);
        for(i = 0; i < indent + 2; ++i) indent2.append(Utilities::INDENT);

        // Surf_Charge element and attributes

        s_oss << indent0 << "name=\"" << this->name << "\"" << std::endl;
        s_oss << indent0 << "specific_area=\"" << this->specific_area  << "\"" << std::endl;
        s_oss << indent0 << "grams=\"" << this->grams     << "\"" << std::endl;
        s_oss << indent0 << "charge_balance=\"" << this->charge_balance << "\"" << std::endl;
        s_oss << indent0 << "mass_water=\"" << this->mass_water << "\"" << std::endl;
        s_oss << indent0 << "la_psi=\"" << this->la_psi << "\"" << std::endl;

        // totals
        s_oss << indent0;
        s_oss << "<diffuse_layer_totals " << std::endl;
        this->diffuse_layer_totals.dump_xml(s_oss, indent + 1);

}

void cxxSurfCharge::dump_raw(std::ostream& s_oss, unsigned int indent)const
{
        //const char    ERR_MESSAGE[] = "Packing surf_charge message: %s, element not found\n";
        unsigned int i;
        s_oss.precision(DBL_DIG - 1);
        std::string indent0(""), indent1(""), indent2("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);
        for(i = 0; i < indent + 1; ++i) indent1.append(Utilities::INDENT);
        for(i = 0; i < indent + 2; ++i) indent2.append(Utilities::INDENT);

        // Surf_Charge element and attributes

        s_oss << indent0 << "-name                  " << this->name << std::endl;
        s_oss << indent0 << "-specific_area         " << this->specific_area  << std::endl;
        s_oss << indent0 << "-grams                 " << this->grams     << std::endl;
        s_oss << indent0 << "-charge_balance        " << this->charge_balance     << std::endl;
        s_oss << indent0 << "-mass_water            " << this->mass_water  << std::endl;
        s_oss << indent0 << "-la_psi                " << this->la_psi << std::endl;

        // totals
        s_oss << indent0;
        s_oss << "-diffuse_layer_totals" << std::endl;
        this->diffuse_layer_totals.dump_raw(s_oss, indent + 1);

}

void cxxSurfCharge::read_raw(CParser& parser)
{
        std::string str;
        
        static std::vector<std::string> vopts;
        if (vopts.empty()) {
                vopts.reserve(10);
                vopts.push_back("name");                       // 0 
                vopts.push_back("specific_area");              // 1 
                vopts.push_back("grams");                      // 2 
                vopts.push_back("charge_balance");             // 3 
                vopts.push_back("mass_water");                 // 4 
                vopts.push_back("la_psi");                     // 5 
                vopts.push_back("diffuse_layer_totals");       // 6 
        }                                                      

        std::istream::pos_type ptr;
        std::istream::pos_type next_char;
        std::string token;
        int opt_save;

        opt_save = CParser::OPT_ERROR;
        bool name_defined(false); 
        bool specific_area_defined(false); 
        bool grams_defined(false); 
        bool charge_balance_defined(false); 
        bool mass_water_defined(false); 
        bool la_psi_defined(false); 

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
                        //parser.error_msg("Unknown input in SURF_CHARGE read.", CParser::OT_CONTINUE);
                        //parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
                        break;

                case 0: // name
                        if (!(parser.get_iss() >> str))
                        {
                                this->name = NULL;
                                parser.incr_input_error();
                                parser.error_msg("Expected string value for name.", CParser::OT_CONTINUE);
                        } else {
                                this->name = string_hsave(str.c_str());
                        }
                        name_defined = true;
                        break;

                case 1: // specific_area
                        if (!(parser.get_iss() >> this->specific_area))
                        {
                                this->specific_area = 0;
                                parser.incr_input_error();
                                parser.error_msg("Expected numeric value for specific_area.", CParser::OT_CONTINUE);
                        }
                        specific_area_defined = true;
                        break;

                case 2: // grams
                        if (!(parser.get_iss() >> this->grams))
                        {
                                this->grams = 0;
                                parser.incr_input_error();
                                parser.error_msg("Expected numeric value for grams.", CParser::OT_CONTINUE);
                        }
                        grams_defined = true;
                        break;


                case 3: // charge_balance
                        if (!(parser.get_iss() >> this->charge_balance))
                        {
                                this->charge_balance = 0;
                                parser.incr_input_error();
                                parser.error_msg("Expected numeric value for charge_balance.", CParser::OT_CONTINUE);
                        }
                        charge_balance_defined = true;
                        break;

                case 4: // mass_water
                        if (!(parser.get_iss() >> this->mass_water))
                        {
                                this->mass_water = 0;
                                parser.incr_input_error();
                                parser.error_msg("Expected numeric value for mass_water.", CParser::OT_CONTINUE);
                        }
                        mass_water_defined = true;
                        break;


                case 5: // la_psi
                        if (!(parser.get_iss() >> this->la_psi))
                        {
                                this->la_psi = 0;
                                parser.incr_input_error();
                                parser.error_msg("Expected numeric value for la_psi.", CParser::OT_CONTINUE);
                        }
                        la_psi_defined = true;
                        break;


                case 6: // diffuse_layer_totals
                        if ( this->diffuse_layer_totals.read_raw(parser, next_char) != CParser::PARSER_OK) {
                                parser.incr_input_error();
                                parser.error_msg("Expected element name and molality for SurfCharge diffuse_layer_totals.", CParser::OT_CONTINUE);
                        }                               
                        opt_save = 6;
                        break;

                }
                if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD) break;
        }
        // members that must be defined
        if (name_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Name not defined for SurfCharge input.", CParser::OT_CONTINUE);
        }
        if (specific_area_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Specific_area not defined for SurfCharge input.", CParser::OT_CONTINUE);
        }
        if (grams_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Grams not defined for SurfCharge input.", CParser::OT_CONTINUE);
        }
        if (charge_balance_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Charge_balance not defined for SurfCharge input.", CParser::OT_CONTINUE);
        }
        if (mass_water_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Mass_water not defined for SurfCharge input.", CParser::OT_CONTINUE);
        }
        if (la_psi_defined == false) {
                parser.incr_input_error();
                parser.error_msg("La_psi not defined for SurfCharge input.", CParser::OT_CONTINUE);
        }
}

#ifdef SKIP
cxxSurfCharge& cxxSurfCharge::read(CParser& parser)
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

        cxxSurfCharge numkey;

        // Read surf_charge number and description
        numkey.read_number_description(parser);

        std::istream::pos_type ptr;
        std::istream::pos_type next_char;
        std::string token;
        CParser::TOKEN_TYPE j;
        
        //cxxSurfCharge& sol = s_map[numkey.n_user()];
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
                        parser.error_msg("Unknown input in SURF_CHARGE keyword.", CParser::OT_CONTINUE);
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
                                sol.surf_charge_pe = conc.get_input_conc();
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
                                parser.error_msg("Expected numeric value for mass of water in surf_charge.", CParser::OT_CONTINUE);
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

