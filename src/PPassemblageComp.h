#if !defined(PPASSEMBLAGECOMP_H_INCLUDED)
#define PPASSEMBLAGECOMP_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

#include "NameDouble.h"
#include "Phreeqc_class.h"

class cxxPPassemblageComp: public PHRQ_base
{

  public:
	cxxPPassemblageComp(PHRQ_io *io=NULL);
	cxxPPassemblageComp(struct pure_phase *, PHRQ_io *io=NULL);
	 ~cxxPPassemblageComp();

	void dump_xml(std::ostream & os, unsigned int indent = 0) const;
	void dump_raw(std::ostream & s_oss, unsigned int indent) const;
	void read_raw(CParser & parser, bool check = true);
	const std::string &Get_name() const
	{
		return this->name;
	}
	void Set_name(char * s)
	{
		if(s != NULL)
			this->name = std::string(s);
		else
			this->name.clear();
	}
	const std::string &Get_add_formula() const
	{
		return this->add_formula;
	}
	void Set_add_formula(char * s)
	{
		if(s != NULL)
			this->add_formula = std::string(s);
		else
			this->add_formula.clear();
	}

	void totalize(PHREEQC_PTR_ARG);
	const cxxNameDouble & Get_totals() const
	{
		return (this->totals);
	};
	double Get_si() const {return this->si;};
	double Get_moles() const {return this->moles;};
	double Get_delta() const {return this->delta;};
	double Get_initial_moles() const {return this->initial_moles;};
	bool Get_force_equality() const {return this->force_equality;};
	bool Get_dissolve_only() const {return this->dissolve_only;};
	bool Get_precipitate_only() const {return this->precipitate_only;};


	void add(const cxxPPassemblageComp & comp, double extensive);
	void multiply(double extensive);

#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < double >&doubles);
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
#endif
protected:
	std::string name;
	std::string add_formula;
	double si;
	double moles;
	double delta;
	double initial_moles;
	bool force_equality;
	bool dissolve_only;
	bool precipitate_only;
	cxxNameDouble totals;

public:

};

#endif // !defined(PPASSEMBLAGECOMP_H_INCLUDED)
