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

	//enum SURFACE_STYPE { UNKNOWN_DL, NO_EDL, DDL, CD_MUSIC };
	//enum SURFACE_DL_TYPE { NO_DL, BORKOVEC_DL, DONNAN_DL } ;
	//enum SURFACE_SITES_UNITS { SITES_ABSOLUTE, SITES_DENSITY } ;

	//struct surface *cxxSurface2surface(PHREEQC_PTR_ARG);

	//struct surf_comp *cxxSurfaceComp2surf_comp(PHREEQC_PTR_ARG);

	void dump_xml(std::ostream & os, unsigned int indent = 0) const;

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void read_raw(CParser & parser, bool check = true);

	bool Get_related_phases(void);

	bool Get_related_rate(void);

	void totalize();

	const cxxNameDouble & Get_totals() const
	{
		return this->totals;
	};

#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < double >&doubles);
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
#endif

	void add(const cxxSurface & addee, double extensive);
	const std::map < std::string, cxxSurfaceComp > & Get_surfaceComps() const {return this->surfaceComps;};
	const std::map < std::string, cxxSurfaceCharge > & Get_surfaceCharges() const {return this->surfaceCharges;};
	SURFACE_TYPE Get_type(void) {return this->type;};
	DIFFUSE_LAYER_TYPE Get_dl_type(void) {return dl_type;};
	SITES_UNITS Get_sites_units(void) {return sites_units;};
	bool Get_only_counter_ions(void) {return only_counter_ions;};
	double Get_thickness(void) {return thickness;};
	double Get_debye_lengths(void) {return debye_lengths;};
	double Get_DDL_viscosity(void) {return DDL_viscosity;};
	double Get_DDL_limit(void) const {return DDL_limit;};
	bool Get_transport(void) {return transport;};

protected:
	std::map < std::string, cxxSurfaceComp > surfaceComps;
	std::map < std::string, cxxSurfaceCharge > surfaceCharges;
	enum SURFACE_TYPE type;
	enum DIFFUSE_LAYER_TYPE dl_type;
	enum SITES_UNITS sites_units;
	bool only_counter_ions;
	double thickness;
	double debye_lengths;
	double DDL_viscosity;
	double DDL_limit;
	bool transport;
	cxxNameDouble totals;

public:
	//static std::map<int, cxxSurface>& map;
};

#endif // !defined(SURFACE_H_INCLUDED)
