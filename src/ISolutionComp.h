#if !defined(ISOLUTIONCOMP_H_INCLUDED)
#define ISOLUTIONCOMP_H_INCLUDED

#include <string>
#include <map>					// std::map
#include <vector>
#include <set>
#include "Phreeqc_class.h"
// forward declarations
class cxxISolution;				// reqd for read and dump_xml

class cxxISolutionComp
{
  public:
	cxxISolutionComp(void);
	  cxxISolutionComp(struct conc *conc_ptr);
	 ~cxxISolutionComp(void);

  public:

	//STATUS_TYPE read(CParser& parser, CSolution& sol);

	void dump_xml(std::ostream & os, unsigned int indent = 0) const;

	const std::string &get_description() const
	{
		return this->description;
	}
	void set_description(char *description1)
	{
		if (description1 != NULL)
			this->description = std::string(description1);
		else
			this->description.clear();
	}

	double get_moles() const
	{
		return this->moles;
	}
	void set_moles(double moles1)
	{
		this->moles = moles1;
	}

	double get_input_conc() const
	{
		return this->input_conc;
	}
	void set_input_conc(double input_conc1)
	{
		this->input_conc = input_conc1;
	}

	std::string get_units()const
	{
		return this->units;
	}
	void set_units(char *units1)
	{
		if (units1 != NULL)
			this->units = std::string(units1);
		else
			this->units.clear();
	}

	const std::string &get_equation_name() const
	{
		return this->equation_name;
	}
	void set_equation_name(char *equation_name1)
	{
		if (equation_name1 != NULL)
			this->equation_name = std::string(equation_name1);
		else
			this->equation_name.clear();

	}

	double get_phase_si() const
	{
		return this->phase_si;
	}
	void set_phase_si(int phase_si1)
	{
		this->phase_si = phase_si1;
	}

	int get_n_pe() const
	{
		return this->n_pe;
	}
	void set_n_pe(int n_pe1)
	{
		this->n_pe = n_pe1;
	}

	const std::string &get_as() const
	{
		return this->as;
	}
	void set_as(char *as1)
	{
		if (as1 != NULL)
			this->as = std::string(as1);
		else
			this->as.clear();
	}

	//double get_gfw()const {return this->gfw;}
	double get_gfw() const
	{
		return this->gfw;
	};
	void set_gfw(double gfw1)
	{
		this->gfw = gfw1;
	}
	void set_gfw(PHREEQC_PTR_ARG);

	bool operator<(const cxxISolutionComp & conc) const
	{
		return ::strcmp(this->description.c_str(), conc.description.c_str()) < 0;
	}

	static struct conc *cxxISolutionComp2conc(PHREEQC_PTR_ARG_COMMA const std::map < std::string,	cxxISolutionComp > &t);

  private:
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
