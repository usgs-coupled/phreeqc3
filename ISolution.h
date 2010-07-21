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
#include "NumKeyword.h"
#include "Solution.h"

class cxxISolution:public cxxSolution
{

  public:
	cxxISolution();
	cxxISolution(PHREEQC_PTR_ARG_COMMA struct solution *);
	//cxxISolution(const cxxISolution&);
	 ~cxxISolution();

	//static cxxISolution& read(CParser& parser);

	//void add(cxxISolutionComp conc)       { this->concs.push_back(conc); }

	struct solution *cxxISolution2solution(PHREEQC_PTR_ARG);

	double get_density() const
	{
		return this->density;
	}
	void set_density(double l_density)
	{
		this->density = l_density;
	}

	std::string get_units() const
	{
		return units;
	}
	void set_units(std::string l_units)
	{
		units = l_units;
	}
	void set_units(char * l_units)
	{
		if (l_units != NULL)
			this->units = std::string(l_units);
		else
			this->units.clear();
	}

	//char * get_redox()const {return this->pe[this->default_pe].get_name();}

	//void dump_xml(std::ostream& os, unsigned int indent = 0)const;

	void ConvertUnits(PHREEQC_PTR_ARG);

#ifdef ORCHESTRA
	void ORCH_write(std::ostream & chemistry_dat, std::ostream & input_dat,
					std::ostream & output_dat);
	void ORCH_write_chemistry(std::ostream & chemistry_dat);
	void ORCH_write_input(std::ostream & input_dat);
	void ORCH_write_output_vars(std::ostream & input_dat);
  private:
	void ORCH_write_chemistry_water(std::ostream & chemistry_dat);
	void ORCH_write_chemistry_primary(std::ostream & chemistry_dat);
	void ORCH_write_chemistry_total_O_H(std::ostream & chemistry_dat);
	void ORCH_write_chemistry_alkalinity(std::ostream & chemistry_dat);
	void ORCH_write_chemistry_minerals(std::ostream & chemistry_dat);
#endif

  protected:
	friend class cxxISolutionComp;	// for this->pe access
	double density;
	std::string units;
	//std::map < char *, cxxISolutionComp, CHARSTAR_LESS > comps;
	std::map < std::string, cxxISolutionComp > comps;
	struct pe_data *pes;
	int default_pe;

  public:
	//static std::map<int, cxxISolution>& map;

};

#endif // !defined(ISOLUTION_H_INCLUDED)
