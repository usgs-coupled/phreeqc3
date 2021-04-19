#ifndef SKIP
#if !defined(CREACTION_H_INCLUDED)
#define CREACTION_H_INCLUDED
#include "Logk.h"
/*----------------------------------------------------------------------
 *   CReaction
 *---------------------------------------------------------------------- */
class CReaction : public Logk
{
public:
	CReaction(void);
	CReaction(size_t ntoken);
	~CReaction(void) {}
	std::vector<double>& Get_logk_cr(void) { return this->logk_cr; }
	void   Set_logk_cr(double* d);
	const std::vector<double>& Get_dz(void) { return this->dz; }
	void   Set_dz(double* d);
	size_t size() { return token.size(); }
	std::vector<class rxn_token>& Get_tokens(void) { return this->token; }
	void Set_tokens(const std::vector<class rxn_token>& t) { this->token = t; }

public:
	std::vector<double> logk_cr;
	std::vector<double> dz;
	std::vector<class rxn_token> token;
};
class rxn_token
{
public:
	~rxn_token() {};
	rxn_token()
	{
		s = NULL;
		coef = 0.0;
		//name.clear();
		end = false;
	}
	void Set_end(bool b) { end = b; }
	bool Get_end() { return (s == NULL) ? true : false; }
	class species* Get_s() { return this->s; }
	void Set_s(class species* sp) { this->s = sp; }
	const std::string& Get_name() { return this->name; }
	void Set_name(const std::string& str) { this->name = str; }

	double coef;
protected:
	std::string name;
	class species* s;
	bool end;
};
#endif // (CREACTION_H_INCLUDED)
#endif