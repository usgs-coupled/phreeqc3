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
	CReaction(bool is_phase = false)
	{
		this->Initialize(is_phase);
	}
	CReaction(size_t ntoken, bool is_phase = false)
	{
		this->Initialize(is_phase);
		this->token.resize(ntoken);
	}
	~CReaction(void) {}
	std::vector<double>& Get_logk_cr(void) { return this->logk_cr; }
	void Set_logk_cr(const std::vector<double>& d);
	const std::vector<double>& Get_dz(void) { return this->dz; }
	void Set_dz(const std::vector<double>& d);
	size_t size() { return token.size(); }
	std::vector<class rxn_token>& Get_tokens(void) { return this->token; }
	void Set_tokens(const std::vector<class rxn_token>& t) { this->token = t; }
	void Set_phase(bool tf) { this->phase = tf; }
	bool Get_phase() { return this->phase; }
	void Set_mu_last(double m) { this->mu_last = m; }
	void Initialize(bool is_phase);
	double Calc_delta_v();
	double Calc_iap();
	double Calc_iap_la();
	double Calc_si(double tempk, double presPa);
	double Calc_si(double tempk, double presPa, double& iap, double& lk);
	double Calc_si_la(double tempk, double presPa);
	double Calc_lk(double tempk, double presPa);
	double Get_lk() { return this->lk; }
	double Get_iap() { return this->iap; }
	double Get_iap_la() { return this->iap_la; }
	double Get_si() { return this->si; }
	double Get_si_la() { return this->Get_si_la(); }
public:
	std::vector<double> logk_cr;
	std::vector<double> dz;
	std::vector<class rxn_token> token;
	bool phase;
	double tc_last;
	double p_atm_last;
	double mu_last;
	double iap;
	double iap_la;
	double lk;
	double si;
	double si_la;
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