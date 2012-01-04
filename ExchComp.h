#if !defined(EXCHCOMP_H_INCLUDED)
#define EXCHCOMP_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

#include "NameDouble.h"

class cxxExchComp: public PHRQ_base
{

  public:
	cxxExchComp(PHRQ_io *io=NULL);
	//cxxExchComp(struct exch_comp *, PHRQ_io *io=NULL);
	 ~cxxExchComp();

	void dump_xml(std::ostream & os, unsigned int indent = 0) const;

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void read_raw(CParser & parser, bool check=true);

	const std::string &Get_formula() const
	{
		return this->formula;
	}
	void Set_formula(const char *cstring)
	{
		if (cstring != NULL)
			this->formula = std::string(cstring);
		else
			this->formula.clear();
	}
	double Get_moles() const
	{
		return this->moles;
	}
	void Set_moles(double d)
	{
		this->moles = d;
	}
	double Get_la() const
	{
		return this->la;
	}
	void Set_la(double d)
	{
		this->la = d;
	}
	double Get_charge_balance() const
	{
		return this->charge_balance;
	}
	void Set_charge_balance(double d)
	{
		this->charge_balance = d;
	}
	const std::string &Get_phase_name() const
	{
		return this->phase_name;
	}
	void Set_phase_name(const char *cstring)
	{
		if (cstring != NULL)
			this->phase_name = std::string(cstring);
		else
			this->phase_name.clear();
	}
	double Get_phase_proportion() const
	{
		return this->phase_proportion;
	}
	void Set_phase_proportion(double d)
	{
		this->phase_proportion = d;
	}
	const std::string &Get_rate_name() const
	{
		return this->rate_name;
	}
	void Set_rate_name(const char *cstring)
	{
		if (cstring != NULL)
			this->rate_name = std::string(cstring);
		else
			this->rate_name.clear();
	}
	double Get_formula_z() const
	{
		return this->formula_z;
	}
	void Set_formula_z(double d)
	{
		this->formula_z = d;
	}
	void Set_totals(struct elt_list *e_l, int count)
	{
		this->totals = cxxNameDouble(e_l, count);
	}
	void Set_totals(struct elt_list *e_l)
	{
		this->totals = cxxNameDouble(e_l);
	}
	void Set_totals(cxxNameDouble nd)
	{
		this->totals = nd;
	}
	void Set_formula_totals(struct elt_list *e_l, int count)
	{
		this->formula_totals = cxxNameDouble(e_l, count);
	}
	void Set_formula_totals(struct elt_list *e_l)
	{
		this->formula_totals = cxxNameDouble(e_l);
	}
	void Set_formula_totals(cxxNameDouble nd)
	{
		this->formula_totals = nd;
	}

	cxxNameDouble & Get_totals() {return (this->totals);}
	cxxNameDouble & Get_formula_totals(void) {return this->formula_totals;}


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
