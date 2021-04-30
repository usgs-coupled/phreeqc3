#if !defined(CREACTION_H_INCLUDED)
#define CREACTION_H_INCLUDED
#include "Logk.h"
/*----------------------------------------------------------------------
 *   CReaction
 *---------------------------------------------------------------------- */
class CReaction 
{
public:
	CReaction(bool is_phase = false)
	{
		this->Initialize(is_phase);
	}
	CReaction(size_t ntoken, bool is_phase = false)
	{
		this->Initialize(is_phase);
		this->tokens.resize(ntoken);
	}
	~CReaction(void) {}
// methods
	double Calc_delta_v();
	double Calc_iap();
	double Calc_iap_la();
	double Calc_dv_lk_x(double tempk, double presPa);
	double Calc_lk_x(double tempk, double presPa);
	double Calc_iap_dv_lk_si(double tempk, double presPa, double& iap, double& lk);
	double Calc_si_iap_only();
	std::vector<double>& Get_dz(void) { return this->dz; }
	std::vector<name_coef>& Get_add_logk() { return this->Logk_cr.Get_add_logk(); }
	Logk::DELTA_H_UNIT& Get_original_deltah_units() { return this->Logk_cr.Get_original_deltah_units(); }
	Logk::DELTA_V_UNIT& Get_original_deltav_units() { return this->Logk_cr.Get_original_deltav_units(); }
	std::vector<double>& Get_logk_original() { return this->Logk_cr.Get_logk_original(); }
	std::vector<double>& Get_logk_selected() { return this->Logk_cr.Get_logk_selected(); }
	std::vector<double>& Get_logk_x() { return this->Logk_cr.Get_logk_x(); }
	std::vector<class rxn_token>& Get_tokens(void) { return this->tokens; }
	void Initialize(bool is_phase);
	size_t Size() { return tokens.size(); }

// not used yet
	double Calc_si_la(double tempk, double presPa);
	double Get_iap() { return this->iap; }
	double Get_iap_la() { return this->iap_la; }
	double Get_lk() { return this->lk; }
	bool Get_phase() { return this->phase; }
	double Get_si() { return this->si; }
	void Set_phase(bool tf) { this->phase = tf; }
	void Set_logk_original(const std::vector<double>& k) { this->Logk_cr.Set_logk_original(k); }
	void Set_selected() { this->Logk_cr.Set_selected(); }
	void Tidy_logk(Phreeqc* phrq_ptr) { this->Logk_cr.Tidy_logk(phrq_ptr); }

protected:
	std::vector<class rxn_token> tokens;
	double si_la;
	std::vector<double> dz;
	double iap;
	double iap_la;
	double lk;
	Logk Logk_cr;
	bool phase;
	double si;
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
