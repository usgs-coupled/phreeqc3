#if !defined(PHASE_H_INCLUDED)
#define PHASE_H_INCLUDED

#define SOLID 4
#include <vector>
#include <string>
#include "CReaction.h"
/*----------------------------------------------------------------------
 *   Phases
 *---------------------------------------------------------------------- */
class phase
{
public:
	~phase() {};
	phase()
	{	/* all data pertinent to a pure solid phase */
		this->Initialize();
	}
	void Initialize();
	double Calc_rxn_delta_v();
	double Calc_rxn_iap();
	double Calc_rxn_si(double tk_x, double ppas_x, double& iap, double& lk);
	double Calc_rxn_lk(double tk_x, double ppas_x);
	double Calc_rxn_si(double tk_x, double ppas_x);

	double Calc_rxn_x_delta_v();
	double Calc_rxn_x_iap();
	double Calc_rxn_x_lk(double tk_x, double ppas_x);
	double Calc_rxn_x_si(double tk_x, double ppas_x);
	double Calc_rxn_x_si(double tk_x, double ppas_x, double& iap, double& lk);
public:
	std::string name;
	std::string formula;
	int in;
	double lk;
	Logk::DELTA_H_UNIT original_units;
	Logk::DELTA_V_UNIT original_deltav_units;
	std::vector<class name_coef> add_logk;
	double moles_x;
	double delta_max;
	double p_soln_x;
	double fraction_x;
	double log10_lambda;
	double log10_fraction_x;
	double dn, dnb, dnc;
	double gn, gntot;
	double gn_n, gntot_n;
	double t_c, p_c, omega;
	double pr_a, pr_b, pr_alpha;
	double pr_tk, pr_p;
	double pr_phi;
	double pr_aa_sum2;
	double delta_v[9];
	double pr_si_f;
	bool pr_in;
	int type;
	std::vector<class elt_list> next_elt;
	std::vector<class elt_list> next_sys_total;
	int check_equation;
	CReaction rxn;
	CReaction rxn_s;
	CReaction rxn_x;
	int replaced;
	int in_system;
};
#endif // !defined(PHASE_H_INCLUDED)