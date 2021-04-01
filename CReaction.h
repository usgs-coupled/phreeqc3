#if !defined(CREACTION_H_INCLUDED)
#define CREACTION_H_INCLUDED
#include <vector>
#include "global_structures.h"

/*----------------------------------------------------------------------
 *   Reaction
 *---------------------------------------------------------------------- */

class CReaction
{
public:
	CReaction(void);
	CReaction(size_t ntoken);
	~CReaction(void) {}
	double* Get_logk(void) { return this->logk; }
	void   Set_logk(double* d);
	double* Get_dz(void) { return this->dz; }
	void   Set_dz(double* d);
	size_t size() { return token.size(); }
	std::vector<struct rxn_token>& Get_tokens(void) { return this->token; }
	void Set_tokens(const std::vector<struct rxn_token>& t) { this->token = t; }

public:
	double logk[21];   //LOG_K_INDICES::MAX_LOG_K_INDICES
	//LDBLE logk[LOG_K_INDICES::MAX_LOG_K_INDICES];
	double dz[3];
	std::vector<struct rxn_token> token;
};
#endif // !defined(CREACTION_H_INCLUDED)