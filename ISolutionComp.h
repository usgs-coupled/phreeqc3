#if !defined(ISOLUTIONCOMP_H_INCLUDED)
#define ISOLUTIONCOMP_H_INCLUDED

//#include "Parser.h"
#include "Utils.h"
#define EXTERNAL extern
#include "global.h"
#include "phqalloc.h"
#include "phrqproto.h"

#include <string>
#include <map>					// std::map
#include <vector>
#include <set>
#include "char_star.h"

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

	char *get_description() const
	{
		return this->description;
	}
	void set_description(char *description)
	{
		this->description = description;
	}

	double get_moles() const
	{
		return this->moles;
	}
	void set_moles(double moles)
	{
		this->moles = moles;
	}

	double get_input_conc() const
	{
		return this->input_conc;
	}
	void set_input_conc(double input_conc)
	{
		this->input_conc = input_conc;
	}

	std::string get_units()const
	{
		return this->units;
	}
	void set_units(char *units)
	{
		this->units = units;
	}

	char *get_equation_name() const
	{
		return this->equation_name;
	}
	void set_equation_name(char *equation_name)
	{
		this->equation_name = equation_name;
	}

	double get_phase_si() const
	{
		return this->phase_si;
	}
	void set_phase_si(int phase_si)
	{
		this->phase_si = phase_si;
	}

	int get_n_pe() const
	{
		return this->n_pe;
	}
	void set_n_pe(int n_pe)
	{
		this->n_pe = n_pe;
	}

	char *get_as() const
	{
		return this->as;
	}
	void set_as(char *as)
	{
		this->as = as;
	}

	//double get_gfw()const {return this->gfw;}
	double get_gfw() const
	{
		return this->gfw;
	};
	void set_gfw(double gfw)
	{
		this->gfw = gfw;
	}
	void set_gfw();

	bool operator<(const cxxISolutionComp & conc) const
	{
		return::strcmp(this->description, conc.description) < 0;
	}

	static struct conc *cxxISolutionComp2conc(const std::map < char *,
											  cxxISolutionComp,
											  CHARSTAR_LESS > &t);

  private:
	char *description;
	double moles;
	double input_conc;
	char *units;
	char *equation_name;
	double phase_si;
	int n_pe;
	char *as;
	double gfw;
};

#endif // ISOLUTIONCOMP_H_INCLUDED
