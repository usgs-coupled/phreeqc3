// ExchComp.cxx: implementation of the cxxExchComp class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)   // disable truncation warning (Only used by debugger)
#endif

#include "Utils.h"   // define first
#include "ExchComp.h"
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

cxxExchComp::cxxExchComp()
        //
        // default constructor for cxxExchComp 
        //
{
        moles                   = 0.0;
        la                      = 0.0;
        charge_balance          = 0.0;
        phase_name              = NULL;
        phase_proportion        = 0.0;
        rate_name               = NULL;
        totals.type             = cxxNameDouble::ND_ELT_MOLES;
        formula_totals.type     = cxxNameDouble::ND_ELT_MOLES;
}

cxxExchComp::cxxExchComp(struct exch_comp *exch_comp_ptr)
        //
        // constructor for cxxExchComp from struct exch_comp
        //
: 
formula_totals(exch_comp_ptr->formula_totals),
totals(exch_comp_ptr->totals)
{
        formula                  = exch_comp_ptr->formula;
        moles                    = exch_comp_ptr->moles;
        // totals in constructor
        //formula_totals in constructor
        la                       = exch_comp_ptr->la;
        charge_balance           = exch_comp_ptr->charge_balance;
        phase_name               = exch_comp_ptr->phase_name;
        phase_proportion         = exch_comp_ptr->phase_proportion;
        rate_name                = exch_comp_ptr->rate_name;
        formula_z                = exch_comp_ptr->formula_z;
}

cxxExchComp::~cxxExchComp()
{
}
#include <iostream>     // std::cout std::cerr

struct master *cxxExchComp::get_master()
{       
        struct master *master_ptr = NULL;
        for (std::map <char *, double, CHARSTAR_LESS>::iterator it = totals.begin(); it != totals.end(); it++) {

                /* Find master species */
                char *eltName = it->first;
                struct element *elt_ptr = element_store(eltName);
                if (elt_ptr->master == NULL) {
                        std::ostringstream error_oss;
                        error_oss << "Master species not in data base for " << elt_ptr->name << std::endl;
                        //Utilities::error_msg(error_oss.str(), STOP);
			error_msg(error_oss.str().c_str(), CONTINUE);
			return(NULL);
                }
                if (elt_ptr->master->type != EX) continue;
                master_ptr = elt_ptr->master;
                break;
        }
        if (master_ptr == NULL) {
		for (std::map <char *, double, CHARSTAR_LESS>::iterator it = this->formula_totals.begin(); it != formula_totals.end(); it++) {

			/* Find master species */
			char *eltName = it->first;
			struct element *elt_ptr = element_store(eltName);
			if (elt_ptr->master == NULL) {
				std::ostringstream error_oss;
				error_oss << "Master species not in data base for " << elt_ptr->name << std::endl;
				//Utilities::error_msg(error_oss.str(), STOP);
				error_msg(error_oss.str().c_str(), CONTINUE);
				return(NULL);
			}
			if (elt_ptr->master->type != EX) continue;
			master_ptr = elt_ptr->master;
			break;
		}
        }
        if (master_ptr == NULL) {
                std::ostringstream error_oss;
                error_oss << "Exchange formula does not contain an exchange master species, " << this->formula << std::endl;
                //Utilities::error_msg(error_oss.str(), CONTINUE);
		error_msg(error_oss.str().c_str(), CONTINUE);

		std::ostringstream oss;
		this->dump_raw(oss, 0);
		std::cerr << oss.str();

        }
        return(master_ptr);
}

struct exch_comp *cxxExchComp::cxxExchComp2exch_comp(std::list<cxxExchComp>& el)
        //
        // Builds exch_comp structure from of cxxExchComp 
        //
{
        struct exch_comp *exch_comp_ptr = (struct exch_comp *) PHRQ_malloc((size_t) (el.size() * sizeof(struct exch_comp)));
        if (exch_comp_ptr == NULL) malloc_error();

        int i = 0;
        for (std::list<cxxExchComp>::iterator it = el.begin(); it != el.end(); ++it) {
                exch_comp_ptr[i].formula                =  it->formula;
                exch_comp_ptr[i].formula_z              =  it->formula_z;
                exch_comp_ptr[i].totals                 =  it->totals.elt_list();
                exch_comp_ptr[i].moles                  =  it->moles;
                exch_comp_ptr[i].formula_totals         =  it->formula_totals.elt_list();
                exch_comp_ptr[i].la                     =  it->la;
                exch_comp_ptr[i].charge_balance         =  it->charge_balance;
                exch_comp_ptr[i].phase_name             =  it->phase_name;
                exch_comp_ptr[i].phase_proportion       =  it->phase_proportion;
                exch_comp_ptr[i].rate_name              =  it->rate_name;
                exch_comp_ptr[i].master                 =  it->get_master();
                i++;
        }
        return(exch_comp_ptr);
}

