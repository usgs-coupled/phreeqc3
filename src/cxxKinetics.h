#if !defined(CXXKINETICS_H_INCLUDED)
#define CXXKINETICS_H_INCLUDED

#include "NumKeyword.h"
#define EXTERNAL extern
#include "global.h"
#include <cassert> // assert
#include <map>     // std::map
#include <string>  // std::string
#include <list>    // std::list
#include <vector>  // std::vector

#include "char_star.h"
#include "KineticsComp.h"

class cxxKinetics : public cxxNumKeyword
{

public:
        cxxKinetics();
        cxxKinetics(struct kinetics *);
        ~cxxKinetics();

        struct kinetics *cxxKinetics2kinetics();

        struct kinetics_comp *cxxKineticsComp2kinetics_comp();

        //void dump_xml(std::ostream& os, unsigned int indent = 0)const;

        void dump_raw(std::ostream& s_oss, unsigned int indent)const;

        void read_raw(CParser& parser);

        bool get_related_phases(void);

        bool get_related_rate(void);

#ifdef USE_MPI
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
	void mpi_pack(std::vector<int>& ints, std::vector<double>& doubles);
#endif

protected:
        std::list<cxxKineticsComp> kineticsComps;
        std::vector<double> steps;
        cxxNameDouble totals;
        double step_divide;
        int rk;
        int bad_step_max;
        bool use_cvode;
public:
        //static std::map<int, cxxKinetics>& map;

};

#endif // !defined(CXXKINETICS_H_INCLUDED)
