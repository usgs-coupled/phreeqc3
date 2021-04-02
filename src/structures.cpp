#include "Utils.h"
#include "Phreeqc.h"
#include <iostream>

#include "phqalloc.h"
#include "Temperature.h"
#include "cxxMix.h"
#include "Exchange.h"
#include "GasPhase.h"
#include "Reaction.h"
#include "PPassemblage.h"
#include "Use.h"
#include "SSassemblage.h"
#include "cxxKinetics.h"
#include "Surface.h"
#include "Solution.h"


/* ---------------------------------------------------------------------- */
int Phreeqc::
clean_up(void)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Free all allocated memory, except strings
	 */
	int i, j;
#if defined MULTICHART
	chart_handler.End_timer();
	output_flush();
#if 0
	// Wait for charts to end
	while (0 != this->chart_handler.Get_active_charts())
	{
		System::Threading::Thread::Sleep(60);
	}
#endif
#endif

	isotopes_x.clear();
	/* model */
	last_model.gas_phase.clear();
	last_model.pp_assemblage.clear();
	last_model.add_formula.clear();
	last_model.si.clear();
	last_model.ss_assemblage.clear();
	last_model.surface_comp.clear();
	last_model.surface_charge.clear();
	/* model */
	free_model_allocs();

	/* species */

	for (j = 0; j < (int)s.size(); j++)
	{
		s_free(s[j]);
		delete s[j];
	}
	s.clear();

	/* master species */

	for (j = 0; j < (int)master.size(); j++)
	{
		master_free(master[j]);
	}
	master.clear();

	/* elements */

	for (j = 0; j < (int)elements.size(); j++)
	{
		delete elements[j];
	}
	elements.clear();
	/* solutions */
	Rxn_solution_map.clear();
	/* surfaces */
	Rxn_surface_map.clear();
	/* exchange */
	Rxn_exchange_map.clear();
	/* pp assemblages */
	Rxn_pp_assemblage_map.clear();
	/* s_s assemblages */
	Rxn_ss_assemblage_map.clear();
	/* irreversible reactions */
	Rxn_reaction_map.clear();
	/* temperature */
	Rxn_temperature_map.clear();
	/* pressure */
	Rxn_pressure_map.clear();
	/* unknowns */
	for (j = 0; j < (int)x.size(); j++)
	{
		unknown_free(x[j]);
	}
	x.clear();
	/* mixtures */
	Rxn_mix_map.clear();
	/* phases */
	for (j = 0; j < (int)phases.size(); j++)
	{
		phase_free(phases[j]);
		delete phases[j];
	}
	phases.clear();
	/* inverse */
	for (j = 0; j < count_inverse; j++)
	{
		inverse_free(&(inverse[j]));
	}
	inverse.clear();
	/* gases */
	Rxn_gas_phase_map.clear();
	/* kinetics */
	Rxn_kinetics_map.clear();
	x0_moles.clear();
	m_temp.clear();
	m_original.clear();
	rk_moles.clear();
	/* rates */
	for (j = 0; j < (int)rates.size(); j++)
	{
		rate_free(&rates[j]);
	}
	rates.clear();
	/* logk table */
	for (j = 0; j < (int)logk.size(); j++)
	{
		logk[j]->add_logk.clear();
		delete logk[j];
	}
	logk.clear();
	save_values.clear();
	/* working pe*/
	pe_x.clear();
	/*species_list*/
	species_list.clear();
	/* transport data */
	stag_data = (struct stag_data*)free_check_null(stag_data);
	cell_data = (struct cell_data*)free_check_null(cell_data);
	/* advection */
	advection_punch.clear();
	advection_print.clear();
	/* selected_output */
	SelectedOutput_map.clear();
	/*  user_print and user_punch */
	UserPunch_map.clear();
	rate_free(user_print);
	delete user_print;
	/*
	   Clear llnl aqueous model parameters
	 */
	llnl_temp.clear();
	llnl_adh.clear();
	llnl_bdh.clear();
	llnl_bdot.clear();
	llnl_co2_coefs.clear();
	/* master_isotope */
	for (i = 0; i < (int)master_isotope.size(); i++)
	{
		delete master_isotope[i];
	}
	master_isotope.clear();
	master_isotope_map.clear();
	/* calculate_value */
	for (i = 0; i < (int)calculate_value.size(); i++)
	{
		calculate_value_free(calculate_value[i]);
		delete calculate_value[i];
	}
	calculate_value.clear();
	calculate_value_map.clear();
	/* isotope_ratio */
	for (i = 0; i < (int)isotope_ratio.size(); i++)
	{
		delete isotope_ratio[i];
	}
	isotope_ratio.clear();
	isotope_ratio_map.clear();
	/* isotope_alpha */
	for (i = 0; i < (int)isotope_alpha.size(); i++)
	{
		delete isotope_alpha[i];
	}
	isotope_alpha.clear();
	isotope_alpha_map.clear();
	/* tally table */
	free_tally_table();
	/* CVODE memory */
	free_cvode();
	/* pitzer */
	pitzer_clean_up();
	/* sit */
	sit_clean_up();
	/* elements, species, phases*/
	elements_map.clear();
	species_map.clear();
	phases_map.clear();
	logk_map.clear();
	/* strings */
	strings_map_clear();
	/* delete basic interpreter */
	basic_free();
	/* change_surf */
	change_surf = (struct Change_Surf *) free_check_null(change_surf);
	/* miscellaneous work space */
	elt_list.clear();
	trxn.token.clear();
	mb_unknowns.clear();
	line = (char *) free_check_null(line);
	line_save = (char *) free_check_null(line_save);
	/* free user database name if defined */
	dump_file_name = (char *) free_check_null(dump_file_name);
#ifdef PHREEQCI_GUI
	free_spread();
#endif
	title_x.clear(); 
	last_title_x.clear();
	count_inverse = 0;

	sformatf_buffer = (char *) free_check_null(sformatf_buffer);
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
reinitialize(void)
/* ---------------------------------------------------------------------- */
{
	/* solutions */
	Rxn_solution_map.clear();
	/* surfaces */
	Rxn_surface_map.clear();
	/* exchange */
	Rxn_exchange_map.clear();
	/* pp assemblages */
	Rxn_pp_assemblage_map.clear();
	/* s_s assemblages */
	Rxn_ss_assemblage_map.clear();
	/* gases */
	Rxn_gas_phase_map.clear();
	/* kinetics */
	Rxn_kinetics_map.clear();
	/* irreversible reactions */
	Rxn_reaction_map.clear();
	// Temperature
	Rxn_temperature_map.clear();
	// Pressure
	Rxn_pressure_map.clear();
	return (OK);
}

/* **********************************************************************
 *
 *   Routines related to structure "element"
 *
 * ********************************************************************** */
/* ---------------------------------------------------------------------- */
int Phreeqc::
element_compare(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
	const struct element *element_ptr1, *element_ptr2;
	element_ptr1 = *(const struct element **) ptr1;
	element_ptr2 = *(const struct element **) ptr2;
/*      return(strcmp_nocase(element_ptr1->name, element_ptr2->name)); */
	return (strcmp(element_ptr1->name, element_ptr2->name));

}

/* ---------------------------------------------------------------------- */
struct element* Phreeqc::
element_store(const char * element)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Function locates the string "element" in the map for elements.
	 *
	 *   If found, pointer to the appropriate element structure is returned.
	 *
	 *   If the string is not found, a new entry is made at the end of
	 *   the elements array (position count_elements) and count_elements is
	 *   incremented. Pointer to the new structure is returned.
	 *
	 *   Arguments:
	 *      element    input, std::string to be located or stored.
	 *
	 *   Returns:
	 *      The address of an elt structure that contains the element data.
	 */
	/*
	 *   Search list
	 */
	std::map<std::string, struct element *>::const_iterator it;
	it = elements_map.find(element);
	if (it != elements_map.end())
	{
		return (it->second);
	}
	/*
	 *   Save new element structure and return pointer to it
	 */
	struct element *elt_ptr = new struct element;
	elt_ptr->name = string_hsave(element);
	elt_ptr->master = NULL;
	elt_ptr->primary = NULL;
	elt_ptr->gfw = 0.0;
	elements.push_back(elt_ptr);
	elements_map[element] = elt_ptr;
	return (elt_ptr);
}

/* **********************************************************************
 *
 *   Routines related to structure "elt_list"
 *
 * ********************************************************************** */
