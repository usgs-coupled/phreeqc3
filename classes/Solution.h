#if !defined(SOLUTION_H_INCLUDED)
#define SOLUTION_H_INCLUDED

#include "NumKeyword.h"
#include "Parser.h"
#include "Conc.h"
#include "Isotope.h"
#include "Pe_Data.h"
#include <cassert> // assert
#include <map>     // std::map
#include <string>  // std::string
#include <list>    // std::list
#include <vector>  // std::vector


class CSolution : public CNumKeyword
{
public:
	CSolution();
	~CSolution();

	static CSolution& read(CParser& parser);

	void add(CConc conc)       { this->totals.push_back(conc); }
	void add(CIsotope isotope) { this->isotopes.push_back(isotope); }

	double get_ph()const {return this->ph;}
	void set_ph(double pH) {this->ph = pH;}

	double get_solution_pe()const {return this->solution_pe;}
	void set_solution_pe(double solution_pe) {this->solution_pe = solution_pe;}

	double get_tc()const {return this->tc;}
	void set_tc(double tc) {this->tc = tc;}

	double get_density()const {return this->density;}
	void set_density(double density) {this->density = density;}

	std::string get_units()const {return this->units;}
	void set_units(std::string units) {this->units = units;}

	std::string get_redox()const {return this->pe[this->default_pe].get_name();}

	long double get_mass_water()const {return this->mass_water;};
	void set_mass_water(long double mass_water) {this->mass_water = mass_water;};

	void dump_xml(std::ostream& os, unsigned int indent = 0)const;

protected:
	friend class CConc; // for this->pe access
	double ph;
	double tc;
	double solution_pe;
	double mu;
	double ah2o;
	double density;
	long double mass_water;
	std::string units;


	std::vector<CConc> totals; /// std::set<CConc> m_totals; ////std::list<CConc> m_totals;
	std::vector<CPe_Data> pe;
	int default_pe;

	std::list<CIsotope> isotopes;

public:
	static std::map<int, CSolution>& map;

};

#endif // !defined(SOLUTION_H_INCLUDED)
