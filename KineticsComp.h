#if !defined(KINETICSCOMP_H_INCLUDED)
#define KINETICSCOMP_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

#include "NameDouble.h"

class cxxKineticsComp: public PHRQ_base
{

public:
	cxxKineticsComp(PHRQ_io *io=NULL);
	cxxKineticsComp(struct kinetics_comp *, PHRQ_io *io=NULL);
	~cxxKineticsComp();

	//static struct kinetics_comp *cxxKineticsComp2kinetics_comp(PHREEQC_PTR_ARG_COMMA std::list < cxxKineticsComp > &el);

	void dump_xml(std::ostream & os, unsigned int indent = 0) const;

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void read_raw(CParser & parser, bool check = true);

	const std::string &Get_rate_name() const
	{
		return this->rate_name;
	}
	void Set_rate_name(char * s)
	{
		if (s != NULL)
			this->rate_name = std::string(s);
		else
			this->rate_name.clear();
	}

	cxxNameDouble &Get_namecoef(void) {return namecoef;};
	double Get_tol(void) {return tol;};	
	double Get_m(void) {return m;};	
	double Get_m0(void) {return m0;};	
	double Get_moles(void) {return moles;};	
	std::vector < double > &Get_d_params(void) {return d_params;};

#ifdef USE_MPI
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
	void mpi_pack(std::vector < int >&ints, std::vector < double >&doubles);
#endif
	void add(const cxxKineticsComp & comp, double extensive);
	void multiply(double extensive);

  protected:
	  std::string rate_name;
	  cxxNameDouble namecoef;		//stoichiometry of reaction
	  double tol;
	  double m;
	  double m0;
	  double moles;
	  std::vector < double >d_params;

  public:

};

#endif // !defined(KINETICSCOMP_H_INCLUDED)
