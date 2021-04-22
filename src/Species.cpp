#include "Species.h"
#include "Phreeqc.h"
/* ---------------------------------------------------------------------- */
void species::Initialize()
{
		name.clear();          // name of species 
		mole_balance.clear();  // formula for mole balance 
		in = FALSE;            // set internally if species in model
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
		erm_ddl = 1.0;     // enrichment factor in DDL
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
		logk.clear();
		logk.resize(Logk::MAX_LOG_K_INDICES, 0.0);
		// 7 coefficients analytical expression for B, D, anion terms and pressure in Jones_Dole viscosity eqn
		for (size_t i = 0; i < 10; i++) Jones_Dole[i] = 0;
		// regression coefficients to calculate temperature dependent phi_0and b_v of Millero density model
		for (size_t i = 0; i < 7; i++) millero[i] = 0;
		original_units = Logk::kjoules;  // enum with original delta H units
		add_logk.clear();
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
		next_elt.clear();
		next_secondary.clear();
		next_sys_total.clear();
		// switch to check equation for charge and element balance
		check_equation = TRUE;
		rxn.Initialize(false);   // data base reaction
		rxn_s.Initialize(false); // reaction converted to secondary and primary master species
		rxn_x.Initialize(false); // reaction to be used in model
		// (1 + sum(g)) * moles
		tot_g_moles = 0;
		// sum(moles*g*Ws/Waq)
		tot_dh2o_moles = 0;
		for (size_t i = 0; i < 5; i++) cd_music[i] = 0;
		//for (size_t i = 0; i < 3; i++) dz[i] = 0;
		dz.clear();
		dz.resize(3, 0.0);
		original_deltav_units = Logk::cm3_per_mol;
}
/* ---------------------------------------------------------------------- */
class species* Phreeqc::
s_search(const std::string& name)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Function locates the string "name" in the species_map.
	 *
	 *   Arguments:
	 *      name  input, a character string to be located in species.
	 *
	 *   Returns:
	 *   If found, pointer to the appropriate species structure is returned.
	 *       else, NULL pointer is returned.
	 */
	class species* s_ptr = NULL;
	std::map<std::string, class species*>::iterator s_it =
		species_map.find(name);
	if (s_it != species_map.end())
	{
		s_ptr = s_it->second;
	}
	return (s_ptr);
}
/* ---------------------------------------------------------------------- */
class species* Phreeqc::
s_store(const std::string& name, LDBLE l_z, int replace_if_found)
/* ---------------------------------------------------------------------- */
{
	//   Search list and save
	class species* s_ptr = NULL;
	s_ptr = s_search(name);
	if (s_ptr != NULL && replace_if_found == FALSE)
	{
		return (s_ptr);
	}
	else if (s_ptr != NULL && replace_if_found == TRUE)
	{
		s_ptr->Initialize();
	}
	else
	{
		size_t n = s.size();
		s.resize(n + 1);
		s[n] = new species;
		s_ptr = s[n];
	}
	// set name and z in pointer in species structure
	s_ptr->name = name;
	s_ptr->z = l_z;
	//   Update map
	species_map[name] = s_ptr;
	return (s_ptr);
}
