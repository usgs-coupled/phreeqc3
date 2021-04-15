#if !defined(TRXN_H_INCLUDED)
#define TRXN_H_INCLUDED
#include <vector>
#include <string>
#include "global_structures.h"
#include "PHRQ_io.h"
class Phreeqc;

class reaction_temp
{
public:
	~reaction_temp() {};
	reaction_temp()
	{
		for (size_t i = 0; i < MAX_LOG_K_INDICES; i++) logk[i] = 0;
		for (size_t i = 0; i < 3; i++) dz[i] = 0;
		//token.clear();
		count_trxn = 0;
		phrq_ptr = NULL;
	}
	bool phase_rxn_to_trxn(class phase* phase_ptr, CReaction& rxn_ref);
	int rewrite_master_to_secondary(class master* master_ptr1,
		class master* master_ptr2);
	int species_rxn_to_trxn(class species* s_ptr);
	bool trxn_add(CReaction& r_ref, double coef, bool combine);
	bool trxn_add_phase(CReaction& r_ref, double coef, bool combine);
	int trxn_combine(void);
	int trxn_compare(const void* ptr1, const void* ptr2);
	bool trxn_copy(CReaction& rxn_ref);
	double trxn_find_coef(const char* str, int start);
	int trxn_multiply(LDBLE coef);
	int trxn_print(void);
	int trxn_reverse_k(void);
	int trxn_sort(void);
	int trxn_swap(const char* token);
	void Set_phrq_ptr(Phreeqc* p) { this->phrq_ptr = p; }
	void Set_count_trxn(size_t count) { this->count_trxn = count; }
	size_t Get_count_trxn(void) { return this->count_trxn; }

	double logk[MAX_LOG_K_INDICES];
	double dz[3];
	std::vector<class rxn_token_temp> token;
	size_t count_trxn;
	Phreeqc* phrq_ptr;
};
class rxn_token_temp
{
public:
	~rxn_token_temp() {};
	rxn_token_temp()
	{	// data for equations, aq. species or minerals
		//name.clear();		// pointer to a species name (formula)
		z = 0;		        // charge on species 
		s = NULL;
		coef = 0;			// coefficient of species name 
	}
	class species* Get_s() { return this->s; }
	void Set_s(class species* sp) { this->s = sp; }
	const std::string& Get_name()const { return this->name; }
	void Set_name(const std::string& str) { this->name = str; }
	double z;
	double coef;
	bool operator<(const rxn_token_temp& rhs) const { return name < rhs.name; }
protected:
	class species* s;
	std::string name;
};
#endif // !defined(TRXN_H_INCLUDED)