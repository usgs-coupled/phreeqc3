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
	cxxPPassemblage(PHRQ_io * io=NULL);
	cxxPPassemblage(struct pp_assemblage *, PHRQ_io * io=NULL);
	cxxPPassemblage(const std::map < int, cxxPPassemblage > &entity_map,
					  cxxMix & mx, int n_user, PHRQ_io * io=NULL);
	~cxxPPassemblage();

	void dump_raw(std::ostream & s_oss, unsigned int indent, int *n_out=NULL) const;

	void read_raw(CParser & parser, bool check = true);

	const cxxNameDouble & Get_totals() const
	{
		return this->totals;
	};
	const cxxNameDouble & Get_eltList() const
	{
		return this->eltList;
	};
	const std::map <std::string, cxxPPassemblageComp > & Get_ppAssemblageComps() const
	{
		return this->ppAssemblageComps;
	};
#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < double >&doubles);
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
#endif

	void totalize(PHREEQC_PTR_ARG);

private:
	void add(const cxxPPassemblage & addee, double extensive);
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
