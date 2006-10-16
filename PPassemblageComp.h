#if !defined(PPASSEMBLAGECOMP_H_INCLUDED)
#define PPASSEMBLAGECOMP_H_INCLUDED

#include "NameDouble.h"
#define EXTERNAL extern
#include "global.h"
#include <cassert> // assert
#include <map>     // std::map
#include <string>  // std::string
#include <list>    // std::list
#include <vector>  // std::vector

#include "char_star.h"

class cxxPPassemblageComp 
{

public:
        cxxPPassemblageComp();
        cxxPPassemblageComp(struct pure_phase *);
        ~cxxPPassemblageComp();


        static struct pure_phase *cxxPPassemblageComp2pure_phase(std::list<cxxPPassemblageComp>& el);

        void dump_xml(std::ostream& os, unsigned int indent = 0)const;

        void dump_raw(std::ostream& s_oss, unsigned int indent)const;

        void read_raw(CParser& parser);

        struct phase *get_phase();
#ifdef USE_MPI
	void mpi_pack(std::vector<int>& ints, std::vector<double>& doubles);
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
#endif
protected:
        char * name;
        char *add_formula;
        double si;
        double moles;
        double delta;
        double initial_moles;
	bool force_equality;
        bool dissolve_only;

public:

};

#endif // !defined(PPASSEMBLAGECOMP_H_INCLUDED)
