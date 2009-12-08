#if !defined(PPASSEMBLAGE_H_INCLUDED)
#define PPASSEMBLAGE_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

#include "NumKeyword.h"
#include "PPassemblageComp.h"
class cxxMix;

class cxxPPassemblage:public cxxNumKeyword
{

  public:
	cxxPPassemblage();
	cxxPPassemblage(struct pp_assemblage *);
	  cxxPPassemblage(PHREEQC_PTR_ARG_COMMA const std::map < int, cxxPPassemblage > &entity_map,
					  cxxMix & mx, int n_user);
	 ~cxxPPassemblage();

	struct pp_assemblage *cxxPPassemblage2pp_assemblage(PHREEQC_PTR_ARG);

	struct pure_phase *cxxPPassemblageComp2pure_phase();

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void read_raw(PHREEQC_PTR_ARG_COMMA CParser & parser, bool check = true);

	const cxxNameDouble & get_totals() const
	{
		return this->totals;
	};

#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < double >&doubles);
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
#endif

	void totalize(PHREEQC_PTR_ARG);
#ifdef ORCHESTRA
	void ORCH_write_chemistry(std::ostream & chemistry_dat);
	void ORCH_write_output_vars(std::ostream & outstream);
	void ORCH_read(std::vector < std::pair < std::string,
				   double >>output_vector,
				   std::vector < std::pair < std::string,
				   double >>::iterator & it);
	void ORCH_store_global(std::map < std::string, double >output_map);
#endif

private:
	void add(PHREEQC_PTR_ARG_COMMA const cxxPPassemblage & addee, double extensive);
	// not written
	void dump_xml(std::ostream & os, unsigned int indent = 0) const;

protected:
	//std::list < cxxPPassemblageComp > ppAssemblageComps;
	std::map <std::string, cxxPPassemblageComp > ppAssemblageComps;
	cxxNameDouble eltList;
	cxxNameDouble totals;

public:
	//static std::map<int, cxxPPassemblage>& map;

};

#endif // !defined(PPASSEMBLAGE_H_INCLUDED)
