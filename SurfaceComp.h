#if !defined(SURFACECOMP_H_INCLUDED)
#define SURFACECOMP_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector
#include "Phreeqc_class.h"
#include "NameDouble.h"

class cxxSurfaceComp: public PHRQ_base
{

public:

	cxxSurfaceComp(PHRQ_io *io=NULL);
	cxxSurfaceComp(struct surface_comp *, PHRQ_io *io=NULL);
	~cxxSurfaceComp();

	const std::string &Get_phase_name() const;
	void Set_phase_name(const char * f);
	const std::string &Get_rate_name() const;
	void Set_rate_name(const char * f);
	const std::string &Get_formula() const;
	void Set_formula(const char * f);
	LDBLE Get_charge_balance() const;
	void Set_charge_balance(LDBLE d);
	void dump_xml(std::ostream & os, unsigned int indent = 0) const;
	void dump_raw(std::ostream & s_oss, unsigned int indent) const;
	void read_raw(CParser & parser, bool check = true);
	const cxxNameDouble & Get_totals() const;
	const cxxNameDouble & Get_formula_totals() const;
	LDBLE Get_formula_z(void) const {return formula_z;};
	LDBLE Get_moles(void) const {return moles;};
	LDBLE Get_la(void) const {return la;};
	LDBLE Get_phase_proportion(void) const {return phase_proportion;};
	LDBLE Get_Dw(void) const {return Dw;};
	void add(const cxxSurfaceComp & comp, LDBLE extensive);
	void multiply(LDBLE extensive);

#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < LDBLE >&doubles);
	void mpi_unpack(int *ints, int *ii, LDBLE *doubles, int *dd);
#endif

protected:
	std::string formula;
	cxxNameDouble formula_totals;
	LDBLE formula_z;
	LDBLE moles;
	cxxNameDouble totals;
	LDBLE la;
	//int charge_number;
	LDBLE charge_balance;
	std::string phase_name;
	LDBLE phase_proportion;
	std::string rate_name;
	LDBLE Dw;
public:

};

#endif // !defined(SURFACECOMP_H_INCLUDED)
