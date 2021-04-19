#define TRUE 1
#define FALSE 0
#include "Phase.h"
#include "Phreeqc.h"
/* **********************************************************************
 *
 *   Routines related to structure "phases"
 *
 * ********************************************************************** */
 /* ---------------------------------------------------------------------- */
class phase* Phreeqc::
	phase_alloc(void)
	/* ---------------------------------------------------------------------- */
{
	/*
	 *   Allocates space to a phase structure and initializes
	 *      arguments: void
	 *      return: pointer to new phase structure
	 */
	class phase* phase_ptr;
	/*
	 *   Allocate space
	 */
	phase_ptr = new class phase;
	/*
	 *   Initialize space
	 */
	phase_init(phase_ptr);
	return (phase_ptr);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
phase_compare(const void* ptr1, const void* ptr2)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Compares names of phases for sort
	 */
	const class phase* phase_ptr1, * phase_ptr2;
	phase_ptr1 = *(const class phase**)ptr1;
	phase_ptr2 = *(const class phase**)ptr2;
	return (strcmp_nocase(phase_ptr1->name.c_str(), phase_ptr2->name.c_str()));
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
phase_compare_string(const void* ptr1, const void* ptr2)
/* ---------------------------------------------------------------------- */
{
	const char* char_ptr;
	const class phase* phase_ptr;
	char_ptr = (const char*)ptr1;
	phase_ptr = *(const class phase**)ptr2;
	return (strcmp_nocase(char_ptr, phase_ptr->name.c_str()));
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
phase_free(class phase* phase_ptr)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Frees memory allocated within phase[i], does not free phase structure
	 *   Input: i, number of phase
	 *   Return: OK
	 */
	if (phase_ptr == NULL)
		return (ERROR);
	phase_ptr->next_elt.clear();
	phase_ptr->next_sys_total.clear();;
	phase_ptr->add_logk.clear();
	return (OK);
}

/* ---------------------------------------------------------------------- */
class phase* Phreeqc::
	phase_bsearch(const std::string& name, int* j, int print)
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
	void* void_ptr;

	void_ptr = NULL;
	if ((int)phases.size() > 0)
	{
		void_ptr = (void*)
			bsearch((void*)&name[0],
				(void*)&phases[0],
				phases.size(),
				sizeof(class phase*), phase_compare_string);
	}
	if (void_ptr == NULL && print == TRUE)
	{
		error_string = sformatf("Could not find phase in list, %s.", name.c_str());
		error_msg(error_string, CONTINUE);
	}

	if (void_ptr == NULL)
	{
		*j = -1;
		return (NULL);
	}

	*j = (int)((class phase**)void_ptr - &phases[0]);
	return (*(class phase**)void_ptr);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
phase_init(class phase* phase_ptr)
/* ---------------------------------------------------------------------- */
/*
 *   set pointers in phase structure to NULL
 */
{
	int i;

	//phase_ptr->name = NULL;
	//phase_ptr->formula = NULL;
	phase_ptr->in = FALSE;
	phase_ptr->lk = 0.0;
	for (i = 0; i < Logk::MAX_LOG_K_INDICES; i++)
		phase_ptr->logk[i] = 0.0;
	phase_ptr->original_units = Logk::kjoules;
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
	phase_ptr->check_equation = TRUE;
	phase_ptr->replaced = 0;
	phase_ptr->in_system = 1;
	phase_ptr->original_deltav_units = Logk::cm3_per_mol;
	return (OK);
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
		phase_free(phase_ptr);
		phase_init(phase_ptr);
		phase_ptr->name = name_in;
		return (phase_ptr);
	}
	/*
	 *   Make new phase structure and return pointer to it
	 */
	size_t n = phases.size();
	phases.resize(n + 1);
	phases[n] = phase_alloc();
	/* set name in phase structure */
	phases[n]->name = name_in;
	/*
	 *   Update map
	 */
	phases_map[name] = phases[n];
	return (phases[n]);
}
