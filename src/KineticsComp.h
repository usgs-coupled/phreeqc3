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

	void dump_xml(std::ostream & os, unsigned int indent = 0) const;

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void read_raw(CParser & parser, bool check = true);

	const std::string &Get_rate_name() const
	{
		return this->rate_name;
	}
	void Set_rate_name(const char * s)
	{
		if (s != NULL)
			this->rate_name = std::string(s);
		else
			this->rate_name.clear();
	}

	const cxxNameDouble &Get_namecoef(void) const {return namecoef;};
	LDBLE Get_tol(void) const {return tol;};	
	LDBLE Get_m(void) const {return m;};	
	LDBLE Get_m0(void) const {return m0;};	
	LDBLE Get_moles(void) const {return moles;};	
	const std::vector < LDBLE > &Get_d_params(void) const {return d_params;};

#ifdef USE_MPI
	void mpi_unpack(int *ints, int *ii, LDBLE *doubles, int *dd);
	void mpi_pack(std::vector < int >&ints, std::vector < LDBLE >&doubles);
#endif
	void add(const cxxKineticsComp & comp, LDBLE extensive);
	void multiply(LDBLE extensive);

  protected:
	  std::string rate_name;
	  cxxNameDouble namecoef;		//stoichiometry of reaction
	  LDBLE tol;
	  LDBLE m;
	  LDBLE m0;
	  LDBLE moles;
	  std::vector < LDBLE >d_params;

  public:

};

#endif // !defined(KINETICSCOMP_H_INCLUDED)
