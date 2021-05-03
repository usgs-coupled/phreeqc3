#include "Utils.h"
#include "Phreeqc.h"
#include <iostream>

#include "phqalloc.h"
#include "Temperature.h"
#include "cxxMix.h"
#include "Exchange.h"
#include "GasPhase.h"
#include "Reaction.h"
#include "PPassemblage.h"
#include "Use.h"
#include "SSassemblage.h"
#include "cxxKinetics.h"
#include "Surface.h"
#include "Solution.h"

#if defined(PHREEQCI_GUI)
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#endif

/* ---------------------------------------------------------------------- */
int Phreeqc::
clean_up(void)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Free all allocated memory, except strings
	 */
	int i, j;
#if defined MULTICHART
	chart_handler.End_timer();
	output_flush();
#if 0
	// Wait for charts to end
	while (0 != this->chart_handler.Get_active_charts())
	{
		System::Threading::Thread::Sleep(60);
	}
#endif
#endif

	isotopes_x.clear();
	/* model */
	last_model.gas_phase.clear();
	last_model.pp_assemblage.clear();
	last_model.add_formula.clear();
	last_model.si.clear();
	last_model.ss_assemblage.clear();
	last_model.surface_comp.clear();
	last_model.surface_charge.clear();
	/* model */
	free_model_allocs();

	/* species */

	for (j = 0; j < (int)s.size(); j++)
	{
		//s_free(s[j]);
		delete s[j];
	}
	s.clear();

	/* master species */

	for (j = 0; j < (int)master.size(); j++)
	{
		//master_free(master[j]);
		delete master[j];
	}
	master.clear();

	/* elements */

	for (j = 0; j < (int)elements.size(); j++)
	{
		delete elements[j];
	}
	elements.clear();
	/* solutions */
	Rxn_solution_map.clear();
	/* surfaces */
	Rxn_surface_map.clear();
	/* exchange */
	Rxn_exchange_map.clear();
	/* pp assemblages */
	Rxn_pp_assemblage_map.clear();
	/* s_s assemblages */
	Rxn_ss_assemblage_map.clear();
	/* irreversible reactions */
	Rxn_reaction_map.clear();
	/* temperature */
	Rxn_temperature_map.clear();
	/* pressure */
	Rxn_pressure_map.clear();
	/* unknowns */
	for (j = 0; j < (int)x.size(); j++)
	{
		unknown_free(x[j]);
	}
	x.clear();
	/* mixtures */
	Rxn_mix_map.clear();
	/* phases */
	for (j = 0; j < (int)phases.size(); j++)
	{
		//phase_free(phases[j]);
		delete phases[j];
	}
	phases.clear();
	// inverse
	inverse.clear();
	/* gases */
	Rxn_gas_phase_map.clear();
	/* kinetics */
	Rxn_kinetics_map.clear();
	x0_moles.clear();
	m_temp.clear();
	m_original.clear();
	rk_moles.clear();
	/* rates */
	for (j = 0; j < (int)rates.size(); j++)
	{
		rate_free(&rates[j]);
	}
	rates.clear();
	/* logk table */
	Logk_map.clear();
	save_values.clear();
	/* working pe*/
	pe_x.clear();
	/*species_list*/
	species_list.clear();
	/* transport data */
	cell_data.clear();
	/* advection */
	advection_punch.clear();
	advection_print.clear();
	/* selected_output */
	SelectedOutput_map.clear();
	/*  user_print and user_punch */
	UserPunch_map.clear();
	rate_free(user_print);
	delete user_print;
	/*
	   Clear llnl aqueous model parameters
	 */
	llnl_temp.clear();
	llnl_adh.clear();
	llnl_bdh.clear();
	llnl_bdot.clear();
	llnl_co2_coefs.clear();
	/* master_isotope */
	for (i = 0; i < (int)master_isotope.size(); i++)
	{
		//delete master_isotope[i];
	}
	master_isotope.clear();
	master_isotope_map.clear();
	/* calculate_value */
	for (i = 0; i < (int)calculate_value.size(); i++)
	{
		calculate_value_free(calculate_value[i]);
		delete calculate_value[i];
	}
	calculate_value.clear();
	calculate_value_map.clear();
	/* isotope_ratio */
	for (i = 0; i < (int)isotope_ratio.size(); i++)
	{
		delete isotope_ratio[i];
	}
	isotope_ratio.clear();
	isotope_ratio_map.clear();
	/* isotope_alpha */
	for (i = 0; i < (int)isotope_alpha.size(); i++)
	{
		delete isotope_alpha[i];
	}
	isotope_alpha.clear();
	isotope_alpha_map.clear();
	/* tally table */
	free_tally_table();
	/* CVODE memory */
	free_cvode();
	/* pitzer */
	pitzer_clean_up();
	/* sit */
	sit_clean_up();
	/* elements, species, phases*/
	elements_map.clear();
	species_map.clear();
	phases_map.clear();
	Logk_map.clear();
	/* strings */
	strings_map_clear();
	/* delete basic interpreter */
	basic_free();
	/* change_surf */
	change_surf = (struct Change_Surf *) free_check_null(change_surf);
	/* miscellaneous work space */
	elt_list.clear();
	trxn.tokens.clear();
	mb_unknowns.clear();
	line = (char *) free_check_null(line);
	line_save = (char *) free_check_null(line_save);
	/* free user database name if defined */
	dump_file_name = (char *) free_check_null(dump_file_name);
