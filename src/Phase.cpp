#define TRUE 1
#define FALSE 0
#include "Phase.h"
#include "Phreeqc.h"
/* **********************************************************************
 *
 *   Routines related to structure "phases"
 *
 * ********************************************************************** */

void phase::Initialize()
{
	name.clear();               //name of phase
	formula.clear();			// chemical formula 
	in = FALSE;					// species used in model if TRUE 
	lk = 0;					    // log10 k at working temperature 
	// log kt0, delh, 6 coefficients analalytical expression
	logk.clear();
	logk.resize(Logk::MAX_LOG_K_INDICES, 0.0);
	// enum with original delta H units 
	original_units = Logk::kjoules;
	original_deltav_units = Logk::cm3_per_mol;
	add_logk.clear();
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
	pr_phi = 1.0;
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
	next_elt.clear();	    
	next_sys_total.clear();
	// switch to check equation for charge and element balance
	check_equation = TRUE;
	// data base reaction
	rxn.Initialize(true);
	// reaction converted to secondary and primary master species
	rxn_s.Initialize(true);
	// reaction to be used in model
	rxn_x.Initialize(true);
	// equation contains solids or gases                    
	replaced = FALSE;
	in_system = FALSE;
}

/* ---------------------------------------------------------------------- */
class phase* Phreeqc::
phase_bsearch(const std::string& name_in, int* j, int print)
/* ---------------------------------------------------------------------- */
{
	*j = -1;
	std::string name = name_in;
	str_tolower(name);
	std::map< std::string, phase* >::iterator it = phases_map.find(name);
	if (it != phases_map.end())
	{
		return it->second;
	}
	return NULL;
}
/* ---------------------------------------------------------------------- */
class phase* Phreeqc::
phase_store(const std::string& name_in)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Function locates the string "name" in the map for phases.
	 *
	 *   If found, pointer to the appropriate phase structure is returned.
	 *
	 *   If the string is not found, a new entry is made at the end of
	 *   the phases array (position count_phases), it is added to the map,
	 *   and the new structure is returned.
	 *
	 *   Arguments:
	 *      name    input, character string to be located or stored.
	 *
	 *   Returns:
	 *      The address of a phase structure that contains the phase data.
	 *      If phase existed, it is reinitialized. The structure returned
	 *      contains only the name of the phase.
	 */
	class phase* phase_ptr = NULL;
	/*
	 *   Search list
	 */
	std::string name = name_in;
	str_tolower(name);
	std::map<std::string, class phase*>::iterator p_it =
		phases_map.find(name);
	if (p_it != phases_map.end())
	{
		phase_ptr = p_it->second;
		phase_ptr->Initialize();
		phase_ptr->name = name_in;
		return (phase_ptr);
	}
	/*
	 *   Make new phase structure and return pointer to it
	 */
	size_t n = phases.size();
	phases.resize(n + 1);
	//phases[n] = phase_alloc();
	phases[n] = new phase;
	/* set name in phase structure */
	phases[n]->name = name_in;
	/*
	 *   Update map
	 */
	phases_map[name] = phases[n];
	return (phases[n]);
}
double phase::
Calc_rxn_delta_v()
{
	if (this->replaced)
		return this->rxn_s.Calc_delta_v();
	return this->rxn.Calc_delta_v();
}
double phase::
Calc_rxn_iap()
{
	if (this->replaced)
		return this->rxn_s.Calc_iap();
	return this->rxn.Calc_iap();
}
double phase::
Calc_rxn_lk(double tk_x, double ppas_x)
{
	if (this->replaced)
		return this->rxn_s.Calc_lk(tk_x, ppas_x);
	return this->rxn_x.Calc_lk(tk_x, ppas_x);
}
double phase::
Calc_rxn_si(double tk_x, double ppas_x/*, Phreeqc* phrq_ptr*/)
{
	if (this->replaced)
		return this->rxn_s.Calc_si(tk_x, ppas_x);
	return this->rxn.Calc_si(tk_x, ppas_x);
}
double phase::
Calc_rxn_x_delta_v()
{
	return this->rxn_x.Calc_delta_v();
}
double phase::
Calc_rxn_x_iap()
{
	return this->rxn_x.Calc_iap();
}
double phase::
Calc_rxn_x_lk(double tk_x, double ppas_x)
{
	return this->rxn_x.Calc_lk(tk_x, ppas_x);
}
double phase::
Calc_rxn_x_si(double tk_x, double ppas_x)
{
	return this->rxn_x.Calc_si(tk_x, ppas_x);
}