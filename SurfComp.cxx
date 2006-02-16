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
