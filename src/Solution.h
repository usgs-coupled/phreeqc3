#if !defined(SOLUTION_H_INCLUDED)
#define SOLUTION_H_INCLUDED

#include "NumKeyword.h"
#include "SolutionIsotopeList.h"
#include "NameDouble.h"
#include "cxxMix.h"

//#define EXTERNAL extern
//#include "global.h"
#include <cassert> // assert
#include <map>     // std::map
#include <string>  // std::string
#include <vector>  // std::vector
#include <iostream>

#include "char_star.h"

class cxxSolution : public cxxNumKeyword
{

public:
        cxxSolution();
        cxxSolution(struct solution *);
	cxxSolution(const std::map<int, cxxSolution> &solution_map, cxxMix &mx, int n_user);
        ~cxxSolution();

        //static cxxSolution& read(CParser& parser);

        double get_tc()const {return this->tc;}
        void set_tc(double tc) {this->tc = tc;}

        double get_ph()const {return this->ph;}
        void set_ph(double pH) {this->ph = pH;}

        double get_pe()const {return this->pe;}
        void set_pe(double pe) {this->pe =pe;}

        double get_mu()const {return this->mu;}
        void set_mu(double mu) {this->mu = mu;}

        double get_ah2o()const {return this->ah2o;}
        void set_ah2o(double ah2o) {this->ah2o = ah2o;}

        double get_total_h()const {return this->total_h;}
        void set_total_h(double total_h) {this->total_h = total_h;}

        double get_total_o()const {return this->total_o;}
        void set_total_o(double total_o) {this->total_o = total_o;}

        double get_cb()const {return this->cb;}
        void set_cb(double cb) {this->cb = cb;}

        double get_mass_water()const {return this->mass_water;}
        void set_mass_water(long double mass_water) {this->mass_water = mass_water;}

        double get_total_alkalinity()const {return this->total_alkalinity;}
        void set_total_alkalinity(double total_alkalinity) {this->total_alkalinity = total_alkalinity;}

	double get_total(char *string)const;
	void set_total(char *string, double value);

	double get_master_activity(char *string)const;
	void set_master_activity(char *string, double value);

	/*
	double get_species_gamma(char *string)const;
	void set_species_gamma(char *string, double value);

	double get_isotope(char *string)const;
	void set_isotope(char *string, double value);
	*/

        struct solution *cxxSolution2solution();

	void dump_raw(std::ostream& s_oss, unsigned int indent)const;
	void write_orchestra(std::ostream& headings, std::ostream& input_data)const;

        void read_raw(CParser& parser);


#ifdef USE_MPI
        void mpi_pack(std::vector<int>& ints, std::vector<double>& doubles);
        void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);
        void mpi_send(int task_number);
        void mpi_recv(int task_number);
#endif

private:
        void zero();
	void add(const cxxSolution &addee, double extensive);
	// not checked
        void dump_xml(std::ostream& os, unsigned int indent = 0)const;

protected:
        double tc;
        double ph;
        double pe;
        double mu;
        double ah2o;
        double total_h;
        double total_o;
        double cb;
        double mass_water;
        double total_alkalinity;
        cxxNameDouble totals;
        //std::list<cxxSolutionIsotope> isotopes;
        cxxNameDouble master_activity;
        cxxNameDouble species_gamma;
	cxxSolutionIsotopeList isotopes;

};

#endif // !defined(SOLUTION_H_INCLUDED)
