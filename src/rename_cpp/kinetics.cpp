#include "Utils.h"
#include "Phreeqc.h"
#include "phqalloc.h"

#include <time.h>

#include "StorageBin.h"
#include "Reaction.h"
#include "cxxKinetics.h"
#include "Solution.h"
#include "cxxMix.h"
#include "PPassemblage.h"
#include "Surface.h"
#include "Exchange.h"
#include "GasPhase.h"
#include "SSassemblage.h"
#include "Temperature.h"

#include <map>
#include <fstream>
#include "nvector_serial.h"		/* definitions of type N_Vector and macro          */
							 /* NV_Ith_S, prototypes for N_VNew, N_VFree      */
/* These macros are defined in order to write code which exactly matches
   the mathematical problem description given above.

   Ith(v,i) references the ith component of the vector v, where i is in
   the range [1..NEQ] and NEQ is defined below. The Ith macro is defined
   using the N_VIth macro in nvector.h. N_VIth numbers the components of
   a vector starting from 0.

   IJth(A,i,j) references the (i,j)th element of the dense matrix A, where
   i and j are in the range [1..NEQ]. The IJth macro is defined using the
   DENSE_ELEM macro in dense.h. DENSE_ELEM numbers rows and columns of a
   dense matrix starting from 0. */

#define Ith(v,i)	NV_Ith_S(v,i-1)	/* Ith numbers components 1..NEQ */
#define IJth(A,i,j) DENSE_ELEM(A,i-1,j-1)	/* IJth numbers rows,cols 1..NEQ */

#define MAX_DIVIDE 2
#define KINETICS_TOL 1e-8;

