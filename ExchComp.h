#if !defined(EXCHCOMP_H_INCLUDED)
#define EXCHCOMP_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

#include "NameDouble.h"

class cxxExchComp
{

  public:
	cxxExchComp();
	cxxExchComp(struct exch_comp *);
	 ~cxxExchComp();

	struct master *get_master();

	static struct exch_comp *cxxExchComp2exch_comp(std::map < std::string, cxxExchComp >
												   &el);

	void dump_xml(std::ostream & os, unsigned int indent = 0) const;

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void read_raw(CParser & parser, bool check=true);

	const std::string &get_formula() const
	{
		return this->formula;
	}
	void set_formula(char *cstring)
	{
		if (cstring != NULL)
			this->formula = std::string(cstring);
		else
			this->formula.clear();
	}
	double get_moles() const
	{
		return this->moles;
	}
	void set_moles(double d)
	{
		this->moles = d;
	}
	double get_la() const
	{
		return this->la;
	}
	void set_la(double d)
	{
		this->la = d;
	}
	double get_charge_balance() const
	{
		return this->charge_balance;
	}
	void set_charge_balance(double d)
	{
		this->charge_balance = d;
	}
	const std::string &get_phase_name() const
	{
		return this->phase_name;
	}
	void set_phase_name(char *cstring)
	{
		if (cstring != NULL)
			this->phase_name = std::string(cstring);
		else
			this->phase_name.clear();
	}
	double get_phase_proportion() const
	{
		return this->phase_proportion;
	}
	void set_phase_proportion(double d)
	{
		this->phase_proportion = d;
	}
	const std::string &get_rate_name() const
	{
		return this->rate_name;
	}
	void set_rate_name(char *cstring)
	{
		if (cstring != NULL)
			this->rate_name = std::string(cstring);
		else
			this->rate_name.clear();
	}
	double get_formula_z() const
	{
		return this->formula_z;
	}
	void set_formula_z(double d)
	{
		this->formula_z = d;
	}
	void set_totals(struct elt_list *e_l, int count)
	{
		this->totals = cxxNameDouble(e_l, count);
	}
	void set_totals(struct elt_list *e_l)
	{
		this->totals = cxxNameDouble(e_l);
	}
	void set_totals(cxxNameDouble nd)
	{
		this->totals = nd;
	}
	void set_formula_totals(struct elt_list *e_l, int count)
	{
		this->formula_totals = cxxNameDouble(e_l, count);
	}
	void set_formula_totals(struct elt_list *e_l)
	{
		this->formula_totals = cxxNameDouble(e_l);
	}
	void set_formula_totals(cxxNameDouble nd)
	{
		this->formula_totals = nd;
	}

	const cxxNameDouble & get_totals() const
	{
		return (this->totals);
	};


	void add(const cxxExchComp & comp, double extensive);
	void multiply(double extensive);


#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < double >&doubles);
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
#endif
  protected:
	std::string formula;
	double moles;
	cxxNameDouble formula_totals;
	cxxNameDouble totals;
	double la;
	double charge_balance;
	std::string phase_name;
	double phase_proportion;
	std::string rate_name;
	double formula_z;			// charge on formula

  public:

};

#endif // !defined(EXCHCOMP_H_INCLUDED)
