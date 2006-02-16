// NameDouble.cxx: implementation of the cxxNameDouble class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)   // disable truncation warning (Only used by debugger)
#endif

#include "Utils.h"   // define first
#include "Conc.h"
#include "NameDouble.h"
#define EXTERNAL extern
#include "global.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include <cassert>     // assert
#include <algorithm>   // std::sort 
#include <map>   // std::sort 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxNameDouble::cxxNameDouble() 
        //
        // default constructor for cxxNameDouble 
        //
{
}

cxxNameDouble::cxxNameDouble(struct elt_list *elt_list_ptr)
        //
        // constructor for cxxNameDouble from list of elt_list
        //
{
        int i;
        if (elt_list_ptr != NULL) {
                for (i = 0; elt_list_ptr[i].elt != NULL; i++) {
                        (*this)[elt_list_ptr[i].elt->name] = elt_list_ptr[i].coef;
                }
        }
        this->type = ND_ELT_MOLES;
}
cxxNameDouble::cxxNameDouble(struct conc *tots)
        //
        // constructor for cxxNameDouble from list of elt_list
        //
{
        int i;
        for (i = 0; tots[i].description != NULL; i++) {
                (*this)[tots[i].description] = tots[i].moles;
        }
        this->type = ND_ELT_MOLES;
}
cxxNameDouble::cxxNameDouble(struct master_activity *ma, int count, cxxNameDouble::ND_TYPE)
        //
        // constructor for cxxNameDouble from list of elt_list
        //
{
        int i;
        for (i = 0; i < count; i++) {
                if (ma[i].description == NULL) continue;
                (*this)[ma[i].description] = ma[i].la;
        }
        this->type = ND_SPECIES_LA;
}
cxxNameDouble::cxxNameDouble(struct name_coef *nc, int count, cxxNameDouble::ND_TYPE)
        //
        // constructor for cxxNameDouble from list of elt_list
        //
{
        int i;
        for (i = 0; i < count; i++) {
                if (nc[i].name == NULL) continue;
                (*this)[nc[i].name] = nc[i].coef;
        }
        this->type = ND_NAME_COEF;
}
cxxNameDouble::~cxxNameDouble()
{
}

struct elt_list *cxxNameDouble::elt_list()
        //
        // Builds a exch_comp structure from instance of cxxNameDouble 
        //
{
        assert (this->type == cxxNameDouble::ND_ELT_MOLES);
        struct elt_list *elt_list_ptr = (struct elt_list *) PHRQ_malloc((size_t)((this->size() + 1) *sizeof(struct elt_list)));
        if (elt_list_ptr == NULL) malloc_error();
        int i = 0;
        for (iterator it = this->begin(); it != this->end(); ++it) {
                elt_list_ptr[i].elt = element_store(it->first);
                elt_list_ptr[i].coef = it->second;
                i++;
        }
        elt_list_ptr[i].elt = NULL;
        elt_list_ptr[i].coef = 0;
        return(elt_list_ptr);
}

struct master_activity *cxxNameDouble::master_activity()const
        //
        // Builds a list of master_activity structures from instance of cxxNameDouble 
        //
{
        int i = 0;
        assert (this->type == cxxNameDouble::ND_SPECIES_LA || this->type == cxxNameDouble::ND_SPECIES_GAMMA);
        struct master_activity *master_activity_ptr = NULL;
        switch ((*this).type) {
        case cxxNameDouble::ND_SPECIES_LA:
                {
                master_activity_ptr= (struct master_activity *) PHRQ_malloc((size_t) (((*this).size() + 1) * sizeof(struct master_activity)));
                if (master_activity_ptr == NULL) malloc_error();
                for (const_iterator it = (*this).begin(); it != (*this).end(); it++) {
                        master_activity_ptr[i].description = (char *)it->first;
                        master_activity_ptr[i].la = it->second;
                        i++;
                }
                }
                master_activity_ptr[i].description = NULL;
                break;
        case cxxNameDouble::ND_SPECIES_GAMMA:
                {
                if ((*this).size() > 0) {
                        master_activity_ptr = (struct master_activity *) PHRQ_malloc((size_t) (((*this).size()) * sizeof(struct master_activity)));
                        if (master_activity_ptr == NULL) malloc_error();
                        for (const_iterator it = (*this).begin(); it != (*this).end(); it++) {
                                master_activity_ptr[i].description = (char *)it->first;
                                master_activity_ptr[i].la = it->second;
                                i++;
                        }
                }
                }
                break;
        case cxxNameDouble::ND_ELT_MOLES:
        case cxxNameDouble::ND_NAME_COEF:
                break;
        }
        return(master_activity_ptr);
}

