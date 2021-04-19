#include "Rate.h"
#include "Phreeqc.h"
/* **********************************************************************
 *
 *   Routines related to structure "rates"
 *
 * ********************************************************************** */
 /* ---------------------------------------------------------------------- */
class rate* Phreeqc::
	rate_bsearch(const char* cptr, int* j)
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
	void* void_ptr;

	if (rates.size() == 0)
	{
		*j = -1;
		return (NULL);
	}
	void_ptr = (void*)
		bsearch((char*)cptr,
			(char*)&rates[0],
			rates.size(),
			sizeof(class rate*), rate_compare_string);

	if (void_ptr == NULL)
	{
		*j = -1;
		return (NULL);
	}

	*j = (int)((class rate*)void_ptr - &rates[0]);
	return ((class rate*)void_ptr);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
rate_compare(const void* ptr1, const void* ptr2)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Compares names of rates for sort
	 */
	const class rate* rate_ptr1, * rate_ptr2;
	rate_ptr1 = *(const class rate**)ptr1;
	rate_ptr2 = *(const class rate**)ptr2;
	return (strcmp_nocase(rate_ptr1->name.c_str(), rate_ptr2->name.c_str()));
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
rate_compare_string(const void* ptr1, const void* ptr2)
/* ---------------------------------------------------------------------- */
{
	const char* char_ptr;
	const class rate* rate_ptr;
	char_ptr = (const char*)ptr1;
	rate_ptr = *(const class rate**)ptr2;
	return (strcmp_nocase(char_ptr, rate_ptr->name.c_str()));
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
rate_free(class rate* rate_ptr)
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
class rate* Phreeqc::
	rate_copy(const class rate* rate_ptr)
	/* ---------------------------------------------------------------------- */
{
	/*
	*   Copies a rate to new allocated space
	*/
	if (rate_ptr == NULL)
		return (NULL);
	class rate* rate_new = new class rate;
	rate_new->name = rate_ptr->name;
	rate_new->commands = rate_ptr->commands;
	rate_new->new_def = TRUE;
	rate_new->linebase = NULL;
	rate_new->varbase = NULL;
	rate_new->loopbase = NULL;
	return (rate_new);
}

/* ---------------------------------------------------------------------- */
class rate* Phreeqc::
	rate_search(std::string name, int* n)
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
	std::map<std::string, int>::iterator it;

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
		if (strcmp_nocase(rates[i].name.c_str(), name.c_str()) == 0)
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
		qsort(&rates[0], rates.size(), sizeof(class rate),
			rate_compare);
	}
	return (OK);
}
