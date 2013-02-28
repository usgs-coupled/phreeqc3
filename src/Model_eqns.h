#if !defined(MODEL_EQNS_H_INCLUDED)
#define MODEL_EQNS_H_INCLUDED
#include "PHRQ_base.h"
#include "Phreeqc.h"
#include <vector>
class Model_eqns: public PHRQ_base
{
public:
	Model_eqns();
	~Model_eqns();

protected:

	std::vector <struct unknown *> x;

	struct unknown *ah2o_unknown;
	struct unknown *alkalinity_unknown;
	struct unknown *carbon_unknown;
	struct unknown *charge_balance_unknown;
	struct unknown *exchange_unknown;
	struct unknown *mass_hydrogen_unknown;
	struct unknown *mass_oxygen_unknown;
	struct unknown *mb_unknown;
	struct unknown *mu_unknown;
	struct unknown *pe_unknown;
	struct unknown *ph_unknown;
	struct unknown *pure_phase_unknown;
	struct unknown *solution_phase_boundary_unknown;
	struct unknown *surface_unknown;
	struct unknown *gas_unknown;
	struct unknown *slack_unknown;
	struct unknown *ss_unknown;
	std::vector<struct unknown *> gas_unknowns;

	LDBLE * array;                                                      // eqn solving
	LDBLE * delta;                                                      // eqn solving
	LDBLE * residual;                                                   // eqn solving
	std::vector<struct species *> s_x;                                  // eqn solving
	std::vector<struct list1> sum_mb1;                                  // eqn solving
	std::vector<struct list2> sum_mb2;                                  // eqn solving
	std::vector<list0> sum_jacob0;                                      // eqn solving
	std::vector<struct list1> sum_jacob1;                               // eqn solving
	std::vector<struct list2> sum_jacob2;                               // eqn solving
	std::vector<struct list2> sum_delta;                                // eqn solving
	std::vector<struct Species_List> species_list;                      // for output
	std::map<std::string, std::vector < std::string> > sum_species_map; // for sum_species function

	Phreeqc * phreeqc_ptr;
};
#endif // !defined(DUMPER_H_INCLUDED)
