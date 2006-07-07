// KineticsComp.cxx: implementation of the cxxKineticsComp class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)   // disable truncation warning (Only used by debugger)
#endif

#include "Utils.h"   // define first
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

cxxKineticsComp::cxxKineticsComp()
        //
        // default constructor for cxxKineticsComp 
        //
{
        rate_name                     = NULL;
        tol                           = 1e-8;
        m                             = 0.0;
        m0                            = 0.0;
        moles                         = 0.0;
        namecoef.type                 = cxxNameDouble::ND_NAME_COEF;
}

cxxKineticsComp::cxxKineticsComp(struct kinetics_comp *kinetics_comp_ptr)
        //
        // constructor for cxxKineticsComp from struct kinetics_comp
        //
: 
namecoef(kinetics_comp_ptr->list, kinetics_comp_ptr->count_list)
{
        rate_name                    = kinetics_comp_ptr->rate_name;
        tol                          = kinetics_comp_ptr->tol;
        m                            = kinetics_comp_ptr->m;
        m0                           = kinetics_comp_ptr->m0;
        moles                        = kinetics_comp_ptr->moles;
        for (int i = 0; i < kinetics_comp_ptr->count_d_params; i++) {
                this->d_params.push_back(kinetics_comp_ptr->d_params[i]);
        }
}

cxxKineticsComp::~cxxKineticsComp()
{
}

