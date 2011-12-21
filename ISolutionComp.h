#if !defined(ISOLUTIONCOMP_H_INCLUDED)
#define ISOLUTIONCOMP_H_INCLUDED

#include <string>
#include <map>					// std::map
#include <vector>
#include <set>
#include "PHRQ_base.h"
// forward declarations
class cxxISolution;				// reqd for read and dump_xml

class cxxISolutionComp: public PHRQ_base
{
  public:
	cxxISolutionComp(PHRQ_io *io=NULL);
	cxxISolutionComp(struct conc *conc_ptr, PHRQ_io *io=NULL);
	~cxxISolutionComp(void);

  public:

	//STATUS_TYPE read(CParser& parser, CSolution& sol);

	void dump_xml(std::ostream & os, unsigned int indent = 0) const;

	const std::string &Get_description() const
	{
		return this->description;
	}
	void Set_description(const char *l_description)
	{
		if (l_description != NULL)
			this->description = std::string(l_description);
		else
			this->description.clear();
	}

	double Get_moles() const
	{
		return this->moles;
	}
	void Set_moles(double l_moles)
	{
		this->moles = l_moles;
	}

	double Get_input_conc() const
	{
		return this->input_conc;
	}
	void Set_input_conc(double l_input_conc)
	{
		this->input_conc = l_input_conc;
	}

	std::string Get_units()const
	{
		return this->units;
	}
	void Set_units(const char *l_units)
	{
		if (l_units != NULL)
			this->units = std::string(l_units);
		else
			this->units.clear();
	}

	const std::string &Get_equation_name() const
	{
		return this->equation_name;
	}
	void Set_equation_name(const char *l_equation_name)
	{
		if (l_equation_name != NULL)
			this->equation_name = std::string(l_equation_name);
		else
			this->equation_name.clear();

	}

	double Get_phase_si() const
	{
		return this->phase_si;
	}
	void Set_phase_si(int l_phase_si)
	{
		this->phase_si = l_phase_si;
	}

	int Get_n_pe() const
	{
		return this->n_pe;
	}
	void Set_n_pe(int l_n_pe)
	{
		this->n_pe = l_n_pe;
	}

	const std::string &Get_as() const
	{
		return this->as;
	}
	void Set_as(const char *l_as)
	{
		if (l_as != NULL)
			this->as = std::string(l_as);
		else
			this->as.clear();
	}

	//double get_gfw()const {return this->gfw;}
	double Get_gfw() const
	{
		return this->gfw;
	};
	void Set_gfw(double l_gfw)
	{
		this->gfw = l_gfw;
	}
	//void Set_gfw(PHREEQC_PTR_ARG);

	bool operator<(const cxxISolutionComp & conc) const
	{
		return ::strcmp(this->description.c_str(), conc.description.c_str()) < 0;
	}

  protected:
	  std::string description;
	  double moles;
	  double input_conc;
	  std::string units;
	  std::string equation_name;
	  double phase_si;
	  int n_pe;
	  std::string as;
	  double gfw;
};

#endif // ISOLUTIONCOMP_H_INCLUDED
