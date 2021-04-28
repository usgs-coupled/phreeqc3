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
		this->Initialize();
	}
	void Initialize();
	class master* Pick_master() { return pick_master; }
	CReaction& Get_rxn() { return rxn; }
	CReaction& Get_rxn_s() { return rxn_s; }
	CReaction& Get_rxn_x() { return rxn_x; }
	//double Calc_rxn_lk(double tk_x, double ppas_x);
	std::string name;
	std::string mole_balance;
	int in;
	int number;
	class master* primary;
	class master* secondary;
	class master* pick_master;
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
	//std::vector<double> logk;
	double Jones_Dole[10];
	double millero[7];
	//std::vector<class name_coef> add_logk;
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
};
#endif // !defined(SPECIES_H_INCLUDED)