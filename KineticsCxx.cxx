// Kinetics.cxx: implementation of the cxxKinetics class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)   // disable truncation warning (Only used by debugger)
#endif

#include "Utils.h"   // define first
#include "KineticsCxx.h"
#include "KineticsComp.h"
#define EXTERNAL extern
#include "global.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include <cassert>     // assert
#include <algorithm>   // std::sort 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxKinetics::cxxKinetics()
        //
        // default constructor for cxxKinetics 
        //
: cxxNumKeyword()
{
        step_divide                 = 1.0;
        rk                          = 3;
        bad_step_max                = 500;
        use_cvode                   = false;
        totals.type                 = cxxNameDouble::ND_ELT_MOLES;
}

cxxKinetics::cxxKinetics(struct kinetics *kinetics_ptr)
        //
        // constructor for cxxKinetics from struct kinetics
        //
: 
cxxNumKeyword(),
totals(kinetics_ptr->totals)
{
        int i;

        this->set_description(kinetics_ptr->description);
        n_user                       = kinetics_ptr->n_user;     
        n_user_end                   = kinetics_ptr->n_user_end;  
        step_divide                  = kinetics_ptr->step_divide; 
        rk                           = kinetics_ptr->rk; 
        bad_step_max                 = kinetics_ptr->bad_step_max; 
        use_cvode                    = (kinetics_ptr->use_cvode == TRUE); 

        // kinetics components
        for (i = 0; i < kinetics_ptr->count_comps; i++) {
                cxxKineticsComp ec(&(kinetics_ptr->comps[i]));
                this->kineticsComps.push_back(ec);
        }

        // steps
        for (i = 0; i < kinetics_ptr->count_steps; i++) {
                this->steps.push_back(kinetics_ptr->steps[i]);
        }
}

cxxKinetics::~cxxKinetics()
{
}

struct kinetics *cxxKinetics::cxxKinetics2kinetics()
        //
        // Builds a kinetics structure from instance of cxxKinetics 
        //
{
        struct kinetics *kinetics_ptr = kinetics_alloc();
        
        kinetics_ptr->description                 = this->get_description();  
        kinetics_ptr->n_user                      = this->n_user;            
        kinetics_ptr->n_user_end                  = this->n_user_end;        
        kinetics_ptr->step_divide                 = this->step_divide;
        kinetics_ptr->rk                          = this->rk;
        kinetics_ptr->bad_step_max                = this->bad_step_max;
        kinetics_ptr->use_cvode                   = (int) this->use_cvode;

        // totals
        kinetics_ptr->totals                      = this->totals.elt_list();

        // comps
        kinetics_ptr->count_comps             = this->kineticsComps.size();
        kinetics_ptr->comps                   = (struct kinetics_comp *) free_check_null(kinetics_ptr->comps);
        kinetics_ptr->comps                   = cxxKineticsComp::cxxKineticsComp2kinetics_comp(this->kineticsComps);

        // steps
    kinetics_ptr->count_steps                     = this->steps.size();
    kinetics_ptr->steps                       = (double *) free_check_null(kinetics_ptr->steps);
        if (this->steps.size() > 0) {
                kinetics_ptr->steps = (double *) PHRQ_malloc((size_t) (this->steps.size() * sizeof(double)));
                if (kinetics_ptr->steps == NULL) malloc_error();
                std::copy(this->steps.begin(), this->steps.end(), kinetics_ptr->steps);
                /*
                int i = 0;
                for (std::vector<double>::iterator it = this->steps.begin(); it != this->steps.end(); it++) {
                        kinetics_ptr->steps[i] = *it;
                }
                */
        }
        return(kinetics_ptr);
}

