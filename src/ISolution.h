#if !defined(ISOLUTION_H_INCLUDED)
#define ISOLUTION_H_INCLUDED

//#include "Parser.h"
#include "ISolutionComp.h"
#include "NumKeyword.h"
#include "Solution.h"
//#include "Isotope.h"
#include <cassert> // assert
#include <map>     // std::map
#include <string>  // std::string
#include <list>    // std::list
#include <vector>  // std::vector
#include <set>     // std::set
#include <iostream>
#include <sstream>
#include <fstream>
class cxxISolution : public cxxSolution
{

public:
        cxxISolution();
        cxxISolution(struct solution *);
        //cxxISolution(const cxxISolution&);
        ~cxxISolution();

        //static cxxISolution& read(CParser& parser);

        //void add(cxxISolutionComp conc)       { this->concs.push_back(conc); }

        struct solution *cxxISolution2solution();

        double get_density()const {return this->density;}
        void set_density(double density) {this->density = density;}

        std::string get_units()const {return units;}
        void set_units(std::string units) {units = units;}

        //char * get_redox()const {return this->pe[this->default_pe].get_name();}

        //void dump_xml(std::ostream& os, unsigned int indent = 0)const;

	void ConvertUnits();

#ifdef ORCHESTRA
	void ORCH_write(std::ostream &chemistry_dat, std::ostream &input_dat, std::ostream &output_dat);
	void ORCH_write_chemistry(std::ostream &chemistry_dat);
	void ORCH_write_input(std::ostream &input_dat);
	void ORCH_write_output_vars(std::ostream &input_dat);
private:
  void ORCH_write_chemistry_water(std::ostream &chemistry_dat);
  void ORCH_write_chemistry_primary(std::ostream &chemistry_dat);
  void ORCH_write_chemistry_total_O_H(std::ostream &chemistry_dat);
  void ORCH_write_chemistry_alkalinity(std::ostream &chemistry_dat);
  void ORCH_write_chemistry_minerals(std::ostream &chemistry_dat);
#endif

protected:
        friend class cxxISolutionComp; // for this->pe access
        double density;
        std::string units;
        std::map<char *, cxxISolutionComp, CHARSTAR_LESS> comps; 
        struct pe_data *pes;
        int default_pe;

public:
        //static std::map<int, cxxISolution>& map;

};

#endif // !defined(ISOLUTION_H_INCLUDED)
