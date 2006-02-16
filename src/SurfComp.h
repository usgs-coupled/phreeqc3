#if !defined(SURFCOMP_H_INCLUDED)
#define SURFCOMP_H_INCLUDED

#include "NameDouble.h"
#define EXTERNAL extern
#include "global.h"
#include <cassert> // assert
#include <map>     // std::map
#include <string>  // std::string
#include <list>    // std::list
#include <vector>  // std::vector

#include "char_star.h"

class cxxSurfComp 
{

public:

        cxxSurfComp();
        cxxSurfComp(struct surface_comp *);
        ~cxxSurfComp();


        struct master *get_master();
        char *get_phase_name()const {return this->phase_name;}
        char *get_rate_name()const {return this->rate_name;}

        static struct surface_comp *cxxSurfComp2surface_comp(std::list<cxxSurfComp>& el);

        void dump_xml(std::ostream& os, unsigned int indent = 0)const;

        void dump_raw(std::ostream& s_oss, unsigned int indent)const;

        void read_raw(CParser& parser);

protected:
        char * formula;
        double moles;
        cxxNameDouble totals; 
        double la;
        int charge_number;
        double charge_balance;
        char   *phase_name;
        double phase_proportion;
        char   *rate_name;

public:

};

#endif // !defined(SURFCOMP_H_INCLUDED)
