#if !defined(SSASSEMBLAGE_H_INCLUDED)
#define SSASSEMBLAGE_H_INCLUDED


#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

#include "NumKeyword.h"
#include "NameDouble.h"
#include "phreeqc_class.h"

class cxxSSassemblageSS;

//#include "cxxMix.h"
class cxxMix;

class cxxSSassemblage:public cxxNumKeyword
{

public:
	cxxSSassemblage(PHRQ_io * io = NULL);
	cxxSSassemblage(struct s_s_assemblage *, PHRQ_io * io = NULL);
	cxxSSassemblage(const std::map < int, cxxSSassemblage > &entity_map,
		cxxMix & mx, int n_user, PHRQ_io * io = NULL);
	~cxxSSassemblage();

	//void dump_xml(std::ostream& os, unsigned int indent = 0)const;

	void dump_raw(std::ostream & s_oss, unsigned int indent, int *n_out=NULL) const;

	void read_raw(CParser & parser, bool check = true);
	const std::map <std::string, cxxSSassemblageSS > & Get_ssAssemblageSSs() const
	{
		return this->ssAssemblageSSs;
	};

#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < double >&doubles);
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
#endif
	void totalize(PHREEQC_PTR_ARG);

	const cxxNameDouble & Get_totals() const
	{
		return this->totals;
	};


private:
	void add(const cxxSSassemblage & addee, double extensive);

protected:
	std::map < std::string, cxxSSassemblageSS > ssAssemblageSSs;
	cxxNameDouble totals;

public:
	//static std::map<int, cxxSSassemblage>& map;

};

#endif // !defined(SSASSEMBLAGE_H_INCLUDED)
