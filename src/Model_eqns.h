#if !defined(MODEL_EQNS_H_INCLUDED)
#define MODEL_EQNS_H_INCLUDED
#include "PHRQ_base.h"
#include "Phreeqc.h"
#include <vector>
class Model_eqns: public PHRQ_base
{
public:
	Model_eqns(Phreeqc * pptr);
	~Model_eqns();
	void Copy_phreeqc_model(void);
	void Copy_to_phreeqc(void);

protected:

	std::vector <struct unknown *> x_ME;

	struct unknown *ah2o_unknown_ME;
	struct unknown *alkalinity_unknown_ME;
	struct unknown *carbon_unknown_ME;
	struct unknown *charge_balance_unknown_ME;
	struct unknown *exchange_unknown_ME;
	struct unknown *mass_hydrogen_unknown_ME;
	struct unknown *mass_oxygen_unknown_ME;
	struct unknown *mb_unknown_ME;
	struct unknown *mu_unknown_ME;
	struct unknown *pe_unknown_ME;
	struct unknown *ph_unknown_ME;
	struct unknown *pure_phase_unknown_ME;
	struct unknown *solution_phase_boundary_unknown_ME;
	struct unknown *surface_unknown_ME;
	struct unknown *gas_unknown_ME;
	struct unknown *slack_unknown_ME;
	struct unknown *ss_unknown_ME;
	std::vector<struct unknown *> gas_unknowns_ME;

	LDBLE * array_ME;                                                      // eqn solving
	LDBLE * delta_ME;                                                      // eqn solving
	LDBLE * residual_ME;                                                   // eqn solving

	std::vector<struct master> master_ME;
	std::vector<struct species> s_ME;
	std::map<int, struct phase> phases_ME;

	std::map < std::string, cxxChemRxn > pe_x_ME;
	std::string default_pe_x_ME;
	std::vector<struct species *> s_x_ME;                                  // eqn solving
	std::vector<struct list1> sum_mb1_ME;                                  // eqn solving
	std::vector<struct list2> sum_mb2_ME;                                  // eqn solving
	std::vector<list0> sum_jacob0_ME;                                      // eqn solving
	std::vector<struct list1> sum_jacob1_ME;                               // eqn solving
	std::vector<struct list2> sum_jacob2_ME;                               // eqn solving
	std::vector<struct list2> sum_delta_ME;                                // eqn solving
	std::vector<struct Species_List> species_list_ME;                      // for output
	std::map<std::string, std::vector < std::string> > sum_species_map_ME; // for sum_species function

	Phreeqc * phreeqc_ptr;
};
#endif // !defined(DUMPER_H_INCLUDED)
