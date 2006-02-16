#if !defined(KINETICSCOMP_H_INCLUDED)
#define KINETICSCOMP_H_INCLUDED

#include "NameDouble.h"
#define EXTERNAL extern
#include "global.h"
#include <cassert> // assert
#include <map>     // std::map
#include <string>  // std::string
#include <list>    // std::list
#include <vector>  // std::vector

#include "char_star.h"

class cxxKineticsComp 
{

public:
        cxxKineticsComp();
        cxxKineticsComp(struct kinetics_comp *);
        ~cxxKineticsComp();

        static struct kinetics_comp *cxxKineticsComp2kinetics_comp(std::list<cxxKineticsComp>& el);

        void dump_xml(std::ostream& os, unsigned int indent = 0)const;

        void dump_raw(std::ostream& s_oss, unsigned int indent)const;

        void read_raw(CParser& parser);

protected:
        char * rate_name;
        cxxNameDouble namecoef;
        double tol;
        double m;
        double m0;
        double moles;
        std::vector<double> d_params;

public:

};

#endif // !defined(KINETICSCOMP_H_INCLUDED)
