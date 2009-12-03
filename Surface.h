#if !defined(SURFACE_H_INCLUDED)
#define SURFACE_H_INCLUDED

#include "NumKeyword.h"
#define EXTERNAL extern
#include "global.h"
#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

#include "SurfaceComp.h"
#include "SurfaceCharge.h"
#include "cxxMix.h"

class cxxSurface:public cxxNumKeyword
{

  public:
	cxxSurface();
	cxxSurface(struct surface *);
	  cxxSurface(const std::map < int, cxxSurface > &entity_map, cxxMix & mx,
				 int n_user);
	 ~cxxSurface();

	//enum SURFACE_STYPE { UNKNOWN_DL, NO_EDL, DDL, CD_MUSIC };
	//enum SURFACE_DL_TYPE { NO_DL, BORKOVEC_DL, DONNAN_DL } ;
	//enum SURFACE_SITES_UNITS { SITES_ABSOLUTE, SITES_DENSITY } ;

	struct surface *cxxSurface2surface();

	struct surf_comp *cxxSurfaceComp2surf_comp();

	void dump_xml(std::ostream & os, unsigned int indent = 0) const;

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void read_raw(CParser & parser, bool check = true);

	bool get_related_phases(void);

	bool get_related_rate(void);

	void totalize();

	const cxxNameDouble & get_totals() const
	{
		return this->totals;
	};

#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < double >&doubles);
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
#endif
private:
	void add(const cxxSurface & addee, double extensive);

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
