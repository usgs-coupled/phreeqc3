
#include "Model_eqns.h"
#include "Solution.h"
#include "PPassemblage.h"
#include "SSassemblage.h"
#include "GasPhase.h"
#include "Utils.h"

Model_eqns::Model_eqns(Phreeqc * pptr)
{
	if (pptr == NULL)
	{
		error_msg("Model_eqns constructor needs a pointer to a Phreeqc instance.", 1);
	}
	phreeqc_ptr                        = pptr;
	ah2o_unknown_ME                    = NULL;
	alkalinity_unknown_ME              = NULL;
	carbon_unknown_ME                  = NULL;
	charge_balance_unknown_ME          = NULL;
	exchange_unknown_ME                = NULL;
	mass_hydrogen_unknown_ME           = NULL;
	mass_oxygen_unknown_ME             = NULL;
	mb_unknown_ME                      = NULL;
	mu_unknown_ME                      = NULL;
	pe_unknown_ME                      = NULL;
	ph_unknown_ME                      = NULL;
	pure_phase_unknown_ME              = NULL;
	solution_phase_boundary_unknown_ME = NULL;
	surface_unknown_ME                 = NULL;
	gas_unknown_ME                     = NULL;
	slack_unknown_ME                   = NULL;
	ss_unknown_ME                      = NULL;
	//std::vector<struct unknown *> gas_unknowns_ME;
	s_diff_layer_ME                    = NULL;

	array_ME = NULL;                                                    // eqn solving
	delta_ME = NULL;                                                    // eqn solving
	residual_ME = NULL;                                                 // eqn solving
	max_unknowns_ME = 0;

	Copy_phreeqc_model();
}
Model_eqns::~Model_eqns(void)
{
	

	for (size_t i = 0; i < this->x_ME.size(); i++)
	{
		phreeqc_ptr->unknown_free(x_ME[i]);
	}
	x_ME.clear();

	array_ME = (LDBLE *) phreeqc_ptr->free_check_null(array_ME);
	delta_ME = (LDBLE *) phreeqc_ptr->free_check_null(delta_ME);
	residual_ME = (LDBLE *) phreeqc_ptr->free_check_null(residual_ME);

	s_x_ME.clear();
	sum_mb1_ME.clear();
	sum_mb2_ME.clear();
	sum_jacob0_ME.clear();
	sum_jacob1_ME.clear();
	sum_jacob2_ME.clear();
	sum_delta_ME.clear();
	species_list_ME.clear();
	sum_species_map_ME.clear();

	// master
	for (size_t i = 0; i < master_ME.size(); i++)
	{
		phreeqc_ptr->rxn_free(master_ME[i].rxn_secondary);
	}
	master_ME.clear();

	// species
	for (size_t i = 0; i < s_ME.size(); i++)
	{
		phreeqc_ptr->rxn_free(s_ME[i].rxn_x);
		phreeqc_ptr->free_check_null(s_ME[i].next_sys_total);
	}
	s_ME.clear();

	// phases
	for (size_t i = 0; i < phases_ME.size(); i++)
	{
		phreeqc_ptr->rxn_free(phases_ME[i].rxn_x);
		phreeqc_ptr->free_check_null(phases_ME[i].next_sys_total);
	}
	phases_ME.clear();
	if (s_diff_layer_ME != NULL)
	{
		s_diff_layer_ME->clear();
		delete s_diff_layer_ME;
	}

	return;
}
void Model_eqns::
Copy_phreeqc_model(void)
{
	this->x_ME                         = phreeqc_ptr->x;
	ah2o_unknown_ME		               = phreeqc_ptr->ah2o_unknown;
	alkalinity_unknown_ME		       = phreeqc_ptr->alkalinity_unknown;
	carbon_unknown_ME		           = phreeqc_ptr->carbon_unknown;
	charge_balance_unknown_ME	       = phreeqc_ptr->charge_balance_unknown;
	exchange_unknown_ME		           = phreeqc_ptr->exchange_unknown;
	mass_hydrogen_unknown_ME	       = phreeqc_ptr->mass_hydrogen_unknown;
	mass_oxygen_unknown_ME		       = phreeqc_ptr->mass_oxygen_unknown;
	mb_unknown_ME			           = phreeqc_ptr->mb_unknown;
	mu_unknown_ME			           = phreeqc_ptr->mu_unknown;
	pe_unknown_ME			           = phreeqc_ptr->pe_unknown;
	ph_unknown_ME			           = phreeqc_ptr->ph_unknown;
	pure_phase_unknown_ME		       = phreeqc_ptr->pure_phase_unknown;
	solution_phase_boundary_unknown_ME = phreeqc_ptr->solution_phase_boundary_unknown;
	surface_unknown_ME		           = phreeqc_ptr->surface_unknown;
	gas_unknown_ME			           = phreeqc_ptr->gas_unknown;
	slack_unknown_ME		           = phreeqc_ptr->slack_unknown;
	ss_unknown_ME			           = phreeqc_ptr->ss_unknown;
	gas_unknowns_ME                    = phreeqc_ptr->gas_unknowns;

	array_ME                           = phreeqc_ptr->array;               // eqn solving
	delta_ME                           = phreeqc_ptr->delta;               // eqn solving
	residual_ME                        = phreeqc_ptr->residual;            // eqn solving
	max_unknowns_ME                    = phreeqc_ptr->max_unknowns;

	// master
	for (int i = 0; i < phreeqc_ptr->count_master; i++)
	{
		if (phreeqc_ptr->master[i]->in == TRUE || phreeqc_ptr->master[i]->unknown != NULL)
		{
			master_ME.push_back(*phreeqc_ptr->master[i]);
			master_ME.back().rxn_secondary = phreeqc_ptr->rxn_dup(phreeqc_ptr->master[i]->rxn_secondary);
		}
	}
	this->master_x_ME = phreeqc_ptr->master_x;
	// species
	for (int i = 0; i < phreeqc_ptr->count_s; i++)
	{
		if (phreeqc_ptr->s[i]->in == TRUE)
		{
			s_ME.push_back(*phreeqc_ptr->s[i]);
			s_ME.back().rxn_x = phreeqc_ptr->rxn_dup(phreeqc_ptr->s[i]->rxn_x);
			s_ME.back().next_sys_total = phreeqc_ptr->elt_list_dup(phreeqc_ptr->s[i]->next_sys_total);
		}
	}
	// phases
	for (int i = 0; i < phreeqc_ptr->count_phases; i++)
	{
		if (phreeqc_ptr->phases[i]->in == TRUE)
		{
			phases_ME[i] = *phreeqc_ptr->phases[i];
			phases_ME[i].rxn_x = phreeqc_ptr->rxn_dup(phreeqc_ptr->phases[i]->rxn_x);
			phases_ME[i].next_sys_total = phreeqc_ptr->elt_list_dup(phreeqc_ptr->phases[i]->next_sys_total);
		}
	}

	pe_x_ME                             = phreeqc_ptr->pe_x;
	default_pe_x_ME                     = phreeqc_ptr->default_pe_x;
	s_x_ME                              = phreeqc_ptr->s_x;                // eqn solving
	sum_mb1_ME                          = phreeqc_ptr->sum_mb1;            // eqn solving
	sum_mb2_ME                          = phreeqc_ptr->sum_mb2;            // eqn solving
	sum_jacob0_ME                       = phreeqc_ptr->sum_jacob0;         // eqn solving
	sum_jacob1_ME                       = phreeqc_ptr->sum_jacob1;         // eqn solving
	sum_jacob2_ME                       = phreeqc_ptr->sum_jacob2;         // eqn solving
	sum_delta_ME                        = phreeqc_ptr->sum_delta;          // eqn solving
	species_list_ME                     = phreeqc_ptr->species_list;       // eqn solving
	sum_species_map_ME                  = phreeqc_ptr->sum_species_map;    // eqn solving
	s_diff_layer_ME                     = phreeqc_ptr->s_diff_layer;

	model_id = phreeqc_ptr->current_model_id;

}
void Model_eqns::
Copy_to_phreeqc(void)
{
	phreeqc_ptr->x                               = this->x_ME;
	phreeqc_ptr->ah2o_unknown	                 = ah2o_unknown_ME;		     
	phreeqc_ptr->alkalinity_unknown	             = alkalinity_unknown_ME;		     
	phreeqc_ptr->carbon_unknown	                 = carbon_unknown_ME;		     
	phreeqc_ptr->charge_balance_unknown	         = charge_balance_unknown_ME;	     
	phreeqc_ptr->exchange_unknown	             = exchange_unknown_ME;		     
	phreeqc_ptr->mass_hydrogen_unknown	         = mass_hydrogen_unknown_ME;	     
	phreeqc_ptr->mass_oxygen_unknown	         = mass_oxygen_unknown_ME;		
	phreeqc_ptr->mb_unknown	                     = mb_unknown_ME;			     
	phreeqc_ptr->mu_unknown	                     = mu_unknown_ME;			     
	phreeqc_ptr->pe_unknown	                     = pe_unknown_ME;			     
	phreeqc_ptr->ph_unknown	                     = ph_unknown_ME;			     
	phreeqc_ptr->pure_phase_unknown	             = pure_phase_unknown_ME;		     
	phreeqc_ptr->solution_phase_boundary_unknown = solution_phase_boundary_unknown_ME; 
	phreeqc_ptr->surface_unknown	             = surface_unknown_ME;		     
	phreeqc_ptr->gas_unknown	                 = gas_unknown_ME;			
	phreeqc_ptr->slack_unknown	                 = slack_unknown_ME;		     
	phreeqc_ptr->ss_unknown	                     = ss_unknown_ME;			     
	phreeqc_ptr->gas_unknowns	                 = gas_unknowns_ME;                    

	phreeqc_ptr->array                           = array_ME;               // eqn solving
	phreeqc_ptr->delta                           = delta_ME;               // eqn solving
	phreeqc_ptr->residual                        = residual_ME;            // eqn solving
	phreeqc_ptr->max_unknowns                    = max_unknowns_ME;

	// master
	for (int i = 0; i < phreeqc_ptr->count_master; i++)
	{
		phreeqc_ptr->master[i]->in = FALSE;
	}
	for (size_t i = 0; i < master_ME.size(); i++)
	{
		int n = master_ME[i].number;	
		phreeqc_ptr->rxn_free(phreeqc_ptr->master[n]->rxn_secondary);
		double total = phreeqc_ptr->master[n]->total;
		memcpy(phreeqc_ptr->master[n], &master_ME[i], sizeof(struct master));
		phreeqc_ptr->master[n]->rxn_secondary = phreeqc_ptr->rxn_dup(master_ME[i].rxn_secondary);
		//phreeqc_ptr->master[n]->unknown = master_ME[i].unknown;
		//phreeqc_ptr->master[n]->pe_rxn = master_ME[i].pe_rxn;
		phreeqc_ptr->master[n]->total = total;
	}
	phreeqc_ptr->master_x = this->master_x_ME;
	// species
	for (int i = 0; i < phreeqc_ptr->count_s; i++)
	{
		phreeqc_ptr->s[i]->in = FALSE;
	}
	for (size_t i = 0; i < s_ME.size(); i++)
	{
		int n = s_ME[i].number;
		phreeqc_ptr->rxn_free(phreeqc_ptr->s[n]->rxn_x);
		phreeqc_ptr->s[n]->next_sys_total = (struct elt_list *) phreeqc_ptr->free_check_null(phreeqc_ptr->s[n]->next_sys_total);
		memcpy(phreeqc_ptr->s[n], &s_ME[i], sizeof(struct species));
		phreeqc_ptr->s[n]->rxn_x = phreeqc_ptr->rxn_dup(s_ME[i].rxn_x);
		phreeqc_ptr->s[n]->next_sys_total = phreeqc_ptr->elt_list_dup(s_ME[i].next_sys_total);
	}
	// phases
	for (int i = 0; i < phreeqc_ptr->count_phases; i++)
	{
		phreeqc_ptr->phases[i]->in = FALSE;
	}
	std::map<int, struct phase>::iterator it;
	for (it = phases_ME.begin(); it != phases_ME.end(); it++)
	{
		int i = it->first;
		phreeqc_ptr->rxn_free(phreeqc_ptr->phases[i]->rxn_x);
		phreeqc_ptr->phases[i]->next_sys_total = (struct elt_list *) phreeqc_ptr->free_check_null(phreeqc_ptr->phases[i]->next_sys_total);
		memcpy(phreeqc_ptr->phases[i], &it->second, sizeof(struct phase));
		phreeqc_ptr->phases[i]->rxn_x = phreeqc_ptr->rxn_dup(it->second.rxn_x);
		phreeqc_ptr->phases[i]->next_sys_total = phreeqc_ptr->elt_list_dup(it->second.next_sys_total);
	}

	phreeqc_ptr->pe_x                             = pe_x_ME;
	phreeqc_ptr->default_pe_x                     = default_pe_x_ME;
	phreeqc_ptr->s_x                              = s_x_ME;                // eqn solving
	phreeqc_ptr->sum_mb1                          = sum_mb1_ME;            // eqn solving
	phreeqc_ptr->sum_mb2                          = sum_mb2_ME;            // eqn solving
	phreeqc_ptr->sum_jacob0                       = sum_jacob0_ME;         // eqn solving
	phreeqc_ptr->sum_jacob1                       = sum_jacob1_ME;         // eqn solving
	phreeqc_ptr->sum_jacob2                       = sum_jacob2_ME;         // eqn solving
	phreeqc_ptr->sum_delta                        = sum_delta_ME;          // eqn solving
	phreeqc_ptr->species_list                     = species_list_ME;       // eqn solving
	phreeqc_ptr->sum_species_map                  = sum_species_map_ME;    // eqn solving
	phreeqc_ptr->s_diff_layer                     = s_diff_layer_ME;
	phreeqc_ptr->s_diff_layer                     = s_diff_layer_ME;
}
void Model_eqns::
Reprep_model(void)
{
	//this->x_ME                         = phreeqc_ptr->x;
	//ah2o_unknown_ME		               = phreeqc_ptr->ah2o_unknown;
	//alkalinity_unknown_ME		       = phreeqc_ptr->alkalinity_unknown;
	//carbon_unknown_ME		           = phreeqc_ptr->carbon_unknown;
	//charge_balance_unknown_ME	       = phreeqc_ptr->charge_balance_unknown;
	//exchange_unknown_ME		           = phreeqc_ptr->exchange_unknown;
	//mass_hydrogen_unknown_ME	       = phreeqc_ptr->mass_hydrogen_unknown;
	//mass_oxygen_unknown_ME		       = phreeqc_ptr->mass_oxygen_unknown;
	//mb_unknown_ME			           = phreeqc_ptr->mb_unknown;
	//mu_unknown_ME			           = phreeqc_ptr->mu_unknown;
	//pe_unknown_ME			           = phreeqc_ptr->pe_unknown;
	//ph_unknown_ME			           = phreeqc_ptr->ph_unknown;
	//pure_phase_unknown_ME		       = phreeqc_ptr->pure_phase_unknown;
	//solution_phase_boundary_unknown_ME = phreeqc_ptr->solution_phase_boundary_unknown;
	//surface_unknown_ME		           = phreeqc_ptr->surface_unknown;
	//gas_unknown_ME			           = phreeqc_ptr->gas_unknown;
	//slack_unknown_ME		           = phreeqc_ptr->slack_unknown;
	//ss_unknown_ME			           = phreeqc_ptr->ss_unknown;
	//gas_unknowns_ME                    = phreeqc_ptr->gas_unknowns;

	//array_ME                           = phreeqc_ptr->array;               // eqn solving
	//delta_ME                           = phreeqc_ptr->delta;               // eqn solving
	//residual_ME                        = phreeqc_ptr->residual;            // eqn solving
	//max_unknowns_ME                    = phreeqc_ptr->max_unknowns;

	// master
	for (size_t i = 0; i < master_ME.size(); i++)
	{
		phreeqc_ptr->rxn_free(master_ME[i].rxn_secondary);
	}
	master_ME.clear();
	for (int i = 0; i < phreeqc_ptr->count_master; i++)
	{
		if (phreeqc_ptr->master[i]->in == TRUE || phreeqc_ptr->master[i]->unknown != NULL)
		{
			master_ME.push_back(*phreeqc_ptr->master[i]);
			master_ME.back().rxn_secondary = phreeqc_ptr->rxn_dup(phreeqc_ptr->master[i]->rxn_secondary);
		}
	}
	// species
	for (size_t i = 0; i < s_ME.size(); i++)
	{
		phreeqc_ptr->rxn_free(s_ME[i].rxn_x);
		phreeqc_ptr->free_check_null(s_ME[i].next_sys_total);
	}
	s_ME.clear();
	for (int i = 0; i < phreeqc_ptr->count_s; i++)
	{
		if (phreeqc_ptr->s[i]->in == TRUE)
		{
			s_ME.push_back(*phreeqc_ptr->s[i]);
			s_ME.back().rxn_x = phreeqc_ptr->rxn_dup(phreeqc_ptr->s[i]->rxn_x);
			s_ME.back().next_sys_total = phreeqc_ptr->elt_list_dup(phreeqc_ptr->s[i]->next_sys_total);
		}
	}
	// phases
	for (size_t i = 0; i < phases_ME.size(); i++)
	{
		phreeqc_ptr->rxn_free(phases_ME[i].rxn_x);
		phreeqc_ptr->free_check_null(phases_ME[i].next_sys_total);
	}
	phases_ME.clear();
	for (int i = 0; i < phreeqc_ptr->count_phases; i++)
	{
		if (phreeqc_ptr->phases[i]->in == TRUE)
		{
			phases_ME[i] = *phreeqc_ptr->phases[i];
			phases_ME[i].rxn_x = phreeqc_ptr->rxn_dup(phreeqc_ptr->phases[i]->rxn_x);
			phases_ME[i].next_sys_total = phreeqc_ptr->elt_list_dup(phreeqc_ptr->phases[i]->next_sys_total);
		}
	}

	pe_x_ME                             = phreeqc_ptr->pe_x;
	default_pe_x_ME                     = phreeqc_ptr->default_pe_x;
	s_x_ME                              = phreeqc_ptr->s_x;                // eqn solving
	sum_mb1_ME                          = phreeqc_ptr->sum_mb1;            // eqn solving
	sum_mb2_ME                          = phreeqc_ptr->sum_mb2;            // eqn solving
	sum_jacob0_ME                       = phreeqc_ptr->sum_jacob0;         // eqn solving
	sum_jacob1_ME                       = phreeqc_ptr->sum_jacob1;         // eqn solving
	sum_jacob2_ME                       = phreeqc_ptr->sum_jacob2;         // eqn solving
	sum_delta_ME                        = phreeqc_ptr->sum_delta;          // eqn solving
	species_list_ME                     = phreeqc_ptr->species_list;       // eqn solving
	sum_species_map_ME                  = phreeqc_ptr->sum_species_map;    // eqn solving
	//s_diff_layer_ME                     = phreeqc_ptr->s_diff_layer;

	//model_id = phreeqc_ptr->current_model_id;

}