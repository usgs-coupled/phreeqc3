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


	struct master *Get_psi_master();
	
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
	void add(const cxxSurfaceCharge & comp, LDBLE extensive);
	void multiply(LDBLE extensive);

#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < LDBLE >&doubles);
	void mpi_unpack(int *ints, int *ii, LDBLE *doubles, int *dd);
#endif
	LDBLE Get_specific_area() const {return this->specific_area;};
	LDBLE Get_grams() const {return this->grams;};
	LDBLE Get_charge_balance() const {return this->charge_balance;};
	LDBLE Get_mass_water() const {return this->mass_water;};
	LDBLE Get_la_psi() const {return this->la_psi;};
	LDBLE Get_la_psi1() const {return this->la_psi1;};
	LDBLE Get_la_psi2() const {return this->la_psi2;};
	LDBLE Get_capacitance0() const {return this->capacitance[0];};
	LDBLE Get_capacitance1() const {return this->capacitance[1];};
	const cxxNameDouble & Get_diffuse_layer_totals(void) const {return this->diffuse_layer_totals;};
protected:
	std::string name;
	LDBLE specific_area;
	LDBLE grams;
	LDBLE charge_balance;
	LDBLE mass_water;
	LDBLE la_psi, la_psi1, la_psi2;
	LDBLE capacitance[2];
	//std::map<LDBLE, cxxSurfDL> g;
	//char * psi_master_name;
	cxxNameDouble diffuse_layer_totals;

public:

};

#endif // !defined(SURFACECHARGE_H_INCLUDED)
