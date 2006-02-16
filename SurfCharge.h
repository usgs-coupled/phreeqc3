#if !defined(SURFCHARGE_H_INCLUDED)
#define SURFCHARGE_H_INCLUDED

#include "NameDouble.h"
#define EXTERNAL extern
#include "global.h"
#include <cassert> // assert
#include <map>     // std::map
#include <string>  // std::string
#include <list>    // std::list
#include <vector>  // std::vector

#include "char_star.h"

class cxxSurfCharge 
{

public:

        cxxSurfCharge();
        cxxSurfCharge(struct surface_charge *);
        ~cxxSurfCharge();


        struct master *get_psi_master();

        static struct surface_charge *cxxSurfCharge2surface_charge(std::list<cxxSurfCharge>& el);

        void dump_xml(std::ostream& os, unsigned int indent = 0)const;

        void dump_raw(std::ostream& s_oss, unsigned int indent)const;

        void read_raw(CParser& parser);

protected:
        char * name;
        double specific_area;
        double grams;
        double charge_balance;
        double mass_water;
        double la_psi;
        //std::map<double, cxxSurfDL> g;
        //char * psi_master_name;
        cxxNameDouble diffuse_layer_totals; 

public:

};

#endif // !defined(SURFCHARGE_H_INCLUDED)