#ifdef PHREEQCI_GUI
	free_spread();
#endif
	title_x.clear(); 
	last_title_x.clear();
	count_inverse = 0;

	sformatf_buffer = (char *) free_check_null(sformatf_buffer);
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
reinitialize(void)
/* ---------------------------------------------------------------------- */
{
	/* solutions */
	Rxn_solution_map.clear();
	/* surfaces */
	Rxn_surface_map.clear();
	/* exchange */
	Rxn_exchange_map.clear();
	/* pp assemblages */
	Rxn_pp_assemblage_map.clear();
	/* s_s assemblages */
	Rxn_ss_assemblage_map.clear();
	/* gases */
	Rxn_gas_phase_map.clear();
	/* kinetics */
	Rxn_kinetics_map.clear();
	/* irreversible reactions */
	Rxn_reaction_map.clear();
	// Temperature
	Rxn_temperature_map.clear();
	// Pressure
	Rxn_pressure_map.clear();
	return (OK);
}
/* **********************************************************************
 *
 *   Routines related to CReaction
 *
 * ********************************************************************** */

/* ---------------------------------------------------------------------- */
double Phreeqc::
rxn_find_coef(CReaction& r_ref, const char* str)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Finds coefficient of token in reaction.
	 *   input: r_ptr, pointer to a reaction structure
	 *	  str, string to find as reaction token
	 *
	 *   Return: 0.0, if token not found
	 *	   coefficient of token, if found.
	 */
	class rxn_token* r_token;
	LDBLE coef;

	r_token = &r_ref.Get_tokens()[1];
	coef = 0.0;
	while (!r_token->Get_end())
	{
		if (strcmp(r_token->Get_s()->name.c_str(), str) == 0)
		{
			coef = r_token->coef;
			break;
		}
		r_token++;
	}
	return (coef);
}



/* ---------------------------------------------------------------------- */
int Phreeqc::
isotope_compare(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
	int i;
	const class isotope *iso_ptr1, *iso_ptr2;

	iso_ptr1 = (const class isotope *) ptr1;
	iso_ptr2 = (const class isotope *) ptr2;
	i = strcmp_nocase(iso_ptr1->elt_name.c_str(), iso_ptr2->elt_name.c_str());
	if (i != 0)
		return (i);
	if (iso_ptr1->isotope_number < iso_ptr2->isotope_number)
	{
		return (-1);
	}
	else if (iso_ptr1->isotope_number > iso_ptr2->isotope_number)
	{
		return (1);
	}
	return (0);
}
/* **********************************************************************
 *
 *   Routines related to structure "species_list"
 *
 * ********************************************************************** */
