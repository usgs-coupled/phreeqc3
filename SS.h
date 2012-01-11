#if !defined(SSASSEMBLAGESS_H_INCLUDED)
#define SSASSEMBLAGESS_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector
#include "phrqtype.h"
#include "NameDouble.h"
#include "Phreeqc_class.h"

class cxxSS: public PHRQ_base
{

  public:
	cxxSS(PHRQ_io *io=NULL);
	cxxSS(struct s_s *, PHRQ_io *io=NULL);
	 ~cxxSS();

	enum SS_PARAMETER_TYPE
	{
		SS_PARM_NONE = -1,
		SS_PARM_A0_A1 = 0,
		SS_PARM_GAMMAS = 1,
		SS_PARM_DIST_COEF = 2,
		SS_PARM_MISCIBILITY = 3,
		SS_PARM_SPINODAL = 4,
		SS_PARM_CRITICAL = 5,
		SS_PARM_ALYOTROPIC = 6,
		SS_PARM_DIM_GUGG = 7,
		SS_PARM_WALDBAUM = 8,
		SS_PARM_MARGULES = 9
	};

	void dump_xml(std::ostream & os, unsigned int indent = 0) const;

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void read_raw(CParser & parser, bool check = true);

	const std::string &Get_name() const
	{
		return this->name;
	}
	void Set_name (const char * s)
	{
		if (s != NULL)
			this->name = std::string(s);
		else
			this->name.clear();
	}

	void totalize(PHREEQC_PTR_ARG);
	const cxxNameDouble & Get_totals() const
	{
		return (this->totals);
	};
	const cxxNameDouble & Get_comps() const
	{
		return (this->comps);
	};

#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < LDBLE >&doubles);
	void mpi_unpack(int *ints, int *ii, LDBLE *doubles, int *dd);
#endif

	void add(const cxxSS & comp, LDBLE extensive);
	void multiply(LDBLE extensive);
	LDBLE Get_a0() const {return (this->a0);};
	LDBLE Get_a1() const {return (this->a1);};
	LDBLE Get_ag0() const {return (this->ag0);};
	LDBLE Get_ag1() const {return (this->ag1);};
	bool Get_miscibility() const {return (this->miscibility);};
	LDBLE Get_xb1() const {return (this->xb1);};
	LDBLE Get_xb2() const {return (this->xb2);};
protected:
	std::string name;
	cxxNameDouble comps;
	LDBLE a0, a1;
	LDBLE ag0, ag1;
	bool miscibility;
	LDBLE xb1, xb2;
	cxxNameDouble totals;

public:

};

#endif // !defined(SSASSEMBLAGESS_H_INCLUDED)
