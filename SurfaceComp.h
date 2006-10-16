#if !defined(SURFACECOMP_H_INCLUDED)
#define SURFACECOMP_H_INCLUDED

#include "NameDouble.h"
#define EXTERNAL extern
#include "global.h"
#include <cassert> // assert
#include <map>     // std::map
#include <string>  // std::string
#include <list>    // std::list
#include <vector>  // std::vector

#include "char_star.h"

class cxxSurfaceComp 
{

public:

        cxxSurfaceComp();
        cxxSurfaceComp(struct surface_comp *);
        ~cxxSurfaceComp();


        struct master *get_master();
        char *get_phase_name()const {return this->phase_name;}
        char *get_rate_name()const {return this->rate_name;}

        static struct surface_comp *cxxSurfaceComp2surface_comp(std::list<cxxSurfaceComp>& el);

        void dump_xml(std::ostream& os, unsigned int indent = 0)const;

        void dump_raw(std::ostream& s_oss, unsigned int indent)const;

        void read_raw(CParser& parser);

#ifdef USE_MPI
	void mpi_pack(std::vector<int>& ints, std::vector<double>& doubles);
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
#endif

protected:
        char * formula;
        cxxNameDouble formula_totals; 
	double formula_z;
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

#endif // !defined(SURFACECOMP_H_INCLUDED)