#ifdef SKIP
void cxxKinetics::dump_xml(std::ostream& s_oss, unsigned int indent)const
{
        //const char    ERR_MESSAGE[] = "Packing kinetics message: %s, element not found\n";
        unsigned int i;
        s_oss.precision(DBL_DIG - 1);
        std::string indent0(""), indent1(""), indent2("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);
        for(i = 0; i < indent + 1; ++i) indent1.append(Utilities::INDENT);
        for(i = 0; i < indent + 2; ++i) indent2.append(Utilities::INDENT);

        // Kinetics element and attributes
        s_oss << indent0;
        s_oss << "<kinetics " << std::endl;

        s_oss << indent1;
        s_oss << "pitzer_kinetics_gammas=\"" << this->pitzer_kinetics_gammas << "\"" << std::endl;

        // components
        s_oss << indent1;
        s_oss << "<component " << std::endl;
        for (std::list<cxxKineticsComp>::const_iterator it = kineticsComps.begin(); it != kineticsComps.end(); ++it) {
                it->dump_xml(s_oss, indent + 2);
        }

        return;
}
#endif

void cxxKinetics::dump_raw(std::ostream& s_oss, unsigned int indent)const
{
        //const char    ERR_MESSAGE[] = "Packing kinetics message: %s, element not found\n";
        unsigned int i;
        s_oss.precision(DBL_DIG - 1);
        std::string indent0(""), indent1(""), indent2("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);
        for(i = 0; i < indent + 1; ++i) indent1.append(Utilities::INDENT);
        for(i = 0; i < indent + 2; ++i) indent2.append(Utilities::INDENT);

        // Kinetics element and attributes
        s_oss << indent0;
        s_oss << "KINETICS_RAW       " << this->n_user  << " " << this->description << std::endl;

        s_oss << indent1;
        s_oss << "-step_divide       " << this->step_divide  << std::endl;

        s_oss << indent1;
        s_oss << "-rk                " << this->rk  << std::endl;

        s_oss << indent1;
        s_oss << "-bad_step_max      " << this->bad_step_max  << std::endl;

        s_oss << indent1;
        s_oss << "-use_cvode         " << this->use_cvode  << std::endl;

        // kineticsComps structures
        for (std::list<cxxKineticsComp>::const_iterator it = kineticsComps.begin(); it != kineticsComps.end(); ++it) {
                s_oss << indent1;
                s_oss << "-component" << std::endl;
                it->dump_raw(s_oss, indent + 2);
        }

        // totals
        s_oss << indent1;
        s_oss << "-totals         "  << std::endl;
        this->totals.dump_raw(s_oss, indent + 2);

        // steps
        s_oss << indent1;
        s_oss << "-steps         "  << std::endl;
        {
                int i = 0;
                s_oss << indent2;
                for (std::vector<double>::const_iterator it = this->steps.begin(); it != this->steps.end(); it++) {
                        if (i++ == 5) {
                                s_oss << std::endl;
                                s_oss << indent2;
                                i = 0;
                        }
                        s_oss << *it << " ";
                }
                s_oss << std::endl;
        }       
        return;
}

void cxxKinetics::read_raw(CParser& parser)
{

        double d;
        static std::vector<std::string> vopts;
        if (vopts.empty()) {
                vopts.reserve(15);
                vopts.push_back("step_divide");     
                vopts.push_back("rk");                  
                vopts.push_back("bad_step_max");                
                vopts.push_back("use_cvode");           
                vopts.push_back("component");           
                vopts.push_back("totals");              
                vopts.push_back("steps");               
        }                                                      
                                                               
        std::istream::pos_type ptr;                            
        std::istream::pos_type next_char;                      
        std::string token;                                     
        int opt_save;
        bool useLastLine(false);

        // Read kinetics number and description
        this->read_number_description(parser);

        opt_save = CParser::OPT_ERROR;
        bool step_divide_defined(false); 
        bool rk_defined(false); 
        bool bad_step_max_defined(false); 
        bool use_cvode_defined(false); 

        for (;;)
        {
                int opt;
                if (useLastLine == false) {
                        opt = parser.get_option(vopts, next_char);
                } else {
                        opt = parser.getOptionFromLastLine(vopts, next_char);
                }
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
                        parser.error_msg("Unknown input in KINETICS_COMP_RAW keyword.", CParser::OT_CONTINUE);
                        parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
                        useLastLine = false;
                        break;

                case 0: // step_divide
                        if (!(parser.get_iss() >> this->step_divide))
                        {
                                this->step_divide = 1.0;
                                parser.incr_input_error();
                                parser.error_msg("Expected numeric value for step_divide.", CParser::OT_CONTINUE);
                        }
                        step_divide_defined = true;
                        useLastLine = false;
                        break;

                case 1: // rk
                        if (!(parser.get_iss() >> this->rk))
                        {
                                this->rk = 3;
                                parser.incr_input_error();
                                parser.error_msg("Expected integer value for rk.", CParser::OT_CONTINUE);
                        }
                        rk_defined = true;
                        useLastLine = false;
                        break;

                case 2: // bad_step_max
                        if (!(parser.get_iss() >> this->bad_step_max))
                        {
                                this->bad_step_max = 500;
                                parser.incr_input_error();
                                parser.error_msg("Expected integer value for bad_step_max.", CParser::OT_CONTINUE);
                        }
                        bad_step_max_defined = true;
                        useLastLine = false;
                        break;

                case 3: // use_cvode
                        if (!(parser.get_iss() >> this->use_cvode))
                        {
                                this->use_cvode = false;
                                parser.incr_input_error();
                                parser.error_msg("Expected boolean value for use_cvode.", CParser::OT_CONTINUE);
                        }
                        use_cvode_defined = true;
                        useLastLine = false;
                        break;

                case 4: // component
                        {
                                cxxKineticsComp kc;
                                kc.read_raw(parser);
                                this->kineticsComps.push_back(kc);
                        }
                        useLastLine = true;
                        break;

                case 5: // totals
                        if ( this->totals.read_raw(parser, next_char) != CParser::PARSER_OK) {
                                parser.incr_input_error();
                                parser.error_msg("Expected element name and molality for KineticsComp totals.", CParser::OT_CONTINUE);
                        }                               
                        opt_save = 5;
                        useLastLine = false;

                case 6: // steps
                        while (parser.copy_token(token, next_char) == CParser::TT_DIGIT) {
                                //sscanf(token.c_str(), "%lf", &d);
                                //this->steps.push_back(d);
				std::istringstream iss(token);
				if (!(iss >> d)) {
					parser.incr_input_error();
					parser.error_msg("Expected numeric value for steps.", CParser::OT_CONTINUE);
				} else {
					this->steps.push_back(d);
				}
                        }
                        opt_save = 6;
                        useLastLine = false;
                }
                if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD) break;
        }
        // members that must be defined
        if (step_divide_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Step_divide not defined for KINETICS_RAW input.", CParser::OT_CONTINUE);
        }
        if (rk_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Rk not defined for KINETICS_RAW input.", CParser::OT_CONTINUE);
        }
        if (bad_step_max_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Bad_step_max not defined for KINETICS_RAW input.", CParser::OT_CONTINUE);
        }
        if (use_cvode_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Use_cvode not defined for KINETICS_RAW input.", CParser::OT_CONTINUE);
        }
}
#ifdef SKIP
cxxKinetics& cxxKinetics::read(CParser& parser)
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

        cxxKinetics numkey;

        // Read kinetics number and description
        numkey.read_number_description(parser);

        std::istream::pos_type ptr;
        std::istream::pos_type next_char;
        std::string token;
        CParser::TOKEN_TYPE j;
        
        //cxxKinetics& sol = s_map[numkey.n_user()];
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
                        parser.error_msg("Unknown input in KINETICS keyword.", CParser::OT_CONTINUE);
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
                                sol.kinetics_pe = conc.get_input_conc();
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
                                parser.error_msg("Expected numeric value for mass of water in kinetics.", CParser::OT_CONTINUE);
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
