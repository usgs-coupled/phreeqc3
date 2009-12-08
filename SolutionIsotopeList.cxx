#include "Utils.h"				// define first
#include <cassert>				// assert
#include <algorithm>			// std::sort

#if !defined(PHREEQC_CLASS)
#define EXTERNAL extern
#include "global.h"
#else
#include "Phreeqc.h"
#endif
#include "SolutionIsotopeList.h"
#include "phqalloc.h"
#include "phrqproto.h"


cxxSolutionIsotopeList::cxxSolutionIsotopeList(void)
	//
	// default constructor for cxxSolution 
	//
{
}

cxxSolutionIsotopeList::~cxxSolutionIsotopeList(void)
		//
		// default destructor for cxxSolution 
		//
{
}

cxxSolutionIsotopeList::cxxSolutionIsotopeList(struct solution *solution_ptr)
{
	int i;
	// Isotopes
	for (i = 0; i < solution_ptr->count_isotopes; i++)
	{
		//cxxSolutionIsotope iso(&solution_ptr->isotopes[i]);
		(*this).push_back(&solution_ptr->isotopes[i]);
	}
}
void
cxxSolutionIsotopeList::add(cxxSolutionIsotopeList old, double intensive,
							double extensive)
{
	for (cxxSolutionIsotopeList::const_iterator itold = old.begin();
		 itold != old.end(); ++itold)
	{
		//for (std::list <cxxSolutionIsotope>::const_iterator itold = old.isotopes.begin(); itold != old.isotopes.end(); ++itold) {
		bool found = false;
		for (cxxSolutionIsotopeList::iterator it = this->begin();
			 it != this->end(); ++it)
		{
			//for (std::list <cxxSolutionIsotope>::iterator it = this->isotopes.begin(); it != this->isotopes.end(); ++it) {
			if ((it->isotope_number == itold->isotope_number) &&
				(it->elt_name == itold->elt_name) &&
				(it->isotope_name == itold->isotope_name))
			{
				it->total += itold->total * extensive;
				it->ratio += itold->ratio * intensive;
				it->ratio_uncertainty += itold->ratio_uncertainty * intensive;
				it->ratio_uncertainty_defined = (it->ratio_uncertainty_defined
												 || itold->
												 ratio_uncertainty_defined);
				found = true;
				break;
			}
		}
		if (!found)
		{
			cxxSolutionIsotope iso;
			iso.total = itold->total * extensive;
			iso.ratio = itold->ratio * intensive;
			iso.ratio_uncertainty = itold->ratio_uncertainty * intensive;
			iso.ratio_uncertainty_defined = itold->ratio_uncertainty_defined;
			this->push_back(iso);
		}
	}
}
void
cxxSolutionIsotopeList::multiply(double extensive)
{
	for (cxxSolutionIsotopeList::iterator it = this->begin();
		 it != this->end(); ++it)
	{
		it->total *= extensive;
	}
}
struct isotope *
cxxSolutionIsotopeList::cxxSolutionIsotopeList2isotope(PHREEQC_PTR_ARG)
{
	struct isotope *iso;
	if (this->size() <= 0)
	{
		return NULL;
	}
	else
	{
		iso =
			(struct isotope *)
			PHRQ_malloc((size_t) ((this->size()) * sizeof(struct isotope)));
		if (iso == NULL)
			P_INSTANCE_POINTER malloc_error();
		int i = 0;
		for (cxxSolutionIsotopeList::iterator it = this->begin();
			 it != this->end(); ++it)
		{
			iso[i].isotope_number = it->isotope_number;
			iso[i].elt_name = P_INSTANCE_POINTER string_hsave(it->elt_name.c_str());
			iso[i].total = it->total;
			iso[i].ratio = it->ratio;
			iso[i].ratio_uncertainty = it->ratio_uncertainty;
			iso[i].master = it->master(P_INSTANCE);
			iso[i].primary = it->primary(P_INSTANCE);
			i++;
		}
	}
	return (iso);
}
