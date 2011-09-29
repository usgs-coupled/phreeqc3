#if !defined(EXCHANGE_H_INCLUDED)
#define EXCHANGE_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include "NumKeyword.h"
#include "ExchComp.h"
#include "NameDouble.h"
#include "PHRQ_base.h"
class cxxMix;

class cxxExchange:public cxxNumKeyword
{

public:
	cxxExchange(PHRQ_io *io=NULL);
	cxxExchange(struct exchange *, PHRQ_io *io=NULL);
	cxxExchange(const std::map < int, cxxExchange > &exchange_map,
		cxxMix & mx, int n_user, PHRQ_io *io=NULL);
	~cxxExchange();

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void read_raw(CParser & parser, bool check = true);

	bool Get_related_phases(void) const;

	bool Get_related_rate(void) const;

	bool Get_pitzer_exchange_gammas() const;
	void Set_pitzer_exchange_gammas(bool b);

	const std::map < std::string, cxxExchComp > &get_exchComps(void) const;
	void totalize();

	const cxxNameDouble & Get_totals() const;

#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < double >&doubles);
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
#endif
private:
	void add(const cxxExchange & addee, double extensive);
	// not written
	void dump_xml(std::ostream & os, unsigned int indent = 0) const;

protected:
	std::map < std::string, cxxExchComp > exchComps;
	bool pitzer_exchange_gammas;
	cxxNameDouble totals;
public:

};

#endif // !defined(EXCHANGE_H_INCLUDED)