void cxxExchComp::dump_xml(std::ostream& s_oss, unsigned int indent)const
{
        //const char    ERR_MESSAGE[] = "Packing exch_comp message: %s, element not found\n";
        unsigned int i;
        s_oss.precision(DBL_DIG - 1);
        std::string indent0(""), indent1(""), indent2("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);
        for(i = 0; i < indent + 1; ++i) indent1.append(Utilities::INDENT);
        for(i = 0; i < indent + 2; ++i) indent2.append(Utilities::INDENT);

        // Exch_Comp element and attributes

        s_oss << indent0 << "formula=\"" << this->formula << "\"" << std::endl;
        s_oss << indent0 << "moles=\"" << this->moles  << "\"" << std::endl;
        s_oss << indent0 << "la=\"" << this->la     << "\"" << std::endl;
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

        // formula_totals
        s_oss << indent0;
        s_oss << "<formula_totals " << std::endl;
        this->formula_totals.dump_xml(s_oss, indent + 1);
}

void cxxExchComp::dump_raw(std::ostream& s_oss, unsigned int indent)const
{
        //const char    ERR_MESSAGE[] = "Packing exch_comp message: %s, element not found\n";
        unsigned int i;
        s_oss.precision(DBL_DIG - 1);
        std::string indent0(""), indent1(""), indent2("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);
        for(i = 0; i < indent + 1; ++i) indent1.append(Utilities::INDENT);
        for(i = 0; i < indent + 2; ++i) indent2.append(Utilities::INDENT);

        // Exch_Comp element and attributes

        s_oss << indent0 << "-formula               " << this->formula << std::endl;
        s_oss << indent0 << "-moles                 " << this->moles  << std::endl;
        s_oss << indent0 << "-la                    " << this->la     << std::endl;
        s_oss << indent0 << "-charge_balance        " << this->charge_balance << std::endl;
        if (this->phase_name != NULL) {
                s_oss << indent0 << "-phase_name            " << this->phase_name << std::endl;
        }
        if (this->rate_name != NULL) {
                s_oss << indent0 << "-rate_name             " << this->rate_name << std::endl;
        }
        s_oss << indent0 << "-phase_proportion              " << this->phase_proportion  << std::endl;
        s_oss << indent0 << "-formula_z                     " << this->formula_z  << std::endl;

        // totals
        s_oss << indent0;
        s_oss << "-totals" << std::endl;
        this->totals.dump_raw(s_oss, indent + 1);

        // formula_totals
        s_oss << indent0;
        s_oss << "-formula_totals" << std::endl;
        this->formula_totals.dump_raw(s_oss, indent + 1);
}

void cxxExchComp::read_raw(CParser& parser)
{
        std::string str;
        
        static std::vector<std::string> vopts;
        if (vopts.empty()) {
                vopts.reserve(10);
                vopts.push_back("formula");                   // 0 
                vopts.push_back("moles");                     // 1
                vopts.push_back("la");                        // 2 
                vopts.push_back("charge_balance");            // 3 
                vopts.push_back("phase_name");                // 4 
                vopts.push_back("rate_name");                 // 5 
                vopts.push_back("formula_z");                 // 6
                vopts.push_back("phase_proportion");          // 7 
                vopts.push_back("totals");                    // 8
                vopts.push_back("formula_totals");            // 9
        }

        std::istream::pos_type ptr;
        std::istream::pos_type next_char;
        std::string token;
        int opt_save;

        opt_save = CParser::OPT_ERROR;
        bool formula_defined(false); 
        bool moles_defined(false); 
        bool la_defined(false); 
        bool charge_balance_defined(false); 
        bool formula_z_defined(false);

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
                        // Allow return to Exchange for more processing
                        //parser.error_msg("Unknown input in EXCH_COMP read.", CParser::OT_CONTINUE);
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

                case 3: // charge_balance
                        if (!(parser.get_iss() >> this->charge_balance))
                        {
                                this->charge_balance = 0;
                                parser.incr_input_error();
                                parser.error_msg("Expected numeric value for charge_balance.", CParser::OT_CONTINUE);
                        }
                        charge_balance_defined = true;
                        break;

                case 4: // phase_name
                        if (!(parser.get_iss() >> str))
                        {
                                this->phase_name = NULL;
                                parser.incr_input_error();
                                parser.error_msg("Expected string value for phase_name.", CParser::OT_CONTINUE);
                        } else {
                                this->phase_name = string_hsave(str.c_str());
                        }
                        break;

                case 5: // rate_name
                        if (!(parser.get_iss() >> str))
                        {
                                this->rate_name = NULL;
                                parser.incr_input_error();
                                parser.error_msg("Expected string value for rate_name.", CParser::OT_CONTINUE);
                        } else {
                                this->rate_name = string_hsave(str.c_str());
                        }
                        break;

                case 6: // formula_z
                        if (!(parser.get_iss() >> this->formula_z))
                        {
                                this->formula_z = 0;
                                parser.incr_input_error();
                                parser.error_msg("Expected numeric value for formula_z.", CParser::OT_CONTINUE);
                        }
                        formula_z_defined = true;
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
                                parser.error_msg("Expected element name and molality for ExchComp totals.", CParser::OT_CONTINUE);
                        }                               
                        opt_save = 8;
                        break;

                case 9: // formula_totals
                        if ( this->formula_totals.read_raw(parser, next_char) != CParser::PARSER_OK) {
                                parser.incr_input_error();
                                parser.error_msg("Expected element name and molality for ExchComp formula totals.", CParser::OT_CONTINUE);
                        }                               
                        opt_save = 9;
                        break;
                }
                if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD) break;
        }
        // members that must be defined
        if (formula_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Formula not defined for ExchComp input.", CParser::OT_CONTINUE);
        }
        if (moles_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Moles not defined for ExchComp input.", CParser::OT_CONTINUE);
        }
        if (la_defined == false) {
                parser.incr_input_error();
                parser.error_msg("La not defined for ExchComp input.", CParser::OT_CONTINUE);
        }
        if (charge_balance_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Charge_balance not defined for ExchComp input.", CParser::OT_CONTINUE);
        }
        if (formula_z_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Formula_z not defined for ExchComp input.", CParser::OT_CONTINUE);
        }
}