/* ---------------------------------------------------------------------- */
int Phreeqc::
calc_kinetic_reaction(struct kinetics *kinetics_ptr, LDBLE time_step)
/* ---------------------------------------------------------------------- */
{
/*
 *	Go through kinetic components to
 *	determine rates and
 *	a list of elements and amounts in
 *	the reaction.
 */
	int i, j, return_value;
	LDBLE coef;
	char l_command[] = "run";
	struct rate *rate_ptr;
/*
 *   Go through list and generate list of elements and
 *   coefficient of elements in reaction
 */
	return_value = OK;
	count_elts = 0;
	paren_count = 0;
	rate_time = time_step;

/*	t1 = clock(); */
	for (i = 0; i < kinetics_ptr->count_comps; i++)
	{
		coef = 0.0;
/*
 *   Send command to basic interpreter
 */
		rate_ptr = rate_search(kinetics_ptr->comps[i].rate_name, &j);
		if (rate_ptr == NULL)
		{
			error_string = sformatf( "Rate not found for %s",
					kinetics_ptr->comps[i].rate_name);
			error_msg(error_string, STOP);
		}
		else
		{
			rate_moles = NAN;
			rate_m = kinetics_ptr->comps[i].m;
			rate_m0 = kinetics_ptr->comps[i].m0;
			rate_p = kinetics_ptr->comps[i].d_params;
			count_rate_p = kinetics_ptr->comps[i].count_d_params;
			if (rate_ptr->new_def == TRUE)
			{
				if (basic_compile
					(rates[j].commands, &rates[j].linebase, &rates[j].varbase,
					 &rates[j].loopbase) != 0)
				{
					error_string = sformatf( "Fatal Basic error in rate %s.",
							kinetics_ptr->comps[i].rate_name);
					error_msg(error_string, STOP);
				}

				rate_ptr->new_def = FALSE;
			}
			if (basic_run
				(l_command, rates[j].linebase, rates[j].varbase,
				 rates[j].loopbase) != 0)
			{
				error_string = sformatf( "Fatal Basic error in rate %s.",
						kinetics_ptr->comps[i].rate_name);
				error_msg(error_string, STOP);
			}
			if (rate_moles == NAN)
			{
				error_string = sformatf( "Moles of reaction not SAVE'd for %s.",
						kinetics_ptr->comps[i].rate_name);
				error_msg(error_string, STOP);
			}
			else
			{

				coef = rate_moles;
			}
		}
/*
 *   Accumulate moles of reaction for component
 */
		kinetics_ptr->comps[i].moles += coef;
		if (coef == 0.0)
			continue;
	}
/*	t2=clock();
	printf("secs in reac %e, t2 %e\n", t2-t1, t1);
 */
	return (return_value);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
calc_final_kinetic_reaction(struct kinetics *kinetics_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *	Go through kinetic components to
 *	using extrapolated values, which were
 *	stored in moles in run_kinetics
 */
	int i, j, k;
	LDBLE coef;
	char token[MAX_LENGTH];
	struct phase *phase_ptr;
	struct master *master_ptr;
/*
 *   Go through list and generate list of elements and
 *   coefficient of elements in reaction
 */
	kinetics_ptr->totals =
		(struct elt_list *) free_check_null(kinetics_ptr->totals);
	count_elts = 0;
	paren_count = 0;
	for (i = 0; i < kinetics_ptr->count_comps; i++)
	{
		if (kinetics_ptr->comps[i].moles > m_temp[i])
		{
			kinetics_ptr->comps[i].moles = m_temp[i];
			kinetics_ptr->comps[i].m = 0;
		}
		coef = kinetics_ptr->comps[i].moles;
		if (coef == 0.0)
			continue;
/*
 *   Reactant is a pure phase, copy formula into token
 */
		for (j = 0; j < kinetics_ptr->comps[i].count_list; j++)
		{
			phase_ptr = NULL;
			strcpy(token, kinetics_ptr->comps[i].list[j].name);
			phase_ptr = phase_bsearch(token, &k, FALSE);
			if (phase_ptr != NULL)
			{
				add_elt_list(phase_ptr->next_elt,
							 coef * kinetics_ptr->comps[i].list[j].coef);
			}
			else
			{
				char * temp_name = string_duplicate(kinetics_ptr->comps[i].list[j].name);
				char * ptr = temp_name;
				get_elts_in_species(&ptr,
									coef *
									kinetics_ptr->comps[i].list[j].coef);
				free_check_null(temp_name);
			}
		}
		if (use.Get_exchange_ptr() != NULL
			//&& use.Get_exchange_ptr()->related_rate == TRUE)
			&& ((cxxExchange *) use.Get_exchange_ptr())->Get_related_rate())
		{
			cxxExchange * exchange_ptr = (cxxExchange *) use.Get_exchange_ptr();
			std::map<std::string, cxxExchComp>::iterator it = exchange_ptr->Get_exchComps().begin();
			for ( ; it != exchange_ptr->Get_exchComps().end(); it++)
			{
				std::string name(it->second.Get_rate_name());

				if (name.size() > 0)
				{
					if (strcmp_nocase
						(kinetics_ptr->comps[i].rate_name,
						 name.c_str()) == 0)
					{
						/* found kinetics component */
						add_elt_list(it->second.Get_formula_totals(), -coef*it->second.Get_phase_proportion());
					}
				}
			}

		}
		if (use.Get_surface_ptr() != NULL && use.Get_surface_ptr()->related_rate == TRUE)
		{
			for (j = 0; j < use.Get_surface_ptr()->count_comps; j++)
			{
				if (use.Get_surface_ptr()->comps[j].rate_name != NULL)
				{
					if (strcmp_nocase
						(kinetics_ptr->comps[i].rate_name,
						 use.Get_surface_ptr()->comps[j].rate_name) == 0)
					{
						/* found kinetics component */
						char * temp_formula = string_duplicate(use.Get_surface_ptr()->comps[j].formula);
						char *ptr = temp_formula;
/* Surface = 0 when m becomes low ...
 */
						if (0.9 * use.Get_surface_ptr()->comps[j].phase_proportion *
							(kinetics_ptr->comps[i].m) < MIN_RELATED_SURFACE)
						{
							master_ptr = master_bsearch(ptr);
							master_ptr->total = 0.0;
						}
						else
						{
							get_elts_in_species(&ptr,
												-coef *
												use.Get_surface_ptr()->comps[j].
												phase_proportion);
						}
						free_check_null(temp_formula);
					}
				}
			}
		}
	}
	kinetics_ptr->totals = elt_list_save();
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
rk_kinetics(int i, LDBLE kin_time, int use_mix, int nsaver,
			LDBLE step_fraction)
/* ---------------------------------------------------------------------- */
{
/*
 * Runge-Kutta-Fehlberg method; 6 evaluations of the derivative
 *	give O(h^5) global error and error estimate
 *	calc_kinetic_reaction(.., ..) calculates moles of intermediate derivatives;
 *	these are calc'd for the whole step h.
 *	calc_final_kinetic reaction(..) translates moles to PHREEQC reaction.
 */
	int j, k, m, save_old;
	int l_bad, step_bad, step_ok;
	int n_reactions;
	LDBLE h, h_old, h_sum;
	LDBLE *rk_moles;
	LDBLE l_error, error_max, safety, moles_max, moles_reduction;
	struct kinetics *kinetics_ptr;
	int equal_rate, zero_rate;

	cxxPPassemblage *pp_assemblage_save = NULL;
	struct ss_assemblage *ss_assemblage_save = NULL;

	LDBLE b31 = 3. / 40., b32 = 9. / 40.,
		b51 = -11. / 54., b53 = -70. / 27., b54 = 35. / 27.,
		b61 = 1631. / 55296., b62 = 175. / 512., b63 = 575. / 13824., b64 =
		44275. / 110592., b65 = 253. / 4096., c1 = 37. / 378., c3 =
		250. / 621., c4 = 125. / 594., c6 = 512. / 1771., dc5 =
		-277. / 14336.;
	LDBLE dc1 = c1 - 2825. / 27648., dc3 = c3 - 18575. / 48384., dc4 =
		c4 - 13525. / 55296., dc6 = c6 - 0.25;
/*
 *  Save kinetics i and solution i, if necessary
 */
	save_old = -2 - (count_cells * (1 + stag_data->count_stag) + 2);
	kinetics_duplicate(i, save_old);
	if (nsaver != i)
	{
		solution_duplicate(i, save_old);
	}

/*
 *   Malloc some space
 */
	if (kinetics_bsearch(i, &m) == NULL)
		return (OK);
	n_reactions = kinetics[m].count_comps;
	rk_moles =
		(LDBLE *) PHRQ_malloc((size_t) 6 * n_reactions * sizeof(LDBLE));
	if (rk_moles == NULL)
		malloc_error();

	/*if (use_mix != NOMIX) last_model.force_prep = TRUE; */
	set_and_run_wrapper(i, use_mix, FALSE, i, step_fraction);
	run_reactions_iterations += iterations;

	saver();
	if (state == TRANSPORT || state == PHAST)
	{
		set_transport(i, NOMIX, TRUE, i);
	}
	else if (state == ADVECTION)
	{
		set_advection(i, NOMIX, TRUE, i);
	}
	else if (state == REACTION)
	{
		set_reaction(i, NOMIX, TRUE);
	}
#ifdef SKIP
	if (use.Get_pp_assemblage_ptr() != NULL)
	{
		pp_assemblage_save =
			(struct pp_assemblage *)
			PHRQ_malloc(sizeof(struct pp_assemblage));
		if (pp_assemblage_save == NULL)
			malloc_error();
	}
#endif
	if (use.Get_ss_assemblage_ptr() != NULL)
	{
		ss_assemblage_save =
			(struct ss_assemblage *)
			PHRQ_malloc(sizeof(struct ss_assemblage));
		if (ss_assemblage_save == NULL)
			malloc_error();
	}

	kinetics_ptr = kinetics_bsearch(i, &m);

	step_bad = step_ok = 0;
	l_bad = FALSE;
	h_sum = 0.;
	h = h_old = kin_time;
	moles_max = 0.1;
	moles_reduction = 1.0;
	safety = 0.7;
	if (kinetics_ptr->rk < 1)
		kinetics_ptr->rk = 1;
	else if (kinetics_ptr->rk > 3)
		kinetics_ptr->rk = 6;

	if (kinetics_ptr->rk == 6)
		equal_rate = FALSE;
	else
		equal_rate = TRUE;
/*
 * if step_divide > 1, initial timestep is divided
 * if			 < 1, step_divide indicates maximal reaction...
 */
	if (kinetics_ptr->step_divide > 1.0)
	{
		h = h_old = kin_time / kinetics_ptr->step_divide;
		equal_rate = FALSE;
	}
	else if (kinetics_ptr->step_divide < 1.0)
		moles_max = kinetics_ptr->step_divide;

	rate_sim_time = rate_sim_time_start + h_sum;

	status(0, NULL);
	while (h_sum < kin_time)
	{

		if (step_bad > kinetics_ptr->bad_step_max)
		{
			error_string = sformatf(
					"Bad RK steps > %d. Please decrease (time)step or increase -bad_step_max.",
					kinetics_ptr->bad_step_max);
			error_msg(error_string, STOP);
		}

	  MOLES_TOO_LARGE:
		if (moles_reduction > 1.0)
		{
			h_old = h;
			h = safety * h / (1.0 + moles_reduction);
			moles_reduction = 1.0;
			equal_rate = FALSE;
			l_bad = TRUE;
		}
/*
 *   find k1
 */
		if (l_bad == TRUE)
		{
			for (j = 0; j < n_reactions; j++)
			{
				rk_moles[j] *= (h / h_old);
				kinetics_ptr->comps[j].moles = rk_moles[j] * 0.2;
				kinetics_ptr->comps[j].m = m_temp[j];
			}
			l_bad = FALSE;
		}
		else
		{
/*
 *   define pointers for calc_kinetic_, they are lost after saver()...
 */
			if (state == TRANSPORT || state == PHAST)
			{
				set_transport(i, NOMIX, TRUE, i);
			}
			else if (state == ADVECTION)
			{
				set_advection(i, NOMIX, TRUE, i);
			}
			else if (state == REACTION)
			{
				set_reaction(i, NOMIX, TRUE);
			}
			/*
			 *   Moles of minerals and solid solutions may change to make positive
			 *   concentrations. Reactions may take out more than is present in
			 *   solution.
			 */
			if (use.Get_pp_assemblage_ptr() != NULL)
			{
				cxxPPassemblage *pp_assemblage_ptr = (cxxPPassemblage *) use.Get_pp_assemblage_ptr();
				cxxPPassemblage * pp_ptr = Utilities::Rxn_find(Rxn_pp_assemblage_map, pp_assemblage_ptr->Get_n_user());
				assert(pp_ptr);
				pp_assemblage_save = new cxxPPassemblage(*pp_ptr);
			}
			if (ss_assemblage_save != NULL)
			{
				ss_assemblage_copy(use.Get_ss_assemblage_ptr(),
									ss_assemblage_save,
									use.Get_ss_assemblage_ptr()->n_user);
			}
			for (j = 0; j < n_reactions; j++)
			{
				kinetics_ptr->comps[j].moles = 0.;
				m_temp[j] = kinetics_ptr->comps[j].m;
			}

			rate_sim_time = rate_sim_time_start + h_sum;
			calc_kinetic_reaction(kinetics_ptr, h);

			/* store k1 in rk_moles ... */
			for (j = 0; j < n_reactions; j++)
			{
				if (moles_reduction * moles_max <
					fabs(kinetics_ptr->comps[j].moles))
				{
					moles_reduction =
						fabs(kinetics_ptr->comps[j].moles) / moles_max;
				}
				/*  define reaction for calculating k2 ... */
				rk_moles[j] = kinetics_ptr->comps[j].moles;
				kinetics_ptr->comps[j].moles *= 0.2;
			}
			if (moles_reduction > 1.0)
				goto MOLES_TOO_LARGE;
		}
/*
 * Quit rk with rk = 1 and equal rates ...
 */
		if (kinetics_ptr->rk == 1 && equal_rate)
		{
			zero_rate = TRUE;
			for (j = 0; j < n_reactions; j++)
			{
				kinetics_ptr->comps[j].moles = rk_moles[j];
				if (fabs(kinetics_ptr->comps[j].moles) > MIN_TOTAL)
					zero_rate = FALSE;
			}

			if (zero_rate == FALSE)
			{
				calc_final_kinetic_reaction(kinetics_ptr);
				for (j = 0; j < n_reactions; j++)
				{
					kinetics_ptr->comps[j].m =
						m_temp[j] - kinetics_ptr->comps[j].moles;
					if (kinetics_ptr->comps[j].m < 1.e-30)
						kinetics_ptr->comps[j].m = 0;
					kinetics_ptr->comps[j].moles = 0.;
				}
				if (set_and_run_wrapper(i, NOMIX, TRUE, i, 0.) ==
					MASS_BALANCE)
				{
					run_reactions_iterations += iterations;
					moles_reduction = 9;
					goto MOLES_TOO_LARGE;
				}
				run_reactions_iterations += iterations;
				calc_kinetic_reaction(kinetics_ptr, h);
				for (j = 0; j < n_reactions; j++)
				{
					if (fabs(rk_moles[j] - kinetics_ptr->comps[j].moles) >
						kinetics_ptr->comps[j].tol)
					{
						equal_rate = FALSE;
						break;
					}
				}
			}
			if (zero_rate || equal_rate)
			{
				/* removing the following line causes different results for 
				   example 6 distributed with the program */
				saver();

				/*  Free space */

				if (pp_assemblage_save != NULL)
				{
					delete pp_assemblage_save;
					pp_assemblage_save = NULL;
				}
				if (ss_assemblage_save != NULL)
				{
					ss_assemblage_free(ss_assemblage_save);
				}
				goto EQUAL_RATE_OUT;
			}
			else
			{
				kinetics_ptr->rk = 3;
				for (j = 0; j < n_reactions; j++)
				{
					kinetics_ptr->comps[j].moles = 0.2 * rk_moles[j];
				}
			}
		}
/*
 * Continue with rk ...
 */
		calc_final_kinetic_reaction(kinetics_ptr);
		if (set_and_run_wrapper(i, NOMIX, TRUE, i, 0.) == MASS_BALANCE)
		{
			run_reactions_iterations += iterations;
			moles_reduction = 9;
			goto MOLES_TOO_LARGE;
		}
		run_reactions_iterations += iterations;

/*
 *   find k2
 */
		for (j = 0; j < n_reactions; j++)
		{
			kinetics_ptr->comps[j].m =
				m_temp[j] - kinetics_ptr->comps[j].moles;
			kinetics_ptr->comps[j].moles = 0.;
		}
		rate_sim_time = rate_sim_time_start + h_sum + 0.2 * h;
		calc_kinetic_reaction(kinetics_ptr, h);

		/*   Reset to values of last saver() */
		if (pp_assemblage_save != NULL)
		{
			Rxn_pp_assemblage_map[pp_assemblage_save->Get_n_user()] = *pp_assemblage_save;
			use.Set_pp_assemblage_ptr(Utilities::Rxn_find(Rxn_pp_assemblage_map, pp_assemblage_save->Get_n_user()));
		}
		if (ss_assemblage_save != NULL)
		{
			ss_assemblage_free(use.Get_ss_assemblage_ptr());
			ss_assemblage_copy(ss_assemblage_save, use.Get_ss_assemblage_ptr(),
								ss_assemblage_save->n_user);
		}

		/* store k2 in rk_moles */
		k = n_reactions;
		for (j = 0; j < n_reactions; j++)
		{
			if (moles_reduction * moles_max <
				fabs(kinetics_ptr->comps[j].moles))
			{
				moles_reduction =
					fabs(kinetics_ptr->comps[j].moles) / moles_max;
			}
			/*  define reaction for calculating k3 */
			rk_moles[k + j] = kinetics_ptr->comps[j].moles;

			kinetics_ptr->comps[j].moles = b31 * rk_moles[j]
				+ b32 * rk_moles[k + j];
/*
 * check for equal_rate ...
 */
			if (equal_rate
				&& fabs(rk_moles[j] - rk_moles[k + j]) >
				kinetics_ptr->comps[j].tol)
			{
				equal_rate = FALSE;
			}
		}
		if (moles_reduction > 1.0)
			goto MOLES_TOO_LARGE;
/*
 * Quit rk with rk = 2 and equal rates ...
 */
		if (kinetics_ptr->rk == 2 && equal_rate)
		{
			for (j = 0; j < n_reactions; j++)
			{
				kinetics_ptr->comps[j].moles =
					0.3 * rk_moles[j] + 0.7 * rk_moles[k + j];
			}
			calc_final_kinetic_reaction(kinetics_ptr);
			for (j = 0; j < n_reactions; j++)
			{
				kinetics_ptr->comps[j].m =
					m_temp[j] - kinetics_ptr->comps[j].moles;
				if (kinetics_ptr->comps[j].m < 1.e-30)
					kinetics_ptr->comps[j].m = 0;
				kinetics_ptr->comps[j].moles = 0.;
			}
			if (set_and_run_wrapper(i, NOMIX, TRUE, i, 0.) == MASS_BALANCE)
			{
				run_reactions_iterations += iterations;
				moles_reduction = 9;
				goto MOLES_TOO_LARGE;
			}
			run_reactions_iterations += iterations;
/*
 * Move next calc'n to rk = 1 when initial rate equals final rate ...
 */
			calc_kinetic_reaction(kinetics_ptr, h);
			for (j = 0; j < n_reactions; j++)
			{
				if (fabs(rk_moles[j] - kinetics_ptr->comps[j].moles) >
					kinetics_ptr->comps[j].tol)
				{
					equal_rate = FALSE;
					break;
				}
			}
			if (equal_rate)
				kinetics_ptr->rk = 1;

			saver();

			/*  Free space */

			if (pp_assemblage_save != NULL)
			{
				delete pp_assemblage_save;
				pp_assemblage_save = NULL;
			}
			if (ss_assemblage_save != NULL)
			{
				ss_assemblage_free(ss_assemblage_save);
			}
			goto EQUAL_RATE_OUT;
		}
/*
 * Continue runge_kutta..
 */
		calc_final_kinetic_reaction(kinetics_ptr);
		if (set_and_run_wrapper(i, NOMIX, TRUE, i, 0.) == MASS_BALANCE)
		{
			run_reactions_iterations += iterations;
			moles_reduction = 9;
			goto MOLES_TOO_LARGE;
		}
		run_reactions_iterations += iterations;
/*
 *   find k3
 */
		for (j = 0; j < n_reactions; j++)
		{
			kinetics_ptr->comps[j].m =
				m_temp[j] - kinetics_ptr->comps[j].moles;
			kinetics_ptr->comps[j].moles = 0.;
		}
		rate_sim_time = rate_sim_time_start + h_sum + 0.3 * h;
		calc_kinetic_reaction(kinetics_ptr, h);

		/*   Reset to values of last saver() */
		if (pp_assemblage_save != NULL)
		{
			Rxn_pp_assemblage_map[pp_assemblage_save->Get_n_user()] = *pp_assemblage_save;
			use.Set_pp_assemblage_ptr(Utilities::Rxn_find(Rxn_pp_assemblage_map, pp_assemblage_save->Get_n_user()));
		}
		if (ss_assemblage_save != NULL)
		{
			ss_assemblage_free(use.Get_ss_assemblage_ptr());
			ss_assemblage_copy(ss_assemblage_save, use.Get_ss_assemblage_ptr(),
								ss_assemblage_save->n_user);
		}

		/* store k3 in rk_moles */
		k = 2 * n_reactions;
		for (j = 0; j < n_reactions; j++)
		{
			if (moles_reduction * moles_max <
				fabs(kinetics_ptr->comps[j].moles))
			{
				moles_reduction =
					fabs(kinetics_ptr->comps[j].moles) / moles_max;
			}
			/*  define reaction for calculating k4 ... */
			rk_moles[k + j] = kinetics_ptr->comps[j].moles;

			kinetics_ptr->comps[j].moles = 0.3 * rk_moles[j]
				- 0.9 * rk_moles[n_reactions + j] + 1.2 * rk_moles[k + j];
/*
 * check for equal_rate ...
 */
			if (equal_rate
				&& fabs(rk_moles[j] - rk_moles[k + j]) >
				kinetics_ptr->comps[j].tol)
				equal_rate = FALSE;
		}
		if (moles_reduction > 1.0)
			goto MOLES_TOO_LARGE;
/*
 * Quit rk with rk = 3 and equal rates ...
 */
		if (kinetics_ptr->rk == 3 && equal_rate)
		{
			for (j = 0; j < n_reactions; j++)
			{
				kinetics_ptr->comps[j].moles = 0.5 * rk_moles[j]
					- 1.5 * rk_moles[n_reactions + j] + 2 * rk_moles[k + j];
			}
			calc_final_kinetic_reaction(kinetics_ptr);
			for (j = 0; j < n_reactions; j++)
			{
				kinetics_ptr->comps[j].m =
					m_temp[j] - kinetics_ptr->comps[j].moles;
				if (kinetics_ptr->comps[j].m < 1.e-30)
					kinetics_ptr->comps[j].m = 0;
				kinetics_ptr->comps[j].moles = 0.;
			}

			if (set_and_run_wrapper(i, NOMIX, TRUE, i, 0.) == MASS_BALANCE)
			{
				run_reactions_iterations += iterations;
				moles_reduction = 9;
				goto MOLES_TOO_LARGE;
			}
			run_reactions_iterations += iterations;
/*
 * Move next calc'n to rk = 1 when initial rate equals final rate ...
 */
			calc_kinetic_reaction(kinetics_ptr, h);
			for (j = 0; j < n_reactions; j++)
			{
				if (fabs(rk_moles[j] - kinetics_ptr->comps[j].moles) >
					kinetics_ptr->comps[j].tol)
				{
					equal_rate = FALSE;
					break;
				}
			}
			if (equal_rate)
				kinetics_ptr->rk = 1;

			saver();

			/*  Free space */

			if (pp_assemblage_save != NULL)
			{
				delete pp_assemblage_save;
				pp_assemblage_save = NULL;
			}
			if (ss_assemblage_save != NULL)
			{
				ss_assemblage_free(ss_assemblage_save);
			}
			goto EQUAL_RATE_OUT;
		}
/*
 * Continue runge_kutta..
 */

		calc_final_kinetic_reaction(kinetics_ptr);
		if (set_and_run_wrapper(i, NOMIX, TRUE, i, 0.) == MASS_BALANCE)
		{
			run_reactions_iterations += iterations;
			moles_reduction = 9;
			goto MOLES_TOO_LARGE;
		}
		run_reactions_iterations += iterations;
/*
 *   find k4
 */
		for (j = 0; j < n_reactions; j++)
		{
			kinetics_ptr->comps[j].m =
				m_temp[j] - kinetics_ptr->comps[j].moles;
			kinetics_ptr->comps[j].moles = 0.;
		}
		rate_sim_time = rate_sim_time_start + h_sum + 0.6 * h;
		calc_kinetic_reaction(kinetics_ptr, h);

		/*   Reset to values of last saver() */
		if (pp_assemblage_save != NULL)
		{
			Rxn_pp_assemblage_map[pp_assemblage_save->Get_n_user()] = *pp_assemblage_save;
			use.Set_pp_assemblage_ptr(Utilities::Rxn_find(Rxn_pp_assemblage_map, pp_assemblage_save->Get_n_user()));
		}
		if (ss_assemblage_save != NULL)
		{
			ss_assemblage_free(use.Get_ss_assemblage_ptr());
			ss_assemblage_copy(ss_assemblage_save, use.Get_ss_assemblage_ptr(),
								ss_assemblage_save->n_user);
		}

		/* store k4 in rk_moles */
		k = 3 * n_reactions;
		for (j = 0; j < n_reactions; j++)
		{
			if (moles_reduction * moles_max <
				fabs(kinetics_ptr->comps[j].moles))
			{
				moles_reduction =
					fabs(kinetics_ptr->comps[j].moles) / moles_max;
			}

			/*  define reaction for calculating k5 */
			rk_moles[k + j] = kinetics_ptr->comps[j].moles;
			kinetics_ptr->comps[j].moles = b51 * rk_moles[j]
				+ 2.5 * rk_moles[n_reactions + j]
				+ b53 * rk_moles[2 * n_reactions + j] + b54 * rk_moles[k + j];
		}
		if (moles_reduction > 1.0)
			goto MOLES_TOO_LARGE;
		calc_final_kinetic_reaction(kinetics_ptr);
		if (set_and_run_wrapper(i, NOMIX, TRUE, i, 0.) == MASS_BALANCE)
		{
			run_reactions_iterations += iterations;
			moles_reduction = 9;
			goto MOLES_TOO_LARGE;
		}
		run_reactions_iterations += iterations;
/*
 *   find k5
 */
		for (j = 0; j < n_reactions; j++)
		{
			kinetics_ptr->comps[j].m =
				m_temp[j] - kinetics_ptr->comps[j].moles;
			kinetics_ptr->comps[j].moles = 0.;
		}
		rate_sim_time = rate_sim_time_start + h_sum + h;
		calc_kinetic_reaction(kinetics_ptr, h);

		/*   Reset to values of last saver() */
		if (pp_assemblage_save != NULL)
		{
			Rxn_pp_assemblage_map[pp_assemblage_save->Get_n_user()] = *pp_assemblage_save;
			use.Set_pp_assemblage_ptr(Utilities::Rxn_find(Rxn_pp_assemblage_map, pp_assemblage_save->Get_n_user()));
		}
		if (ss_assemblage_save != NULL)
		{
			ss_assemblage_free(use.Get_ss_assemblage_ptr());
			ss_assemblage_copy(ss_assemblage_save, use.Get_ss_assemblage_ptr(),
								ss_assemblage_save->n_user);
		}

		/* store k5 in rk_moles */
		k = 4 * n_reactions;
		for (j = 0; j < n_reactions; j++)
		{
			if (moles_reduction * moles_max <
				fabs(kinetics_ptr->comps[j].moles))
			{
				moles_reduction =
					fabs(kinetics_ptr->comps[j].moles) / moles_max;
			}

			/*  define reaction for calculating k6 */
			rk_moles[k + j] = kinetics_ptr->comps[j].moles;
			kinetics_ptr->comps[j].moles = b61 * rk_moles[j]
				+ b62 * rk_moles[n_reactions + j]
				+ b63 * rk_moles[2 * n_reactions + j]
				+ b64 * rk_moles[3 * n_reactions + j] + b65 * rk_moles[k + j];
		}
		if (moles_reduction > 1.0)
			goto MOLES_TOO_LARGE;
		calc_final_kinetic_reaction(kinetics_ptr);
		if (set_and_run_wrapper(i, NOMIX, TRUE, i, 0.) == MASS_BALANCE)
		{
			run_reactions_iterations += iterations;
			moles_reduction = 9;
			goto MOLES_TOO_LARGE;
		}
		run_reactions_iterations += iterations;
/*
 *   find k6
 */
		for (j = 0; j < n_reactions; j++)
		{
			kinetics_ptr->comps[j].m =
				m_temp[j] - kinetics_ptr->comps[j].moles;
			kinetics_ptr->comps[j].moles = 0.;
		}
		rate_sim_time = rate_sim_time_start + h_sum + 0.875 * h;
		calc_kinetic_reaction(kinetics_ptr, h);

		/*   Reset to values of last saver() */
		if (pp_assemblage_save != NULL)
		{
			Rxn_pp_assemblage_map[pp_assemblage_save->Get_n_user()] = *pp_assemblage_save;
			use.Set_pp_assemblage_ptr(Utilities::Rxn_find(Rxn_pp_assemblage_map, pp_assemblage_save->Get_n_user()));
		}
		if (ss_assemblage_save != NULL)
		{
			ss_assemblage_free(use.Get_ss_assemblage_ptr());
			ss_assemblage_copy(ss_assemblage_save, use.Get_ss_assemblage_ptr(),
								ss_assemblage_save->n_user);
		}

		/* store k6 in rk_moles */
		k = 5 * n_reactions;
		for (j = 0; j < n_reactions; j++)
		{
			rk_moles[k + j] = kinetics_ptr->comps[j].moles;
		}

/*
 *   Evaluate error
 */
		error_max = 0.;
		for (j = 0; j < n_reactions; j++)
		{
			l_error = fabs(dc1 * rk_moles[j]
						 + dc3 * rk_moles[2 * n_reactions + j]
						 + dc4 * rk_moles[3 * n_reactions + j]
						 + dc5 * rk_moles[4 * n_reactions + j]
						 + dc6 * rk_moles[5 * n_reactions + j]);

			/* tol is in moles/l */
			l_error /= kinetics_ptr->comps[j].tol;
			if (l_error > error_max)
				error_max = l_error;
		}

/*
 *   repeat with smaller step
 */
/* printf("timest %g ; error_max %g\n", h, error_max); */
		if (error_max > 1)
		{
			h_old = h;
			if (step_ok == 0)
				h = h * safety / error_max;
			else
				h = h * safety * pow(error_max, (LDBLE) -0.25);
			l_bad = TRUE;
			step_bad++;
		}
		else
		{
/*
 *   OK, calculate result
 */
			for (j = 0; j < n_reactions; j++)
			{
				kinetics_ptr->comps[j].moles = c1 * rk_moles[j]
					+ c3 * rk_moles[2 * n_reactions + j]
					+ c4 * rk_moles[3 * n_reactions + j]
					+ c6 * rk_moles[5 * n_reactions + j];
			}
			calc_final_kinetic_reaction(kinetics_ptr);
			for (j = 0; j < n_reactions; j++)
			{
				kinetics_ptr->comps[j].m =
					m_temp[j] - kinetics_ptr->comps[j].moles;
				if (kinetics_ptr->comps[j].m < 1.e-30)
					kinetics_ptr->comps[j].m = 0;
				kinetics_ptr->comps[j].moles = 0.;
			}

			if (set_and_run_wrapper(i, NOMIX, TRUE, i, 0.) == MASS_BALANCE)
			{
				run_reactions_iterations += iterations;
				moles_reduction = 9;
				goto MOLES_TOO_LARGE;
			}
			run_reactions_iterations += iterations;
/*
 * Move next calc'n to rk = 1 when initial rate equals final rate ...
 */
			calc_kinetic_reaction(kinetics_ptr, h);
			for (j = 0; j < n_reactions; j++)
			{
				if (fabs(rk_moles[j] - kinetics_ptr->comps[j].moles) >
					kinetics_ptr->comps[j].tol)
				{
					equal_rate = FALSE;
					break;
				}
			}
			if (equal_rate && kinetics_ptr->rk < 6)
				kinetics_ptr->rk = 1;

			saver();

			step_ok++;
			h_sum += h;
			/*  Free space */

			if (pp_assemblage_save != NULL)
			{
				delete pp_assemblage_save;
				pp_assemblage_save = NULL;
			}
			if (ss_assemblage_save != NULL)
			{
				ss_assemblage_free(ss_assemblage_save);
			}
/*
 *   and increase step size ...
 */
			if (h_sum < kin_time)
			{
				if (error_max > 0.000577)
				{
					h = h * safety * pow(error_max, (LDBLE) -0.2e0);
				}
				else
				{
					h *= 4;
				}
				if (h > (kin_time - h_sum))
					h = (kin_time - h_sum);
			}
		}
#if !defined(PHREEQCI_GUI)
#ifndef PHREEQ98
		if (pr.status == TRUE && status_on == TRUE
			&& (int) (1e3 / CLOCKS_PER_SEC * ((float) clock() - status_timer)) > status_interval)
		{
			char str[MAX_LENGTH];
			backspace_screen(37);
			sprintf(str, "RK-steps: Bad%4d. OK%5d. Time %3d%%", step_bad,
					step_ok, (int) (100 * h_sum / kin_time));
			screen_msg(sformatf("%-37s", str));
		}
#endif
#endif
	}

  EQUAL_RATE_OUT:

/*
 *   Run one more time to get distribution of species
 */
	if (state >= REACTION || nsaver != i)
	{
		set_and_run_wrapper(i, NOMIX, FALSE, nsaver, 0.);
		run_reactions_iterations += iterations;
	}
/*	saver();  */ /* reset for printing */
	if (use_mix == DISP)
	{
		//use.Get_mix_ptr() = &mix[count_mix - count_cells + i - 1];
		use.Set_mix_ptr(Utilities::Rxn_find(Dispersion_mix_map, i));
		use.Set_mix_in(true);
		use.Set_n_mix_user(i);
	}
	else if ((use_mix == STAG || use_mix == TRUE) && state == TRANSPORT)
	{
		//use.Get_mix_ptr() = mix_search(i, &use.n_mix, FALSE);
		use.Set_mix_ptr(Utilities::Rxn_find(Rxn_mix_map, i));
		if (use.Get_mix_ptr() != NULL)
		{
			use.Set_mix_in(true);
			use.Set_n_mix_user(i);
		}
	}
/*
 *  Restore solution i, if necessary
 */
	if (nsaver != i)
	{
		solution_duplicate(save_old, i);
	}
	rk_moles = (LDBLE *) free_check_null(rk_moles);

	rate_sim_time = rate_sim_time_start + kin_time;
	use.Set_kinetics_in(true);

	/*  Free space */

	if (pp_assemblage_save != NULL)
	{
		delete pp_assemblage_save;
		pp_assemblage_save = NULL;
	}
	if (ss_assemblage_save != NULL)
	{
		ss_assemblage_save =
			(struct ss_assemblage *) free_check_null(ss_assemblage_save);
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
set_and_run_wrapper(int i, int use_mix, int use_kinetics, int nsaver,
					LDBLE step_fraction)
/* ---------------------------------------------------------------------- */
{
	int j, converge, max_try;
	int old_diag, old_itmax;
	LDBLE old_tol, old_min_value, old_step, old_pe, old_pp_column_scale;
	LDBLE small_pe_step, small_step;
	cxxPPassemblage *pp_assemblage_save = NULL;
	struct ss_assemblage *ss_assemblage_save = NULL;
	struct kinetics *kinetics_save = NULL;

	
	small_pe_step = 5.;
	small_step = 10.;
	converge = FALSE;

	old_diag = diagonal_scale;
	old_itmax = itmax;
	old_tol = ineq_tol;
	old_step = step_size;
	old_pe = pe_step_size;
	old_min_value = min_value;
	old_pp_column_scale = pp_column_scale;

	if (state == TRANSPORT || state == PHAST)
	{
		set_transport(i, use_mix, use_kinetics, i);
	}
	else if (state == ADVECTION)
	{
		set_advection(i, use_mix, use_kinetics, i);
	}
	else if (state == REACTION)
	{
		set_reaction(i, use_mix, use_kinetics);
	}
	if (use.Get_pp_assemblage_ptr() != NULL)
	{
		cxxPPassemblage * pp_assemblage_ptr = (cxxPPassemblage *) use.Get_pp_assemblage_ptr();
		if (pp_assemblage_save)
		{
			delete pp_assemblage_save;
			pp_assemblage_save = NULL;
		}
		pp_assemblage_save = new cxxPPassemblage(*pp_assemblage_ptr);
	}
	if (use.Get_ss_assemblage_ptr() != NULL)
	{
		ss_assemblage_save =
			(struct ss_assemblage *)
			PHRQ_malloc(sizeof(struct ss_assemblage));
		if (ss_assemblage_save == NULL)
			malloc_error();
		ss_assemblage_copy(use.Get_ss_assemblage_ptr(), ss_assemblage_save,
							use.Get_ss_assemblage_ptr()->n_user);
	}
	if (use.Get_kinetics_ptr() != NULL)
	{
		kinetics_save =
			(struct kinetics *) PHRQ_malloc(sizeof(struct kinetics));
		if (kinetics_save == NULL)
			malloc_error();
		kinetics_copy(use.Get_kinetics_ptr(), kinetics_save,
					  use.Get_kinetics_ptr()->n_user);
	}

	if (pitzer_model == TRUE || sit_model == TRUE)
	{
		diagonal_scale = TRUE;
		always_full_pitzer = FALSE;
		max_try = 13;
	}
	else
	{
		max_try = 13;
	}
	max_try = (max_tries < max_try) ? max_tries : max_try;
	/*max_try = 1; */
	for (j = 0; j < max_try; j++)
	{
		if (j == 1)
		{
			always_full_pitzer = TRUE;
			if (pe_step_size <= small_pe_step && step_size <= small_step)
				continue;
			itmax *= 2;
			step_size = small_step;
			pe_step_size = small_pe_step;
			error_string = sformatf(
					"Trying smaller step size, pe step size %g, %g ... \n",
					(double) step_size, (double) pe_step_size);
			warning_msg(error_string);
		}
		else if (j == 2)
		{
			itmax *= 2;
			ineq_tol /= 10.;
			error_string = sformatf( "Trying reduced tolerance %g ...\n",
					(double) ineq_tol);
			warning_msg(error_string);
		}
		else if (j == 3)
		{
			itmax *= 2;
			ineq_tol *= 10.;
			error_string = sformatf( "Trying increased tolerance %g ...\n",
					(double) ineq_tol);
			warning_msg(error_string);
		}
		else if (j == 4)
		{
			itmax *= 2;
			if (diagonal_scale == TRUE)
			{
				diagonal_scale = FALSE;
			}
			else
			{
				diagonal_scale = TRUE;
			}
			error_string = sformatf( "Trying diagonal scaling ...\n");
			warning_msg(error_string);
		}
		else if (j == 5)
		{
			itmax *= 2;
			if (diagonal_scale == TRUE)
			{
				diagonal_scale = FALSE;
			}
			else
			{
				diagonal_scale = TRUE;
			}
			ineq_tol /= 10.;
			error_string = sformatf(
					"Trying diagonal scaling and reduced tolerance %g ...\n",
					(double) ineq_tol);
			warning_msg(error_string);
		}
		else if (j == 6)
		{
			if (pitzer_model == TRUE || sit_model == TRUE) continue;
			itmax *= 2;
			pp_column_scale = 1e-10;
			error_string = sformatf(
					"Trying scaling pure_phase columns %g ...\n",
					(double) pp_column_scale);
			warning_msg(error_string);
		}
		else if (j == 7)
		{
			if (pitzer_model == TRUE || sit_model == TRUE) continue;
			itmax *= 2;
			pp_column_scale = 1e-10;
			if (diagonal_scale == TRUE)
			{
				diagonal_scale = FALSE;
			}
			else
			{
				diagonal_scale = TRUE;
			}
			error_string = sformatf(
					"Trying scaling pure_phase columns and diagonal scale %g ...\n",
					(double) pp_column_scale);
			warning_msg(error_string);
		}
		else if (j == 8)
		{
			if (pitzer_model == TRUE || sit_model == TRUE) continue;
			itmax *= 2;
			min_value *= 10;
			error_string = sformatf( "Trying increased scaling %g ...\n",
					(double) min_value);
			warning_msg(error_string);
		}
		else if (j == 9)
		{
			if (pitzer_model == TRUE || sit_model == TRUE) continue;
			aqueous_only = 5;
			error_string = sformatf(
					"Skipping optimize equations for first %d iterations ...\n",
					aqueous_only);
			warning_msg(error_string);
		}
		else if (j == 10)
		{
			if (pitzer_model == TRUE || sit_model == TRUE) continue;
			negative_concentrations = TRUE;
			error_string = sformatf(
					"Adding inequality to make concentrations greater than zero.\n");
			warning_msg(error_string);
		}
		else if (j == 11)
		{
			itmax *= 2;
			ineq_tol /= 100.;
			error_string = sformatf( "Trying reduced tolerance %g ...\n",
					(double) ineq_tol);
			warning_msg(error_string);
		}
		else if (j == 12)
		{
			itmax *= 2;
			ineq_tol /= 1000.;
			error_string = sformatf( "Trying reduced tolerance %g ...\n",
					(double) ineq_tol);
			warning_msg(error_string);
		}
		if (j > 0)
		{
			if (pp_assemblage_save != NULL)
			{
				Rxn_pp_assemblage_map[pp_assemblage_save->Get_n_user()] = *pp_assemblage_save;
				use.Set_pp_assemblage_ptr(Utilities::Rxn_find(Rxn_pp_assemblage_map, pp_assemblage_save->Get_n_user()));
			}
			if (ss_assemblage_save != NULL)
			{
				ss_assemblage_free(use.Get_ss_assemblage_ptr());
				ss_assemblage_copy(ss_assemblage_save,
									use.Get_ss_assemblage_ptr(),
									ss_assemblage_save->n_user);
			}
			if (kinetics_save != NULL)
			{
				kinetics_free(use.Get_kinetics_ptr());
				kinetics_copy(kinetics_save, use.Get_kinetics_ptr(),
							  kinetics_save->n_user);
			}
		}
		set_and_run_attempt = j;

		converge =
			set_and_run(i, use_mix, use_kinetics, nsaver, step_fraction);
		/* reset values */
		diagonal_scale = old_diag;
		itmax = old_itmax;
		ineq_tol = old_tol;
		step_size = old_step;
		pe_step_size = old_pe;
		min_value = old_min_value;
		pp_column_scale = old_pp_column_scale;
		aqueous_only = 0;
		negative_concentrations = FALSE;
		if (converge == TRUE)
		{
			break;
		}
		else if (converge == MASS_BALANCE)
		{
			break;
		}
		warning_msg
			("Numerical method failed with this set of convergence parameters.\n");
	}
	if (converge == FALSE && use.Get_kinetics_ptr() != NULL
		&& use.Get_kinetics_ptr()->use_cvode == TRUE)
	{
		error_string = sformatf(
				"Numerical method failed on all parameter combinations, retrying integration");
		warning_msg(error_string);
		converge = MASS_BALANCE;
	}
	if (converge == FALSE)
	{
/*
 *   write to error.inp what failed to converge.
 */
		std::ofstream error_input("error.inp");
		//cxxStorageBin error_bin(PHREEQC_THIS_COMMA &use, phrq_io);
		cxxStorageBin error_bin;
		Use2cxxStorageBin(error_bin);
		error_bin.dump_raw(error_input, 0);
		error_input.close();

		/* if (state == TRANSPORT && dump_modulus == 0) dump(); */
		check_residuals();
		pr.all = TRUE;
		pr.gas_phase = use.Get_gas_phase_in();
		pr.pp_assemblage = use.Get_pp_assemblage_in();
		pr.ss_assemblage = use.Get_ss_assemblage_in();
		pr.surface = use.Get_surface_in();
		pr.exchange = use.Get_exchange_in();
		pr.totals = TRUE;
		pr.species = TRUE;
		pr.saturation_indices = TRUE;
		pr.irrev = use.Get_reaction_in();
		pr.mix = use.Get_mix_in();
		pr.reaction = TRUE;
		pr.use = TRUE;
		sum_species();
		print_all();
		error_string = sformatf(
				"Numerical method failed on all combinations of convergence parameters");
		error_msg(error_string, STOP);
	}
	if (pp_assemblage_save != NULL)
	{
		delete pp_assemblage_save;
		pp_assemblage_save = NULL;
	}
	if (ss_assemblage_save != NULL)
	{
		ss_assemblage_free(ss_assemblage_save);
		ss_assemblage_save =
			(struct ss_assemblage *) free_check_null(ss_assemblage_save);
	}
	if (kinetics_save != NULL)
	{
		kinetics_free(kinetics_save);
		kinetics_save = (struct kinetics *) free_check_null(kinetics_save);
	}
	if (converge == MASS_BALANCE)
	{
		return (MASS_BALANCE);
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
set_and_run(int i, int use_mix, int use_kinetics, int nsaver,
			LDBLE step_fraction)
/* ---------------------------------------------------------------------- */
{
/*
 *   i			--user number for soln, reaction, etc.
 *   use_mix	--integer flag
					state == TRANSPORT: DISP, STAG, NOMIX
					state == REACTION: TRUE, FALSE
 *   use_kinetics --true or false flag to calculate kinetic reactions
 *   nsaver	   --user number to store solution
 *   step_fraction--fraction of irreversible reaction to add
 */
	int n,n2, converge;
	if (state == TRANSPORT || state == PHAST)
	{
		set_transport(i, use_mix, use_kinetics, nsaver);
	}
	else if (state == ADVECTION)
	{
		set_advection(i, use_mix, use_kinetics, nsaver);
	}
	else if (state == REACTION)
	{
		set_reaction(i, use_mix, use_kinetics);
	}
	cell = i;
/*
 *   Take step
 */
	if (state >= REACTION)
	{
		if (step(step_fraction) == MASS_BALANCE)
		{
			return (MASS_BALANCE);
		}
/*
 *  Always use solution, exchange, and surface -1
 */
		use.Set_solution_ptr(solution_bsearch(-1, &n, TRUE));

		/* new */
		if (use.Get_exchange_ptr() != NULL)
		{
			//use.Get_exchange_ptr() = exchange_bsearch(-1, &n1);
			use.Set_exchange_ptr(Utilities::Rxn_find(Rxn_exchange_map, -1));
		}
		if (use.Get_surface_ptr() != NULL)
		{
			use.Set_surface_ptr(surface_bsearch(-1, &n2));
		}

/*
 *  Adjust the total pressure to the gas pressure
 */
		if (use.Get_gas_phase_ptr() != NULL)
		{
			cxxGasPhase *gas_phase_ptr = (cxxGasPhase *) use.Get_gas_phase_ptr();
			if (gas_phase_ptr->Get_type() == cxxGasPhase::GP_PRESSURE) 
			{
			/*
			 * Fixed-pressure Gas phase and solution will react 
			 * Change total pressure of current simulation to pressure 
			 * of gas phase
			 */
				patm_x = gas_phase_ptr->Get_total_p();
			}
			/*  fixed volume gas phase is handled in calc_gas_pressures */

		}
	}
	/* end new */
	if (use.Get_surface_ptr() != NULL)
	{
		dl_type_x = use.Get_surface_ptr()->dl_type;
	}
	if (use.Get_surface_ptr() != NULL && dl_type_x != NO_DL)
	{
		converge = surface_model();
	}
	else
	{
		prep();
		k_temp(use.Get_solution_ptr()->tc, use.Get_solution_ptr()->patm);
		set(FALSE);
		converge = model();
	}
	sum_species();
	return (converge);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
set_transport(int i, int use_mix, int use_kinetics, int nsaver)
/* ---------------------------------------------------------------------- */
{
/*
 *   i			--user number for soln, reaction, etc.
 *   use_mix	  --integer flag
					state == TRANSPORT: DISP, STAG, NOMIX
					state == REACTION: TRUE, FALSE
 *   use_kinetics --true or false flag to calculate kinetic reactions
 *   nsaver	   --user number to store solution
 */
	int n;

	cell = i;
	reaction_step = 1;
/*
 *   Find mixture or solution
 */

	use.Set_mix_ptr(NULL);
	use.Set_mix_in(false);
	if (use_mix == DISP)
	{
		//use.Get_mix_ptr() = &mix[count_mix - count_cells + i - 1];
		use.Set_mix_ptr(Utilities::Rxn_find(Dispersion_mix_map, i));
		use.Set_mix_in(true);
		use.Set_n_mix_user(i);
		use.Set_n_mix_user_orig(i);
	}
	else if (use_mix == STAG && multi_Dflag != TRUE)
	{
		//use.Get_mix_ptr() = mix_search(i, &use.n_mix, FALSE);
		use.Set_mix_ptr(Utilities::Rxn_find(Rxn_mix_map, i));
		if (use.Get_mix_ptr() != NULL)
		{
			use.Set_mix_in(true);
			use.Set_n_mix_user(i);
			use.Set_n_mix_user_orig(i);
		}
		else
		{
			int n_solution;
			use.Set_solution_ptr(solution_bsearch(i, &n_solution, FALSE));
			//use.Set_n_solution(n_solution);
			if (use.Get_solution_ptr() == NULL)
			{
				error_string = sformatf( "Solution %d not found.",
						use.Get_n_solution_user());
				error_msg(error_string, STOP);
			}
			use.Set_n_solution_user(i);
			use.Set_solution_in(true);
		}
	}
	else
	{
		int n_solution;
		use.Set_solution_ptr(solution_bsearch(i, &n_solution, FALSE));
		//use.Set_n_solution(n_solution);
		if (use.Get_solution_ptr() == NULL)
		{
			error_string = sformatf( "Solution %d not found.",
					use.Get_n_solution_user());
			error_msg(error_string, STOP);
		}
		use.Set_n_solution_user(i);
		use.Set_solution_in(true);
	}
	save.solution = TRUE;
	save.n_solution_user = nsaver;
	save.n_solution_user_end = nsaver;
/*
 *   Find pure phase assemblage
 */

	//use.Get_pp_assemblage_ptr() = pp_assemblage_bsearch(i, &use.n_pp_assemblage);
	use.Set_pp_assemblage_ptr(Utilities::Rxn_find(Rxn_pp_assemblage_map, i));
	if (use.Get_pp_assemblage_ptr() != NULL)
	{
		use.Set_pp_assemblage_in(true);
		use.Set_n_pp_assemblage_user(i);
		save.pp_assemblage = TRUE;
		save.n_pp_assemblage_user = i;
		save.n_pp_assemblage_user_end = i;
	}
	else
	{
		use.Set_pp_assemblage_in(false);
		save.pp_assemblage = FALSE;
	}
/*
 *   Find irreversible reaction
 */
	//use.irrev_ptr = irrev_bsearch(i, &use.n_irrev);
	use.Set_reaction_ptr(Utilities::Rxn_find(Rxn_reaction_map, i));
	if (use.Get_reaction_ptr() != NULL)
	{
		use.Set_reaction_in(true);
		use.Set_n_reaction_user(i);
	}
	else
	{
		use.Set_reaction_in(false);
	}
/*
 *   Find exchange
 */
	//use.Get_exchange_ptr() = exchange_bsearch(i, &use.n_exchange);
	use.Set_exchange_ptr(Utilities::Rxn_find(Rxn_exchange_map, i));
	if (use.Get_exchange_ptr() != NULL)
	{
		use.Set_exchange_in(true);
		use.Set_n_exchange_user(i);
		save.exchange = TRUE;
		save.n_exchange_user = i;
		save.n_exchange_user_end = i;
	}
	else
	{
		use.Set_exchange_in(false);
		save.exchange = FALSE;
	}

/*
 *   Find surface
 */
	int n_surface;
	use.Set_surface_ptr(surface_bsearch(i, &n_surface));
	//use.Set_n_surface(n_surface);
	if (use.Get_surface_ptr() != NULL)
	{
		use.Set_surface_in(true);
		use.Set_n_surface_user(i);
		save.surface = TRUE;
		save.n_surface_user = i;
		save.n_surface_user_end = i;
	}
	else
	{
		use.Set_surface_in(false);
		save.surface = FALSE;
		dl_type_x = NO_DL;
	}
/*
 *   Find temperature;  temp retardation is done in step
 */
	use.Set_temperature_ptr(Utilities::Rxn_find(Rxn_temperature_map, i));
	if (use.Get_temperature_ptr() != NULL)
	{
		use.Set_temperature_in(true);
		use.Set_n_temperature_user(i);
	}
	else
	{
		use.Set_temperature_in(false);
	}
/*
 *   Find pressure
 */
	use.Set_pressure_ptr(Utilities::Rxn_find(Rxn_pressure_map, i));
	if (use.Get_pressure_ptr() != NULL)
	{
		use.Set_pressure_in(true);
		use.Set_n_pressure_user(i);
	}
	else
	{
		use.Set_pressure_in(false);
	}
/*
 *   Find gas
 */
	//use.Get_gas_phase_ptr() = gas_phase_bsearch(i, &use.n_gas_phase);
	use.Set_gas_phase_ptr(Utilities::Rxn_find(Rxn_gas_phase_map, i));
	if (use.Get_gas_phase_ptr() != NULL)
	{
		use.Set_gas_phase_in(true);
		use.Set_n_gas_phase_user(i);
		save.gas_phase = TRUE;
		save.n_gas_phase_user = i;
		save.n_gas_phase_user_end = i;
	}
	else
	{
		use.Set_gas_phase_in(false);
		save.gas_phase = FALSE;
	}
/*
 *   Find ss_assemblage
 */
	int n_ss_assemblage;
	use.Set_ss_assemblage_ptr(ss_assemblage_bsearch(i, &n_ss_assemblage));
	//use.Set_n_ss_assemblage(n_ss_assemblage);
	if (use.Get_ss_assemblage_ptr() != NULL)
	{
		use.Set_ss_assemblage_in(true);
		use.Set_n_ss_assemblage_user(i);
		save.ss_assemblage = TRUE;
		save.n_ss_assemblage_user = i;
		save.n_ss_assemblage_user_end = i;
	}
	else
	{
		use.Set_ss_assemblage_in(false);
		save.ss_assemblage = FALSE;
	}
/*
 *   Find kinetics
 */
	use.Set_kinetics_ptr(NULL);
	use.Set_kinetics_in(false);
	save.kinetics = FALSE;
	if (use_kinetics == TRUE)
	{
		use.Set_kinetics_ptr(kinetics_bsearch(i, &n));
		if (use.Get_kinetics_ptr() != NULL)
		{
			use.Set_n_kinetics_user(i);
			//use.Set_n_kinetics(n);
			use.Set_kinetics_in(true);
			save.kinetics = TRUE;
			save.n_kinetics_user = i;
			save.n_kinetics_user_end = i;
		}
	}
	/*
	if (use.irrev_ptr != NULL && use.Get_kinetics_ptr() != NULL)
	{
		warning_msg("Should not use REACTION in same simulation with KINETICS.");
	}
	*/
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
set_reaction(int i, int use_mix, int use_kinetics)
/* ---------------------------------------------------------------------- */
{
/*
 *   i			--user number for soln, reaction, etc.
 *   use_mix	  --integer flag
					state == TRANSPORT: DISP, STAG, NOMIX
					state == REACTION: TRUE, FALSE
 *   use_kinetics --true or false flag to calculate kinetic reactions
 */
/*
 *   Find mixture or solution
 */
	use.Set_mix_ptr(NULL);
	use.Set_solution_ptr(NULL);
	if (use_mix == TRUE && use.Get_mix_in() == TRUE)
	{
		//use.Get_mix_ptr() = mix_bsearch(i, &use.n_mix);
		use.Set_mix_ptr(Utilities::Rxn_find(Rxn_mix_map, i));
		if (use.Get_mix_ptr() == NULL)
		{
			error_string = sformatf( "MIX %d not found.", i);
			error_msg(error_string, STOP);
		}
	}
	else
	{
		int n_solution;
		use.Set_solution_ptr(solution_bsearch(i, &n_solution, FALSE));
		//use.Set_n_solution(n_solution);
		if (use.Get_solution_ptr() == NULL)
		{
			error_string = sformatf( "Solution %d not found.", i);
			error_msg(error_string, STOP);
		}
	}
/*
 *   Find pure phase assemblage
 */
	if (use.Get_pp_assemblage_in() == TRUE)
	{
		use.Set_pp_assemblage_ptr(Utilities::Rxn_find(Rxn_pp_assemblage_map, i));
		if (use.Get_pp_assemblage_ptr() == NULL)
		{
			error_string = sformatf( "PP_ASSEMBLAGE %d not found.", i);
			error_msg(error_string, STOP);
		}
	}

/*
 *   Find irreversible reaction
 */
	if (use.Get_reaction_in() == TRUE)
	{
		//use.irrev_ptr = irrev_bsearch(i, &use.n_irrev);
		use.Set_reaction_ptr(Utilities::Rxn_find(Rxn_reaction_map, i));
		if (use.Get_reaction_ptr() == NULL)
		{
			error_string = sformatf( "REACTION %d not found.", i);
			error_msg(error_string, STOP);
		}
	}
/*
 *   Find exchange
 */
	if (use.Get_exchange_in() == TRUE)
	{
		//use.Get_exchange_ptr() = exchange_bsearch(i, &use.n_exchange);
		use.Set_exchange_ptr(Utilities::Rxn_find(Rxn_exchange_map, i));
		if (use.Get_exchange_ptr() == NULL)
		{
			error_string = sformatf( "EXCHANGE %d not found.", i);
			error_msg(error_string, STOP);
		}
	}
/*
 *   Find surface
 */
	dl_type_x = NO_DL;
	if (use.Get_surface_in() == TRUE)
	{
		int n_surface;
		use.Set_surface_ptr(surface_bsearch(i, &n_surface));
		//use.Set_n_surface(n_surface);
		if (use.Get_surface_ptr() == NULL)
		{
			error_string = sformatf( "SURFACE %d not found.", i);
			error_msg(error_string, STOP);
		}
	}
/*
 *   Find temperature;  temp retardation is done in step
 */
	if (use.Get_temperature_in() == TRUE)
	{
		use.Set_temperature_ptr(Utilities::Rxn_find(Rxn_temperature_map, i));
		if (use.Get_temperature_ptr() == NULL)
		{
			error_string = sformatf( "TEMPERATURE %d not found.", i);
			error_msg(error_string, STOP);
		}
	}
/*
 *   Find pressure
 */
	if (use.Get_pressure_in() == TRUE)
	{
		use.Set_pressure_ptr(Utilities::Rxn_find(Rxn_pressure_map, i));
		if (use.Get_pressure_ptr() == NULL)
		{
			error_string = sformatf( "PRESSURE %d not found.", i);
			error_msg(error_string, STOP);
		}
	}
/*
 *   Find gas
 */
	if (use.Get_gas_phase_in() == TRUE)
	{
		use.Set_gas_phase_ptr(Utilities::Rxn_find(Rxn_gas_phase_map, i));
		if (use.Get_gas_phase_ptr() == NULL)
		{
			error_string = sformatf( "GAS_PHASE %d not found.", i);
			error_msg(error_string, STOP);
		}
	}
/*
 *   Find ss_assemblage
 */
	if (use.Get_ss_assemblage_in() == TRUE)
	{
		int n_ss_assemblage;
		use.Set_ss_assemblage_ptr(ss_assemblage_bsearch(i, &n_ss_assemblage));
		//use.Set_n_ss_assemblage(n_ss_assemblage);
		if (use.Get_ss_assemblage_ptr() == NULL)
		{
			error_string = sformatf( "Solid-solution Assemblage %d not found.",
					i);
			error_msg(error_string, STOP);
		}
	}
/*
 *   Find kinetics
 */
	if (use_kinetics == TRUE && use.Get_kinetics_in() == TRUE)
	{
		int n_kinetics;
		use.Set_kinetics_ptr(kinetics_bsearch(i, &n_kinetics));
		//use.Set_n_kinetics(n_kinetics);
		if (use.Get_kinetics_ptr() == NULL)
		{
			error_string = sformatf( "KINETICS %d not found.", i);
			error_msg(error_string, STOP);
		}
	}
	else
	{
		use.Set_kinetics_ptr(NULL);
	}
	/*
	if (use.irrev_ptr != NULL && use.Get_kinetics_ptr() != NULL)
	{
		warning_msg("Should not use REACTION in same simulation with KINETICS.");
	}
	*/
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
run_reactions(int i, LDBLE kin_time, int use_mix, LDBLE step_fraction)
/* ---------------------------------------------------------------------- */
{
/*
 * Kinetics calculations
 * Rates and moles of each reaction are calculated in calc_kinetic_reaction
 * Total number of moles in reaction is stored in kinetics[i].totals
 */

	int j, n, converge, m_iter;
	int pr_all_save;
	int nsaver;
	struct kinetics *kinetics_ptr;
	cxxPPassemblage *pp_assemblage_ptr;
	struct ss_assemblage *ss_assemblage_ptr;
	cxxUse use_save;
	int save_old, m, n_reactions /*, nok, nbad */ ;

	/* CVODE definitions */
	realtype ropt[OPT_SIZE], reltol, t, tout, tout1, sum_t;
	long int iopt[OPT_SIZE];
	int flag;
/*
 *   Set nsaver
 */
	run_reactions_iterations = 0;
	kin_time_x = kin_time;
	nsaver = i;
	if (state == TRANSPORT || state == PHAST)
	{
		if (use_mix == DISP)
		{
			nsaver = -2;
		}
		else if (use_mix == STAG)
		{
			nsaver = -2 - i;
		}
	}
	if (state == ADVECTION)
	{
		nsaver = -2;
	}
/*
 * Check that reaction exists for this cell ..
 */
	if (kin_time <= 0 ||
		(state == REACTION && use.Get_kinetics_in() == FALSE) ||
		(state == TRANSPORT && kinetics_bsearch(i, &n) == NULL) ||
		(state == PHAST && kinetics_bsearch(i, &n) == NULL) ||
		(state == ADVECTION && kinetics_bsearch(i, &n) == NULL))
	{
		converge =
			set_and_run_wrapper(i, use_mix, FALSE, nsaver, step_fraction);
		if (converge == MASS_BALANCE)
			error_msg
				("Negative concentration in system. Stopping calculation.",
				 STOP);
		run_reactions_iterations += iterations;
	}
	else
	{
/*
 *   Save moles of kinetic reactants for printout...
 */
		kinetics_ptr = kinetics_bsearch(i, &n);

		m_temp =
			(LDBLE *) PHRQ_malloc((size_t) kinetics_ptr->count_comps *
								  sizeof(LDBLE));
		if (m_temp == NULL)
			malloc_error();

		m_original =
			(LDBLE *) PHRQ_malloc((size_t) kinetics_ptr->count_comps *
								  sizeof(LDBLE));
		if (m_original == NULL)
			malloc_error();

		for (j = 0; j < kinetics_ptr->count_comps; j++)
		{
			m_original[j] = kinetics_ptr->comps[j].m;
			m_temp[j] = kinetics_ptr->comps[j].m;
		}
/*
*   Start the loop for timestepping ...
 *   Use either Runge-Kutta-Fehlberg, or final result extrapolation
 */
		pr_all_save = pr.all;
		pr.all = FALSE;
/*
 *   This condition makes output equal for incremental_reactions TRUE/FALSE...
 *		(if (incremental_reactions == FALSE || reaction_step == 1)
 */
		store_get_equi_reactants(i, FALSE);
		if (kinetics_ptr->use_cvode == FALSE)
		{
/* in case dispersivity is not wanted..
			if (multi_Dflag)
				rk_kinetics(i, kin_time, NOMIX, nsaver, step_fraction);
			else
 */ rk_kinetics(i, kin_time, use_mix, nsaver, step_fraction);
		}
		else
		{
			save_old = -2 - (count_cells * (1 + stag_data->count_stag) + 2);
			if (nsaver != i)
			{
				solution_duplicate(i, save_old);
			}
			for (j = 0; j < OPT_SIZE; j++)
			{
				iopt[j] = 0;
				ropt[j] = 0;
			}

/*
 *	Do mix first
 */
			kinetics_ptr = kinetics_bsearch(i, &m);
			n_reactions = kinetics_ptr->count_comps;
			cvode_n_user = i;
			cvode_kinetics_ptr = (void *) kinetics_ptr;
			cvode_n_reactions = n_reactions;
			cvode_rate_sim_time_start = rate_sim_time_start;
			cvode_rate_sim_time = rate_sim_time;

			if (multi_Dflag)
				converge = set_and_run_wrapper(i, NOMIX, FALSE, i, 0.0);
			else
				converge = set_and_run_wrapper(i, use_mix, FALSE, i, 0.0);
			if (converge == MASS_BALANCE)
				error_msg
					("Negative concentration in system. Stopping calculation.",
					 STOP);
			saver();
			//pp_assemblage_ptr = pp_assemblage_bsearch(i, &n);
			pp_assemblage_ptr = Utilities::Rxn_find(Rxn_pp_assemblage_map, i);
			ss_assemblage_ptr = ss_assemblage_bsearch(i, &n);
			if (pp_assemblage_ptr != NULL)
			{
				cvode_pp_assemblage_save = new cxxPPassemblage(*pp_assemblage_ptr);
			}
			if (ss_assemblage_ptr != NULL)
			{
				cvode_ss_assemblage_save =
					(struct ss_assemblage *)
					PHRQ_malloc(sizeof(struct ss_assemblage));
				if (cvode_ss_assemblage_save == NULL)
					malloc_error();
				ss_assemblage_copy(ss_assemblage_ptr,
									cvode_ss_assemblage_save,
									ss_assemblage_ptr->n_user);
			}

			/* allocate space for CVODE */
			kinetics_machEnv = M_EnvInit_Serial(n_reactions);
			kinetics_machEnv->phreeqc_ptr = this;
			kinetics_y = N_VNew(n_reactions, kinetics_machEnv);	/* Allocate y, abstol vectors */
			if (kinetics_y == NULL)
				malloc_error();
			cvode_last_good_y = N_VNew(n_reactions, kinetics_machEnv);	/* Allocate y, abstol vectors */
			if (cvode_last_good_y == NULL)
				malloc_error();
			cvode_prev_good_y = N_VNew(n_reactions, kinetics_machEnv);	/* Allocate y, abstol vectors */
			if (cvode_prev_good_y == NULL)
				malloc_error();
			kinetics_abstol = N_VNew(n_reactions, kinetics_machEnv);
			if (kinetics_abstol == NULL)
				malloc_error();

/*
 *	Set y to 0.0
 */
			for (j = 0; j < n_reactions; j++)
			{
				kinetics_ptr->comps[j].moles = 0.0;
				Ith(kinetics_y, j + 1) = 0.0;
				Ith(kinetics_abstol, j + 1) = kinetics_ptr->comps[j].tol;
				/*Ith(abstol,j+1) = 1e-8; */
				/* m_temp[j] = kinetics_ptr->comps[j].m; */
			}
			reltol = 0.0;

			/* Call CVodeMalloc to initialize CVODE:

			   NEQ   is the problem size = number of equations
			   f       is the user's right hand side function in y'=f(t,y)
			   T0     is the initial time
			   y       is the initial dependent variable vector
			   BDF   specifies the Backward Differentiation Formula
			   NEWTON  specifies a Newton iteration
			   SV     specifies scalar relative and vector absolute tolerances
			   &reltol is a pointer to the scalar relative tolerance
			   abstol  is the absolute tolerance vector
			   FALSE   indicates there are no optional inputs in iopt and ropt
			   iopt is an array used to communicate optional integer input and output
			   ropt is an array used to communicate optional real input and output

			   A pointer to CVODE problem memory is returned and stored in cvode_mem. */
			/* Don't know what this does */
			/*
			   iopt[SLDET] = TRUE;
			   cvode_mem = CVodeMalloc(n_reactions, f, 0.0, y, BDF, NEWTON, SV, &reltol, abstol, NULL, NULL, TRUE, iopt, ropt, machEnv);
			   cvode_mem = CVodeMalloc(n_reactions, f, 0.0, y, ADAMS, FUNCTIONAL, SV, &reltol, abstol, NULL, NULL, FALSE, iopt, ropt, machEnv);
			   iopt[MXSTEP] is maximum number of steps that CVODE tries.
			 */
			iopt[MXSTEP] = kinetics_ptr->cvode_steps;
			iopt[MAXORD] = kinetics_ptr->cvode_order;
			kinetics_cvode_mem =
				CVodeMalloc(n_reactions, f, 0.0, kinetics_y, BDF, NEWTON, SV,
							&reltol, kinetics_abstol, this, NULL, TRUE, iopt,
							ropt, kinetics_machEnv);
			if (kinetics_cvode_mem == NULL)
				malloc_error();

			/* Call CVDense to specify the CVODE dense linear solver with the
			   user-supplied Jacobian routine Jac. */
			flag = CVDense(kinetics_cvode_mem, Jac, this);
			if (flag != SUCCESS)
			{
				error_msg("CVDense failed.", STOP);
			}
			t = 0;
			tout = kin_time;
			/*ropt[HMAX] = tout/10.; */
			/*ropt[HMIN] = 1e-17; */
			use_save = use;
			flag = CVode(kinetics_cvode_mem, tout, kinetics_y, &t, NORMAL);
			rate_sim_time = rate_sim_time_start + t;
			/*
			   printf("At t = %0.4e   y =%14.6e  %14.6e  %14.6e\n",
			   t, Ith(y,1), Ith(y,2), Ith(y,3));
			 */
			m_iter = 0;
			sum_t = 0;
		  RESTART:
			while (flag != SUCCESS)
			{
				sum_t += cvode_last_good_time;
				error_string = sformatf(
						"CVode incomplete at cvode_steps %d. Cell: %d\tTime: %e\tCvode calls: %d, continuing...\n",
						(int) iopt[NST], cell_no, (double) sum_t, m_iter + 1);
				warning_msg(error_string);
#ifdef DEBUG_KINETICS
				if (m_iter > 5)
					dump_kinetics_stderr(cell_no);
#endif

				cvode_last_good_time = 0;
				if (++m_iter >= kinetics_ptr->bad_step_max)
				{
					m_temp = (LDBLE *) free_check_null(m_temp);
					m_original = (LDBLE *) free_check_null(m_original);
					error_msg("Repeated restart of integration.", STOP);
				}
				tout1 = tout - sum_t;
				t = 0;
				N_VScale(1.0, cvode_last_good_y, kinetics_y);
				for (j = 0; j < OPT_SIZE; j++)
				{
					iopt[j] = 0;
					ropt[j] = 0;
				}
				CVodeFree(kinetics_cvode_mem);	/* Free the CVODE problem memory */
				iopt[MXSTEP] = kinetics_ptr->cvode_steps;
				iopt[MAXORD] = kinetics_ptr->cvode_order;
				kinetics_cvode_mem =
					CVodeMalloc(n_reactions, f, 0.0, kinetics_y, BDF, NEWTON,
								SV, &reltol, kinetics_abstol, this, NULL,
								TRUE, iopt, ropt, kinetics_machEnv);
				if (kinetics_cvode_mem == NULL)
					malloc_error();

				/* Call CVDense to specify the CVODE dense linear solver with the
				   user-supplied Jacobian routine Jac. */
				flag = CVDense(kinetics_cvode_mem, Jac, this);
				if (flag != SUCCESS)
				{
					error_msg("CVDense failed.", STOP);
				}
				flag =
					CVode(kinetics_cvode_mem, tout1, kinetics_y, &t, NORMAL);
				/*
				   error_string = sformatf( "CVode failed, flag=%d.\n", flag);
				   error_msg(error_string, STOP);
				 */
			}
			/*
			   odeint(&ystart[-1], n_reactions, 0, kin_time, kinetics_ptr->comps[0].tol, kin_time/kinetics_ptr->step_divide, 0.0, &nok, &nbad, i, nsaver );
			 */
			for (j = 0; j < n_reactions; j++)
			{
				kinetics_ptr->comps[j].moles = Ith(kinetics_y, j + 1);
				kinetics_ptr->comps[j].m =
					m_original[j] - kinetics_ptr->comps[j].moles;
				if (kinetics_ptr->comps[j].m < 0)
				{
					kinetics_ptr->comps[j].moles = m_original[j];
					kinetics_ptr->comps[j].m = 0.0;
				}
			}
			if (use.Get_pp_assemblage_ptr() != NULL)
			{
				Rxn_pp_assemblage_map[cvode_pp_assemblage_save->Get_n_user()] = *cvode_pp_assemblage_save;
				use.Set_pp_assemblage_ptr(Utilities::Rxn_find(Rxn_pp_assemblage_map, cvode_pp_assemblage_save->Get_n_user()));
			}
			if (use.Get_ss_assemblage_ptr() != NULL)
			{
				ss_assemblage_free(use.Get_ss_assemblage_ptr());
				ss_assemblage_copy(cvode_ss_assemblage_save,
									use.Get_ss_assemblage_ptr(), i);
			}
			calc_final_kinetic_reaction(kinetics_ptr);
			if (set_and_run_wrapper(i, NOMIX, TRUE, nsaver, 1.0) ==
				MASS_BALANCE)
			{
				/*error_msg("FAIL 2 after successful integration in CVode", CONTINUE); */
				warning_msg("FAIL 2 after successful integration in CVode");
				flag = -1;
				goto RESTART;
			}
			for (j = 0; j < kinetics_ptr->count_comps; j++)
			{
				kinetics_ptr->comps[j].m =
					m_original[j] - kinetics_ptr->comps[j].moles;
			}
/*
 *  Restore solution i, if necessary
 */
			if (nsaver != i)
			{
				solution_duplicate(save_old, i);
			}
			free_cvode();
			use.Set_mix_in(use_save.Get_mix_in());
			use.Set_mix_ptr(use_save.Get_mix_ptr());
		}

		rate_sim_time = rate_sim_time_start + kin_time;
		store_get_equi_reactants(i, TRUE);
		pr.all = pr_all_save;

		kinetics_ptr = kinetics_bsearch(i, &n);
		for (j = 0; j < kinetics_ptr->count_comps; j++)
		{
			kinetics_ptr->comps[j].moles =
				m_original[j] - kinetics_ptr->comps[j].m;
/*						if (kinetics_ptr->comps[j].moles < 1.e-15) kinetics_ptr->comps[j].moles = 0.0;
 */ }
		m_temp = (LDBLE *) free_check_null(m_temp);
		m_original = (LDBLE *) free_check_null(m_original);
	}
	iterations = run_reactions_iterations;
	if (cvode_pp_assemblage_save != NULL)
	{
		delete cvode_pp_assemblage_save;
		cvode_pp_assemblage_save = NULL;
	}
	if (cvode_ss_assemblage_save != NULL)
	{
		ss_assemblage_free(cvode_ss_assemblage_save);
		cvode_ss_assemblage_save =
			(struct ss_assemblage *)
			free_check_null(cvode_ss_assemblage_save);
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
free_cvode(void)
/* ---------------------------------------------------------------------- */
{
	if (kinetics_y != NULL)
		N_VFree(kinetics_y);	/* Free vector */
	kinetics_y = NULL;
	if (cvode_last_good_y != NULL)
		N_VFree(cvode_last_good_y);	/* Free vector */
	cvode_last_good_y = NULL;
	if (cvode_prev_good_y != NULL)
		N_VFree(cvode_prev_good_y);	/* Free vector */
	cvode_prev_good_y = NULL;
	if (kinetics_abstol != NULL)
		N_VFree(kinetics_abstol);	/* Free vector */
	kinetics_abstol = NULL;
	if (kinetics_cvode_mem != NULL)
		CVodeFree(kinetics_cvode_mem);	/* Free the CVODE problem memory */
	kinetics_cvode_mem = NULL;
	if (kinetics_machEnv != NULL)
		M_EnvFree_Serial(kinetics_machEnv);	/* Free the machine environment memory */
	kinetics_machEnv = NULL;
	if (cvode_pp_assemblage_save != NULL)
	{
		delete cvode_pp_assemblage_save;
		cvode_pp_assemblage_save = NULL;
	}
	if (cvode_ss_assemblage_save != NULL)
	{
		ss_assemblage_free(cvode_ss_assemblage_save);
		cvode_ss_assemblage_save =
			(struct ss_assemblage *)
			free_check_null(cvode_ss_assemblage_save);
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
set_advection(int i, int use_mix, int use_kinetics, int nsaver)
/* ---------------------------------------------------------------------- */
{
/*
 *   i			--user number for soln, reaction, etc.
 *   use_mix	  --integer flag
					state == TRANSPORT: DISP, STAG, NOMIX
					state == REACTION: TRUE, FALSE
			state == ADVECTION: TRUE, FALSE
 *   use_kinetics --true or false flag to calculate kinetic reactions
 *   nsaver	   --user number to store solution
 */
	int n;

	cell = i;
	reaction_step = 1;
/*
 *   Find mixture or solution
 */

	use.Set_mix_ptr(NULL);
	use.Set_mix_in(false);
	use.Set_mix_ptr(Utilities::Rxn_find(Rxn_mix_map, i));
	if (use_mix == TRUE && use.Get_mix_ptr() != NULL)
	{
		use.Set_mix_in(true);
		use.Set_n_mix_user(i);
		use.Set_n_mix_user_orig(i);
		use.Set_n_solution_user(i);
	}
	else
	{
		int n_solution;
		use.Set_solution_ptr(solution_bsearch(i, &n_solution, FALSE));
		//use.Set_n_solution(n_solution);
		if (use.Get_solution_ptr() == NULL)
		{
			error_string = sformatf( "Solution %d not found.",
					use.Get_n_solution_user());
			error_msg(error_string, STOP);
		}
		use.Set_n_solution_user(i);
		use.Set_solution_in(true);
	}
	save.solution = TRUE;
	save.n_solution_user = nsaver;
	save.n_solution_user_end = nsaver;
/*
 *   Find pure phase assemblage
 */

	//use.Get_pp_assemblage_ptr() = pp_assemblage_bsearch(i, &use.n_pp_assemblage);
	use.Set_pp_assemblage_ptr(Utilities::Rxn_find(Rxn_pp_assemblage_map, i));
	if (use.Get_pp_assemblage_ptr() != NULL)
	{
		use.Set_pp_assemblage_in(true);
		use.Set_n_pp_assemblage_user(i);
		save.pp_assemblage = TRUE;
		save.n_pp_assemblage_user = i;
		save.n_pp_assemblage_user_end = i;
	}
	else
	{
		use.Set_pp_assemblage_in(false);
		save.pp_assemblage = FALSE;
	}
/*
 *   Find irreversible reaction
 */
	//use.irrev_ptr = irrev_bsearch(i, &use.n_irrev);
	use.Set_reaction_ptr(Utilities::Rxn_find(Rxn_reaction_map, i));
	if (use.Get_reaction_ptr() != NULL)
	{
		use.Set_reaction_in(true);
		use.Set_n_reaction_user(i);
	}
	else
	{
		use.Set_reaction_in(false);
	}
/*
 *   Find exchange
 */
	//use.Get_exchange_ptr() = exchange_bsearch(i, &use.n_exchange);
	use.Set_exchange_ptr(Utilities::Rxn_find(Rxn_exchange_map, i));
	if (use.Get_exchange_ptr() != NULL)
	{
		use.Set_exchange_in(true);
		use.Set_n_exchange_user(i);
		save.exchange = TRUE;
		save.n_exchange_user = i;
		save.n_exchange_user_end = i;
	}
	else
	{
		use.Set_exchange_in(false);
		save.exchange = FALSE;
	}

/*
 *   Find surface
 */
	int n_surface;
	use.Set_surface_ptr(surface_bsearch(i, &n_surface));
	//use.Set_n_surface(n_surface);
	if (use.Get_surface_ptr() != NULL)
	{
		use.Set_surface_in(true);
		use.Set_n_surface_user(i);
		save.surface = TRUE;
		save.n_surface_user = i;
		save.n_surface_user_end = i;
	}
	else
	{
		use.Set_surface_in(false);
		save.surface = FALSE;
		dl_type_x = NO_DL;
	}
/*
 *   Find temperature;  temp retardation is done in step
 */
	use.Set_temperature_ptr(Utilities::Rxn_find(Rxn_temperature_map, i));
	if (use.Get_temperature_ptr() != NULL)
	{
		use.Set_temperature_in(true);
		use.Set_n_temperature_user(i);
	}
	else
	{
		use.Set_temperature_in(false);
	}
/*
 *   Find pressure
 */
	use.Set_pressure_ptr(Utilities::Rxn_find(Rxn_pressure_map, i));
	if (use.Get_pressure_ptr() != NULL)
	{
		use.Set_pressure_in(true);
		use.Set_n_pressure_user(i);
	}
	else
	{
		use.Set_pressure_in(false);
	}
/*
 *   Find gas
 */
	//use.Get_gas_phase_ptr() = gas_phase_bsearch(i, &use.n_gas_phase);
	use.Set_gas_phase_ptr(Utilities::Rxn_find(Rxn_gas_phase_map, i));
	if (use.Get_gas_phase_ptr() != NULL)
	{
		use.Set_gas_phase_in(true);
		use.Set_n_gas_phase_user(i);
		save.gas_phase = TRUE;
		save.n_gas_phase_user = i;
		save.n_gas_phase_user_end = i;
	}
	else
	{
		use.Set_gas_phase_in(false);
		save.gas_phase = FALSE;
	}
/*
 *   Find ss_assemblage
 */
	int n_ss_assemblage;
	use.Set_ss_assemblage_ptr(ss_assemblage_bsearch(i, &n_ss_assemblage));
	//use.Set_n_ss_assemblage(n_ss_assemblage);
	if (use.Get_ss_assemblage_ptr() != NULL)
	{
		use.Set_ss_assemblage_in(true);
		use.Set_n_ss_assemblage_user(i);
		save.ss_assemblage = TRUE;
		save.n_ss_assemblage_user = i;
		save.n_ss_assemblage_user_end = i;
	}
	else
	{
		use.Set_ss_assemblage_in(false);
		save.ss_assemblage = FALSE;
	}
/*
 *   Find kinetics
 */
	use.Set_kinetics_ptr(NULL);
	use.Set_kinetics_in(false);
	save.kinetics = FALSE;
	if (use_kinetics == TRUE)
	{
		use.Set_kinetics_ptr(kinetics_bsearch(i, &n));
		if (use.Get_kinetics_ptr() != NULL)
		{
			use.Set_n_kinetics_user(i);
			//use.Set_n_kinetics(n);
			use.Set_kinetics_in(true);
			save.kinetics = TRUE;
			save.n_kinetics_user = i;
			save.n_kinetics_user_end = i;
		}
	}
	/*
	if (use.irrev_ptr != NULL && use.Get_kinetics_ptr() != NULL)
	{
		warning_msg("Should not use REACTION in same simulation with KINETICS.");
	}
	*/
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
store_get_equi_reactants(int l, int kin_end)
/* ---------------------------------------------------------------------- */
{
	int i, j, k;

	if (use.Get_pp_assemblage_in() == TRUE)
	{
		use.Set_pp_assemblage_ptr(Utilities::Rxn_find(Rxn_pp_assemblage_map, l));
	}
	else
		use.Set_pp_assemblage_ptr(NULL);
	cxxPPassemblage * pp_assemblage_ptr = (cxxPPassemblage *) use.Get_pp_assemblage_ptr();
	if (use.Get_gas_phase_in() == TRUE)
	{
		//use.Get_gas_phase_ptr() = gas_phase_bsearch(l, &use.n_gas_phase);
		use.Set_gas_phase_ptr(Utilities::Rxn_find(Rxn_gas_phase_map, l));
	}
	else
		use.Set_gas_phase_ptr(NULL);
	if (use.Get_ss_assemblage_in() == TRUE)
	{
		int n_ss_assemblage;
		use.Set_ss_assemblage_ptr(
			ss_assemblage_bsearch(l, &n_ss_assemblage));
		//use.Set_n_ss_assemblage(n_ss_assemblage);
	}
	else
		use.Set_ss_assemblage_ptr(NULL);

	if (kin_end == FALSE)
	{
		count_pp = count_s_s = count_pg = 0;
		if (use.Get_pp_assemblage_ptr() != NULL)
			//count_pp = use.Get_pp_assemblage_ptr()->count_comps;
			count_pp = (int) pp_assemblage_ptr->Get_pp_assemblage_comps().size();
		if (use.Get_gas_phase_ptr() != NULL)
		{
			cxxGasPhase *gas_phase_ptr = (cxxGasPhase *) use.Get_gas_phase_ptr();
			count_pg = (int) gas_phase_ptr->Get_gas_comps().size();
		}
		if (use.Get_ss_assemblage_ptr() != NULL)
		{
			for (j = 0; j < use.Get_ss_assemblage_ptr()->count_s_s; j++)
			{
				for (i = 0; i < use.Get_ss_assemblage_ptr()->s_s[j].count_comps;
					 i++)
				{
					count_s_s++;
				}
			}
		}
		k = count_pp + count_s_s + count_pg;
		x0_moles = NULL;
		if (k == 0)
			return (OK);
		x0_moles = (LDBLE *) PHRQ_malloc((size_t) k * sizeof(LDBLE));
		if (x0_moles == NULL)
			malloc_error();
		for (i = 0; i < k; i++)
		{
		  x0_moles[i] = 0.0;
		}
		k = -1;
		if (pp_assemblage_ptr)
		{
			std::map<std::string, cxxPPassemblageComp>::iterator it;
			it =  pp_assemblage_ptr->Get_pp_assemblage_comps().begin();
			for ( ; it != pp_assemblage_ptr->Get_pp_assemblage_comps().end(); it++)
			{
				x0_moles[++k] = it->second.Get_moles();
			}
		}
#ifdef SKIP
		for (j = 0; j < count_pp; j++)
		{
			x0_moles[++k] = use.Get_pp_assemblage_ptr()->pure_phases[j].moles;
		}
#endif
		{
			cxxGasPhase *gas_phase_ptr = (cxxGasPhase *) use.Get_gas_phase_ptr();
			if (gas_phase_ptr)
			{
				for (size_t l = 0; l < gas_phase_ptr->Get_gas_comps().size(); l++)
				{
					x0_moles[++k] += gas_phase_ptr->Get_gas_comps()[l].Get_moles();
				}
			}
		}
		if (count_s_s != 0)
		{
			for (j = 0; j < use.Get_ss_assemblage_ptr()->count_s_s; j++)
			{
				for (i = 0; i < use.Get_ss_assemblage_ptr()->s_s[j].count_comps;
					 i++)
				{
					x0_moles[++k] =
						use.Get_ss_assemblage_ptr()->s_s[j].comps[i].moles;
				}
/*!!!! also miscibility gap comps ??
 */
			}
		}
	}
	else
	{
		k = -1;
		if (pp_assemblage_ptr && count_pp > 0)
		{
			std::map<std::string, cxxPPassemblageComp>::iterator it;
			it =  pp_assemblage_ptr->Get_pp_assemblage_comps().begin();
			for ( ; it != pp_assemblage_ptr->Get_pp_assemblage_comps().end(); it++)
			{
				it->second.Set_moles(x0_moles[++k]);
				it->second.Set_delta(0.0);
			}
		}
#ifdef SKIP
		for (j = 0; j < count_pp; j++)
		{
			use.Get_pp_assemblage_ptr()->pure_phases[j].moles = x0_moles[++k];
			use.Get_pp_assemblage_ptr()->pure_phases[j].delta = 0.0;
		}
#endif
		{
			cxxGasPhase *gas_phase_ptr = (cxxGasPhase *) use.Get_gas_phase_ptr();
			if (gas_phase_ptr && count_pg)
			{
				std::vector<cxxGasComp> temp_comps(gas_phase_ptr->Get_gas_comps());
				for (size_t l = 0; l < temp_comps.size(); l++)
				{
					temp_comps[l].Set_moles(x0_moles[++k]);
				}
				gas_phase_ptr->Set_gas_comps(temp_comps);
			}
		}
		if (count_s_s != 0)
		{
			for (j = 0; j < use.Get_ss_assemblage_ptr()->count_s_s; j++)
			{
				for (i = 0; i < use.Get_ss_assemblage_ptr()->s_s[j].count_comps;
					 i++)
				{
					use.Get_ss_assemblage_ptr()->s_s[j].comps[i].initial_moles =
						x0_moles[++k];
				}
/*!!!! also miscibility gap comps ??
 */
			}
		}
/*
 *   This condition makes output equal for incremental_reactions TRUE/FALSE...
 *		if (incremental_reactions == FALSE || reaction_step == count_total_steps)
 */
		x0_moles = (LDBLE *) free_check_null(x0_moles);
	}
	return (OK);
}


void Phreeqc::
f(integertype N, realtype t, N_Vector y, N_Vector ydot,
			  void *f_data)
{
	int i, n_reactions, n_user;
	LDBLE step_fraction;
	struct kinetics *kinetics_ptr;
	Phreeqc *pThis = (Phreeqc *) f_data;

	pThis->cvode_error = FALSE;
	n_reactions = pThis->cvode_n_reactions;
	n_user = pThis->cvode_n_user;
	kinetics_ptr = (struct kinetics *) pThis->cvode_kinetics_ptr;
	step_fraction = pThis->cvode_step_fraction;
	pThis->rate_sim_time = pThis->cvode_rate_sim_time;

	for (i = 0; i < n_reactions; i++)
	{
		/*
		   kinetics_ptr->comps[i].moles = y[i + 1];
		   kinetics_ptr->comps[i].m = m_original[i] - y[i + 1];
		 */
		kinetics_ptr->comps[i].moles = Ith(y, i + 1);
		kinetics_ptr->comps[i].m = pThis->m_original[i] - Ith(y, i + 1);
		if (kinetics_ptr->comps[i].m < 0)
		{
			/*
			   NOTE: y is not correct if it is greater than m_original
			   However, it seems to work to let y wander off, but use
			   .moles as the correct integral.
			   It does not work to reset Y to m_original, presumably
			   because the rational extrapolation gets screwed up.
			 */

			/*
			   Ith(y,i + 1) = m_original[i];
			 */
			kinetics_ptr->comps[i].moles = pThis->m_original[i];
			kinetics_ptr->comps[i].m = 0.0;
		}
	}
	pThis->calc_final_kinetic_reaction(kinetics_ptr);
	/*      if (set_and_run(n_user, FALSE, TRUE, n_user, step_fraction) == MASS_BALANCE) { */
	if (pThis->use.Get_pp_assemblage_ptr() != NULL)
	{
		pThis->Rxn_pp_assemblage_map[pThis->cvode_pp_assemblage_save->Get_n_user()] = *pThis->cvode_pp_assemblage_save;
		pThis->use.Set_pp_assemblage_ptr(Utilities::Rxn_find(pThis->Rxn_pp_assemblage_map, pThis->cvode_pp_assemblage_save->Get_n_user()));
	}
	if (pThis->use.Get_ss_assemblage_ptr() != NULL)
	{
		pThis->ss_assemblage_free(pThis->use.Get_ss_assemblage_ptr());
		pThis->ss_assemblage_copy(pThis->cvode_ss_assemblage_save,
							pThis->use.Get_ss_assemblage_ptr(), n_user);
	}

	if (pThis->set_and_run_wrapper(n_user, FALSE, TRUE, n_user, 0.0) == MASS_BALANCE)
	{
		pThis->run_reactions_iterations += pThis->iterations;
		pThis->cvode_error = TRUE;
		/*
		   error_msg("Mass balance error in f", CONTINUE);
		 */
		return;
	}
	if (pThis->cvode_test == TRUE)
	{
		return;
	}
	pThis->run_reactions_iterations += pThis->iterations;
	for (i = 0; i < n_reactions; i++)
	{
		kinetics_ptr->comps[i].moles = 0.0;
	}
	pThis->calc_kinetic_reaction(kinetics_ptr, 1.0);
	for (i = 0; i < n_reactions; i++)
	{
		/*
		   dydx[i + 1] = kinetics_ptr->comps[i].moles;
		 */
		Ith(ydot, i + 1) = kinetics_ptr->comps[i].moles;
	}
	return;
}

/*
static void Jac(integertype N, DenseMat J, RhsFn f, void *f_data, realtype t,
				N_Vector y, N_Vector fy, N_Vector ewt, realtype h,
				realtype uround, void *jac_data, long int *nfePtr,
				N_Vector vtemp1, N_Vector vtemp2, N_Vector vtemp3);
*/

void  Phreeqc::
Jac(integertype N, DenseMat J, RhsFn f, void *f_data,
	realtype t, N_Vector y, N_Vector fy, N_Vector ewt,
					 realtype h, realtype uround, void *jac_data,
					 long int *nfePtr, N_Vector vtemp1, N_Vector vtemp2,
					 N_Vector vtemp3)
{
	int count_cvode_errors;
	int i, j, n_reactions, n_user;
	LDBLE *initial_rates, del;
	struct kinetics *kinetics_ptr;
	LDBLE step_fraction;

	Phreeqc *pThis = (Phreeqc *) f_data;

	pThis->cvode_error = FALSE;
	n_reactions = pThis->cvode_n_reactions;
	n_user = pThis->cvode_n_user;
	kinetics_ptr = (struct kinetics *) pThis->cvode_kinetics_ptr;
	step_fraction = pThis->cvode_step_fraction;
	pThis->rate_sim_time = pThis->cvode_rate_sim_time;

	initial_rates =
		(LDBLE *) pThis->PHRQ_malloc ((size_t) n_reactions * sizeof(LDBLE));
	if (initial_rates == NULL)
		pThis->malloc_error();

	for (i = 0; i < n_reactions; i++)
	{
		/*
		   kinetics_ptr->comps[i].moles = y[i + 1];
		   kinetics_ptr->comps[i].m = m_original[i] - y[i + 1];
		 */
		kinetics_ptr->comps[i].moles = Ith(y, i + 1);
		kinetics_ptr->comps[i].m = pThis->m_original[i] - Ith(y, i + 1);
		if (kinetics_ptr->comps[i].m < 0)
		{
			/*
			   NOTE: y is not correct if it is greater than m_original
			   However, it seems to work to let y wander off, but use
			   .moles as the correct integral.
			   It does not work to reset Y to m_original, presumably
			   because the rational extrapolation gets screwed up.
			 */

			/*
			   Ith(y,i + 1) = m_original[i];
			 */
			kinetics_ptr->comps[i].moles = pThis->m_original[i];
			kinetics_ptr->comps[i].m = 0.0;
		}
	}
	pThis->calc_final_kinetic_reaction(kinetics_ptr);
	/* if (set_and_run(n_user, FALSE, TRUE, n_user, step_fraction) == MASS_BALANCE) { */
	if (pThis->use.Get_pp_assemblage_ptr() != NULL)
	{
		pThis->Rxn_pp_assemblage_map[pThis->cvode_pp_assemblage_save->Get_n_user()] = *pThis->cvode_pp_assemblage_save;
		pThis->use.Set_pp_assemblage_ptr(Utilities::Rxn_find(pThis->Rxn_pp_assemblage_map, pThis->cvode_pp_assemblage_save->Get_n_user()));
	}
	if (pThis->set_and_run_wrapper(n_user, FALSE, TRUE, n_user, 0.0) == MASS_BALANCE)
	{
		pThis->run_reactions_iterations += pThis->iterations;
		pThis->cvode_error = TRUE;
		/*
		   error_msg("Mass balance error in jacobian", CONTINUE);
		 */
		initial_rates = (LDBLE *) pThis->free_check_null(initial_rates);
		return;
	}
	pThis->run_reactions_iterations += pThis->iterations;
	for (i = 0; i < n_reactions; i++)
		kinetics_ptr->comps[i].moles = 0.0;
	pThis->calc_kinetic_reaction(kinetics_ptr, 1.0);
	for (i = 0; i < n_reactions; i++)
	{
		initial_rates[i] = kinetics_ptr->comps[i].moles;
	}
	for (i = 0; i < n_reactions; i++)
	{
		/* calculate reaction up to current time */
		del = 1e-12;
		pThis->cvode_error = TRUE;
		count_cvode_errors = 0;
		while (pThis->cvode_error == TRUE)
		{
			del /= 10.;
			for (j = 0; j < n_reactions; j++)
			{
				/*
				   kinetics_ptr->comps[j].moles = y[j + 1];
				   kinetics_ptr->comps[j].m = m_original[j] - y[j + 1];
				 */
				kinetics_ptr->comps[j].moles = Ith(y, j + 1);
				kinetics_ptr->comps[j].m = pThis->m_original[j] - Ith(y, j + 1);
				if (kinetics_ptr->comps[i].m < 0)
				{
					/*
					   NOTE: y is not correct if it is greater than m_original
					   However, it seems to work to let y wander off, but use
					   .moles as the correct integral.
					   It does not work to reset Y to m_original, presumably
					   because the rational extrapolation gets screwed up.
					 */

					/*
					   Ith(y,i + 1) = m_original[i];
					 */
					kinetics_ptr->comps[i].moles = pThis->m_original[i];
					kinetics_ptr->comps[i].m = 0.0;
				}
			}

			/* Add small amount of ith reaction */
			kinetics_ptr->comps[i].m -= del;
			if (kinetics_ptr->comps[i].m < 0)
			{
				kinetics_ptr->comps[i].m = 0;
			}
			kinetics_ptr->comps[i].moles += del;
			pThis->calc_final_kinetic_reaction(kinetics_ptr);
			if (pThis->use.Get_pp_assemblage_ptr() != NULL)
			{
				pThis->Rxn_pp_assemblage_map[pThis->cvode_pp_assemblage_save->Get_n_user()] = *pThis->cvode_pp_assemblage_save;
				pThis->use.Set_pp_assemblage_ptr(Utilities::Rxn_find(pThis->Rxn_pp_assemblage_map, pThis->cvode_pp_assemblage_save->Get_n_user()));
			}
			if (pThis->set_and_run_wrapper
				(n_user, FALSE, TRUE, n_user, step_fraction) == MASS_BALANCE)
			{
				count_cvode_errors++;
				pThis->cvode_error = TRUE;
				if (count_cvode_errors > 30)
				{
					initial_rates = (LDBLE *) pThis->free_check_null(initial_rates);
					return;
				}
				pThis->run_reactions_iterations += pThis->iterations;
				continue;
			}
			pThis->cvode_error = FALSE;
			pThis->run_reactions_iterations += pThis->iterations;
			/*kinetics_ptr->comps[i].moles -= del; */
			for (j = 0; j < n_reactions; j++)
				kinetics_ptr->comps[j].moles = 0.0;
			pThis->calc_kinetic_reaction(kinetics_ptr, 1.0);

			/* calculate new rates for df/dy[i] */
			/* dfdx[i + 1] = 0.0; */
			for (j = 0; j < n_reactions; j++)
			{
				IJth(J, j + 1, i + 1) =
					(kinetics_ptr->comps[j].moles - initial_rates[j]) / del;
			}
		}
	}
	for (i = 0; i < n_reactions; i++)
	{
		kinetics_ptr->comps[i].moles = 0;
	}
	initial_rates = (LDBLE *) pThis->free_check_null(initial_rates);
	return;
}

void Phreeqc::
cvode_init(void)
{
	cvode_kinetics_ptr = NULL;
	cvode_test = 0;
	cvode_error = 0;
	cvode_n_user = -99;
	cvode_n_reactions = -99;
	cvode_step_fraction = 0.0;
	cvode_rate_sim_time = 0.0;
	cvode_rate_sim_time_start = 0.0;
	cvode_last_good_time = 0.0;
	cvode_prev_good_time = 0.0;
	cvode_last_good_y = NULL;
	cvode_prev_good_y = NULL;
	kinetics_machEnv = NULL;
	kinetics_y = kinetics_abstol = NULL;
	kinetics_cvode_mem = NULL;
	cvode_pp_assemblage_save = NULL;
	cvode_ss_assemblage_save = NULL;
	return;
}
