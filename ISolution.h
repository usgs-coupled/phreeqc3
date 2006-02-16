#if !defined(ISOLUTION_H_INCLUDED)
#define ISOLUTION_H_INCLUDED

//#include "Parser.h"
#include "NumKeyword.h"
#include "Solution.h"
#include "Conc.h"
//#include "Isotope.h"
#include <cassert> // assert
#include <map>     // std::map
#include <string>  // std::string
#include <list>    // std::list
#include <vector>  // std::vector
#include <set>     // std::set


class cxxISolution : public cxxSolution
{

public:
        cxxISolution();
        cxxISolution(struct solution *);
        //cxxISolution(const cxxISolution&);
        ~cxxISolution();

        //static cxxISolution& read(CParser& parser);

        //void add(cxxConc conc)       { this->concs.push_back(conc); }

        struct solution *cxxISolution2solution();

        double get_density()const {return this->density;}
        void set_density(double density) {this->density = density;}

        std::string get_units()const {return units;}
        void set_units(std::string units) {units = units;}

        //char * get_redox()const {return this->pe[this->default_pe].get_name();}

        //void dump_xml(std::ostream& os, unsigned int indent = 0)const;

protected:
        friend class cxxConc; // for this->pe access
        double density;
        std::string units;
        std::set<cxxConc> concs; 
        //std::map <char *, struct reaction *> pe;
        struct pe_data *pes;
        int default_pe;

public:
        //static std::map<int, cxxISolution>& map;

};

#endif // !defined(ISOLUTION_H_INCLUDED)