/* ---------------------------------------------------------------------- */
int Phreeqc::
elt_list_combine(void)
/* ---------------------------------------------------------------------- */
/*
 *      Function goes through the list of elements pointed to by elt_list
 *      and combines the coefficients of elements that are the same.
 *      Assumes elt_list has been sorted by element name.
 */
{
	int i, j;

	//if (count_elts < 1)
	//{
	//	output_msg("elt_list_combine: How did this happen?\n");
	//	return (ERROR);
	//}
	if (count_elts <= 1)
	{
		return (OK);
	}
	qsort(&elt_list[0], count_elts,
		sizeof(struct elt_list), Phreeqc::elt_list_compare);
	j = 0;
	for (i = 1; i < count_elts; i++)
	{
		if (elt_list[i].elt == elt_list[j].elt)
		{
			elt_list[j].coef += elt_list[i].coef;
		}
		else
		{
			j++;
			if (i != j)
			{
				elt_list[j].elt = elt_list[i].elt;
				elt_list[j].coef = elt_list[i].coef;
			}
		}
	}
	count_elts = j + 1;
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
elt_list_compare(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
	const struct elt_list *a, *b;

	a = (const struct elt_list *) ptr1;
	b = (const struct elt_list *) ptr2;
	return (strncmp(a->elt->name, b->elt->name, MAX_LENGTH));
}

/* ---------------------------------------------------------------------- */
struct elt_list * Phreeqc::
elt_list_dup(struct elt_list *elt_list_ptr_old)
/* ---------------------------------------------------------------------- */
{
/*
 *  Duplicates the elt_list structure pointed to by elt_list_ptr_old.
 */
	int i, count_totals;
	struct elt_list *elt_list_ptr_new;
/*
 *   Count totals data and copy
 */
	if (elt_list_ptr_old == NULL)
		return (NULL);
	for (i = 0; elt_list_ptr_old[i].elt != NULL; i++);
	count_totals = i;
/*
 *   Malloc space and store element data
 */
	elt_list_ptr_new = (struct elt_list *) PHRQ_malloc(
		(count_totals + 1) * sizeof(struct elt_list));
	if (elt_list_ptr_new == NULL)
		malloc_error();
	memcpy(elt_list_ptr_new, elt_list_ptr_old,
		   (count_totals + 1) * sizeof(struct elt_list));
	return (elt_list_ptr_new);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
elt_list_print(struct elt_list *elt_list_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *  Duplicates the elt_list structure pointed to by elt_list_ptr_old.
 */
	int i;
/*
 *   Debug print for element list
 */
	if (elt_list_ptr == NULL)
		return (ERROR);
	output_msg(sformatf( "Elt_list\n"));
	for (i = 0; elt_list_ptr[i].elt != NULL; i++)
	{
		output_msg(sformatf( "\t%s\t%e\n", elt_list_ptr[i].elt->name,
				   (double) elt_list_ptr[i].coef));
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
cxxNameDouble Phreeqc::
elt_list_NameDouble(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Takes data from work space elt_list, makes NameDouble
 */
	cxxNameDouble nd;
	for(int i = 0; i < count_elts; i++)
	{
		nd.add(elt_list[i].elt->name, elt_list[i].coef);
	}
	return (nd);
}
/* ---------------------------------------------------------------------- */
struct elt_list * Phreeqc::
elt_list_save(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Takes data from work space elt_list, allocates a new elt_list structure,
 *   copies data from work space to new structure, and returns pointer to
 *   new structure.
 */
	int j;
	struct elt_list *elt_list_ptr;
/*
 *   Sort elements in reaction and combine
 */
	elt_list_combine();
/*
 *   Malloc space and store element data
 */
	elt_list_ptr = (struct elt_list*)PHRQ_malloc(
		(count_elts + 1) * sizeof(struct elt_list));
	if (elt_list_ptr == NULL)
	{
		malloc_error();
	}
	else
	{
		for (j = 0; j < count_elts; j++)
		{
			elt_list_ptr[j].elt = elt_list[j].elt;
			elt_list_ptr[j].coef = elt_list[j].coef;
		}
		elt_list_ptr[count_elts].elt = NULL;
	}
	return (elt_list_ptr);
}
/* ---------------------------------------------------------------------- */
struct elt_list * Phreeqc::
NameDouble2elt_list(const cxxNameDouble &nd)
/* ---------------------------------------------------------------------- */
{
/*
 *   Takes NameDouble allocates space and fills new elt_list struct
 */
	struct elt_list *elt_list_ptr = (struct elt_list *) PHRQ_malloc(
		(nd.size() + 1) * sizeof(struct elt_list));
	if (elt_list_ptr == NULL)
	{
		malloc_error();
	}
	else
	{
		cxxNameDouble::const_iterator it = nd.begin();
		int i = 0;
		for( ; it != nd.end(); it++)
		{
			elt_list_ptr[i].elt = element_store(it->first.c_str());
			elt_list_ptr[i].coef = it->second;
			i++;
		}
		elt_list_ptr[i].elt = NULL;
		elt_list_ptr[i].coef = 0;
	}
	return (elt_list_ptr);
}
/* **********************************************************************
 *
 *   Routines related to structure "inverse"
 *
 * ********************************************************************** */
/* ---------------------------------------------------------------------- */
struct inverse * Phreeqc::
inverse_alloc(void)
/* ---------------------------------------------------------------------- */
/*
 *   Allocates space for a new inverse structure at position count_inverse.
 *   Initializes structure.
 *      arguments
 *      input:  none
 *      output: pointer to an inverse structure
 *      return: OK
 */
{
	struct inverse *inverse_ptr = NULL;
	inverse.resize(count_inverse + 1);
	inverse_ptr = &(inverse[count_inverse++]);
/*
 *   Initialize variables
 */
	inverse_ptr->description = NULL;
	inverse_ptr->count_solns = 0;
/*
 *   allocate space for pointers in structure to NULL
 */
	inverse_ptr->count_solns = 0;

	return (inverse_ptr);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
inverse_compare(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
/*
 *   Compare inverse values for n_user
 */
	const struct inverse *nptr1;
	const struct inverse *nptr2;

	nptr1 = (const struct inverse *) ptr1;
	nptr2 = (const struct inverse *) ptr2;
	if (nptr1->n_user > nptr2->n_user)
		return (1);
	if (nptr1->n_user < nptr2->n_user)
		return (-1);
	return (0);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
inverse_delete(int i)
/* ---------------------------------------------------------------------- */
{
/*
 *   Deletes inverse i from list (i is not user number),
 *   Frees memory allocated to inverse struct
 *   Input: i, number of inverse struct to delete
 *   Return: OK
 */
	inverse_free(&(inverse[i]));
	inverse.erase(inverse.begin() + (size_t)i);
	count_inverse--;
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
inverse_free(struct inverse *inverse_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Free all memory for an inverse structure.
 */
	int i;

	inverse_ptr->description =
		(char *) free_check_null(inverse_ptr->description);
/*   Free solns */
	inverse_ptr->solns.clear();

/*   Free uncertainties */
	inverse_ptr->uncertainties.clear();
	inverse_ptr->ph_uncertainties.clear();

/*   Free force_solns */
	inverse_ptr->force_solns.clear();

/*   Free elts */
	for (i = 0; i < inverse_ptr->elts.size(); i++)
	{
		inverse_ptr->elts[i].uncertainties.clear();
	};
	inverse_ptr->elts.clear();

/*   Free isotopes */
	for (i = 0; i < inverse_ptr->isotopes.size(); i++)
	{
		inverse_ptr->isotopes[i].uncertainties.clear();
	};
	inverse_ptr->isotopes.clear();

	for (i = 0; i < inverse_ptr->i_u.size(); i++)
	{
		inverse_ptr->i_u[i].uncertainties.clear();
	};
	inverse_ptr->i_u.clear();

/*   Free phases */
	for (i = 0; i < inverse_ptr->phases.size(); i++)
	{
		inverse_ptr->phases[i].isotopes.clear();
	}
	inverse_ptr->phases.clear();

/*   Free carbon derivatives */
	inverse_ptr->dalk_dph.clear(); 
	inverse_ptr->dalk_dc.clear();

	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
inverse_isotope_compare(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
	int i;
	const struct inv_isotope *iso_ptr1, *iso_ptr2;

	iso_ptr1 = (const struct inv_isotope *) ptr1;
	iso_ptr2 = (const struct inv_isotope *) ptr2;
	i = strcmp_nocase(iso_ptr1->elt_name, iso_ptr2->elt_name);
	if (i != 0)
		return (i);
	if (iso_ptr1->isotope_number < iso_ptr2->isotope_number)
	{
		return (-1);
	}
	else if (iso_ptr1->isotope_number > iso_ptr2->isotope_number)
	{
		return (1);
	}
	return (0);
}

/* ---------------------------------------------------------------------- */
struct inverse * Phreeqc::
inverse_search(int n_user, int *n)
/* ---------------------------------------------------------------------- */
{
/*   Linear search of the structure array "inverse" for user number n_user.
 *
 *   Arguments:
 *      n_user  input, user number
 *      n       output, position in inverse
 *
 *   Returns:
 *      if found, the address of the inverse element
 *      if not found, NULL
 *
 */
	int i;
	for (i = 0; i < count_inverse; i++)
	{
		if (inverse[i].n_user == n_user)
		{
			*n = i;
			return (&(inverse[i]));
		}
	}
/*
 *   An inverse structure with n_user was not found
 */
	return (NULL);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
inverse_sort(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Sort array of inverse structures
 */
	if (count_inverse > 1)
	{
		qsort(&inverse[0], (size_t) count_inverse,
			  sizeof(struct inverse), inverse_compare);
	}
	return (OK);
}

/* **********************************************************************
 *
 *   Routines related to structure "master"
 *
 * ********************************************************************** */
/* ---------------------------------------------------------------------- */
struct master * Phreeqc::
master_alloc(void)
/* ---------------------------------------------------------------------- */
/*
 *   Allocates space to a master structure and initializes the space.
 *      arguments: void
 *      return: pointer to a master structure
 */
{
	struct master *ptr = new struct master;
/*
 *   set pointers in structure to NULL
 */
	ptr->in = FALSE;
	ptr->number = -1;
	ptr->last_model = -1;
	ptr->type = 0;
	ptr->primary = FALSE;
	ptr->coef = 0.0;
	ptr->total = 0.0;
	ptr->isotope_ratio = 0;
	ptr->isotope_ratio_uncertainty = 0;
	ptr->isotope = 0;
	ptr->total_primary = 0;
	ptr->elt = NULL;
	ptr->alk = 0.0;
	ptr->gfw = 0.0;
	ptr->gfw_formula = NULL;
	ptr->unknown = NULL;
	ptr->s = NULL;
	ptr->pe_rxn = NULL;
	ptr->minor_isotope = FALSE;
	return (ptr);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
master_delete(const char* cptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Delete master species:  Free memory of master species structure, free
 *   the structure, and remove from array master.
 *
 *   Input
 *	ptr  character string with name of element or valence state
 *   Returns
 *	TRUE if master species was deleted.
 *	FALSE if master species was not found.
 */
	int n;

	if (master_search(cptr, &n) == NULL)
		return (FALSE);
	master_free(master[n]);
	master.erase(master.begin() + n);
	return (TRUE);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
master_free(struct master *master_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Free memory pointed to by master species pointer, master_ptr.
 *   Frees master_ptr itself.
 */
	if (master_ptr == NULL)
		return (ERROR);
	delete master_ptr;
	return (OK);
}

/* ---------------------------------------------------------------------- */
struct master * Phreeqc::
master_bsearch(const char* cptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Uses binary search. Assumes master is in sort order.
 *   Find master species for string (*cptr) containing name of element or valence state.
 *
 *   Input: cptr    pointer to string containing element name
 *
 *   Return: pointer to master structure containing name cptr or NULL.
 */
	void *void_ptr;
	if (master.size() == 0)
	{
		return (NULL);
	}
	void_ptr = bsearch((const char *) cptr,
					   (char *) &master[0],
					   master.size(),
					   sizeof(struct master *), master_compare_string);
	if (void_ptr == NULL)
	{
		void_ptr = bsearch(cptr,
			(char*)&master[0],
			master.size(),
			sizeof(struct master*), master_compare_string);
	}
	if (void_ptr == NULL)
	{
		return (NULL);
	}
	else
	{
		return (*(struct master **) void_ptr);
	}
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
master_compare_string(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
	const char *string_ptr;
	const struct master *master_ptr;

	string_ptr = (const char *) ptr1;
	master_ptr = *(const struct master **) ptr2;
	return (strcmp_nocase(string_ptr, master_ptr->elt->name));
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
master_compare(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
	const struct master *master_ptr1, *master_ptr2;
	master_ptr1 = *(const struct master **) ptr1;
	master_ptr2 = *(const struct master **) ptr2;
	return (strcmp_nocase(master_ptr1->elt->name, master_ptr2->elt->name));
}

/* ---------------------------------------------------------------------- */
struct master * Phreeqc::
master_bsearch_primary(const char* cptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Find primary master species for first element in the string, cptr.
 *   Uses binary search. Assumes master is in sort order.
 */
	int l;
	const char* cptr1;
	struct master *master_ptr_primary;
/*
 *   Find element name
 */
	cptr1 = cptr;
	{
		std::string elt;
		get_elt(&cptr1, elt, &l);
		/*
		 *   Search master species list
		 */
		master_ptr_primary = master_bsearch(elt.c_str());
	}
	if (master_ptr_primary == NULL)
	{
		input_error++;
		error_string = sformatf(
				"Could not find primary master species for %s.", cptr);
		error_msg(error_string, CONTINUE);
	}
	return (master_ptr_primary);
}
/* ---------------------------------------------------------------------- */
struct master * Phreeqc::
master_bsearch_secondary(const char* cptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Find secondary master species that corresponds to the primary master species.
 *   i.e. S(6) for S.
 */
	int l;
	const char* cptr1;
	std::string elt;
	struct master *master_ptr_primary, *master_ptr=NULL, *master_ptr_secondary=NULL;
/*
 *   Find element name
 */
	cptr1 = cptr;
	get_elt(&cptr1, elt, &l);
/*
 *   Search master species list
 */
	master_ptr_primary = master_bsearch(elt.c_str());
	if (master_ptr_primary == NULL)
	{
		input_error++;
		error_string = sformatf(
				"Could not find primary master species for %s.", cptr);
		error_msg(error_string, CONTINUE);
	}
/*
 *  If last in list or not redox
*/
	if (master_ptr_primary)
	{
		if ((master_ptr_primary->number >= (int)master.size() - 1) || 
			(master[(size_t)master_ptr_primary->number + 1]->elt->primary != master_ptr_primary))
		{
			return(master_ptr_primary);
		}
		/*
		*  Find secondary master with same species as primary
		*/
		master_ptr = NULL;
		for (size_t j = master_ptr_primary->number + 1; j < master.size(); j++)
		{
			if (master[j]->s == master_ptr_primary->s)
			{
				master_ptr = master[j];
			}
		}
	}
/*
 *
 */
	if (master_ptr != NULL && master_ptr->elt != NULL && (master_ptr->elt->primary == master_ptr_primary))
	{
		master_ptr_secondary = master_ptr;
	}
	else
	{		
		input_error++;
		error_string = sformatf(
				"Could not find secondary master species for %s.", cptr);
		error_msg(error_string, STOP);
	}


	return (master_ptr_secondary);
}
/* ---------------------------------------------------------------------- */
struct master * Phreeqc::
master_search(const char* cptr, int *n)
/* ---------------------------------------------------------------------- */
{
/*
 *   Linear search of master to find master species in string, cptr.
 *   Returns pointer if found. n contains position in array master.
 *   Returns NULL if not found.
 */
	int i;
	struct master *master_ptr;
/*
 *   Search master species list
 */
	*n = -999;
	for (i = 0; i < (int)master.size(); i++)
	{
		if (strcmp(cptr, master[i]->elt->name) == 0)
		{
			*n = i;
			master_ptr = master[i];
			return (master_ptr);
		}
	}
	return (NULL);
}
/* **********************************************************************
 *
 *   Routines related to structure "phases"
 *
 * ********************************************************************** */
/* ---------------------------------------------------------------------- */
struct phase * Phreeqc::
phase_alloc(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Allocates space to a phase structure and initializes
 *      arguments: void
 *      return: pointer to new phase structure
 */
	struct phase *phase_ptr;
/*
 *   Allocate space
 */
	phase_ptr = new struct phase;
/*
 *   Initialize space
 */
	phase_init(phase_ptr);
	return (phase_ptr);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
phase_compare(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
/*
 *   Compares names of phases for sort
 */
	const struct phase *phase_ptr1, *phase_ptr2;
	phase_ptr1 = *(const struct phase **) ptr1;
	phase_ptr2 = *(const struct phase **) ptr2;
	return (strcmp_nocase(phase_ptr1->name, phase_ptr2->name));
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
phase_compare_string(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
	const char *char_ptr;
	const struct phase *phase_ptr;
	char_ptr = (const char *) ptr1;
	phase_ptr = *(const struct phase **) ptr2;
	return (strcmp_nocase(char_ptr, phase_ptr->name));
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
phase_delete(int i)
/* ---------------------------------------------------------------------- */
{
/*
 *   Deletes phase i from list, phases
 *   Frees memory allocated to phase[i] and renumbers phases to remove number i.
 *   Input: i, number of phase
 *   Return: OK
 */
	phase_free(phases[i]);
	phases.erase(phases.begin() + (size_t)i);
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
phase_free(struct phase *phase_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Frees memory allocated within phase[i], does not free phase structure
 *   Input: i, number of phase
 *   Return: OK
 */
	if (phase_ptr == NULL)
		return (ERROR);
	phase_ptr->next_elt =
		(struct elt_list *) free_check_null(phase_ptr->next_elt);
	phase_ptr->next_sys_total =
		(struct elt_list *) free_check_null(phase_ptr->next_sys_total);
	phase_ptr->add_logk.clear(); 
	return (OK);
}

/* ---------------------------------------------------------------------- */
struct phase * Phreeqc::
phase_bsearch(const char* cptr, int *j, int print)
/* ---------------------------------------------------------------------- */
{
/*   Binary search the structure array "phases" for a name that is equal to
 *   cptr. Assumes array phases is in sort order.
 *
 *   Arguments:
 *      name  input, a character string to be located in phases.
 *      j	    index number in array phases.
 *
 *   Returns:
 *      if found, pointer to phase structure.
 *      if not found, NULL
 *
 */
	void *void_ptr;

	void_ptr = NULL;
	if ((int)phases.size() > 0)
	{
		void_ptr = (void *)
			bsearch((char *) cptr,
					(char *) &phases[0],
					phases.size(),
					sizeof(struct phase *), phase_compare_string);
	}
	if (void_ptr == NULL && print == TRUE)
	{
		error_string = sformatf( "Could not find phase in list, %s.", cptr);
		error_msg(error_string, CONTINUE);
	}

	if (void_ptr == NULL)
	{
		*j = -1;
		return (NULL);
	}

	*j = (int) ((struct phase **) void_ptr - &phases[0]);
	return (*(struct phase **) void_ptr);
}

/* ---------------------------------------------------------------------- */
 int Phreeqc::
phase_init(struct phase *phase_ptr)
/* ---------------------------------------------------------------------- */
/*
 *   set pointers in phase structure to NULL
 */
{
	int i;

	phase_ptr->name = NULL;
	phase_ptr->formula = NULL;
	phase_ptr->in = FALSE;
	phase_ptr->lk = 0.0;
	for (i = 0; i < MAX_LOG_K_INDICES; i++)
		phase_ptr->logk[i] = 0.0;
	phase_ptr->original_units = kjoules;
	phase_ptr->add_logk.clear();
	phase_ptr->moles_x = 0;
	phase_ptr->delta_max = 0;
	phase_ptr->p_soln_x = 0;
	phase_ptr->fraction_x = 0;
	phase_ptr->log10_lambda = 0;
	phase_ptr->log10_fraction_x = 0;
	phase_ptr->dn = 0;
	phase_ptr->dnb = 0;
	phase_ptr->dnc = 0;
	phase_ptr->gn = 0;
	phase_ptr->gntot = 0;
	phase_ptr->t_c = 0.0;
	phase_ptr->p_c = 0.0;
	phase_ptr->omega = 0.0;
	phase_ptr->pr_a = 0.0;
	phase_ptr->pr_b = 0.0;
	phase_ptr->pr_alpha = 0.0;
	phase_ptr->pr_tk = 0;
	phase_ptr->pr_p = 0;
	phase_ptr->pr_phi = 1.0;
	phase_ptr->pr_aa_sum2 = 0;
	for (i = 0; i < 9; i++)
		phase_ptr->delta_v[i] = 0.0;
	phase_ptr->pr_si_f = 0;
	phase_ptr->pr_in = false;
	phase_ptr->type = SOLID;
	phase_ptr->next_elt = NULL;
	phase_ptr->next_sys_total = NULL;
	phase_ptr->check_equation = TRUE;
	phase_ptr->replaced = 0;
	phase_ptr->in_system = 1;
	phase_ptr->original_deltav_units = cm3_per_mol;
	return (OK);
}

/* ---------------------------------------------------------------------- */
struct phase * Phreeqc::
phase_store(const char *name_in)
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
	struct phase *phase_ptr = NULL;
/*
 *   Search list
 */
	std::string name = name_in;
	str_tolower(name);
	std::map<std::string, struct phase*>::iterator p_it =
		phases_map.find(name);
	if (p_it != phases_map.end())
	{
		phase_ptr = p_it->second;
		phase_free(phase_ptr);
		phase_init(phase_ptr);
		phase_ptr->name = string_hsave(name_in);
		return (phase_ptr);
	}
/*
 *   Make new phase structure and return pointer to it
 */
	size_t n = phases.size();
	phases.resize(n + 1);
	phases[n] = phase_alloc();
	/* set name in phase structure */
	phases[n]->name = string_hsave(name_in);
/*
 *   Update map
 */
	phases_map[name] = phases[n];
	return (phases[n]);
}
/* **********************************************************************
 *
 *   Routines related to structure "rates"
 *
 * ********************************************************************** */
/* ---------------------------------------------------------------------- */
struct rate * Phreeqc::
rate_bsearch(const char* cptr, int *j)
/* ---------------------------------------------------------------------- */
{
/*   Binary search the structure array "rates" for a name that is equal to
 *   cptr. Assumes array rates is in sort order.
 *
 *   Arguments:
 *      name  input, a character string to be located in rates.
 *      j	    index number in array rates.
 *
 *   Returns:
 *      if found, pointer to rate structure.
 *      if not found, NULL
 *
 */
	void *void_ptr;

	if (rates.size() == 0)
	{
		*j = -1;
		return (NULL);
	}
	void_ptr = (void *)
		bsearch((char *) cptr,
				(char *) &rates[0],
				rates.size(),
				sizeof(struct rate *), rate_compare_string);

	if (void_ptr == NULL)
	{
		*j = -1;
		return (NULL);
	}

	*j = (int) ((struct rate *) void_ptr - &rates[0]);
	return ((struct rate *) void_ptr);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
rate_compare(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
/*
 *   Compares names of rates for sort
 */
	const struct rate *rate_ptr1, *rate_ptr2;
	rate_ptr1 = *(const struct rate **) ptr1;
	rate_ptr2 = *(const struct rate **) ptr2;
	return (strcmp_nocase(rate_ptr1->name, rate_ptr2->name));
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
rate_compare_string(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
	const char *char_ptr;
	const struct rate *rate_ptr;
	char_ptr = (const char *) ptr1;
	rate_ptr = *(const struct rate **) ptr2;
	return (strcmp_nocase(char_ptr, rate_ptr->name));
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
rate_free(struct rate *rate_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Frees memory allocated within rate[i], does not free rate structure
 *   Input: i, number of rate
 *   Return: OK
 */
	

	if (rate_ptr == NULL)
		return (ERROR);
	rate_ptr->commands.clear();
	if (rate_ptr->linebase != NULL)
	{
		char cmd[] = "new; quit";
		basic_run(cmd, rate_ptr->linebase, rate_ptr->varbase, rate_ptr->loopbase);
		rate_ptr->linebase = NULL;
		rate_ptr->varbase = NULL;
		rate_ptr->loopbase = NULL;
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
struct rate * Phreeqc::
rate_copy(struct rate *rate_ptr)
/* ---------------------------------------------------------------------- */
{
	/*
	*   Copies a rate to new allocated space
	*/
	if (rate_ptr == NULL)
		return (NULL);
	struct rate* rate_new = new struct rate;
	rate_new->commands = rate_ptr->commands;
	rate_new->new_def = TRUE;
	rate_new->linebase = NULL;
	rate_new->varbase = NULL;
	rate_new->loopbase = NULL;
	return (rate_new);
}

/* ---------------------------------------------------------------------- */
struct rate * Phreeqc::
rate_search(const char *name_in, int *n)
/* ---------------------------------------------------------------------- */
{
/*   Linear search of the structure array "rates" for name.
 *
 *   Arguments:
 *     name     input, name of rate
 *      n       output, position in rates
 *
 *   Returns:
 *      if found, the address of the pp_assemblage element
 *      if not found, NULL
 */
	std::map<const char *, int>::iterator it;

	const char * name;
	name = string_hsave(name_in);

	it = rates_map.find(name);
	if (it != rates_map.end())
	{
		*n = it->second;
		if (*n >= 0)
		{
			return &(rates[it->second]);
		}
		return NULL;
	}

	int i;
	*n = -1;
	for (i = 0; i < (int)rates.size(); i++)
	{
		if (strcmp_nocase(rates[i].name, name) == 0)
		{
			*n = i;
			rates_map[name] = i;
			return (&(rates[i]));
		}
	}
/*
 *   rate name not found
 */
	rates_map[name] = *n;
	return (NULL);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
rate_sort(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Sort array of rate structures
 */
	if (rates.size() > 1)
	{
		qsort(&rates[0], rates.size(), sizeof(struct rate),
			  rate_compare);
	}
	return (OK);
}
/* **********************************************************************
 *
 *   Routines related to structure "species"
 *
 * ********************************************************************** */
/* ---------------------------------------------------------------------- */
struct species * Phreeqc::
s_alloc(void)
/* ---------------------------------------------------------------------- */
/*
 *   Allocates space to a species structure, initializes
 *      arguments: void
 *      return: pointer to a species structure
 */
{
	struct species *s_ptr;
	s_ptr = new struct species;
/*
 *   set pointers in structure to NULL, variables to zero
 */
	s_init(s_ptr);

	return (s_ptr);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
s_compare(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
	const struct species *s_ptr1, *s_ptr2;
	s_ptr1 = *(const struct species **) ptr1;
	s_ptr2 = *(const struct species **) ptr2;
	return (strcmp(s_ptr1->name, s_ptr2->name));

}

/* ---------------------------------------------------------------------- */
int Phreeqc::
s_delete(int i)
/* ---------------------------------------------------------------------- */
{
/*
 *   Delete species i: free memory and renumber array of pointers, s.
 */
	s_free(s[i]);
	s[i] = (struct species *) free_check_null(s[i]);
	s.erase(s.begin() + i);
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
s_free(struct species *s_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Free space allocated for species structure, s_ptr. Does not free s_ptr.
 */
	if (s_ptr == NULL)
		return (ERROR);
	s_ptr->next_elt = (struct elt_list *) free_check_null(s_ptr->next_elt);
	s_ptr->next_secondary =
		(struct elt_list *) free_check_null(s_ptr->next_secondary);
	s_ptr->next_sys_total =
		(struct elt_list *) free_check_null(s_ptr->next_sys_total);
	s_ptr->add_logk.clear();
	//rxn_free(s_ptr->rxn);
	//rxn_free(s_ptr->rxn_s);
	//rxn_free(s_ptr->rxn_x);
	return (OK);
}

/* ---------------------------------------------------------------------- */
 int Phreeqc::
s_init(struct species *s_ptr)
/* ---------------------------------------------------------------------- */
/*
 *      return: pointer to a species structure
 */
{
	int i;
/*
 *   set pointers in structure to NULL
 */
	s_ptr->name = NULL;
	s_ptr->mole_balance = NULL;
	s_ptr->in = FALSE;
	s_ptr->number = 0;
	s_ptr->primary = NULL;
	s_ptr->secondary = NULL;
	s_ptr->gfw = 0.0;
	s_ptr->z = 0.0;
	s_ptr->dw = 0.0;
	s_ptr->dw_t = 0.0;
	s_ptr->dw_a = 0.0;
	s_ptr->dw_a2 = 0.0;
	s_ptr->erm_ddl = 1.0;
	s_ptr->equiv = 0;
	s_ptr->alk = 0.0;
	s_ptr->carbon = 0.0;
	s_ptr->co2 = 0.0;
	s_ptr->h = 0.0;
	s_ptr->o = 0.0;
	s_ptr->dha = 0.0;
	s_ptr->dhb = 0.0;
	s_ptr->a_f = 0.0;
	s_ptr->lk = 0.0;
	for (i = 0; i < MAX_LOG_K_INDICES; i++)
	{
		s_ptr->logk[i] = 0.0;
	}
	for (i = 0; i < 10; i++)
	{
		s_ptr->Jones_Dole[i] = 0.0;
	}
/* VP: Density Start */
	for (i = 0; i < 6; i++)
	{
		s_ptr->millero[i] = 0.0;
	}
/* VP: Density End */
	s_ptr->original_units = kjoules;
	s_ptr->add_logk.clear();
	s_ptr->lg = 0.0;
	s_ptr->lg_pitzer = 0.0;
	s_ptr->lm = 0.0;
	s_ptr->la = 0.0;
	s_ptr->dg = 0.0;
	s_ptr->dg_total_g = 0;
	s_ptr->moles = 0.0;
	s_ptr->type = 0;
	s_ptr->gflag = 0;
	s_ptr->exch_gflag = 0;
	s_ptr->next_elt = NULL;
	s_ptr->next_secondary = NULL;
	s_ptr->next_sys_total = NULL;
	s_ptr->check_equation = TRUE;
	s_ptr->tot_g_moles = 0;
	s_ptr->tot_dh2o_moles = 0;
	for (i = 0; i < 5; i++)
	{
		s_ptr->cd_music[i] = 0.0;
	}
	for (i = 0; i < 3; i++)
	{
		s_ptr->dz[i] = 0.0;
	}
	s_ptr->original_deltav_units = cm3_per_mol;
	return (OK);
}
/* ---------------------------------------------------------------------- */
struct species* Phreeqc::
s_search(const char* name)
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
	struct species* s_ptr = NULL;
	std::map<std::string, struct species*>::iterator s_it = 
		species_map.find(name);
	if (s_it != species_map.end())
	{
		s_ptr = s_it->second;
	}
	return (s_ptr);
}
/* ---------------------------------------------------------------------- */
struct species * Phreeqc::
s_store(const char *name, LDBLE l_z, int replace_if_found)
/* ---------------------------------------------------------------------- */
{
/*
 *   Function locates the string "name" in the map for species.
 *
 *   Pointer to a species structure is always returned.
 *
 *   If the string is not found, a new entry is made at the end of
 *      the elements array (position count_elements) and count_elements is
 *      incremented. A new entry is made in the map. Pointer to
 *      the new structure is returned.
 *   If "name" is found and replace is true, pointers in old species structure
 *      are freed and replaced with additional input.
 *   If "name" is found and replace is false, the old species structure is not
 *      modified and a pointer to it is returned.
 *
 *   Arguments:
 *      name    input, character string to be found in "species".
 *      l_z      input, charge on "name"
 *      replace_if_found input, TRUE means reinitialize species if found
 *		     FALSE means just return pointer if found.
 *
 *   Returns:
 *      pointer to species structure "s" where "name" can be found.
 */

/*
 *   Search list
 */
	struct species* s_ptr = NULL;
	s_ptr = s_search(name);
	if (s_ptr != NULL && replace_if_found == FALSE)
	{
		return (s_ptr);
	}
	else if (s_ptr != NULL && replace_if_found == TRUE)
	{
		s_free(s_ptr);
		s_init(s_ptr);
	}
	else
	{
		size_t n = s.size();
		s.resize(n + 1);
		/* Make new species structure */
		s[n] = s_alloc();
		s_ptr = s[n];
	}
	/* set name and z in pointer in species structure */
	s_ptr->name = string_hsave(name);
	s_ptr->z = l_z;
/*
 *   Update map
 */
	species_map[name] = s_ptr;
	return (s_ptr);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
isotope_compare(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
	int i;
	const struct isotope *iso_ptr1, *iso_ptr2;

	iso_ptr1 = (const struct isotope *) ptr1;
	iso_ptr2 = (const struct isotope *) ptr2;
	i = strcmp_nocase(iso_ptr1->elt_name, iso_ptr2->elt_name);
	if (i != 0)
		return (i);
	if (iso_ptr1->isotope_number < iso_ptr2->isotope_number)
	{
		return (-1);
	}
	else if (iso_ptr1->isotope_number > iso_ptr2->isotope_number)
	{
		return (1);
	}
	return (0);
}
/* **********************************************************************
 *
 *   Routines related to structure "species_list"
 *
 * ********************************************************************** */
/* ---------------------------------------------------------------------- */
 int Phreeqc::
species_list_compare(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
	int j;
	const char *name1, *name2;
	const struct species_list *nptr1, *nptr2;

	nptr1 = (const struct species_list *) ptr1;
	nptr2 = (const struct species_list *) ptr2;

/*
 *   Put H+ first
 */
	if (nptr1->master_s != nptr2->master_s)
	{
		/*
		if (nptr1->master_s == s_hplus)
			return (-1);
		if (nptr2->master_s == s_hplus)
			return (1);
		*/
		if ((strcmp(nptr1->master_s->name,"H+") == 0) || (strcmp(nptr1->master_s->name,"H3O+") == 0))
			return (-1);
		if ((strcmp(nptr2->master_s->name,"H+") == 0) || (strcmp(nptr2->master_s->name,"H3O+") == 0))
			return (1);
	}
/*
 *   Other element valence states
 */
	if (nptr1->master_s->secondary != NULL)
	{
		name1 = nptr1->master_s->secondary->elt->name;
	}
	else
	{
		name1 = nptr1->master_s->primary->elt->name;
	}
	if (nptr2->master_s->secondary != NULL)
	{
		name2 = nptr2->master_s->secondary->elt->name;
	}
	else
	{
		name2 = nptr2->master_s->primary->elt->name;
	}
/*
 *   Compare name of primary or secondary master species; log molality
 */

	j = strcmp(name1, name2);

/*
 *   Different master species
 */
	if (j != 0)
		return (j);

/*
 *   Else, descending order by log molality
 */
	if (nptr1->s->lm > nptr2->s->lm)
	{
		return (-1);
	}
	else if (nptr1->s->lm < nptr2->s->lm)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
species_list_compare_alk(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
	const struct species_list *nptr1, *nptr2;
	LDBLE alk1, alk2;

	nptr1 = (const struct species_list *) ptr1;
	nptr2 = (const struct species_list *) ptr2;
/*
 *   Else, descending order by log molality
 */
	alk1 = fabs(under(nptr1->s->lm) * nptr1->s->alk);
	alk2 = fabs(under(nptr2->s->lm) * nptr2->s->alk);

	if (alk1 > alk2)
	{
		return (-1);
	}
	else if (alk1 < alk2)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
species_list_compare_master(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
	const char *name1, *name2;
	const struct species_list *nptr1, *nptr2;

	nptr1 = (const struct species_list *) ptr1;
	nptr2 = (const struct species_list *) ptr2;

/*
 *   Put H+ first
 */
	if (nptr1->master_s != nptr2->master_s)
	{
		/*
		if (nptr1->master_s == s_hplus)
			return (-1);
		if (nptr2->master_s == s_hplus)
			return (1);
		*/
		if ((strcmp(nptr1->master_s->name,"H+") == 0) || (strcmp(nptr1->master_s->name,"H3O+") == 0))
			return (-1);
		if ((strcmp(nptr2->master_s->name,"H+") == 0) || (strcmp(nptr2->master_s->name,"H3O+") == 0))
			return (1);
	}
/*
 *   Other element valence states
 */
	if (nptr1->master_s->secondary != NULL)
	{
		name1 = nptr1->master_s->secondary->elt->name;
	}
	else
	{
		name1 = nptr1->master_s->primary->elt->name;
	}
	if (nptr2->master_s->secondary != NULL)
	{
		name2 = nptr2->master_s->secondary->elt->name;
	}
	else
	{
		name2 = nptr2->master_s->primary->elt->name;
	}
/*
 *   Compare name of primary or secondary master species; log molality
 */

	return (strcmp(name1, name2));
}


/* ---------------------------------------------------------------------- */
int Phreeqc::
species_list_sort(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Sort list using rules in species_list_compare
 */
	if (species_list.size() > 1)
	{
		qsort(&species_list[0], species_list.size(),
			  sizeof(struct species_list), species_list_compare);
	}
	return (OK);
}

/* **********************************************************************
 *
 *   Routines related to structure "surface"
 *
 * ********************************************************************** */
/* ---------------------------------------------------------------------- */
struct Change_Surf * Phreeqc::
change_surf_alloc(int count)
/* ---------------------------------------------------------------------- */
{
	if (count == 1)
		return (change_surf);
	change_surf =
		(struct Change_Surf *) PHRQ_realloc(change_surf,
											(size_t) count *
											sizeof(struct Change_Surf));
	if (change_surf == NULL)
		malloc_error();
	change_surf[count - 1].cell_no = -99;
	change_surf[count - 1].next = FALSE;
	change_surf[count - 2].next = TRUE;

	return (change_surf);
}
/* ---------------------------------------------------------------------- */
struct master * Phreeqc::
surface_get_psi_master(const char *name, int plane)
/* ---------------------------------------------------------------------- */
{
	struct master *master_ptr;
	std::string token;

	if (name == NULL)
		return (NULL);
	token = name;
	token.append("_psi");
	switch (plane)
	{
	case SURF_PSI:
		break;
	case SURF_PSI1:
		token.append("b");
		break;
	case SURF_PSI2:
		token.append("d");
		break;
	default:
		error_msg("Unknown plane for surface_get_psi_master", STOP);
	}
	master_ptr = master_bsearch(token.c_str());
	return (master_ptr);
}
/* **********************************************************************
 *
 *   Routines related to structure "trxn"
 *
 * ********************************************************************** */
/* ---------------------------------------------------------------------- */
int Phreeqc::
rxn_token_temp_compare(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
	const struct rxn_token_temp *rxn_token_temp_ptr1, *rxn_token_temp_ptr2;
	rxn_token_temp_ptr1 = (const struct rxn_token_temp *) ptr1;
	rxn_token_temp_ptr2 = (const struct rxn_token_temp *) ptr2;
	return (strcmp(rxn_token_temp_ptr1->name, rxn_token_temp_ptr2->name));
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
trxn_add(cxxChemRxn &r_ptr, LDBLE coef, int combine)
/* ---------------------------------------------------------------------- */
{
/*
 *   Adds reactions together.
 *
 *   Global variable count_trxn determines which position in trxn is used.
 *      If count_trxn=0, then the equation effectively is copied into trxn.
 *      If count_trxn>0, then new equation is added to existing equation.
 *
 *   Arguments:
 *      *r_ptr	 points to rxn structure to add.
 *
 *       coef	  added equation is multiplied by coef.
 *       combine       if TRUE, reaction is reaction is sorted and
 *		     like terms combined.
 */
/*
 *   Accumulate log k for reaction
 */
	if (count_trxn == 0)
	{
		for (int i = 0; i < MAX_LOG_K_INDICES; i++)
		{
			trxn.logk[i] = r_ptr.Get_logk()[i];
		}
		for (int i = 0; i < 3; i++)
		{
			trxn.dz[i] = r_ptr.Get_dz()[i];
		}
	}
	else
	{
		for (int i = 0; i < MAX_LOG_K_INDICES; i++)
		{
			trxn.logk[i] += coef * (r_ptr.Get_logk()[i]);
		}
		for (int i = 0; i < 3; i++)
		{
			trxn.dz[i] += coef * r_ptr.Get_dz()[i];
		}
	}
/*
 *   Copy  equation into work space
 */
	for (size_t j = 0; j < r_ptr.Get_tokens().size(); j++)
	{
		if (count_trxn + 1 > trxn.token.size())
			trxn.token.resize(count_trxn + 1);
		trxn.token[count_trxn].name = r_ptr.Get_tokens()[j].name;
		trxn.token[count_trxn].s = r_ptr.Get_tokens()[j].s;
		trxn.token[count_trxn].coef = coef * r_ptr.Get_tokens()[j].coef;
		count_trxn++;
	}
	if (combine == TRUE)
		trxn_combine();
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
trxn_combine(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Combines coefficients of tokens that are equal in temporary
 *   reaction structure, trxn.
 */
	int j, k;
/*
 *   Sort trxn species
 */
	trxn_sort();
/*
 *   Combine trxn tokens
 */
	j = 1;
	for (k = 2; k < count_trxn; k++)
	{
		if (trxn.token[k].s != NULL)
		{
			if ((j > 0) && (trxn.token[k].s == trxn.token[j].s))
			{
				trxn.token[j].coef += trxn.token[k].coef;
				if (equal(trxn.token[j].coef, 0.0, 1e-5))
					j--;
			}
			else
			{
				j++;
				if (k != j)
				{
					trxn.token[j].name = trxn.token[k].name;
					trxn.token[j].s = trxn.token[k].s;
					trxn.token[j].coef = trxn.token[k].coef;
				}
			}
		}
		else
		{
			if ((j > 0) && (trxn.token[k].s == trxn.token[j].s)
				&& (trxn.token[k].name == trxn.token[j].name))
			{
				trxn.token[j].coef += trxn.token[k].coef;
				if (equal(trxn.token[j].coef, 0.0, 1e-5))
					j--;
			}
			else
			{
				j++;
				if (k != j)
				{
					trxn.token[j].name = trxn.token[k].name;
					trxn.token[j].s = trxn.token[k].s;
					trxn.token[j].coef = trxn.token[k].coef;
				}
			}
		}
	}
	count_trxn = j + 1;			/* number excluding final NULL */
	return (OK);
}
/* ---------------------------------------------------------------------- */
LDBLE Phreeqc::
trxn_find_coef(const char *str, int start)
/* ---------------------------------------------------------------------- */
{
/*
 *   Finds coefficient of specified token in trxn.
 *   Input: str, token name in reaction.
 *
 *   Return: 0.0, if token not found.
 *	   coefficient of token, if token found.
 */
	int i;
	LDBLE coef;

	coef = 0.0;
	for (i = start; i < count_trxn; i++)
	{
		if (strcmp(trxn.token[i].s->name, str) == 0)
		{
			coef = trxn.token[i].coef;
			break;
		}
	}
	return (coef);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
trxn_multiply(LDBLE coef)
/* ---------------------------------------------------------------------- */
{
/*
 *   Multiplies temporary reaction, trxn,  by a constant
 *
 *   Arguments:
 *       input: coef	  multiplier.
 */
	int i;
/*
 *   Multiply log k for reaction
 */
	for (i = 0; i < MAX_LOG_K_INDICES; i++)
	{
		trxn.logk[i] *= coef;
	}
/*
 *   Multiply dz for reaction
 */
	for (i = 0; i < 3; i++)
	{
		trxn.dz[i] *= coef;
	}
/*
 *   Multiply coefficients of reaction
 */
	for (i = 0; i < count_trxn; i++)
	{
		trxn.token[i].coef *= coef;
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
trxn_print(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Prints trxn
 */
	int i;
/*
 *   Print log k for reaction
 */

	output_msg(sformatf( "\tlog k data:\n"));
	for (i = 0; i < MAX_LOG_K_INDICES; i++)
	{
		output_msg(sformatf( "\t\t%f\n", (double) trxn.logk[i]));
	}

/*
 *   Print dz for reaction
 */
	output_msg(sformatf( "\tdz data:\n"));
	for (i = 0; i < 3; i++)
	{
		output_msg(sformatf( "\t\t%f\n", (double) trxn.dz[i]));
	}
/*
 *   Print stoichiometry
 */
	output_msg(sformatf( "\tReaction stoichiometry\n"));
	for (i = 0; i < count_trxn; i++)
	{
		output_msg(sformatf( "\t\t%-20s\t%10.2f\n", trxn.token[i].name,
				   (double) trxn.token[i].coef));
	}
	output_msg(sformatf( "\n"));
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
trxn_reverse_k(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Changes K from dissociation to association and back
 */
	int i;
/*
 *   Accumulate log k for reaction
 */
   for (i = 0; i < MAX_LOG_K_INDICES; i++)
	{
		trxn.logk[i] = -trxn.logk[i];
	}
	for (i = 0; i < 3; i++)
	{
		trxn.dz[i] = -trxn.dz[i];
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
trxn_sort(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Compare names in tokens in trxn array for sorting
 */
	if (count_trxn - 1 > 1)
	{
		qsort(&trxn.token[1],
			(size_t)count_trxn - 1,
			sizeof(struct rxn_token_temp),
			rxn_token_temp_compare);
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
trxn_swap(const char *token)
/* ---------------------------------------------------------------------- */
{
/*
 *   Moves specified token to initial position in reaction.
 *   Input: token, token name to move to initial position.
 *
 *   Return: ERROR, if token not found.
 *	   OK, if token moved to initial position.
 */
	int i, j;
	LDBLE coef;
/*
 *   Locate token
 */
	for (j = 0; j < count_trxn; j++)
	{
		if (strcmp(trxn.token[j].s->name, token) == 0)
			break;
	}
	if (j >= count_trxn)
	{
		input_error++;
		error_string = sformatf( "Could not find token in equation, %s.", token);
		error_msg(error_string, CONTINUE);
		for (i = 0; i < count_trxn; i++)
		{
			output_msg(sformatf( "%f\t%s\t",
					   (double) trxn.token[i].coef, trxn.token[i].name));
		}
		output_msg(sformatf( "\n"));
		return (ERROR);
	}
/*
 *   Swap token to first position
 */
	trxn.token[count_trxn].name = trxn.token[0].name;
	trxn.token[count_trxn].s = trxn.token[0].s;
	trxn.token[count_trxn].coef = trxn.token[0].coef;

	trxn.token[0].name = trxn.token[j].name;
	trxn.token[0].s = trxn.token[j].s;
	trxn.token[0].coef = trxn.token[j].coef;

	trxn.token[j].name = trxn.token[count_trxn].name;
	trxn.token[j].s = trxn.token[count_trxn].s;
	trxn.token[j].coef = trxn.token[count_trxn].coef;
/*
 *   Make coefficient of token -1.0
 */
	coef = -1.0 / trxn.token[0].coef;
	trxn_multiply(coef);
	return (OK);
}

/* **********************************************************************
 *
 *   Routines related to structure "unknown"
 *
 * ********************************************************************** */
/* ---------------------------------------------------------------------- */
struct unknown * Phreeqc::
unknown_alloc(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Allocates space to an "unknown" structure
 *      arguments: void
 *      return: pointer to an "unknown" structure
 */
	struct unknown *unknown_ptr;
/*
 *   Allocate space
 */
	unknown_ptr = new struct unknown;
/*
 *   set pointers in structure to NULL
 */
	unknown_ptr->type = 0;
	unknown_ptr->moles = 0.0;
	unknown_ptr->ln_moles = 0.0;
	unknown_ptr->f = 0.0;
	unknown_ptr->sum = 0.0;
	unknown_ptr->delta = 0.0;
	unknown_ptr->la = 0.0;
	unknown_ptr->number = 0;
	unknown_ptr->description = NULL;
	unknown_ptr->phase = NULL;
	unknown_ptr->si = 0.0;
	unknown_ptr->s = NULL;
	unknown_ptr->exch_comp = NULL;
	unknown_ptr->pp_assemblage_comp_name = NULL;
	unknown_ptr->pp_assemblage_comp_ptr = NULL;
	unknown_ptr->ss_name = NULL;
	unknown_ptr->ss_ptr = NULL;
	unknown_ptr->ss_comp_name = NULL;
	unknown_ptr->ss_comp_ptr = NULL;
	unknown_ptr->ss_comp_number = 0;
	unknown_ptr->ss_in = FALSE;
	unknown_ptr->surface_comp = NULL;
	unknown_ptr->related_moles = 0.0;
	unknown_ptr->potential_unknown = NULL;
	unknown_ptr->potential_unknown1 = NULL;
	unknown_ptr->potential_unknown2 = NULL;
	unknown_ptr->phase_unknown = NULL;
	unknown_ptr->surface_charge = NULL;
	unknown_ptr->mass_water = 0.0;
	unknown_ptr->dissolve_only = FALSE;
	unknown_ptr->inert_moles = 0.0;
	unknown_ptr->V_m = 0.0;
	unknown_ptr->pressure = 0.0;
	unknown_ptr->mb_number = 0;
	unknown_ptr->iteration = 0;

	return (unknown_ptr);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
unknown_delete(int i)
/* ---------------------------------------------------------------------- */
{
/*
 *   Delete unknow from list x
 */
	unknown_free(x[i]);
	x.erase(x.begin() + (size_t)i);
	count_unknowns--;
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
unknown_free(struct unknown *unknown_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Frees space allocated to an unknown structure, frees unknown_ptr.
 */
	if (unknown_ptr == NULL)
		return (ERROR);
	unknown_ptr->master.clear();
	if (unknown_ptr->type == SURFACE_CB)
	{
		/*
		   surface_charge_free(unknown_ptr->surface_charge);
		   unknown_ptr->surface_charge = (struct surface_charge *) free_check_null(unknown_ptr->surface_charge);
		 */
	}
	unknown_ptr->comp_unknowns.clear();
	delete unknown_ptr;
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
system_duplicate(int i, int save_old)
/* ---------------------------------------------------------------------- */
{
	Utilities::Rxn_copy(Rxn_solution_map, i, save_old);

	Utilities::Rxn_copy(Rxn_pp_assemblage_map, i, save_old);

	Utilities::Rxn_copy(Rxn_exchange_map, i, save_old);

	Utilities::Rxn_copy(Rxn_surface_map, i, save_old);

	Utilities::Rxn_copy(Rxn_gas_phase_map, i, save_old);

	Utilities::Rxn_copy(Rxn_kinetics_map, i, save_old);

	Utilities::Rxn_copy(Rxn_ss_assemblage_map, i, save_old);

	return (OK);
}

/* ---------------------------------------------------------------------- */
struct logk * Phreeqc::
logk_store(const char *name_in, int replace_if_found)
/* ---------------------------------------------------------------------- */
{
/*
 *   Function locates the string "name" in the map for logk.
 *
 *   Pointer to a logk structure is always returned.
 *
 *   If the string is not found, a new entry is made in the map. Pointer to
 *      the new structure is returned.
 *   If "name" is found and replace is true, pointers in old logk structure
 *      are freed and replaced with additional input.
 *   If "name" is found and replace is false, the old logk structure is not
 *      modified and a pointer to it is returned.
 *
 *   Arguments:
 *      name    input, character string to be found in "logk".
 *      replace_if_found input, TRUE means reinitialize logk structure if found
 *		     FALSE means just return pointer if found.
 *
 *   Returns:
 *      pointer to logk structure "logk" where "name" can be found.
 */
/*
 *   Search list
 */
	struct logk* logk_ptr = NULL;
	std::string name = name_in;
	str_tolower(name);
	std::map<std::string, struct logk*>::iterator it =
		logk_map.find(name);

	if (it != logk_map.end() && replace_if_found == FALSE)
	{
		logk_ptr = it->second;
		return (logk_ptr);
	}
	else if (it != logk_map.end() && replace_if_found == TRUE)
	{
		logk_ptr = it->second;
		logk_init(logk_ptr);
	}
	else
	{
		/* Make new logk structure */
		size_t n = logk.size();
		logk.resize(n + 1);
		logk[n] = logk_alloc();
		logk_ptr = logk[n];
	}
	/* set name and z in pointer in logk structure */
	logk_ptr->name = string_hsave(name_in);
/*
 *   Update map
 */
	logk_map[name] = logk_ptr;
	return (logk_ptr);
}

/* ---------------------------------------------------------------------- */
struct logk * Phreeqc::
logk_alloc(void)
/* ---------------------------------------------------------------------- */
/*
 *   Allocates space to a logk structure, initializes
 *      arguments: void
 *      return: pointer to a logk structure
 */
{
	struct logk *logk_ptr;
	logk_ptr = new struct logk;
/*
 *   set pointers in structure to NULL, variables to zero
 */
	logk_init(logk_ptr);

	return (logk_ptr);
}

/* ---------------------------------------------------------------------- */
 int Phreeqc::
logk_init(struct logk *logk_ptr)
/* ---------------------------------------------------------------------- */
/*
 *      return: pointer to a logk structure
 */
{
	int i;
/*
 *   set pointers in structure to NULL
 */
	logk_ptr->name = NULL;
/*
 *   set varibles = 0
 */
	logk_ptr->lk = 0.0;
	for (i = 0; i < MAX_LOG_K_INDICES; i++)
	{
		logk_ptr->log_k[i] = 0.0;
		logk_ptr->log_k_original[i] = 0.0;
	}
	logk_ptr->add_logk.clear(); 
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
logk_copy2orig(struct logk *logk_ptr)
/* ---------------------------------------------------------------------- */
/*
 *   Copies log k data to logk_original
 */
{
	int i;
	for (i = 0; i < MAX_LOG_K_INDICES; i++)
	{
		logk_ptr->log_k_original[i] = logk_ptr->log_k[i];
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
struct logk * Phreeqc::
logk_search(const char *name_in)
/* ---------------------------------------------------------------------- */
{
/*
 *   Function locates the string "name" in the map for logk.
 *
 *   Arguments:
 *      name    input, character string to be found in "logk".
 *
 *   Returns:
 *      pointer to logk structure "logk" where "name" can be found.
 *      or NULL if not found.
 */
	struct logk *logk_ptr;
/*
 *   Search list
 */
	std::string name = name_in;
	str_tolower(name);
	std::map<std::string, struct logk*>::iterator l_it =
		logk_map.find(name);
	if (l_it != logk_map.end())
	{
		logk_ptr = l_it->second;
		return (logk_ptr);
	}
	return (NULL);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
entity_exists(const char *name, int n_user)
/* ---------------------------------------------------------------------- */
{
/*
 *   Reads solution,		   0 Solution
 *	 reaction,		   1 Reaction
 *	 exchange,		   2 Exchange
 *	 surface,		    3 Surface
 *	 gas_phase,		  4 Gas_phase
 *	 equilibrium_phases,	 5 Pure_phase
 *	 solid_solution,	     6 Ss_phase
 *	 kinetics,		   7 Kinetics
 *	 mix,			8 Mix
 *	 reaction_temperature	9 Temperature
 *	 unknown		     10 UnKnown
 */
	int return_value;
	char token[MAX_LENGTH];
	enum entity_type type;
/*
 *   Read keyword
 */
	strncpy(token, name, MAX_LENGTH-1);
	token[MAX_LENGTH-1] = '\0';
	type = get_entity_enum(token);
	return_value = TRUE;
	switch (type)
	{
	case UnKnown:
		warning_msg
			("EXISTS expecting keyword solution, mix, kinetics, reaction, reaction_temperature, equilibrium_phases, exchange, surface, gas_phase, or solid_solutions.");
		return_value = 2;
		break;
	case Solution:				/* Solution */
		if (Utilities::Rxn_find(Rxn_solution_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
		break;
	case Pure_phase:			/* Pure phases */
		if (Utilities::Rxn_find(Rxn_pp_assemblage_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
		break;
	case Reaction:				/* Reaction */
		if (Utilities::Rxn_find(Rxn_reaction_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
		break;
	case Mix:					/* Mix */
		if (Utilities::Rxn_find(Rxn_mix_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
		break;
	case Exchange:				/* Ex */
		if (Utilities::Rxn_find(Rxn_exchange_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
		break;
	case Surface:				/* Surface */
		if (Utilities::Rxn_find(Rxn_surface_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
		break;
	case Temperature:
		if (Utilities::Rxn_find(Rxn_temperature_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
	case Pressure:
		if (Utilities::Rxn_find(Rxn_pressure_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
	case Gas_phase:			/* Gas */
		if (Utilities::Rxn_find(Rxn_gas_phase_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
		break;
	case Kinetics:				/* Kinetics */
		if (Utilities::Rxn_find(Rxn_kinetics_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
		break;
	case Ss_phase:				/* solid_solutions */
		if (Utilities::Rxn_find(Rxn_ss_assemblage_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
		break;
	}
	return (return_value);
}

/* ---------------------------------------------------------------------- */
enum entity_type Phreeqc::
get_entity_enum(char *name)
/* ---------------------------------------------------------------------- */
{
/*
 *   Reads solution,		   0 Solution
 *	 reaction,		   1 Reaction
 *	 exchange,		   2 Exchange
 *	 surface,		    3 Surface
 *	 gas_phase,		  4 Gas_phase
 *	 equilibrium_phases,	 5 Pure_phase
 *	 solid_solution,	     6 Ss_phase
 *	 kinetics,		   7 Kinetics
 *	 mix,			8 Mix
 *	 reaction_temperature	9 Temperature
 *   reaction_pressure
 *	 unknown		     10 UnKnown
 *
 */
	int i;
	const char* cptr;
	char token[MAX_LENGTH];
/*
 *   Read keyword
 */
	cptr = name;
	copy_token(token, &cptr, &i);
	check_key(token);

	switch (next_keyword)
	{
	case Keywords::KEY_SOLUTION:					/* Solution */
		return (Solution);
		break;
	case Keywords::KEY_EQUILIBRIUM_PHASES:		/* Pure phases */
		return (Pure_phase);
		break;
	case Keywords::KEY_REACTION:					/* Reaction */
		return (Reaction);
		break;
	case Keywords::KEY_MIX:						/* Mix */
		return (Mix);
		break;
	case Keywords::KEY_EXCHANGE:					/* Ex */
		return (Exchange);
		break;
	case Keywords::KEY_SURFACE:					/* Surface */
		return (Surface);
		break;
	case Keywords::KEY_REACTION_TEMPERATURE:		/* Temperature */
		return (Temperature);
		break;
	case Keywords::KEY_REACTION_PRESSURE:		/* Pressure */
		return (Pressure);
		break;
	case Keywords::KEY_GAS_PHASE:					/* Gas */
		return (Gas_phase);
		break;
	case Keywords::KEY_KINETICS:					/* Kinetics */
		return (Kinetics);
		break;
	case Keywords::KEY_SOLID_SOLUTIONS:			/* solid_solutions */
		return (Ss_phase);
		break;
	default:
		warning_msg
			("EXISTS expecting keyword solution, mix, kinetics, reaction, reaction_temperature, equilibrium_phases, exchange, surface, gas_phase, or solid_solutions.");
		break;
	}
	return (UnKnown);
}

/*
 * copier routines
 */
/* ---------------------------------------------------------------------- */
int Phreeqc::
copier_add(struct copier *copier_ptr, int n_user, int start, int end)
/* ---------------------------------------------------------------------- */
/*
 *   add new set of copy instructions
 */
{
	copier_ptr->n_user.push_back(n_user);
	copier_ptr->start.push_back(start);
	copier_ptr->end.push_back(end);
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
copier_clear(struct copier* copier_ptr)
/* ---------------------------------------------------------------------- */
/*
 *   clear copier
 */
{
	copier_ptr->n_user.clear();
	copier_ptr->start.clear();
	copier_ptr->end.clear();
	return (OK);
}

#include "StorageBin.h"

/* ---------------------------------------------------------------------- */
void Phreeqc::
Use2cxxStorageBin(cxxStorageBin & sb)
/* ---------------------------------------------------------------------- */
{
	//Add everything from use structure to storagebin sb

	sb.Get_system().Set_io(sb.Get_io());
	if (use.Get_mix_in())
	{
		cxxMix *entity = use.Get_mix_ptr();
		if (entity != NULL)
		{
			sb.Set_Mix(use.Get_n_mix_user(), entity);
		}

		// put mix solutions in sb
		cxxMix * mix_ptr = use.Get_mix_ptr();
		std::map<int, LDBLE>::const_iterator cit;
		for (cit = mix_ptr->Get_mixComps().begin(); cit != mix_ptr->Get_mixComps().end(); cit++)
		{
			cxxSolution *entity = Utilities::Rxn_find(Rxn_solution_map, cit->first);
			if (entity != NULL)
			{
				sb.Set_Solution(cit->first, entity);
			}
		}
	}
	else if (use.Get_solution_in())
	{
		cxxSolution *entity = Utilities::Rxn_find(Rxn_solution_map, use.Get_n_solution_user());
		if (entity != NULL)
		{
			sb.Set_Solution(use.Get_n_solution_user(), entity);
		}
	}
	if (use.Get_pp_assemblage_in())
	{
		cxxPPassemblage *entity_ptr = Utilities::Rxn_find(Rxn_pp_assemblage_map, use.Get_n_pp_assemblage_user());
		if (entity_ptr != NULL)
		{
			sb.Set_PPassemblage(use.Get_n_pp_assemblage_user(), entity_ptr);
		}
	}
	if (use.Get_exchange_in())
	{
		cxxExchange *entity_ptr = Utilities::Rxn_find(Rxn_exchange_map, use.Get_n_exchange_user());
		if (entity_ptr != NULL)
		{
			sb.Set_Exchange(use.Get_n_exchange_user(), entity_ptr);
		}
	}
	if (use.Get_surface_in())
	{
		cxxSurface *entity_ptr = Utilities::Rxn_find(Rxn_surface_map, use.Get_n_surface_user());
		if (entity_ptr != NULL)
		{
			sb.Set_Surface(use.Get_n_surface_user(), entity_ptr);
		}
	}
	if (use.Get_gas_phase_in())
	{
		cxxGasPhase *entity_ptr = Utilities::Rxn_find(Rxn_gas_phase_map, use.Get_n_gas_phase_user());
		if (entity_ptr != NULL)
		{
			sb.Set_GasPhase(use.Get_n_gas_phase_user(), entity_ptr);
		}
	}
	if (use.Get_ss_assemblage_in())
	{
		cxxSSassemblage *entity_ptr = Utilities::Rxn_find(Rxn_ss_assemblage_map, use.Get_n_ss_assemblage_user());
		if (entity_ptr != NULL)
		{
			sb.Set_SSassemblage(use.Get_n_ss_assemblage_user(), entity_ptr);
		}
	}
	if (use.Get_kinetics_in())
	{
		cxxKinetics *entity_ptr = Utilities::Rxn_find(Rxn_kinetics_map, use.Get_n_kinetics_user());
		if (entity_ptr != NULL)
		{
			sb.Set_Kinetics(use.Get_n_kinetics_user(), entity_ptr);
		}
	}
	if (use.Get_reaction_in())
	{
		cxxReaction *entity = Utilities::Rxn_find(Rxn_reaction_map, use.Get_n_reaction_user());
		if (entity != NULL)
		{
			sb.Set_Reaction(use.Get_n_reaction_user(), entity);
		}
	}
	if (use.Get_temperature_in())
	{
		cxxTemperature *entity = Utilities::Rxn_find(Rxn_temperature_map, use.Get_n_temperature_user());
		if (entity != NULL)
		{
			sb.Set_Temperature(use.Get_n_temperature_user(), entity);
		}
	}
	if (use.Get_pressure_in())
	{
		cxxPressure *entity = Utilities::Rxn_find(Rxn_pressure_map, use.Get_n_pressure_user());
		if (entity != NULL)
		{
			sb.Set_Pressure(use.Get_n_pressure_user(), entity);
		}
	}
}

void Phreeqc::
phreeqc2cxxStorageBin(cxxStorageBin & sb)
	//
	// Fills StorageBin sb with all reactants from phreeqc instance.
	// equivalent to old import_phreeqc.
	//
{
	// Solutions
	{
		std::map<int, cxxSolution>::iterator it;
		for (it = Rxn_solution_map.begin(); it != Rxn_solution_map.end(); it++)
		{
			sb.Set_Solution(it->second.Get_n_user(), &(it->second));	
		}
	}
	// Exchangers
	{
		std::map<int, cxxExchange>::iterator it;
		for (it = Rxn_exchange_map.begin(); it != Rxn_exchange_map.end(); it++)
		{
			sb.Set_Exchange(it->second.Get_n_user(), &(it->second));	
		}
	}
	// GasPhases
	{
		std::map<int, cxxGasPhase>::iterator it;
		for (it = Rxn_gas_phase_map.begin(); it != Rxn_gas_phase_map.end(); it++)
		{
			sb.Set_GasPhase(it->second.Get_n_user(), &(it->second));	
		}
	}

	// Kinetics
	{
		std::map<int, cxxKinetics>::iterator it;
		for (it = Rxn_kinetics_map.begin(); it != Rxn_kinetics_map.end(); it++)
		{
			sb.Set_Kinetics(it->second.Get_n_user(), &(it->second));	
		}
	}
	// PPassemblages
	{
		std::map<int, cxxPPassemblage>::iterator it;
		for (it = Rxn_pp_assemblage_map.begin(); it != Rxn_pp_assemblage_map.end(); it++)
		{
			sb.Set_PPassemblage(it->second.Get_n_user(), &(it->second));	
		}
	}
	// SSassemblages
	{
		std::map<int, cxxSSassemblage>::iterator it;
		for (it = Rxn_ss_assemblage_map.begin(); it != Rxn_ss_assemblage_map.end(); it++)
		{
			sb.Set_SSassemblage(it->second.Get_n_user(), &(it->second));	
		}
	}
	// Surfaces
	{
		std::map<int, cxxSurface>::iterator it;
		for (it = Rxn_surface_map.begin(); it != Rxn_surface_map.end(); it++)
		{
			sb.Set_Surface(it->second.Get_n_user(), &(it->second));	
		}
	}
	// Mixes
	{
		std::map<int, cxxMix>::iterator it;
		for (it = Rxn_mix_map.begin(); it != Rxn_mix_map.end(); it++)
		{
			sb.Set_Mix(it->second.Get_n_user(), &(it->second));	
		}
	}

	// Reactions
	{
		std::map<int, cxxReaction>::iterator it;
		for (it = Rxn_reaction_map.begin(); it != Rxn_reaction_map.end(); it++)
		{
			sb.Set_Reaction(it->second.Get_n_user(), &(it->second));	
		}
	}

	// Temperatures
	{
		std::map<int, cxxTemperature>::iterator it;
		for (it = Rxn_temperature_map.begin(); it != Rxn_temperature_map.end(); it++)
		{
			sb.Set_Temperature(it->second.Get_n_user(), &(it->second));	
		}
	}

	// Pressures
	{
		std::map<int, cxxPressure>::iterator it;
		for (it = Rxn_pressure_map.begin(); it != Rxn_pressure_map.end(); it++)
		{
			sb.Set_Pressure(it->second.Get_n_user(), &(it->second));	
		}
	}
}

void Phreeqc::
phreeqc2cxxStorageBin(cxxStorageBin & sb, int n)
		//
		// copy phreeqc reactants numbered n to StorageBin sb
		//
{
	// Solutions
	{
		cxxSolution *entity_ptr = Utilities::Rxn_find(Rxn_solution_map, n);
		if (entity_ptr != NULL)
		{
			sb.Set_Solution(n, entity_ptr);
		}
	}
	// Exchangers
	{
		cxxExchange *entity_ptr = Utilities::Rxn_find(Rxn_exchange_map, n);
		if (entity_ptr != NULL)
		{
			sb.Set_Exchange(n, entity_ptr);
		}
	}

	// GasPhases
	{
		cxxGasPhase *entity_ptr = Utilities::Rxn_find(Rxn_gas_phase_map, n);
		if (entity_ptr != NULL)
		{
			sb.Set_GasPhase(n, entity_ptr);
		}
	}

	// Kinetics
	{
		cxxKinetics *entity_ptr = Utilities::Rxn_find(Rxn_kinetics_map, n);
		if (entity_ptr != NULL)
		{
			sb.Set_Kinetics(n, entity_ptr);
		}
	}
	// PPassemblages
	{
		cxxPPassemblage *entity_ptr = Utilities::Rxn_find(Rxn_pp_assemblage_map, n);
		if (entity_ptr != NULL)
		{
			sb.Set_PPassemblage(n, entity_ptr);
		}
	}
	// SSassemblages
	{
		cxxSSassemblage *entity_ptr = Utilities::Rxn_find(Rxn_ss_assemblage_map, n);
		if (entity_ptr != NULL)
		{
			sb.Set_SSassemblage(n, entity_ptr);
		}
	}
	// Surfaces
	{
		cxxSurface *entity_ptr = Utilities::Rxn_find(Rxn_surface_map, n);
		if (entity_ptr != NULL)
		{
			sb.Set_Surface(n, entity_ptr);
		}
	}
}
void Phreeqc::
cxxStorageBin2phreeqc(cxxStorageBin & sb, int n)
//
// copy all reactants from storage bin number n to phreeqc
// replaces any existing reactants in phreeqc
//
{
	// Solutions
	{
		std::map < int, cxxSolution >::const_iterator it = sb.Get_Solutions().find(n);
		if (it != sb.Get_Solutions().end())
		{
			Rxn_solution_map[n] = it->second;
		}
	}
	// Exchangers
	{
		std::map < int, cxxExchange >::const_iterator it = sb.Get_Exchangers().find(n);
		if (it != sb.Get_Exchangers().end())
		{
			Rxn_exchange_map[n] = it->second;
		}
	}

	// GasPhases
	{
		std::map < int, cxxGasPhase >::const_iterator it = sb.Get_GasPhases().find(n);
		if (it != sb.Get_GasPhases().end())
		{
			Rxn_gas_phase_map[n] = it->second;
		}
	}

	// Kinetics
	{
		std::map < int, cxxKinetics >::const_iterator it = sb.Get_Kinetics().find(n);
		if (it != sb.Get_Kinetics().end())
		{
			Rxn_kinetics_map[n] = it->second;
		}
	}
	// PPassemblages
	{
		std::map < int, cxxPPassemblage >::const_iterator it = sb.Get_PPassemblages().find(n);
		if (it != sb.Get_PPassemblages().end())
		{
			Rxn_pp_assemblage_map[n] = it->second;
		}
	}
	// SSassemblages
	{
		std::map < int, cxxSSassemblage >::const_iterator it = sb.Get_SSassemblages().find(n);
		if (it != sb.Get_SSassemblages().end())
		{
			Rxn_ss_assemblage_map[n] = it->second;
		}
	}
	// Surfaces
	{
		std::map < int, cxxSurface >::const_iterator it = sb.Get_Surfaces().find(n);
		if (it != sb.Get_Surfaces().end())
		{
			Rxn_surface_map[n] = it->second;
		}
	}
	// Mixes
	{
		std::map < int, cxxMix >::const_iterator it = sb.Get_Mixes().find(n);
		if (it != sb.Get_Mixes().end())
		{
			Rxn_mix_map[n] = it->second;
		}
	}

	// Reactions
	{
		std::map < int, cxxReaction >::const_iterator it = sb.Get_Reactions().find(n);
		if (it != sb.Get_Reactions().end())
		{
			Rxn_reaction_map[n] = it->second;
		}
	}
	// Temperatures
	{
		std::map < int, cxxTemperature >::const_iterator it = sb.Get_Temperatures().find(n);
		if (it != sb.Get_Temperatures().end())
		{
			Rxn_temperature_map[n] = it->second;
		}
	}
	// Pressures
	{
		std::map < int, cxxPressure >::const_iterator it = sb.Get_Pressures().find(n);
		if (it != sb.Get_Pressures().end())
		{
			Rxn_pressure_map[n] = it->second;
		}
	}
}
void Phreeqc::
cxxStorageBin2phreeqc(cxxStorageBin & sb)
//
// copy data from storage bin to phreeqc
// replaces any existing reactants in phreeqc
//
{
	// Solutions
	{
		std::map < int, cxxSolution >::const_iterator it = sb.Get_Solutions().begin();
		for ( ; it != sb.Get_Solutions().end(); it++)
		{
			Rxn_solution_map[it->first] = it->second;
		}
	}
	// Exchangers
	{
		std::map < int, cxxExchange >::const_iterator it = sb.Get_Exchangers().begin();
		for ( ; it != sb.Get_Exchangers().end(); it++)
		{
			Rxn_exchange_map[it->first] = it->second;
		}
	}

	// GasPhases
	{
		std::map < int, cxxGasPhase >::const_iterator it = sb.Get_GasPhases().begin();
		for ( ; it != sb.Get_GasPhases().end(); it++)
		{
			Rxn_gas_phase_map[it->first] = it->second;
		}
	}

	// Kinetics
	{
		std::map < int, cxxKinetics >::const_iterator it = sb.Get_Kinetics().begin();
		for ( ; it != sb.Get_Kinetics().end(); it++)
		{
			Rxn_kinetics_map[it->first] = it->second;
		}
	}
	// PPassemblages
	{
		std::map < int, cxxPPassemblage >::const_iterator it = sb.Get_PPassemblages().begin();
		for ( ; it != sb.Get_PPassemblages().end(); it++)
		{
			Rxn_pp_assemblage_map[it->first] = it->second;
		}
	}
	// SSassemblages
	{
		std::map < int, cxxSSassemblage >::const_iterator it = sb.Get_SSassemblages().begin();
		for ( ; it != sb.Get_SSassemblages().end(); it++)
		{
			Rxn_ss_assemblage_map[it->first] = it->second;
		}
	}
	// Surfaces
	{
		std::map < int, cxxSurface >::const_iterator it = sb.Get_Surfaces().begin();
		for ( ; it != sb.Get_Surfaces().end(); it++)
		{
			Rxn_surface_map[it->first] = it->second;
		}
	}
	// Mixes
	{
		std::map < int, cxxMix >::const_iterator it = sb.Get_Mixes().begin();
		for ( ; it != sb.Get_Mixes().end(); it++)
		{
			Rxn_mix_map[it->first] = it->second;
		}
	}

	// Reactions
	{
		std::map < int, cxxReaction >::const_iterator it = sb.Get_Reactions().begin();
		for ( ; it != sb.Get_Reactions().end(); it++)
		{
			Rxn_reaction_map[it->first] = it->second;
		}
	}
	// Temperatures
	{
		std::map < int, cxxTemperature >::const_iterator it = sb.Get_Temperatures().begin();
		for ( ; it != sb.Get_Temperatures().end(); it++)
		{
			Rxn_temperature_map[it->first] = it->second;
		}
	}
	// Pressures
	{
		std::map < int, cxxPressure >::const_iterator it = sb.Get_Pressures().begin();
		for ( ; it != sb.Get_Pressures().end(); it++)
		{
			Rxn_pressure_map[it->first] = it->second;
		}
	}
}


