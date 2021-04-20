#include "Element.h"
#include "Phreeqc.h"
/* **********************************************************************
 *
 *   Routines related to structure "element"
 *
 * ********************************************************************** */
/* ---------------------------------------------------------------------- */
class element* Phreeqc::
element_store(const std::string& element)
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
	std::map<std::string, class element*>::const_iterator it;
	it = elements_map.find(element);
	if (it != elements_map.end())
	{
		return (it->second);
	}
	/*
	 *   Save new element structure and return pointer to it
	 */
	class element* elt_ptr = new class element;
	elt_ptr->name = element;
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
add_elt_list(const cxxNameDouble& nd, LDBLE coef)
/* ---------------------------------------------------------------------- */
{
	cxxNameDouble::const_iterator cit = nd.begin();
	for (; cit != nd.end(); cit++)
	{
		if (count_elts >= (int)elt_list.size())
		{
			elt_list.resize(count_elts + 1);
		}
		elt_list[count_elts].elt = element_store(cit->first.c_str());
		elt_list[count_elts].coef = cit->second * coef;
		count_elts++;
	}
	return (OK);
}
int Phreeqc::
add_elt_list(const std::vector<class elt_list>& el, double coef)
/* ---------------------------------------------------------------------- */
{
	const class elt_list* elt_list_ptr = &el[0];

	for (; elt_list_ptr->elt != NULL; elt_list_ptr++)
	{
		if (count_elts >= elt_list.size())
		{
			elt_list.resize(count_elts + 1);
		}
		elt_list[count_elts].elt = elt_list_ptr->elt;
		elt_list[count_elts].coef = elt_list_ptr->coef * coef;
		count_elts++;
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
	elt_list_combine();
	for (j = 0; j < count_elts; j++)
	{
		if (elt_list[j].elt->name == "H")
		{
			found_h = j;
			coef_h = elt_list[j].coef;
		}
		else if (elt_list[j].elt->name == "O")
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
		elt_list_combine();
		return (OK);
	}
	elt_list[found_h].coef = coef;
	return (OK);
}
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
		sizeof(class elt_list), Phreeqc::elt_list_compare);
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
elt_list_compare(const void* ptr1, const void* ptr2)
/* ---------------------------------------------------------------------- */
{
	const class elt_list* a, * b;

	a = (const class elt_list*)ptr1;
	b = (const class elt_list*)ptr2;
	return (strncmp(a->elt->name.c_str(), b->elt->name.c_str(), MAX_LENGTH));
}
/* ---------------------------------------------------------------------- */
std::vector<class elt_list> Phreeqc::
elt_list_internal_copy(const std::vector<class elt_list>& el)
/* ---------------------------------------------------------------------- */
{
	std::vector<class elt_list> new_elt_list;
	if (el.size() == 0) return new_elt_list;
	const class elt_list* elt_list_ptr = &el[0];

	new_elt_list.resize(el.size());
	size_t count = 0;
	for (; elt_list_ptr->elt != NULL; elt_list_ptr++)
	{
		new_elt_list[count].elt = element_store(elt_list_ptr->elt->name.c_str());
		new_elt_list[count].coef = elt_list_ptr->coef;
		count++;
	}
	new_elt_list[count].elt = NULL;
	return new_elt_list;
}
/* ---------------------------------------------------------------------- */
std::vector<class elt_list> Phreeqc::
elt_list_vsave(void)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Takes data from work space elt_list, allocates a new elt_list structure,
	 *   copies data from work space to new structure, and returns pointer to
	 *   new structure.
	 */
	size_t j;
	std::vector<class elt_list> new_elt_list;
	/*
	 *   Sort elements in reaction and combine
	 */
	elt_list_combine();
	/*
	 *   Malloc space and store element data
	 */
	new_elt_list.resize(count_elts + 1);
	for (j = 0; j < count_elts; j++)
	{
		new_elt_list[j].elt = elt_list[j].elt;
		new_elt_list[j].coef = elt_list[j].coef;
	}
	new_elt_list[count_elts].elt = NULL;
	return new_elt_list;
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
	for (int i = 0; i < count_elts; i++)
	{
		nd.add(elt_list[i].elt->name.c_str(), elt_list[i].coef);
	}
	return (nd);
}
