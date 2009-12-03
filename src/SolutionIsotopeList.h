#if !defined(SOLUTIONISOTOPELIST_H_INCLUDED)
#define SOLUTIONISOTOPELIST_H_INCLUDED

//#define EXTERNAL extern
#include "SolutionIsotope.h"
//#include "global.h"
#include <cassert>				// assert
#include <string>				// std::string
#include <list>					// std::list

#include "Parser.h"

class cxxSolutionIsotopeList:public
	std::list <
	cxxSolutionIsotope >
{

public:
	cxxSolutionIsotopeList();

	cxxSolutionIsotopeList(struct solution *solution_ptr);

	~cxxSolutionIsotopeList();

	struct isotope * cxxSolutionIsotopeList2isotope();
	void add(cxxSolutionIsotopeList oldlist, double intensive, double extensive);
	void multiply(double extensive);

protected:


  public:


};

#endif // !defined(SOLUTIONISOTOPELIST_H_INCLUDED)
