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

class cxxSurfaceComp
{

  public:

	cxxSurfaceComp();
	cxxSurfaceComp(struct surface_comp *);
	 ~cxxSurfaceComp();


	struct master *get_master();
	const std::string &get_phase_name() const
	{
		return this->phase_name;
	}
	void set_phase_name(char * f) 
	{
		if (f != NULL)
			this->phase_name = std::string(f);
		else
			this->phase_name.clear();
	}
	const std::string &get_rate_name() const
	{
		return this->rate_name;
	}
	void set_rate_name(char * f) 
	{
		if (f != NULL)
			this->rate_name = std::string(f);
		else
			this->rate_name.clear();
	}
	const std::string &get_formula() const
	{
		return this->formula;
	}
	void set_formula(char * f) 
	{
		if (f != NULL)
			this->formula = std::string(f);
		else
			this->formula.clear();
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

	std::string charge_name()
	{
		char * str = string_hsave(this->formula.c_str());
		return (get_charge_name(str));
	};

	//static std::string &get_charge_name(char *token)
	//{
	//	char name[100];
	//	int l;
	//	char *ptr1 = token;
	//	get_elt(&ptr1, name, &l);
	//	ptr1 = strchr(name, '_');
	//	if (ptr1 != NULL)
	//	{
	//		ptr1[0] = '\0';
	//	}
	//	return (string_hsave(name));
	//};
	static std::string get_charge_name(char *token)
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
		return (std::string(name));
	};
#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < double >&doubles);
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
#endif

  protected:
	  std::string formula;
	  cxxNameDouble formula_totals;
	  double formula_z;
	  double moles;
	  cxxNameDouble totals;
	  double la;
	  //int charge_number;
	  double charge_balance;
	  std::string phase_name;
	  double phase_proportion;
	  std::string rate_name;
	  double Dw;
  public:

};

#endif // !defined(SURFACECOMP_H_INCLUDED)