struct conc *cxxNameDouble::conc()const
        // for Solutions, not ISolutions
        // takes a map of (elt name, moles)
        // returns list of conc structures
{
        struct conc *c;
        assert (this->type == cxxNameDouble::ND_ELT_MOLES);
        c = (struct conc *) PHRQ_malloc((size_t) (((*this).size() + 1) * sizeof(struct conc)));
        if (c == NULL) malloc_error();
        int i = 0;
        for (const_iterator it = (*this).begin(); it != (*this).end(); ++it) {
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

struct name_coef *cxxNameDouble::name_coef()const
        //
        // Builds a name_coef structure from instance of cxxNameDouble 
        //
{
        assert (this->type == cxxNameDouble::ND_NAME_COEF);
        struct name_coef *name_coef_ptr = (struct name_coef *) PHRQ_malloc((size_t)((this->size()) *sizeof(struct name_coef)));
        if (name_coef_ptr == NULL) malloc_error();
        int i = 0;
        for (const_iterator it = (*this).begin(); it != (*this).end(); ++it) {
                name_coef_ptr[i].name = it->first;
                name_coef_ptr[i].coef = it->second;
                i++;
        }
        return(name_coef_ptr);
}

void cxxNameDouble::dump_xml(std::ostream& s_oss, unsigned int indent)const
{
        //const char    ERR_MESSAGE[] = "Packing exch_comp message: %s, element not found\n";
        unsigned int i;
        s_oss.precision(DBL_DIG - 1);
        std::string indent0(""), indent1("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);
        for(i = 0; i < indent + 1; ++i) indent1.append(Utilities::INDENT);
        std::string xmlElement, xmlAtt1, xmlAtt2;

        switch ((*this).type) {
        case cxxNameDouble::ND_SPECIES_LA:
                xmlElement = "<soln_m_a ";
                xmlAtt1 = " m_a_desc=\"";
                xmlAtt1 = " m_a_la=\"";
                break;
        case cxxNameDouble::ND_SPECIES_GAMMA:
                xmlElement = "<soln_s_g ";
                xmlAtt1 = " m_a_desc=\"";
                xmlAtt1 = " m_a_la=\"";
                break;
        case cxxNameDouble::ND_ELT_MOLES:
                xmlElement = "<soln_total ";
                xmlAtt1 = " conc_desc=\"";
                xmlAtt1 = " conc_moles=\"";
                break;
        case cxxNameDouble::ND_NAME_COEF:
                xmlElement = "<NameCoef ";
                xmlAtt1 = " name=\"";
                xmlAtt1 = " coef=\"";
		break;
        }
        
        for (const_iterator it = (*this).begin(); it != (*this).end(); it++) {
                s_oss << indent0;
                s_oss << xmlElement << xmlAtt1 << it->first << xmlAtt2 << it->second << "/>" << std::endl;
        }
}

void cxxNameDouble::dump_raw(std::ostream& s_oss, unsigned int indent)const
{
        //const char    ERR_MESSAGE[] = "Packing exch_comp message: %s, element not found\n";
        unsigned int i;
        s_oss.precision(DBL_DIG - 1);
        std::string indent0("");
        for(i = 0; i < indent; ++i) indent0.append(Utilities::INDENT);
        
        for (const_iterator it = (*this).begin(); it != (*this).end(); it++) {
                s_oss << indent0;
                s_oss << it->first << "   " << it->second << std::endl;
        }
}
CParser::STATUS_TYPE cxxNameDouble::read_raw(CParser& parser, std::istream::pos_type& pos)
{
        std::string token;
        char * ctoken;
        double d;

        CParser::TOKEN_TYPE j;

        //m_line_iss.seekg(pos);        

        j = parser.copy_token(token, pos);

        if (j == CParser::TT_EMPTY) return CParser::PARSER_OK;

        if( !(parser.get_iss() >> d)) {
                return CParser::PARSER_ERROR;
        }
        ctoken = string_hsave(token.c_str());
        (*this)[ctoken] = d;
        return CParser::PARSER_OK;
}