/* ---------------------------------------------------------------------- */
int Phreeqc::
species_list_compare(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
	int j;
	const char *name1, *name2;
	const class species_list *nptr1, *nptr2;

	nptr1 = (const class species_list *) ptr1;
	nptr2 = (const class species_list *) ptr2;

/*
 *   Put H+ first
 */
	if (nptr1->master_s != nptr2->master_s)
	{
		/*
		if (nptr1->master_s == s_hplus)
			return (-1);
		if (nptr2->master_s == s_hplus)
			return (1);
		*/
		if ((strcmp(nptr1->master_s->name.c_str(),"H+") == 0) || (strcmp(nptr1->master_s->name.c_str(),"H3O+") == 0))
			return (-1);
		if ((strcmp(nptr2->master_s->name.c_str(),"H+") == 0) || (strcmp(nptr2->master_s->name.c_str(),"H3O+") == 0))
			return (1);
	}
/*
 *   Other element valence states
 */
	if (nptr1->master_s->secondary != NULL)
	{
		name1 = nptr1->master_s->secondary->elt->name.c_str();
	}
	else
	{
		name1 = nptr1->master_s->primary->elt->name.c_str();
	}
	if (nptr2->master_s->secondary != NULL)
	{
		name2 = nptr2->master_s->secondary->elt->name.c_str();
	}
	else
	{
		name2 = nptr2->master_s->primary->elt->name.c_str();
	}
/*
 *   Compare name of primary or secondary master species; log molality
 */

	j = strcmp(name1, name2);

/*
 *   Different master species
 */
	if (j != 0)
		return (j);

/*
 *   Else, descending order by log molality
 */
	if (nptr1->s->lm > nptr2->s->lm)
	{
		return (-1);
	}
	else if (nptr1->s->lm < nptr2->s->lm)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
species_list_compare_alk(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
	const class species_list *nptr1, *nptr2;
	LDBLE alk1, alk2;

	nptr1 = (const class species_list *) ptr1;
	nptr2 = (const class species_list *) ptr2;
/*
 *   Else, descending order by log molality
 */
	alk1 = fabs(under(nptr1->s->lm) * nptr1->s->alk);
	alk2 = fabs(under(nptr2->s->lm) * nptr2->s->alk);

	if (alk1 > alk2)
	{
		return (-1);
	}
	else if (alk1 < alk2)
	{
		return (1);
	}
	else
	{
		return (0);
	}
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
species_list_compare_master(const void *ptr1, const void *ptr2)
/* ---------------------------------------------------------------------- */
{
	const char *name1, *name2;
	const class species_list *nptr1, *nptr2;

	nptr1 = (const class species_list *) ptr1;
	nptr2 = (const class species_list *) ptr2;

/*
 *   Put H+ first
 */
	if (nptr1->master_s != nptr2->master_s)
	{
		/*
		if (nptr1->master_s == s_hplus)
			return (-1);
		if (nptr2->master_s == s_hplus)
			return (1);
		*/
		if ((strcmp(nptr1->master_s->name.c_str(),"H+") == 0) || (strcmp(nptr1->master_s->name.c_str(),"H3O+") == 0))
			return (-1);
		if ((strcmp(nptr2->master_s->name.c_str(),"H+") == 0) || (strcmp(nptr2->master_s->name.c_str(),"H3O+") == 0))
			return (1);
	}
/*
 *   Other element valence states
 */
	if (nptr1->master_s->secondary != NULL)
	{
		name1 = nptr1->master_s->secondary->elt->name.c_str();
	}
	else
	{
		name1 = nptr1->master_s->primary->elt->name.c_str();
	}
	if (nptr2->master_s->secondary != NULL)
	{
		name2 = nptr2->master_s->secondary->elt->name.c_str();
	}
	else
	{
		name2 = nptr2->master_s->primary->elt->name.c_str();
	}
/*
 *   Compare name of primary or secondary master species; log molality
 */

	return (strcmp(name1, name2));
}


/* ---------------------------------------------------------------------- */
int Phreeqc::
species_list_sort(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Sort list using rules in species_list_compare
 */
	if (species_list.size() > 1)
	{
		qsort(&species_list[0], species_list.size(),
			  sizeof(class species_list), species_list_compare);
	}
	return (OK);
}

/* **********************************************************************
 *
 *   Routines related to structure "surface"
 *
 * ********************************************************************** */
/* ---------------------------------------------------------------------- */
struct Change_Surf * Phreeqc::
change_surf_alloc(int count)
/* ---------------------------------------------------------------------- */
{
	if (count == 1)
		return (change_surf);
	change_surf =
		(struct Change_Surf *) PHRQ_realloc(change_surf,
											(size_t) count *
											sizeof(struct Change_Surf));
	if (change_surf == NULL)
		malloc_error();
	change_surf[count - 1].cell_no = -99;
	change_surf[count - 1].next = FALSE;
	change_surf[count - 2].next = TRUE;

	return (change_surf);
}
/* ---------------------------------------------------------------------- */
class master * Phreeqc::
surface_get_psi_master(const std::string& name, int plane)
/* ---------------------------------------------------------------------- */
{
	class master *master_ptr;
	std::string tokens;

	if (name.size() == 0)
		return (NULL);
	tokens = name;
	tokens.append("_psi");
	switch (plane)
	{
	case SURF_PSI:
		break;
	case SURF_PSI1:
		tokens.append("b");
		break;
	case SURF_PSI2:
		tokens.append("d");
		break;
	default:
		error_msg("Unknown plane for surface_get_psi_master", STOP);
	}
	master_ptr = master_bsearch(tokens.c_str());
	return (master_ptr);
}
/* **********************************************************************
 *
 *   Routines related to structure "trxn"
 *
 * ********************************************************************** */

/* ---------------------------------------------------------------------- */


/* **********************************************************************
 *
 *   Routines related to structure "unknown"
 *
 * ********************************************************************** */
/* ---------------------------------------------------------------------- */
class unknown * Phreeqc::
unknown_alloc(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Allocates space to an "unknown" structure
 *      arguments: void
 *      return: pointer to an "unknown" structure
 */
	class unknown *unknown_ptr;
/*
 *   Allocate space
 */
	unknown_ptr = new class unknown;
/*
 *   set pointers in structure to NULL
 */
	unknown_ptr->type = 0;
	unknown_ptr->moles = 0.0;
	unknown_ptr->ln_moles = 0.0;
	unknown_ptr->f = 0.0;
	unknown_ptr->sum = 0.0;
	unknown_ptr->delta = 0.0;
	unknown_ptr->la = 0.0;
	unknown_ptr->number = 0;
	//unknown_ptr->description.clear();
	unknown_ptr->phase = NULL;
	unknown_ptr->si = 0.0;
	unknown_ptr->s = NULL;
	//unknown_ptr->exch_comp.clear();
	//unknown_ptr->pp_assemblage_comp_name.clear();
	unknown_ptr->pp_assemblage_comp_ptr = NULL;
	//unknown_ptr->ss_name.clear();
	unknown_ptr->ss_ptr = NULL;
	//unknown_ptr->ss_comp_name.clear();
	unknown_ptr->ss_comp_ptr = NULL;
	unknown_ptr->ss_comp_number = 0;
	unknown_ptr->ss_in = FALSE;
	//unknown_ptr->surface_comp.clear();
	unknown_ptr->related_moles = 0.0;
	unknown_ptr->potential_unknown = NULL;
	unknown_ptr->potential_unknown1 = NULL;
	unknown_ptr->potential_unknown2 = NULL;
	unknown_ptr->phase_unknown = NULL;
	//unknown_ptr->surface_charge.clear();
	unknown_ptr->mass_water = 0.0;
	unknown_ptr->dissolve_only = FALSE;
	unknown_ptr->inert_moles = 0.0;
	unknown_ptr->V_m = 0.0;
	unknown_ptr->pressure = 0.0;
	unknown_ptr->mb_number = 0;
	unknown_ptr->iteration = 0;

	return (unknown_ptr);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
unknown_delete(int i)
/* ---------------------------------------------------------------------- */
{
/*
 *   Delete unknow from list x
 */
	unknown_free(x[i]);
	x.erase(x.begin() + (size_t)i);
	count_unknowns--;
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
unknown_free(class unknown *unknown_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *   Frees space allocated to an unknown structure, frees unknown_ptr.
 */
	if (unknown_ptr == NULL)
		return (ERROR);
	unknown_ptr->master.clear();
	if (unknown_ptr->type == SURFACE_CB)
	{
		/*
		   surface_charge_free(unknown_ptr->surface_charge);
		   unknown_ptr->surface_charge = (struct surface_charge *) free_check_null(unknown_ptr->surface_charge);
		 */
	}
	unknown_ptr->comp_unknowns.clear();
	delete unknown_ptr;
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
system_duplicate(int i, int save_old)
/* ---------------------------------------------------------------------- */
{
	Utilities::Rxn_copy(Rxn_solution_map, i, save_old);

	Utilities::Rxn_copy(Rxn_pp_assemblage_map, i, save_old);

	Utilities::Rxn_copy(Rxn_exchange_map, i, save_old);

	Utilities::Rxn_copy(Rxn_surface_map, i, save_old);

	Utilities::Rxn_copy(Rxn_gas_phase_map, i, save_old);

	Utilities::Rxn_copy(Rxn_kinetics_map, i, save_old);

	Utilities::Rxn_copy(Rxn_ss_assemblage_map, i, save_old);

	return (OK);
}
/* ---------------------------------------------------------------------- */
void Phreeqc::
Logk_store(const std::string& name_in, class Logk& lk)
	/* ---------------------------------------------------------------------- */
{
	std::string name = name_in;
	str_tolower(name);
	Logk_map[name] = lk;
	return;
}
/* ---------------------------------------------------------------------- */
std::map<std::string, class Logk>::iterator Phreeqc::
Logk_search(const std::string& name_in)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Search map
	 */
	std::string name = name_in;
	str_tolower(name);
	return	Logk_map.find(name);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
entity_exists(const char *name, int n_user)
/* ---------------------------------------------------------------------- */
{
/*
 *   Reads solution,		   0 Solution
 *	 reaction,		   1 Reaction
 *	 exchange,		   2 Exchange
 *	 surface,		    3 Surface
 *	 gas_phase,		  4 Gas_phase
 *	 equilibrium_phases,	 5 Pure_phase
 *	 solid_solution,	     6 Ss_phase
 *	 kinetics,		   7 Kinetics
 *	 mix,			8 Mix
 *	 reaction_temperature	9 Temperature
 *	 unknown		     10 UnKnown
 */
	int return_value;
	char tokens[MAX_LENGTH];
	enum entity_type type;
/*
 *   Read keyword
 */
	strncpy(tokens, name, MAX_LENGTH-1);
	tokens[MAX_LENGTH-1] = '\0';
	type = get_entity_enum(tokens);
	return_value = TRUE;
	switch (type)
	{
	case UnKnown:
		warning_msg
			("EXISTS expecting keyword solution, mix, kinetics, reaction, reaction_temperature, equilibrium_phases, exchange, surface, gas_phase, or solid_solutions.");
		return_value = 2;
		break;
	case Solution:				/* Solution */
		if (Utilities::Rxn_find(Rxn_solution_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
		break;
	case Pure_phase:			/* Pure phases */
		if (Utilities::Rxn_find(Rxn_pp_assemblage_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
		break;
	case Reaction:				/* Reaction */
		if (Utilities::Rxn_find(Rxn_reaction_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
		break;
	case Mix:					/* Mix */
		if (Utilities::Rxn_find(Rxn_mix_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
		break;
	case Exchange:				/* Ex */
		if (Utilities::Rxn_find(Rxn_exchange_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
		break;
	case Surface:				/* Surface */
		if (Utilities::Rxn_find(Rxn_surface_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
		break;
	case Temperature:
		if (Utilities::Rxn_find(Rxn_temperature_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
	case Pressure:
		if (Utilities::Rxn_find(Rxn_pressure_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
	case Gas_phase:			/* Gas */
		if (Utilities::Rxn_find(Rxn_gas_phase_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
		break;
	case Kinetics:				/* Kinetics */
		if (Utilities::Rxn_find(Rxn_kinetics_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
		break;
	case Ss_phase:				/* solid_solutions */
		if (Utilities::Rxn_find(Rxn_ss_assemblage_map, n_user) == NULL)
		{
			return_value = FALSE;
		}
		break;
	}
	return (return_value);
}

/* ---------------------------------------------------------------------- */
enum entity_type Phreeqc::
get_entity_enum(char *name)
/* ---------------------------------------------------------------------- */
{
/*
 *   Reads solution,		   0 Solution
 *	 reaction,		   1 Reaction
 *	 exchange,		   2 Exchange
 *	 surface,		    3 Surface
 *	 gas_phase,		  4 Gas_phase
 *	 equilibrium_phases,	 5 Pure_phase
 *	 solid_solution,	     6 Ss_phase
 *	 kinetics,		   7 Kinetics
 *	 mix,			8 Mix
 *	 reaction_temperature	9 Temperature
 *   reaction_pressure
 *	 unknown		     10 UnKnown
 *
 */
	int i;
	const char* cptr;
	char tokens[MAX_LENGTH];
/*
 *   Read keyword
 */
	cptr = name;
	copy_token(tokens, &cptr, &i);
	check_key(tokens);

	switch (next_keyword)
	{
	case Keywords::KEY_SOLUTION:					/* Solution */
		return (Solution);
		break;
	case Keywords::KEY_EQUILIBRIUM_PHASES:		/* Pure phases */
		return (Pure_phase);
		break;
	case Keywords::KEY_REACTION:					/* Reaction */
		return (Reaction);
		break;
	case Keywords::KEY_MIX:						/* Mix */
		return (Mix);
		break;
	case Keywords::KEY_EXCHANGE:					/* Ex */
		return (Exchange);
		break;
	case Keywords::KEY_SURFACE:					/* Surface */
		return (Surface);
		break;
	case Keywords::KEY_REACTION_TEMPERATURE:		/* Temperature */
		return (Temperature);
		break;
	case Keywords::KEY_REACTION_PRESSURE:		/* Pressure */
		return (Pressure);
		break;
	case Keywords::KEY_GAS_PHASE:					/* Gas */
		return (Gas_phase);
		break;
	case Keywords::KEY_KINETICS:					/* Kinetics */
		return (Kinetics);
		break;
	case Keywords::KEY_SOLID_SOLUTIONS:			/* solid_solutions */
		return (Ss_phase);
		break;
	default:
		warning_msg
			("EXISTS expecting keyword solution, mix, kinetics, reaction, reaction_temperature, equilibrium_phases, exchange, surface, gas_phase, or solid_solutions.");
		break;
	}
	return (UnKnown);
}

/*
 * copier routines
 */
/* ---------------------------------------------------------------------- */
int Phreeqc::
copier_add(class copier *copier_ptr, int n_user, int start, int end)
/* ---------------------------------------------------------------------- */
/*
 *   add new set of copy instructions
 */
{
	copier_ptr->n_user.push_back(n_user);
	copier_ptr->start.push_back(start);
	copier_ptr->end.push_back(end);
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
copier_clear(class copier* copier_ptr)
/* ---------------------------------------------------------------------- */
/*
 *   clear copier
 */
{
	copier_ptr->n_user.clear();
	copier_ptr->start.clear();
	copier_ptr->end.clear();
	return (OK);
}

#include "StorageBin.h"

/* ---------------------------------------------------------------------- */
void Phreeqc::
Use2cxxStorageBin(cxxStorageBin & sb)
/* ---------------------------------------------------------------------- */
{
	//Add everything from use structure to storagebin sb

	sb.Get_system().Set_io(sb.Get_io());
	if (use.Get_mix_in())
	{
		cxxMix *entity = use.Get_mix_ptr();
		if (entity != NULL)
		{
			sb.Set_Mix(use.Get_n_mix_user(), entity);
		}

		// put mix solutions in sb
		cxxMix * mix_ptr = use.Get_mix_ptr();
		std::map<int, LDBLE>::const_iterator cit;
		for (cit = mix_ptr->Get_mixComps().begin(); cit != mix_ptr->Get_mixComps().end(); cit++)
		{
			cxxSolution *entity = Utilities::Rxn_find(Rxn_solution_map, cit->first);
			if (entity != NULL)
			{
				sb.Set_Solution(cit->first, entity);
			}
		}
	}
	else if (use.Get_solution_in())
	{
		cxxSolution *entity = Utilities::Rxn_find(Rxn_solution_map, use.Get_n_solution_user());
		if (entity != NULL)
		{
			sb.Set_Solution(use.Get_n_solution_user(), entity);
		}
	}
	if (use.Get_pp_assemblage_in())
	{
		cxxPPassemblage *entity_ptr = Utilities::Rxn_find(Rxn_pp_assemblage_map, use.Get_n_pp_assemblage_user());
		if (entity_ptr != NULL)
		{
			sb.Set_PPassemblage(use.Get_n_pp_assemblage_user(), entity_ptr);
		}
	}
	if (use.Get_exchange_in())
	{
		cxxExchange *entity_ptr = Utilities::Rxn_find(Rxn_exchange_map, use.Get_n_exchange_user());
		if (entity_ptr != NULL)
		{
			sb.Set_Exchange(use.Get_n_exchange_user(), entity_ptr);
		}
	}
	if (use.Get_surface_in())
	{
		cxxSurface *entity_ptr = Utilities::Rxn_find(Rxn_surface_map, use.Get_n_surface_user());
		if (entity_ptr != NULL)
		{
			sb.Set_Surface(use.Get_n_surface_user(), entity_ptr);
		}
	}
	if (use.Get_gas_phase_in())
	{
		cxxGasPhase *entity_ptr = Utilities::Rxn_find(Rxn_gas_phase_map, use.Get_n_gas_phase_user());
		if (entity_ptr != NULL)
		{
			sb.Set_GasPhase(use.Get_n_gas_phase_user(), entity_ptr);
		}
	}
	if (use.Get_ss_assemblage_in())
	{
		cxxSSassemblage *entity_ptr = Utilities::Rxn_find(Rxn_ss_assemblage_map, use.Get_n_ss_assemblage_user());
		if (entity_ptr != NULL)
		{
			sb.Set_SSassemblage(use.Get_n_ss_assemblage_user(), entity_ptr);
		}
	}
	if (use.Get_kinetics_in())
	{
		cxxKinetics *entity_ptr = Utilities::Rxn_find(Rxn_kinetics_map, use.Get_n_kinetics_user());
		if (entity_ptr != NULL)
		{
			sb.Set_Kinetics(use.Get_n_kinetics_user(), entity_ptr);
		}
	}
	if (use.Get_reaction_in())
	{
		cxxReaction *entity = Utilities::Rxn_find(Rxn_reaction_map, use.Get_n_reaction_user());
		if (entity != NULL)
		{
			sb.Set_Reaction(use.Get_n_reaction_user(), entity);
		}
	}
	if (use.Get_temperature_in())
	{
		cxxTemperature *entity = Utilities::Rxn_find(Rxn_temperature_map, use.Get_n_temperature_user());
		if (entity != NULL)
		{
			sb.Set_Temperature(use.Get_n_temperature_user(), entity);
		}
	}
	if (use.Get_pressure_in())
	{
		cxxPressure *entity = Utilities::Rxn_find(Rxn_pressure_map, use.Get_n_pressure_user());
		if (entity != NULL)
		{
			sb.Set_Pressure(use.Get_n_pressure_user(), entity);
		}
	}
}

void Phreeqc::
phreeqc2cxxStorageBin(cxxStorageBin & sb)
	//
	// Fills StorageBin sb with all reactants from phreeqc instance.
	// equivalent to old import_phreeqc.
	//
{
	// Solutions
	{
		std::map<int, cxxSolution>::iterator it;
		for (it = Rxn_solution_map.begin(); it != Rxn_solution_map.end(); it++)
		{
			sb.Set_Solution(it->second.Get_n_user(), &(it->second));	
		}
	}
	// Exchangers
	{
		std::map<int, cxxExchange>::iterator it;
		for (it = Rxn_exchange_map.begin(); it != Rxn_exchange_map.end(); it++)
		{
			sb.Set_Exchange(it->second.Get_n_user(), &(it->second));	
		}
	}
	// GasPhases
	{
		std::map<int, cxxGasPhase>::iterator it;
		for (it = Rxn_gas_phase_map.begin(); it != Rxn_gas_phase_map.end(); it++)
		{
			sb.Set_GasPhase(it->second.Get_n_user(), &(it->second));	
		}
	}

	// Kinetics
	{
		std::map<int, cxxKinetics>::iterator it;
		for (it = Rxn_kinetics_map.begin(); it != Rxn_kinetics_map.end(); it++)
		{
			sb.Set_Kinetics(it->second.Get_n_user(), &(it->second));	
		}
	}
	// PPassemblages
	{
		std::map<int, cxxPPassemblage>::iterator it;
		for (it = Rxn_pp_assemblage_map.begin(); it != Rxn_pp_assemblage_map.end(); it++)
		{
			sb.Set_PPassemblage(it->second.Get_n_user(), &(it->second));	
		}
	}
	// SSassemblages
	{
		std::map<int, cxxSSassemblage>::iterator it;
		for (it = Rxn_ss_assemblage_map.begin(); it != Rxn_ss_assemblage_map.end(); it++)
		{
			sb.Set_SSassemblage(it->second.Get_n_user(), &(it->second));	
		}
	}
	// Surfaces
	{
		std::map<int, cxxSurface>::iterator it;
		for (it = Rxn_surface_map.begin(); it != Rxn_surface_map.end(); it++)
		{
			sb.Set_Surface(it->second.Get_n_user(), &(it->second));	
		}
	}
	// Mixes
	{
		std::map<int, cxxMix>::iterator it;
		for (it = Rxn_mix_map.begin(); it != Rxn_mix_map.end(); it++)
		{
			sb.Set_Mix(it->second.Get_n_user(), &(it->second));	
		}
	}

	// Reactions
	{
		std::map<int, cxxReaction>::iterator it;
		for (it = Rxn_reaction_map.begin(); it != Rxn_reaction_map.end(); it++)
		{
			sb.Set_Reaction(it->second.Get_n_user(), &(it->second));	
		}
	}

	// Temperatures
	{
		std::map<int, cxxTemperature>::iterator it;
		for (it = Rxn_temperature_map.begin(); it != Rxn_temperature_map.end(); it++)
		{
			sb.Set_Temperature(it->second.Get_n_user(), &(it->second));	
		}
	}

	// Pressures
	{
		std::map<int, cxxPressure>::iterator it;
		for (it = Rxn_pressure_map.begin(); it != Rxn_pressure_map.end(); it++)
		{
			sb.Set_Pressure(it->second.Get_n_user(), &(it->second));	
		}
	}
}

void Phreeqc::
phreeqc2cxxStorageBin(cxxStorageBin & sb, int n)
		//
		// copy phreeqc reactants numbered n to StorageBin sb
		//
{
	// Solutions
	{
		cxxSolution *entity_ptr = Utilities::Rxn_find(Rxn_solution_map, n);
		if (entity_ptr != NULL)
		{
			sb.Set_Solution(n, entity_ptr);
		}
	}
	// Exchangers
	{
		cxxExchange *entity_ptr = Utilities::Rxn_find(Rxn_exchange_map, n);
		if (entity_ptr != NULL)
		{
			sb.Set_Exchange(n, entity_ptr);
		}
	}

	// GasPhases
	{
		cxxGasPhase *entity_ptr = Utilities::Rxn_find(Rxn_gas_phase_map, n);
		if (entity_ptr != NULL)
		{
			sb.Set_GasPhase(n, entity_ptr);
		}
	}

	// Kinetics
	{
		cxxKinetics *entity_ptr = Utilities::Rxn_find(Rxn_kinetics_map, n);
		if (entity_ptr != NULL)
		{
			sb.Set_Kinetics(n, entity_ptr);
		}
	}
	// PPassemblages
	{
		cxxPPassemblage *entity_ptr = Utilities::Rxn_find(Rxn_pp_assemblage_map, n);
		if (entity_ptr != NULL)
		{
			sb.Set_PPassemblage(n, entity_ptr);
		}
	}
	// SSassemblages
	{
		cxxSSassemblage *entity_ptr = Utilities::Rxn_find(Rxn_ss_assemblage_map, n);
		if (entity_ptr != NULL)
		{
			sb.Set_SSassemblage(n, entity_ptr);
		}
	}
	// Surfaces
	{
		cxxSurface *entity_ptr = Utilities::Rxn_find(Rxn_surface_map, n);
		if (entity_ptr != NULL)
		{
			sb.Set_Surface(n, entity_ptr);
		}
	}
}
void Phreeqc::
cxxStorageBin2phreeqc(cxxStorageBin & sb, int n)
//
// copy all reactants from storage bin number n to phreeqc
// replaces any existing reactants in phreeqc
//
{
	// Solutions
	{
		std::map < int, cxxSolution >::const_iterator it = sb.Get_Solutions().find(n);
		if (it != sb.Get_Solutions().end())
		{
			Rxn_solution_map[n] = it->second;
		}
	}
	// Exchangers
	{
		std::map < int, cxxExchange >::const_iterator it = sb.Get_Exchangers().find(n);
		if (it != sb.Get_Exchangers().end())
		{
			Rxn_exchange_map[n] = it->second;
		}
	}

	// GasPhases
	{
		std::map < int, cxxGasPhase >::const_iterator it = sb.Get_GasPhases().find(n);
		if (it != sb.Get_GasPhases().end())
		{
			Rxn_gas_phase_map[n] = it->second;
		}
	}

	// Kinetics
	{
		std::map < int, cxxKinetics >::const_iterator it = sb.Get_Kinetics().find(n);
		if (it != sb.Get_Kinetics().end())
		{
			Rxn_kinetics_map[n] = it->second;
		}
	}
	// PPassemblages
	{
		std::map < int, cxxPPassemblage >::const_iterator it = sb.Get_PPassemblages().find(n);
		if (it != sb.Get_PPassemblages().end())
		{
			Rxn_pp_assemblage_map[n] = it->second;
		}
	}
	// SSassemblages
	{
		std::map < int, cxxSSassemblage >::const_iterator it = sb.Get_SSassemblages().find(n);
		if (it != sb.Get_SSassemblages().end())
		{
			Rxn_ss_assemblage_map[n] = it->second;
		}
	}
	// Surfaces
	{
		std::map < int, cxxSurface >::const_iterator it = sb.Get_Surfaces().find(n);
		if (it != sb.Get_Surfaces().end())
		{
			Rxn_surface_map[n] = it->second;
		}
	}
	// Mixes
	{
		std::map < int, cxxMix >::const_iterator it = sb.Get_Mixes().find(n);
		if (it != sb.Get_Mixes().end())
		{
			Rxn_mix_map[n] = it->second;
		}
	}

	// Reactions
	{
		std::map < int, cxxReaction >::const_iterator it = sb.Get_Reactions().find(n);
		if (it != sb.Get_Reactions().end())
		{
			Rxn_reaction_map[n] = it->second;
		}
	}
	// Temperatures
	{
		std::map < int, cxxTemperature >::const_iterator it = sb.Get_Temperatures().find(n);
		if (it != sb.Get_Temperatures().end())
		{
			Rxn_temperature_map[n] = it->second;
		}
	}
	// Pressures
	{
		std::map < int, cxxPressure >::const_iterator it = sb.Get_Pressures().find(n);
		if (it != sb.Get_Pressures().end())
		{
			Rxn_pressure_map[n] = it->second;
		}
	}
}
void Phreeqc::
cxxStorageBin2phreeqc(cxxStorageBin & sb)
//
// copy data from storage bin to phreeqc
// replaces any existing reactants in phreeqc
//
{
	// Solutions
	{
		std::map < int, cxxSolution >::const_iterator it = sb.Get_Solutions().begin();
		for ( ; it != sb.Get_Solutions().end(); it++)
		{
			Rxn_solution_map[it->first] = it->second;
		}
	}
	// Exchangers
	{
		std::map < int, cxxExchange >::const_iterator it = sb.Get_Exchangers().begin();
		for ( ; it != sb.Get_Exchangers().end(); it++)
		{
			Rxn_exchange_map[it->first] = it->second;
		}
	}

	// GasPhases
	{
		std::map < int, cxxGasPhase >::const_iterator it = sb.Get_GasPhases().begin();
		for ( ; it != sb.Get_GasPhases().end(); it++)
		{
			Rxn_gas_phase_map[it->first] = it->second;
		}
	}

	// Kinetics
	{
		std::map < int, cxxKinetics >::const_iterator it = sb.Get_Kinetics().begin();
		for ( ; it != sb.Get_Kinetics().end(); it++)
		{
			Rxn_kinetics_map[it->first] = it->second;
		}
	}
	// PPassemblages
	{
		std::map < int, cxxPPassemblage >::const_iterator it = sb.Get_PPassemblages().begin();
		for ( ; it != sb.Get_PPassemblages().end(); it++)
		{
			Rxn_pp_assemblage_map[it->first] = it->second;
		}
	}
	// SSassemblages
	{
		std::map < int, cxxSSassemblage >::const_iterator it = sb.Get_SSassemblages().begin();
		for ( ; it != sb.Get_SSassemblages().end(); it++)
		{
			Rxn_ss_assemblage_map[it->first] = it->second;
		}
	}
	// Surfaces
	{
		std::map < int, cxxSurface >::const_iterator it = sb.Get_Surfaces().begin();
		for ( ; it != sb.Get_Surfaces().end(); it++)
		{
			Rxn_surface_map[it->first] = it->second;
		}
	}
	// Mixes
	{
		std::map < int, cxxMix >::const_iterator it = sb.Get_Mixes().begin();
		for ( ; it != sb.Get_Mixes().end(); it++)
		{
			Rxn_mix_map[it->first] = it->second;
		}
	}

	// Reactions
	{
		std::map < int, cxxReaction >::const_iterator it = sb.Get_Reactions().begin();
		for ( ; it != sb.Get_Reactions().end(); it++)
		{
			Rxn_reaction_map[it->first] = it->second;
		}
	}
	// Temperatures
	{
		std::map < int, cxxTemperature >::const_iterator it = sb.Get_Temperatures().begin();
		for ( ; it != sb.Get_Temperatures().end(); it++)
		{
			Rxn_temperature_map[it->first] = it->second;
		}
	}
	// Pressures
	{
		std::map < int, cxxPressure >::const_iterator it = sb.Get_Pressures().begin();
		for ( ; it != sb.Get_Pressures().end(); it++)
		{
			Rxn_pressure_map[it->first] = it->second;
		}
	}
}


