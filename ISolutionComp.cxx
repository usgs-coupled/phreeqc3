#ifdef _DEBUG
#pragma warning(disable : 4786)   // disable truncation warning (Only used by debugger)
#endif

#include "ISolutionComp.h"
#include "ISolution.h"
#include "Utils.h"
#include <cassert>
#define EXTERNAL extern
#include "global.h"
#include "phrqproto.h"
#include "phqalloc.h"

cxxISolutionComp::cxxISolutionComp(void)
: description(NULL)
, moles(0.0)
, input_conc(0.0)
, units(NULL)
, equation_name(NULL)
, phase_si(0.0)
, n_pe(-1)
, as(NULL)
, gfw(0.0)
        //, skip(0);
        //, phase(NULL)
{
}
cxxISolutionComp::cxxISolutionComp(struct conc *conc_ptr)
{
        description         = conc_ptr->description;
        moles               = conc_ptr->moles; 
        input_conc          = conc_ptr->input_conc;
        units               = conc_ptr->units;
        equation_name       = conc_ptr->equation_name;
        phase_si            = conc_ptr->phase_si;
        n_pe                = conc_ptr->n_pe;
        as                  = conc_ptr->as;
        gfw                 = conc_ptr->gfw;
        //skip                = conc_ptr->skip;
        //phase               = conc_ptr->phase;
}

cxxISolutionComp::~cxxISolutionComp(void)
{
}
/*
struct conc *cxxISolutionComp::concarray(std::map <char *, double, CHARSTAR_LESS> &totals)
        // for Solutions, not ISolutions
        // takes a map of (elt name, moles)
        // returns list of conc structures
{
        struct conc *c;
        c = (struct conc *) PHRQ_malloc((size_t) ((totals.size() + 1) * sizeof(struct conc)));
        if (c == NULL) malloc_error();
        int i = 0;
        for (std::map <char *, double, CHARSTAR_LESS>::const_iterator it = totals.begin(); it != totals.end(); ++it) {
                c[i].description         = (char *)it->first;
                c[i].moles               = it->second;
                c[i].input_conc          = it->second;
                c[i].units               = NULL;
                c[i].equation_name       = NULL;
                c[i].phase_si            = 0.0;
                c[i].n_pe                = 0;
                c[i].as                  = NULL;
                c[i].gfw                 = 0.0;
                //c[i].skip                = 0;
                c[i].phase               = NULL;
                i++;
        }                       
        c[i].description = NULL;
        return(c);
}
*/
struct conc *cxxISolutionComp::cxxISolutionComp2conc(const std::set <cxxISolutionComp> &totals)
        // for ISolutions
        // takes a std::vector cxxISolutionComp structures
        // returns list of conc structures
{
        struct conc *c;
        c = (struct conc *) PHRQ_malloc((size_t) ((totals.size() + 1) * sizeof(struct conc)));
        if (c == NULL) malloc_error();
        int i = 0;
        for (std::set<cxxISolutionComp>::const_iterator it = totals.begin(); it != totals.end(); ++it) {
                c[i].description         = it->description;
                c[i].moles               = it->moles;
                c[i].input_conc          = it->input_conc;
                c[i].units               = it->units;
                c[i].equation_name       = it->equation_name;
                c[i].phase_si            = it->phase_si;
                c[i].n_pe                = it->n_pe;
                c[i].as                  = it->as;
                c[i].gfw                 = it->gfw;
                //c[i].skip                = 0;
                c[i].phase               = NULL;
                i++;
        }                       
        c[i].description = NULL;
        return(c);
}

