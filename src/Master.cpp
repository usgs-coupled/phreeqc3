#include "Master.h"
#include "Phreeqc.h"
/* **********************************************************************
 *
 *   Routines related to structure "master"
 *
 * ********************************************************************** */
 /* ---------------------------------------------------------------------- */
class master* Phreeqc::
	master_alloc(void)
	/* ---------------------------------------------------------------------- */
	/*
	 *   Allocates space to a master structure and initializes the space.
	 *      arguments: void
	 *      return: pointer to a master structure
	 */
{
	class master* ptr = new class master;
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
	//ptr->gfw_formula.clear();
	ptr->unknown = NULL;
	ptr->s = NULL;
	ptr->pe_rxn.clear();
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
master_free(class master* master_ptr)
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
class master* Phreeqc::
	master_bsearch(const std::string& cptr)
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
	void* void_ptr;
	if (master.size() == 0)
	{
		return (NULL);
	}
	void_ptr = bsearch((const char*)cptr.c_str(),
		(char*)&master[0],
		master.size(),
		sizeof(class master*), master_compare_string);
	if (void_ptr == NULL)
	{
		void_ptr = bsearch(cptr.c_str(),
			(char*)&master[0],
			master.size(),
			sizeof(class master*), master_compare_string);
	}
	if (void_ptr == NULL)
	{
		return (NULL);
	}
	else
	{
		return (*(class master**)void_ptr);
	}
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
master_compare_string(const void* ptr1, const void* ptr2)
/* ---------------------------------------------------------------------- */
{
	const char* string_ptr;
	const class master* master_ptr;

	string_ptr = (const char*)ptr1;
	master_ptr = *(const class master**)ptr2;
	return (strcmp_nocase(string_ptr, master_ptr->elt->name.c_str()));
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
master_compare(const void* ptr1, const void* ptr2)
/* ---------------------------------------------------------------------- */
{
	const class master* master_ptr1, * master_ptr2;
	master_ptr1 = *(const class master**)ptr1;
	master_ptr2 = *(const class master**)ptr2;
	return (strcmp_nocase(master_ptr1->elt->name.c_str(), master_ptr2->elt->name.c_str()));
}
class master* Phreeqc::
	master_bsearch_primary(const std::string& cstring)
{
	return master_bsearch_primary(cstring.c_str());
}
/* ---------------------------------------------------------------------- */
class master* Phreeqc::
	master_bsearch_primary(const char* cptr)
	/* ---------------------------------------------------------------------- */
{
	/*
	 *   Find primary master species for first element in the string, cptr.
	 *   Uses binary search. Assumes master is in sort order.
	 */
	int l;
	const char* cptr1;
	class master* master_ptr_primary;
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
class master* Phreeqc::
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
	class master* master_ptr_primary, * master_ptr = NULL, * master_ptr_secondary = NULL;
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
class master* Phreeqc::
	master_search(const char* cptr, int* n)
	/* ---------------------------------------------------------------------- */
{
	/*
	 *   Linear search of master to find master species in string, cptr.
	 *   Returns pointer if found. n contains position in array master.
	 *   Returns NULL if not found.
	 */
	int i;
	class master* master_ptr;
	/*
	 *   Search master species list
	 */
	*n = -999;
	for (i = 0; i < (int)master.size(); i++)
	{
		if (cptr == master[i]->elt->name)
		{
			*n = i;
			master_ptr = master[i];
			return (master_ptr);
		}
	}
	return (NULL);
}
