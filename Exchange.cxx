// Exchange.cxx: implementation of the cxxExchange class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)   // disable truncation warning (Only used by debugger)
#endif

#include <iostream>     // std::cout std::cerr
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
        exchange_ptr->count_comps = (int) this->exchComps.size();
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
#ifdef USE_MPI
/* ---------------------------------------------------------------------- */
void cxxExchange::mpi_pack(std::vector<int>& ints, std::vector<double>& doubles)
/* ---------------------------------------------------------------------- */
{
	/* int n_user; */
	ints.push_back(this->n_user);
	
	ints.push_back((int) this->pitzer_exchange_gammas);
	ints.push_back((int) this->exchComps.size());
	for (std::list<cxxExchComp>::iterator it = this->exchComps.begin(); it != this->exchComps.end(); it++) {
		it->mpi_pack(ints, doubles);
	}
}
/* ---------------------------------------------------------------------- */
void cxxExchange::mpi_unpack(int *ints, int *ii, double *doubles, int *dd)
/* ---------------------------------------------------------------------- */
{
	int i = *ii;
	int d = *dd;
	/* int n_user; */
	this->n_user = ints[i++];
	this->n_user_end = this->n_user;
	this->description = " ";

	
	this->pitzer_exchange_gammas = (bool) ints[i++];
	int count = ints[i++];
	this->exchComps.clear();
	for (int n = 0; n < count; n++) {
		cxxExchComp ec;
		ec.mpi_unpack(ints, &i, doubles, &d);
		this->exchComps.push_back(ec);
	}
	*ii = i;
	*dd = d;
}
#endif