#ifdef SKIP
cxxISolutionComp::STATUS_TYPE cxxISolutionComp::read(CParser& parser, cxxISolution& solution)
{
        // std::string& str = parser.line(); 
        std::string str = parser.line();

        // defaults set in ctor

        // Remove space between "kg" and "solution" or "water" in units
        Utilities::replace("Kg", "kg", str);
        Utilities::replace("KG", "kg", str);
        while (Utilities::replace("kg ", "kg", str));

        std::istream::pos_type ptr = 0;

        //
        // Read master species list for mass balance equation
        //
        std::string token;
        std::string token1;
        int count_redox_states = 0;
        CParser::TOKEN_TYPE j;
        while ( ((j = parser.copy_token(token, ptr)) == CParser::TT_UPPER ) ||
                ( token[0] == '[' ) ||
                ( Utilities::strcmp_nocase_arg1(token.c_str(), "ph") == 0 ) ||
                ( Utilities::strcmp_nocase_arg1(token.c_str(), "pe") == 0 ) )
        {
                ++count_redox_states;
                Utilities::replace("(+", "(", token);
                if (count_redox_states > 1) token1 += " ";
                token1 += token;
        }
        if (count_redox_states == 0) {
                parser.incr_input_error();
                parser.error_msg("No element or master species given for concentration input.", CParser::OT_CONTINUE);
                return cxxISolutionComp::ERROR;
        }
        description = token1;

        // Determine if reading alkalinity, allow equivalents for units
        Utilities::str_tolower(token1);
        bool alk = false;
        if (token1.find("alk") == 0) {
                alk = true;
        }

        // Read concentration
        if (!(std::istringstream(token) >> this->input_conc)) {
                std::ostringstream err;
                err << "Concentration data error for " << token1 << " in solution input.";
                parser.error_msg(err, CParser::OT_CONTINUE);
                return cxxISolutionComp::ERROR;
        }
        if ( (j = parser.copy_token(token, ptr)) == CParser::TT_EMPTY) return cxxISolutionComp::OK;

        // Read optional data
        token1 = token;
        
        // Check for units info
        if (parser.check_units(token1, alk, false, solution.get_units(), false) == CParser::OK) {
                if (parser.check_units(token1, alk, false, solution.get_units(), true) == CParser::OK) {
                        this->units = token1;
                        if ( (j = parser.copy_token(token, ptr)) == CParser::TT_EMPTY) return cxxISolutionComp::OK;
                } else {
                        return cxxISolutionComp::ERROR;
                }
        }

        // Check for "as" followed by formula to be used for gfw
        token1 = token;
        Utilities::str_tolower(token1);
        if (token1.compare("as") == 0)
        {
                parser.copy_token(token, ptr);
                this->as = token;
                if ( (j = parser.copy_token(token, ptr)) == CParser::TT_EMPTY) return cxxISolutionComp::OK;
        }
        // Check for "gfw" followed by gram formula weight
        else if (token1.compare("gfw") == 0)
        {
                if (parser.copy_token(token, ptr) != CParser::TT_DIGIT) {
                        parser.error_msg("Expecting gram formula weight.", CParser::OT_CONTINUE);
                        return cxxISolutionComp::ERROR;
                } else {
                        parser.get_iss() >> this->gfw;
                        if ( (j = parser.copy_token(token, ptr)) == CParser::TT_EMPTY) return cxxISolutionComp::OK;
                }
        }

        // Check for redox couple for pe
        if  ( Utilities::strcmp_nocase_arg1(token.c_str(), "pe") == 0 ) {
                this->n_pe = cxxPe_Data::store(solution.pe, token);
                if ( (j = parser.copy_token(token, ptr)) == CParser::TT_EMPTY) return cxxISolutionComp::OK;
        } else if (token.find("/") != std::string::npos) {
                if (parser.parse_couple(token) == CParser::OK) {
                        this->n_pe = cxxPe_Data::store(solution.pe, token);
                        if ( (j = parser.copy_token(token, ptr)) == CParser::TT_EMPTY) return cxxISolutionComp::OK;
                } else {
                        return cxxISolutionComp::ERROR;
                }
        }

        // Must have phase
        this->equation_name = token;
        if ( (j = parser.copy_token(token, ptr)) == CParser::TT_EMPTY) return cxxISolutionComp::OK;

        // Check for saturation index
        if (!(std::istringstream(token) >> this->phase_si))
        {
                parser.error_msg("Expected saturation index.", CParser::OT_CONTINUE);
                return cxxISolutionComp::ERROR;
        }
        return cxxISolutionComp::OK;
}
#endif

#ifdef SKIP
void cxxISolutionComp::dump_xml(std::ostream& s_oss, unsigned int indent)const
{
        unsigned int i;
        std::string indent0("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);

        s_oss << indent0;
        s_oss << "<soln_total" ;

        s_oss << " conc_desc=\"" << this->description << "\"" ;

        s_oss << " conc_moles=\"" << this->moles << "\"" ;

        s_oss << "\">" << std::endl;
}
#endif
