#if !defined(SOLUTION_H_INCLUDED)
#define SOLUTION_H_INCLUDED

#include "NumKeyword.h"
#include "Isotope.h"
#include "Conc.h"
#include "NameDouble.h"
#define EXTERNAL extern
#include "global.h"
#include <cassert> // assert
#include <map>     // std::map
#include <string>  // std::string
#include <list>    // std::list
#include <vector>  // std::vector

#include "char_star.h"

class cxxSolution : public cxxNumKeyword
{

public:
        cxxSolution();
        cxxSolution(struct solution *);
        //cxxSolution(const cxxSolution&);
        ~cxxSolution();

        //static cxxSolution& read(CParser& parser);

        double get_tc()const {return this->tc;}
        void set_tc(double tc) {this->tc = tc;}

        double get_ph()const {return this->ph;}
        void set_ph(double pH) {this->ph = pH;}

        double get_pe()const {return this->pe;}
        void set_pe(double pe) {this->pe =pe;}

        double get_mu()const {return this->mu;}
        void set_mu(double mu) {this->pe = mu;}

        double get_ah2o()const {return this->ah2o;}
        void set_ah2o(double ah2o) {this->pe = ah2o;}

        double get_total_h()const {return this->total_h;}
        void set_total_h(double total_h) {this->pe = mu;}

        double get_total_o()const {return this->total_o;}
        void set_total_o(double total_o) {this->pe = mu;}

        double get_mass_water()const {return this->mass_water;};
        void set_mass_water(long double mass_water) {this->mass_water = mass_water;};

        double get_total_alkalinity()const {return this->total_alkalinity;}
        void set_total_alkalinity(double total_alkalinity) {this->pe = total_alkalinity;}

        //char * get_pe_reaction()const {return this->pe_reaction;}
        //void set_pe_reaction(char * pe_reaction) {this->pe_reaction = pe_reaction;}

        struct solution *cxxSolution2solution();

        void dump_xml(std::ostream& os, unsigned int indent = 0)const;

        void dump_raw(std::ostream& s_oss, unsigned int indent)const;

        void read_raw(CParser& parser);


protected:
        double tc;
        double ph;
        double pe;
        double mu;
        double ah2o;
        double total_h;
        double total_o;
        double cb;
        double mass_water;
        double total_alkalinity;
        // maps element name to moles
        //std::map <char *, double, CHARSTAR_LESS> totals; 
        cxxNameDouble totals;
        std::list<cxxIsotope> isotopes;
        // maps master species name log activity
        //std::map <char *, double> master_activity;
        cxxNameDouble master_activity;
        // maps species name to Pitzer activty coefficient 
        //std::map <char *, double> species_gamma;
        cxxNameDouble species_gamma;
public:
        //static std::map<int, cxxSolution>& map;

};

#endif // !defined(SOLUTION_H_INCLUDED)
