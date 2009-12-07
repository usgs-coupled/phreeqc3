#if !defined(SURFACECHARGE_H_INCLUDED)
#define SURFACECHARGE_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

#include "NameDouble.h"

class cxxSurfaceCharge
{

public:

	cxxSurfaceCharge();
	cxxSurfaceCharge(struct surface_charge *);
	~cxxSurfaceCharge();


	struct master *get_psi_master();

	static struct surface_charge *cxxSurfaceCharge2surface_charge(std::map < std::string, cxxSurfaceCharge > &el);

	void dump_xml(std::ostream & os, unsigned int indent = 0) const;

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void read_raw(CParser & parser, bool check = true);

	const std::string &get_name() const
	{
		return this->name;
	}

	void set_name(const char * s)
	{
		if (s != NULL)
			this->name = std::string(s);
		else
			this->name.clear();

	}
	void add(const cxxSurfaceCharge & comp, double extensive);
	void multiply(double extensive);

#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < double >&doubles);
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
#endif

protected:
	std::string name;
	double specific_area;
	double grams;
	double charge_balance;
	double mass_water;
	double la_psi, la_psi1, la_psi2;
	double capacitance[2];
	//std::map<double, cxxSurfDL> g;
	//char * psi_master_name;
	cxxNameDouble diffuse_layer_totals;

public:

};

#endif // !defined(SURFACECHARGE_H_INCLUDED)
