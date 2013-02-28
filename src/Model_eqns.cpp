
#include "Model_eqns.h"
Model_eqns::Model_eqns()
{

	ah2o_unknown                    = NULL;
	alkalinity_unknown              = NULL;
	carbon_unknown                  = NULL;
	charge_balance_unknown          = NULL;
	exchange_unknown                = NULL;
	mass_hydrogen_unknown           = NULL;
	mass_oxygen_unknown             = NULL;
	mb_unknown                      = NULL;
	mu_unknown                      = NULL;
	pe_unknown                      = NULL;
	ph_unknown                      = NULL;
	pure_phase_unknown              = NULL;
	solution_phase_boundary_unknown = NULL;
	surface_unknown                 = NULL;
	gas_unknown                     = NULL;
	slack_unknown                   = NULL;
	ss_unknown                      = NULL;
	std::vector<struct unknown *> gas_unknowns;

	array = NULL;                                                    // eqn solving
	delta = NULL;                                                    // eqn solving
	residual = NULL;                                                 // eqn solving

	phreeqc_ptr = NULL;
};
Model_eqns::~Model_eqns(void)
{
};

