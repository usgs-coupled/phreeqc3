#include "Master.h"
#include "Phreeqc.h"
/* **********************************************************************
 *
 *   Routines related to structure "master"
 *
 * ********************************************************************** */
/* ---------------------------------------------------------------------- */
int Phreeqc::
master_delete(const std::string& name)
/* ---------------------------------------------------------------------- */
{
	// delete instance, update vector and maps
	int n;
	if (master_find(name) == NULL)
		return FALSE;
	if (master_search(name, &n) == NULL)
		return (FALSE);
	delete master[n];
	master.erase(master.begin() + n);
	master_map.erase(name);
	std::string lc;
	lc = name;
	master_map_lc.erase(lc);
	return (TRUE);
}
/* ---------------------------------------------------------------------- */
class master* Phreeqc::
master_bsearch(const std::string& name)
/* ---------------------------------------------------------------------- */
{
	// lower case search
	if (master.size() > 0)
	{
		std::string lc = name;
		str_tolower(lc);
		std::map<std::string, class master*>::iterator it =
			master_map_lc.find(lc);
		if (it != master_map_lc.end())
			return it->second;
	}
	return NULL;
}
/* ---------------------------------------------------------------------- */
class master* Phreeqc::
master_bsearch_primary(const std::string& name)
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
	cptr1 = name.c_str();
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
			"Could not find primary master species for %s.", name.c_str());
		error_msg(error_string, CONTINUE);
	}
	return (master_ptr_primary);
}
/* ---------------------------------------------------------------------- */
class master* Phreeqc::
master_bsearch_secondary(const std::string& name)
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
	cptr1 = name.c_str();
	get_elt(&cptr1, elt, &l);
	/*
	 *   Search master species list
	 */
	master_ptr_primary = master_bsearch(elt.c_str());
	if (master_ptr_primary == NULL)
	{
		input_error++;
		error_string = sformatf(
			"Could not find primary master species for %s.", name.c_str());
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
	if (master_ptr != NULL && master_ptr->elt != NULL && 
		(master_ptr->elt->primary == master_ptr_primary))
	{
		master_ptr_secondary = master_ptr;
	}
	else
	{
		input_error++;
		error_string = sformatf(
			"Could not find secondary master species for %s.", name.c_str());
		error_msg(error_string, STOP);
	}
	return (master_ptr_secondary);
}
/* ---------------------------------------------------------------------- */
class master* Phreeqc::
master_search(const std::string& name, int* n)
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
		if (name == master[i]->elt->name)
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
	std::string lc = name;
	str_tolower(lc);
	master_map_lc[lc] = master_ptr;
	return master_ptr;
}
