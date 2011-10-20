#if !defined(ISOLUTION_H_INCLUDED)
#define ISOLUTION_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector
#include <set>					// std::set
#include <iostream>
#include <sstream>
#include <fstream>

#include "ISolutionComp.h"
//#include "NumKeyword.h"
#include "Solution.h"

class cxxISolution:public cxxSolution
{

  public:
	cxxISolution(PHRQ_io *io=NULL);
	 ~cxxISolution();
	//static cxxISolution& read(CParser& parser);
	//void add(cxxISolutionComp conc)       { this->concs.push_back(conc); }

	double Get_density() const
	{
		return this->density;
	}
	void Set_density(double l_density)
	{
		this->density = l_density;
	}

	std::string Get_units() const
	{
		return units;
	}
	void Set_units(std::string l_units)
	{
		units = l_units;
	}
	void Set_units(char * l_units)
	{
		if (l_units != NULL)
			this->units = std::string(l_units);
		else
			this->units.clear();
	}

	//char * get_redox()const {return this->pe[this->default_pe].get_name();}
	//void dump_xml(std::ostream& os, unsigned int indent = 0)const;
	//void ConvertUnits(PHREEQC_PTR_ARG);

  protected:
	friend class cxxISolutionComp;	// for this->pe access
	double density;
	std::string units;
	std::map < std::string, cxxISolutionComp > comps;
	struct pe_data *pes;
	int default_pe;

};

#endif // !defined(ISOLUTION_H_INCLUDED)
