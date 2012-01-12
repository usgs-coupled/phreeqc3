#include "Utils.h"
#include "Phreeqc.h"
#include "phqalloc.h"
#include <vector>
#include <assert.h>
#include "Exchange.h"
#include "GasPhase.h"
#include "PPassemblage.h"
/* ---------------------------------------------------------------------- */
int Phreeqc::
prep(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Input is model defined by the structure use.
 *   Most of routine is skipped if model, as defined by master.total
 *      plus use.pure_phases, is same as previous calculation.
 *   Routine sets up struct unknown for each unknown.
 *   Determines elements, species, and phases that are in the model.
 *   Calculates mass-action equations for each species and phase.
 *   Routine builds a set of lists for calculating mass balance and
 *      for building jacobian.
 */
	struct solution *solution_ptr;

	if (state >= REACTION)
	{
		same_model = check_same_model();
	}
	else
	{
		same_model = FALSE;
		last_model.force_prep = TRUE;
	}
	/*same_model = FALSE; */
/*
 *   Initialize s, master, and unknown pointers
 */
	solution_ptr = use.Get_solution_ptr();
	if (solution_ptr == NULL)
	{
		error_msg("Solution needed for calculation not found, stopping.",
				  STOP);
	}
	description_x = (char *) free_check_null(description_x);
	description_x = string_duplicate(solution_ptr->description);
/*
 *   Allocate space for unknowns
 *   Must allocate all necessary space before pointers to
 *   X are set.
 */

	if (!same_model && !switch_numerical)
		numerical_fixed_volume = false;
	if (same_model == FALSE || switch_numerical)
	{
		clear();
		setup_unknowns();
/*
 *   Set unknown pointers, unknown types, validity checks
 */
		if (state == INITIAL_SOLUTION)
			convert_units(solution_ptr);
		setup_solution();
		setup_exchange();
		setup_surface();
		setup_pure_phases();
		setup_gas_phase();
		setup_ss_assemblage();
		setup_related_surface();
		setup_slack();
		tidy_redox();
		if (get_input_errors() > 0)
		{
			error_msg("Program terminating due to input errors.", STOP);
		}
/*
 *   Allocate space for array
 */
/*
		array = (LDBLE *) PHRQ_malloc( (size_t) (count_unknowns+1) * count_unknowns * sizeof( LDBLE ));
		if (array == NULL) malloc_error();
		delta = (LDBLE *) PHRQ_malloc( (size_t) count_unknowns * sizeof( LDBLE ));
		if (delta == NULL) malloc_error();
		residual = (LDBLE *) PHRQ_malloc( (size_t) count_unknowns * sizeof( LDBLE ));
		if (residual == NULL) malloc_error();
*/
		array =
			(LDBLE *) PHRQ_malloc((size_t) (max_unknowns + 1) *
								  max_unknowns * sizeof(LDBLE));
		if (array == NULL)
			malloc_error();
		delta = (LDBLE *) PHRQ_malloc((size_t) max_unknowns * sizeof(LDBLE));
		if (delta == NULL)
			malloc_error();
		residual =
			(LDBLE *) PHRQ_malloc((size_t) max_unknowns * sizeof(LDBLE));
		if (residual == NULL)
			malloc_error();
		for (int j = 0; j < max_unknowns; j++)
		{
		  residual[j] = 0;
		}

/*
 *   Build lists to fill Jacobian array and species list
 */
		build_model();
		adjust_setup_pure_phases();
		adjust_setup_solution();
	}
	else
	{
/*
 *   If model is same, just update masses, don't rebuild unknowns and lists
 */
		quick_setup();
	}
	if (get_input_errors() > 0)
	{
		error_msg("Program stopping due to input errors.", STOP);
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
 int Phreeqc::
quick_setup(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Routine is used if model is the same as previous model
 *   Assumes moles of elements, exchangers, surfaces, gases, and solid solutions have
 *       been accumulated in array master, usually by subroutine step.
 *   Updates essential information for the model.
 */
	int i, j, k, l;
	char token[MAX_LENGTH], name[MAX_LENGTH];

	for (i = 0; i < count_master; i++)
	{
		if (master[i]->s->type == SURF_PSI)
			continue;
		if (master[i]->s == s_eminus ||
			master[i]->s == s_hplus ||
			master[i]->s == s_h2o || master[i]->s == s_h2
			|| master[i]->s == s_o2)
			continue;
		if (master[i]->total > 0)
		{
			if (master[i]->s->secondary != NULL)
			{
				master[i]->s->secondary->unknown->moles = master[i]->total;
			}
			else
			{
				master[i]->unknown->moles = master[i]->total;
			}
		}
	}
/*
 *   Reaction: pH for charge balance
 */
	ph_unknown->moles = use.Get_solution_ptr()->cb;
/*
 *   Reaction: pe for total hydrogen
 */
	if (mass_hydrogen_unknown != NULL)
	{
/* Use H - 2O linear combination in place of H */
#define COMBINE
		/*#define COMBINE_CHARGE */
#ifdef COMBINE
		mass_hydrogen_unknown->moles =
			use.Get_solution_ptr()->total_h - 2 * use.Get_solution_ptr()->total_o;
#else
		mass_hydrogen_unknown->moles = use.Get_solution_ptr()->total_h;
#endif
	}
/*
 *   Reaction H2O for total oxygen
 */
	if (mass_oxygen_unknown != NULL)
	{
		mass_oxygen_unknown->moles = use.Get_solution_ptr()->total_o;
	}

/*
 *   pp_assemblage
 */
	j = 0;
	for (i = 0; i < count_unknowns; i++)
	{
		if (x[i]->type == PP)
		{
			cxxPPassemblage * pp_assemblage_ptr = (cxxPPassemblage *) use.Get_pp_assemblage_ptr();
			std::map<std::string, cxxPPassemblageComp>::iterator it;
			it =  pp_assemblage_ptr->Get_pp_assemblage_comps().find(x[i]->pp_assemblage_comp_name);
			assert(it != pp_assemblage_ptr->Get_pp_assemblage_comps().end());
			cxxPPassemblageComp * comp_ptr = &(it->second);

			x[i]->moles = comp_ptr->Get_moles();
			/* A. Crapsi */
			x[i]->si    = comp_ptr->Get_si();
			x[i]->delta = comp_ptr->Get_delta();
			/* End A. Crapsi */
			x[i]->dissolve_only = comp_ptr->Get_dissolve_only() ? TRUE : FALSE;
			comp_ptr->Set_delta(0.0);
		}
	}
	// Need to update SIs for gases
	adjust_setup_pure_phases();

/*
 *   gas phase
 */
	if (gas_unknown != NULL)
	{
		cxxGasPhase * gas_phase_ptr = (cxxGasPhase *) use.Get_gas_phase_ptr();
		if ((gas_phase_ptr->Get_type() == cxxGasPhase::GP_VOLUME) && 
			numerical_fixed_volume && 
			(gas_phase_ptr->Get_pr_in() || force_numerical_fixed_volume))
		{
			//for (i = 0; i < use.Get_gas_phase_ptr()->count_comps; i++)
			//{
			for (size_t i = 0; i < gas_phase_ptr->Get_gas_comps().size(); i++)
			{	
				cxxGasComp *gc_ptr = &(gas_phase_ptr->Get_gas_comps()[i]);
				//int k;
				//struct phase *phase_ptr = phase_bsearch(gc_ptr->Get_phase_name().c_str() , &k, FALSE);
				gas_unknowns[i]->moles = gc_ptr->Get_moles();
				if (gas_unknowns[i]->moles <= 0)
					gas_unknowns[i]->moles = MIN_TOTAL;
				gas_unknowns[i]->phase->pr_in = false;
				gas_unknowns[i]->phase->pr_phi = 1.0;
				gas_unknowns[i]->phase->pr_p = 0;
				//gas_unknowns[i]->gas_phase = use.Get_gas_phase_ptr();
			}
			same_pressure = FALSE;
		}
		else
		{
			gas_unknown->moles = 0.0;
			for (size_t i = 0; i < gas_phase_ptr->Get_gas_comps().size(); i++)
			{	
				cxxGasComp *gc_ptr = &(gas_phase_ptr->Get_gas_comps()[i]);
				gas_unknown->moles += gc_ptr->Get_moles();
			}
			if (gas_unknown->moles <= 0)
				gas_unknown->moles = MIN_TOTAL;
			gas_unknown->ln_moles = log(gas_unknown->moles);
			//gas_unknown->gas_phase = gas_phase_ptr;
		}
	}

/*
 *   ss_assemblage
 */
	if (s_s_unknown != NULL)
	{
		for (i = 0; i < count_unknowns; i++)
		{
			if (x[i]->type == S_S_MOLES)
				break;
		}
		for (j = 0; j < use.Get_ss_assemblage_ptr()->count_s_s; j++)
		{
			for (k = 0; k < use.Get_ss_assemblage_ptr()->s_s[j].count_comps; k++)
			{
				x[i]->s_s = &(use.Get_ss_assemblage_ptr()->s_s[j]);
				x[i]->s_s_comp = &(use.Get_ss_assemblage_ptr()->s_s[j].comps[k]);
				x[i]->s_s_comp_number = j;
				x[i]->moles = x[i]->s_s_comp->moles;
				if (x[i]->moles <= 0)
				{
					x[i]->moles = MIN_TOTAL_SS;
					x[i]->s_s_comp->moles = MIN_TOTAL_SS;
				}
				x[i]->s_s_comp->initial_moles = x[i]->moles;
				x[i]->ln_moles = log(x[i]->moles);

				x[i]->phase->dn = x[i]->s_s_comp->dn;
				x[i]->phase->dnb = x[i]->s_s_comp->dnb;
				x[i]->phase->dnc = x[i]->s_s_comp->dnc;
				x[i]->phase->log10_fraction_x =
					x[i]->s_s_comp->log10_fraction_x;
				x[i]->phase->log10_lambda = x[i]->s_s_comp->log10_lambda;
				i++;
			}
		}
	}
/*
 *   exchange
 */
	// number of moles is set from master->moles above
/*
 *   surface
 */
	if (use.Get_surface_ptr() != NULL)
	{
		for (i = 0; i < count_unknowns; i++)
		{
			if (x[i]->type == SURFACE)
			{
				break;
			}
		}
		j = 0;
		k = 0;
		for (; i < count_unknowns; i++)
		{
			if (x[i]->type == SURFACE_CB)
			{
				x[i]->surface_charge = &(use.Get_surface_ptr()->charge[j]);
				x[i]->related_moles = x[i]->surface_charge->grams;
				x[i]->mass_water = use.Get_surface_ptr()->charge[j++].mass_water;
				x[i]->surface_comp = x[i - 1]->surface_comp;

				/* test that charge and surface match */
				char * temp_formula = string_duplicate(x[i]->surface_comp->formula);
				char * ptr = temp_formula;
				copy_token(token, &ptr, &l);
				char * ptr1 = token;
				get_elt(&ptr1, name, &l);
				ptr1 = strchr(name, '_');
				if (ptr1 != NULL)
					ptr1[0] = '\0';
				if (strcmp(name, x[i]->surface_charge->name) != 0)
				{
					free_check_null(temp_formula);
					error_string = sformatf(
							"Internal error: Surface charge name %s does not match surface component name %s\nTry alphabetical order for surfaces in SURFACE",
							x[i]->surface_charge->name,
							x[i]->surface_comp->formula);
					error_msg(error_string, STOP);
				}
				free_check_null(temp_formula);

				/* moles picked up from master->total */
			}
			else if (x[i]->type == SURFACE_CB1 || x[i]->type == SURFACE_CB2)
			{
				/*
				   charge = use.Get_surface_ptr()->comps[i].charge;
				   x[count_unknowns]->surface_charge = &use.Get_surface_ptr()->charge[charge];
				 */
				x[i]->surface_charge = x[i - 1]->surface_charge;
				x[i]->related_moles = x[i]->surface_charge->grams;
				x[i]->mass_water = x[i]->surface_charge->mass_water;
			}
			else if (x[i]->type == SURFACE)
			{
				x[i]->surface_comp = &(use.Get_surface_ptr()->comps[k++]);
				/* moles picked up from master->total
				   except for surfaces related to kinetic minerals ... */
				if (x[i]->surface_comp->rate_name != NULL)
				{
					for (l = 0; x[i]->surface_comp->totals[l].elt != NULL;
						 l++)
					{
						if (x[i]->surface_comp->totals[l].elt->master->type !=
							SURF)
							continue;
						if (strcmp_nocase
							(x[i]->description,
							 x[i]->surface_comp->totals[l].elt->name) == 0)
						{
							x[i]->moles = x[i]->surface_comp->totals[l].coef;
							/* printf("%s  moles %e\n", x[i]->description, x[i]->moles); */
						}
					}
				}
			}
			else
			{
				break;
			}

		}
	}
	save_model();
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
build_gas_phase(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Put coefficients into lists to sum iaps to test for equilibrium
 *   Put coefficients into lists to build jacobian for 
 *      sum of partial pressures equation and
 *      mass balance equations for elements contained in gases
 */
	//int i, j;
	int row, col;
	struct master *master_ptr;
	struct rxn_token *rxn_ptr;
	//struct gas_comp *gas_comp_ptr;
	//struct phase *phase_ptr;
	struct unknown *unknown_ptr;
	LDBLE coef, coef_elt;

	if (gas_unknown == NULL)
		return (OK);
	cxxGasPhase * gas_phase_ptr = (cxxGasPhase *) use.Get_gas_phase_ptr();
	if (gas_phase_ptr->Get_type() == cxxGasPhase::GP_VOLUME && 
		(gas_phase_ptr->Get_pr_in() || force_numerical_fixed_volume) && 
		numerical_fixed_volume)
	{
		return build_fixed_volume_gas();
	}
	for (size_t i = 0; i < gas_phase_ptr->Get_gas_comps().size(); i++)
	{	
		cxxGasComp *gc_ptr = &(gas_phase_ptr->Get_gas_comps()[i]);
		int k;
		struct phase *phase_ptr = phase_bsearch(gc_ptr->Get_phase_name().c_str() , &k, FALSE);
		assert(phase_ptr);
/*
 *   Determine elements in gas component
 */
		count_elts = 0;
		paren_count = 0;
		if (phase_ptr->rxn_x == NULL)
			continue;
		add_elt_list(phase_ptr->next_elt, 1.0);
#ifdef COMBINE
		change_hydrogen_in_elt_list(0);
#endif
/*
 *   Build mass balance sums for each element in gas
 */
		if (debug_prep == TRUE)
		{
			output_msg(sformatf( "\n\tMass balance summations %s.\n\n",
					   phase_ptr->name));
		}

		/* All elements in gas */
		for (int j = 0; j < count_elts; j++)
		{
			unknown_ptr = NULL;
			if (strcmp(elt_list[j].elt->name, "H") == 0)
			{
				unknown_ptr = mass_hydrogen_unknown;
			}
			else if (strcmp(elt_list[j].elt->name, "O") == 0)
			{
				unknown_ptr = mass_oxygen_unknown;
			}
			else
			{
				if (elt_list[j].elt->primary->in == TRUE)
				{
					unknown_ptr = elt_list[j].elt->primary->unknown;
				}
				else if (elt_list[j].elt->primary->s->secondary != NULL)
				{
					unknown_ptr =
						elt_list[j].elt->primary->s->secondary->unknown;
				}
			}
			if (unknown_ptr != NULL)
			{
				coef = elt_list[j].coef;
				store_mb(&(phase_ptr->moles_x), &(unknown_ptr->f), coef);
				if (debug_prep == TRUE)
				{
					output_msg(sformatf( "\t\t%-24s%10.3f\n",
							   unknown_ptr->description, (double) coef));
				}
			}
		}
		if (gas_phase_ptr->Get_type() == cxxGasPhase::GP_PRESSURE)
		{
			/* Total pressure of gases */
			store_mb(&(phase_ptr->p_soln_x), &(gas_unknown->f), 1.0);
		}
/*
 *   Build jacobian sums for mass balance equations
 */
		if (debug_prep == TRUE)
		{
			output_msg(sformatf( "\n\tJacobian summations %s.\n\n",
					   phase_ptr->name));
		}
		for (int j = 0; j < count_elts; j++)
		{
			unknown_ptr = NULL;
			if (strcmp(elt_list[j].elt->name, "H") == 0)
			{
				unknown_ptr = mass_hydrogen_unknown;
			}
			else if (strcmp(elt_list[j].elt->name, "O") == 0)
			{
				unknown_ptr = mass_oxygen_unknown;
			}
			else
			{
				if (elt_list[j].elt->primary->in == TRUE)
				{
					unknown_ptr = elt_list[j].elt->primary->unknown;
				}
				else if (elt_list[j].elt->primary->s->secondary != NULL)
				{
					unknown_ptr =
						elt_list[j].elt->primary->s->secondary->unknown;
				}
			}
			if (unknown_ptr == NULL)
			{
				continue;
			}
			if (debug_prep == TRUE)
			{
				output_msg(sformatf( "\n\t%s.\n",
						   unknown_ptr->description));
			}
			row = unknown_ptr->number * (count_unknowns + 1);
			coef_elt = elt_list[j].coef;
			for (rxn_ptr = phase_ptr->rxn_x->token + 1;
				 rxn_ptr->s != NULL; rxn_ptr++)
			{

				if (rxn_ptr->s->secondary != NULL
					&& rxn_ptr->s->secondary->in == TRUE)
				{
					master_ptr = rxn_ptr->s->secondary;
				}
				else
				{
					master_ptr = rxn_ptr->s->primary;
				}
				if (master_ptr == NULL)
				{
					error_string = sformatf(
							"Element needed for gas component, %s, is not in model.",
							phase_ptr->name);
					warning_msg(error_string);
					//error_msg(error_string, CONTINUE);
					//input_error++;
					continue;
				}
				if (debug_prep == TRUE)
				{
					output_msg(sformatf( "\t\t%s\n",
							   master_ptr->s->name));
				}
				if (master_ptr->unknown == NULL)
				{
					continue;
				}
				if (master_ptr->in == FALSE)
				{
					error_string = sformatf(
							"Element, %s, in phase, %s, is not in model.",
							master_ptr->elt->name, phase_ptr->name);
					error_msg(error_string, CONTINUE);
					input_error++;
				}
				col = master_ptr->unknown->number;
				coef = coef_elt * rxn_ptr->coef;
				store_jacob(&(phase_ptr->moles_x),
							&(array[row + col]), coef);
				if (debug_prep == TRUE)
				{
					output_msg(sformatf( "\t\t%-24s%10.3f\t%d\t%d\n",
							   master_ptr->s->name, (double) coef,
							   row / (count_unknowns + 1), col));
				}
			}
			if (gas_phase_ptr->Get_type() == cxxGasPhase::GP_PRESSURE)
			{
				/* derivative wrt total moles of gas */
				store_jacob(&(phase_ptr->fraction_x),
							&(array[row + gas_unknown->number]), coef_elt);
				if (debug_prep == TRUE)
				{
					output_msg(sformatf( "\t\t%-24s%10.3f\t%d\t%d\n",
							   "gas moles", (double) elt_list[j].coef,
							   row / (count_unknowns + 1),
							   gas_unknown->number));
				}
			}
		}
/*
 *   Build jacobian sums for sum of partial pressures equation
 */
		if (gas_phase_ptr->Get_type() != cxxGasPhase::GP_PRESSURE)
			continue;
		if (debug_prep == TRUE)
		{
			output_msg(sformatf( "\n\tPartial pressure eqn %s.\n\n",
					   phase_ptr->name));
		}
		unknown_ptr = gas_unknown;
		row = unknown_ptr->number * (count_unknowns + 1);
		for (rxn_ptr = phase_ptr->rxn_x->token + 1; rxn_ptr->s != NULL; rxn_ptr++)
		{
			if (rxn_ptr->s != s_eminus && rxn_ptr->s->in == FALSE)
			{
				error_string = sformatf(
					"Element in species, %s, in phase, %s, is not in model.",
					rxn_ptr->s->name, phase_ptr->name);
				warning_msg(error_string);
				//error_msg(error_string, CONTINUE);
				//input_error++;
			}
			else
			{
				if (rxn_ptr->s->secondary != NULL
					&& rxn_ptr->s->secondary->in == TRUE)
				{
					master_ptr = rxn_ptr->s->secondary;
				}
				else 
				{
					master_ptr = rxn_ptr->s->primary;
				}

				if (master_ptr == NULL)
				{
					error_string = sformatf(
						"Master species for %s, in phase, %s, is not in model.",
						rxn_ptr->s->name, phase_ptr->name);
					error_msg(error_string, CONTINUE);
					input_error++;
				}
				else
				{
					if (debug_prep == TRUE)
					{
						output_msg(sformatf( "\t\t%s\n", master_ptr->s->name));
					}
					if (master_ptr->unknown == NULL)
					{
						assert(false);
						continue;
					}
					if (master_ptr->in == FALSE)
					{
						error_string = sformatf(
							"Element, %s, in phase, %s, is not in model.",
							master_ptr->elt->name, phase_ptr->name);
						warning_msg(error_string);
						//error_msg(error_string, CONTINUE);
						//input_error++;
					}
					col = master_ptr->unknown->number;
					coef = rxn_ptr->coef;
					store_jacob(&(phase_ptr->p_soln_x), &(array[row + col]), coef);
					if (debug_prep == TRUE)
					{
						output_msg(sformatf( "\t\t%-24s%10.3f\t%d\t%d\n",
							master_ptr->s->name, (double) coef,
							row / (count_unknowns + 1), col));
					}
				}
			}
		}
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
build_ss_assemblage(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Put coefficients into lists to sum iaps to test for equilibrium
 *   Put coefficients into lists to build jacobian for 
 *      mass action equation for component
 *      mass balance equations for elements contained in solid solutions
 */
	int i, j, k, l, stop;
	int row, col;
	struct master *master_ptr;
	struct rxn_token *rxn_ptr;
	struct s_s *s_s_ptr, *s_s_ptr_old;
	char token[MAX_LENGTH];
	char *ptr;

	if (s_s_unknown == NULL)
		return (OK);
	s_s_ptr_old = NULL;
	col = 0;
	for (i = 0; i < count_unknowns; i++)
	{
		if (x[i]->type != S_S_MOLES)
			continue;
		s_s_ptr = x[i]->s_s;
		if (s_s_ptr != s_s_ptr_old)
		{
			col = x[i]->number;
			s_s_ptr_old = s_s_ptr;
		}
/*
 *   Calculate function value (inverse saturation index)
 */
		if (x[i]->phase->rxn_x == NULL)
			continue;
		store_mb(&(x[i]->phase->lk), &(x[i]->f), 1.0);
		for (rxn_ptr = x[i]->phase->rxn_x->token + 1; rxn_ptr->s != NULL;
			 rxn_ptr++)
		{
			store_mb(&(rxn_ptr->s->la), &(x[i]->f), -rxn_ptr->coef);
		}
		/* include mole fraction */
		store_mb(&(x[i]->phase->log10_fraction_x), &(x[i]->f), 1.0);

		/* include activity coeficient */
		store_mb(&(x[i]->phase->log10_lambda), &(x[i]->f), 1.0);
/*
 *   Put coefficients into mass action equations
 */
		/* first IAP terms */
		for (rxn_ptr = x[i]->phase->rxn_x->token + 1; rxn_ptr->s != NULL;
			 rxn_ptr++)
		{
			if (rxn_ptr->s->secondary != NULL
				&& rxn_ptr->s->secondary->in == TRUE)
			{
				master_ptr = rxn_ptr->s->secondary;
			}
			else
			{
				master_ptr = rxn_ptr->s->primary;
			}
			if (master_ptr == NULL || master_ptr->unknown == NULL)
				continue;
			store_jacob0(x[i]->number, master_ptr->unknown->number,
						 rxn_ptr->coef);
		}

		if (s_s_ptr->a0 != 0.0 || s_s_ptr->a1 != 0.0)
		{
/*
 *   For binary solid solution
 */
			/* next dnc terms */
			row = x[i]->number * (count_unknowns + 1);
			if (x[i]->s_s_comp_number == 0)
			{
				col = x[i]->number;
			}
			else
			{
				col = x[i]->number - 1;
			}
			store_jacob(&(x[i]->phase->dnc), &(array[row + col]), -1);

			/* next dnb terms */
			col++;
			store_jacob(&(x[i]->phase->dnb), &(array[row + col]), -1);
		}
		else
		{
/*
 *   For ideal solid solution
 */
			row = x[i]->number * (count_unknowns + 1);
			for (j = 0; j < s_s_ptr->count_comps; j++)
			{
				if (j != x[i]->s_s_comp_number)
				{
/*					store_jacob (&(s_s_ptr->dn), &(array[row + col + j]), -1.0); */
					store_jacob(&(x[i]->phase->dn), &(array[row + col + j]),
								-1.0);
				}
				else
				{
					store_jacob(&(x[i]->phase->dnb), &(array[row + col + j]),
								-1.0);
				}
			}
		}
/*
 *   Put coefficients into mass balance equations
 */
		count_elts = 0;
		paren_count = 0;
		strcpy(token, x[i]->phase->formula);
		ptr = token;
		get_elts_in_species(&ptr, 1.0);
/*
 *   Go through elements in phase
 */
#ifdef COMBINE
		change_hydrogen_in_elt_list(0);
#endif
		for (j = 0; j < count_elts; j++)
		{

			if (strcmp(elt_list[j].elt->name, "H") == 0
				&& mass_hydrogen_unknown != NULL)
			{
				store_jacob0(mass_hydrogen_unknown->number, x[i]->number,
							 -elt_list[j].coef);
				store_sum_deltas(&(delta[i]), &mass_hydrogen_unknown->delta,
								 elt_list[j].coef);

			}
			else if (strcmp(elt_list[j].elt->name, "O") == 0
					 && mass_oxygen_unknown != NULL)
			{
				store_jacob0(mass_oxygen_unknown->number, x[i]->number,
							 -elt_list[j].coef);
				store_sum_deltas(&(delta[i]), &mass_oxygen_unknown->delta,
								 elt_list[j].coef);

			}
			else
			{
				master_ptr = elt_list[j].elt->primary;
				if (master_ptr->in == FALSE)
				{
					master_ptr = master_ptr->s->secondary;
				}
				if (master_ptr == NULL || master_ptr->in == FALSE)
				{
					if (state != ADVECTION && state != TRANSPORT
						&& state != PHAST)
					{
						error_string = sformatf(
								"Element in phase, %s, is not in model.",
								x[i]->phase->name);
						warning_msg(error_string);
					}
					if (master_ptr != NULL)
					{
						master_ptr->s->la = -999.9;
					}
/*
 *   Master species is in model
 */
				}
				else if (master_ptr->in == TRUE)
				{
					store_jacob0(master_ptr->unknown->number, x[i]->number,
								 -elt_list[j].coef);
					store_sum_deltas(&delta[i], &master_ptr->unknown->delta,
									 elt_list[j].coef);
/*
 *   Master species in equation needs to be rewritten
 */
				}
				else if (master_ptr->in == REWRITE)
				{
					stop = FALSE;
					for (k = 0; k < count_unknowns; k++)
					{
						if (x[k]->type != MB)
							continue;
						for (l = 0; x[k]->master[l] != NULL; l++)
						{
							if (x[k]->master[l] == master_ptr)
							{
								store_jacob0(x[k]->master[0]->unknown->
											 number, x[i]->number,
											 -elt_list[j].coef);
								store_sum_deltas(&delta[i],
												 &x[k]->master[0]->unknown->
												 delta, elt_list[j].coef);
								stop = TRUE;
								break;
							}
						}
						if (stop == TRUE)
							break;
					}
				}
			}
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
build_jacobian_sums(int k)
/* ---------------------------------------------------------------------- */
{
/*
 *   Function builds lists sum_jacob1 and sum_jacob2 that describe how to sum molalities
 *   to form jacobian.
 */
	int i, j, kk;
	int count_g;
	LDBLE coef;
	LDBLE *source, *target;

	if (debug_prep == TRUE)
		output_msg(sformatf( "\n\tJacobian summations.\n"));
/*
 *   Calculate jacobian coefficients for each mass balance equation
 */
	for (i = 0; i < count_mb_unknowns; i++)
	{
/*
 *   Store d(moles) for a mass balance equation
 */
		/* initial solution only */
		if (mb_unknowns[i].unknown->type == SOLUTION_PHASE_BOUNDARY)
		{
			continue;
		}
		coef = mb_unknowns[i].coef;
		if (debug_prep == TRUE)
			output_msg(sformatf( "\n\tMass balance eq:  %s\t%f\n",
					   mb_unknowns[i].unknown->description, (double) coef));
		store_dn(k, mb_unknowns[i].source, mb_unknowns[i].unknown->number,
				 coef, mb_unknowns[i].gamma_source);
/*
 *   Add extra terms for change in dg/dx in diffuse layer model
 */
		if (s[k]->type >= H2O || dl_type_x == NO_DL)
		{
			continue;
		}
		else if ((mb_unknowns[i].unknown->type == MB ||
				  mb_unknowns[i].unknown->type == MH ||
				  mb_unknowns[i].unknown->type == MH2O) && state >= REACTION)
		{
			if (mass_oxygen_unknown != NULL)
			{
				/* term for water, sum of all surfaces */
				source = &s[k]->tot_dh2o_moles;
				target =
					&(array
					  [mb_unknowns[i].unknown->number * (count_unknowns + 1) +
					   mass_oxygen_unknown->number]);
				store_jacob(source, target, coef);
				if (debug_prep == TRUE)
				{
					output_msg(sformatf( "\t\t%-24s%10.3f\t%d\t%d\n",
							   "sum[dn(i,s)/dlnwater]", (double) coef,
							   mb_unknowns[i].unknown->number,
							   mass_oxygen_unknown->number));
				}
			}

			/* terms for psi, one for each surface */
			count_g = 0;
			for (j = 0; j < count_unknowns; j++)
			{
				if (x[j]->type != SURFACE_CB)
					continue;
				/*if (x[j]->type < SURFACE_CB || x[j]->type > SURFACE_CB1) continue; */
				source = &s[k]->diff_layer[count_g].dx_moles;
				target = &(array[mb_unknowns[i].unknown->number *
								 (count_unknowns + 1) + x[j]->number]);
				store_jacob(source, target, coef);
				if (debug_prep == TRUE)
				{
					output_msg(sformatf( "\t\t%-24s%10.3f\t%d\t%d\n",
							   "dg/dlny", (double) coef,
							   mb_unknowns[i].unknown->number, x[j]->number));
				}
				count_g++;
				if (count_g >= use.Get_surface_ptr()->count_charge)
					break;
			}

			/* terms for related phases */
			count_g = 0;
			for (j = 0; j < count_unknowns; j++)
			{
				if (x[j]->type != SURFACE_CB)
					continue;
				/*if (x[j]->type < SURFACE_CB || x[j]->type > SURFACE_CB1) continue; */

				/* has related phase */
				if (x[j - 1]->surface_comp->phase_name == NULL)
					continue;

				/* now find the related phase */
				for (kk = count_unknowns - 1; kk >= 0; kk--)
				{
					if (x[kk]->type != PP)
						continue;
					if (x[kk]->phase->name ==
						x[j - 1]->surface_comp->phase_name)
						break;
				}

				if (kk >= 0)
				{
					source = &s[k]->diff_layer[count_g].drelated_moles;
					target = &(array[mb_unknowns[i].unknown->number *
									 (count_unknowns + 1) + x[kk]->number]);
					store_jacob(source, target, coef);
					if (debug_prep == TRUE)
					{
						output_msg(sformatf(
								   "\t\t%-24s%10.3f\t%d\t%d\n", "dphase",
								   (double) coef,
								   mb_unknowns[i].unknown->number,
								   x[kk]->number));
					}
				}
				count_g++;
				if (count_g >= use.Get_surface_ptr()->count_charge)
					break;
			}

		}
		else if (mb_unknowns[i].unknown->type == SURFACE_CB)
		{
			count_g = 0;
			for (j = 0; j < count_unknowns; j++)
			{
				if (x[j]->type != SURFACE_CB)
					continue;
				if (mb_unknowns[i].unknown->number == x[j]->number)
				{
					source = &s[k]->diff_layer[count_g].dx_moles;
					target = &(array[mb_unknowns[i].unknown->number *
									 (count_unknowns + 1) + x[j]->number]);
					store_jacob(source, target, coef);
					if (debug_prep == TRUE)
					{
						output_msg(sformatf(
								   "\t\t%-24s%10.3f\t%d\t%d\n", "dg/dlny",
								   (double) coef,
								   mb_unknowns[i].unknown->number,
								   x[j]->number));
					}

					/* term for related phase */
					/* has related phase */
					if (x[j - 1]->surface_comp->phase_name != NULL)
					{

						/* now find the related phase */
						for (kk = count_unknowns - 1; kk >= 0; kk--)
						{
							if (x[kk]->type != PP)
								continue;
							if (x[kk]->phase->name ==
								x[j - 1]->surface_comp->phase_name)
								break;
						}
						if (kk >= 0)
						{
							source =
								&s[k]->diff_layer[count_g].drelated_moles;
							target =
								&(array
								  [mb_unknowns[i].unknown->number *
								   (count_unknowns + 1) + x[kk]->number]);
							store_jacob(source, target, coef);
							if (debug_prep == TRUE)
							{
								output_msg(sformatf(
										   "\t\t%-24s%10.3f\t%d\t%d\n",
										   "dphase", (double) coef,
										   mb_unknowns[i].unknown->number,
										   x[kk]->number));
							}
						}
					}

					if (mass_oxygen_unknown != NULL)
					{
						/* term for water, for same surfaces */
						source = &s[k]->diff_layer[count_g].dh2o_moles;
						target = &(array[mb_unknowns[i].unknown->number *
										 (count_unknowns + 1) +
										 mass_oxygen_unknown->number]);
						store_jacob(source, target, coef);
						if (debug_prep == TRUE)
						{
							output_msg(sformatf(
									   "\t\t%-24s%10.3f\t%d\t%d\n",
									   "dn(i,s)/dlnwater", (double) coef,
									   mb_unknowns[i].unknown->number,
									   mass_oxygen_unknown->number));
						}
					}
					break;
				}
				count_g++;
				if (count_g >= use.Get_surface_ptr()->count_charge)
					break;
			}
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
build_mb_sums(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Function builds lists sum_mb1 and sum_mb2  that describe how to sum molalities
 *   to calculate mass balance sums, including activity of water, ionic strength,
 *   charge balance, and alkalinity.
 */
	int i;
	LDBLE *target;
/*
 *   Make space for lists
 */
	if (count_sum_mb1 + count_mb_unknowns >= max_sum_mb1)
	{
		space((void **) ((void *) &sum_mb1),
			  count_sum_mb1 + count_mb_unknowns, &max_sum_mb1,
			  sizeof(struct list1));
	}
	if (count_sum_mb2 + count_mb_unknowns >= max_sum_mb2)
	{
		space((void **) ((void *) &sum_mb2),
			  count_sum_mb2 + count_mb_unknowns, &max_sum_mb2,
			  sizeof(struct list2));
	}

	if (debug_prep == TRUE)
	{
		output_msg(sformatf( "\n\tMass balance summations.\n\n"));
	}
	for (i = 0; i < count_mb_unknowns; i++)
	{
		target = &(mb_unknowns[i].unknown->f);
		store_mb(mb_unknowns[i].source, target, mb_unknowns[i].coef);
		if (debug_prep == TRUE)
		{
			output_msg(sformatf( "\t\t%-24s%10.3f\n",
					   mb_unknowns[i].unknown->description,
					   (double) mb_unknowns[i].coef));
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
build_model(void)
/* ---------------------------------------------------------------------- */
{
/*
 *    Guts of prep. Determines species in model, rewrites equations,
 *    builds lists for mass balance and jacobian sums.
 */
	int i, j, j0, k;
	LDBLE coef_e;

	if (s_hplus == NULL || s_eminus == NULL || s_h2o == NULL)
	{
		error_msg("Data base is missing H+, H2O, or e- species.", CONTINUE);
		input_error++;
	}
/*
 *   Make space for lists of pointers to species in the model
 */

	max_s_x = MAX_S;
	space((void **) ((void *) &s_x), INIT, &max_s_x,
		  sizeof(struct species *));

	max_sum_mb1 = MAX_SUM_MB;
	count_sum_mb1 = 0;
	space((void **) ((void *) &sum_mb1), INIT, &max_sum_mb1,
		  sizeof(struct list1));

	max_sum_mb2 = MAX_SUM_MB;
	count_sum_mb2 = 0;
	space((void **) ((void *) &sum_mb2), INIT, &max_sum_mb2,
		  sizeof(struct list2));

	max_sum_jacob0 = MAX_SUM_JACOB0;
	count_sum_jacob0 = 0;
	space((void **) ((void *) &sum_jacob0), INIT, &max_sum_jacob0,
		  sizeof(struct list0));

	max_sum_jacob1 = MAX_SUM_JACOB1;
	count_sum_jacob1 = 0;
	space((void **) ((void *) &sum_jacob1), INIT, &max_sum_jacob1,
		  sizeof(struct list1));

	max_sum_jacob2 = MAX_SUM_JACOB2;
	count_sum_jacob2 = 0;
	space((void **) ((void *) &sum_jacob2), INIT, &max_sum_jacob2,
		  sizeof(struct list2));


	max_sum_delta = MAX_SUM_JACOB0;
	count_sum_delta = 0;
	space((void **) ((void *) &sum_delta), INIT, &max_sum_delta,
		  sizeof(struct list2));

	max_species_list = 5 * MAX_S;
	count_species_list = 0;
	species_list = (struct species_list *) free_check_null(species_list);
	space((void **) ((void *) &species_list), INIT, &max_species_list,
		  sizeof(struct species_list));

/*
 *   Pick species in the model, determine reaction for model, build jacobian
 */
	count_s_x = 0;
	compute_gfw("H2O", &gfw_water);
	gfw_water *= 0.001;
	for (i = 0; i < count_s; i++)
	{
		if (s[i]->type > H2O && s[i]->type != EX && s[i]->type != SURF)
			continue;
		s[i]->in = FALSE;
		count_trxn = 0;
		trxn_add(s[i]->rxn_s, 1.0, FALSE);	/* rxn_s is set in tidy_model */
/*
 *   Check if species is in model
 */
		s[i]->in = inout();
		if (s[i]->in == TRUE)
		{
			/* for isotopes, activity of water is for 1H and 16O */
			if (s[i]->gflag == 9)
			{
				gfw_water = 18.0 / 1000.0;
			}
			if (pitzer_model == FALSE && sit_model == FALSE)
				s[i]->lg = 0.0;
			if (count_s_x + 1 >= max_s_x)
			{
				space((void **) ((void *) &s_x), count_s_x + 1,
					  &max_s_x, sizeof(struct species *));
			}
			s_x[count_s_x++] = s[i];
/*
 *   Write mass action equation for current model
 */
			write_mass_action_eqn_x(STOP);
			if (s[i]->type == SURF)
			{
				add_potential_factor();
				add_cd_music_factors(i);
			}
			rxn_free(s[i]->rxn_x);
			s[i]->rxn_x = rxn_alloc(count_trxn + 1);
			trxn_copy(s[i]->rxn_x);
			for (j = 0; j < 3; j++)
			{
				s[i]->dz[j] = s[i]->rxn_x->dz[j];
			}
			if (debug_prep == TRUE)
			{
				output_msg(sformatf( "\n%s\n\tMass-action equation\n",
						   s[i]->name));
				trxn_print();
			}
/*
 *   Determine mass balance equations, build sums for mass balance, build sums for jacobian
 */
			count_trxn = 0;
			trxn_add(s[i]->rxn_s, 1.0, FALSE);
			if (s[i]->next_secondary == NULL)
			{
				write_mb_eqn_x();
			}
			else
			{
				count_elts = 0;
				add_elt_list(s[i]->next_secondary, 1.0);
			}
			if (s[i]->type == SURF)
			{
				add_potential_factor();
				add_cd_music_factors(i);
				add_surface_charge_balance();
				add_cd_music_charge_balances(i);
			}
			if (debug_prep == TRUE)
			{
				output_msg(sformatf( "\tElement composition\n",
						   trxn.token[0].s->name));
				for (j = 0; j < count_elts; j++)
				{
					output_msg(sformatf( "\t\t%-20s\t%10.2f\n",
							   elt_list[j].elt->name,
							   (double) elt_list[j].coef));
				}
			}
			if (debug_prep == TRUE)
			{
				output_msg(sformatf( "\n\tMass balance equation\n",
						   s[i]->name));
				trxn_print();
			}
			if (s[i]->type < EMINUS)
			{
				mb_for_species_aq(i);
			}
			else if (s[i]->type == EX)
			{
				mb_for_species_ex(i);
			}
			else if (s[i]->type == SURF)
			{
				mb_for_species_surf(i);
			}
#ifdef COMBINE
			build_mb_sums();
#else
			if (s[i] != s_h2o)
			{
				build_mb_sums();
			}
#endif
			if (!pitzer_model && !sit_model)
				build_jacobian_sums(i);
/*
 *    Build list of species for summing and printing
 */
			if (s[i]->next_secondary == NULL)
			{
				write_mb_for_species_list(i);
			}
			else
			{
				count_elts = 0;
				add_elt_list(s[i]->next_secondary, 1.0);
			}
			build_species_list(i);
		}
	}
	if (dl_type_x != NO_DL && (pitzer_model == TRUE || sit_model == TRUE))
	{
		error_msg("-diffuse_layer option not available for Pizer or SIT model",
				  STOP);
	}
/*
 *   Sum diffuse layer water into hydrogen and oxygen mass balances
 */
	if (dl_type_x != NO_DL && state >= REACTION)
	{
		for (i = 0; i < count_unknowns; i++)
		{
			if (x[i]->type == SURFACE_CB)
			{
#ifndef COMBINE
				store_mb(&(x[i]->mass_water),
						 &(mass_hydrogen_unknown->f), 2 / gfw_water);
#endif
				if (mass_oxygen_unknown != NULL)
				{
					store_mb(&(x[i]->mass_water),
							 &(mass_oxygen_unknown->f), 1 / gfw_water);
				}
			}
		}
	}
/*
 *   For Pizer model add lg unknown for each aqueous species
 */

	if (pitzer_model == TRUE || sit_model == TRUE)
	{
		j0 = count_unknowns;
		j = count_unknowns + count_s_x;
		k = j0;
		for (i = j0; i < j; i++)
		{
			if (s_x[i - j0]->type == EX)
				continue;
			if (s_x[i - j0]->type == SURF)
				continue;
			x[k]->number = k;
			x[k]->type = PITZER_GAMMA;
			x[k]->s = s_x[i - j0];
			x[k]->description = s_x[i - j0]->name;
			k++;
			count_unknowns++;
		}
	}
/*
 *   Rewrite phases to current master species
 */
	for (i = 0; i < count_phases; i++)
	{
		count_trxn = 0;
		trxn_add_phase(phases[i]->rxn_s, 1.0, FALSE);
		trxn_reverse_k();
		phases[i]->in = inout();
		if (phases[i]->in == TRUE)
		{
/*
 *   Replace e- in original equation with default redox reaction
 */
			coef_e = trxn_find_coef("e-", 1);
			if (equal(coef_e, 0.0, TOL) == FALSE)
			{
				trxn_add(pe_x[default_pe_x].rxn, coef_e, TRUE);
			}
/*
 *   Rewrite reaction to current master species
 */
			write_mass_action_eqn_x(STOP);
			trxn_reverse_k();
			rxn_free(phases[i]->rxn_x);
			if (debug_prep == TRUE)
			{
				output_msg(sformatf( "\nPhase: %s\n", phases[i]->name));
				trxn_print();
			}
			phases[i]->rxn_x = rxn_alloc(count_trxn + 1);
			trxn_copy(phases[i]->rxn_x);
			write_phase_sys_total(i);
		}
	}
	build_solution_phase_boundaries();
	build_pure_phases();
	build_min_exch();
	build_min_surface();
	build_gas_phase();
	build_ss_assemblage();
/*
 *   Sort species list, by master only
 */
	qsort(&species_list[0], (size_t) count_species_list,
		  (size_t) sizeof(struct species_list), species_list_compare_master);
/*
 *   Save model description
 */
	save_model();
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
build_pure_phases(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Includes calculation of inverse saturation index in sum_mb.
 *   Puts coefficients in iap and mass balance equations for each phase.
 */
	//int i;
	//int stop, j, k, l;
	//char token[MAX_LENGTH];
	bool stop;
	std::string token;
	char *ptr;
	struct master *master_ptr;
	struct rxn_token *rxn_ptr;
/*
 *   Build into sums the logic to calculate inverse saturation indices for
 *   pure phases
 */
	if (pure_phase_unknown == NULL)
		return (OK);

	cxxPPassemblage * pp_assemblage_ptr = (cxxPPassemblage *) use.Get_pp_assemblage_ptr();
/*
 *   Calculate inverse saturation index
 */
	for (int i = 0; i < count_unknowns; i++)
	{
		if (x[i]->type != PP || x[i]->phase->rxn_x == NULL)
			continue;
		if (pure_phase_unknown == NULL)
			pure_phase_unknown = x[i];

		store_mb(&(x[i]->phase->lk), &(x[i]->f), 1.0);
		store_mb(&(x[i]->si), &(x[i]->f), 1.0);

		for (rxn_ptr = x[i]->phase->rxn_x->token + 1; rxn_ptr->s != NULL;
			 rxn_ptr++)
		{
			store_mb(&(rxn_ptr->s->la), &(x[i]->f), -rxn_ptr->coef);
		}
	}
	for (int i = 0; i < count_unknowns; i++)
	{
/*
 *  rxn_x is null if an element in phase is not in solution
 */
		if (x[i]->type != PP || x[i]->phase->rxn_x == NULL)
			continue;
/*
 *   Put coefficients into IAP equations
 */
		for (rxn_ptr = x[i]->phase->rxn_x->token + 1; rxn_ptr->s != NULL;
			 rxn_ptr++)
		{
			if (rxn_ptr->s->secondary != NULL
				&& rxn_ptr->s->secondary->in == TRUE)
			{
				master_ptr = rxn_ptr->s->secondary;
			}
			else
			{
				master_ptr = rxn_ptr->s->primary;
			}
			if (master_ptr == NULL || master_ptr->unknown == NULL)
				continue;
			store_jacob0(x[i]->number, master_ptr->unknown->number,
						 rxn_ptr->coef);
		}
/*
 *   Put coefficients into mass balance equations
 */
		count_elts = 0;
		paren_count = 0;
		cxxPPassemblageComp * comp_ptr = pp_assemblage_ptr->Find(x[i]->pp_assemblage_comp_name);
		if (comp_ptr->Get_add_formula().size() > 0)
		{
			char * char_name = string_duplicate(comp_ptr->Get_add_formula().c_str());
			ptr = char_name;
			get_elts_in_species(&ptr, 1.0);
			free_check_null(char_name);
		}
		else
		{
			char * char_name = string_duplicate(x[i]->phase->formula);
			ptr = char_name;
			get_elts_in_species(&ptr, 1.0);
			free_check_null(char_name);
		}
/*
 *   Go through elements in phase
 */

#ifdef COMBINE
		change_hydrogen_in_elt_list(0);
#endif
		for (int j = 0; j < count_elts; j++)
		{

			if (strcmp(elt_list[j].elt->name, "H") == 0
				&& mass_hydrogen_unknown != NULL)
			{
				store_jacob0(mass_hydrogen_unknown->number, x[i]->number,
							 -elt_list[j].coef);
				store_sum_deltas(&(delta[i]), &mass_hydrogen_unknown->delta,
								 elt_list[j].coef);

			}
			else if (strcmp(elt_list[j].elt->name, "O") == 0
					 && mass_oxygen_unknown != NULL)
			{
				store_jacob0(mass_oxygen_unknown->number, x[i]->number,
							 -elt_list[j].coef);
				store_sum_deltas(&(delta[i]), &mass_oxygen_unknown->delta,
								 elt_list[j].coef);

			}
			else
			{
				master_ptr = elt_list[j].elt->primary;
				if (master_ptr->in == FALSE)
				{
					master_ptr = master_ptr->s->secondary;
				}
				if (master_ptr == NULL || master_ptr->in == FALSE)
				{
					if (state != ADVECTION && state != TRANSPORT
						&& state != PHAST)
					{
						error_string = sformatf(
								"Element in phase, %s, is not in model.",
								x[i]->phase->name);
						warning_msg(error_string);
					}
					if (master_ptr != NULL)
					{
						master_ptr->s->la = -999.9;
					}
/*
 *   Master species is in model
 */
				}
				else if (master_ptr->in == TRUE)
				{
					store_jacob0(master_ptr->unknown->number, x[i]->number,
								 -elt_list[j].coef);
					store_sum_deltas(&delta[i], &master_ptr->unknown->delta,
									 elt_list[j].coef);
/*
 *   Master species in equation needs to be rewritten
 */
				}
				else if (master_ptr->in == REWRITE)
				{
					stop = false;
					for (int k = 0; k < count_unknowns; k++)
					{
						if (x[k]->type != MB)
							continue;
						for (int l = 0; x[k]->master[l] != NULL; l++)
						{
							if (x[k]->master[l] == master_ptr)
							{
								store_jacob0(x[k]->master[0]->unknown->
											 number, x[i]->number,
											 -elt_list[j].coef);
								store_sum_deltas(&delta[i],
												 &x[k]->master[0]->unknown->
												 delta, elt_list[j].coef);
								stop = TRUE;
								break;
							}
						}
						if (stop == TRUE)
							break;
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
build_pure_phases(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Includes calculation of inverse saturation index in sum_mb.
 *   Puts coefficients in iap and mass balance equations for each phase.
 */
	int i;
	int stop, j, k, l;
	char token[MAX_LENGTH];
	char *ptr;
	struct master *master_ptr;
	struct rxn_token *rxn_ptr;
/*
 *   Build into sums the logic to calculate inverse saturation indices for
 *   pure phases
 */
	if (pure_phase_unknown == NULL)
		return (OK);
/*
 *   Calculate inverse saturation index
 */
	for (i = 0; i < count_unknowns; i++)
	{
		if (x[i]->type != PP || x[i]->phase->rxn_x == NULL)
			continue;
		if (pure_phase_unknown == NULL)
			pure_phase_unknown = x[i];

		store_mb(&(x[i]->phase->lk), &(x[i]->f), 1.0);
		store_mb(&(x[i]->si), &(x[i]->f), 1.0);

		for (rxn_ptr = x[i]->phase->rxn_x->token + 1; rxn_ptr->s != NULL;
			 rxn_ptr++)
		{
			store_mb(&(rxn_ptr->s->la), &(x[i]->f), -rxn_ptr->coef);
		}
	}
	for (i = 0; i < count_unknowns; i++)
	{
/*
 *  rxn_x is null if an element in phase is not in solution
 */
		if (x[i]->type != PP || x[i]->phase->rxn_x == NULL)
			continue;
/*
 *   Put coefficients into IAP equations
 */
		for (rxn_ptr = x[i]->phase->rxn_x->token + 1; rxn_ptr->s != NULL;
			 rxn_ptr++)
		{
			if (rxn_ptr->s->secondary != NULL
				&& rxn_ptr->s->secondary->in == TRUE)
			{
				master_ptr = rxn_ptr->s->secondary;
			}
			else
			{
				master_ptr = rxn_ptr->s->primary;
			}
			if (master_ptr == NULL || master_ptr->unknown == NULL)
				continue;
			store_jacob0(x[i]->number, master_ptr->unknown->number,
						 rxn_ptr->coef);
		}
/*
 *   Put coefficients into mass balance equations
 */
		count_elts = 0;
		paren_count = 0;
		if (x[i]->pure_phase->add_formula != NULL)
		{
			strcpy(token, x[i]->pure_phase->add_formula);
			ptr = token;
			get_elts_in_species(&ptr, 1.0);
		}
		else
		{
			strcpy(token, x[i]->phase->formula);
			ptr = token;
			get_elts_in_species(&ptr, 1.0);
		}
/*
 *   Go through elements in phase
 */

#ifdef COMBINE
		change_hydrogen_in_elt_list(0);
#endif
		for (j = 0; j < count_elts; j++)
		{

			if (strcmp(elt_list[j].elt->name, "H") == 0
				&& mass_hydrogen_unknown != NULL)
			{
				store_jacob0(mass_hydrogen_unknown->number, x[i]->number,
							 -elt_list[j].coef);
				store_sum_deltas(&(delta[i]), &mass_hydrogen_unknown->delta,
								 elt_list[j].coef);

			}
			else if (strcmp(elt_list[j].elt->name, "O") == 0
					 && mass_oxygen_unknown != NULL)
			{
				store_jacob0(mass_oxygen_unknown->number, x[i]->number,
							 -elt_list[j].coef);
				store_sum_deltas(&(delta[i]), &mass_oxygen_unknown->delta,
								 elt_list[j].coef);

			}
			else
			{
				master_ptr = elt_list[j].elt->primary;
				if (master_ptr->in == FALSE)
				{
					master_ptr = master_ptr->s->secondary;
				}
				if (master_ptr == NULL || master_ptr->in == FALSE)
				{
					if (state != ADVECTION && state != TRANSPORT
						&& state != PHAST)
					{
						error_string = sformatf(
								"Element in phase, %s, is not in model.",
								x[i]->phase->name);
						warning_msg(error_string);
					}
					if (master_ptr != NULL)
					{
						master_ptr->s->la = -999.9;
					}
/*
 *   Master species is in model
 */
				}
				else if (master_ptr->in == TRUE)
				{
					store_jacob0(master_ptr->unknown->number, x[i]->number,
								 -elt_list[j].coef);
					store_sum_deltas(&delta[i], &master_ptr->unknown->delta,
									 elt_list[j].coef);
/*
 *   Master species in equation needs to be rewritten
 */
				}
				else if (master_ptr->in == REWRITE)
				{
					stop = FALSE;
					for (k = 0; k < count_unknowns; k++)
					{
						if (x[k]->type != MB)
							continue;
						for (l = 0; x[k]->master[l] != NULL; l++)
						{
							if (x[k]->master[l] == master_ptr)
							{
								store_jacob0(x[k]->master[0]->unknown->
											 number, x[i]->number,
											 -elt_list[j].coef);
								store_sum_deltas(&delta[i],
												 &x[k]->master[0]->unknown->
												 delta, elt_list[j].coef);
								stop = TRUE;
								break;
							}
						}
						if (stop == TRUE)
							break;
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
build_solution_phase_boundaries(void)
/* ---------------------------------------------------------------------- */
{
	int i;
	struct master *master_ptr;
	struct rxn_token *rxn_ptr;
/*
 *   Build into sums the logic to calculate inverse saturation indices for
 *   solution phase boundaries
 */
	if (solution_phase_boundary_unknown == NULL)
		return (OK);
/*
 *   Calculate inverse saturation index
 */
	for (i = 0; i < count_unknowns; i++)
	{
		if (x[i]->type != SOLUTION_PHASE_BOUNDARY)
			continue;

		store_mb(&(x[i]->phase->lk), &(x[i]->f), 1.0);
		store_mb(&(x[i]->si), &(x[i]->f), 1.0);
		if (x[i]->phase->in != TRUE)
		{
			error_string = sformatf(
					"Solution does not contain all elements for phase-boundary mineral, %s.",
					x[i]->phase->name);
			error_msg(error_string, CONTINUE);
			input_error++;
			break;
		}
		for (rxn_ptr = x[i]->phase->rxn_x->token + 1; rxn_ptr->s != NULL;
			 rxn_ptr++)
		{
			store_mb(&(rxn_ptr->s->la), &(x[i]->f), -rxn_ptr->coef);
		}
	}
	if (get_input_errors() > 0)
		return (ERROR);
/*
 *   Put coefficients into array
 */
	for (i = 0; i < count_unknowns; i++)
	{
		if (x[i]->type != SOLUTION_PHASE_BOUNDARY)
			continue;
		for (rxn_ptr = x[i]->phase->rxn_x->token + 1; rxn_ptr->s != NULL;
			 rxn_ptr++)
		{
			if (rxn_ptr->s->secondary != NULL
				&& rxn_ptr->s->secondary->in == TRUE)
			{
				master_ptr = rxn_ptr->s->secondary;
			}
			else
			{
				master_ptr = rxn_ptr->s->primary;
			}
			if (master_ptr->unknown == NULL)
				continue;
			store_jacob0(x[i]->number, master_ptr->unknown->number,
						 rxn_ptr->coef);
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
build_species_list(int n)
/* ---------------------------------------------------------------------- */
{
/*
 *   Builds a list that includes an entry for each master species in each
 *   secondary reaction. Used for summing species of each element and 
 *   printing results.
 */
	int j;
	struct master *master_ptr;
/*
 *   Check space and store reaction token name and pointer to species
 */
	if (count_species_list + count_elts >= max_species_list)
	{
		space((void **) ((void *) &species_list),
			  count_species_list + count_elts, &max_species_list,
			  sizeof(struct species_list));
	}
/*
 *   Treat species made only with H+, e-, and H2O specially
 */
	if (is_special(s[n]) == TRUE)
	{
		species_list[count_species_list].master_s = s_hplus;
		species_list[count_species_list].s = s[n];
		species_list[count_species_list].coef = 0.0;
		count_species_list++;
		return (OK);
	}
/*
 *   Treat exchange species specially
 */
	if (s[n]->type == EX)
	{
		if (s[n]->primary != NULL)
			return (OK);		/* master species has zero molality */
		for (j = 0; j < count_elts; j++)
		{
			if (elt_list[j].elt->master->s->type != EX)
				continue;
			master_ptr = elt_list[j].elt->master;
			species_list[count_species_list].master_s =
				elt_list[j].elt->master->s;
			species_list[count_species_list].s = s[n];
			species_list[count_species_list].coef = master_ptr->coef *
				elt_list[j].coef;
			count_species_list++;
		}
		return (OK);
	}
/*
 *   Treat surface species specially
 */
	if (s[n]->type == SURF_PSI)
		return (OK);
	if (s[n]->type == SURF)
	{
		for (j = 0; j < count_elts; j++)
		{
			if (elt_list[j].elt->master->s->type != SURF)
				continue;
			master_ptr = elt_list[j].elt->master;
			species_list[count_species_list].master_s =
				elt_list[j].elt->master->s;
			species_list[count_species_list].s = s[n];
			species_list[count_species_list].coef = master_ptr->coef *
				elt_list[j].coef;
			count_species_list++;
		}
		return (OK);
	}
/*
 *   Other aqueous species
 */
	for (j = 0; j < count_elts; j++)
	{
		if (is_special(elt_list[j].elt->master->s) == TRUE)
			continue;
		if (elt_list[j].elt->master->s->secondary != NULL)
		{
			master_ptr = elt_list[j].elt->master->s->secondary;
		}
		else
		{
			master_ptr = elt_list[j].elt->master->s->primary;
		}
		species_list[count_species_list].master_s = master_ptr->s;
		species_list[count_species_list].s = s[n];
/*
 *    Find coefficient for element represented by master species
 */
		species_list[count_species_list].coef = master_ptr->coef *
			elt_list[j].coef;
		count_species_list++;
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
clear(void)
/* ---------------------------------------------------------------------- */
{
	int i;
/*
 *   Resets information for setting up a new model
 */
	struct solution *solution_ptr;
/*
 *   Clear species solution-dependent data
 */
	solution_ptr = use.Get_solution_ptr();

	for (i = 0; i < count_s; i++)
	{
		s[i]->in = FALSE;
	}
/*
 *   Set pe structure
 */
	pe_data_free(pe_x);
	pe_x = pe_data_dup(solution_ptr->pe);
	default_pe_x = solution_ptr->default_pe;
/*
 *   Clear master species solution-dependent data
 */
	for (i = 0; i < count_master; i++)
	{
		master[i]->in = FALSE;
		master[i]->unknown = NULL;
		master[i]->pe_rxn = &(pe_x[solution_ptr->default_pe].rxn);
/*
 *   copy primary reaction to secondary reaction
 */
		rxn_free(master[i]->rxn_secondary);
		master[i]->rxn_secondary = rxn_dup(master[i]->rxn_primary);
	}

	if (state == INITIAL_SOLUTION)
	{
		s_h2o->secondary->in = TRUE;
		s_hplus->secondary->in = TRUE;
	}
	else
	{
		s_h2o->primary->in = TRUE;
		s_hplus->primary->in = TRUE;
	}
	s_eminus->primary->in = TRUE;
/*
 *   Set all unknown pointers to NULL
 */
	mb_unknown = NULL;
	ah2o_unknown = NULL;
	mass_hydrogen_unknown = NULL;
	mass_oxygen_unknown = NULL;
	mu_unknown = NULL;
	alkalinity_unknown = NULL;
	carbon_unknown = NULL;
	ph_unknown = NULL;
	pe_unknown = NULL;
	charge_balance_unknown = NULL;
	solution_phase_boundary_unknown = NULL;
	pure_phase_unknown = NULL;
	exchange_unknown = NULL;
	surface_unknown = NULL;
	gas_unknown = NULL;
	s_s_unknown = NULL;
/*
 *   Free arrays used in model   
 */
	free_model_allocs();

	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
convert_units(struct solution *solution_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Converts solution concentrations to moles/kg water
 *   Uses totals.input conc to calculate totals.moles.
 */
	int i, l;
	LDBLE sum_solutes;
	char c;
	struct master *master_ptr;
	struct conc *tot_ptr;
	char token[MAX_LENGTH];
/*
 *   Convert units
 */
	sum_solutes = exp(-solution_ptr->ph * LOG_10);
	for (i = 0; solution_ptr->totals[i].description != NULL; i++)
	{
		master_ptr = master_bsearch(solution_ptr->totals[i].description);
		if (master_ptr != NULL)
		{
			if (master_ptr->minor_isotope == TRUE)
				continue;
		}
		tot_ptr = &(solution_ptr->totals[i]);
		tot_ptr->moles = 0.0;
		if (strcmp(tot_ptr->description, "H(1)") == 0 ||
			strcmp(tot_ptr->description, "E") == 0)
		{
			continue;
		}
		if (tot_ptr->input_conc <= 0)
			continue;
/*
 *   Get gfw
 */
		/* use given gfw if gfw > 0.0 */
		/* use formula give with "as" */
		if (tot_ptr->gfw <= 0.0)
		{
			if (tot_ptr->as != NULL)
			{
				/* use given chemical formula to calculate gfw */
				if (compute_gfw(tot_ptr->as, &(tot_ptr->gfw)) == ERROR)
				{
					error_string = sformatf( "Could not compute gfw, %s.",
							tot_ptr->as);
					error_msg(error_string, CONTINUE);
					input_error++;
				}
				if (strcmp(tot_ptr->description, "Alkalinity") == 0 &&
					strcmp(tot_ptr->as, "CaCO3") == 0)
				{
					tot_ptr->gfw /= 2.;
					error_string = sformatf(
							"Equivalent wt for alkalinity should be Ca.5(CO3).5. Using %g g/eq.",
							(double) tot_ptr->gfw);
					warning_msg(error_string);
				}
				/* use gfw of master species */
			}
			else
			{
				char * temp_desc = string_duplicate(tot_ptr->description);
				char *ptr = temp_desc;
				copy_token(token, &ptr, &l);
				master_ptr = master_bsearch(token);
				free_check_null(temp_desc);
				if (master_ptr != NULL)
				{
					/* use gfw for element redox state */
					tot_ptr->gfw = master_ptr->gfw;
				}
				else
				{
					error_string = sformatf( "Could not find gfw, %s.",
							tot_ptr->description);
					error_msg(error_string, CONTINUE);
					input_error++;
					continue;
				}
			}
		}
/*
 *   Convert liters to kg solution
 */
		tot_ptr->moles = tot_ptr->input_conc;
		if (strstr(solution_ptr->units, "/l") != NULL)
		{
			tot_ptr->moles *= 1.0 / (solution_ptr->density);
		}
/*
 *   Convert milli or micro
 */
		c = tot_ptr->units[0];
		if (c == 'm')
		{
			tot_ptr->moles *= 1e-3;
		}
		else if (c == 'u')
		{
			tot_ptr->moles *= 1e-6;
		}
/*
 *   Sum grams of solute, convert from moles necessary
 */
		if (strstr(tot_ptr->units, "g/kgs") != NULL ||
			strstr(tot_ptr->units, "g/l") != NULL)
		{
			sum_solutes += tot_ptr->moles;
		}
		else if (strstr(tot_ptr->units, "Mol/kgs") != NULL ||
				 strstr(tot_ptr->units, "Mol/l") != NULL ||
				 strstr(tot_ptr->units, "eq/l") != NULL)
		{
			sum_solutes += (tot_ptr->moles) * (tot_ptr->gfw);
		}
/*
 *   Convert grams to moles, if necessary
 */
		if (strstr(tot_ptr->units, "g/") != NULL && tot_ptr->gfw != 0.0)
		{
			tot_ptr->moles /= tot_ptr->gfw;
		}
	}
/*
 *   Convert /kgs to /kgw
 */
	if (strstr(solution_ptr->units, "kgs") != NULL ||
		strstr(solution_ptr->units, "/l") != NULL)
	{
		mass_water_aq_x = 1.0 - 1e-3 * sum_solutes;
		for (i = 0; solution_ptr->totals[i].description != NULL; i++)
		{
			solution_ptr->totals[i].moles /= mass_water_aq_x;
		}
	}
/*
 *   Scale by mass of water in solution
 */
	mass_water_aq_x = solution_ptr->mass_water;
	for (i = 0; solution_ptr->totals[i].description != NULL; i++)
	{
		solution_ptr->totals[i].moles *= mass_water_aq_x;
	}

	solution_ptr->units = moles_per_kilogram_string;

	return (OK);
}

/* ---------------------------------------------------------------------- */
struct master ** Phreeqc::
get_list_master_ptrs(char *ptr, struct master *master_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Input: ptr contains a list of one or more master species names
 *   Output: space is allocated and a list of master species pointers is
 *           returned.
 */
	int j, l, count_list;
	char token[MAX_LENGTH];
	struct master **master_ptr_list;
	struct master *master_ptr0;
/*
 *   Make list of master species pointers
 */
	count_list = 0;
	master_ptr_list = unknown_alloc_master();
	master_ptr0 = master_ptr;
	if (master_ptr0 == master_ptr->s->primary)
	{
/*
 *   First in list is primary species
 */
		for (j = 0; j < count_master; j++)
		{
			if (master[j] == master_ptr0)
				break;
		}
		j++;
/*
 *   Element has only one valence
 */
		if (j >= count_master || master[j]->elt->primary != master_ptr0)
		{
			master_ptr_list[count_list++] = master_ptr0;
/*
 *   Element has multiple valences
 */
		}
		else
		{
			if (master_ptr0->s->secondary == NULL)
			{
				error_string = sformatf(
						"Master species for valence states of element %s are not correct.\n\tPossibly related to master species for %s.",
						master_ptr0->elt->name, master[j]->elt->name);
				error_msg(error_string, CONTINUE);
				input_error++;
			}
			master_ptr_list[count_list++] = master_ptr0->s->secondary;
			while (j < count_master && master[j]->elt->primary == master_ptr0)
			{
				if (master[j]->s->primary == NULL)
				{
					master_ptr_list =
						(struct master **) PHRQ_realloc((void *)
														master_ptr_list,
														(size_t) (count_list
																  +
																  2) *
														sizeof(struct master
															   *));
					if (master_ptr_list == NULL)
						malloc_error();
					master_ptr_list[count_list++] = master[j];
				}
				j++;
			}
		}
	}
	else
	{
/*
 *   First in list is secondary species, Include all valences from input
 */
		master_ptr_list[count_list++] = master_ptr0;
		while (copy_token(token, &ptr, &l) != EMPTY)
		{
			master_ptr = master_bsearch(token);
			if (master_ptr != NULL)
			{
				master_ptr_list =
					(struct master **) PHRQ_realloc((void *) master_ptr_list,
													(size_t) (count_list +
															  2) *
													sizeof(struct master *));
				if (master_ptr_list == NULL)
					malloc_error();
				master_ptr_list[count_list++] = master_ptr;
			}
		}
	}
	master_ptr_list[count_list] = NULL;
	return (master_ptr_list);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
inout(void)
/* ---------------------------------------------------------------------- */
{
	int i;
	struct rxn_token_temp *token_ptr;
/*
 *   Routine goes through trxn to determine if each master species is
 *   in this model.
 *   Assumes equation is written in terms of primary and secondary species
 *   Checks to see if in is TRUE or REWRITE for each species
 *   Returns TRUE if in model
 *           FALSE if not
 */
	for (i = 1; i < count_trxn; i++)
	{
		token_ptr = &(trxn.token[i]);
		/*   Check primary master species in */
		if (token_ptr->s->primary != NULL
			&& (token_ptr->s->primary->in == TRUE))
			continue;
		/*   Check secondary master species */
		if ((token_ptr->s->secondary != NULL)
			&& (token_ptr->s->secondary->in != FALSE))
		{
			continue;
		}
		/*   Must be primary master species that is out */
		return (FALSE);
	}
	return (TRUE);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
is_special(struct species *l_spec)
/* ---------------------------------------------------------------------- */
{
/*
 *   Checks to see if a species is composed of only H, O, and e-
 *   Returns TRUE if true
 *           FALSE if not
 */
	int special;
	struct rxn_token *token_ptr;

	special = TRUE;
	for (token_ptr = l_spec->rxn_s->token + 1; token_ptr->s != NULL;
		 token_ptr++)
	{
		if (token_ptr->s != s_hplus &&
			token_ptr->s != s_h2o && token_ptr->s != s_eminus)
		{
			special = FALSE;
			break;
		}
	}
	return (special);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
store_mb_unknowns(struct unknown *unknown_ptr, LDBLE * LDBLE_ptr, LDBLE coef,
				  LDBLE * gamma_ptr)
/* ---------------------------------------------------------------------- */
/*
 *   Takes an unknown pointer and a coefficient and puts in
 *   list of mb_unknowns
 */
{
	if (equal(coef, 0.0, TOL) == TRUE)
		return (OK);
	if ((count_mb_unknowns + 1) >= max_mb_unknowns)
	{
		space((void **) ((void *) &mb_unknowns), count_mb_unknowns + 1,
			  &max_mb_unknowns, sizeof(struct unknown_list));
	}
	mb_unknowns[count_mb_unknowns].unknown = unknown_ptr;
	mb_unknowns[count_mb_unknowns].source = LDBLE_ptr;
	mb_unknowns[count_mb_unknowns].gamma_source = gamma_ptr;
	mb_unknowns[count_mb_unknowns].coef = coef;
	count_mb_unknowns++;
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
mb_for_species_aq(int n)
/* ---------------------------------------------------------------------- */
{
/*
 *   Make list of mass balance and charge balance equations in which
 *   to insert species n. 
 *
 *        count_mb_unknowns - number of equations and summation relations
 *        mb_unknowns.unknown - pointer to unknown which contains row number
 *        mb_unknowns.source - pointer to the LDBLE number to be multiplied
 *                             by coef, usually moles.
 *        mb_unknowns.coef - coefficient of s[n] in equation or relation
 */
	int i, j;
	struct master *master_ptr;
	struct unknown *unknown_ptr;

	count_mb_unknowns = 0;
/*
 *   e- does not appear in any mass balances
 */
	if (s[n]->type == EMINUS)
		return (OK);
/* 
 *   Do not include diffuse layer in cb, alk, ah2o, mu
 */
	if (charge_balance_unknown != NULL && s[n]->type < H2O)
	{
		store_mb_unknowns(charge_balance_unknown, &s[n]->moles, s[n]->z,
						  &s[n]->dg);
	}
	if (alkalinity_unknown != NULL && s[n]->type < H2O)
	{
		store_mb_unknowns(alkalinity_unknown, &s[n]->moles, s[n]->alk,
						  &s[n]->dg);
	}
	if (ah2o_unknown != NULL && s[n]->type < H2O)
	{
		store_mb_unknowns(ah2o_unknown, &s[n]->moles, 1.0, &s[n]->dg);
	}
	if (mu_unknown != NULL && s[n]->type < H2O)
	{
		store_mb_unknowns(mu_unknown, &s[n]->moles, s[n]->z * s[n]->z,
						  &s[n]->dg);
	}
/* 
 *   Include diffuse layer in hydrogen and oxygen mass balance
 */
	if (mass_hydrogen_unknown != NULL)
	{
		if (dl_type_x != NO_DL && state >= REACTION)
		{
#ifdef COMBINE
			store_mb_unknowns(mass_hydrogen_unknown, &s[n]->tot_g_moles,
							  s[n]->h - 2 * s[n]->o, &s[n]->dg_total_g);
#else
			store_mb_unknowns(mass_hydrogen_unknown, &s[n]->tot_g_moles,
							  s[n]->h, &s[n]->dg_total_g);
#endif
		}
		else
		{
#ifdef COMBINE
			store_mb_unknowns(mass_hydrogen_unknown, &s[n]->moles,
							  s[n]->h - 2 * s[n]->o, &s[n]->dg);
#else
			store_mb_unknowns(mass_hydrogen_unknown, &s[n]->moles, s[n]->h,
							  &s[n]->dg);
#endif
		}
	}
	if (mass_oxygen_unknown != NULL)
	{
		if (dl_type_x != NO_DL && state >= REACTION)
		{
			store_mb_unknowns(mass_oxygen_unknown, &s[n]->tot_g_moles,
							  s[n]->o, &s[n]->dg_total_g);
		}
		else
		{
			store_mb_unknowns(mass_oxygen_unknown, &s[n]->moles, s[n]->o,
							  &s[n]->dg);
		}
	}
/* 
 *   Sum diffuse layer charge into (surface + DL) charge balance
 */
	if (use.Get_surface_ptr() != NULL && s[n]->type < H2O && dl_type_x != NO_DL)
	{
		j = 0;
		for (i = 0; i < count_unknowns; i++)
		{
			if (x[i]->type == SURFACE_CB)
			{
				unknown_ptr = x[i];
				if (use.Get_surface_ptr()->type == CD_MUSIC)
					unknown_ptr = x[i + 2];
				store_mb_unknowns(unknown_ptr, &s[n]->diff_layer[j].g_moles,
								  s[n]->z, &s[n]->diff_layer[j].dg_g_moles);
				j++;
			}
		}
	}
/*
 *   Other mass balances
 */
	for (i = 0; i < count_elts; i++)
	{
		if (elt_list[i].elt->master->s->type > AQ &&
			elt_list[i].elt->master->s->type < SOLID)
			continue;
		master_ptr = elt_list[i].elt->master;
		if (master_ptr->primary == TRUE)
		{
			if (master_ptr->s->secondary != NULL)
			{
				master_ptr = master_ptr->s->secondary;
			}
		}
		if (master_ptr->unknown == ph_unknown)
		{
			continue;
		}
		else if (master_ptr->unknown == pe_unknown)
		{
			continue;
		}
		else if (master_ptr->unknown == charge_balance_unknown)
		{
			continue;
		}
		else if (master_ptr->unknown == alkalinity_unknown)
		{
			continue;
		}
		else if (master_ptr->unknown->type == SOLUTION_PHASE_BOUNDARY)
		{
			continue;
		}
		if (dl_type_x != NO_DL && state >= REACTION)
		{
			store_mb_unknowns(master_ptr->unknown,
							  &s[n]->tot_g_moles,
							  elt_list[i].coef * master_ptr->coef,
							  &s[n]->dg_total_g);
		}
		else
		{
			store_mb_unknowns(master_ptr->unknown,
							  &s[n]->moles,
							  elt_list[i].coef * master_ptr->coef, &s[n]->dg);
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
mb_for_species_ex(int n)
/* ---------------------------------------------------------------------- */
{
/*
 *   Make list of mass balance and charge balance equations in which
 *   to insert exchange species n. 
 *
 *        count_mb_unknowns - number of equations and summation relations
 *        mb_unknowns.source - pointer to the LDBLE number to be multiplied
 *                             by coef, usually moles.
 *        mb_unknowns.unknown - pointer to unknown which contains row number
 *        mb_unknowns.coef - coefficient of s[n] in equation or relation
 */
	int i;
	struct master *master_ptr;
	count_mb_unknowns = 0;
/*
 *   Master species for exchange do not appear in any mass balances
 */
	if (s[n]->type == EX && s[n]->primary != NULL)
		return (OK);
/* 
 *   Include diffuse layer in hydrogen and oxygen mass balance
 */
	if (charge_balance_unknown != NULL)
	{
		store_mb_unknowns(charge_balance_unknown, &s[n]->moles, s[n]->z,
						  &s[n]->dg);
	}
	if (mass_hydrogen_unknown != NULL)
	{
#ifdef COMBINE
		store_mb_unknowns(mass_hydrogen_unknown, &s[n]->moles,
						  s[n]->h - 2 * s[n]->o, &s[n]->dg);
#else
		store_mb_unknowns(mass_hydrogen_unknown, &s[n]->moles, s[n]->h,
						  &s[n]->dg);
#endif
	}
	if (mass_oxygen_unknown != NULL)
	{
		store_mb_unknowns(mass_oxygen_unknown, &s[n]->moles, s[n]->o,
						  &s[n]->dg);
	}
/*
 *   Other mass balances
 */
	for (i = 0; i < count_elts; i++)
	{
		if (elt_list[i].elt->master->s->type > AQ &&
			elt_list[i].elt->master->s->type < SOLID)
			continue;
		master_ptr = elt_list[i].elt->master;
		if (master_ptr->primary == TRUE)
		{
			if (master_ptr->s->secondary != NULL)
			{
				master_ptr = master_ptr->s->secondary;
			}
		}
/*
 *   Special for ph_unknown, pe_unknown, and alkalinity_unknown
 */
		if (master_ptr->unknown == ph_unknown)
		{
			continue;
		}
		else if (master_ptr->unknown == pe_unknown)
		{
			continue;
		}
		else if (master_ptr->unknown == alkalinity_unknown)
		{
			continue;
		}
/*
 *   EX, sum exchange species only into EXCH mass balance in initial calculation
 *   into all mass balances in reaction calculation
 */
		if (state >= REACTION || master_ptr->s->type == EX)
		{
			store_mb_unknowns(master_ptr->unknown, &s[n]->moles,
							  elt_list[i].coef * master_ptr->coef, &s[n]->dg);
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
mb_for_species_surf(int n)
/* ---------------------------------------------------------------------- */
{
/*
 *   Make list of mass balance and charge balance equations in which
 *   to insert species n. 
 *
 *        count_mb_unknowns - number of equations and summation relations
 *        mb_unknowns.source - pointer to the LDBLE number to be multiplied
 *                             by coef, usually moles.
 *        mb_unknowns.unknown - pointer to unknown which contains row number
 *        mb_unknowns.coef - coefficient of s[n] in equation or relation
 */
	int i;
	struct master *master_ptr;

	count_mb_unknowns = 0;
/*
 *   Include in charge balance, if diffuse_layer_x == FALSE
 */
	if (charge_balance_unknown != NULL && dl_type_x == NO_DL)
	{
		store_mb_unknowns(charge_balance_unknown, &s[n]->moles, s[n]->z,
						  &s[n]->dg);
	}
/* 
 *   Include diffuse layer in hydrogen and oxygen mass balance
 */
	if (mass_hydrogen_unknown != NULL)
	{
#ifdef COMBINE
		store_mb_unknowns(mass_hydrogen_unknown, &s[n]->moles,
						  s[n]->h - 2 * s[n]->o, &s[n]->dg);
#else
		store_mb_unknowns(mass_hydrogen_unknown, &s[n]->moles, s[n]->h,
						  &s[n]->dg);
#endif
	}
	if (mass_oxygen_unknown != NULL)
	{
		store_mb_unknowns(mass_oxygen_unknown, &s[n]->moles, s[n]->o,
						  &s[n]->dg);
	}
/*
 *   Other mass balances
 */
/*
 *   Other mass balances
 */
	for (i = 0; i < count_elts; i++)
	{
/*   Skip H+, e-, and H2O */
		if (elt_list[i].elt->master->s->type > AQ &&
			elt_list[i].elt->master->s->type < SOLID)
			continue;
		master_ptr = elt_list[i].elt->master;
		if (master_ptr->primary == TRUE)
		{
			if (master_ptr->s->secondary != NULL)
			{
				master_ptr = master_ptr->s->secondary;
			}
		}
/*
 *   SURF_PSI, sum surface species in (surface + DL) charge balance
 */
		if (master_ptr->s->type == SURF_PSI
			&& use.Get_surface_ptr()->type != CD_MUSIC)
		{
			store_mb_unknowns(master_ptr->unknown, &s[n]->moles, s[n]->z,
							  &s[n]->dg);
			continue;
		}
		if (master_ptr->s->type == SURF_PSI
			&& use.Get_surface_ptr()->type == CD_MUSIC)
		{
			store_mb_unknowns(master_ptr->unknown, &s[n]->moles, s[n]->dz[0],
							  &s[n]->dg);
			continue;
		}
		if (master_ptr->s->type == SURF_PSI1)
		{
			store_mb_unknowns(master_ptr->unknown, &s[n]->moles, s[n]->dz[1],
							  &s[n]->dg);
			continue;
		}
		if (master_ptr->s->type == SURF_PSI2)
		{
			store_mb_unknowns(master_ptr->unknown, &s[n]->moles, s[n]->dz[2],
							  &s[n]->dg);
			/*
			   if (diffuse_layer_x == TRUE) {
			   store_mb_unknowns(master_ptr->unknown, &s[n]->moles, s[n]->z, &s[n]->dg ); 
			   } else {
			   store_mb_unknowns(master_ptr->unknown, &s[n]->moles, s[n]->dz[2], &s[n]->dg ); 
			   }
			 */
			continue;
		}
/*
 *   Special for ph_unknown, pe_unknown, and alkalinity_unknown
 */
		if (master_ptr->unknown == ph_unknown)
		{
			continue;
		}
		else if (master_ptr->unknown == pe_unknown)
		{
			continue;
		}
		else if (master_ptr->unknown == alkalinity_unknown)
		{
			continue;
		}
/*
 *   SURF, sum surface species only into SURFACE mass balance in initial calculation
 *   into all mass balances in reaction calculation
 */
		if (state >= REACTION || master_ptr->s->type == SURF)
		{
			store_mb_unknowns(master_ptr->unknown, &s[n]->moles,
							  elt_list[i].coef * master_ptr->coef, &s[n]->dg);
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
reprep(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   If a basis species has been switched, makes new model.
 *   Unknowns are not changed, but mass-action equations are
 *   rewritten and lists for mass balance and jacobian are regenerated
 */
	int i;
/*
 *   Initialize s, master, and unknown pointers
 */
	for (i = 0; i < count_master; i++)
	{
		if (master[i]->in == FALSE)
			continue;
		rxn_free(master[i]->rxn_secondary);
		master[i]->rxn_secondary = rxn_dup(master[i]->rxn_primary);
	}
	resetup_master();
/*
 *   Set unknown pointers, unknown types, validity checks
 */
	tidy_redox();
	if (get_input_errors() > 0)
	{
		error_msg("Program terminating due to input errors.", STOP);
	}
/*
 *   Free arrays built in build_model
 */
	s_x = (struct species **) free_check_null(s_x);
	sum_mb1 = (struct list1 *) free_check_null(sum_mb1);
	sum_mb2 = (struct list2 *) free_check_null(sum_mb2);
	sum_jacob0 = (struct list0 *) free_check_null(sum_jacob0);
	sum_jacob1 = (struct list1 *) free_check_null(sum_jacob1);
	sum_jacob2 = (struct list2 *) free_check_null(sum_jacob2);
	sum_delta = (struct list2 *) free_check_null(sum_delta);
/*
 *   Build model again
 */
	build_model();
	same_model = FALSE;
	k_temp(tc_x, patm_x);

	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
resetup_master(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   For basis switch, rewrite equations for master species
 *   Set master_ptr->rxn_secondary,
 *       master_ptr->pe_rxn,
 *       and special cases for alkalinity, carbon, and pH.
 */
	int i, j;
	struct master *master_ptr, *master_ptr0;

	for (i = 0; i < count_unknowns; i++)
	{
		if (x[i]->type != MB)
			continue;
		master_ptr0 = x[i]->master[0];
		for (j = 0; (master_ptr = x[i]->master[j]) != NULL; j++)
		{
/*
 *   Set flags
 */
			if (j == 0)
			{
				if (master_ptr->s->primary == NULL)
				{
					rxn_free(master_ptr->rxn_secondary);
					master_ptr->rxn_secondary = rxn_dup(master_ptr->s->rxn_s);
				}
			}
			else
			{
				if (master_ptr0->s->primary == NULL)
				{
					rewrite_master_to_secondary(master_ptr, master_ptr0);
					rxn_free(master_ptr->rxn_secondary);
					master_ptr->rxn_secondary = rxn_alloc(count_trxn + 1);
					trxn_copy(master_ptr->rxn_secondary);
				}
			}
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
write_mass_action_eqn_x(int stop)
/* ---------------------------------------------------------------------- */
{
/*
 *   Reduce mass-action equation to the master species that are in the model
 */
	LDBLE coef_e;
	int count, repeat;
	int i, count_rxn_orig;
/*
 *   Rewrite any secondary master species flagged REWRITE
 *   Replace pe if necessary
 */
	count = 0;
	repeat = TRUE;
	while (repeat == TRUE)
	{
		count++;
		if (count > MAX_ADD_EQUATIONS)
		{
			error_string = sformatf( "Could not reduce equation "
					"to primary and secondary species that are "
					"in the model\n\t Species: %s.", trxn.token[0].s->name);
			if (stop == STOP)
			{
				input_error++;
				error_msg(error_string, CONTINUE);
			}
			else
			{
				warning_msg(error_string);
			}
			return (ERROR);
		}
		repeat = FALSE;
		count_rxn_orig = count_trxn;
		for (i = 1; i < count_rxn_orig; i++)
		{
			if (trxn.token[i].s->secondary == NULL)
				continue;
			if (trxn.token[i].s->secondary->in == REWRITE)
			{
				repeat = TRUE;
				coef_e =
					rxn_find_coef(trxn.token[i].s->secondary->rxn_secondary,
								  "e-");
				trxn_add(trxn.token[i].s->secondary->rxn_secondary,
						 trxn.token[i].coef, FALSE);
				if (equal(coef_e, 0.0, TOL) == FALSE)
				{
					trxn_add(*(trxn.token[i].s->secondary->pe_rxn),
							 trxn.token[i].coef * coef_e, FALSE);
				}
			}
		}
		trxn_combine();
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
add_potential_factor(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Add the potential factor to surface mass-action equations.
 *   Factor is essentially the activity coefficient, representing
 *   the work required to bring charged ions to the surface
 */
	int i;
	char token[MAX_LENGTH];
	LDBLE sum_z;
	struct master *master_ptr;
	struct unknown *unknown_ptr;

	/*if (use.Get_surface_ptr()->edl == FALSE) return(OK); */
	if (use.Get_surface_ptr()->type != DDL)
		return (OK);
	sum_z = 0.0;
	master_ptr = NULL;
/*
 *   Find sum of charge of aqueous species and surface master species
 */
	for (i = 1; i < count_trxn; i++)
	{
		if (trxn.token[i].s->type == AQ || trxn.token[i].s == s_hplus ||
			trxn.token[i].s == s_eminus)
		{
			sum_z += trxn.token[i].s->z * trxn.token[i].coef;
		}
		if (trxn.token[i].s->type == SURF)
		{
			master_ptr = trxn.token[i].s->primary;
		}
	}
/*
 *  Find potential unknown for surface species
 */
	if (master_ptr == NULL)
	{
		error_string = sformatf(
				"Did not find a surface species in equation defining %s",
				trxn.token[0].name);
		error_msg(error_string, CONTINUE);
		error_string = sformatf(
				"One of the following must be defined with SURFACE_SPECIES:");
		error_msg(error_string, CONTINUE);
		for (i = 1; i < count_trxn; i++)
		{
			error_string = sformatf( "     %s", trxn.token[i].name);
			error_msg(error_string, CONTINUE);
		}
		input_error++;
		return (ERROR);
	}
	strcpy(token, master_ptr->elt->name);
	unknown_ptr = find_surface_charge_unknown(token, SURF_PSI);
	if (unknown_ptr == NULL)
	{
		error_string = sformatf(
				"No potential unknown found for surface species %s.", token);
		error_msg(error_string, STOP);
	}
	master_ptr = unknown_ptr->master[0];	/* potential for surface component */
/*
 *   Make sure there is space
 */
	if (count_trxn + 1 >= max_trxn)
	{
		space((void **) ((void *) &(trxn.token)), count_trxn + 1, &max_trxn,
			  sizeof(struct rxn_token_temp));
	}
/*
 *   Include psi in mass action equation
 */
	if (master_ptr != NULL)
	{
		trxn.token[count_trxn].name = master_ptr->s->name;
		trxn.token[count_trxn].s = master_ptr->s;
		trxn.token[count_trxn].coef = -2.0 * sum_z;
		count_trxn++;
	}
	else
	{
		output_msg(sformatf(
				   "How did this happen in add potential factor?\n"));
	}

	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
add_cd_music_factors(int n)
/* ---------------------------------------------------------------------- */
{
/*
 *   Add the potential factors for cd_music to surface mass-action equations.
 *   Factors are essentially the activity coefficient, representing
 *   the work required to bring charged ions to the three charge layers 
 *   of the cd_music model
 */
	int i;
	char token[MAX_LENGTH];
	struct master *master_ptr;
	struct unknown *unknown_ptr;

	/*if (use.Get_surface_ptr()->edl == FALSE) return(OK); */
	if (use.Get_surface_ptr()->type != CD_MUSIC)
		return (OK);
	master_ptr = NULL;
/*
 *   Find sum of charge of aqueous species and surface master species
 */
	for (i = 1; i < count_trxn; i++)
	{
		if (trxn.token[i].s->type == SURF)
		{
			master_ptr = trxn.token[i].s->primary;
		}
	}
/*
 *  Find potential unknown for surface species
 */
	if (master_ptr == NULL)
	{
		error_string = sformatf(
				"Did not find a surface species in equation defining %s",
				trxn.token[0].name);
		error_msg(error_string, CONTINUE);
		error_string = sformatf(
				"One of the following must be defined with SURFACE_SPECIES:");
		error_msg(error_string, CONTINUE);
		for (i = 1; i < count_trxn; i++)
		{
			error_string = sformatf( "     %s", trxn.token[i].name);
			error_msg(error_string, CONTINUE);
		}
		input_error++;
		return (ERROR);
	}
	strcpy(token, master_ptr->elt->name);
	/*
	 *  Plane 0
	 */
	unknown_ptr = find_surface_charge_unknown(token, SURF_PSI);
	if (unknown_ptr == NULL)
	{
		error_string = sformatf(
				"No potential unknown found for surface species %s.", token);
		error_msg(error_string, STOP);
	}
	master_ptr = unknown_ptr->master[0];	/* potential for surface component */
	/*
	 *   Make sure there is space
	 */
	if (count_trxn + 3 >= max_trxn)
	{
		space((void **) ((void *) &(trxn.token)), count_trxn + 3, &max_trxn,
			  sizeof(struct rxn_token_temp));
	}
	/*
	 *   Include psi in mass action equation
	 */
	trxn.token[count_trxn].name = master_ptr->s->name;
	trxn.token[count_trxn].s = master_ptr->s;
	/*trxn.token[count_trxn].coef = s[n]->dz[0];*/
	trxn.token[count_trxn].coef = trxn.dz[0];

	count_trxn++;

	/*
	 *  Plane 1
	 */
	unknown_ptr = find_surface_charge_unknown(token, SURF_PSI1);
	if (unknown_ptr == NULL)
	{
		error_string = sformatf(
				"No potential unknown found for surface species %s.", token);
		error_msg(error_string, STOP);
	}
	master_ptr = unknown_ptr->master[0];	/* potential for surface component */
	/*
	 *   Include psi in mass action equation
	 */
	trxn.token[count_trxn].name = master_ptr->s->name;
	trxn.token[count_trxn].s = master_ptr->s;
	/*trxn.token[count_trxn].coef = s[n]->dz[1];*/
	trxn.token[count_trxn].coef = trxn.dz[1];
	count_trxn++;
	/*
	 *  Plane 2
	 */
	unknown_ptr = find_surface_charge_unknown(token, SURF_PSI2);
	if (unknown_ptr == NULL)
	{
		error_string = sformatf(
				"No potential unknown found for surface species %s.", token);
		error_msg(error_string, STOP);
	}
	master_ptr = unknown_ptr->master[0];	/* potential for surface component */
	/*
	 *   Include psi in mass action equation
	 */
	trxn.token[count_trxn].name = master_ptr->s->name;
	trxn.token[count_trxn].s = master_ptr->s;
	/*trxn.token[count_trxn].coef = s[n]->dz[2];*/
	trxn.token[count_trxn].coef = trxn.dz[2];
	count_trxn++;

	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
add_surface_charge_balance(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Include charge balance in list for mass-balance equations
 */
	int i;
	char *ptr;
	char token[MAX_LENGTH];

	struct master *master_ptr;
	struct unknown *unknown_ptr;

	if (use.Get_surface_ptr()->type != DDL)
		return (OK);
	master_ptr = NULL;
/*
 *   Find master species
 */
	for (i = 0; i < count_elts; i++)
	{
		if (elt_list[i].elt->primary->s->type == SURF)
		{
			master_ptr = elt_list[i].elt->primary;
			break;
		}
	}
	if (i >= count_elts)
	{
		error_string = sformatf(
				"No surface master species found for surface species.");
		error_msg(error_string, STOP);
	}
/*
 *  Find potential unknown for surface species
 */
	strcpy(token, master_ptr->elt->name);
	unknown_ptr = find_surface_charge_unknown(token, SURF_PSI);
	if (unknown_ptr == NULL)
	{
		error_string = sformatf(
				"No potential unknown found for surface species %s.", token);
		error_msg(error_string, STOP);
	}
	master_ptr = unknown_ptr->master[0];	/* potential for surface component */
/*
 *   Include charge balance in list for mass-balance equations
 */
	char * temp_name = string_duplicate(master_ptr->elt->name);
	ptr = temp_name;
	get_secondary_in_species(&ptr, 1.0);
	free_check_null(temp_name);

	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
add_cd_music_charge_balances(int n)
/* ---------------------------------------------------------------------- */
{
/*
 *   Add the potential factor to surface mass-action equations.
 *   Factor is essentially the activity coefficient, representing
 *   the work required to bring charged ions to the surface
 */
	int i;
	char token[MAX_LENGTH];

	struct master *master_ptr;
	struct unknown *unknown_ptr;

	if (use.Get_surface_ptr()->type != CD_MUSIC)
		return (OK);
	master_ptr = NULL;
/*
 *   Find master species
 */
	for (i = 0; i < count_elts; i++)
	{
		if (elt_list[i].elt->primary->s->type == SURF)
		{
			master_ptr = elt_list[i].elt->primary;
			break;
		}
	}
	if (i >= count_elts)
	{
		error_string = sformatf(
				"No surface master species found for surface species.");
		error_msg(error_string, STOP);
	}
	/*
	 *  Find potential unknown for plane 0
	 */
	strcpy(token, master_ptr->elt->name);
	unknown_ptr = find_surface_charge_unknown(token, SURF_PSI);
	master_ptr = unknown_ptr->master[0];	/* potential for surface component */
	/*
	 *   Include charge balance in list for mass-balance equations
	 */
	{
		char * temp_name = string_duplicate( master_ptr->elt->name);
		char *ptr = temp_name;
		get_secondary_in_species(&ptr, s[n]->dz[0]);
		free_check_null(temp_name);
	}
	/*
	 *  Find potential unknown for plane 1
	 */
	strcpy(token, master_ptr->elt->name);
	unknown_ptr = find_surface_charge_unknown(token, SURF_PSI1);
	master_ptr = unknown_ptr->master[0];	/* potential for surface component */
	/*
	 *   Include charge balance in list for mass-balance equations
	 */
	{
		char * temp_name = string_duplicate( master_ptr->elt->name);
		char *ptr = temp_name;
		get_secondary_in_species(&ptr, s[n]->dz[1]);
		free_check_null(temp_name);
	}
	/*
	 *  Find potential unknown for plane 2
	 */
	strcpy(token, master_ptr->elt->name);
	unknown_ptr = find_surface_charge_unknown(token, SURF_PSI2);
	master_ptr = unknown_ptr->master[0];	/* potential for surface component */
	/*
	 *   Include charge balance in list for mass-balance equations
	 */
	{
		char * temp_name = string_duplicate(master_ptr->elt->name);
		char *ptr = temp_name;
		get_secondary_in_species(&ptr, s[n]->dz[2]);
		free_check_null(temp_name);
	}

	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
rewrite_master_to_secondary(struct master *master_ptr1,
							struct master *master_ptr2)
/* ---------------------------------------------------------------------- */
{
/*
 *   Write equation for secondary master species in terms of another secondary master species
 *   Store result in rxn_secondary of master_ptr.
 */
	LDBLE coef1, coef2;
	struct master *master_ptr_p1, *master_ptr_p2;
/*
 *   Check that the two master species have the same primary master species
 */
	master_ptr_p1 = master_ptr1->elt->primary;
	master_ptr_p2 = master_ptr2->elt->primary;
	if (master_ptr_p1 != master_ptr_p2 || master_ptr_p1 == NULL)
	{
		error_string = sformatf(
				"All redox states must be for the same element. %s\t%s.",
				master_ptr1->elt->name, master_ptr2->elt->name);
		error_msg(error_string, CONTINUE);
		input_error++;
		return (ERROR);
	}
/*
 *   Find coefficient of primary master in reaction
 */
	coef1 = rxn_find_coef(master_ptr1->rxn_primary, master_ptr_p1->s->name);
	coef2 = rxn_find_coef(master_ptr2->rxn_primary, master_ptr_p1->s->name);
	if (equal(coef1, 0.0, TOL) == TRUE || equal(coef2, 0.0, TOL) == TRUE)
	{
		error_string = sformatf(
				"One of these equations does not contain master species for element, %s or %s.",
				master_ptr1->s->name, master_ptr2->s->name);
		error_msg(error_string, CONTINUE);
		input_error++;
		return (ERROR);
	}
/*
 *   Rewrite equation to secondary master species
 */
	count_trxn = 0;
	trxn_add(master_ptr1->rxn_primary, 1.0, FALSE);
	trxn_add(master_ptr2->rxn_primary, -coef1 / coef2, TRUE);
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
setup_exchange(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Fill in data for exchanger in unknowns structures
 */
	//int i, j;
	struct master *master_ptr;
	struct master **master_ptr_list;

	if (use.Get_exchange_ptr() == NULL)
		return (OK);
	std::vector<cxxExchComp *> comps = ((cxxExchange *) use.Get_exchange_ptr())->Vectorize();
	for (size_t j = 0; j < comps.size(); j++)
	{
		cxxExchComp * comp_ptr = comps[j];
		cxxNameDouble nd(comp_ptr->Get_totals());
		cxxNameDouble::iterator it = nd.begin();
		for ( ; it != nd.end(); it++)
		{
/*
 *   Find master species
 */
			//master_ptr = use.Get_exchange_ptr()->comps[j].totals[i].elt->master;
			element * elt_ptr = element_store(it->first.c_str());
			if (elt_ptr == NULL || elt_ptr->master == NULL)
			{
				error_string = sformatf( "Master species not in data "
						"base for %s, skipping element.",
						it->first.c_str());
				input_error++;
				error_msg(error_string, CONTINUE);
				continue;
			}
			master_ptr = elt_ptr->master;
			if (master_ptr->type != EX)
				continue;
/*
 *   Check for data already given
 */
			if (master_ptr->in != FALSE)
			{
				x[master_ptr->unknown->number]->moles +=
					it->second;
			}
			else
			{
/*
 *   Set flags
 */
				master_ptr_list = unknown_alloc_master();
				master_ptr_list[0] = master_ptr;
				master_ptr->in = TRUE;
/*
 *   Set unknown data
 */
				x[count_unknowns]->type = EXCH;
				x[count_unknowns]->exch_comp = string_hsave(it->first.c_str());
				x[count_unknowns]->description = elt_ptr->name;
				x[count_unknowns]->moles = it->second;
				x[count_unknowns]->master = master_ptr_list;
				x[count_unknowns]->master[0]->unknown = x[count_unknowns];
				count_unknowns++;
			}
		}
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
setup_gas_phase(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Fill in data for gas phase unknown (sum of partial pressures)
 *   in unknown structure
 */
	//int i;
	if (use.Get_gas_phase_ptr() == NULL)
		return (OK);
	cxxGasPhase * gas_phase_ptr = (cxxGasPhase *) use.Get_gas_phase_ptr();
	if (gas_phase_ptr->Get_type() == cxxGasPhase::GP_VOLUME && (
		gas_phase_ptr->Get_pr_in() || force_numerical_fixed_volume) && numerical_fixed_volume)
	{
		return setup_fixed_volume_gas();
	}

/*
 *   One for total moles in gas
 */
	x[count_unknowns]->type = GAS_MOLES;
	x[count_unknowns]->description = string_hsave("gas moles");
	x[count_unknowns]->moles = 0.0;
	for (size_t i = 0; i < gas_phase_ptr->Get_gas_comps().size(); i++)
	{	
		cxxGasComp *gc_ptr = &(gas_phase_ptr->Get_gas_comps()[i]);
		x[count_unknowns]->moles += gc_ptr->Get_moles();
	}
	if (x[count_unknowns]->moles <= 0)
		x[count_unknowns]->moles = MIN_TOTAL;
	x[count_unknowns]->ln_moles = log(x[count_unknowns]->moles);
	//x[count_unknowns]->gas_phase = use.Get_gas_phase_ptr();
	gas_unknown = x[count_unknowns];
	count_unknowns++;
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
setup_slack(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Fill in data for slack unknown
 */
	slack_unknown = NULL;
	if (slack)
	{
		x[count_unknowns]->type = SLACK;
		x[count_unknowns]->description = string_hsave("slack");
		x[count_unknowns]->moles = 0.0;
		x[count_unknowns]->number = count_unknowns;
		slack_unknown = x[count_unknowns];
		count_unknowns++;
	}
	return (OK);
}

#ifdef SKIP
/* ---------------------------------------------------------------------- */
int Phreeqc::
setup_slack(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Fill in data for slack unknown
 */

	slack_unknown = NULL;
	if (slack)
	{
		int i = count_unknowns;
		int j;
		for (j = 0; j < i; j++)
		{
			if (x[j]->type == MB)
			{
				x[count_unknowns]->type = SLACK;
				x[count_unknowns]->description = string_hsave("slack");
				x[count_unknowns]->moles = 0.0;
				x[count_unknowns]->number = count_unknowns;
				x[count_unknowns]->mb_number = j;
				slack_unknown = x[count_unknowns];
				count_unknowns++;
			}
		}
		if (count_unknowns > i)
		{
			slack_unknown = x[i];
		}
	}
	
	return (OK);
}
#endif
/* ---------------------------------------------------------------------- */
int Phreeqc::
setup_ss_assemblage(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Fill in data for solid solution unknowns (sum of partial pressures)
 *   in unknown structure
 */
	int i, j;
	if (use.Get_ss_assemblage_ptr() == NULL)
		return (OK);
/*
 *   One for each component in each solid solution
 */
	s_s_unknown = NULL;
	for (j = 0; j < use.Get_ss_assemblage_ptr()->count_s_s; j++)
	{
		for (i = 0; i < use.Get_ss_assemblage_ptr()->s_s[j].count_comps; i++)
		{
			x[count_unknowns]->type = S_S_MOLES;
			x[count_unknowns]->description =
				string_hsave(use.Get_ss_assemblage_ptr()->s_s[j].comps[i].name);
			x[count_unknowns]->moles = 0.0;
			if (use.Get_ss_assemblage_ptr()->s_s[j].comps[i].moles <= 0)
			{
				use.Get_ss_assemblage_ptr()->s_s[j].comps[i].moles = MIN_TOTAL_SS;
			}
			x[count_unknowns]->moles =
				use.Get_ss_assemblage_ptr()->s_s[j].comps[i].moles;
			use.Get_ss_assemblage_ptr()->s_s[j].comps[i].initial_moles =
				x[count_unknowns]->moles;
			x[count_unknowns]->ln_moles = log(x[count_unknowns]->moles);
			x[count_unknowns]->s_s = &(use.Get_ss_assemblage_ptr()->s_s[j]);
			x[count_unknowns]->s_s_comp =
				&(use.Get_ss_assemblage_ptr()->s_s[j].comps[i]);
			x[count_unknowns]->s_s_comp_number = i;
			x[count_unknowns]->phase =
				use.Get_ss_assemblage_ptr()->s_s[j].comps[i].phase;
			x[count_unknowns]->number = count_unknowns;
			x[count_unknowns]->phase->dn =
				use.Get_ss_assemblage_ptr()->s_s[j].comps[i].dn;
			x[count_unknowns]->phase->dnb =
				use.Get_ss_assemblage_ptr()->s_s[j].comps[i].dnb;
			x[count_unknowns]->phase->dnc =
				use.Get_ss_assemblage_ptr()->s_s[j].comps[i].dnc;
			x[count_unknowns]->phase->log10_fraction_x =
				use.Get_ss_assemblage_ptr()->s_s[j].comps[i].log10_fraction_x;
			x[count_unknowns]->phase->log10_lambda =
				use.Get_ss_assemblage_ptr()->s_s[j].comps[i].log10_lambda;
			if (s_s_unknown == NULL)
				s_s_unknown = x[count_unknowns];
			count_unknowns++;
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
setup_surface(void)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Fill in data for surface assemblage in unknown structure
	 */
	int i, j, k, plane;
	struct master *master_ptr;
	struct master **master_ptr_list;
	struct unknown *unknown_ptr, **unknown_target;
	char token[MAX_LENGTH], cb_suffix[MAX_LENGTH], psi_suffix[MAX_LENGTH],
		mass_balance_name[MAX_LENGTH];
	const char *name1, *name2;
	int mb_unknown_number, type;

	if (use.Get_surface_ptr() == NULL)
		return (OK);

	for (i = 0; i < use.Get_surface_ptr()->count_comps; i++)
	{
		/*
		 *   Find master species for each surface, setup unknown structure
		 */
		for (j = 0; use.Get_surface_ptr()->comps[i].totals[j].elt != NULL; j++)
		{
			master_ptr = use.Get_surface_ptr()->comps[i].totals[j].elt->master;
			if (master_ptr == NULL)
			{
				error_string = sformatf(
						"Master species not in data base for %s, skipping element.",
						use.Get_surface_ptr()->comps[i].totals[j].elt->name);
				warning_msg(error_string);
				continue;
			}
			if (master_ptr->type != SURF)
				continue;
			/*
			 *   Check that data not already given
			 */ if (master_ptr->in != FALSE)
			{
				error_string = sformatf(
						"Analytical data entered twice for %s.",
						master_ptr->s->name);
				error_msg(error_string, CONTINUE);
				input_error++;
				continue;
			}
			/*
			 *   Set flags
			 */
			use.Get_surface_ptr()->comps[i].master = master_ptr;
			master_ptr_list = unknown_alloc_master();
			master_ptr_list[0] = master_ptr;
			master_ptr->in = TRUE;
			/*
			 *   Setup mass balance unknown
			 */
			x[count_unknowns]->type = SURFACE;
			x[count_unknowns]->description =
				use.Get_surface_ptr()->comps[i].totals[j].elt->name;
			x[count_unknowns]->number = count_unknowns;
			x[count_unknowns]->surface_comp = &(use.Get_surface_ptr()->comps[i]);
			x[count_unknowns]->master = master_ptr_list;
			x[count_unknowns]->master[0]->unknown = x[count_unknowns];
			x[count_unknowns]->moles =
				use.Get_surface_ptr()->comps[i].totals[j].coef;
			if (surface_unknown == NULL)
				surface_unknown = x[count_unknowns];
			x[count_unknowns]->potential_unknown = NULL;
			count_unknowns++;
			/*if (use.Get_surface_ptr()->edl == FALSE) continue; */
			if (use.Get_surface_ptr()->type == DDL)
			{
				/*
				 *   Setup surface-potential unknown
				 */
				strcpy(token, master_ptr->elt->name);
				unknown_ptr = find_surface_charge_unknown(token, SURF_PSI);
				if (unknown_ptr != NULL)
				{
					x[count_unknowns - 1]->potential_unknown = unknown_ptr;
				}
				else
				{
					/*
					 *   Find master species
					 */
					replace("_CB", "_psi", token);
					master_ptr = master_bsearch(token);
					master_ptr_list = unknown_alloc_master();
					master_ptr_list[0] = master_ptr;
					master_ptr->in = TRUE;
					/*
					 *   Find surface charge structure
					 */
					x[count_unknowns]->type = SURFACE_CB;
					k = use.Get_surface_ptr()->comps[i].charge;
					x[count_unknowns]->surface_charge =
						&use.Get_surface_ptr()->charge[k];
					x[count_unknowns]->related_moles =
						x[count_unknowns]->surface_charge->grams;
					x[count_unknowns]->mass_water =
						use.Get_surface_ptr()->charge[k].mass_water;
					replace("_psi", "_CB", token);
					x[count_unknowns]->description = string_hsave(token);
					x[count_unknowns]->master = master_ptr_list;
					/*use.Get_surface_ptr()->charge[k].psi_master = x[count_unknowns]->master[0]; */
					x[count_unknowns]->master[0]->unknown = x[count_unknowns];
					x[count_unknowns]->moles = 0.0;
					x[count_unknowns - 1]->potential_unknown =
						x[count_unknowns];
					x[count_unknowns]->surface_comp =
						x[count_unknowns - 1]->surface_comp;
					count_unknowns++;
				}
			}
			else if (use.Get_surface_ptr()->type == CD_MUSIC)
			{
				/*
				 *   Setup 3 surface-potential unknowns
				 */
				mb_unknown_number = count_unknowns - 1;
				strcpy(token, master_ptr->elt->name);
				strcpy(mass_balance_name, token);
				for (plane = SURF_PSI; plane <= SURF_PSI2; plane++)
				{
					strcpy(cb_suffix, "_CB");
					strcpy(psi_suffix, "_psi");
					unknown_target = NULL;
					type = SURFACE_CB;
					switch (plane)
					{
					case SURF_PSI:
						type = SURFACE_CB;
						unknown_target =
							&(x[mb_unknown_number]->potential_unknown);
						break;
					case SURF_PSI1:
						strcat(cb_suffix, "b");
						strcat(psi_suffix, "b");
						type = SURFACE_CB1;
						unknown_target =
							&(x[mb_unknown_number]->potential_unknown1);
						break;
					case SURF_PSI2:
						strcat(cb_suffix, "d");
						strcat(psi_suffix, "d");
						type = SURFACE_CB2;
						unknown_target =
							&(x[mb_unknown_number]->potential_unknown2);
						break;
					}
					unknown_ptr = find_surface_charge_unknown(token, plane);
					if (unknown_ptr != NULL)
					{
						*unknown_target = unknown_ptr;
					}
					else
					{
						/*
						 *   Find master species
						 */
						replace(cb_suffix, psi_suffix, token);
						master_ptr = master_bsearch(token);
						master_ptr_list = unknown_alloc_master();
						master_ptr_list[0] = master_ptr;
						master_ptr->in = TRUE;
						/*
						 *   Find surface charge structure
						 */
						x[count_unknowns]->type = type;
						k = use.Get_surface_ptr()->comps[i].charge;
						x[count_unknowns]->surface_charge =
							&use.Get_surface_ptr()->charge[k];
						x[count_unknowns]->related_moles =
							x[count_unknowns]->surface_charge->grams;
						x[count_unknowns]->mass_water =
							use.Get_surface_ptr()->charge[k].mass_water;
						replace(psi_suffix, cb_suffix, token);
						x[count_unknowns]->description = string_hsave(token);
						x[count_unknowns]->master = master_ptr_list;
						/*
						 *   Find surface charge structure
						 */
						if (plane == SURF_PSI)
						{
							/*use.Get_surface_ptr()->charge[k].psi_master = x[count_unknowns]->master[0]; */
							x[mb_unknown_number]->potential_unknown =
								x[count_unknowns];
						}
						else if (plane == SURF_PSI1)
						{
							/*use.Get_surface_ptr()->charge[k].psi_master1 = x[count_unknowns]->master[0]; */
							x[mb_unknown_number]->potential_unknown1 =
								x[count_unknowns];
						}
						else if (plane == SURF_PSI2)
						{
							/*use.Get_surface_ptr()->charge[k].psi_master2 = x[count_unknowns]->master[0]; */
							x[mb_unknown_number]->potential_unknown2 =
								x[count_unknowns];
						}
						x[count_unknowns]->master[0]->unknown =
							x[count_unknowns];
						x[count_unknowns]->moles = 0.0;
						x[count_unknowns]->surface_comp =
							x[mb_unknown_number]->surface_comp;
						count_unknowns++;
					}
				}
				/* Add SURFACE unknown to a list for SURF_PSI */
				unknown_ptr = find_surface_charge_unknown(token, SURF_PSI);
				unknown_ptr->comp_unknowns =
					(struct unknown **) PHRQ_realloc(unknown_ptr->
													 comp_unknowns,
													 (size_t) ((unknown_ptr->
																count_comp_unknowns
																+
																1) *
															   sizeof(struct
																	  unknown
																	  *)));
				if (unknown_ptr->comp_unknowns == NULL)
					malloc_error();
				unknown_ptr->comp_unknowns[unknown_ptr->
										   count_comp_unknowns++] =
					x[mb_unknown_number];
			}
		}
	}
	/*
	 *   check related phases
	 */
	if (use.Get_surface_ptr()->related_phases == TRUE)
	{
		for (i = 0; i < count_unknowns; i++)
		{
			if (x[i]->type != SURFACE_CB)
				continue;
			for (j = 0; j < count_unknowns; j++)
			{
				if (x[j]->type != SURFACE)
					continue;
				if (x[j]->potential_unknown != x[i])
					continue;
				if (x[j]->surface_comp->phase_name !=
					x[i]->surface_comp->phase_name)
				{
					if (x[i]->surface_comp->phase_name == NULL)
					{
						name1 = string_hsave("None");
					}
					else
					{
						name1 = x[i]->surface_comp->phase_name;
					}
					if (x[j]->surface_comp->phase_name == NULL)
					{
						name2 = string_hsave("None");
					}
					else
					{
						name2 = x[j]->surface_comp->phase_name;
					}
					input_error++;
					error_string = sformatf(
							"All surface sites for a single component must be related to the same phase.\n\tSite: %s is related to %s, Site: %s is related to %s",
							x[i]->surface_comp->master->s->name, name1,
							x[j]->surface_comp->master->s->name, name2);

					error_msg(error_string, CONTINUE);
				}
			}
		}
	}
	/*
	 *   check related kinetics
	 */
	if (use.Get_surface_ptr()->related_rate == TRUE)
	{
		for (i = 0; i < count_unknowns; i++)
		{
			if (x[i]->type != SURFACE_CB)
				continue;
			for (j = 0; j < count_unknowns; j++)
			{
				if (x[j]->type != SURFACE)
					continue;
				if (x[j]->potential_unknown != x[i])
					continue;
				if (x[j]->surface_comp->rate_name !=
					x[i]->surface_comp->rate_name)
				{
					if (x[i]->surface_comp->rate_name == NULL)
					{
						name1 = string_hsave("None");
					}
					else
					{
						name1 = x[i]->surface_comp->rate_name;
					}
					if (x[j]->surface_comp->rate_name == NULL)
					{
						name2 = string_hsave("None");
					}
					else
					{
						name2 = x[j]->surface_comp->rate_name;
					}
					input_error++;
					error_string = sformatf(
							"All surface sites for a single component must be related to the same kinetic reaction.\n\tSite: %s is related to %s, Site: %s is related to %s",
							x[i]->surface_comp->master->s->name, name1,
							x[j]->surface_comp->master->s->name, name2);

					error_msg(error_string, CONTINUE);
				}
			}
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
struct unknown * Phreeqc::
find_surface_charge_unknown(char *str_ptr, int plane)
/* ---------------------------------------------------------------------- */
{
/*
 *    Makes name for the potential unknown and returns in str_ptr
 *    Returns NULL if this unknown not in unknown list else
 *    returns a pointer to the potential unknown
 */
	int i;
	char *ptr;
	char token[MAX_LENGTH];

	replace("_", " ", str_ptr);
	ptr = str_ptr;
	copy_token(token, &ptr, &i);
	if (plane == SURF_PSI)
	{
		strcat(token, "_CB");
	}
	else if (plane == SURF_PSI1)
	{
		strcat(token, "_CBb");
	}
	else if (plane == SURF_PSI2)
	{
		strcat(token, "_CBd");
	}
	strcpy(str_ptr, token);
	for (i = 0; i < count_unknowns; i++)
	{
		if (strcmp(token, x[i]->description) == 0)
		{
			return (x[i]);
		}
	}
	return (NULL);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
setup_master_rxn(struct master **master_ptr_list, struct reaction **pe_rxn)
/* ---------------------------------------------------------------------- */
{
/*
 *   Rewrites rxn_secondary for all redox states in list
 *   First, in = TRUE; others, in = REWRITE 
 */
	int j;
	struct master *master_ptr, *master_ptr0;
/*
 *   Set master_ptr->in, master_ptr->rxn
 */
	master_ptr0 = master_ptr_list[0];
	for (j = 0; (master_ptr = master_ptr_list[j]) != NULL; j++)
	{
/*
 *   Check that data not already given
 */
		if (master_ptr->s == s_h2o)
		{
			error_string = sformatf(
					"Can not enter concentration data for O(-2),\n\tdissolved oxygen is O(0),\n\tfor mass of water, use -water identifier.");
			error_msg(error_string, CONTINUE);
			input_error++;
			continue;
		}

		if (master_ptr->in != FALSE)
		{
			if (master_ptr->s != s_eminus && master_ptr->s != s_hplus)
			{
				error_string = sformatf(
						"Analytical data entered twice for %s.",
						master_ptr->s->name);
				error_msg(error_string, CONTINUE);
				input_error++;
				continue;
			}
		}
/*
 *   Set flags
 */
		if (j == 0)
		{
			master_ptr->in = TRUE;
			if (master_ptr->s->primary == NULL)
			{
				rxn_free(master_ptr->rxn_secondary);
				master_ptr->rxn_secondary = rxn_dup(master_ptr->s->rxn_s);
/* debug
                                trxn_print ();
 */
			}
		}
		else
		{
			master_ptr->in = REWRITE;
			if (master_ptr0->s->primary == NULL)
			{
				rewrite_master_to_secondary(master_ptr, master_ptr0);
				rxn_free(master_ptr->rxn_secondary);
				master_ptr->rxn_secondary = rxn_alloc(count_trxn + 1);
				trxn_copy(master_ptr->rxn_secondary);
/* debug
					trxn_print ();
 */
			}
		}
		master_ptr->pe_rxn = pe_rxn;
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
LDBLE Phreeqc::
calc_PR(std::vector<struct phase *> phase_ptrs, LDBLE P, LDBLE TK, LDBLE V_m)
/* ---------------------------------------------------------------------- */
/*  Calculate fugacity and fugacity coefficient for gas pressures if critical T and P
    are defined.
  1) Solve molar volume V_m or total pressure P from Peng-Robinson's EOS:
  P = R * T / (V_m - b) - a * aa / (V_m^2 + 2 * b * V_m - b^2)
     a = 0.457235 * (R * T_c)^2 / P_c
     b = 0.077796 * R * T_c / P_c
     aa = (1 + kk * (1 - T_r^0.5))^2
     kk = 0.37464 + 1.54226 * omega - 0.26992 * omega^2
     T_r = T / T_c
  multicomponent gas phase:
     use: b_sum = Sum(x_i * b), x_i is mole-fraction
          a_aa_sum = Sum_i( Sum_j(x_i * x_j * (a_i * aa_i * a_j * aa_j)^0.5) )
  2) Find the fugacity coefficient phi for gas i:
  log(phi_i) = B_ratio * (z - 1) - log(z - B) + A / (2.8284 * B) * (B_ratio - 2 / a_aa_sum * a_aa_sum2) *\
           log((z + 2.4142 * B) / (z - 0.4142 * B))
     B_ratio = b_i / b_sum
     A = a_aa_sum * P / R_TK^2
     B = b_sum * P / R_TK
     a_aa_sum2 = Sum_j(x_j * (a_aa_i * a_aa_j)^0.5
  3) correct the solubility of gas i with:
  pr_si_f = log10(phi_i) -  Delta_V_i * (P - 1) / (2.303 * R * TK);
*/
{
	int i, i1, n_g = phase_ptrs.size();
	LDBLE T_c, P_c;
	LDBLE A, B, B_r, /*b2,*/ kk, oo, a_aa, T_r;
	LDBLE m_sum, /*b_sum, a_aa_sum,*/ a_aa_sum2;
	LDBLE phi;
	LDBLE /*R_TK,*/ R = R_LITER_ATM; /* L atm / (K mol) */
	LDBLE r3[4], r3_12, rp, rp3, rq, rz, ri, ri1, one_3 = 0.33333333333333333;
	LDBLE disct, vinit, v1, ddp, dp_dv, dp_dv2;
	int it;
	struct phase *phase_ptr, *phase_ptr1;
	cxxGasPhase * gas_phase_ptr = (cxxGasPhase *) use.Get_gas_phase_ptr();
	bool halved;
	R_TK = R * TK;
	m_sum = b_sum = a_aa_sum = 0.0;
	for (i = 0; i < n_g; i++)
	{
		phase_ptr = phase_ptrs[i];
		if (n_g > 1)
		{
			if (phase_ptr->moles_x == 0)
				continue;
			m_sum += phase_ptr->moles_x;
		}
		if (phase_ptr->t_c == 0.0 || phase_ptr->p_c == 0.0)
			continue;
		if (!phase_ptr->pr_a)
		{
			T_c = phase_ptr->t_c;
			P_c = phase_ptr->p_c;
			phase_ptr->pr_a = 0.457235 * R * R * T_c * T_c / P_c;
			phase_ptr->pr_b = 0.077796 * R * T_c / P_c;
			T_r = TK / T_c;
			oo = phase_ptr->omega;
			kk = 0.37464 + oo * (1.54226 - 0.26992 * oo);
			phase_ptr->pr_alpha = pow(1 + kk * (1 - sqrt(T_r)), 2);
			phase_ptr->pr_tk = TK;
			phase_ptr->pr_in = true;
		}
		if (phase_ptr->pr_tk != TK)
		{
			T_r = TK / phase_ptr->t_c;
			oo = phase_ptr->omega;
			kk = 0.37464 + oo * (1.54226 - 0.26992 * oo);
			phase_ptr->pr_alpha = pow(1 + kk * (1 - sqrt(T_r)), 2);
			phase_ptr->pr_tk = TK;
			phase_ptr->pr_in = true;
		}
	}
	for (i = 0; i < n_g; i++)
	{
		phase_ptr = phase_ptrs[i];
		if (n_g == 1)
		{
			phase_ptr->fraction_x = 1.0;
			break;
		}
		if (m_sum == 0)
			return (OK);
		phase_ptr->fraction_x = phase_ptr->moles_x / m_sum;
	}
	 
	for (i = 0; i < n_g; i++)
	{
		a_aa_sum2 = 0.0;
		phase_ptr = phase_ptrs[i];
		if (phase_ptr->t_c == 0.0 || phase_ptr->p_c == 0.0)
			continue;
		b_sum += phase_ptr->fraction_x * phase_ptr->pr_b;
		for (i1 = 0; i1 < n_g; i1++)
		{
			phase_ptr1 = phase_ptrs[i1];
			if (phase_ptr1->t_c == 0.0 || phase_ptr1->p_c == 0.0)
				continue;
			if (phase_ptr1->fraction_x == 0)
				continue;
			a_aa = sqrt(phase_ptr->pr_a * phase_ptr->pr_alpha *
				        phase_ptr1->pr_a * phase_ptr1->pr_alpha);
			if (!strcmp(phase_ptr->name, "H2O(g)"))
			{
				if (!strcmp(phase_ptr1->name, "CO2(g)"))
					a_aa *= 0.81; // Soreide and Whitson, 1992, FPE 77, 217
				else if (!strcmp(phase_ptr1->name, "H2S(g)"))
					a_aa *= 0.81;
				else if (!strcmp(phase_ptr1->name, "CH4(g)"))
					a_aa *= 0.51;
				else if (!strcmp(phase_ptr1->name, "N2(g)"))
					a_aa *= 0.51;
			}
			if (!strcmp(phase_ptr1->name, "H2O(g)"))
			{
				if (!strcmp(phase_ptr->name, "CO2(g)"))
					a_aa *= 0.81;
				else if (!strcmp(phase_ptr->name, "H2S(g)"))
					a_aa *= 0.81;
				else if (!strcmp(phase_ptr->name, "CH4(g)"))
					a_aa *= 0.51;
				else if (!strcmp(phase_ptr->name, "N2(g)"))
					a_aa *= 0.51;
			}
			a_aa_sum += phase_ptr->fraction_x * phase_ptr1->fraction_x * a_aa;
			a_aa_sum2 += phase_ptr1->fraction_x * a_aa;
		}
		phase_ptr->pr_aa_sum2 = a_aa_sum2;
	}
	b2 = b_sum * b_sum;

	if (V_m)
	{
		P = R_TK / (V_m - b_sum) - a_aa_sum / (V_m * (V_m + 2 * b_sum) - b2);
		if (iterations > 0 && P < 150)
		{
			// check for 3-roots...
			r3[1] = b_sum - R_TK / P;
			r3[2] = -3.0 * b2 + (a_aa_sum - R_TK * 2.0 * b_sum) / P;
			r3[3] = b2 * b_sum + (R_TK * b2 - b_sum * a_aa_sum) / P;
			// the discriminant of the cubic eqn...
			disct = 18. * r3[1] * r3[2] * r3[3] -
				4. * pow(r3[1], 3) * r3[3] + 
				r3[1] * r3[1] * r3[2] * r3[2] -
				4. * pow(r3[2], 3) - 
				27. * r3[3] * r3[3];
			//if (iterations > 50)
			//	it = 0;	// debug
			if (disct > 0)
			{
				// 3-roots, find the largest P...
				it = 0;
				halved = false;
				ddp = 1e-9;
				v1 = vinit = 0.429;
				dp_dv = f_Vm(v1, this);
				while (fabs(dp_dv) > 1e-11 && it < 40)
				{
					it +=1;
					dp_dv2 = f_Vm(v1 - ddp, this);
					v1 -= (dp_dv * ddp / (dp_dv - dp_dv2));
					if (!halved && (v1 > vinit || v1 < 0.03))
					{
						vinit -= 0.05;
						if (vinit < 0.03)
						{
							vinit = halve(f_Vm, 0.03, 1.0, 1e-3);
							if (f_Vm(vinit - 2e-3, this) < 0)
								vinit = halve(f_Vm, vinit + 2e-3, 1.0, 1e-3);
							halved = true;
						}
						v1 = vinit;
					}
					dp_dv = f_Vm(v1, this);
					if (fabs(dp_dv) < 1e-11)
					{
						if (f_Vm(v1 - 1e-4, this) < 0)
						{
							v1 = halve(f_Vm, v1 + 1e-4, 1.0, 1e-3);
							dp_dv = f_Vm(v1, this);
						}
					}
				}
				if (it == 40)
				{
// accept a (possible) whobble in the curve...
//					error_msg("No convergence when calculating P in Peng-Robinson.", STOP);
				}
				if (V_m < v1)
					P = R_TK / (v1 - b_sum) - a_aa_sum / (v1 * (v1 + 2 * b_sum) - b2);
			}
		}
		if (P <= 0) // iterations = -1
			P = 1.;
	} else
	{
		r3[1] = b_sum - R_TK / P;
		r3_12 = r3[1] * r3[1];
		r3[2] = -3.0 * b2 + (a_aa_sum - R_TK * 2.0 * b_sum) / P;
		r3[3] = b2 * b_sum + (R_TK * b2 - b_sum * a_aa_sum) / P;
		// solve t^3 + rp*t + rq = 0.
		// molar volume V_m = t - r3[1] / 3... 
		rp = r3[2] - r3_12 / 3;
		rp3 = rp * rp * rp;
		rq = (2.0 * r3_12 * r3[1] - 9.0 * r3[1] * r3[2]) / 27 + r3[3];
		rz = rq * rq / 4 + rp3 / 27;
		if (rz >= 0) // Cardono's method...
		{
			ri = sqrt(rz);
			if (ri + rq / 2 <= 0)
			{
				V_m = pow(ri - rq / 2, one_3) + pow(- ri - rq / 2, one_3) - r3[1] / 3;
			}
			else
			{
				ri = - pow(ri + rq / 2, one_3);
				V_m = ri - rp / (3.0 * ri) - r3[1] / 3;
			}
		}
		else // use complex plane...
		{
			ri = sqrt(- rp3 / 27); // rp < 0
			ri1 = acos(- rq / 2 / ri);
			V_m = 2.0 * pow(ri, one_3) * cos(ri1 / 3) - r3[1] / 3;
		}
	}
 // calculate the fugacity coefficients...
	for (i = 0; i < n_g; i++)
	{
		phase_ptr = phase_ptrs[i];
		if (phase_ptr->fraction_x == 0.0)
		{
			phase_ptr->pr_p = 0;
			phase_ptr->pr_phi = 1;
			phase_ptr->pr_si_f = 0.0;
			continue;
		}
		phase_ptr->pr_p = phase_ptr->fraction_x * P;
		if (phase_ptr->t_c == 0.0 || phase_ptr->p_c == 0.0)
		{
			phase_ptr->pr_phi = 1;
			continue;
		}
		rz = P * V_m / R_TK;
		A = a_aa_sum * P / (R_TK * R_TK);
		B = b_sum * P / R_TK;
		B_r = phase_ptr->pr_b / b_sum;
		if (rz > B)
			phi = B_r * (rz - 1) - log(rz - B) + A / (2.828427 * B) * (B_r - 2.0 * phase_ptr->pr_aa_sum2 / a_aa_sum) *
				  log((rz + 2.41421356 * B) / (rz - 0.41421356 * B));
		else
			phi = -3.0; // fugacity coefficient > 0.05
		phase_ptr->pr_phi = exp(phi);
		phase_ptr->pr_si_f = phi / LOG_10;
		// for initial equilibrations, adapt log_k of the gas phase...
		if (state < REACTION)
		{
			phase_ptr->lk = calc_lk_phase(phase_ptr, TK, P);
		}
		phase_ptr->pr_in = true;
	}
	if (gas_phase_ptr && iterations > 2)
	{
		gas_phase_ptr->Set_total_p(P);
		gas_phase_ptr->Set_v_m(V_m);
		return (OK);
	}
	return (V_m);
}

LDBLE Phreeqc::
f_Vm(LDBLE v1, void *cookie)
/* ---------------------------------------------------------------------- */
{
	Phreeqc * pThis;

	pThis = (Phreeqc *) cookie;

	LDBLE dp_dv = -pThis->R_TK / ((v1 - pThis->b_sum) * (v1 - pThis->b_sum)) +
					pThis->a_aa_sum * (2 * v1 + 2 * pThis->b_sum) /
					((v1 * (v1 + 2. * pThis->b_sum) - pThis->b2) * (v1 * (v1 + 2. * pThis->b_sum) - pThis->b2));
	return dp_dv;
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
setup_pure_phases(void)
/* ---------------------------------------------------------------------- */
{
	LDBLE si_org;
/*
 *   Fills in data for pure_phase assemglage in unknown structure
 */

	if (use.Get_pp_assemblage_ptr() == NULL)
		return (OK);
	cxxPPassemblage * pp_assemblage_ptr = (cxxPPassemblage *) use.Get_pp_assemblage_ptr();
/*
 *   Setup unknowns
 */
	std::map<std::string, cxxPPassemblageComp>::iterator it;
	it =  pp_assemblage_ptr->Get_pp_assemblage_comps().begin();
	for ( ; it != pp_assemblage_ptr->Get_pp_assemblage_comps().end(); it++)
	{
		cxxPPassemblageComp * comp_ptr = &(it->second);
		int j;
		struct phase * phase_ptr = phase_bsearch(it->first.c_str(), &j, FALSE);
		assert(phase_ptr);
		x[count_unknowns]->type = PP;
		x[count_unknowns]->description = string_hsave(comp_ptr->Get_name().c_str());
		x[count_unknowns]->pp_assemblage_comp_name = x[count_unknowns]->description;
		x[count_unknowns]->moles = comp_ptr->Get_moles();
		x[count_unknowns]->phase = phase_ptr;
		x[count_unknowns]->si = comp_ptr->Get_si();
		si_org = comp_ptr->Get_si_org();
		/* si_org is used for Peng-Robinson gas, with the fugacity
		   coefficient added later in adjust_pure_phases,
		   when rxn_x has been defined for each phase in the model */
		x[count_unknowns]->delta = comp_ptr->Get_delta();	
		x[count_unknowns]->dissolve_only = comp_ptr->Get_dissolve_only();
		if (pure_phase_unknown == NULL)
			pure_phase_unknown = x[count_unknowns];
		count_unknowns++;
	}
	return (OK);
}
#ifdef SKIP
/* ---------------------------------------------------------------------- */
int Phreeqc::
setup_pure_phases(void)
/* ---------------------------------------------------------------------- */
{
	int i;
	struct phase *phase_ptr;
	LDBLE si_org;
/*
 *   Fills in data for pure_phase assemglage in unknown structure
 */

	if (use.Get_pp_assemblage_ptr() == NULL)
		return (OK);
/*
 *   Setup unknowns
 */
	for (i = 0; i < use.Get_pp_assemblage_ptr()->count_comps; i++)
	{
		x[count_unknowns]->type = PP;
		x[count_unknowns]->description =
			use.Get_pp_assemblage_ptr()->pure_phases[i].name;
		x[count_unknowns]->moles =
			use.Get_pp_assemblage_ptr()->pure_phases[i].moles;
		x[count_unknowns]->phase = phase_ptr =
			use.Get_pp_assemblage_ptr()->pure_phases[i].phase;
		x[count_unknowns]->si = use.Get_pp_assemblage_ptr()->pure_phases[i].si;
		si_org = use.Get_pp_assemblage_ptr()->pure_phases[i].si_org;
		/* si_org is used for Peng-Robinson gas, with the fugacity
		   coefficient added later in adjust_pure_phases,
		   when rxn_x has been defined for each phase in the model */
		x[count_unknowns]->delta =
			use.Get_pp_assemblage_ptr()->pure_phases[i].delta;
		x[count_unknowns]->pure_phase =
			&(use.Get_pp_assemblage_ptr()->pure_phases[i]);
		x[count_unknowns]->dissolve_only =
			use.Get_pp_assemblage_ptr()->pure_phases[i].dissolve_only;
		if (pure_phase_unknown == NULL)
			pure_phase_unknown = x[count_unknowns];
		count_unknowns++;
	}
	return (OK);
}
#endif
/* ---------------------------------------------------------------------- */
int Phreeqc::
adjust_setup_pure_phases(void)
/* ---------------------------------------------------------------------- */
{
	int i;
	struct phase *phase_ptr;
	LDBLE si_org, p, t;
/*
 *   Fills in data for pure_phase assemglage in unknown structure
 */

	if (use.Get_pp_assemblage_ptr() == NULL)
		return (OK);
	cxxPPassemblage * pp_assemblage_ptr = (cxxPPassemblage *) use.Get_pp_assemblage_ptr();
/*
 *   Adjust si for gases
 */
	for (i = 0; i < count_unknowns; i++)
	{
		std::vector<struct phase *> phase_ptrs;
		if (x[i]->type == PP)
		{
			phase_ptr = x[i]->phase;
			phase_ptrs.push_back(phase_ptr);
			cxxPPassemblageComp * comp_ptr = pp_assemblage_ptr->Find(x[i]->pp_assemblage_comp_name);
			si_org = comp_ptr->Get_si_org();
			if (phase_ptr->p_c > 0 && phase_ptr->t_c > 0)
			{
				p = exp(si_org * LOG_10);
				t = use.Get_solution_ptr()->tc + 273.15;
				if (!phase_ptr->pr_in || p != phase_ptr->pr_p || t != phase_ptr->pr_tk)
				{
					calc_PR(phase_ptrs, p, t, 0);
				}
				x[i]->si = si_org + phase_ptr->pr_si_f;
			}
		}
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
setup_solution(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Fills in data in unknown structure for the solution
 */
	int l, i, j;
	struct master *master_ptr;
	struct solution *solution_ptr;
	char *ptr;
	char token[MAX_LENGTH];
	struct master_isotope *master_isotope_ptr;
	struct phase *phase_ptr;

	solution_ptr = use.Get_solution_ptr();
	count_unknowns = 0;
	for (i = 0; solution_ptr->totals[i].description != NULL; i++)
	{
		/*              solution_ptr->totals[i].skip = FALSE; */
		char * temp_desc = string_duplicate(solution_ptr->totals[i].description);
		ptr = temp_desc;
		copy_token(token, &ptr, &l);
		master_ptr = master_bsearch(token);
		/*
		 * Treat minor isotopes as special in initial solution calculation
		 */
		if ((state == INITIAL_SOLUTION) && (master_ptr != NULL)
			&& (master_ptr->minor_isotope == TRUE)
			&& (initial_solution_isotopes == FALSE))
		{
			master_isotope_ptr = master_isotope_search(token);
			if (master_isotope_ptr != NULL)
			{
				master_isotope_ptr->ratio =
					solution_ptr->totals[i].input_conc;
			}
			/*                      solution_ptr->totals[i].skip = TRUE; */
			free_check_null(temp_desc);
			continue;
		}
/*
 *   Check that total not <= zero
 */
		if (solution_ptr->totals[i].input_conc <= 0.0)
		{
			if (strcmp(token, "H(1)") != 0 && strcmp(token, "E") != 0)
			{
				/*                              solution_ptr->totals[i].skip = TRUE; */
				free_check_null(temp_desc);
				continue;
			}
		}
/*
 *   Find master species
 */
		master_ptr = master_bsearch(token);
		if (master_ptr == NULL)
		{
			/*                      solution_ptr->totals[i].skip = TRUE; */
			error_string = sformatf(
					"Master species not in data base for %s, skipping element.",
					solution_ptr->totals[i].description);
			warning_msg(error_string);
			free_check_null(temp_desc);
			continue;
		}
		if (master_ptr->type != AQ)
		{
			/*                      solution_ptr->totals[i].skip = TRUE; */
			error_string = sformatf(
					"Only aqueous concentrations are allowed in solution data, ignoring %s.",
					solution_ptr->totals[i].description);
			warning_msg(error_string);
			free_check_null(temp_desc);
			continue;
		}
/*
 *   Store list of master species pointers, set master[i].in and master[i].rxn for list
 */
		x[count_unknowns]->master = get_list_master_ptrs(ptr, master_ptr);
		setup_master_rxn(x[count_unknowns]->master,
						 &(pe_x[solution_ptr->totals[i].n_pe].rxn));
		
/*
 *   Set default unknown data
 */
		x[count_unknowns]->type = MB;
		x[count_unknowns]->description = solution_ptr->totals[i].description;
		x[count_unknowns]->total = &(solution_ptr->totals[i]);
		for (j = 0; x[count_unknowns]->master[j] != NULL; j++)
		{
			x[count_unknowns]->master[j]->unknown = x[count_unknowns];
		}
		x[count_unknowns]->moles = solution_ptr->totals[i].moles;
/*
 *   Set pointers
 */
		free_check_null(temp_desc);
		temp_desc = string_duplicate(solution_ptr->totals[i].description);
		ptr = temp_desc;
		copy_token(token, &ptr, &l);
		str_tolower(token);
		if (strstr(token, "alk") != NULL)
		{
			if (alkalinity_unknown == NULL)
			{
				x[count_unknowns]->type = ALK;
				alkalinity_unknown = x[count_unknowns];
			}
			else
			{
				error_msg("Alkalinity entered more than once.", CONTINUE);
				input_error++;
			}
		}
		else if (strcmp(token, "c") == 0 || strcmp(token, "c(4)") == 0)
		{
			if (carbon_unknown == NULL)
			{
				carbon_unknown = x[count_unknowns];
			}
			else
			{
				error_msg("Carbon entered more than once.", CONTINUE);
				input_error++;
			}
		}
		else if (strcmp(token, "h(1)") == 0)
		{
			if (ph_unknown == NULL)
			{
				ph_unknown = x[count_unknowns];
			}
			else
			{
				error_msg("pH entered more than once.", CONTINUE);
				input_error++;
			}
		}
		else if (strcmp(token, "e") == 0)
		{
			if (pe_unknown == NULL)
			{
				pe_unknown = x[count_unknowns];
			}
			else
			{
				error_msg("pe entered more than once.", CONTINUE);
				input_error++;
			}
		}
		free_check_null(temp_desc);
/*
 *   Charge balance unknown
 */
		if (solution_ptr->totals[i].equation_name != NULL)
		{
			char * temp_eq_name = string_duplicate(solution_ptr->totals[i].equation_name);
			ptr = temp_eq_name;
			copy_token(token, &ptr, &l);
			str_tolower(token);
			if (strstr(token, "charge") != NULL)
			{
				if (charge_balance_unknown == NULL)
				{
					charge_balance_unknown = x[count_unknowns];
					x[count_unknowns]->type = CB;
					if (charge_balance_unknown == ph_unknown)
					{
						x[count_unknowns]->moles = solution_ptr->cb;
					}
				}
				else
				{
					error_msg("Charge balance specified for more"
							  " than one species.", CONTINUE);
					input_error++;
				}
			}
			else
			{
/*
 *   Solution phase boundaries
 */
				solution_ptr->totals[i].phase =
					phase_bsearch(solution_ptr->totals[i].equation_name, &l,
								  FALSE);
				if (solution_ptr->totals[i].phase == NULL)
				{
					error_string = sformatf( "Expected a mineral name, %s.",
							solution_ptr->totals[i].equation_name);
					error_msg(error_string, CONTINUE);
					input_error++;
				}
				x[count_unknowns]->type = SOLUTION_PHASE_BOUNDARY;
				phase_ptr = solution_ptr->totals[i].phase;
				x[count_unknowns]->phase = phase_ptr;
				x[count_unknowns]->si = solution_ptr->totals[i].phase_si;
				/* For Peng-Robinson gas, the fugacity
				   coefficient is added later in adjust_setup_solution,
				   when rxn_x has been defined for each phase in the model */
				if (solution_phase_boundary_unknown == NULL)
				{
					solution_phase_boundary_unknown = x[count_unknowns];
				}
			}
			free_check_null(temp_eq_name);
		}
		count_unknowns++;
	}
/*
 *   Set mb_unknown
 */
	if (count_unknowns > 0)
		mb_unknown = x[0];
/*
 *   Special for alkalinity
 */
	if (alkalinity_unknown != NULL)
	{
		if (carbon_unknown != NULL)
		{
/*
 *   pH adjusted to obtain given alkalinity
 */
			if (ph_unknown == NULL)
			{
				output_msg(sformatf("\npH will be adjusted to obtain desired alkalinity.\n\n"));
				ph_unknown = alkalinity_unknown;
				master_ptr = master_bsearch("H(1)");
				alkalinity_unknown->master[0] = master_ptr;
				master_ptr->in = TRUE;
				master_ptr->unknown = ph_unknown;
				ph_unknown->master[0] = master_ptr;
				ph_unknown->description = string_hsave("H(1)");
			}
			else
			{
				error_msg("pH adjustment is needed for alkalinity but"
						  " charge balance or a phase boundary was also specified.",
						  CONTINUE);
				input_error++;
			}
/*
 *   Carbonate ion adjusted to obtain given alkalintiy
 */
		}
		else
		{
			if (alkalinity_unknown->master[0]->s->secondary != NULL)
			{
				alkalinity_unknown->master[0]->s->secondary->in = TRUE;
				alkalinity_unknown->master[0]->s->secondary->unknown =
					alkalinity_unknown;
			}
			else
			{
				error_msg
					("Error in definition of Alkalinity in SOLUTION_MASTER_SPECIES and SOLUTION_SPECIES.\n\tAlkalinity master species should be same as master species for C(4).",
					 CONTINUE);
				input_error++;
			}
		}
	}
	if (pitzer_model == FALSE && sit_model == FALSE)
	{
		/*
		 *   Ionic strength
		 */
		mu_unknown = x[count_unknowns];
		x[count_unknowns]->description = string_hsave("Mu");
		x[count_unknowns]->type = MU;
		x[count_unknowns]->number = count_unknowns;
		x[count_unknowns]->moles = 0.0;
		mu_unknown = x[count_unknowns];
		count_unknowns++;
	}
	/*
	 *   Activity of water
	 */
	ah2o_unknown = x[count_unknowns];
	ah2o_unknown->description = string_hsave("A(H2O)");
	ah2o_unknown->type = AH2O;
	ah2o_unknown->number = count_unknowns;
	ah2o_unknown->master = unknown_alloc_master();
	ah2o_unknown->master[0] = master_bsearch("O");
	ah2o_unknown->master[0]->unknown = ah2o_unknown;
	ah2o_unknown->moles = 0.0;
	count_unknowns++;

	if (state >= REACTION)
	{
/*

 *   Reaction: pH for charge balance
 */
		ph_unknown = x[count_unknowns];
		ph_unknown->description = string_hsave("pH");
		ph_unknown->type = CB;
		ph_unknown->moles = solution_ptr->cb;
		ph_unknown->number = count_unknowns;
		ph_unknown->master = unknown_alloc_master();
		ph_unknown->master[0] = s_hplus->primary;
		ph_unknown->master[0]->unknown = ph_unknown;
		charge_balance_unknown = ph_unknown;
		count_unknowns++;
/*
 *   Reaction: pe for total hydrogen
 */
		pe_unknown = x[count_unknowns];
		mass_hydrogen_unknown = x[count_unknowns];
		mass_hydrogen_unknown->description = string_hsave("Hydrogen");
		mass_hydrogen_unknown->type = MH;
#ifdef COMBINE
		mass_hydrogen_unknown->moles =
			solution_ptr->total_h - 2 * solution_ptr->total_o;
#else
		mass_hydrogen_unknown->moles = solution_ptr->total_h;
#endif
		mass_hydrogen_unknown->number = count_unknowns;
		mass_hydrogen_unknown->master = unknown_alloc_master();
		mass_hydrogen_unknown->master[0] = s_eminus->primary;
		mass_hydrogen_unknown->master[0]->unknown = mass_hydrogen_unknown;
		count_unknowns++;
/*
 *   Reaction H2O for total oxygen
 */
		mass_oxygen_unknown = x[count_unknowns];
		mass_oxygen_unknown->description = string_hsave("Oxygen");
		mass_oxygen_unknown->type = MH2O;
		mass_oxygen_unknown->moles = solution_ptr->total_o;
		mass_oxygen_unknown->number = count_unknowns;
		mass_oxygen_unknown->master = unknown_alloc_master();
		mass_oxygen_unknown->master[0] = s_h2o->primary;
		count_unknowns++;
	}
/*
 *   Validity tests
 */
	if ((ph_unknown != NULL) &&
		(ph_unknown == charge_balance_unknown)
		&& (alkalinity_unknown != NULL))
	{
		error_msg("pH adustment can not attain charge balance"
				  " when alkalinity is fixed.", CONTINUE);
		input_error++;
	}
	if ((alkalinity_unknown != NULL) &&
		(alkalinity_unknown->type == CB ||
		 alkalinity_unknown->type == SOLUTION_PHASE_BOUNDARY))
	{
		error_msg("Alkalinity can not be used with charge balance"
				  " or solution phase boundary constraints.", CONTINUE);
		input_error++;
	}

	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
adjust_setup_solution(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Fills in data in unknown structure for the solution
 */
	int i;
	struct phase *phase_ptr;
	LDBLE p, t;

	for (i = 0; i < count_unknowns; i++)
	{
		std::vector<struct phase *> phase_ptrs;
		if (x[i]->type == SOLUTION_PHASE_BOUNDARY)
		{
			//solution_ptr->totals[i].phase =
			//	phase_bsearch(solution_ptr->totals[i].equation_name, &l,
			//				  FALSE);
			//if (solution_ptr->totals[i].phase == NULL)
			//{
			//	error_string = sformatf( "Expected a mineral name, %s.",
			//			solution_ptr->totals[i].equation_name);
			//	error_msg(error_string, CONTINUE);
			//	input_error++;
			//}
			x[count_unknowns]->type = SOLUTION_PHASE_BOUNDARY;
			//phase_ptr = solution_ptr->totals[i].phase;
			//x[count_unknowns]->phase = phase_ptr;
			//x[count_unknowns]->si = solution_ptr->totals[i].phase_si;

			phase_ptr = x[i]->phase;
			phase_ptrs.push_back(phase_ptr);
			if (phase_ptr->p_c > 0 && phase_ptr->t_c > 0)
			{
				p = exp(x[i]->si * LOG_10);
				t = use.Get_solution_ptr()->tc + 273.15;
				if (!phase_ptr->pr_in || p != phase_ptr->pr_p || t != phase_ptr->pr_tk)
				{
					calc_PR(phase_ptrs, p, t, 0);
				}
				x[i]->si += phase_ptr->pr_si_f;
			}
		}
	}
	return (OK);

}
/* ---------------------------------------------------------------------- */
struct master ** Phreeqc::
unknown_alloc_master(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Allocates space for a list of 2 master pointers
 */
	struct master **master_ptr;

	master_ptr = (struct master **) PHRQ_malloc(2 * sizeof(struct master *));
	if (master_ptr == NULL)
		malloc_error();
	master_ptr[0] = NULL;
	master_ptr[1] = NULL;
	return (master_ptr);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
setup_unknowns(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Counts unknowns and allocates space for unknown structures
 */
	int i;
	struct solution *solution_ptr;

	solution_ptr = use.Get_solution_ptr();
/*
 *   Calculate maximum number of unknowns
 */
	max_unknowns = 0;
/*
 *   Count mass balance in solution
 */
	for (i = 0; solution_ptr->totals[i].description != NULL; i++)
		max_unknowns++;
/*
 *   Add 5 for ionic strength, activity of water, charge balance, total H, total O
 */
	max_unknowns += 5;
/*
 *   Count pure phases
 */
	if (use.Get_pp_assemblage_ptr() != NULL)
	{
		cxxPPassemblage * pp_assemblage_ptr = (cxxPPassemblage *) use.Get_pp_assemblage_ptr();
		max_unknowns += (int) pp_assemblage_ptr->Get_pp_assemblage_comps().size();
		//max_unknowns += use.Get_pp_assemblage_ptr()->count_comps;

/*
		for (i = 0; use.Get_pp_assemblage_ptr()->pure_phases[i].name != NULL; i++ ) {
			max_unknowns++;
		}
 */
	}
/*
 *   Count exchange
 */
	if (use.Get_exchange_ptr() != NULL)
	{
		cxxExchange *exchange_ptr = (cxxExchange *) use.Get_exchange_ptr();
		std::vector<cxxExchComp *> comps = exchange_ptr->Vectorize();
		for (size_t j = 0; j < comps.size(); j++)
		{
			cxxExchComp * comp_ptr = comps[j];
			cxxNameDouble nd(comp_ptr->Get_totals());
			cxxNameDouble::iterator it = nd.begin();
			for ( ; it != nd.end(); it++)
			{
				element * elt_ptr = element_store(it->first.c_str());
				if (elt_ptr == NULL || elt_ptr->master == NULL)
				{
					error_string = sformatf(
							"Master species missing for element %s",
							it->first.c_str());
					error_msg(error_string, STOP);
				}
				if (elt_ptr->master->type == EX)
				{
					max_unknowns++;
				}
			}
		}
	}
/*
 *   Count surfaces
 */
	if (use.Get_surface_ptr() != NULL)
	{
		if (use.Get_surface_ptr()->type != CD_MUSIC)
		{
			max_unknowns +=
				use.Get_surface_ptr()->count_comps + use.Get_surface_ptr()->count_charge;
		}
		else
		{
			max_unknowns +=
				use.Get_surface_ptr()->count_comps +
				4 * use.Get_surface_ptr()->count_charge;
		}
	}
/*
 *   Count gas components
 */
	if (use.Get_gas_phase_ptr() != NULL)
	{
		cxxGasPhase * gas_phase_ptr = (cxxGasPhase *) use.Get_gas_phase_ptr();
		if (gas_phase_ptr->Get_type() == cxxGasPhase::GP_VOLUME && 
			(gas_phase_ptr->Get_pr_in() || force_numerical_fixed_volume) && numerical_fixed_volume)
		{
			max_unknowns += gas_phase_ptr->Get_gas_comps().size();
		}
		else
		{
			max_unknowns++;
		}
	}
/*
 *   Count solid solutions
 */
	if (use.Get_ss_assemblage_ptr() != NULL)
	{
		/*              max_unknowns += 2 * use.Get_ss_assemblage_ptr()->count_s_s; */
		for (i = 0; i < use.Get_ss_assemblage_ptr()->count_s_s; i++)
		{
			max_unknowns += use.Get_ss_assemblage_ptr()->s_s[i].count_comps;
		}
	}

/*
 *   Pitzer/Sit
 */
	max_unknowns++;
	if (pitzer_model == TRUE || sit_model == TRUE)
	{
		max_unknowns += count_s;
	}
/*
 *   One for slack
 */
	max_unknowns++;

/*
 *   Allocate space for pointer array and structures
 */

	space((void **) ((void *) &x), INIT, &max_unknowns,
		  sizeof(struct unknown *));
	for (i = 0; i < max_unknowns; i++)
	{
		x[i] = (struct unknown *) unknown_alloc();
		x[i]->number = i;
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
store_dn(int k, LDBLE * source, int row, LDBLE coef_in, LDBLE * gamma_source)
/* ---------------------------------------------------------------------- */
{
/*
 *   Stores the terms for d moles of species k in solution into row, multiplied
 *   by coef_in
 */
	int col;
	LDBLE coef;
	struct rxn_token *rxn_ptr;
	struct master *master_ptr;

	if (equal(coef_in, 0.0, TOL) == TRUE)
	{
		return (OK);
	}
/*   Gamma term for d molality of species */
/*   Note dg includes molality as a factor */

	row = row * (count_unknowns + 1);
	if (s[k]->type != SURF && s[k] != s_h2o)
	{
		if (debug_prep == TRUE)
		{
			output_msg(sformatf( "\t\t%-24s%10.3f\t%d\t%d\n",
					   "Activity coefficient", (double) (-1.0 * coef_in),
					   row / (count_unknowns + 1), mu_unknown->number));
		}
		/* mu term */
		if (gamma_source != NULL)
		{
			store_jacob(gamma_source, &array[row + mu_unknown->number],
						-1.0 * coef_in);
		}
	}
/*
 *   Mass of water factor
 */
	if (mass_oxygen_unknown != NULL && s[k]->type != EX && s[k]->type != SURF)
	{
		if (debug_prep == TRUE)
		{
			output_msg(sformatf( "\t\t%-24s%10.3f\t%d\t%d\n",
					   mass_oxygen_unknown->master[0]->s->name,
					   (double) coef_in, row / (count_unknowns + 1),
					   mass_oxygen_unknown->number));
		}
		store_jacob(source, &(array[row + mass_oxygen_unknown->number]),
					coef_in);
	}
	if (s[k] == s_h2o)
		return (OK);
	for (rxn_ptr = s[k]->rxn_x->token + 1; rxn_ptr->s != NULL; rxn_ptr++)
	{
		if (rxn_ptr->s->secondary != NULL
			&& rxn_ptr->s->secondary->in == TRUE)
		{
			master_ptr = rxn_ptr->s->secondary;
		}
		else
		{
			master_ptr = rxn_ptr->s->primary;
		}
		if (debug_prep == TRUE)
		{
			output_msg(sformatf( "\t\t%s\n", master_ptr->s->name));
		}
		if (master_ptr->unknown == NULL)
			continue;
		col = master_ptr->unknown->number;
		coef = coef_in * rxn_ptr->coef;
		store_jacob(source, &(array[row + col]), coef);
		if (debug_prep == TRUE)
		{
			output_msg(sformatf( "\t\t%-24s%10.3f\t%d\t%d\n",
					   master_ptr->s->name, (double) coef,
					   row / (count_unknowns + 1), col));
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
store_jacob(LDBLE * source, LDBLE * target, LDBLE coef)
/* ---------------------------------------------------------------------- */
{
/*
 *   Adds a new item to either sum_jacob1 or sum_jacob2
 *   If coef is 1.0, adds to sum_jacob1, which does not require a multiply
 *   Otherwise, adds to sum_jacob2, which allows multiply by coef
 */
	if (equal(coef, 1.0, TOL) == TRUE)
	{
		if (debug_prep == TRUE)
		{
			output_msg(sformatf( "\t\tjacob1 %d\n", count_sum_jacob1));
		}
		sum_jacob1[count_sum_jacob1].source = source;
		sum_jacob1[count_sum_jacob1++].target = target;
		/*    Check space */
		if (count_sum_jacob1 >= max_sum_jacob1)
		{
			space((void **) ((void *) &sum_jacob1), count_sum_jacob1,
				  &max_sum_jacob1, sizeof(struct list1));
		}
	}
	else
	{
		if (debug_prep == TRUE)
		{
			output_msg(sformatf( "\t\tjacob2 %d\n", count_sum_jacob2));
		}
		sum_jacob2[count_sum_jacob2].source = source;
		sum_jacob2[count_sum_jacob2].target = target;
		sum_jacob2[count_sum_jacob2++].coef = coef;
		/*    Check space */
		if (count_sum_jacob2 >= max_sum_jacob2)
		{
			space((void **) ((void *) &sum_jacob2), count_sum_jacob2,
				  &max_sum_jacob2, sizeof(struct list2));
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
store_jacob0(int row, int column, LDBLE coef)
/* ---------------------------------------------------------------------- */
{
/*
 *   Stores in list a constant coef which will be added into jacobian array
 */
	sum_jacob0[count_sum_jacob0].target =
		&(array[row * (count_unknowns + 1) + column]);
	sum_jacob0[count_sum_jacob0++].coef = coef;
	/*    Check space */
	if (count_sum_jacob0 >= max_sum_jacob0)
	{
		space((void **) ((void *) &sum_jacob0), count_sum_jacob0,
			  &max_sum_jacob0, sizeof(struct list0));
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
store_mb(LDBLE * source, LDBLE * target, LDBLE coef)
/* ---------------------------------------------------------------------- */
{
/*
 *   Adds item to list sum_mb1 or sum_mb2
 *   If coef is 1.0, adds to sum_mb1, which does not require a multiply
 *   else, adds to sum_mb2, which will multiply by coef
 */
	if (equal(coef, 1.0, TOL) == TRUE)
	{
		sum_mb1[count_sum_mb1].source = source;
		sum_mb1[count_sum_mb1++].target = target;
		if (count_sum_mb1 >= max_sum_mb1)
		{
			space((void **) ((void *) &sum_mb1),
				  count_sum_mb1 + count_trxn + 4, &max_sum_mb1,
				  sizeof(struct list1));
		}
	}
	else
	{
		sum_mb2[count_sum_mb2].source = source;
		sum_mb2[count_sum_mb2].coef = coef;
		sum_mb2[count_sum_mb2++].target = target;
		if (count_sum_mb2 >= max_sum_mb2)
		{
			space((void **) ((void *) &sum_mb2), count_sum_mb2,
				  &max_sum_mb2, sizeof(struct list2));
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
store_sum_deltas(LDBLE * source, LDBLE * target, LDBLE coef)
/* ---------------------------------------------------------------------- */
{
/*
 *   List sum_delta is summed to determine the change in the mass of 
 *   each element due to mass transfers of minerals, changes show up
 *   in x[i]->delta. These may be multiplied by a factor under some
 *   situations where the entire calculated step is not taken
 */
	sum_delta[count_sum_delta].source = source;
	sum_delta[count_sum_delta].target = target;
	sum_delta[count_sum_delta++].coef = coef;
	/*    Check space */
	if (count_sum_delta >= max_sum_delta)
	{
		space((void **) ((void *) &sum_delta), count_sum_delta,
			  &max_sum_delta, sizeof(struct list2));
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
switch_bases(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Check if activity of first master species is predominant among activities of
 *   secondary master species included in mass balance.
 */
	int i, j;
	int first;
	int return_value;
	LDBLE la, la1;
	struct master *master_ptr;

	return_value = FALSE;
	for (i = 0; i < count_unknowns; i++)
	{
		if (x[i]->type != MB)
			continue;
		first = 0;
		la = x[i]->master[0]->s->la;
		for (j = 1; x[i]->master[j] != NULL; j++)
		{
			la1 = x[i]->master[j]->s->lm + x[i]->master[j]->s->lg;
			if (first == 0 && la1 > la + 10.)
			{
				la = la1;
				first = j;
			}
			else if (first != 0 && la1 > la)
			{
				la = la1;
				first = j;
			}
		}
		if (first != 0)
		{
			master_ptr = x[i]->master[0];
			x[i]->master[0] = x[i]->master[first];
			x[i]->master[0]->in = TRUE;
			x[i]->master[first] = master_ptr;
			x[i]->master[first]->in = REWRITE;
/*
			fprintf(stderr, "Switching bases to %s.\tIteration %d\n",
					   x[i]->master[0]->s->name, iterations, la, x[i]->master[0]->s->la);
 */
			x[i]->master[0]->s->la = la;

			log_msg(sformatf( "Switching bases to %s.\tIteration %d\n",
					   x[i]->master[0]->s->name, iterations));
			return_value = TRUE;
		}
	}
	return (return_value);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
tidy_redox(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Write pe redox reactions (rxn in struct pe_data) in terms of master species
 *   defined in analytical data
 *   
 */
	int i, j, l;
	char *ptr;
	char token[MAX_LENGTH], tok1[MAX_LENGTH], tok2[MAX_LENGTH];
	struct pe_data *pe_data_ptr;
	struct master *master_ptr1, *master_ptr2;
/*
 *   Keep valences of oxygen and hydrogen in model, if not already in
 */
	for (i = 0; i < count_master; i++)
	{
		if (master[i]->primary == TRUE &&
			(master[i]->s == s_hplus || master[i]->s == s_h2o))
		{
			j = i + 1;
			while (j < count_master && master[j]->elt->primary == master[i])
			{
				if (master[j]->in == FALSE && master[j]->s != master[i]->s)
				{
					master[j]->in = REWRITE;
					master[j]->pe_rxn = master[i]->pe_rxn;
				}
				j++;
			}
		}
	}
/*
 *   Writes equations for e- for each redox couple used in solution n
 */
	for (pe_data_ptr = pe_x; pe_data_ptr->name != NULL; pe_data_ptr++)
	{
		if (strcmp_nocase_arg1(pe_data_ptr->name, "pe") == 0)
		{
			rxn_free(pe_data_ptr->rxn);
			pe_data_ptr->rxn = rxn_dup(s_eminus->rxn);
		}
		else
		{
			strcpy(token, pe_data_ptr->name);
			replace("/", " ", token);
			ptr = token;
/*
 *   Get redox states and elements from redox couple
 */
			copy_token(tok1, &ptr, &l);
			copy_token(tok2, &ptr, &l);
/*
 *   Find master species
 */
			master_ptr1 = master_bsearch(tok1);
			master_ptr2 = master_bsearch(tok2);
			if (master_ptr1 != NULL && master_ptr2 != NULL)
			{
				rewrite_master_to_secondary(master_ptr1, master_ptr2);
/*
 *   Rewrite equation to e-
 */
				trxn_swap("e-");
			}
			else
			{
				error_string = sformatf(
						"Can not find master species for redox couple, %s.",
						pe_data_ptr->name);
				error_msg(error_string, STOP);
			}
			if (inout() == FALSE)
			{
				error_string = sformatf(
						"Analytical data missing for redox couple, %s\n\t Using pe instead.",
						pe_data_ptr->name);
				warning_msg(error_string);
				rxn_free(pe_data_ptr->rxn);
				pe_data_ptr->rxn = rxn_dup(s_eminus->rxn);
				pe_data_ptr->name = pe_x[0].name;
			}
			else
			{
				rxn_free(pe_data_ptr->rxn);
				pe_data_ptr->rxn = rxn_alloc(count_trxn + 1);
				trxn_copy(pe_data_ptr->rxn);
			}
		}
	}
/*
 *   Rewrite equations to master species that are "in" the model
 */
	for (pe_data_ptr = pe_x; pe_data_ptr->name != NULL; pe_data_ptr++)
	{
		count_trxn = 0;
		trxn_add(pe_data_ptr->rxn, 1.0, FALSE);
		if (write_mass_action_eqn_x(CONTINUE) == FALSE)
		{
			error_string = sformatf( "Could not rewrite redox "
					"couple equation for %s\n\t Possibly missing data for one "
					"of the redox states.", pe_data_ptr->name);
			warning_msg(error_string);
			error_string = sformatf( "Using pe instead of %s.",
					pe_data_ptr->name);
			warning_msg(error_string);
			rxn_free(pe_data_ptr->rxn);
			pe_data_ptr->rxn = rxn_dup(pe_x[0].rxn);
			pe_data_ptr->name = pe_x[0].name;
		}
		else
		{
			rxn_free(pe_data_ptr->rxn);
			pe_data_ptr->rxn = rxn_alloc(count_trxn + 1);
			trxn_copy(pe_data_ptr->rxn);
		}
	}

	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
write_mb_eqn_x(void)
/* ---------------------------------------------------------------------- */
{
	int count, repeat;
	int i, count_rxn_orig;
	int j, k;
	struct master *master_ptr;
/*
 *   Rewrite any secondary master species flagged REWRITE
 *   Don't add in any pe reactions
 */
	count = 0;
	repeat = TRUE;
	while (repeat == TRUE)
	{
		count++;
		if (count > MAX_ADD_EQUATIONS)
		{
			error_string = sformatf( "Could not reduce equation "
					"to primary and secondary species that are "
					"in the model, %s.", trxn.token[0].s->name);
			error_msg(error_string, CONTINUE);
			return (ERROR);
		}
		repeat = FALSE;
		count_rxn_orig = count_trxn;
		for (i = 1; i < count_rxn_orig; i++)
		{
			if (trxn.token[i].s->secondary == NULL)
				continue;
			if (trxn.token[i].s->secondary->in == REWRITE)
			{
				repeat = TRUE;
				trxn_add(trxn.token[i].s->secondary->rxn_secondary,
						 trxn.token[i].coef, FALSE);
			}
		}
		trxn_combine();
	}
/*
 *  
 */
	count_elts = 0;
	paren_count = 0;
	for (i = 1; i < count_trxn; i++)
	{
		j = count_elts;
		char * temp_name = string_duplicate(trxn.token[i].s->name);
		char * ptr = temp_name;
		get_elts_in_species(&ptr, trxn.token[i].coef);
		free_check_null(temp_name);
		for (k = j; k < count_elts; k++)
		{
			if (trxn.token[i].s->secondary != NULL)
			{
				master_ptr = trxn.token[i].s->secondary->elt->primary;
			}
			else
			{
				master_ptr = trxn.token[i].s->primary;
			}
			if (elt_list[k].elt == master_ptr->elt)
			{
				elt_list[k].coef = 0.0;
				break;
			}
		}
		if (trxn.token[i].s->secondary == NULL)
		{
			char * temp_name = string_duplicate(trxn.token[i].s->primary->elt->name);
			char *ptr = temp_name;
			get_secondary_in_species(&ptr, trxn.token[i].coef);
			free_check_null(temp_name);
		}
		else
		{
			char * temp_name = string_duplicate(trxn.token[i].s->secondary->elt->name);
			ptr = temp_name;
			get_secondary_in_species(&ptr, trxn.token[i].coef);
			free_check_null(temp_name);
		}
	}
	if (count_elts > 0)
	{
		qsort(elt_list, (size_t) count_elts,
			  (size_t) sizeof(struct elt_list), elt_list_compare);
		elt_list_combine();
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
write_mb_for_species_list(int n)
/* ---------------------------------------------------------------------- */
{
/*
 *   Sets up data to add to species_list
 *   Original secondary redox states are retained
 */
	int i;
/*
 *   Start with secondary reaction
 */
	count_trxn = 0;
	trxn_add(s[n]->rxn_s, 1.0, FALSE);
/*
 *   Copy to elt_list
 */
	count_elts = 0;
	paren_count = 0;
	for (i = 1; i < count_trxn; i++)
	{
		if (trxn.token[i].s->secondary == NULL)
		{
			char * temp_name = string_duplicate(trxn.token[i].s->primary->elt->name);
			char * ptr = temp_name;
			get_secondary_in_species(&ptr, trxn.token[i].coef);
			free_check_null(temp_name);
		}
		else
		{
			char * temp_name = string_duplicate(trxn.token[i].s->secondary->elt->name);
			char * ptr = temp_name;
			if (get_secondary_in_species(&ptr, trxn.token[i].coef) == ERROR)
			{
				input_error++;
				error_string = sformatf( "Error parsing %s.", trxn.token[i].s->secondary->elt->name);
				error_msg(error_string, CONTINUE);
			}
			free_check_null(temp_name);
		}
	}
	for (i = 0; i < count_elts; i++)
	{
		if (strcmp(elt_list[i].elt->name, "O(-2)") == 0)
		{
			if (count_elts >= max_elts)
			{
				space((void **) ((void *) &elt_list), count_elts, &max_elts,
					  sizeof(struct elt_list));
			}
			elt_list[count_elts].elt = element_h_one;
			elt_list[count_elts].coef = elt_list[i].coef * 2;
			count_elts++;
		}
	}
	if (count_elts > 0)
	{
		qsort(elt_list, (size_t) count_elts,
			  (size_t) sizeof(struct elt_list), elt_list_compare);
		elt_list_combine();
	}
	s[n]->next_sys_total =
		(struct elt_list *) free_check_null(s[n]->next_sys_total);
	s[n]->next_sys_total = elt_list_save();
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
write_phase_sys_total(int n)
/* ---------------------------------------------------------------------- */
{
/*
 *   Sets up data to add to species_list
 *   Original secondary redox states are retained
 */
	int i;
/*
 *   Start with secondary reaction
 */
	count_trxn = 0;
	trxn_add_phase(phases[n]->rxn_s, 1.0, FALSE);
/*
 *   Copy to elt_list
 */
	count_elts = 0;
	paren_count = 0;
	for (i = 1; i < count_trxn; i++)
	{
		if (trxn.token[i].s->secondary == NULL)
		{
			char * temp_name = string_duplicate(trxn.token[i].s->primary->elt->name);
			char *ptr = temp_name;
			get_secondary_in_species(&ptr, trxn.token[i].coef);
			free_check_null(temp_name);
		}
		else
		{
			char * temp_name = string_duplicate(trxn.token[i].s->secondary->elt->name);
			char *ptr = temp_name;
			get_secondary_in_species(&ptr, trxn.token[i].coef);
			free_check_null(temp_name);
		}
	}
	for (i = 0; i < count_elts; i++)
	{
		if (strcmp(elt_list[i].elt->name, "O(-2)") == 0)
		{
			if (count_elts >= max_elts)
			{
				space((void **) ((void *) &elt_list), count_elts, &max_elts,
					  sizeof(struct elt_list));
			}
			elt_list[count_elts].elt = element_h_one;
			elt_list[count_elts].coef = elt_list[i].coef * 2;
			count_elts++;
		}
	}
	if (count_elts > 0)
	{
		qsort(elt_list, (size_t) count_elts,
			  (size_t) sizeof(struct elt_list), elt_list_compare);
		elt_list_combine();
	}
	phases[n]->next_sys_total =
		(struct elt_list *) free_check_null(phases[n]->next_sys_total);
	phases[n]->next_sys_total = elt_list_save();
	return (OK);
}

/* ---------------------------------------------------------------------- */
LDBLE Phreeqc::
calc_delta_v(reaction *r_ptr, bool phase)
/* ---------------------------------------------------------------------- */
{
/* calculate delta_v from molar volumes */

	int p = -1;
	LDBLE d_v = 0.0;

	if (phase)
		p = 1; /* for phases: reactants have coef's < 0, products have coef's > 0, v.v. for species */

	for (size_t i = 0; r_ptr->token[i].name; i++)
	{
		if (!r_ptr->token[i].s)
			continue;
		if (!strcmp(r_ptr->token[i].s->name, "H+"))
			continue;
		if (!strcmp(r_ptr->token[i].s->name, "e-"))
			continue;
		else if (r_ptr->token[i].s->logk[vm_tc])
			d_v += p * r_ptr->token[i].coef * r_ptr->token[i].s->logk[vm_tc];
	}
	return d_v;
}

/* ---------------------------------------------------------------------- */
LDBLE Phreeqc::
calc_lk_phase(phase *p_ptr, LDBLE TK, LDBLE pa)
/* ---------------------------------------------------------------------- */
{
/* 
 * calculate log_k for a single phase, correct for pressure 
 */

	reaction *r_ptr = (p_ptr->rxn_x ? p_ptr->rxn_x :\
		(p_ptr->rxn_s ? p_ptr->rxn_s : NULL));
	if (!r_ptr)
		return 0.0;
	if (!r_ptr->logk[vm0])
		return k_calc(r_ptr->logk, TK, pa * PASCAL_PER_ATM);

	LDBLE tc = TK - 273.15;
	LDBLE kp_t, d_v = 0.0;

	for (size_t i = 0; r_ptr->token[i].name; i++)
	{
		if (!r_ptr->token[i].s)
			continue;
		if (!strcmp(r_ptr->token[i].s->name, "H+"))
			continue;
		if (!strcmp(r_ptr->token[i].s->name, "e-"))
			continue;
		if (!strcmp(r_ptr->token[i].s->name, "H2O"))
		{
				d_v += r_ptr->token[i].coef * 18.016 / calc_rho_0(tc, pa);
		}
		else if (r_ptr->token[i].s->logk[vm0])
		{
			d_v += r_ptr->token[i].coef * (r_ptr->token[i].s->logk[vm0] +\
				(r_ptr->token[i].s->logk[vm1] + r_ptr->token[i].s->logk[vm2] * tc) * tc);
			if (r_ptr->token[i].s->logk[kappa])
			{
				kp_t = r_ptr->token[i].s->logk[kappa];
				//if (r_ptr->token[i].s->z > 0)
				//{
				//	/* correct kappa of cations for temperature, but kappa should be <= 0, Table 43.6 */
				//	kp_t += 4e-5 * (tc - 25);
				//	if (kp_t > 0)
				//		kp_t = 0;
				//}
				d_v += kp_t * pa;
			}
		}
	}
	d_v -= p_ptr->logk[vm0] + (p_ptr->logk[vm1] * tc +	p_ptr->logk[vm2] * tc) * tc;
	r_ptr->logk[delta_v] = d_v;

	if (!strcmp(r_ptr->token[0].name, "H2O(g)"))
		r_ptr->logk[delta_v] = 0.0;

	return k_calc(r_ptr->logk, TK, pa * PASCAL_PER_ATM);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
calc_vm(LDBLE tc, LDBLE pa)
/* ---------------------------------------------------------------------- */
{
/*
 *  Calculate molar volumes for aqueous species, using the millero parm's.
 *  Read.cpp copies millero[0..3] into logk[vm0 + 0..3], or reads them directly with -Vm.
 */
	LDBLE kp_t;

	/* S_v * Iv^0.5 is from Redlich and Meyer, Chem. Rev. 64, 221,
	   Use mu_x for the volume averaged Iv, the difference is negligible....*/
	LDBLE Sv_I = 0.5 * (1.444 + (0.016799 + (-8.4055e-6 + 5.5153e-7 * tc) * tc) * tc) * sqrt(mu_x);
	// Sv_I = 0.0;
	for (int i = 0; i < count_s_x; i++)
	{
		if (!strcmp(s_x[i]->name, "H2O"))
		{
			s_x[i]->logk[vm_tc] = 18.016 / rho_0;
			continue;
		}
		if (!s_x[i]->logk[vm0])
			continue;
		/* the volume terms... */
		s_x[i]->rxn_x->logk[vm_tc] = s_x[i]->logk[vm0] + (s_x[i]->logk[vm1] + s_x[i]->logk[vm2] * tc) * tc;
		if (s_x[i]->logk[kappa])
		{
			kp_t = s_x[i]->logk[kappa];
			//if (s_x[i]->z > 0)
			//{
			//	/* correct kappa of cations for temperature, but kappa should be <= 0, Table 43.6 */
			//	kp_t += 4e-5 * (tc - 25);
			//	if (kp_t > 0)
			//		kp_t = 0;
			//}
			s_x[i]->rxn_x->logk[vm_tc] += kp_t * pa;
		}

		/* the ionic strength term * Iv^0.5... */
		s_x[i]->rxn_x->logk[vm_tc] += 0.5 * s_x[i]->z * s_x[i]->z * Sv_I;
		/* plus the volume terms * Iv, take mu_x for Iv... */
		//s_x[i]->rxn_x->logk[vm_tc] += (s_x[i]->millero[3] + (s_x[i]->millero[4] + s_x[i]->millero[5] * tc) * tc) * mu_x;

		/* for calculating delta_v of the reaction... */
		s_search(s_x[i]->name)->logk[vm_tc] = s_x[i]->rxn_x->logk[vm_tc];
	}
	return OK;
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
k_temp(LDBLE tc, LDBLE pa) /* pa - pressure in atm */
/* ---------------------------------------------------------------------- */
{
/*
 *  Calculates log k's for all species and pure_phases
 */
	if (same_model == TRUE && same_temperature == TRUE && same_pressure == TRUE)
		return (OK);
	int i;
	LDBLE tempk = tc + 273.15;
/*
 *  Calculate log k for all aqueous species
 */
	/* calculate relative molar volumes for tc... */
	rho_0 = calc_rho_0(tc, pa);
	calc_vm(tc, pa);

	for (i = 0; i < count_s_x; i++)
	{
		if (s_x[i]->rxn_x->logk[vm_tc])
		/* calculate delta_v for the reaction... */
			s_x[i]->rxn_x->logk[delta_v] = calc_delta_v(s_x[i]->rxn_x, false);

		if (same_model && same_temperature && s_x[i]->rxn_x->logk[delta_v] == 0)
			continue;
		s_x[i]->lk = k_calc(s_x[i]->rxn_x->logk, tempk, pa * PASCAL_PER_ATM);
	}
/*
 *    Calculate log k for all pure phases
 */
	for (i = 0; i < count_phases; i++)
	{
		if (phases[i]->in == TRUE)  
		{
			phases[i]->rxn_x->logk[delta_v] = calc_delta_v(phases[i]->rxn_x, true) -
				(phases[i]->logk[vm0] + phases[i]->logk[vm1] * tc + phases[i]->logk[vm2] * tc * tc);
			phases[i]->lk = k_calc(phases[i]->rxn_x->logk, tempk, pa * PASCAL_PER_ATM);
		}
	}
/*
 *    Calculate miscibility gaps for solid solutions
 */
	if (use.Get_ss_assemblage_ptr() != NULL)
	{
		for (i = 0; i < use.Get_ss_assemblage_ptr()->count_s_s; i++)
		{
			if (fabs(tempk - use.Get_ss_assemblage_ptr()->s_s[i].tk) > 0.01)
			{
				s_s_prep(tempk, &(use.Get_ss_assemblage_ptr()->s_s[i]), FALSE);
			}
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
LDBLE Phreeqc::
k_calc(LDBLE * l_logk, LDBLE tempk, LDBLE presPa)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Calculates log k at specified temperature and pressure
	 *   Returns calculated log k.
	 *
	 *   delta_v is in cm3/mol.
	 */

	/* Molar energy */
	LDBLE me = tempk * R_KJ_DEG_MOL;

	/* Pressure difference */
	LDBLE delta_p = presPa - REF_PRES_PASCAL;

	/* Calculate new log k value for this temperature and pressure */
	LDBLE lk = l_logk[logK_T0] 
		- l_logk[delta_h] * (298.15 - tempk) / (LOG_10 * me * 298.15)
		+ l_logk[T_A1]
		+ l_logk[T_A2] * tempk
		+ l_logk[T_A3] / tempk
		+ l_logk[T_A4] * log10(tempk)
		+ l_logk[T_A5] / (tempk * tempk)
		+ l_logk[T_A6] * tempk * tempk;
	if (delta_p > 0)
		/* cm3 * J /mol = 1e-9 m3 * kJ /mol */
		lk -= l_logk[delta_v] * 1E-9 * delta_p / (LOG_10 * me);
	return lk;
}


/* ---------------------------------------------------------------------- */
 int Phreeqc::
save_model(void)
/* ---------------------------------------------------------------------- */
{
	int i;
/*
 *   save temperature
 */
	last_model.temperature = tc_x;
/*
 *   save pressure
 */
	last_model.pressure = patm_x;
/*
 *   mark master species 
 */
	for (i = 0; i < count_master; i++)
	{
		master[i]->last_model = FALSE;
		if (master[i]->total > 0)
		{
			if (master[i]->primary == TRUE)
			{
				master[i]->last_model = TRUE;
			}
			else
			{
				/* mark primary master */
				master[i]->s->secondary->elt->primary->last_model = TRUE;
			}
		}
	}
/*
 *   save list of phase pointers for gas phase
 */
	last_model.gas_phase =
		(struct phase **) free_check_null(last_model.gas_phase);
	if (use.Get_gas_phase_ptr() != NULL)
	{
		cxxGasPhase * gas_phase_ptr = (cxxGasPhase *) use.Get_gas_phase_ptr();
		last_model.count_gas_phase = (int) gas_phase_ptr->Get_gas_comps().size();
		last_model.gas_phase =
			(struct phase **) PHRQ_malloc((size_t) last_model.count_gas_phase *
										  sizeof(struct phase *));
		if (last_model.gas_phase == NULL)
			malloc_error();
		for (size_t i = 0; i < gas_phase_ptr->Get_gas_comps().size(); i++)
		{	
			cxxGasComp *gc_ptr = &(gas_phase_ptr->Get_gas_comps()[i]);
			int k;
			struct phase *phase_ptr = phase_bsearch(gc_ptr->Get_phase_name().c_str() , &k, FALSE);
			assert(phase_ptr);
			last_model.gas_phase[i] = phase_ptr;
		}
	}
	else
	{
		last_model.count_gas_phase = 0;
		last_model.gas_phase = NULL;
	}
/*
 *   save list of names of solid solutions
 */
	last_model.ss_assemblage =
		(const char **) free_check_null(last_model.ss_assemblage);
	if (use.Get_ss_assemblage_ptr() != NULL)
	{
		last_model.count_ss_assemblage = use.Get_ss_assemblage_ptr()->count_s_s;
		last_model.ss_assemblage =
			(const char **) PHRQ_malloc((size_t) use.Get_ss_assemblage_ptr()->
								  count_s_s * sizeof(char *));
		if (last_model.ss_assemblage == NULL)
			malloc_error();
		for (i = 0; i < use.Get_ss_assemblage_ptr()->count_s_s; i++)
		{
			last_model.ss_assemblage[i] =
				use.Get_ss_assemblage_ptr()->s_s[i].name;
		}
	}
	else
	{
		last_model.count_ss_assemblage = 0;
		last_model.ss_assemblage = NULL;
	}
/*
 *   save list of phase pointers for pp_assemblage
 */
	last_model.pp_assemblage =
		(struct phase **) free_check_null(last_model.pp_assemblage);
	last_model.add_formula =
		(const char **) free_check_null(last_model.add_formula);
	last_model.si = (LDBLE *) free_check_null(last_model.si);
	if (use.Get_pp_assemblage_ptr() != NULL)
	{
		cxxPPassemblage * pp_assemblage_ptr = (cxxPPassemblage *) use.Get_pp_assemblage_ptr();
		last_model.count_pp_assemblage = (int) pp_assemblage_ptr->Get_pp_assemblage_comps().size();
		last_model.pp_assemblage =
			(struct phase **) PHRQ_malloc((size_t) last_model.count_pp_assemblage *
										  sizeof(struct phase *));
		if (last_model.pp_assemblage == NULL)
			malloc_error();
		last_model.add_formula =
			(const char **) PHRQ_malloc((size_t)last_model.count_pp_assemblage * sizeof(char *));
		if (last_model.add_formula == NULL)
			malloc_error();
		last_model.si =
			(LDBLE *) PHRQ_malloc((size_t) last_model.count_pp_assemblage * sizeof(LDBLE));
		if (last_model.si == NULL)
			malloc_error();
		std::map<std::string, cxxPPassemblageComp>::iterator it;
		it =  pp_assemblage_ptr->Get_pp_assemblage_comps().begin();
		i = 0;
		for ( ; it != pp_assemblage_ptr->Get_pp_assemblage_comps().end(); it++)
		{
			int j;
			struct phase * phase_ptr = phase_bsearch(it->first.c_str(), &j, false);
			assert(phase_ptr);
			last_model.pp_assemblage[i] = phase_ptr;
			last_model.add_formula[i] = string_hsave(it->second.Get_add_formula().c_str());
			last_model.si[i] = it->second.Get_si();
			i++;
		}
	}
	else
	{
		last_model.count_pp_assemblage = 0;
		last_model.pp_assemblage = NULL;
		last_model.add_formula = NULL;
		last_model.si = NULL;
	}
#ifdef SKIP
	if (use.Get_pp_assemblage_ptr() != NULL)
	{
		last_model.count_pp_assemblage = use.Get_pp_assemblage_ptr()->count_comps;
		last_model.pp_assemblage =
			(struct phase **) PHRQ_malloc((size_t) use.Get_pp_assemblage_ptr()->
										  count_comps *
										  sizeof(struct phase *));
		if (last_model.pp_assemblage == NULL)
			malloc_error();
		last_model.add_formula =
			(const char **) PHRQ_malloc((size_t) use.Get_pp_assemblage_ptr()->
								  count_comps * sizeof(char *));
		if (last_model.add_formula == NULL)
			malloc_error();
		last_model.si =
			(LDBLE *) PHRQ_malloc((size_t) use.Get_pp_assemblage_ptr()->
								  count_comps * sizeof(LDBLE));
		if (last_model.si == NULL)
			malloc_error();
		for (i = 0; i < use.Get_pp_assemblage_ptr()->count_comps; i++)
		{
			last_model.pp_assemblage[i] =
				use.Get_pp_assemblage_ptr()->pure_phases[i].phase;
			last_model.add_formula[i] =
				use.Get_pp_assemblage_ptr()->pure_phases[i].add_formula;
			last_model.si[i] = use.Get_pp_assemblage_ptr()->pure_phases[i].si;
		}
	}
	else
	{
		last_model.count_pp_assemblage = 0;
		last_model.pp_assemblage = NULL;
		last_model.add_formula = NULL;
		last_model.si = NULL;
	}
#endif
/*
 *   save data for surface
 */
	last_model.surface_comp =
		(const char **) free_check_null(last_model.surface_comp);
	last_model.surface_charge =
		(const char **) free_check_null(last_model.surface_charge);
	if (use.Get_surface_ptr() != NULL)
	{
		/* comps */
		last_model.count_surface_comp = use.Get_surface_ptr()->count_comps;
		last_model.surface_comp =
			(const char **) PHRQ_malloc((size_t) use.Get_surface_ptr()->count_comps *
								  sizeof(char *));
		if (last_model.surface_comp == NULL)
			malloc_error();
		for (i = 0; i < use.Get_surface_ptr()->count_comps; i++)
		{
			last_model.surface_comp[i] = use.Get_surface_ptr()->comps[i].formula;
		}
		/* charge */
		last_model.count_surface_charge = use.Get_surface_ptr()->count_charge;
		last_model.surface_charge =
			(const char **) PHRQ_malloc((size_t) use.Get_surface_ptr()->count_charge *
								  sizeof(char *));
		if (last_model.surface_charge == NULL)
			malloc_error();
		for (i = 0; i < use.Get_surface_ptr()->count_charge; i++)
		{
			last_model.surface_charge[i] = use.Get_surface_ptr()->charge[i].name;
		}
		last_model.dl_type = use.Get_surface_ptr()->dl_type;
		/*last_model.edl = use.Get_surface_ptr()->edl; */
		last_model.surface_type = use.Get_surface_ptr()->type;
	}
	else
	{
		last_model.dl_type = NO_DL;
		/*last_model.edl = -1; */
		last_model.surface_type = UNKNOWN_DL;
		last_model.count_surface_comp = 0;
		last_model.surface_comp = NULL;
		last_model.count_surface_charge = 0;
		last_model.surface_charge = NULL;
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
check_same_model(void)
/* ---------------------------------------------------------------------- */
{
	int i;
/*
 *   Force new model to be built in prep
 */
	if (last_model.force_prep == TRUE)
	{
		last_model.force_prep = FALSE;
		same_temperature = FALSE;
		same_pressure = FALSE;
		return (FALSE);
	}
/*
 *   Check temperature
 */
	if (fabs(tc_x - last_model.temperature) < 0.05)
	{
		same_temperature = TRUE;
	}
	else
	{
		same_temperature = FALSE;
	}
/*
 *   Check pressure
 */
	if (fabs(patm_x - last_model.pressure) < 0.01)
	{
		same_pressure = TRUE;
	}
	else
	{
		same_pressure = FALSE;
	}
/*
 *   Check master species
 */
	for (i = 0; i < count_master; i++)
	{
/*
		output_msg(sformatf("%s\t%e\t%d\n", master[i]->elt->name,
			master[i]->total, master[i]->last_model);
 */
		if (master[i]->s == s_hplus || master[i]->s == s_h2o)
			continue;
		if (master[i]->total > MIN_TOTAL && master[i]->last_model == TRUE)
		{
			if (master[i]->s->secondary != NULL)
			{
				if (master[i]->s->secondary->unknown != NULL)
					continue;
			}
			else
			{
				if (master[i]->unknown != NULL)
					continue;
			}
		}
		if (master[i]->total <= MIN_TOTAL && master[i]->last_model == FALSE)
			continue;
		return (FALSE);
	}
/*
 *   Check gas_phase
 */
	if (use.Get_gas_phase_ptr() != NULL)
	{
		cxxGasPhase * gas_phase_ptr = (cxxGasPhase *) use.Get_gas_phase_ptr();
		if (last_model.gas_phase == NULL)
			return (FALSE);
		if (last_model.count_gas_phase != (int) gas_phase_ptr->Get_gas_comps().size())
			return (FALSE);
		for (i = 0; i < (int) gas_phase_ptr->Get_gas_comps().size(); i++)
		{
			cxxGasComp *gc_ptr = &(gas_phase_ptr->Get_gas_comps()[i]);
			int k;
			struct phase *phase_ptr = phase_bsearch(gc_ptr->Get_phase_name().c_str() , &k, FALSE);
			assert(phase_ptr);
			if (last_model.gas_phase[i] != phase_ptr)
			{
				return (FALSE);
			}
		}
	}
	else
	{
		if (last_model.gas_phase != NULL)
			return (FALSE);
	}
/*
 *   Check solid solutions
 */
	if (use.Get_ss_assemblage_ptr() != NULL)
	{
		if (last_model.count_ss_assemblage !=
			use.Get_ss_assemblage_ptr()->count_s_s)
			return (FALSE);
		for (i = 0; i < use.Get_ss_assemblage_ptr()->count_s_s; i++)
		{
			if (last_model.ss_assemblage[i] !=
				use.Get_ss_assemblage_ptr()->s_s[i].name)
			{
				return (FALSE);
			}
		}
	}
	else
	{
		if (last_model.ss_assemblage != NULL)
			return (FALSE);
	}
/*
 *   Check pure_phases
 */
	if (use.Get_pp_assemblage_ptr() != NULL)
	{
		cxxPPassemblage * pp_assemblage_ptr = (cxxPPassemblage *) use.Get_pp_assemblage_ptr();
		if (last_model.count_pp_assemblage != (int) pp_assemblage_ptr->Get_pp_assemblage_comps().size())
			return (FALSE);

		std::map<std::string, cxxPPassemblageComp>::iterator it;
		it =  pp_assemblage_ptr->Get_pp_assemblage_comps().begin();
		i = 0;
		for ( ; it != pp_assemblage_ptr->Get_pp_assemblage_comps().end(); it++)
		{
			int j;
			struct phase * phase_ptr = phase_bsearch(it->first.c_str(), &j, FALSE);
			assert(phase_ptr); 
			if (last_model.pp_assemblage[i] != phase_ptr)
			{
				return (FALSE);
			}
			if (last_model.add_formula[i] !=
				string_hsave(it->second.Get_add_formula().c_str()))
			{
				return (FALSE);
			}
			i++;
			/* A. Crapsi
			if (last_model.si[i] != use.Get_pp_assemblage_ptr()->pure_phases[i].si)
			{
				return (FALSE);
			}
			*/
		}
	}
	else
	{
		if (last_model.pp_assemblage != NULL)
			return (FALSE);
	}
#ifdef SKIP
	if (use.Get_pp_assemblage_ptr() != NULL)
	{
		if (last_model.count_pp_assemblage !=
			use.Get_pp_assemblage_ptr()->count_comps)
			return (FALSE);
		for (i = 0; i < use.Get_pp_assemblage_ptr()->count_comps; i++)
		{
			if (last_model.pp_assemblage[i] !=
				use.Get_pp_assemblage_ptr()->pure_phases[i].phase)
			{
				return (FALSE);
			}
			if (last_model.add_formula[i] !=
				use.Get_pp_assemblage_ptr()->pure_phases[i].add_formula)
			{
				return (FALSE);
			}
			/* A. Crapsi
			if (last_model.si[i] != use.Get_pp_assemblage_ptr()->pure_phases[i].si)
			{
				return (FALSE);
			}
			*/
		}
	}
	else
	{
		if (last_model.pp_assemblage != NULL)
			return (FALSE);
	}
#endif
/*
 *   Check surface
 */
	if (use.Get_surface_ptr() != NULL)
	{
		if (last_model.count_surface_comp != use.Get_surface_ptr()->count_comps)
			return (FALSE);
		if (last_model.count_surface_charge != use.Get_surface_ptr()->count_charge)
			return (FALSE);
		if (last_model.dl_type != use.Get_surface_ptr()->dl_type)
			return (FALSE);
		/*if (last_model.edl != use.Get_surface_ptr()->edl) return(FALSE); */
		if (last_model.surface_type != use.Get_surface_ptr()->type)
			return (FALSE);
		/*
		   if (last_model.only_counter_ions != use.Get_surface_ptr()->only_counter_ions) return(FALSE);
		 */
		for (i = 0; i < use.Get_surface_ptr()->count_comps; i++)
		{
			if (last_model.surface_comp[i] !=
				use.Get_surface_ptr()->comps[i].formula)
				return (FALSE);
		}
		for (i = 0; i < use.Get_surface_ptr()->count_charge; i++)
		{
			if (last_model.surface_charge[i] !=
				use.Get_surface_ptr()->charge[i].name)
				return (FALSE);
		}
	}
	else
	{
		if (last_model.surface_comp != NULL)
			return (FALSE);
	}
/*
 *   Model is the same
 */
	return (TRUE);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
build_min_exch(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Defines proportionality factor between mineral and exchanger to
 *   jacob0
 */
	int j, k, jj;
	int row;
	//struct exch_comp *comp_ptr;
	struct master *master_ptr;
	struct unknown *unknown_ptr;
	LDBLE coef;

	if (use.Get_exchange_ptr() == NULL)
		return (OK);
	cxxExchange *ex_ptr = (cxxExchange *) use.Get_exchange_ptr();
	int n_user = ex_ptr->Get_n_user();
	cxxExchange * exchange_ptr = Utilities::Rxn_find(Rxn_exchange_map, n_user);
	if (exchange_ptr == NULL)
	{
		input_error++;
		error_string = sformatf( "Exchange %d not found.",
				use.Get_n_exchange_user());
		error_msg(error_string, CONTINUE);
	}
	n_user = exchange_ptr->Get_n_user();
	if (!exchange_ptr->Get_related_phases())
		return (OK);
	std::vector<cxxExchComp *> comps = exchange_ptr->Vectorize();
	for (size_t i = 0; i < comps.size(); i++)
	{
		cxxExchComp * comp_ptr = comps[i];
		if (comp_ptr->Get_phase_name().size() == 0)
			continue;
		// Find exchange master
		cxxNameDouble nd(comp_ptr->Get_totals());
		cxxNameDouble::iterator it = nd.begin();
		struct master *exchange_master = NULL;
		for ( ; it != nd.end(); it++)
		{
			element * elt_ptr = element_store(it->first.c_str());
			assert (elt_ptr);
			if (elt_ptr->master->type == EX)
			{
				exchange_master = elt_ptr->master;
			}
		}
		if (exchange_master == NULL)
		{
			input_error++;
			error_string = sformatf(
					"Did not find master exchange species for %s",
					comp_ptr->Get_formula().c_str());
			error_msg(error_string, CONTINUE);
			continue;
		}
		/* find unknown number */
		for (j = count_unknowns - 1; j >= 0; j--)
		{
			if (x[j]->type != EXCH)
				continue;
			if (x[j]->master[0] == exchange_master)
				break;
		}
		for (k = count_unknowns - 1; k >= 0; k--)
		{
			if (x[k]->type != PP)
				continue;
			//if (x[k]->phase->name == exchange[n].comps[i].phase_name)
			if (x[k]->phase->name == string_hsave(comp_ptr->Get_phase_name().c_str()))
				break;
		}
		if (j == -1)
		{
			input_error++;
			error_string = sformatf(
					"Did not find unknown for master exchange species %s",
					exchange_master->s->name);
			error_msg(error_string, CONTINUE);
		}
		if (j == -1 || k == -1)
			continue;
/*
 *   Build jacobian
 */

		/* charge balance */
		store_jacob0(charge_balance_unknown->number, x[k]->number,
					 comp_ptr->Get_formula_z() * comp_ptr->Get_phase_proportion());
		store_sum_deltas(&delta[k], &charge_balance_unknown->delta,
						 -comp_ptr->Get_formula_z() * comp_ptr->Get_phase_proportion());


		/* mole balance balance */
		count_elts = 0;
		paren_count = 0;
		add_elt_list(comp_ptr->Get_formula_totals(), 1.0);
#ifdef COMBINE
		change_hydrogen_in_elt_list(0);
#endif
		for (jj = 0; jj < count_elts; jj++)
		{
			master_ptr = elt_list[jj].elt->primary;
			if (master_ptr == NULL)
			{
				input_error++;
				error_string = sformatf(
						"Did not find unknown for exchange related to mineral %s",
						comp_ptr->Get_phase_name().c_str());
				error_msg(error_string, STOP);
			}
			if (master_ptr->in == FALSE)
			{
				master_ptr = master_ptr->s->secondary;
			}
			if (master_ptr->s->type == EX)
			{
				if (equal
					(x[j]->moles,
					 x[k]->moles * elt_list[jj].coef *
					 comp_ptr->Get_phase_proportion(),
					 5.0 * convergence_tolerance) == FALSE)
				{
					error_string = sformatf(
							"Resetting number of sites in exchanger %s (=%e) to be consistent with moles of phase %s (=%e).\n%s",
							master_ptr->s->name, (double) x[j]->moles,
							comp_ptr->Get_phase_name().c_str(),
							(double) (x[k]->moles * elt_list[jj].coef *
									  comp_ptr->Get_phase_proportion()),
							"\tHas equilibrium_phase assemblage been redefined?\n");
					warning_msg(error_string);
					x[j]->moles =
						x[k]->moles * elt_list[jj].coef *
						comp_ptr->Get_phase_proportion();
				}
			}
			coef = elt_list[jj].coef;
			if (master_ptr->s == s_hplus)
			{
				row = mass_hydrogen_unknown->number;
				unknown_ptr = mass_hydrogen_unknown;
			}
			else if (master_ptr->s == s_h2o)
			{
				row = mass_oxygen_unknown->number;
				unknown_ptr = mass_oxygen_unknown;
			}
			else
			{
				row = master_ptr->unknown->number;
				unknown_ptr = master_ptr->unknown;
			}
			store_jacob0(row, x[k]->number,
						 coef * comp_ptr->Get_phase_proportion());
			store_sum_deltas(&delta[k], &unknown_ptr->delta,
							 -coef * comp_ptr->Get_phase_proportion());
		}
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
build_min_surface(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Defines proportionality factor between mineral and surface to
 *   jacob0
 */
	int i, j, k, n, jj, row;
	struct elt_list *next_elt;
	struct surface_comp *comp_ptr;
	struct unknown *unknown_ptr;
	struct master *master_ptr;
	LDBLE coef;

	if (use.Get_surface_ptr() == NULL)
		return (OK);
	if (surface_bsearch(use.Get_surface_ptr()->n_user, &n) == NULL)
	{
		input_error++;
		error_string = sformatf( "Surface %d not found.",
				use.Get_surface_ptr()->n_user);
		error_msg(error_string, CONTINUE);
	}
	if (surface[n].related_phases == FALSE)
		return (OK);
	for (i = 0; i < surface[n].count_comps; i++)
	{
		if (surface[n].comps[i].phase_name == NULL)
			continue;

		/* find unknown number */
		for (j = count_unknowns - 1; j >= 0; j--)
		{
			if (x[j]->type != SURFACE)
				continue;
			if (x[j]->master[0] == surface[n].comps[i].master)
				break;
		}
		for (k = count_unknowns - 1; k >= 0; k--)
		{
			if (x[k]->type != PP)
				continue;
			if (x[k]->phase->name == surface[n].comps[i].phase_name)
				break;
		}
		if (j == -1)
		{
			input_error++;
			error_string = sformatf(
					"Did not find unknown for master surface species %s",
					surface[n].comps[i].master->s->name);
			error_msg(error_string, CONTINUE);
		}
		if (j == -1 || k == -1)
			continue;

		comp_ptr = x[j]->surface_comp;

		if (surface[n].type == CD_MUSIC)
		{
			/* Add formula for CD_MUSIC */
			next_elt = comp_ptr->formula_totals;
		}
		else
		{
			/* Add master species for non CD_MUSIC */
			next_elt = x[j]->master[0]->s->next_elt;
		}


		/* update grams == moles in this case */
		if (j < count_unknowns - 1 && x[j + 1]->type == SURFACE_CB)
		{
			store_sum_deltas(&delta[k], &(x[j + 1]->related_moles), -1.0);
		}

		/* charge balance */
		store_jacob0(charge_balance_unknown->number, x[k]->number,
					 comp_ptr->formula_z * comp_ptr->phase_proportion);
		store_sum_deltas(&delta[k], &charge_balance_unknown->delta,
						 -comp_ptr->formula_z * comp_ptr->phase_proportion);


		count_elts = 0;
		paren_count = 0;
		add_elt_list(next_elt, 1.0);
#ifdef COMBINE
		change_hydrogen_in_elt_list(0);
#endif
		for (jj = 0; jj < count_elts; jj++)
		{
			master_ptr = elt_list[jj].elt->primary;
			if (master_ptr->in == FALSE)
			{
				master_ptr = master_ptr->s->secondary;
			}
			if (master_ptr == NULL)
			{
				input_error++;
				error_string = sformatf(
						"Did not find unknown for surface related to mineral %s",
						surface[n].comps[i].phase_name);
				error_msg(error_string, STOP);
			}
			if (master_ptr->s->type == SURF)
			{
				if (equal
					(x[j]->moles,
					 x[k]->moles * elt_list[jj].coef *
					 comp_ptr->phase_proportion,
					 5.0 * convergence_tolerance) == FALSE)
				{
					error_string = sformatf(
							"Resetting number of sites in surface %s (=%e) to be consistent with moles of phase %s (=%e).\n%s",
							master_ptr->s->name, (double) x[j]->moles,
							comp_ptr->phase_name,
							(double) (x[k]->moles * elt_list[jj].coef *
									  comp_ptr->phase_proportion),
							"\tHas equilibrium_phase assemblage been redefined?\n");
					warning_msg(error_string);
					x[j]->moles =
						x[k]->moles * elt_list[jj].coef *
						comp_ptr->phase_proportion;
				}
			}
			coef = elt_list[jj].coef;
			if (master_ptr->s == s_hplus)
			{
				row = mass_hydrogen_unknown->number;
				unknown_ptr = mass_hydrogen_unknown;
			}
			else if (master_ptr->s == s_h2o)
			{
				row = mass_oxygen_unknown->number;
				unknown_ptr = mass_oxygen_unknown;
			}
			else
			{
				row = master_ptr->unknown->number;
				unknown_ptr = master_ptr->unknown;
			}
			store_jacob0(row, x[k]->number,
						 coef * comp_ptr->phase_proportion);
			store_sum_deltas(&delta[k], &unknown_ptr->delta,
							 -coef * comp_ptr->phase_proportion);
		}

	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
setup_related_surface(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Fill in data for surface assemblage in unknown structure
 */
	int i, k;
	struct surface_comp *comp_ptr;

	if (use.Get_surface_ptr() == NULL)
		return (OK);
	if (use.Get_surface_ptr()->related_phases == FALSE)
		return (OK);

	for (i = 0; i < count_unknowns; i++)
	{
		if (x[i]->type == SURFACE && x[i]->surface_comp->phase_name != NULL)
		{
			for (k = count_unknowns - 1; k >= 0; k--)
			{
				if (x[k]->type != PP)
					continue;
				if (x[k]->phase->name == x[i]->surface_comp->phase_name)
					break;
			}
			if (k == -1)
				continue;

			comp_ptr = x[i]->surface_comp;
			x[i]->phase_unknown = x[k];
/* !!!!! */
			x[i]->moles = x[k]->moles * comp_ptr->phase_proportion;

		}
		else if (x[i]->type == SURFACE_CB
				 && x[i - 1]->surface_comp->phase_name != NULL)
		{
			for (k = count_unknowns - 1; k >= 0; k--)
			{
				if (x[k]->type != PP)
					continue;
				if (x[k]->phase->name == x[i]->surface_comp->phase_name)
					break;
			}
			if (k == -1)
				continue;

			comp_ptr = x[i]->surface_comp;
			x[i]->phase_unknown = x[k];
/* !!!! Added for security, not checked... */
			x[i]->related_moles = x[k]->moles * comp_ptr->phase_proportion;
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
change_hydrogen_in_elt_list(LDBLE charge)
/* ---------------------------------------------------------------------- */
{
	int j;
	int found_h, found_o;
	LDBLE coef_h, coef_o, coef;
	found_h = -1;
	found_o = -1;
	coef_h = 0.0;
	coef_o = 0.0;
	qsort(elt_list, (size_t) count_elts,
		  (size_t) sizeof(struct elt_list), elt_list_compare);
	elt_list_combine();
	for (j = 0; j < count_elts; j++)
	{
		if (strcmp(elt_list[j].elt->name, "H") == 0)
		{
			found_h = j;
			coef_h = elt_list[j].coef;
		}
		else if (strcmp(elt_list[j].elt->name, "O") == 0)
		{
			found_o = j;
			coef_o = elt_list[j].coef;
		}
	}
	coef = coef_h - 2 * coef_o - charge;
	if (found_h < 0 && found_o < 0)
		return (OK);
	if (found_h >= 0 && found_o < 0)
		return (OK);
	if (found_h < 0 && found_o >= 0)
	{
		elt_list[count_elts].elt = s_hplus->primary->elt;
		elt_list[count_elts].coef = coef;
		count_elts++;
		qsort(elt_list, (size_t) count_elts,
			  (size_t) sizeof(struct elt_list), elt_list_compare);
		elt_list_combine();
		return (OK);
	}
	elt_list[found_h].coef = coef;
	return (OK);
}
