#if !defined(SSASSEMBLAGESS_H_INCLUDED)
#define SSASSEMBLAGESS_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

#include "NameDouble.h"

class cxxSSassemblageSS
{

  public:
	cxxSSassemblageSS();
	cxxSSassemblageSS(struct s_s *);
	 ~cxxSSassemblageSS();

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

	static struct s_s *cxxSSassemblageSS2s_s(std::map < std::string, cxxSSassemblageSS >
											 &el);

	void dump_xml(std::ostream & os, unsigned int indent = 0) const;

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void read_raw(CParser & parser, bool check = true);

	const std::string &get_name() const
	{
		return this->name;
	}
	void set_name (const char * s)
	{
		if (s != NULL)
			this->name = std::string(s);
		else
			this->name.clear();
	}

	void totalize();
	const cxxNameDouble & get_totals() const
	{
		return (this->totals);
	};

#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < double >&doubles);
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
#endif

	void add(const cxxSSassemblageSS & comp, double extensive);
	void multiply(double extensive);

protected:
	std::string name;
	cxxNameDouble comps;
	double a0, a1;
	double ag0, ag1;
	bool miscibility;
	double xb1, xb2;
	cxxNameDouble totals;

public:

};

#endif // !defined(SSASSEMBLAGESS_H_INCLUDED)
