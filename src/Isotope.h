#if !defined(ISOTOPE_H_INCLUDED)
#define ISOTOPE_H_INCLUDED

#include "Parser.h"
#include <ostream>   // std::ostream
#include <string>    // std::string
#include <list>    // std::list

class cxxIsotope
{
public:
        cxxIsotope(void);
        cxxIsotope(struct isotope *isotope_ptr);
        ~cxxIsotope(void);

        enum STATUS {
                ERROR = 0,
                OK    = 1
        };

        //cxxIsotope::STATUS read(CParser& parser);
        static struct isotope * list2isotope(std::list<cxxIsotope> &t);

        void dump_xml(std::ostream& os, unsigned int indent)const;
        void dump_raw(std::ostream& os, unsigned int indent)const;

        CParser::STATUS_TYPE read_raw(CParser& parser);

        char * get_isotope_name()const          { return this->isotope_name;}
        void   set_isotope_name(char * cstring) { this->isotope_name = cstring;}
        char * get_elt_name()const              { return this->elt_name;}
        void   set_elt_name(char * cstring)     { this->elt_name = cstring;}

        double get_ratio()const              { return this->ratio; }

        double get_ratio_uncertainty()const  { return this->ratio_uncertainty; }

        bool get_ratio_uncertainty_defined()const  { return this->ratio_uncertainty_defined; }

        bool operator<(const cxxIsotope& conc)const;
        
        struct master *master(void);
        struct master *primary(void);

private:
        double       isotope_number;
        char *       elt_name;
        char *       isotope_name;
        double       total;
        double       ratio;
        double       ratio_uncertainty;
        //struct master *master;
        //struct master *primary;
        bool         ratio_uncertainty_defined;
};
#endif // ISOTOPE_H_INCLUDED
