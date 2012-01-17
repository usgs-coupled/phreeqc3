#include "Phreeqc.h"
#include "phqalloc.h"

#include "Utils.h"
#include "StorageBin.h"
#include "Solution.h"
#include "PPassemblage.h"
#include "SSassemblage.h"
#include "SS.h"
#include "NameDouble.h"
#include "Temperature.h"
#include "cxxMix.h"
#include "Exchange.h"
#include "GasPhase.h"
#include "Reaction.h"
#include "PPassemblage.h"
#include "SSassemblage.h"

/* ---------------------------------------------------------------------- */
int Phreeqc::
step(LDBLE step_fraction)
/* ---------------------------------------------------------------------- */
{
/*
 *   zero global solution, add solution or mixture, add exchange,
 *   add surface, add gas phase, add solid solutions,
 *   set temperature, and add reaction. 
 *   Ensure all elements
 *   included in any of these are present in small amounts.
 *   Save result as n_user -1.
 */
	LDBLE difftemp;
	int step_number;
	cxxPPassemblage *pp_assemblage_save = NULL;
	cxxSSassemblage *ss_assemblage_save = NULL;
/*
 *   Zero out global solution data
 */

	xsolution_zero();
/*
 *   Set reaction to zero
 */
	step_x = 0.0;
	step_number = reaction_step;
/*
 *   Mixing or solution
 */
	if (use.Get_mix_ptr() != NULL)
	{
		add_mix((cxxMix *) use.Get_mix_ptr());
	}
	else if (use.Get_solution_ptr() != NULL)
	{
		add_solution(use.Get_solution_ptr(), 1.0, 1.0);
	}
	else
	{
		input_error++;
		error_msg("Neither mixing nor an initial solution have "
				  "been defined in reaction step.", STOP);
	}
/*
 *   Reaction
 */
	if (use.Get_reaction_ptr() != NULL)
	{
		//add_reaction(use.irrev_ptr, step_number, step_fraction);
		add_reaction( use.Get_reaction_ptr(), step_number, step_fraction);
	}
/*
 *   Kinetics
 */
	if (use.Get_kinetics_ptr() != NULL)
	{
		add_kinetics(use.Get_kinetics_ptr());
	}
/*
 *   Exchange
 */
	if (use.Get_exchange_ptr() != NULL)
	{
		add_exchange((cxxExchange *) use.Get_exchange_ptr());
	}
/*
 *   Surface
 */
	if (use.Get_surface_ptr() != NULL)
	{
		add_surface(use.Get_surface_ptr());
	}
/*
 *   Gases
 */
	if (use.Get_gas_phase_ptr() != NULL)
	{
		cxxGasPhase * gas_phase_ptr = (cxxGasPhase *) use.Get_gas_phase_ptr();
		add_gas_phase(gas_phase_ptr);
	}
/*
 *   Temperature
 */
	if (use.Get_temperature_ptr() != NULL)
	{
		cxxTemperature *t_ptr = (cxxTemperature *) use.Get_temperature_ptr();
		tc_x = t_ptr->Temperature_for_step(step_number);
	}
	if ((state == TRANSPORT) && (transport_step != 0) &&
		(cell > 0) && (cell != count_cells + 1))
	{
		difftemp = tc_x - cell_data[cell - 1].temp;
		cell_data[cell - 1].temp += difftemp / tempr;
		tc_x = cell_data[cell - 1].temp;
	}
/*
 *   Pressure
 */
	if (use.Get_pressure_ptr() != NULL)
	{
		cxxPressure *p_ptr = (cxxPressure *) use.Get_pressure_ptr();
		patm_x = p_ptr->Pressure_for_step(step_number);
	}
/*
 *   Pure phases and solid solutions are added to avoid
 *   zero or negative concentrations
 */
/*
 *   Pure phases
 */
	if (use.Get_pp_assemblage_ptr() != NULL)
	{
		cxxPPassemblage * pp_assemblage_ptr = (cxxPPassemblage *) use.Get_pp_assemblage_ptr();
		pp_assemblage_save = new cxxPPassemblage(*pp_assemblage_ptr);
		add_pp_assemblage(pp_assemblage_ptr);
	}
#ifdef SKIP
	if (use.Get_pp_assemblage_ptr() != NULL)
	{
		pp_assemblage_save =
			(struct pp_assemblage *)
			PHRQ_malloc(sizeof(struct pp_assemblage));
		if (pp_assemblage_save == NULL)
			malloc_error();
		pp_assemblage_copy(use.Get_pp_assemblage_ptr(), pp_assemblage_save,
						   use.Get_pp_assemblage_ptr()->n_user);
		add_pp_assemblage(use.Get_pp_assemblage_ptr());
	}
#endif
/*
 *   Solid solutions
 */
	if (use.Get_ss_assemblage_ptr() != NULL)
	{
		ss_assemblage_save = new cxxSSassemblage(*use.Get_ss_assemblage_ptr());
		add_ss_assemblage(use.Get_ss_assemblage_ptr());
	}
#ifdef SKIP
	if (use.Get_ss_assemblage_ptr() != NULL)
	{
		ss_assemblage_save =
			(struct ss_assemblage *)
			PHRQ_malloc(sizeof(struct ss_assemblage));
		if (ss_assemblage_save == NULL)
			malloc_error();
		ss_assemblage_copy(use.Get_ss_assemblage_ptr(), ss_assemblage_save,
							use.Get_ss_assemblage_ptr()->n_user);
		add_ss_assemblage(use.Get_ss_assemblage_ptr());
	}
#endif
/*
 *   Check that elements are available for gas components,
 *   pure phases, and solid solutions
 */
	if (use.Get_gas_phase_ptr() != NULL)
	{
		cxxGasPhase * gas_phase_ptr = (cxxGasPhase *) use.Get_gas_phase_ptr();
		gas_phase_check(gas_phase_ptr);
	}
	if (use.Get_pp_assemblage_ptr() != NULL)
	{
		cxxPPassemblage * pp_assemblage_ptr = (cxxPPassemblage *) use.Get_pp_assemblage_ptr();
		pp_assemblage_check(pp_assemblage_ptr);
	}
	if (use.Get_ss_assemblage_ptr() != NULL)
	{
		ss_assemblage_check(use.Get_ss_assemblage_ptr());
	}
/*
 *   Check that element moles are >= zero
 */
	if (solution_check() == MASS_BALANCE)
	{
		/* reset moles and deltas */
		if (use.Get_pp_assemblage_ptr() != NULL)
		{
			Rxn_pp_assemblage_map[pp_assemblage_save->Get_n_user()] = *pp_assemblage_save;
			use.Set_pp_assemblage_ptr(Utilities::Rxn_find(Rxn_pp_assemblage_map, pp_assemblage_save->Get_n_user()));
		}
		if (use.Get_ss_assemblage_ptr() != NULL)
		{
			Rxn_ss_assemblage_map[ss_assemblage_save->Get_n_user()] = *ss_assemblage_save;
			use.Set_ss_assemblage_ptr(Utilities::Rxn_find(Rxn_ss_assemblage_map, ss_assemblage_save->Get_n_user()));
		}
#ifdef SKIP
		if (use.Get_ss_assemblage_ptr() != NULL)
		{
			ss_assemblage_free(use.Get_ss_assemblage_ptr());
			ss_assemblage_copy(ss_assemblage_save, use.Get_ss_assemblage_ptr(),
								use.Get_ss_assemblage_ptr()->n_user);
			ss_assemblage_free(ss_assemblage_save);
			ss_assemblage_save =
				(struct ss_assemblage *)
				free_check_null(ss_assemblage_save);
		}
#endif
		return (MASS_BALANCE);
	}
/*
 *   Copy global into solution n_user = -1
 */
	xsolution_save(-1);
	step_save_surf(-1);
	step_save_exch(-1);
/*
 *   Clean up temporary space
 */
	if (pp_assemblage_save != NULL)
	{
		//pp_assemblage_free(pp_assemblage_save);
		//pp_assemblage_save =
		//	(struct pp_assemblage *) free_check_null(pp_assemblage_save);
		delete pp_assemblage_save;
		pp_assemblage_save = NULL;
	}
	//if (ss_assemblage_save != NULL)
	//{
	//	ss_assemblage_free(ss_assemblage_save);
	//	ss_assemblage_save =
	//		(struct ss_assemblage *) free_check_null(ss_assemblage_save);
	//}

	//
	// Solution -1 has sum of solution/mix, exchange, surface, gas_phase
	// reaction, kinetics
	// 
	// Determine system totals, calculate maximum mineral precipitation
	if (use.Get_pp_assemblage_in() || use.Get_ss_assemblage_in())
	{
		cxxStorageBin sys_bin(phrq_io);
		//cxxSolution soln(PHREEQC_THIS_COMMA -1, phrq_io);
		int n;
		struct solution *sol = solution_bsearch(-1, &n, false);
		cxxSolution soln(sol, phrq_io);
		sys_bin.Set_Solution(-1, soln);
		if (use.Get_pp_assemblage_in())
		{
			sys_bin.Set_PPassemblage(-1, use.Get_pp_assemblage_ptr());
		}
		if (use.Get_ss_assemblage_in())
		{
			sys_bin.Set_SSassemblage(-1, *use.Get_ss_assemblage_ptr());
		}
		sys_bin.Set_System(-1);
		sys_bin.Get_System().totalize(PHREEQC_THIS);
		cxxNameDouble sys_tots = sys_bin.Get_System().Get_Totals();
		if (use.Get_pp_assemblage_in())
		{
			cxxPPassemblage *pp_assemblage_ptr = sys_bin.Get_PPassemblage(-1);
			std::map<std::string, cxxPPassemblageComp>::iterator it;
			it =  pp_assemblage_ptr->Get_pp_assemblage_comps().begin();
			for ( ; it != pp_assemblage_ptr->Get_pp_assemblage_comps().end(); it++)
			{
				int n;
				struct phase *p_ptr = phase_bsearch((it->first).c_str(), &n, FALSE);
				struct elt_list *e_ptr;
				LDBLE min = 1e10;
				for (e_ptr = p_ptr->next_elt; e_ptr->elt != NULL; e_ptr++)
				{
					std::string e(e_ptr->elt->primary->elt->name);
					cxxNameDouble::iterator st = sys_tots.find(e.c_str());
					if (st != sys_tots.end())
					{
						LDBLE m1 = st->second / e_ptr->coef;
						if (m1 < min) min = m1;
					}
				}
				p_ptr->delta_max = min;
			}
		}
		if (use.Get_ss_assemblage_in())
		{
			cxxSSassemblage *ss_assemblage_ptr = sys_bin.Get_SSassemblage(-1);
			std::vector<cxxSS *> ss_ptrs = ss_assemblage_ptr->Vectorize();
			for (size_t j = 0; j < ss_ptrs.size(); j++)
			//for (it = ss->Get_ssAssemblageSSs().begin(); it != ss->Get_ssAssemblageSSs().end(); it++)
			{
				cxxSS * ss_ptr = ss_ptrs[j];
				for (size_t k = 0; k < ss_ptr->Get_ss_comps().size(); k++)
				{
					cxxSScomp *comp_ptr = &(ss_ptr->Get_ss_comps()[k]);
					int n;
					struct phase *p_ptr = phase_bsearch(comp_ptr->Get_name().c_str(), &n, FALSE);

					struct elt_list *e_ptr;
					LDBLE min = 1e10;
					for (e_ptr = p_ptr->next_elt; e_ptr->elt != NULL; e_ptr++)
					{
						std::string e(e_ptr->elt->primary->elt->name);
						cxxNameDouble::iterator st = sys_tots.find(e.c_str());
						if (st != sys_tots.end())
						{
							LDBLE m1 = st->second / e_ptr->coef;
							if (m1 < min) 
							{
								min = m1;
							}
						}
					}
					p_ptr->delta_max = min;
				}
			}
		}
	}

	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
xsolution_zero(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Zero out _x variables, master->totals, and species->la
 */
	int i;
/*
 *   Zero totals in master structures
 */
	new_x = FALSE;

	tc_x = 0.0;
	patm_x = 0;
	ph_x = 0.0;
	solution_pe_x = 0.0;
	mu_x = 0.0;
	ah2o_x = 0.0;
	density_x = 0.0;
	total_h_x = 0.0;
	total_o_x = 0.0;
	cb_x = 0.0;
	mass_water_aq_x = 0.0;
	units_x = moles_per_kilogram_string;

	for (i = 0; i < count_master; i++)
	{
		master[i]->total = 0.0;
		master[i]->total_primary = 0.0;
		master[i]->s->la = 0.0;
	}
	if (pitzer_model == TRUE || sit_model == TRUE)
	{
		for (i = 0; i < count_s; i++)
		{
			s[i]->lg = 0.0;
		}
	}
/*
 *   Copy pe data (not sure this will be used
 */
/*
	pe_data_free (pe_x);
	pe_x = pe_data_alloc();
 */
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
add_solution(struct solution *solution_ptr, LDBLE extensive, LDBLE intensive)
/* ---------------------------------------------------------------------- */
{
/*
 *   Accumulate solution data in master->totals and _x variables.
 *
 *   extensive is multiplication factor for solution
 *   intensive is fraction of all multiplication factors for all solutions
 */
	int i;
	struct master *master_ptr;
	struct species *species_ptr;
/*
 *   Add solution to global variables
 */
	tc_x += solution_ptr->tc * intensive;
	ph_x += solution_ptr->ph * intensive;
	patm_x += solution_ptr->patm * intensive;
	//
	//k_temp(tc_x, patm_x);
	solution_pe_x += solution_ptr->solution_pe * intensive;
	mu_x += solution_ptr->mu * intensive;
	ah2o_x += solution_ptr->ah2o * intensive;
	density_x += solution_ptr->density * intensive;

	total_h_x += solution_ptr->total_h * extensive;
	total_o_x += solution_ptr->total_o * extensive;
	cb_x += solution_ptr->cb * extensive;
	mass_water_aq_x += solution_ptr->mass_water * extensive;
/*
 *   Copy totals data into primary master species
 */
	for (i = 0; solution_ptr->totals[i].description != NULL; i++)
	{
		master_ptr =
			master_bsearch_primary(solution_ptr->totals[i].description);
		master_ptr->total += solution_ptr->totals[i].moles * extensive;
	}
/*
 *   Accumulate initial guesses for activities
 */
	/*for (i=0; solution_ptr->master_activity[i].description != NULL; i++) { */
	for (i = 0; i < solution_ptr->count_master_activity; i++)
	{
		if (solution_ptr->master_activity[i].description != NULL)
		{
			master_ptr =
				master_bsearch(solution_ptr->master_activity[i].description);
			if (master_ptr != NULL)
			{
				master_ptr->s->la +=
					solution_ptr->master_activity[i].la * intensive;
			}
		}
	}
/*
 *   Accumulate initial guesses for log gamma
 */
	if (pitzer_model == TRUE || sit_model == TRUE)
	{
		for (i = 0; i < solution_ptr->count_species_gamma; i++)
		{
			species_ptr =
				s_search(solution_ptr->species_gamma[i].description);
			if (species_ptr != NULL)
			{
				species_ptr->lg +=
					solution_ptr->species_gamma[i].la * intensive;
			}
		}
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
add_exchange(cxxExchange *exchange_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Accumulate exchange data in master->totals and _x variables.
 */
	struct master *master_ptr;

	if (exchange_ptr == NULL)
		return (OK);
/*
 *   Add element concentrations on exchanger to master species totals
 */
	std::vector<cxxExchComp *> comps = exchange_ptr->Vectorize();
	for (size_t i = 0; i < comps.size(); i++)
	{
		cxxExchComp *comp_ptr = comps[i];
		cxxNameDouble nd(comp_ptr->Get_totals());
		cxxNameDouble::iterator it = nd.begin();
		//for (j = 0; exchange_ptr->comps[i].totals[j].elt != NULL; j++)
		for ( ; it != nd.end(); it++)
		{
			struct element *elt_ptr = element_store(it->first.c_str());
			LDBLE coef = it->second;
			assert(elt_ptr != NULL && elt_ptr->primary != NULL);
			master_ptr = elt_ptr->primary;
			if (master_ptr->s == s_hplus)
			{
				total_h_x += coef;
			}
			else if (master_ptr->s == s_h2o)
			{
				total_o_x += coef;
			}
			else
			{
				master_ptr->total += coef;
			}
		}
	}
	if (exchange_ptr->Get_new_def())
	{
		for (int i = 0; i < count_master; i++)
		{
			if (master[i]->type == EX && master[i]->total > 0)
			{
				master[i]->s->la = log10(0.1 * master[i]->total);
			}
		}
	}
	else
	{
		for (size_t i = 0; i < comps.size(); i++)
		{
			cxxExchComp *comp_ptr = comps[i];
			cxxNameDouble nd(comp_ptr->Get_totals());
			cxxNameDouble::iterator it = nd.begin();
			//for (j = 0; exchange_ptr->comps[i].totals[j].elt != NULL; j++)
			for ( ; it != nd.end(); it++)
			{	
				struct element *elt_ptr = element_store(it->first.c_str());
				assert(elt_ptr->master);
				if (elt_ptr->master->type == EX)
				{
					elt_ptr->master->s->la = comps[i]->Get_la();
				}
			}

			//exchange_ptr->comps[i].master->s->la = exchange_ptr->comps[i].la;
			cb_x += comps[i]->Get_charge_balance();
		}
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
add_surface(struct surface *surface_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Accumulate surface data in master->totals and _x variables.
 */
	int i, j;
	struct master *master_ptr;

	if (surface_ptr == NULL)
		return (OK);
/*
 *   Add element concentrations on surface to master species totals
 */
	dl_type_x = surface_ptr->dl_type;
	for (i = 0; i < surface_ptr->count_comps; i++)
	{
		/*if(surface_ptr->edl == FALSE) { */
		if (surface_ptr->type == NO_EDL)
		{
			cb_x += surface_ptr->comps[i].cb;
		}
#ifdef SKIP_MUSIC
		if (surface_ptr->type == CD_MUSIC)
		{
			cb_x += surface_ptr->comps[i].cb;
		}
#endif
		if (surface_ptr->new_def == FALSE)
		{
			surface_ptr->comps[i].master->s->la = surface_ptr->comps[i].la;
		}
/*
 *   Add surface and specifically sorbed elements
 */
		for (j = 0; surface_ptr->comps[i].totals[j].elt != NULL; j++)
		{
			master_ptr = surface_ptr->comps[i].totals[j].elt->primary;
			if (master_ptr == NULL)
			{
				input_error++;
				error_string = sformatf( "Element not defined in database, %s.",
						surface_ptr->comps[i].totals[j].elt->name);
				error_msg(error_string, STOP);
			}
			if (master_ptr->s == s_hplus)
			{
				total_h_x += surface_ptr->comps[i].totals[j].coef;
			}
			else if (master_ptr->s == s_h2o)
			{
				total_o_x += surface_ptr->comps[i].totals[j].coef;
			}
			else
			{
				master_ptr->total += surface_ptr->comps[i].totals[j].coef;
			}
		}
	}
	/*if (surface_ptr->edl == FALSE) return(OK); */
	if (surface_ptr->type != DDL && surface_ptr->type != CD_MUSIC)
		return (OK);
	for (i = 0; i < surface_ptr->count_charge; i++)
	{
		/*if (surface_ptr->edl == TRUE) { */
		/*cb_x += surface_ptr->charge[i].charge_balance; */
		if (surface_ptr->type == DDL || surface_ptr->type == CD_MUSIC)
		{
			cb_x += surface_ptr->charge[i].charge_balance;
		}
		if (surface_ptr->new_def == FALSE)
		{
			master_ptr =
				surface_get_psi_master(surface_ptr->charge[i].name, SURF_PSI);
			master_ptr->s->la = surface_ptr->charge[i].la_psi;
			/*surface_ptr->charge[i].psi_master->s->la = surface_ptr->charge[i].la_psi; */
		}
/*
 *   Add diffuse layer elements (including water in Debye layer)
 */
		if (surface_ptr->dl_type != NO_DL && surface_ptr->new_def == FALSE)
		{
			for (j = 0;
				 surface_ptr->charge[i].diffuse_layer_totals[j].elt != NULL;
				 j++)
			{
				master_ptr =
					surface_ptr->charge[i].diffuse_layer_totals[j].elt->
					primary;
				if (master_ptr->s == s_hplus)
				{
					total_h_x +=
						surface_ptr->charge[i].diffuse_layer_totals[j].coef;
				}
				else if (master_ptr->s == s_h2o)
				{
					total_o_x +=
						surface_ptr->charge[i].diffuse_layer_totals[j].coef;
				}
				else
				{
					master_ptr->total +=
						surface_ptr->charge[i].diffuse_layer_totals[j].coef;
				}
			}
		}
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
add_mix(cxxMix *mix_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   calls add_solution to accumulate all data in master->totals
 *   and other variables.
 */
	int n;
	LDBLE sum_fractions, intensive, extensive;
	struct solution *solution_ptr;
	int count_positive;
	LDBLE sum_positive;

	if (mix_ptr == NULL)
		return (OK);
	if (mix_ptr->Get_mixComps().size() == 0)
		return (OK);
	sum_fractions = 0.0;
	sum_positive = 0.0;
	count_positive = 0;
	std::map<int, LDBLE>::const_iterator it;
	for (it = mix_ptr->Get_mixComps().begin(); it != mix_ptr->Get_mixComps().end(); it++)
	{
		sum_fractions += it->second;
		if (it->second > 0)
		{
			sum_positive += it->second;
			count_positive++;
		}
	}
	for (it = mix_ptr->Get_mixComps().begin(); it != mix_ptr->Get_mixComps().end(); it++)
	{
		solution_ptr =	solution_bsearch(it->first, &n, TRUE);
		if (solution_ptr == NULL)
		{
			input_error++;
			continue;
		}
		extensive = it->second;
		intensive = extensive / sum_fractions;
		if (count_positive < (int) mix_ptr->Get_mixComps().size())
		{
			if (it->second > 0)
			{
				intensive = extensive / sum_positive;
			}
			else
			{
				intensive = 0;
			}
		}
		add_solution(solution_ptr, extensive, intensive);
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
add_pp_assemblage(cxxPPassemblage *pp_assemblage_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Add a small amount of each phase if necessary to insure
 *   all elements exist in solution.
 */
	int i;
	LDBLE amount_to_add, total;
	char token[MAX_LENGTH];
	char *ptr;
	//struct pure_phase *pure_phase_ptr;
	struct master *master_ptr;
	//struct phase *phase_ptr;
	//LDBLE p, t;

	//pure_phase_ptr = pp_assemblage_ptr->pure_phases;

	// Should be be done when it gets to prep (modified quick_setup in case same_model).
	//for (j = 0; j < pp_assemblage_ptr->count_comps; j++)
	//{
	//	phase_ptr = pure_phase_ptr[j].phase;
	//	if (/*pure_phase_ptr[j].si_org > 0 && */phase_ptr->p_c > 0 && phase_ptr->t_c > 0)
	//	{
	//		p = exp(pure_phase_ptr[j].si_org * LOG_10);
	//		t = tc_x + 273.15;
	//		if (!phase_ptr->pr_in || p != phase_ptr->pr_p || t != phase_ptr->pr_tk)
	//		{
	//			calc_PR(&phase_ptr, 1, p, t, 0);
	//		}
	//		pure_phase_ptr[j].si = pure_phase_ptr[j].si_org + phase_ptr->pr_si_f;
	//	}
	//}

	if (check_pp_assemblage(pp_assemblage_ptr) == OK)
		return (OK);
/*
 *   Go through list and generate list of elements and
 *   coefficient of elements in reaction
 */
	count_elts = 0;
	paren_count = 0;
/*
 *   Check that all elements are in solution for phases with greater than zero mass
 */
	std::map<std::string, cxxPPassemblageComp>::iterator it;
	it =  pp_assemblage_ptr->Get_pp_assemblage_comps().begin();
	for ( ; it != pp_assemblage_ptr->Get_pp_assemblage_comps().end(); it++)
	{
		cxxPPassemblageComp * comp_ptr = &(it->second);
		if (comp_ptr->Get_precipitate_only()) continue;
		int l;
		struct phase * phase_ptr = phase_bsearch(it->first.c_str(), &l, FALSE);
		count_elts = 0;
		paren_count = 0;
		amount_to_add = 0.0;
		comp_ptr->Set_delta(0.0);
		if (comp_ptr->Get_add_formula().size() > 0)
		{
			strcpy(token, comp_ptr->Get_add_formula().c_str());
			ptr = &(token[0]);
			get_elts_in_species(&ptr, 1.0);
		}
		else
		{
			strcpy(token, phase_ptr->formula);
			add_elt_list(phase_ptr->next_elt, 1.0);
		}
		if (comp_ptr->Get_moles() > 0.0)
		{
			for (i = 0; i < count_elts; i++)
			{
				master_ptr = elt_list[i].elt->primary;
				if (master_ptr->s == s_hplus)
				{
					continue;
				}
				else if (master_ptr->s == s_h2o)
				{
					continue;
				}
				else if (master_ptr->total > MIN_TOTAL)
				{
					continue;
				}
				else
				{
					total = (-master_ptr->total + 1e-10) / elt_list[i].coef;
					if (amount_to_add < total)
					{
						amount_to_add = total;
					}
				}
			}
			if (comp_ptr->Get_moles() < amount_to_add)
			{
				amount_to_add =comp_ptr->Get_moles();
			}
		}
		if (amount_to_add > 0.0)
		{
			comp_ptr->Set_moles(comp_ptr->Get_moles() - amount_to_add);
			comp_ptr->Set_delta(amount_to_add);
/*
 *   Add reaction to totals
 */
			for (i = 0; i < count_elts; i++)
			{
				master_ptr = elt_list[i].elt->primary;
				if (master_ptr->s == s_hplus)
				{
					total_h_x += elt_list[i].coef * amount_to_add;
				}
				else if (master_ptr->s == s_h2o)
				{
					total_o_x += elt_list[i].coef * amount_to_add;
				}
				else
				{
					master_ptr->total += elt_list[i].coef * amount_to_add;
				}
			}
		}
	}
	return (OK);
}
#ifdef SKIP
/* ---------------------------------------------------------------------- */
int Phreeqc::
add_pp_assemblage(struct pp_assemblage *pp_assemblage_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Add a small amount of each phase if necessary to insure
 *   all elements exist in solution.
 */
	int i, j;
	LDBLE amount_to_add, total;
	char token[MAX_LENGTH];
	char *ptr;
	struct pure_phase *pure_phase_ptr;
	struct master *master_ptr;
	//struct phase *phase_ptr;
	//LDBLE p, t;

	pure_phase_ptr = pp_assemblage_ptr->pure_phases;

	// Should be be done when it gets to prep (modified quick_setup in case same_model).
	//for (j = 0; j < pp_assemblage_ptr->count_comps; j++)
	//{
	//	phase_ptr = pure_phase_ptr[j].phase;
	//	if (/*pure_phase_ptr[j].si_org > 0 && */phase_ptr->p_c > 0 && phase_ptr->t_c > 0)
	//	{
	//		p = exp(pure_phase_ptr[j].si_org * LOG_10);
	//		t = tc_x + 273.15;
	//		if (!phase_ptr->pr_in || p != phase_ptr->pr_p || t != phase_ptr->pr_tk)
	//		{
	//			calc_PR(&phase_ptr, 1, p, t, 0);
	//		}
	//		pure_phase_ptr[j].si = pure_phase_ptr[j].si_org + phase_ptr->pr_si_f;
	//	}
	//}

	if (check_pp_assemblage(pp_assemblage_ptr) == OK)
		return (OK);
/*
 *   Go through list and generate list of elements and
 *   coefficient of elements in reaction
 */
	count_elts = 0;
	paren_count = 0;
/*
 *   Check that all elements are in solution for phases with greater than zero mass
 */
	pure_phase_ptr = pp_assemblage_ptr->pure_phases;
	for (j = 0; j < pp_assemblage_ptr->count_comps; j++)
	{
		if (pure_phase_ptr[j].precipitate_only) continue;
		count_elts = 0;
		paren_count = 0;
		amount_to_add = 0.0;
		pure_phase_ptr[j].delta = 0.0;
		if (pure_phase_ptr[j].add_formula != NULL)
		{
			strcpy(token, pure_phase_ptr[j].add_formula);
			ptr = &(token[0]);
			get_elts_in_species(&ptr, 1.0);
		}
		else
		{
			strcpy(token, pure_phase_ptr[j].phase->formula);
			add_elt_list(pure_phase_ptr[j].phase->next_elt, 1.0);
		}
		if (pure_phase_ptr[j].moles > 0.0)
		{
			for (i = 0; i < count_elts; i++)
			{
				master_ptr = elt_list[i].elt->primary;
				if (master_ptr->s == s_hplus)
				{
					continue;
				}
				else if (master_ptr->s == s_h2o)
				{
					continue;
				}
				else if (master_ptr->total > MIN_TOTAL)
				{
					continue;
				}
				else
				{
					total = (-master_ptr->total + 1e-10) / elt_list[i].coef;
					if (amount_to_add < total)
					{
						amount_to_add = total;
					}
				}
			}
			if (pure_phase_ptr[j].moles < amount_to_add)
			{
				amount_to_add = pure_phase_ptr[j].moles;
			}
		}
		if (amount_to_add > 0.0)
		{
			pure_phase_ptr[j].moles -= amount_to_add;
			pure_phase_ptr[j].delta = amount_to_add;
/*
 *   Add reaction to totals
 */
			for (i = 0; i < count_elts; i++)
			{
				master_ptr = elt_list[i].elt->primary;
				if (master_ptr->s == s_hplus)
				{
					total_h_x += elt_list[i].coef * amount_to_add;
				}
				else if (master_ptr->s == s_h2o)
				{
					total_o_x += elt_list[i].coef * amount_to_add;
				}
				else
				{
					master_ptr->total += elt_list[i].coef * amount_to_add;
				}
			}
		}
	}
	return (OK);
}
#endif
/* ---------------------------------------------------------------------- */
 int Phreeqc::
check_pp_assemblage(cxxPPassemblage *pp_assemblage_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Check list of all elements in pure_phase assemblage to see
 *   if all are in model. Return true if all are present,
 *   Return false if one or more is missing.
 */
	struct master *master_ptr;

	cxxNameDouble nd = pp_assemblage_ptr->Get_eltList();
	cxxNameDouble::iterator it;
	for (it = nd.begin(); it != nd.end(); it++)
	{
		struct element *elt_ptr = element_store(it->first.c_str());
		if (elt_ptr == NULL || elt_ptr->primary == NULL)
		{
			return FALSE;
		}

		master_ptr = elt_ptr->primary;
		if (master_ptr->s == s_h2o || master_ptr->s == s_hplus)
			continue;
		if (master_ptr->total > MIN_TOTAL)
			continue;
		return (FALSE);
	}
	return (TRUE);
}
#ifdef SKIP
/* ---------------------------------------------------------------------- */
 int Phreeqc::
check_pp_assemblage(struct pp_assemblage *pp_assemblage_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Check list of all elements in pure_phase assemblage to see
 *   if all are in model. Return true if all are present,
 *   Return false if one or more is missing.
 */
	int j;
	struct master *master_ptr;
	for (j = 0; pp_assemblage_ptr->next_elt[j].elt != NULL; j++)
	{
		master_ptr = pp_assemblage_ptr->next_elt[j].elt->primary;
		if (master_ptr->s == s_h2o || master_ptr->s == s_hplus)
			continue;
		if (master_ptr->total > MIN_TOTAL)
			continue;
		return (FALSE);
	}
	return (TRUE);
}
#endif
 /* ---------------------------------------------------------------------- */
int Phreeqc::
add_reaction(cxxReaction *reaction_ptr, int step_number, LDBLE step_fraction)
/* ---------------------------------------------------------------------- */
{
/*
 *   Add irreversible reaction
 */
	char c;
	struct master *master_ptr;
/*
 *   Calculate and save reaction
 */
/* !!!!! with kinetics reaction, coeff's may change 
 *       and reaction_calc must be called ....
 */
	if (reaction_ptr == NULL)
		return OK;

	reaction_calc(reaction_ptr);

/*
 *   Step size
 */
	if (incremental_reactions == FALSE)
	{
		//if (irrev_ptr->count_steps > 0)
		if (!reaction_ptr->Get_equalIncrements() && reaction_ptr->Get_steps().size()> 0 )
		{
			if (step_number > (int) reaction_ptr->Get_steps().size())
			{
				step_x = reaction_ptr->Get_steps()[reaction_ptr->Get_steps().size() - 1];
			}
			else
			{
				step_x = reaction_ptr->Get_steps()[step_number - 1];
			}
		}
		else if (reaction_ptr->Get_equalIncrements() && reaction_ptr->Get_steps().size()> 0)
		{
			if (step_number > (int) reaction_ptr->Get_actualSteps())
			{
				step_x = reaction_ptr->Get_steps()[0];
			}
			else
			{
				step_x = reaction_ptr->Get_steps()[0] *
					((LDBLE) step_number) /
					((LDBLE) (reaction_ptr->Get_actualSteps()));
			}
		}
		else
		{
			step_x = 0.0;
		}
	}
	else
	{
		/* Incremental reactions */
		if (!reaction_ptr->Get_equalIncrements() && reaction_ptr->Get_steps().size()> 0)
		{
			if (step_number > (int) reaction_ptr->Get_actualSteps())
			{
				step_x = reaction_ptr->Get_steps()[reaction_ptr->Get_actualSteps() - 1];
			}
			else
			{
				step_x = reaction_ptr->Get_steps()[step_number - 1];
			}
		}
		else if (reaction_ptr->Get_equalIncrements() && reaction_ptr->Get_steps().size()> 0)
		{
			if (step_number > (int) reaction_ptr->Get_actualSteps())
			{
				step_x = 0;
			}
			else
			{
				step_x = reaction_ptr->Get_steps()[0] / ((LDBLE) (reaction_ptr->Get_actualSteps()));
			}
		}
		else
		{
			step_x = 0.0;
		}
	}
/*
 *   Convert units
 */
	//c = irrev_ptr->units[0];
	c = reaction_ptr->Get_units().c_str()[0];
	if (c == 'm')
	{
		step_x *= 1e-3;
	}
	else if (c == 'u')
	{
		step_x *= 1e-6;
	}
	else if (c == 'n')
	{
		step_x *= 1e-9;
	}
/*
 *   Add reaction to totals
 */
	//for (i = 0; irrev_ptr->elts[i].elt != NULL; i++)
	cxxNameDouble::const_iterator it = reaction_ptr->Get_elementList().begin();
	for ( ; it != reaction_ptr->Get_elementList().end(); it++)
	{
		struct element * elt_ptr = element_store(it->first.c_str());
		LDBLE coef = it->second;
		if (elt_ptr == NULL)
		{
			assert (false);
		}
		master_ptr = elt_ptr->primary;
		if (master_ptr == NULL)
		{
			assert (false);
		}
		if (master_ptr->s == s_hplus)
		{
			total_h_x += coef * step_x * step_fraction;
		}
		else if (master_ptr->s == s_h2o)
		{
			total_o_x += coef * step_x * step_fraction;
		}
		else
		{
			master_ptr->total += coef * step_x * step_fraction;
		}
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
reaction_calc(cxxReaction *reaction_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *    Go through irreversible reaction initially to
 *    determine a list of elements and amounts in 
 *    the reaction.
 */
	//int i, j, 
	int return_value;
	LDBLE coef;
	//char token[MAX_LENGTH];
	char *ptr;
	struct phase *phase_ptr;
/*
 *   Go through list and generate list of elements and
 *   coefficient of elements in reaction
 */
	return_value = OK;
	count_elts = 0;
	paren_count = 0;

	cxxNameDouble nd(reaction_ptr->Get_reactantList());
	cxxNameDouble::iterator it;
	for (it = nd.begin(); it != nd.end(); it++)
	{
		coef = it->second;
		int j;
		phase_ptr = phase_bsearch(it->first.c_str(), &j, FALSE);
/*
 *   Reactant is a pure phase, copy formula into token
 */
		if (phase_ptr != NULL)
		{
			add_elt_list(phase_ptr->next_elt, coef);
		}
		else
		{
			//ptr = &(token[0]);
			char * token = string_duplicate(it->first.c_str());
			ptr = token;
			get_elts_in_species(&ptr, coef);
			free_check_null(token);
		}
	}
/*
 *   Check that all elements are in database
 */
	for (int i = 0; i < count_elts; i++)
	{
		if (elt_list[i].elt->master == NULL)
		{
			error_string = sformatf(
					"Element or phase not defined in database, %s.",
					elt_list[i].elt->name);
			error_msg(error_string, CONTINUE);
			input_error++;
			return_value = ERROR;
		}
	}
	//irrev_ptr->elts = elt_list_save();
	reaction_ptr->Set_elementList(elt_list_NameDouble());

	return (return_value);
}
 /* ---------------------------------------------------------------------- */
int Phreeqc::
add_gas_phase(cxxGasPhase *gas_phase_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Accumulate gas data in master->totals and _x variables.
 */
	int i;

	//struct gas_comp *gas_comp_ptr;
	struct master *master_ptr;

	if (gas_phase_ptr == NULL)
		return (OK);
	//gas_comp_ptr = gas_phase_ptr->comps;
/*
 *   calculate reaction
 */
	count_elts = 0;
	paren_count = 0;
	//for (i = 0; i < gas_phase_ptr->count_comps; i++)
	for (size_t i = 0; i < gas_phase_ptr->Get_gas_comps().size(); i++)
	{
		cxxGasComp *gc_ptr = &(gas_phase_ptr->Get_gas_comps()[i]);
		int k;
		struct phase *phase_ptr = phase_bsearch(gc_ptr->Get_phase_name().c_str() , &k, FALSE);
		assert(phase_ptr);

		add_elt_list(phase_ptr->next_elt, gc_ptr->Get_moles());
	}
/*
 *   Sort elements in reaction and combine
 */
	if (count_elts > 0)
	{
		qsort(elt_list, (size_t) count_elts,
			  (size_t) sizeof(struct elt_list), elt_list_compare);
		elt_list_combine();
	}
/*
 *   Add gas elements to totals
 */
	for (i = 0; i < count_elts; i++)
	{
		master_ptr = elt_list[i].elt->primary;
		if (master_ptr->s == s_hplus)
		{
			total_h_x += elt_list[i].coef;
		}
		else if (master_ptr->s == s_h2o)
		{
			total_o_x += elt_list[i].coef;
		}
		else
		{
			master_ptr->total += elt_list[i].coef;
		}
	}
	if (gas_phase_ptr->Get_type() == cxxGasPhase::GP_PRESSURE && fabs(gas_phase_ptr->Get_total_p() - patm_x) > 0.01)
	{
		same_pressure = FALSE;
		patm_x = gas_phase_ptr->Get_total_p();
		k_temp(tc_x, patm_x);
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
add_ss_assemblage(cxxSSassemblage *ss_assemblage_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Accumulate solid_solution data in master->totals and _x variables.
 */
	int i, j, k;
	LDBLE amount_to_add, total;
	//struct s_s *s_s_ptr;
	struct master *master_ptr;
	//char token[MAX_LENGTH];
	char *ptr;

	if (ss_assemblage_ptr == NULL)
		return (OK);
	count_elts = 0;
	paren_count = 0;
/*
 *   Check that all elements are in solution for phases with greater than zero mass
 */
	std::vector<cxxSS *> ss_ptrs = ss_assemblage_ptr->Vectorize();
	for (i = 0; i < (int) ss_ptrs.size(); i++)
	{
		cxxSS * ss_ptr = ss_ptrs[i];
		count_elts = 0;
		paren_count = 0;
		for (j = 0; j < (int) ss_ptr->Get_ss_comps().size(); j++)
		{
			cxxSScomp *comp_ptr = &(ss_ptr->Get_ss_comps()[j]);
			int l;
			struct phase * phase_ptr = phase_bsearch(comp_ptr->Get_name().c_str(), &l, FALSE);

			amount_to_add = 0.0;
			comp_ptr->Set_delta(0.0);
			if (comp_ptr->Get_moles() > 0.0)
			{
				//strcpy(token, s_s_ptr->comps[j].phase->formula);
				char * token = string_duplicate(phase_ptr->formula);
				ptr = &(token[0]);
				get_elts_in_species(&ptr, 1.0);
				free_check_null(token);
				for (k = 0; k < count_elts; k++)
				{
					master_ptr = elt_list[k].elt->primary;
					if (master_ptr->s == s_hplus)
					{
						continue;
					}
					else if (master_ptr->s == s_h2o)
					{
						continue;
					}
					else if (master_ptr->total > MIN_TOTAL_SS)
					{
						continue;
					}
					else
					{
						total =
							(-master_ptr->total + 1e-10) / elt_list[k].coef;
						if (amount_to_add < total)
						{
							amount_to_add = total;
						}
					}
				}
			}
			if (comp_ptr->Get_moles() < amount_to_add)
			{
				amount_to_add = comp_ptr->Get_moles();
			}
			if (amount_to_add > 0.0)
			{
				comp_ptr->Set_moles(comp_ptr->Get_moles() - amount_to_add);
				comp_ptr->Set_delta(amount_to_add);
/*
 *   Add reaction to totals
 */
				for (k = 0; k < count_elts; k++)
				{
					master_ptr = elt_list[k].elt->primary;
					if (master_ptr->s == s_hplus)
					{
						total_h_x += elt_list[k].coef * amount_to_add;
					}
					else if (master_ptr->s == s_h2o)
					{
						total_o_x += elt_list[k].coef * amount_to_add;
					}
					else
					{
						master_ptr->total += elt_list[k].coef * amount_to_add;
					}
				}
			}
		}
	}
	return (OK);
}
#ifdef SKIP
/* ---------------------------------------------------------------------- */
int Phreeqc::
add_ss_assemblage(cxxSSassemblage *ss_assemblage_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Accumulate solid_solution data in master->totals and _x variables.
 */
	int i, j, k;
	LDBLE amount_to_add, total;
	struct s_s *s_s_ptr;
	struct master *master_ptr;
	char token[MAX_LENGTH];
	char *ptr;

	if (ss_assemblage_ptr == NULL)
		return (OK);
	count_elts = 0;
	paren_count = 0;
/*
 *   Check that all elements are in solution for phases with greater than zero mass
 */
	for (i = 0; i < ss_assemblage_ptr->count_s_s; i++)
	{
		count_elts = 0;
		paren_count = 0;
		s_s_ptr = &(ss_assemblage_ptr->s_s[i]);
		for (j = 0; j < s_s_ptr->count_comps; j++)
		{
			amount_to_add = 0.0;
			s_s_ptr->comps[j].delta = 0.0;
			if (s_s_ptr->comps[j].moles > 0.0)
			{
				strcpy(token, s_s_ptr->comps[j].phase->formula);
				ptr = &(token[0]);
				get_elts_in_species(&ptr, 1.0);
				for (k = 0; k < count_elts; k++)
				{
					master_ptr = elt_list[k].elt->primary;
					if (master_ptr->s == s_hplus)
					{
						continue;
					}
					else if (master_ptr->s == s_h2o)
					{
						continue;
					}
					else if (master_ptr->total > MIN_TOTAL_SS)
					{
						continue;
					}
					else
					{
						total =
							(-master_ptr->total + 1e-10) / elt_list[k].coef;
						if (amount_to_add < total)
						{
							amount_to_add = total;
						}
					}
				}
			}
			if (s_s_ptr->comps[j].moles < amount_to_add)
			{
				amount_to_add = s_s_ptr->comps[j].moles;
			}
			if (amount_to_add > 0.0)
			{
				s_s_ptr->comps[j].moles -= amount_to_add;
				s_s_ptr->comps[j].delta = amount_to_add;
/*
 *   Add reaction to totals
 */
				for (k = 0; k < count_elts; k++)
				{
					master_ptr = elt_list[k].elt->primary;
					if (master_ptr->s == s_hplus)
					{
						total_h_x += elt_list[k].coef * amount_to_add;
					}
					else if (master_ptr->s == s_h2o)
					{
						total_o_x += elt_list[k].coef * amount_to_add;
					}
					else
					{
						master_ptr->total += elt_list[k].coef * amount_to_add;
					}
				}
			}
		}
	}
	return (OK);
}
#endif
/* ---------------------------------------------------------------------- */
int Phreeqc::
add_kinetics(struct kinetics *kinetics_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Add kinetic reaction
 */
	int i;
	struct master *master_ptr;
/*
 *   Add reaction to totals
 */
	if (kinetics_ptr->totals == NULL)
		return (OK);
	for (i = 0; kinetics_ptr->totals[i].elt != NULL; i++)
	{
		master_ptr = kinetics_ptr->totals[i].elt->primary;
		if (master_ptr == NULL)
		{
			input_error++;
			error_string = sformatf(
					"Element %s in kinetic reaction not found in database.",
					kinetics_ptr->totals[i].elt->name);
			error_msg(error_string, STOP);
		}
		if (master_ptr->s == s_hplus)
		{
			total_h_x += kinetics_ptr->totals[i].coef;
		}
		else if (master_ptr->s == s_h2o)
		{
			total_o_x += kinetics_ptr->totals[i].coef;
		}
		else
		{
			master_ptr->total += kinetics_ptr->totals[i].coef;
		}
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
gas_phase_check(cxxGasPhase *gas_phase_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Check for missing elements
 */
	//int i, j;

	//struct gas_comp *gas_comp_ptr;
	struct master *master_ptr;

	if (gas_phase_ptr == NULL)
		return (OK);
	//gas_comp_ptr = gas_phase_ptr->comps;
/*
 *   Check that all elements are in solution for phases with zero mass
 */
	for (size_t i = 0; i < gas_phase_ptr->Get_gas_comps().size(); i++)
	//for (i = 0; i < gas_phase_ptr->count_comps; i++)
	{
		cxxGasComp *gc_ptr = &(gas_phase_ptr->Get_gas_comps()[i]);
		int k;
		struct phase *phase_ptr = phase_bsearch(gc_ptr->Get_phase_name().c_str() , &k, FALSE);
		count_elts = 0;
		paren_count = 0;
		if (gc_ptr->Get_moles() <= 0.0)
		{
			add_elt_list(phase_ptr->next_elt, 1.0);
			for (int j = 0; j < count_elts; j++)
			{
				master_ptr = elt_list[j].elt->primary;
				if (master_ptr->s == s_hplus)
				{
					continue;
				}
				else if (master_ptr->s == s_h2o)
				{
					continue;
				}
				else if (master_ptr->total > MIN_TOTAL)
				{
					continue;
				}
				else
				{
					if (state != ADVECTION && state != TRANSPORT
						&& state != PHAST)
					{
						error_string = sformatf(
								"Element %s is contained in gas %s (which has 0.0 mass),\nbut is not in solution or other phases.",
								elt_list[j].elt->name,
								phase_ptr->name);
						warning_msg(error_string);
					}
				}
			}
		}
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
pp_assemblage_check(cxxPPassemblage *pp_assemblage_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Check for missing elements
 */
	//int i, j, k;
	//char token[MAX_LENGTH];
	std::string token;
	char *ptr;
	//struct pure_phase *pure_phase_ptr;
	struct master *master_ptr;

	if (check_pp_assemblage(pp_assemblage_ptr) == OK)
		return (OK);
/*
 *   Check that all elements are in solution for phases with zero mass
 */
	std::map<std::string, cxxPPassemblageComp>::iterator it;
	it =  pp_assemblage_ptr->Get_pp_assemblage_comps().begin();
	for ( ; it != pp_assemblage_ptr->Get_pp_assemblage_comps().end(); it++)
	{
		cxxPPassemblageComp * comp_ptr = &(it->second);
		int l;
		struct phase * phase_ptr = phase_bsearch(it->first.c_str(), &l, FALSE);
		count_elts = 0;
		paren_count = 0;
		if (comp_ptr->Get_moles() <= 0.0)
		{
			comp_ptr->Set_delta(0.0);
			if (comp_ptr->Get_add_formula().size() > 0)
			{
				token = comp_ptr->Get_add_formula();
				ptr = &(token[0]);
				get_elts_in_species(&ptr, 1.0);
			}
			else
			{
				token = phase_ptr->formula;
				//strcpy(token, pure_phase_ptr[j].phase->formula);
				add_elt_list(phase_ptr->next_elt, 1.0);
			}
			for (int i = 0; i < count_elts; i++)
			{
				master_ptr = elt_list[i].elt->primary;
				if (master_ptr->s == s_hplus)
				{
					continue;
				}
				else if (master_ptr->s == s_h2o)
				{
					continue;
				}
				else if (master_ptr->total > MIN_TOTAL)
				{
					continue;
				}
				else
				{
					if (state != ADVECTION && state != TRANSPORT
						&& state != PHAST)
					{
						error_string = sformatf(
								"Element %s is contained in %s (which has 0.0 mass),"
								"\t\nbut is not in solution or other phases.",
								elt_list[i].elt->name,
								phase_ptr->name);
						warning_msg(error_string);
					}
/*
 *   Make la's of all master species for the element small, so SI will be small
 *   and no mass transfer will be calculated
 */
					for (int k = 0; k < count_master; k++)
					{
						if (master[k]->elt->primary == master_ptr)
						{
							master[k]->s->la = -9999.999;
						}
					}
				}
			}
		}
	}
	return (OK);
}
#ifdef SKIP
/* ---------------------------------------------------------------------- */
int Phreeqc::
pp_assemblage_check(struct pp_assemblage *pp_assemblage_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Check for missing elements
 */
	int i, j, k;
	char token[MAX_LENGTH];
	char *ptr;
	struct pure_phase *pure_phase_ptr;
	struct master *master_ptr;

	if (check_pp_assemblage(pp_assemblage_ptr) == OK)
		return (OK);
/*
 *   Check that all elements are in solution for phases with zero mass
 */
	pure_phase_ptr = pp_assemblage_ptr->pure_phases;
	for (j = 0; j < pp_assemblage_ptr->count_comps; j++)
	{
		count_elts = 0;
		paren_count = 0;
		if (pure_phase_ptr[j].moles <= 0.0)
		{
			pure_phase_ptr[j].delta = 0.0;
			if (pure_phase_ptr[j].add_formula != NULL)
			{
				strcpy(token, pure_phase_ptr[j].add_formula);
				ptr = &(token[0]);
				get_elts_in_species(&ptr, 1.0);
			}
			else
			{
				strcpy(token, pure_phase_ptr[j].phase->formula);
				add_elt_list(pure_phase_ptr[j].phase->next_elt, 1.0);
			}
			for (i = 0; i < count_elts; i++)
			{
				master_ptr = elt_list[i].elt->primary;
				if (master_ptr->s == s_hplus)
				{
					continue;
				}
				else if (master_ptr->s == s_h2o)
				{
					continue;
				}
				else if (master_ptr->total > MIN_TOTAL)
				{
					continue;
				}
				else
				{
					if (state != ADVECTION && state != TRANSPORT
						&& state != PHAST)
					{
						error_string = sformatf(
								"Element %s is contained in %s (which has 0.0 mass),"
								"\t\nbut is not in solution or other phases.",
								elt_list[i].elt->name,
								pure_phase_ptr[j].phase->name);
						warning_msg(error_string);
					}
/*
 *   Make la's of all master species for the element small, so SI will be small
 *   and no mass transfer will be calculated
 */
					for (k = 0; k < count_master; k++)
					{
						if (master[k]->elt->primary == master_ptr)
						{
							master[k]->s->la = -9999.999;
						}
					}
				}
			}
		}
	}
	return (OK);
}
#endif

/* ---------------------------------------------------------------------- */
int Phreeqc::
ss_assemblage_check(cxxSSassemblage *ss_assemblage_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Check for missing elements
 */
	int j, k;
	struct master *master_ptr;

	if (ss_assemblage_ptr == NULL)
		return (OK);
/*
 *   Check that all elements are in solution for phases with zero mass
 */
	std::vector<cxxSS *> ss_ptrs = ss_assemblage_ptr->Vectorize();
	for (j = 0; j < (int) ss_ptrs.size(); j++)
	{
		cxxSS * ss_ptr = ss_ptrs[j];
		for (k = 0; k < (int) ss_ptr->Get_ss_comps().size(); k++)
		{
			cxxSScomp *comp_ptr = &(ss_ptr->Get_ss_comps()[k]);
			int l;
			struct phase *phase_ptr = phase_bsearch(comp_ptr->Get_name().c_str(), &l, FALSE);
			count_elts = 0;
			paren_count = 0;
			if (comp_ptr->Get_moles() <= 0.0)
			{
				add_elt_list(phase_ptr->next_elt, 1.0);
				for (l = 0; l < count_elts; l++)
				{
					master_ptr = elt_list[l].elt->primary;
					if (master_ptr->s == s_hplus)
					{
						continue;
					}
					else if (master_ptr->s == s_h2o)
					{
						continue;
					}
					else if (master_ptr->total > MIN_TOTAL_SS)
					{
						continue;
					}
					else
					{
						if (state != ADVECTION && state != TRANSPORT
							&& state != PHAST)
						{
							error_string = sformatf(
									"Element %s is contained in solid solution %s (which has 0.0 mass),\nbut is not in solution or other phases.",
									elt_list[l].elt->name,
									phase_ptr->name);
							warning_msg(error_string);
						}
					}
					/*
					 *   Make la's of all master species for the element small, 
					 *   so SI will be small
					 *   and no mass transfer will be calculated
					 */
					for (k = 0; k < count_master; k++)
					{
						if (master[k]->elt->primary == master_ptr)
						{
							master[k]->s->la = -9999.999;
						}
					}
				}
			}
		}
	}
	return (OK);
}
#ifdef SKIP
/* ---------------------------------------------------------------------- */
int Phreeqc::
ss_assemblage_check(struct ss_assemblage *ss_assemblage_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Check for missing elements
 */
	int i, j, k, l;
	struct master *master_ptr;

	if (ss_assemblage_ptr == NULL)
		return (OK);
/*
 *   Check that all elements are in solution for phases with zero mass
 */
	for (i = 0; i < ss_assemblage_ptr->count_s_s; i++)
	{
		for (j = 0; j < ss_assemblage_ptr->s_s[i].count_comps; j++)
		{
			count_elts = 0;
			paren_count = 0;
			if (ss_assemblage_ptr->s_s[i].comps[j].moles <= 0.0)
			{
				add_elt_list(ss_assemblage_ptr->s_s[i].comps[j].phase->
							 next_elt, 1.0);
				for (l = 0; l < count_elts; l++)
				{
					master_ptr = elt_list[l].elt->primary;
					if (master_ptr->s == s_hplus)
					{
						continue;
					}
					else if (master_ptr->s == s_h2o)
					{
						continue;
					}
					else if (master_ptr->total > MIN_TOTAL_SS)
					{
						continue;
					}
					else
					{
						if (state != ADVECTION && state != TRANSPORT
							&& state != PHAST)
						{
							error_string = sformatf(
									"Element %s is contained in solid solution %s (which has 0.0 mass),\nbut is not in solution or other phases.",
									elt_list[l].elt->name,
									ss_assemblage_ptr->s_s[i].comps[j].
									phase->name);
							warning_msg(error_string);
						}
					}
					/*
					 *   Make la's of all master species for the element small, 
					 *   so SI will be small
					 *   and no mass transfer will be calculated
					 */
					for (k = 0; k < count_master; k++)
					{
						if (master[k]->elt->primary == master_ptr)
						{
							master[k]->s->la = -9999.999;
						}
					}
				}
			}
		}
	}
	return (OK);
}
#endif
/* ---------------------------------------------------------------------- */
int Phreeqc::
solution_check(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Check for missing elements
 */
	int i;
	struct master *master_ptr;

/*
 *   Check that all elements are in solution for phases with zero mass
 */
	for (i = 0; i < count_master; i++)
	{
		master_ptr = master[i];
		if (master_ptr->total >= 0.0)
			continue;
		if (master_ptr->total > -MIN_TOTAL)
		{
			master_ptr->total = 0;
			continue;
		}
		if (master_ptr->s == s_eminus || master_ptr->s == s_h2o
			|| master_ptr->s == s_hplus || master_ptr->s == s_h3oplus)
		{
			master_ptr->total = 0;
			continue;
		}
		/*
		   sprintf (error_string,
		   "Element %s has negative moles in solution, %e. \n\tErroneous mole balance occurs as moles are added to produce zero moles.\n\tUsually caused by KINETICS, REACTION, or diffuse layer calculation.\n\tMay be due to large time steps in early part of KINETICS simulation or negative concentrations in the diffuse layer.",
		   master_ptr->elt->name, (LDBLE) master_ptr->total);
		 */
		error_string = sformatf(
				"Negative moles in solution for %s, %e. Recovering...",
				master_ptr->elt->name, (double) master_ptr->total);
		warning_msg(error_string);
		return (MASS_BALANCE);
	}
	return (OK);
}
