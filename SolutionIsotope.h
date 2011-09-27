#if !defined(SOLUTIONISOTOPE_H_INCLUDED)
#define SOLUTIONISOTOPE_H_INCLUDED

#include <ostream>				// std::ostream
#include <string>				// std::string
#include <list>					// std::list
#include "Parser.h"
#include "Phreeqc_class.h"

class cxxSolutionIsotope: public PHRQ_base
{
  public:
	cxxSolutionIsotope(PHRQ_io *io=NULL);
	cxxSolutionIsotope(struct isotope *isotope_ptr, PHRQ_io *io=NULL);
	~cxxSolutionIsotope(void);

	/*
	   enum STATUS {
	   ERROR = 0,
	   OK    = 1
	   };
	 */
	//cxxSolutionIsotope::STATUS read(CParser& parser);
	//static struct isotope *list2isotope(PHREEQC_PTR_ARG_COMMA std::list < cxxSolutionIsotope > &t);

	void dump_xml(std::ostream & os, unsigned int indent) const;
	void dump_raw(std::ostream & os, unsigned int indent) const;

	CParser::STATUS_TYPE read_raw(CParser & parser, std::istream::pos_type next_char);

	double get_isotope_number() const
	{
		return this->isotope_number;
	}
	void set_isotope_number(double d)
	{
		this->isotope_number = d;
	}
	const std::string &get_elt_name() const
	{
		return this->elt_name;
	}
	void set_elt_name(const char *cstring)
	{
		if (cstring != NULL)
			this->elt_name = std::string(cstring);
		else
			this->elt_name.clear();
	}
	const std::string &get_isotope_name() const
	{
		return this->isotope_name;
	}
	void set_isotope_name(const char *cstring)
	{
		if (cstring != NULL)
			this->isotope_name = std::string(cstring);
		else
			this->isotope_name.clear();
	}
	double get_total() const
	{
		return this->total;
	}
	void set_total(double d)
	{
		this->total = d;
	}

	double get_ratio() const
	{
		return this->ratio;
	}

	double get_ratio_uncertainty() const
	{
		return this->ratio_uncertainty;
	}

	bool get_ratio_uncertainty_defined() const
	{
		return this->ratio_uncertainty_defined;
	}

	bool operator<(const cxxSolutionIsotope & conc) const;

	//struct master *master(PHREEQC_PTR_ARG);
	//struct master *primary(PHREEQC_PTR_ARG);

	void add(const cxxSolutionIsotope & isotope_ptr, double intensive,
			 double extensive);
	void multiply(double extensive);

  protected:
	friend class cxxSolutionIsotopeList;
	double isotope_number;
	std::string elt_name;
	std::string isotope_name;
	double total;
	double ratio;
	double ratio_uncertainty;
	bool ratio_uncertainty_defined;
};
#endif // SOLUTIONISOTOPE_H_INCLUDED
