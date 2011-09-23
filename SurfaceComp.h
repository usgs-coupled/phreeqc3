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

	struct master *get_master(PHREEQC_PTR_ARG);
	const std::string &get_phase_name() const;
	void set_phase_name(char * f);
	const std::string &get_rate_name() const;
	void set_rate_name(char * f);
	const std::string &get_formula() const;
	void set_formula(char * f);
	double get_charge_balance() const;
	void set_charge_balance(double d);
	static struct surface_comp *cxxSurfaceComp2surface_comp(PHREEQC_PTR_ARG_COMMA std::map < std::string, cxxSurfaceComp > &el);
	void dump_xml(std::ostream & os, unsigned int indent = 0) const;
	void dump_raw(std::ostream & s_oss, unsigned int indent) const;
	void read_raw(CParser & parser, bool check = true);
	const cxxNameDouble & get_totals() const;
	void add(PHREEQC_PTR_ARG_COMMA const cxxSurfaceComp & comp, double extensive);
	void multiply(double extensive);
	std::string charge_name(PHREEQC_PTR_ARG);
	static std::string get_charge_name(PHREEQC_PTR_ARG_COMMA char *token);
#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < double >&doubles);
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
#endif

protected:
	std::string formula;
	cxxNameDouble formula_totals;
	double formula_z;
	double moles;
	cxxNameDouble totals;
	double la;
	//int charge_number;
	double charge_balance;
	std::string phase_name;
	double phase_proportion;
	std::string rate_name;
	double Dw;
public:

};

#endif // !defined(SURFACECOMP_H_INCLUDED)
