#include "Species.h"
#include "Phreeqc.h"
/* **********************************************************************
 *
 *   Routines related to structure "species"
 *
 * ********************************************************************** */
 /* ---------------------------------------------------------------------- */
class species* Phreeqc::
	s_alloc(void)
	/* ---------------------------------------------------------------------- */
	/*
	 *   Allocates space to a species structure, initializes
	 *      arguments: void
	 *      return: pointer to a species structure
	 */
{
	class species* s_ptr;
	s_ptr = new class species;
	/*
	 *   set pointers in structure to NULL, variables to zero
	 */
	s_init(s_ptr);

	return (s_ptr);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
s_compare(const void* ptr1, const void* ptr2)
/* ---------------------------------------------------------------------- */
{
	const class species* s_ptr1, * s_ptr2;
	s_ptr1 = *(const class species**)ptr1;
	s_ptr2 = *(const class species**)ptr2;
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
	s[i] = (class species*)free_check_null(s[i]);
	s.erase(s.begin() + i);
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
s_free(class species* s_ptr)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Free space allocated for species structure, s_ptr. Does not free s_ptr.
	 */
	if (s_ptr == NULL)
		return (ERROR);
	s_ptr->next_elt.clear();
	s_ptr->next_secondary.clear();
	s_ptr->next_sys_total.clear();
	s_ptr->add_logk.clear();
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
s_init(class species* s_ptr)
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
	//s_ptr->mole_balance.clear();
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
	for (i = 0; i < Logk::MAX_LOG_K_INDICES; i++)
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
	s_ptr->original_units = Logk::kjoules;
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
	s_ptr->original_deltav_units = Logk::cm3_per_mol;
	return (OK);
}
/* ---------------------------------------------------------------------- */
class species* Phreeqc::
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
	s_store(const char* name, LDBLE l_z, int replace_if_found)
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
	class species* s_ptr = NULL;
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
