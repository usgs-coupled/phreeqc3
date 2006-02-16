#if !defined(CONC_H_INCLUDED)
#define CONC_H_INCLUDED

//#include "Parser.h"
#include "Utils.h"

#include <string>
#include <map>     // std::map
#include <vector>
#include <set>
#include "char_star.h"

// forward declarations
class cxxISolution;  // reqd for read and dump_xml

class cxxConc
{
public:
        cxxConc(void);
        cxxConc(struct conc *conc_ptr);
        ~cxxConc(void);

        enum STATUS_TYPE {
                ERROR  = 0,
                OK     = 1
        };

public:

        //STATUS_TYPE read(CParser& parser, CSolution& sol);

        void dump_xml(std::ostream& os, unsigned int indent = 0)const;

        double get_input_conc()const {return this->input_conc;}
        void set_input_conc(double input_conc) {this->input_conc = input_conc;}

        std::string get_equation_name()const {return this->equation_name;}
        void set_equation_name(char * equation_name) {this->equation_name = equation_name;}

        std::string get_description()const {return this->description;}
        void set_description(char * description) {this->description = description;}

        std::string get_units()const {return this->units;}
        void set_units(char * units) {this->units = units;}

        int get_n_pe()const {return this->n_pe;}
        void set_n_pe(int n_pe) {this->n_pe = n_pe;}

        //bool operator<(const cxxConc& conc)const    { return (this->description < conc.description); }
        bool operator<(const cxxConc& conc)const    { return ::strcmp(this->description, conc.description) < 0; }

        //static struct conc * concarray(std::map<char *, double, CHARSTAR_LESS> &t );

        static struct conc * cxxConc2conc(const std::set<cxxConc> &t );

private:
        char * description;
        double moles;
        double input_conc;
        char * units;
        char * equation_name;
        //struct phase *phase;
        double phase_si;
        int n_pe;
        char * as;
        double gfw;
        //int skip;
};

#endif // CONC_H_INCLUDED
