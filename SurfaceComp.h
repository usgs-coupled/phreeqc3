#if !defined(SURFACECOMP_H_INCLUDED)
#define SURFACECOMP_H_INCLUDED

#include "NameDouble.h"
#define EXTERNAL extern
#include "global.h"
#include "phrqproto.h"
#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

#include "char_star.h"

class cxxSurfaceComp
{

  public:

	cxxSurfaceComp();
	cxxSurfaceComp(struct surface_comp *);
	 ~cxxSurfaceComp();


	struct master *get_master();
	char *get_phase_name() const
	{
		return this->phase_name;
	}
	char *get_rate_name() const
	{
		return this->rate_name;
	}
	char *get_formula() const
	{
		return this->formula;
	}
	double get_charge_balance() const
	{
		return this->charge_balance;
	}
	void set_charge_balance(double d)
	{
		this->charge_balance = d;
	}

	static struct surface_comp *cxxSurfaceComp2surface_comp(std::map < std::string, cxxSurfaceComp > &el);

	void dump_xml(std::ostream & os, unsigned int indent = 0) const;

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void read_raw(CParser & parser, bool check = true);
	const cxxNameDouble & get_totals() const
	{
		return (this->totals);
	};


	void add(const cxxSurfaceComp & comp, double extensive);
	void multiply(double extensive);

	char *charge_name()
	{
		return (get_charge_name(this->formula));
	};

	static char *get_charge_name(char *token)
	{
		char name[100];
		int l;
		char *ptr1 = token;
		get_elt(&ptr1, name, &l);
		ptr1 = strchr(name, '_');
		if (ptr1 != NULL)
		{
			ptr1[0] = '\0';
		}
		return (string_hsave(name));
	};

#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < double >&doubles);
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
#endif

  protected:
	char *formula;
	cxxNameDouble formula_totals;
	double formula_z;
	double moles;
	cxxNameDouble totals;
	double la;
	//int charge_number;
	double charge_balance;
	char *phase_name;
	double phase_proportion;
	char *rate_name;
	double Dw;
  public:

};

#endif // !defined(SURFACECOMP_H_INCLUDED)
