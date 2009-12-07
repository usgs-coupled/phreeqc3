#if !defined(EXCHANGE_H_INCLUDED)
#define EXCHANGE_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include "NumKeyword.h"
#include "ExchComp.h"
#include "NameDouble.h"
class cxxMix;

class cxxExchange:public cxxNumKeyword
{

public:
	cxxExchange();
	cxxExchange(int n_user);
	cxxExchange(struct exchange *);
	cxxExchange(const std::map < int, cxxExchange > &exchange_map,
		cxxMix & mx, int n_user);
	~cxxExchange();

	struct exchange *cxxExchange2exchange();

	struct exch_comp *cxxExchComp2exch_comp();

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void read_raw(CParser & parser, bool check = true);

	bool get_related_phases(void);

	bool get_related_rate(void);

	bool get_pitzer_exchange_gammas();
	void set_pitzer_exchange_gammas(bool b);

	std::map < std::string, cxxExchComp > &get_exchComps(void);
	void totalize();

	const cxxNameDouble & get_totals() const;

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
