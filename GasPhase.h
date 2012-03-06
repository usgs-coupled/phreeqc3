#if !defined(GASPHASE_H_INCLUDED)
#define GASPHASE_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector
#include "phrqtype.h"
#include "NumKeyword.h"
#include "NameDouble.h"
#include "Phreeqc_class.h"
#include "GasComp.h"
class cxxMix;

class cxxGasPhase:public cxxNumKeyword
{

  public:
	cxxGasPhase(PHRQ_io * io=NULL);
	cxxGasPhase(std::map < int, cxxGasPhase > &entity_map,
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
	void mpi_pack(std::vector < int >&ints, std::vector < LDBLE >&doubles);
	void mpi_unpack(int *ints, int *ii, LDBLE *doubles, int *dd);
#endif

	void totalize(PHREEQC_PTR_ARG);

	const cxxNameDouble & Get_totals() const
	{
		return this->totals;
	};
	std::vector<cxxGasComp> & Get_gas_comps(void) {return gas_comps;};
	void Set_gas_comps(const std::vector<cxxGasComp> v) {gas_comps = v;};

	GP_TYPE Get_type(void) const {return type;};
	void Set_type(GP_TYPE t) {type = t;};
	LDBLE Get_total_p(void) const {return total_p;};
	LDBLE Get_volume(void) const {return volume;};
	void Set_volume(LDBLE v) {volume = v;};
	LDBLE Get_v_m(void) const {return v_m;};
	void Set_v_m(LDBLE v) {v_m = v;};
	bool Get_pr_in(void) const {return pr_in;};
	void Set_pr_in(bool tf) {pr_in = tf;};
	cxxNameDouble & Get_totals(void) {return totals;};

	bool Get_new_def(void) const {return this->new_def;};
	void Set_new_def(bool tf) {this->new_def = tf;};
	bool Get_solution_equilibria(void) const {return this->solution_equilibria;};
	void Set_solution_equilibria(bool tf) {this->solution_equilibria = tf;};
	int Get_n_solution(void) const {return this->n_solution;};
	void Set_n_solution(int i) {this->n_solution = i;};
	LDBLE Get_total_moles(void) {return (LDBLE) total_moles;};
	void Set_total_moles(LDBLE t) {total_moles = t;};
	LDBLE Get_total_p(void) {return (LDBLE) total_p;};
	void Set_total_p(LDBLE t) {total_p = t;};
	LDBLE Get_temperature(void) {return (LDBLE) temperature;};
	void Set_temperature(LDBLE t) {temperature = t;};
	LDBLE Calc_total_moles(void);
	cxxGasComp *Find_comp(const char * comp_name);

protected:
	void add(const cxxGasPhase & addee, LDBLE extensive);

protected:
	// candidate variables for GAS_PHASE_MODIFY
	GP_TYPE type;
	LDBLE total_p;
	LDBLE volume;
	std::vector<cxxGasComp> gas_comps;

	// GAS_PHASE_MODIFY with new_def=true variables
	bool new_def;
	bool solution_equilibria;
	int n_solution;
	LDBLE temperature;

	// internal variables
	LDBLE total_moles;
	LDBLE v_m;
	bool pr_in;
	cxxNameDouble totals;
};

#endif // !defined(GASPHASE_H_INCLUDED)
