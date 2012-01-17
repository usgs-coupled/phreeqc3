#if !defined(SSASSEMBLAGESS_H_INCLUDED)
#define SSASSEMBLAGESS_H_INCLUDED

#include <cassert>				// assert
#include <vector>
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector
#include "phrqtype.h"
#include "NameDouble.h"
#include "Phreeqc_class.h"
#include "SScomp.h"


class cxxSS: public PHRQ_base
{

  public:
	cxxSS(PHRQ_io *io=NULL);
	//cxxSS(struct s_s *, PHRQ_io *io=NULL);
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
	void Set_name (const std::string & s) {this->name = s;}
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
#ifdef SKIP
	const cxxNameDouble & Get_comps() const
	{
		return (this->comps);
	};
#endif
#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < LDBLE >&doubles);
	void mpi_unpack(int *ints, int *ii, LDBLE *doubles, int *dd);
#endif
	cxxSScomp * Find(const char * comp_name);

	void add(const cxxSS & comp, LDBLE extensive);
	void multiply(LDBLE extensive);

	LDBLE Get_total_moles(void) const {return total_moles;}
	void Set_total_moles(LDBLE t) {total_moles = t;}

	LDBLE Get_dn(void) const {return (this->dn);}
	void Set_dn(LDBLE t) {this->dn = t;}

	LDBLE Get_a0() const {return (this->a0);}
	void Set_a0(LDBLE t) {a0 = t;}
	LDBLE Get_a1() const {return (this->a1);}
	void Set_a1(LDBLE t) {a1 = t;}
	LDBLE Get_ag0() const {return (this->ag0);}
	void Set_ag0(LDBLE t) {ag0 = t;}
	LDBLE Get_ag1() const {return (this->ag1);}
	void Set_ag1(LDBLE t) {ag1 = t;}
	bool Get_ss_in() const {return (this->ss_in);}
	void Set_ss_in(bool t) {ss_in = t;}
	bool Get_miscibility() const {return (this->miscibility);}
	void Set_miscibility(bool t) {miscibility = t;}
	bool Get_spinodal() const {return (this->spinodal);}
	void Set_spinodal(bool t) {spinodal = t;}
	LDBLE Get_xb1() const {return (this->xb1);}
	void Set_xb1(LDBLE t) {xb1 = t;}
	LDBLE Get_xb2() const {return (this->xb2);}
	void Set_xb2(LDBLE t) {xb2 = t;}
	std::vector<cxxSScomp> & Get_ss_comps(void) {return ss_comps;}
	void Set_ss_comps(const std::vector<cxxSScomp> & comps) {ss_comps = comps;}

	LDBLE Get_tk(void) {return this->tk;}
	void Set_tk(LDBLE t) {this->tk = t;}
	SS_PARAMETER_TYPE Get_input_case(void) {return this->input_case;}
	void Set_input_case(SS_PARAMETER_TYPE t) {this->input_case = t;}
	std::vector<LDBLE> & Get_p(void) {return this->p;}
	void Set_p(const std::vector<LDBLE> & t) {this->p = t;}
protected:
	std::string name;
	//cxxNameDouble comps;
	LDBLE total_moles;
	LDBLE dn;
	LDBLE a0, a1;
	LDBLE ag0, ag1;
	bool ss_in;
	bool miscibility;
	bool spinodal;
	LDBLE tk, xb1, xb2;
	SS_PARAMETER_TYPE input_case;
	std::vector<cxxSScomp> ss_comps;
	std::vector<LDBLE> p;
	cxxNameDouble totals;
public:

};

#endif // !defined(SSASSEMBLAGESS_H_INCLUDED)
