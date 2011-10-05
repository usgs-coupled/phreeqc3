#if !defined(GASPHASE_H_INCLUDED)
#define GASPHASE_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

#include "NumKeyword.h"
#include "NameDouble.h"
#include "phreeqc_class.h"
class cxxMix;

class cxxGasPhase:public cxxNumKeyword
{

  public:
	cxxGasPhase(PHRQ_io * io=NULL);
	cxxGasPhase(struct gas_phase *, PHRQ_io * io=NULL);
	  cxxGasPhase(const std::map < int, cxxGasPhase > &entity_map,
				  cxxMix & mx, int n_user, PHRQ_io * io=NULL);
	 ~cxxGasPhase();

	enum GP_TYPE
	{
		GP_PRESSURE = 0,
		GP_VOLUME = 1
	};

	//void dump_xml(std::ostream& os, unsigned int indent = 0)const;

	void dump_raw(std::ostream & s_oss, unsigned int indent, int *n_out=NULL) const;

	void read_raw(CParser & parser, bool check = true);

#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < double >&doubles);
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
#endif

	void totalize(PHREEQC_PTR_ARG);

	const cxxNameDouble & Get_totals() const
	{
		return this->totals;
	};
	const cxxNameDouble & Get_gasPhaseComps(void) const {return gasPhaseComps;};
	GP_TYPE Get_type(void) const {return type;};
	double Get_total_p(void) const {return total_p;};
	double Get_volume(void) const {return volume;};
	cxxNameDouble & Get_totals(void) {return totals;};

private:
	void add(const cxxGasPhase & addee, double extensive);

protected:
	cxxNameDouble gasPhaseComps;
	GP_TYPE type;
	double total_p;
	double volume;
	cxxNameDouble totals;

public:
	//static std::map<int, cxxGasPhase>& map;

};

#endif // !defined(GASPHASE_H_INCLUDED)
