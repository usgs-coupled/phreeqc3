#if !defined(EXCHCOMP_H_INCLUDED)
#define EXCHCOMP_H_INCLUDED

#include "NameDouble.h"
#define EXTERNAL extern
#include "global.h"
#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

#include "char_star.h"

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

	char *get_formula() const
	{
		return this->formula;
	}
	void set_formula(char *cstring)
	{
		this->formula = cstring;
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
	char *get_phase_name() const
	{
		return this->phase_name;
	}
	void set_phase_name(char *cstring)
	{
		this->phase_name = cstring;
	}
	double get_phase_proportion() const
	{
		return this->phase_proportion;
	}
	void set_phase_proportion(double d)
	{
		this->phase_proportion = d;
	}
	char *get_rate_name() const
	{
		return this->rate_name;
	}
	void set_rate_name(char *cstring)
	{
		this->rate_name = cstring;
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
	char *formula;
	double moles;
	cxxNameDouble formula_totals;
	cxxNameDouble totals;
	double la;
	double charge_balance;
	char *phase_name;
	double phase_proportion;
	char *rate_name;
	double formula_z;			// charge on formula

  public:

};

#endif // !defined(EXCHCOMP_H_INCLUDED)
