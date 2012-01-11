#if !defined(CXXKINETICS_H_INCLUDED)
#define CXXKINETICS_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector
#include "phrqtype.h"
#include "NumKeyword.h"
#include "KineticsComp.h"
#include "PHRQ_base.h"
class cxxMix;

class cxxKinetics:public cxxNumKeyword
{

  public:
	cxxKinetics(PHRQ_io *io=NULL);
	cxxKinetics(struct kinetics *, PHRQ_io *io=NULL);
	cxxKinetics(const std::map < int, cxxKinetics > &entity_map, cxxMix & mx,
				int n_user, PHRQ_io *io=NULL);
	~cxxKinetics();

	//void dump_xml(std::ostream& os, unsigned int indent = 0)const;

	void dump_raw(std::ostream & s_oss, unsigned int indent, int * n_out=NULL) const;

	void read_raw(CParser & parser, bool check = true);

	//bool Get_related_phases(void) const;
	//bool Get_related_rate(void) const;

	const std::vector < LDBLE > &Get_steps(void) const {return steps;};
	LDBLE Get_step_divide(void) const {return step_divide;};
	int Get_rk(void) const {return rk;};
	int Get_bad_step_max(void) const {return bad_step_max;};
	bool Get_use_cvode(void) const {return use_cvode;};
	int Get_cvode_steps(void) const {return cvode_steps;};
	int Get_cvode_order(void) const {return cvode_order;};
	const std::list < cxxKineticsComp > &Get_kineticsComps(void) const {return kineticsComps;};
	const cxxNameDouble & Get_totals(void) const {return this->totals;};
	int Get_equal_steps(void) const {return equal_steps;};


#ifdef USE_MPI
	void mpi_unpack(int *ints, int *ii, LDBLE *doubles, int *dd);
	void mpi_pack(std::vector < int >&ints, std::vector < LDBLE >&doubles);
#endif
  protected:
	void add(const cxxKinetics & addee, LDBLE extensive);

  protected:
	std::list < cxxKineticsComp > kineticsComps;
	std::vector < LDBLE >steps;
	int equal_steps;
	cxxNameDouble totals;
	LDBLE step_divide;
	int rk;
	int bad_step_max;
	bool use_cvode;
	int cvode_steps;
	int cvode_order;

};

#endif // !defined(CXXKINETICS_H_INCLUDED)
