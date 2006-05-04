// PPassemblage.cxx: implementation of the cxxPPassemblage class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)   // disable truncation warning (Only used by debugger)
#endif

#include "Utils.h"   // define first
#include "PPassemblage.h"
#include "PPassemblageComp.h"
#define EXTERNAL extern
#include "global.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include <cassert>     // assert
#include <algorithm>   // std::sort 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxPPassemblage::cxxPPassemblage()
        //
        // default constructor for cxxPPassemblage 
        //
: cxxNumKeyword()
{
        eltList.type                    = cxxNameDouble::ND_ELT_MOLES;
}

cxxPPassemblage::cxxPPassemblage(struct pp_assemblage *pp_assemblage_ptr)
        //
        // constructor for cxxPPassemblage from struct PPassemblage
        //
: 
cxxNumKeyword(),
eltList(pp_assemblage_ptr->next_elt)
{
        int i;

        this->set_description(pp_assemblage_ptr->description);
        n_user                       = pp_assemblage_ptr->n_user;        
        n_user_end                   = pp_assemblage_ptr->n_user_end;  
        for (i = 0; i < pp_assemblage_ptr->count_comps; i++) {
                cxxPPassemblageComp ppComp(&(pp_assemblage_ptr->pure_phases[i]));
                ppAssemblageComps.push_back(ppComp);
        }
}

cxxPPassemblage::~cxxPPassemblage()
{
}

struct pp_assemblage *cxxPPassemblage::cxxPPassemblage2pp_assemblage()
        //
        // Builds a pp_assemblage structure from instance of cxxPPassemblage 
        //
{
        struct pp_assemblage *pp_assemblage_ptr = pp_assemblage_alloc();
        
        pp_assemblage_ptr->description                 = this->get_description();  
        pp_assemblage_ptr->n_user                      = this->n_user;               
        pp_assemblage_ptr->n_user_end                  = this->n_user_end;           
        pp_assemblage_ptr->new_def                     = FALSE;                    
        pp_assemblage_ptr->count_comps                 = this->ppAssemblageComps.size();
        pp_assemblage_ptr->pure_phases                 = (struct pure_phase *) free_check_null(pp_assemblage_ptr->pure_phases);
        pp_assemblage_ptr->pure_phases                 = cxxPPassemblageComp::cxxPPassemblageComp2pure_phase(this->ppAssemblageComps);
        pp_assemblage_ptr->next_elt                    = this->eltList.elt_list();
        return(pp_assemblage_ptr);
}

void cxxPPassemblage::dump_xml(std::ostream& s_oss, unsigned int indent)const
{
        //const char    ERR_MESSAGE[] = "Packing PPassemblage message: %s, element not found\n";
        unsigned int i;
        s_oss.precision(DBL_DIG - 1);
        std::string indent0(""), indent1(""), indent2("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);
        for(i = 0; i < indent + 1; ++i) indent1.append(Utilities::INDENT);
        for(i = 0; i < indent + 2; ++i) indent2.append(Utilities::INDENT);

        // PPassemblage element and attributes
        s_oss << indent0;
        s_oss << "<EQUILIBRIUM_PHASES " << std::endl;

        // eltList
        this->eltList.dump_xml(s_oss, indent + 1);

        // ppAssemblageComps
        s_oss << indent1;
        s_oss << "<pure_phases " << std::endl;
        for (std::list<cxxPPassemblageComp>::const_iterator it = ppAssemblageComps.begin(); it != ppAssemblageComps.end(); ++it) {
                it->dump_xml(s_oss, indent + 2);
        }
}

void cxxPPassemblage::dump_raw(std::ostream& s_oss, unsigned int indent)const
{
        //const char    ERR_MESSAGE[] = "Packing PPassemblage message: %s, element not found\n";
        unsigned int i;
        s_oss.precision(DBL_DIG - 1);
        std::string indent0(""), indent1(""), indent2("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);
        for(i = 0; i < indent + 1; ++i) indent1.append(Utilities::INDENT);
        for(i = 0; i < indent + 2; ++i) indent2.append(Utilities::INDENT);

        // PPassemblage element and attributes
        s_oss << indent0;
        s_oss << "EQUILIBRIUM_PHASES_RAW       " << this->n_user  << " " << this->description << std::endl;

        // eltList
        
        s_oss << indent1;
        s_oss << "-eltList       " << std::endl;
        this->eltList.dump_raw(s_oss, indent + 2);

        // ppAssemblagComps
        for (std::list<cxxPPassemblageComp>::const_iterator it = ppAssemblageComps.begin(); it != ppAssemblageComps.end(); ++it) {
                s_oss << indent1;
                s_oss << "-component" << std::endl;
                it->dump_raw(s_oss, indent + 2);
        }
}

void cxxPPassemblage::read_raw(CParser& parser)
{
        static std::vector<std::string> vopts;
        if (vopts.empty()) {
                vopts.reserve(15);
                vopts.push_back("eltlist");                    // 0
                vopts.push_back("component");                  // 1
        }                                                      
                                                               
        std::istream::pos_type ptr;                            
        std::istream::pos_type next_char;                      
        std::string token;                                     
        int opt_save;
        bool useLastLine(false);

        // Read PPassemblage number and description
        this->read_number_description(parser);

        opt_save = CParser::OPT_ERROR;

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
                        parser.error_msg("Unknown input in EQUILIBRIUM_PHASES_RAW keyword.", CParser::OT_CONTINUE);
                        parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
                        useLastLine = false;
                        break;

                case 0: // eltList
                        if ( this->eltList.read_raw(parser, next_char) != CParser::PARSER_OK) {
                                parser.incr_input_error();
                                parser.error_msg("Expected element name and moles for totals.", CParser::OT_CONTINUE);
                        }                       
                        opt_save = 0;
                        break;

                case 1: // component
                        {
                                cxxPPassemblageComp ppComp;
                                ppComp.read_raw(parser);
                                this->ppAssemblageComps.push_back(ppComp);
                        }
                        useLastLine = true;
                        break;
                }
                if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD) break;
        }
        // members that must be defined
}
#ifdef USE_MPI
/* ---------------------------------------------------------------------- */
void cxxPPassemblage::mpi_pack(std::vector<int>& ints, std::vector<double>& doubles)
/* ---------------------------------------------------------------------- */
{
	/* int n_user; */
	ints.push_back(this->n_user);
	ints.push_back(this->ppAssemblageComps.size());
	for (std::list<cxxPPassemblageComp>::iterator it = this->ppAssemblageComps.begin(); it != this->ppAssemblageComps.end(); it++) {
		it->mpi_pack(ints, doubles);
	}
	this->eltList.mpi_pack(ints, doubles);
}
/* ---------------------------------------------------------------------- */
void cxxPPassemblage::mpi_unpack(int *ints, int *ii, double *doubles, int *dd)
/* ---------------------------------------------------------------------- */
{
	int i = *ii;
	int d = *dd;
	/* int n_user; */
	this->n_user = ints[i++];
	this->n_user_end = this->n_user;
	this->description = " ";

	int count = ints[i++];
	this->ppAssemblageComps.clear();
	for (int n = 0; n < count; n++) {
		cxxPPassemblageComp ppc;
		ppc.mpi_unpack(ints, &i, doubles, &d);
		this->ppAssemblageComps.push_back(ppc);
	}
	this->eltList.mpi_unpack(ints, &i, doubles, &d);
	*ii = i;
	*dd = d;
}
#endif
