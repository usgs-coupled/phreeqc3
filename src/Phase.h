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
		//name.clear()              //name of phase
		//formula.clear();			// chemical formula 
		in = FALSE;					// species used in model if TRUE 
		lk = 0;					    // log10 k at working temperature 
		// log kt0, delh, 6 coefficients analalytical expression
		//for (size_t i = 0; i < Logk::MAX_LOG_K_INDICES; i++) logk[i] = 0;
		logk.resize(Logk::MAX_LOG_K_INDICES, 0.0);
		// enum with original delta H units 
		original_units = Logk::kjoules;
		original_deltav_units = Logk::cm3_per_mol;
		//add_logk.clear();
		moles_x = 0;
		delta_max = 0;
		p_soln_x = 0;
		fraction_x = 0;
		log10_lambda = 0;
		log10_fraction_x = 0;
		dn = 0, dnb = 0, dnc = 0;
		gn = 0, gntot = 0;
		gn_n = 0, gntot_n = 0;
		// gas: critical TK, critical P(atm), Pitzer acentric coeff 
		t_c = 0, p_c = 0, omega = 0;
		// Peng-Robinson parm's
		pr_a = 0, pr_b = 0, pr_alpha = 0;
		// Temperature (K), Pressure (atm)
		pr_tk = 0, pr_p = 0;
		// fugacity coefficient (-) 
		pr_phi = 0;
		// for calculating multicomponent phi 
		pr_aa_sum2 = 0;
		// Logk::delta_v[0] = [1] + [2]*T + [3]/T + [4]*log10(T) + [5]/T^2 + [6]*T^2 + [7]*P
		for (size_t i = 0; i < 9; i++) delta_v[i] = 0;
		// si adapter: log10(phi) - Logk::delta_v[0] * (P - 1) /RT
		pr_si_f = 0;
		// Peng-Robinson in the calc's, or not
		pr_in = false;
		// flag indicating presence in model and types of equations
		type = SOLID;
		// list of elements in phase 
		//next_elt.clear();	    
		//next_sys_total.clear();
		// switch to check equation for charge and element balance
		check_equation = TRUE;
		// data base reaction
		//CReaction rxn;
		// reaction converted to secondary and primary master species
		//CReaction rxn_s;
		// reaction to be used in model
		//CReaction rxn_x;
		// equation contains solids or gases                    
		replaced = FALSE;
		in_system = FALSE;
	}
	std::string name;
	std::string formula;
	int in;
	double lk;
	std::vector<double> logk;
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