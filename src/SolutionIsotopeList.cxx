#include "Utils.h"				// define first
#include <cassert>				// assert
#include <algorithm>			// std::sort

#include "Phreeqc.h"
#include "SolutionIsotopeList.h"
#include "phqalloc.h"



cxxSolutionIsotopeList::cxxSolutionIsotopeList(PHRQ_io * io)
:
PHRQ_base(io)
	//
	// default constructor for cxxSolution 
	//
{
}

cxxSolutionIsotopeList::cxxSolutionIsotopeList(struct solution *solution_ptr, PHRQ_io * io)
:
PHRQ_base(io)
{
	int i;
	// Isotopes
	for (i = 0; i < solution_ptr->count_isotopes; i++)
	{
		cxxSolutionIsotope iso(&solution_ptr->isotopes[i], this->Get_io());
		(*this).push_back(iso);
	}
}

cxxSolutionIsotopeList::~cxxSolutionIsotopeList()
		//
		// default destructor for cxxSolution 
		//
{
}
void
cxxSolutionIsotopeList::add(cxxSolutionIsotopeList old, LDBLE intensive,
							LDBLE extensive)
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
			cxxSolutionIsotope iso(this->Get_io());
			iso.total = itold->total * extensive;
			iso.ratio = itold->ratio * intensive;
			iso.ratio_uncertainty = itold->ratio_uncertainty * intensive;
			iso.ratio_uncertainty_defined = itold->ratio_uncertainty_defined;
			this->push_back(iso);
		}
	}
}
void
cxxSolutionIsotopeList::multiply(LDBLE extensive)
{
	for (cxxSolutionIsotopeList::iterator it = this->begin();
		 it != this->end(); ++it)
	{
		it->total *= extensive;
	}
}

