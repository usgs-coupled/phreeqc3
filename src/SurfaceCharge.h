#if !defined(SURFACECHARGE_H_INCLUDED)
#define SURFACECHARGE_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

#include "NameDouble.h"
#include "Phreeqc_class.h"
#include "PHRQ_base.h"

class cxxSurfaceCharge: public PHRQ_base
{

public:

	cxxSurfaceCharge(PHRQ_io *io=NULL);
	cxxSurfaceCharge(struct surface_charge *, PHRQ_io *io=NULL);
	~cxxSurfaceCharge();


	struct master *get_psi_master();
	
	void dump_xml(std::ostream & os, unsigned int indent = 0) const;

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void read_raw(CParser & parser, bool check = true);

	const std::string &Get_name() const	{return this->name;};
	void Set_name(const char * s)
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
	double Get_specific_area() const {return this->specific_area;};
	double Get_grams() const {return this->grams;};
	double Get_charge_balance() const {return this->charge_balance;};
	double Get_mass_water() const {return this->mass_water;};
	double Get_la_psi() const {return this->la_psi;};
	double Get_la_psi1() const {return this->la_psi1;};
	double Get_la_psi2() const {return this->la_psi2;};
	double Get_capacitance0() const {return this->capacitance[0];};
	double Get_capacitance1() const {return this->capacitance[1];};
	const cxxNameDouble & Get_diffuse_layer_totals(void) const {return this->diffuse_layer_totals;};
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
