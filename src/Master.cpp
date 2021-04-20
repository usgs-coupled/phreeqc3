#include "Master.h"
#include "Phreeqc.h"
/* **********************************************************************
 *
 *   Routines related to structure "master"
 *
 * ********************************************************************** */
/* ---------------------------------------------------------------------- */
int Phreeqc::
master_delete(const std::string& cptr)
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
	if (master_find(cptr) == NULL)
		return FALSE;
	if (master_search(cptr, &n) == NULL)
		return (FALSE);
	delete master[n];
	master.erase(master.begin() + n);
	master_map.erase(cptr);
	return (TRUE);
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
	if (master.size() > 0)
	{
		void_ptr = bsearch((const char*)cptr.c_str(),
			(char*)&master[0],
			master.size(),
			sizeof(class master*), master_compare_string);
		if (void_ptr != NULL)
		{
			return (*(class master**)void_ptr);
		}
	}
	return NULL;
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
/* ---------------------------------------------------------------------- */
class master* Phreeqc::
master_bsearch_primary(const std::string& cptr)
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
	cptr1 = cptr.c_str();
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
			"Could not find primary master species for %s.", cptr.c_str());
		error_msg(error_string, CONTINUE);
	}
	return (master_ptr_primary);
}
/* ---------------------------------------------------------------------- */
class master* Phreeqc::
master_bsearch_secondary(const std::string& cptr)
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
	cptr1 = cptr.c_str();
	get_elt(&cptr1, elt, &l);
	/*
	 *   Search master species list
	 */
	master_ptr_primary = master_bsearch(elt.c_str());
	if (master_ptr_primary == NULL)
	{
		input_error++;
		error_string = sformatf(
			"Could not find primary master species for %s.", cptr.c_str());
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
			"Could not find secondary master species for %s.", cptr.c_str());
		error_msg(error_string, STOP);
	}


	return (master_ptr_secondary);
}
/* ---------------------------------------------------------------------- */
class master* Phreeqc::
master_search(const std::string& cptr, int* n)
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
/* ---------------------------------------------------------------------- */
class master* Phreeqc::
master_find(const std::string& name)
/* ---------------------------------------------------------------------- */
{

	std::map<std::string, class master*>::iterator it =
		master_map.find(name);
	if (it != master_map.end())
	{
		return it->second;
	}
	return NULL;
}
class master* Phreeqc::
master_store(const std::string& name)
/* ---------------------------------------------------------------------- */
{
	class master* master_ptr = master_find(name);
	if (master_ptr != NULL)
	{
		return master_ptr;
	}
	master_ptr = new class master;
	master.push_back(master_ptr);
	master_map[name] = master_ptr;
	return master_ptr;
}
