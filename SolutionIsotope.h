#if !defined(SOLUTIONISOTOPE_H_INCLUDED)
#define SOLUTIONISOTOPE_H_INCLUDED

#include "Parser.h"
#include <ostream>   // std::ostream
#include <string>    // std::string
#include <list>    // std::list

class cxxSolutionIsotope
{
public:
        cxxSolutionIsotope(void);
        cxxSolutionIsotope(struct isotope *isotope_ptr);
        ~cxxSolutionIsotope(void);

	/*
        enum STATUS {
                ERROR = 0,
                OK    = 1
        };
	*/
        //cxxSolutionIsotope::STATUS read(CParser& parser);
        static struct isotope * list2isotope(std::list<cxxSolutionIsotope> &t);

        void dump_xml(std::ostream& os, unsigned int indent)const;
        void dump_raw(std::ostream& os, unsigned int indent)const;

        CParser::STATUS_TYPE read_raw(CParser& parser);

        double get_isotope_number()const        { return this->isotope_number;}
        void   set_isotope_number(double d)     { this->isotope_number = d;}
        char * get_elt_name()const              { return this->elt_name;}
        void   set_elt_name(char * cstring)     { this->elt_name = cstring;}
        char * get_isotope_name()const          { return this->isotope_name;}
        void   set_isotope_name(char * cstring) { this->isotope_name = cstring;}
        double get_total()const                 { return this->total;}
        void   set_total(double d)              { this->total = d;}

        double get_ratio()const              { return this->ratio; }

        double get_ratio_uncertainty()const  { return this->ratio_uncertainty; }

        bool get_ratio_uncertainty_defined()const  { return this->ratio_uncertainty_defined; }

        bool operator<(const cxxSolutionIsotope& conc)const;
        
        struct master *master(void);
        struct master *primary(void);

	void add(const cxxSolutionIsotope &isotope_ptr, double intensive, double extensive);
	void multiply(double extensive);

protected:
	friend class cxxSolutionIsotopeList;
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
#endif // SOLUTIONISOTOPE_H_INCLUDED
