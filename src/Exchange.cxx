// Exchange.cxx: implementation of the cxxExchange class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)   // disable truncation warning (Only used by debugger)
#endif

#include "Utils.h"   // define first
#include "Exchange.h"
#include "ExchComp.h"
#define EXTERNAL extern
#include "global.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include <cassert>     // assert
#include <algorithm>   // std::sort 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxExchange::cxxExchange()
        //
        // default constructor for cxxExchange 
        //
: cxxNumKeyword()
{
        pitzer_exchange_gammas  = false;
}

cxxExchange::cxxExchange(struct exchange *exchange_ptr)
        //
        // constructor for cxxExchange from struct exchange
        //
: 
cxxNumKeyword()

{
        int i;

        this->set_description(exchange_ptr->description);
        n_user                       = exchange_ptr->n_user;     
        n_user_end                   = exchange_ptr->n_user_end;  
        pitzer_exchange_gammas       = (exchange_ptr->pitzer_exchange_gammas == TRUE);
        for (i = 0; i < exchange_ptr->count_comps; i++) {
                cxxExchComp ec(&(exchange_ptr->comps[i]));
                exchComps.push_back(ec);
        }




}

cxxExchange::~cxxExchange()
{
}

bool cxxExchange::get_related_phases()
{       
        for (std::list<cxxExchComp>::const_iterator it = this->exchComps.begin(); it != this->exchComps.end(); ++it) {
                if (it->get_phase_name() == NULL) continue;
                return(true);
        }
        return(false);
}

bool cxxExchange::get_related_rate()
{       
        for (std::list<cxxExchComp>::const_iterator it = this->exchComps.begin(); it != this->exchComps.end(); ++it) {
                if (it->get_rate_name() == NULL) continue;
                return(true);
        }
        return(false);
}

struct exchange *cxxExchange::cxxExchange2exchange()
        //
        // Builds a exchange structure from instance of cxxExchange 
        //
{
        struct exchange *exchange_ptr = exchange_alloc();
        
        exchange_ptr->description                 = this->get_description();  
        exchange_ptr->n_user                      = this->n_user;            
        exchange_ptr->n_user_end                  = this->n_user_end;        
        exchange_ptr->new_def                     = FALSE;                    
        exchange_ptr->solution_equilibria         = FALSE;
        exchange_ptr->n_solution                  = -2;
        exchange_ptr->related_phases              = (int) this->get_related_phases();
        exchange_ptr->related_rate                = (int) this->get_related_rate();
        exchange_ptr->pitzer_exchange_gammas      = (int) this->pitzer_exchange_gammas;
        exchange_ptr->count_comps = this->exchComps.size();
        exchange_ptr->comps = (struct exch_comp *) free_check_null(exchange_ptr->comps);
        exchange_ptr->comps = cxxExchComp::cxxExchComp2exch_comp(this->exchComps);
        return(exchange_ptr);
}

void cxxExchange::dump_xml(std::ostream& s_oss, unsigned int indent)const
{
        //const char    ERR_MESSAGE[] = "Packing exchange message: %s, element not found\n";
        unsigned int i;
        s_oss.precision(DBL_DIG - 1);
        std::string indent0(""), indent1(""), indent2("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);
        for(i = 0; i < indent + 1; ++i) indent1.append(Utilities::INDENT);
        for(i = 0; i < indent + 2; ++i) indent2.append(Utilities::INDENT);

        // Exchange element and attributes
        s_oss << indent0;
        s_oss << "<exchange " << std::endl;

        s_oss << indent1;
        s_oss << "pitzer_exchange_gammas=\"" << this->pitzer_exchange_gammas << "\"" << std::endl;

        // components
        s_oss << indent1;
        s_oss << "<component " << std::endl;
        for (std::list<cxxExchComp>::const_iterator it = exchComps.begin(); it != exchComps.end(); ++it) {
                it->dump_xml(s_oss, indent + 2);
        }

        return;
}

void cxxExchange::dump_raw(std::ostream& s_oss, unsigned int indent)const
{
        //const char    ERR_MESSAGE[] = "Packing exchange message: %s, element not found\n";
        unsigned int i;
        s_oss.precision(DBL_DIG - 1);
        std::string indent0(""), indent1(""), indent2("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);
        for(i = 0; i < indent + 1; ++i) indent1.append(Utilities::INDENT);
        for(i = 0; i < indent + 2; ++i) indent2.append(Utilities::INDENT);

        // Exchange element and attributes
        s_oss << indent0;
        s_oss << "EXCHANGE_RAW       " << this->n_user  << " " << this->description << std::endl;

        s_oss << indent1;
        s_oss << "-pitzer_exchange_gammas " << this->pitzer_exchange_gammas << std::endl;

        // exchComps structures
        for (std::list<cxxExchComp>::const_iterator it = exchComps.begin(); it != exchComps.end(); ++it) {
                s_oss << indent1;
                s_oss << "-component" << std::endl;
                it->dump_raw(s_oss, indent + 2);
        }

        return;
}

void cxxExchange::read_raw(CParser& parser)
{
        static std::vector<std::string> vopts;
        if (vopts.empty()) {
                vopts.reserve(15);
                vopts.push_back("pitzer_exchange_gammas");     // 0
                vopts.push_back("component");                  // 1
        }                                                      
                                                               
        std::istream::pos_type ptr;                            
        std::istream::pos_type next_char;                      
        std::string token;                                     
        int opt_save;
        bool useLastLine(false);

        // Read exchange number and description
        this->read_number_description(parser);

        opt_save = CParser::OPT_ERROR;
        bool pitzer_exchange_gammas_defined(false); 

        for (;;)
        {
                int opt;
                if (useLastLine == false) {
                        opt = parser.get_option(vopts, next_char);
                } else {
                        opt = parser.getOptionFromLastLine(vopts, next_char);
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
                        parser.error_msg("Unknown input in EXCH_COMP_RAW keyword.", CParser::OT_CONTINUE);
                        parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
                        useLastLine = false;
                        break;

                case 0: // pitzer_exchange_gammas
                        if (!(parser.get_iss() >> this->pitzer_exchange_gammas))
                        {
                                this->pitzer_exchange_gammas = false;
                                parser.incr_input_error();
                                parser.error_msg("Expected boolean value for pitzer_exchange_gammas.", CParser::OT_CONTINUE);
                        }
                        pitzer_exchange_gammas_defined = true;
                        useLastLine = false;
                        break;
                case 1: // component
                        {
                                cxxExchComp ec;
                                ec.read_raw(parser);
                                this->exchComps.push_back(ec);
                        }
                        useLastLine = true;
                        break;
                }
                if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD) break;
        }
        // members that must be defined
        if (pitzer_exchange_gammas_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Pitzer_exchange_gammsa not defined for EXCHANGE_RAW input.", CParser::OT_CONTINUE);
        }
}
#ifdef SKIP
cxxExchange& cxxExchange::read(CParser& parser)
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

        cxxExchange numkey;

        // Read exchange number and description
        numkey.read_number_description(parser);

        std::istream::pos_type ptr;
        std::istream::pos_type next_char;
        std::string token;
        CParser::TOKEN_TYPE j;
        
        //cxxExchange& sol = s_map[numkey.n_user()];
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
                        parser.error_msg("Unknown input in EXCHANGE keyword.", CParser::OT_CONTINUE);
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
                                sol.exchange_pe = conc.get_input_conc();
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
                                parser.error_msg("Expected numeric value for mass of water in exchange.", CParser::OT_CONTINUE);
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
