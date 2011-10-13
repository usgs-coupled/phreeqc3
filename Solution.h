#if !defined(SOLUTION_H_INCLUDED)
#define SOLUTION_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <vector>				// std::vector
#include <iostream>
#include "NumKeyword.h"
#include "SolutionIsotopeList.h"
#include "NameDouble.h"
#include "PHRQ_base.h"
#include "PHRQ_io.h"
#include "phreeqc_class.h"
class cxxMix;

class cxxSolution:public cxxNumKeyword
{

  public:
	cxxSolution(PHRQ_io *io=NULL);
	cxxSolution(struct solution *, PHRQ_io *io=NULL);
	cxxSolution( const std::map < int, cxxSolution > &solution_map,
				  cxxMix & mx, int n_user, PHRQ_io *io=NULL);
	~cxxSolution();

	double Get_tc() const
	{
		return this->tc;
	}
	void Set_tc(double l_tc)
	{
		this->tc = l_tc;
	}

	double Get_ph() const
	{
		return this->ph;
	}
	void Set_ph(double pH)
	{
		this->ph = pH;
	}

	double Get_pe() const
	{
		return this->pe;
	}
	void Set_pe(double l_pe)
	{
		this->pe = l_pe;
	}

	double Get_mu() const
	{
		return this->mu;
	}
	void Set_mu(double l_mu)
	{
		this->mu = l_mu;
	}

	double Get_ah2o() const
	{
		return this->ah2o;
	}
	void Set_ah2o(double l_ah2o)
	{
		this->ah2o = l_ah2o;
	}

	double Get_total_h() const
	{
		return this->total_h;
	}
	void Set_total_h(double l_total_h)
	{
		this->total_h = l_total_h;
	}

	double Get_total_o() const
	{
		return this->total_o;
	}
	void Set_total_o(double l_total_o)
	{
		this->total_o = l_total_o;
	}

	double Get_cb() const
	{
		return this->cb;
	}
	void Set_cb(double l_cb)
	{
		this->cb = l_cb;
	}

	double Get_mass_water() const
	{
		return this->mass_water;
	}
	void Set_mass_water(long double l_mass_water)
	{
		this->mass_water = l_mass_water;
	}

	double Get_total_alkalinity() const
	{
		return this->total_alkalinity;
	}
	void Set_total_alkalinity(double l_total_alkalinity)
	{
		this->total_alkalinity = l_total_alkalinity;
	}

	double Get_total(char *string) const;
	double Get_total_element(const char *string) const;
	void Set_total(char *string, double value);

	const cxxNameDouble & Get_totals(void) const
	{
		return this->totals;
	}
	void Set_totals(cxxNameDouble & nd)
	{
		this->totals = nd;
		this->totals.type = cxxNameDouble::ND_ELT_MOLES;
	}
	void clear_totals()
	{
		this->totals.clear();
	}
	void clear_master_activity()
	{
		this->master_activity.clear();
	}

	const cxxNameDouble & Get_master_activity(void) const
	{
		return this->master_activity;
	}
	const cxxNameDouble & Get_species_gamma(void) const
	{
		return this->species_gamma;
	}
	double Get_master_activity(char *string) const;
	void Set_master_activity(char *string, double value);
	const cxxSolutionIsotopeList & Get_isotopes(void) const {return isotopes;};
	void dump_raw(std::ostream & s_oss, unsigned int indent, int *n_out=NULL) const;

	void read_raw(CParser & parser, bool check = true);
	void multiply(double extensive);
	//void modify_activities(PHREEQC_PTR_ARG_COMMA const cxxSolution & original);
	void modify_activities(const cxxSolution & original);
	void Simplify_totals();
	void Update(const cxxNameDouble &nd);

#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < double >&doubles);
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
	void mpi_send(int task_number);
	void mpi_recv(int task_number);
#endif

  private:
	void zero();
	void add(const cxxSolution & addee, double extensive);
	// not checked
	void dump_xml(std::ostream & os, unsigned int indent = 0) const;

  protected:
	double tc;
	double ph;
	double pe;
	double mu;
	double ah2o;
	double total_h;
	double total_o;
	double cb;
	double mass_water;
	double total_alkalinity;
	cxxNameDouble totals;
	//std::list<cxxSolutionIsotope> isotopes;
	cxxNameDouble master_activity;
	cxxNameDouble species_gamma;
	cxxSolutionIsotopeList isotopes;

};

#endif // !defined(SOLUTION_H_INCLUDED)
