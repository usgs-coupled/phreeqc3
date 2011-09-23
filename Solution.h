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
class cxxMix;

class cxxSolution:public cxxNumKeyword
{

  public:
	cxxSolution(PHRQ_io *io=NULL);
	cxxSolution(struct solution *, PHRQ_io *io=NULL);
	cxxSolution(PHREEQC_PTR_ARG_COMMA int n_user, PHRQ_io *io=NULL);
	cxxSolution(PHREEQC_PTR_ARG_COMMA const std::map < int, cxxSolution > &solution_map,
				  cxxMix & mx, int n_user, PHRQ_io *io=NULL);
	~cxxSolution();

	//static cxxSolution& read(CParser& parser);

	double get_tc() const
	{
		return this->tc;
	}
	void set_tc(double l_tc)
	{
		this->tc = l_tc;
	}

	double get_ph() const
	{
		return this->ph;
	}
	void set_ph(double pH)
	{
		this->ph = pH;
	}

	double get_pe() const
	{
		return this->pe;
	}
	void set_pe(double l_pe)
	{
		this->pe = l_pe;
	}

	double get_mu() const
	{
		return this->mu;
	}
	void set_mu(double l_mu)
	{
		this->mu = l_mu;
	}

	double get_ah2o() const
	{
		return this->ah2o;
	}
	void set_ah2o(double l_ah2o)
	{
		this->ah2o = l_ah2o;
	}

	double get_total_h() const
	{
		return this->total_h;
	}
	void set_total_h(double l_total_h)
	{
		this->total_h = l_total_h;
	}

	double get_total_o() const
	{
		return this->total_o;
	}
	void set_total_o(double l_total_o)
	{
		this->total_o = l_total_o;
	}

	double get_cb() const
	{
		return this->cb;
	}
	void set_cb(double l_cb)
	{
		this->cb = l_cb;
	}

	double get_mass_water() const
	{
		return this->mass_water;
	}
	void set_mass_water(long double l_mass_water)
	{
		this->mass_water = l_mass_water;
	}

	double get_total_alkalinity() const
	{
		return this->total_alkalinity;
	}
	void set_total_alkalinity(double l_total_alkalinity)
	{
		this->total_alkalinity = l_total_alkalinity;
	}

	double get_total(char *string) const;
	double get_total_element(char *string) const;
	void set_total(char *string, double value);

	const cxxNameDouble & get_totals(void) const
	{
		return this->totals;
	}
	void set_totals(cxxNameDouble & nd)
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

	const cxxNameDouble & get_master_activity(void) const
	{
		return this->master_activity;
	}
	double get_master_activity(char *string) const;
	void set_master_activity(char *string, double value);

	/*
	   double get_species_gamma(char *string)const;
	   void set_species_gamma(char *string, double value);

	   double get_isotope(char *string)const;
	   void set_isotope(char *string, double value);
	 */

	struct solution *cxxSolution2solution(PHREEQC_PTR_ARG);

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void read_raw(CParser & parser, bool check = true);
	void multiply(double extensive);
	void modify_activities(PHREEQC_PTR_ARG_COMMA const cxxSolution & original);
#ifdef ORCHESTRA
	void ORCH_write(std::ostream & headings, std::ostream & input_data) const;
	void ORCH_read(std::vector < std::pair < std::string,
				   double >>output_vector,
				   std::vector < std::pair < std::string,
				   double >>::iterator & it);
	void ORCH_store_global(std::map < std::string, double >output_map);
#endif

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
