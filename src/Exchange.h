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
	//cxxExchange(struct exchange *, PHRQ_io *io=NULL);
	cxxExchange(const std::map < int, cxxExchange > &exchange_map,
		cxxMix & mx, int n_user, PHRQ_io *io=NULL);
	~cxxExchange();

	void dump_raw(std::ostream & s_oss, unsigned int indent, int *n_out=NULL) const;

	void read_raw(CParser & parser, bool check = true);

	bool Get_related_phases(void) const;

	bool Get_related_rate(void) const;

	bool Get_pitzer_exchange_gammas() const;
	void Set_pitzer_exchange_gammas(bool b);

	bool Get_new_def(void) const {return this->new_def;};
	void Set_new_def(bool tf) {this->new_def = tf;};
	bool Get_solution_equilibria(void) const {return this->solution_equilibria;};
	void Set_solution_equilibria(bool tf) {this->solution_equilibria = tf;};
	int Get_n_solution(void) const {return this->n_solution;};
	void Set_n_solution(int i) {this->n_solution = i;};
	cxxExchComp *ExchComp_find(std::string s);

	std::map < std::string, cxxExchComp > &Get_exchComps(void) ;
	void totalize();

	const cxxNameDouble & Get_totals() const;
	std::vector<cxxExchComp *> Vectorize(void);

#ifdef USE_MPI
	void mpi_pack(std::vector < int >&ints, std::vector < double >&doubles);
	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
#endif
protected:
	void add(const cxxExchange & addee, double extensive);
	// not written
	void dump_xml(std::ostream & os, unsigned int indent = 0) const;

protected:
	bool new_def;
	bool solution_equilibria;
	int n_solution;
	std::map < std::string, cxxExchComp > exchComps;
	bool pitzer_exchange_gammas;
	cxxNameDouble totals;
public:

};

#endif // !defined(EXCHANGE_H_INCLUDED)
