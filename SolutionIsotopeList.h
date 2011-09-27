#if !defined(SOLUTIONISOTOPELIST_H_INCLUDED)
#define SOLUTIONISOTOPELIST_H_INCLUDED

#include <cassert>				// assert
#include <string>				// std::string
#include <list>					// std::list

#include "SolutionIsotope.h"

class cxxSolutionIsotopeList:public
	std::list <	cxxSolutionIsotope >, public PHRQ_base
{

public:
	cxxSolutionIsotopeList(PHRQ_io *io=NULL);

	cxxSolutionIsotopeList(struct solution *solution_ptr, PHRQ_io *io=NULL);

	~cxxSolutionIsotopeList();

	//struct isotope * cxxSolutionIsotopeList2isotope(PHREEQC_PTR_ARG);
	void add(cxxSolutionIsotopeList oldlist, double intensive, double extensive);
	void multiply(double extensive);

protected:


public:


};

#endif // !defined(SOLUTIONISOTOPELIST_H_INCLUDED)
