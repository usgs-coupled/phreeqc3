#include "Phreeqc.h"
#include "phqalloc.h"


/* **********************************************************************
 *
 *   Routines related to structure "pitz_param"
 *
 * ********************************************************************** */
/* ---------------------------------------------------------------------- */
struct pitz_param * Phreeqc::
pitz_param_alloc(void)
/* ---------------------------------------------------------------------- */
{
	struct pitz_param *pitz_param_ptr;
	pitz_param_ptr =
		(struct pitz_param *) PHRQ_malloc(sizeof(struct pitz_param));
	if (pitz_param_ptr == NULL)
		malloc_error();
	return (pitz_param_ptr);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
pitz_param_init(struct pitz_param *pitz_param_ptr)
/* ---------------------------------------------------------------------- */
{
	int i;
/*
 *   Frees all data associated with pitz_param structure.
 */

	if (pitz_param_ptr == NULL)
		return (ERROR);
	pitz_param_ptr->species[0] = NULL;
	pitz_param_ptr->species[1] = NULL;
	pitz_param_ptr->species[2] = NULL;
	pitz_param_ptr->ispec[0] = -1;
	pitz_param_ptr->ispec[1] = -1;
	pitz_param_ptr->ispec[2] = -1;
	pitz_param_ptr->type = TYPE_Other;
	pitz_param_ptr->p = 0.0;
	pitz_param_ptr->U.b0 = 0.0;
	for (i = 0; i < 6; i++)
	{
		pitz_param_ptr->a[i] = 0.0;
	}
	pitz_param_ptr->alpha = 0.0;
	pitz_param_ptr->thetas = NULL;
	return (OK);
}

/* ---------------------------------------------------------------------- */
struct pitz_param * Phreeqc::
pitz_param_read(char *string, int n)
/* ---------------------------------------------------------------------- */
{
/*
 *   Read pitzer parameter info from string
 *   n is number of species (character values)
 *          
 */
	int l, i, j, k;
	char *ptr;
	char token[2 * MAX_LENGTH];
	struct pitz_param pzp, *pzp_ptr;

	if (n != 2 && n != 3)
		return (NULL);
	if (string == NULL)
		return (NULL);

	pitz_param_init(&pzp);
	ptr = string;
	if (copy_token(token, &ptr, &l) == EMPTY)
		return (NULL);
	ptr = string;
	for (i = 0; i < n; i++)
	{
		if (copy_token(token, &ptr, &l) == EMPTY)
			return (NULL);
		pzp.species[i] = string_hsave(token);
	}
	k = 0;
	for (i = 0; i < 6; i++)
	{
		if (copy_token(token, &ptr, &l) == EMPTY)
			break;
		j = sscanf(token, SCANFORMAT, &pzp.a[i]);
		if (j <= 0)
			break;
		k++;
	}
	if (k <= 0)
		return (NULL);
	pzp_ptr = pitz_param_duplicate(&pzp);
	return (pzp_ptr);
}

/* ---------------------------------------------------------------------- */
struct pitz_param * Phreeqc::
pitz_param_duplicate(struct pitz_param *old_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Allocates space and makes duplicate copy of pitz_param structure
 */
	struct pitz_param *new_ptr;

	new_ptr = pitz_param_alloc();
	pitz_param_init(new_ptr);
/*
 *   Copy data
 */
	pitz_param_copy(old_ptr, new_ptr);
	return (new_ptr);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
pitz_param_copy(struct pitz_param *old_ptr, struct pitz_param *new_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Copies pitz_param data from old_ptr to new location, new_ptr.
 *   Space for the new_ptr structure must already be malloced.
 */
/*
 *   Store data for structure pitz_param
 */
	memcpy(new_ptr, old_ptr, sizeof(struct pitz_param));
	return (OK);
}

#include <list>
#include <string>
/* ---------------------------------------------------------------------- */
int Phreeqc::
pitz_param_search(struct pitz_param *pzp_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *  Does linear search of pitz_params for same type and species
 *  Returns -1 if not found, index number in pitz_params if found
 */
	int i;
	if (pzp_ptr == NULL)
		return -1;
	if (pzp_ptr->type == TYPE_Other)
		return -1;

	std::list<std::string> new_parm;

	for (i = 0; i < 3; i++)
	{
		if (pzp_ptr->species[i] != NULL) new_parm.push_back(pzp_ptr->species[i]);
	}
	new_parm.sort();

	for (i = 0; i < count_pitz_param; i++)
	{
		if (pitz_params[i]->type != pzp_ptr->type) continue;
		std::list<std::string> old_parm;
		int j;
		for (j = 0; j < 3; j++)
		{
			if (pitz_params[i]->species[j] != NULL) old_parm.push_back(pitz_params[i]->species[j]);
		}
		old_parm.sort();
		if (old_parm.size() != new_parm.size()) continue;
		bool found = true;
		std::list<std::string>::iterator nit = new_parm.begin();
		std::list<std::string>::iterator oit = old_parm.begin();
		while (nit != new_parm.end())
		{
			if (*nit != *oit) 
			{
				found = false;
				break;
			}
			nit++;
			oit++;
		}
		if (found) break;
	}
	if (i >= count_pitz_param)
	{
		return -1;
	}
	return i;
}


#include <list>
/* ---------------------------------------------------------------------- */
int Phreeqc::
sit_param_search(struct pitz_param *pzp_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *  Does linear search of pitz_params for same type and species
 *  Returns -1 if not found, index number in pitz_params if found
 */
	int i;
	if (pzp_ptr == NULL)
		return -1;
	if (pzp_ptr->type == TYPE_Other)
		return -1;

	std::list<std::string> new_parm;

	for (i = 0; i < 3; i++)
	{
		if (pzp_ptr->species[i] != NULL) new_parm.push_back(pzp_ptr->species[i]);
	}
	new_parm.sort();

	for (i = 0; i < count_sit_param; i++)
	{
		if (sit_params[i]->type != pzp_ptr->type) continue;
		std::list<std::string> old_parm;
		int j;
		for (j = 0; j < 3; j++)
		{
			if (sit_params[i]->species[j] != NULL) old_parm.push_back(sit_params[i]->species[j]);
		}
		old_parm.sort();
		if (old_parm.size() != new_parm.size()) continue;
		bool found = true;
		std::list<std::string>::iterator nit = new_parm.begin();
		std::list<std::string>::iterator oit = old_parm.begin();
		while (nit != new_parm.end())
		{
			if (*nit != *oit) 
			{
				found = false;
				break;
			}
			nit++;
			oit++;
		}
		if (found) break;
	}
	if (i >= count_sit_param)
	{
		return -1;
	}
	return i;
}

/* **********************************************************************
 *
 *   Routines related to structure "theta_parm"
 *
 * ********************************************************************** */
/* ---------------------------------------------------------------------- */
struct theta_param * Phreeqc::
theta_param_alloc(void)
/* ---------------------------------------------------------------------- */
{
	struct theta_param *theta_param_ptr;
	theta_param_ptr =
		(struct theta_param *) PHRQ_malloc(sizeof(struct theta_param));
	if (theta_param_ptr == NULL)
		malloc_error();
	return (theta_param_ptr);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
theta_param_init(struct theta_param *theta_param_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Frees all data associated with theta_param structure.
 */

	if (theta_param_ptr == NULL)
		return (ERROR);
	theta_param_ptr->zj = 0;
	theta_param_ptr->zk = 0;
	theta_param_ptr->etheta = 0;
	theta_param_ptr->ethetap = 0;
	return (OK);
}

/* ---------------------------------------------------------------------- */
struct theta_param * Phreeqc::
theta_param_search(LDBLE zj, LDBLE zk)
/* ---------------------------------------------------------------------- */
{
/*
 *  Does linear search of theta_params for same charge
 *  Returns NULL if not found, index number in theta_params if found
 */
	int i;
	for (i = 0; i < count_theta_param; i++)
	{
		if ((theta_params[i]->zj == zj && theta_params[i]->zk == zk) ||
			(theta_params[i]->zj == zk && theta_params[i]->zk == zj))
		{
			return theta_params[i];
		}
	}
	return NULL;
}