struct kinetics_comp *cxxKineticsComp::cxxKineticsComp2kinetics_comp(std::list<cxxKineticsComp>& el)
        //
        // Builds kinetics_comp structure from of cxxKineticsComp 
        //
{
        struct kinetics_comp *kinetics_comp_ptr = (struct kinetics_comp *) PHRQ_malloc((size_t) (el.size() * sizeof(struct kinetics_comp)));
        if (kinetics_comp_ptr == NULL) malloc_error();

        int i = 0;
        for (std::list<cxxKineticsComp>::iterator it = el.begin(); it != el.end(); ++it) {
                kinetics_comp_ptr[i].rate_name              =  it->rate_name;
                kinetics_comp_ptr[i].list                   =  it->namecoef.name_coef();
                kinetics_comp_ptr[i].count_list             =  (int) it->namecoef.size();
                kinetics_comp_ptr[i].tol                    =  it->tol;
                kinetics_comp_ptr[i].m                      =  it->m;
                kinetics_comp_ptr[i].initial_moles          =  0.;
                kinetics_comp_ptr[i].m0                     =  it->m0;
                kinetics_comp_ptr[i].moles                  =  it->moles;
                kinetics_comp_ptr[i].count_c_params         =  0;
                kinetics_comp_ptr[i].c_params               =  NULL;
/*
                kinetics_comp_ptr[i].count_d_params         =  0;
                kinetics_comp_ptr[i].d_params               =  NULL;
*/

                kinetics_comp_ptr[i].count_d_params         =  (int) it->d_params.size();
                kinetics_comp_ptr[i].d_params               =  NULL;
                if (it->d_params.size() > 0) {
                        kinetics_comp_ptr[i].d_params = (double *) PHRQ_malloc((size_t) (it->d_params.size() * sizeof(double)));
                        if (kinetics_comp_ptr[i].d_params == NULL) malloc_error();
                        std::copy(it->d_params.begin(), it->d_params.end(), kinetics_comp_ptr[i].d_params);
                }
		i++;
        }
        return(kinetics_comp_ptr);
}
#ifdef SKIP
void cxxKineticsComp::dump_xml(std::ostream& s_oss, unsigned int indent)const
{
        //const char    ERR_MESSAGE[] = "Packing kinetics_comp message: %s, element not found\n";
        unsigned int i;
        s_oss.precision(DBL_DIG - 1);
        std::string indent0(""), indent1(""), indent2("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);
        for(i = 0; i < indent + 1; ++i) indent1.append(Utilities::INDENT);
        for(i = 0; i < indent + 2; ++i) indent2.append(Utilities::INDENT);

        // Kinetics_Comp element and attributes

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
#endif
void cxxKineticsComp::dump_raw(std::ostream& s_oss, unsigned int indent)const
{
        //const char    ERR_MESSAGE[] = "Packing kinetics_comp message: %s, element not found\n";
        unsigned int i;
        s_oss.precision(DBL_DIG - 1);
        std::string indent0(""), indent1(""), indent2("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);
        for(i = 0; i < indent + 1; ++i) indent1.append(Utilities::INDENT);
        for(i = 0; i < indent + 2; ++i) indent2.append(Utilities::INDENT);

        // Kinetics_Comp element and attributes

        s_oss << indent0 << "-rate_name             " << this->rate_name << std::endl;
        s_oss << indent0 << "-tol                   " << this->tol  << std::endl;
        s_oss << indent0 << "-m                     " << this->m     << std::endl;
        s_oss << indent0 << "-m0                    " << this->m0  << std::endl;
        s_oss << indent0 << "-moles                 " << this->moles  << std::endl;

        // namecoef
        s_oss << indent0;
        s_oss << "-namecoef" << std::endl;
        this->namecoef.dump_raw(s_oss, indent + 1);

        // d_params
        s_oss << indent0;
        s_oss << "-d_params" << std::endl;
        {
                int i = 0;
                s_oss << indent1;
                for (std::vector<double>::const_iterator it = d_params.begin(); it != d_params.end(); it++) {
                        if (i++ == 5) {
                                s_oss << std::endl;
                                s_oss << indent1;
                                i = 0;
                        }
                        s_oss << *it << " ";
                }
                s_oss << std::endl;
        }
}

void cxxKineticsComp::read_raw(CParser& parser)
{
        std::string str;
        double d;

        static std::vector<std::string> vopts;
        if (vopts.empty()) {
                vopts.reserve(10);
                vopts.push_back("rate_name");                  // 0
                vopts.push_back("tol");                        // 1
                vopts.push_back("m");                          // 2
                vopts.push_back("m0");                         // 3
                vopts.push_back("moles");                      // 4
                vopts.push_back("namecoef");                   // 5
                vopts.push_back("d_params");                   // 6
        }                                                      
                                                               
        std::istream::pos_type ptr;
        std::istream::pos_type next_char;
        std::string token;
        int opt_save;

        opt_save = CParser::OPT_ERROR;
        bool rate_name_defined(false); 
        bool tol_defined(false); 
        bool m_defined(false); 
        bool m0_defined(false);
        bool moles_defined(false);

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
                        // Allow return to Kinetics for more processing
                        //parser.error_msg("Unknown input in KINETICS_COMP read.", CParser::OT_CONTINUE);
                        //parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
                        break;

                case 0: // rate_name
                        if (!(parser.get_iss() >> str))
                        {
                                this->rate_name = NULL;
                                parser.incr_input_error();
                                parser.error_msg("Expected string value for rate_name.", CParser::OT_CONTINUE);
                        } else {
                                this->rate_name = string_hsave(str.c_str());
                        }
                        rate_name_defined = true;
                        break;

                case 1: // tol
                        if (!(parser.get_iss() >> this->tol))
                        {
                                this->tol = 0;
                                parser.incr_input_error();
                                parser.error_msg("Expected numeric value for tol.", CParser::OT_CONTINUE);
                        }
                        tol_defined = true;
                        break;

                case 2: // m
                        if (!(parser.get_iss() >> this->m))
                        {
                                this->m = 0;
                                parser.incr_input_error();
                                parser.error_msg("Expected numeric value for m.", CParser::OT_CONTINUE);
                        }
                        m_defined = true;
                        break;

                case 3: // m0
                        if (!(parser.get_iss() >> this->m0))
                        {
                                this->m0 = 0;
                                parser.incr_input_error();
                                parser.error_msg("Expected numeric value for m0.", CParser::OT_CONTINUE);
                        }
                        m0_defined = true;
                        break;


                case 4: // moles
                        if (!(parser.get_iss() >> this->moles))
                        {
                                this->moles = 0;
                                parser.incr_input_error();
                                parser.error_msg("Expected numeric value for moles.", CParser::OT_CONTINUE);
                        }
                        moles_defined = true;
                        break;


                case 5: // namecoef
                        if ( this->namecoef.read_raw(parser, next_char) != CParser::PARSER_OK) {
                                parser.incr_input_error();
                                parser.error_msg("Expected element name and molality for namecoef.", CParser::OT_CONTINUE);
                        }                               
                        opt_save = 5;
                        break;

                case 6: // d_params
                        while (parser.copy_token(token, next_char) == CParser::TT_DIGIT) {
                                sscanf(token.c_str(), "%lf", &d);
                                this->d_params.push_back(d);
                        }
                        opt_save = 6;
                }
                if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD) break;
        }
        // members that must be defined
        if (rate_name_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Rate_name not defined for KineticsComp input.", CParser::OT_CONTINUE);
        }
        if (tol_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Tol not defined for KineticsComp input.", CParser::OT_CONTINUE);
        }
        if (m_defined == false) {
                parser.incr_input_error();
                parser.error_msg("M not defined for KineticsComp input.", CParser::OT_CONTINUE);
        }
        if (m0_defined == false) {
                parser.incr_input_error();
                parser.error_msg("M0 not defined for KineticsComp input.", CParser::OT_CONTINUE);
        }
        if (moles_defined == false) {
                parser.incr_input_error();
                parser.error_msg("Moles not defined for KineticsComp input.", CParser::OT_CONTINUE);
        }
}
#ifdef USE_MPI
#include "Dictionary.h"
void cxxKineticsComp::mpi_pack(std::vector<int>& ints, std::vector<double>& doubles)
{
	extern cxxDictionary dictionary;
	ints.push_back(dictionary.string2int(this->rate_name));
	this->namecoef.mpi_pack(ints, doubles);
	doubles.push_back(this->tol);
        doubles.push_back(this->m);
        doubles.push_back(this->m0);
        doubles.push_back(this->moles);
	ints.push_back((int) this->d_params.size());
	for (std::vector<double>::iterator it = this->d_params.begin(); it != this->d_params.end(); it++) {
		doubles.push_back(*it);
	}
}
void cxxKineticsComp::mpi_unpack(int *ints, int *ii, double *doubles, int *dd)
{
	extern cxxDictionary dictionary;
	int i = *ii;
	int d = *dd;
	this->rate_name = dictionary.int2char(ints[i++]);
	this->namecoef.mpi_unpack(ints, &i, doubles, &d);
	this->tol = doubles[d++];
        this->m = doubles[d++];
        this->m0 = doubles[d++];
        this->moles = doubles[d++];
	int n = ints[i++];
	this->d_params.clear();
	for (int j = 0; j < n; j++) {
		this->d_params.push_back(doubles[d++]);
	}
	*ii = i;
	*dd = d;
}
#endif
