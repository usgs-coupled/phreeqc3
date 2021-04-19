#if !defined(SPECIES_H_INCLUDED)
#define SPECIES_H_INCLUDED
#define TRUE 1
#define FALSE 0
#include <vector>
#include "CReaction.h"
/*----------------------------------------------------------------------
 *   Species
 *---------------------------------------------------------------------- */
class species
{
public:
	~species() {};
	species()
	{	/* all data pertinent to an aqueous species */
		name = NULL;          // name of species 
		//mole_balance.clear();  // formula for mole balance 
		in = FALSE;           // set internally if species in model
		number = 0;
		// points to master species list, NULL if not primary master
		primary = NULL;
		// points to master species list, NULL if not secondary master
		secondary = NULL;
		gfw = 0;              // gram formula wt of species
		z = 0;                // charge of species
		// tracer diffusion coefficient in water at 25oC, m2/s
		dw = 0;
		// correct Dw for temperature: Dw(TK) = Dw(298.15) * exp(dw_t / TK - dw_t / 298.15)
		dw_t = 0;
		// parms for calc'ng SC = SC0 * exp(-dw_a * z * mu^0.5 / (1 + DH_B * dw_a2 * mu^0.5))
		dw_a = 0;
		dw_a2 = 0;
		dw_a_visc = 0;   // viscosity correction of SC
		dw_t_SC = 0;     // contribution to SC, for calc'ng transport number with BASIC
		dw_corr = 0;	 // dw corrected for TK and mu
		erm_ddl = 0;     // enrichment factor in DDL
		equiv = 0;       // equivalents in exchange species
		alk = 0;	     // alkalinity of species, used for cec in exchange
		carbon = 0;      // stoichiometric coefficient of carbon in species
		co2 = 0;         // stoichiometric coefficient of C(4) in species
		h = 0;           // stoichiometric coefficient of H in species
		// stoichiometric coefficient of O in species
		o = 0;
		// WATEQ Debye Huckel a and b-dot; active_fraction coef for exchange species
		dha = 0, dhb = 0, a_f = 0;
		lk = 0;           // log10 k at working temperature
		// log kt0, delh, 6 coefficients analytical expression + volume terms
		//for (size_t i = 0; i < Logk::MAX_LOG_K_INDICES; i++) logk[i] = 0;
		logk.resize(Logk::MAX_LOG_K_INDICES, 0.0);
		// 7 coefficients analytical expression for B, D, anion terms and pressure in Jones_Dole viscosity eqn
		for (size_t i = 0; i < 10; i++) Jones_Dole[i] = 0;
		// regression coefficients to calculate temperature dependent phi_0and b_v of Millero density model
		for (size_t i = 0; i < 7; i++) millero[i] = 0;
		original_units = Logk::kjoules;  // enum with original delta H units
		//add_logk.clear();
		lg = 0;            // log10 activity coefficient, gamma
		lg_pitzer = 0;     // log10 activity coefficient, from pitzer calculation
		lm = 0;            // log10 molality
		la = 0;		       // log10 activity
		dg = 0;		       // gamma term for jacobian
		dg_total_g = 0;
		moles = 0;		   // moles in solution; moles/mass_water = molality
		type = 0;          // flag indicating presence in model and types of equations
		gflag = 0;		   // flag for preferred activity coef eqn
		exch_gflag = 0;    // flag for preferred activity coef eqn
		// vector of elements
		//next_elt.clear();
		//next_secondary.clear();
		//next_sys_total.clear();
		// switch to check equation for charge and element balance
		check_equation = TRUE;
		//CReaction rxn;   // data base reaction
		//CReaction rxn_s; // reaction converted to secondary and primary master species
		//CReaction rxn_x; // reaction to be used in model
		// (1 + sum(g)) * moles
		tot_g_moles = 0;
		// sum(moles*g*Ws/Waq)
		tot_dh2o_moles = 0;
		for (size_t i = 0; i < 5; i++) cd_music[i] = 0;
		//for (size_t i = 0; i < 3; i++) dz[i] = 0;
		dz.resize(3, 0.0);
		original_deltav_units = Logk::cm3_per_mol;
	}
	const char* name;
	std::string mole_balance;
	int in;
	int number;
	class master* primary;
	class master* secondary;
	double gfw;
	double z;
	double dw;
	double dw_t;
	double dw_a;
	double dw_a2;
	double dw_a_visc;
	double dw_t_SC;
	double dw_corr;
	double erm_ddl;
	double equiv;
	double alk;
	double carbon;
	double co2;
	double h;
	double o;
	double dha, dhb, a_f;
	double lk;
	std::vector<double> logk;
	double Jones_Dole[10];
	double millero[7];
	Logk::DELTA_H_UNIT original_units;
	std::vector<class name_coef> add_logk;
	double lg;
	double lg_pitzer;
	double lm;
	double la;
	double dg;
	double dg_total_g;
	double moles;
	int type;
	int gflag;
	int exch_gflag;
	std::vector<class elt_list> next_elt;
	std::vector<class elt_list> next_secondary;
	std::vector<class elt_list> next_sys_total;
	int check_equation;
	CReaction rxn;
	CReaction rxn_s;
	CReaction rxn_x;
	double tot_g_moles;
	double tot_dh2o_moles;
	double cd_music[5];
	std::vector<double> dz;
	Logk::DELTA_V_UNIT original_deltav_units;
};
#endif // !defined(SPECIES_H_INCLUDED)