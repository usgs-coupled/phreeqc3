#if !defined(SURFACE_H_INCLUDED)
#define SURFACE_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

#include "NumKeyword.h"
#include "SurfaceComp.h"
#include "SurfaceCharge.h"
class cxxMix;

class cxxSurface:public cxxNumKeyword
{

  public:
	cxxSurface(PHRQ_io *io=NULL);
	cxxSurface(struct surface *, PHRQ_io *io=NULL);
	cxxSurface(const std::map < int, cxxSurface > &entity_map, cxxMix & mx,
				 int n_user, PHRQ_io *io=NULL);
	~cxxSurface();

	void dump_xml(std::ostream & os, unsigned int indent = 0) const;

	void dump_raw(std::ostream & s_oss, unsigned int indent, int *n_out=NULL) const;

	void read_raw(CParser & parser, bool check = true);

	bool Get_related_phases(void) const;

	bool Get_related_rate(void) const;

	void totalize();

	const cxxNameDouble & Get_totals() const
	{
		return this->totals;
	};

#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < LDBLE >&doubles);
	void mpi_unpack(int *ints, int *ii, LDBLE *doubles, int *dd);
#endif

	void add(const cxxSurface & addee, LDBLE extensive);
	const std::map < std::string, cxxSurfaceComp > & Get_surfaceComps() const {return this->surfaceComps;};
	const std::map < std::string, cxxSurfaceCharge > & Get_surfaceCharges() const {return this->surfaceCharges;};
	SURFACE_TYPE Get_type(void) const {return this->type;};
	DIFFUSE_LAYER_TYPE Get_dl_type(void) const {return dl_type;};
	SITES_UNITS Get_sites_units(void) const {return sites_units;};
	bool Get_only_counter_ions(void) const {return only_counter_ions;};
	LDBLE Get_thickness(void) const {return thickness;};
	LDBLE Get_debye_lengths(void) const {return debye_lengths;};
	LDBLE Get_DDL_viscosity(void) const {return DDL_viscosity;};
	LDBLE Get_DDL_limit(void) const {return DDL_limit;};
	bool Get_transport(void) const {return transport;};

protected:
	std::map < std::string, cxxSurfaceComp > surfaceComps;
	std::map < std::string, cxxSurfaceCharge > surfaceCharges;
	enum SURFACE_TYPE type;
	enum DIFFUSE_LAYER_TYPE dl_type;
	enum SITES_UNITS sites_units;
	bool only_counter_ions;
	LDBLE thickness;
	LDBLE debye_lengths;
	LDBLE DDL_viscosity;
	LDBLE DDL_limit;
	bool transport;
	cxxNameDouble totals;

};

#endif // !defined(SURFACE_H_INCLUDED)
