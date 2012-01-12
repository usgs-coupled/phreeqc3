#include "Utils.h"
#include "Phreeqc.h"
#include "phqalloc.h"
#include "Temperature.h"
#include "Exchange.h"
#include "GasPhase.h"
#include "Reaction.h"
#include "PPassemblage.h"

/*   
     Calling sequence 

Initialization:
---------------

build_tally_table();                   finds all elements (rows),
                                       possible return values (columns),
				       allocates space
get_tally_table_rows_columns(int *rows, int *columns)
				       returns number of rows and columns in table
get_tally_table_row_heading(int row, char *string)
				       row is C row number
				       returns row descripter for row
get_tally_table_column_heading(int column, int *type, char *string)
				       column is C column number
				       returns column heading for column

Each call to phreeqc:
---------------------

zero_tally_table();                    initialize table to 0s
set_reaction_moles(n_user, moles)      n_user is reservoir number
                                       moles is number of moles of reaction to add

int set_reaction_temperature(n_user, tc)

fill_tally_table(int *n_user, int index_conservative, int n_buffer)
                                       n_user is reservoir number
				       index_conservative is solution number
				           where conservative mixing is stored
                                       slot is 0 for initial

   run phreeqc here.

fill_tally_table(int *n_user, int index_conservative, int n_buffer)
                                       n_user is reservoir number
				       index_conservative is solution number
				           where conservative mixing is stored
                                       slot is 1 for final
store_tally_table(LDBLE *array, int row_dim, int col_dim, LDBLE fill_factor) 
                                       row_dim is Fortran dimension
                                       col_dim is Fortran dimension
				       array is space from Fortran
				       stores conservative mixing (column 0)
				       stores reaction (column 1)
				       difference between slot 1 and slot 0 for
				       all other intities (columns 2-n)

Finalization:
-------------
int free_tally_table(void);       Frees space

*/

/* ---------------------------------------------------------------------- */
int Phreeqc::
get_all_components(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Counts components in any defined solution, gas_phase, exchanger,
 *   surface, or pure_phase_assemblage
 *
 *   Returns n_comp, which is total, including H, O, elements, and Charge
 *           names contains character strings with names of components
 */
	int i, j;
/*
 *   Accumulate all aqueous components
 */
	add_all_components_tally();
/*
 *   Count components, 2 for hydrogen, oxygen,  + others,
 */
	tally_count_component = 0;
	for (i = 0; i < count_master; i++)
	{
		if (master[i]->total > 0.0 && master[i]->s->type == AQ)
		{
			tally_count_component++;
		}
	}
/*
 *   Put information in buffer.
 *   Buffer contains an entry for every primary master
 *   species that can be used in the transport problem.
 *   Each entry in buffer is sent to HST for transort.
 */
	t_buffer =
		(struct tally_buffer *) PHRQ_malloc((size_t) tally_count_component *
											sizeof(struct tally_buffer));
	j = 0;
	for (i = 0; i < count_master; i++)
	{
		if (master[i]->total > 0.0 && master[i]->s->type == AQ)
		{
			t_buffer[j].name = master[i]->elt->name;
			t_buffer[j].master = master[i];
			t_buffer[j].gfw = master[i]->elt->gfw;
			j++;
		}
	}
#ifdef SKIP
	output_msg( "List of Components:\n" );
	for (i = 0; i < tally_count_component; i++)
	{
		output_msg(sformatf( "\t%d\t%s\n", i + 1, buffer[i].name));
		/*
		   for (j=0; buffer[i].name[j] != '\0'; j++) {
		   names[i * length + j] = buffer[i].name[j];
		   }
		 */
	}
#endif
	/*
	 *  Return value
	 */
	/**n_comp = count_component;*/
	count_tally_table_rows = tally_count_component;
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
store_tally_table(LDBLE * l_array, int row_dim, int col_dim, LDBLE fill_factor)
/* ---------------------------------------------------------------------- */
{
	int i, j;
	if (tally_table == NULL)
	{
		input_error++;
		error_msg("Tally table not defined, get_tally_table_rows_columns",
				  CONTINUE);
		return (ERROR);
	}
	if (count_tally_table_rows > row_dim)
	{
		input_error++;
		error_msg
			("Too many tally table rows for Fortran storage, store_tally_table",
			 CONTINUE);
		return (ERROR);
	}
	if (count_tally_table_columns > col_dim)
	{
		input_error++;
		error_msg
			("Too many tally table columns for Fortran storage, store_tally_table",
			 CONTINUE);
		return (ERROR);
	}
	/*
	 * store conservative mixing solution
	 */
	for (j = 0; j < count_tally_table_rows; j++)
	{
		l_array[j] = tally_table[0].total[1][j].moles;
	}
	/*
	 * store reaction solution
	 */
	for (j = 0; j < count_tally_table_rows; j++)
	{
		l_array[row_dim + j] = tally_table[1].total[1][j].moles;
	}
	/*
	 *   Calculate deltas
	 */

	diff_tally_table();

	/*
	 * store deltas for all other columns
	 */
	for (i = 2; i < count_tally_table_columns; i++)
	{
		for (j = 0; j < count_tally_table_rows; j++)
		{
			l_array[i * row_dim + j] =
				tally_table[i].total[2][j].moles / fill_factor;
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
get_tally_table_rows_columns(int *rows, int *columns)
/* ---------------------------------------------------------------------- */
{
	*rows = 0;
	*columns = 0;
	if (tally_table == NULL)
	{
		input_error++;
		error_msg("tally table not defined, get_tally_table_rows_columns",
				  CONTINUE);
		return (ERROR);
	}
	*rows = count_tally_table_rows;
	*columns = count_tally_table_columns;
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
get_tally_table_row_heading(int row, char *string)
/* ---------------------------------------------------------------------- */
{
	/*
	 *  row is C row number
	 */
	strcpy(string, "");
	if (tally_table == NULL)
	{
		input_error++;
		error_msg("Tally table not defined, get_tally_table row_heading",
				  CONTINUE);
		return (ERROR);
	}
	if (row >= count_tally_table_rows)
	{
		input_error++;
		error_msg
			("Row exceeds tally table size, get_tally_table row_heading",
			 CONTINUE);
		return (ERROR);
	}
	strcpy(string, t_buffer[row].name);
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
get_tally_table_column_heading(int column, int *type, char *string)
/* ---------------------------------------------------------------------- */
{
	/*
	 *  column is C column number
	 */
	*type = -1;
	strcpy(string, "");
	if (tally_table == NULL)
	{
		input_error++;
		error_msg("tally table not defined, get_tally_table_column_heading",
				  CONTINUE);
		return (ERROR);
	}
	if (column >= count_tally_table_columns)
	{
		input_error++;
		error_msg
			("column exceeds tally table size, get_tally_table_column_heading",
			 CONTINUE);
		return (ERROR);
	}
	strcpy(string, tally_table[column].name);
	*type = tally_table[column].type;
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
free_tally_table(void)
/* ---------------------------------------------------------------------- */
{
	int i, k;
	if (tally_table == NULL)
		return (OK);
	for (i = 0; i < count_tally_table_columns; i++)
	{
		if (tally_table[i].formula != NULL)
			tally_table[i].formula =
				(struct elt_list *) free_check_null(tally_table[i].formula);
		for (k = 0; k < 3; k++)
		{
			tally_table[i].total[k] =
				(struct tally_buffer *) free_check_null(tally_table[i].
														total[k]);
		}
	}
	tally_table = (struct tally *) free_check_null(tally_table);
	t_buffer = (struct tally_buffer *) free_check_null(t_buffer);
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
zero_tally_table(void)
/* ---------------------------------------------------------------------- */
{
	int i, j, k;
	for (i = 0; i < count_tally_table_columns; i++)
	{
		for (j = 0; j < count_tally_table_rows; j++)
		{
			for (k = 0; k < 3; k++)
			{
				tally_table[i].total[k][j].moles = 0;
			}
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
diff_tally_table(void)
/* ---------------------------------------------------------------------- */
{
	int i, j;
	/*
	   output_msg("Difference\n\n");
	 */
	for (i = 0; i < count_tally_table_columns; i++)
	{
		for (j = 0; j < count_tally_table_rows; j++)
		{
			tally_table[i].total[2][j].moles =
				tally_table[i].total[1][j].moles -
				tally_table[i].total[0][j].moles;
		}

		/*
		   output_msg(sformatf( "Column %d\t%s\tType: %d\n", i, tally_table[i].name, tally_table[i].type));
		   for (j = 0; j < count_tally_table_rows; j++) {
		   output_msg(sformatf( "\t%d\t%s\t%e\n", j, tally_table[i].total[2][j].name, (double) tally_table[i].total[2][j].moles));
		   }
		 */
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
print_tally_table(void)
/* ---------------------------------------------------------------------- */
{
	int i, j;
	output_msg(sformatf( "Tally_table\n\n"));
	for (i = 0; i < count_tally_table_columns; i++)
	{
		output_msg(sformatf( "%s\tType: %d\n", tally_table[i].name,
				   tally_table[i].type));
		output_msg(sformatf( "\n"));
		output_msg(sformatf( "\t%15s\t%15s\t%15s\n", "Initial",
				   "Final", "Difference"));
		for (j = 0; j < count_tally_table_rows; j++)
		{
			output_msg(sformatf( "%5s\t%15g\t%15g\t%15g\n",
					   t_buffer[j].name, tally_table[i].total[0][j].moles,
					   tally_table[i].total[1][j].moles,
					   tally_table[i].total[2][j].moles));
		}
		output_msg(sformatf( "\n"));
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
fill_tally_table(int *n_user, int index_conservative, int n_buffer)
/* ---------------------------------------------------------------------- */
{
/*
 *   Routine accumulates elements from all solutions, phases, gas phases,
 *   exchangers, and surfaces. 
 */
	int i, j, k, n;
	struct solution *solution_ptr;
	//struct irrev *irrev_ptr;
	//struct pp_assemblage *pp_assemblage_ptr;
	//struct exchange *exchange_ptr;
	struct surface *surface_ptr;
	struct ss_assemblage *ss_assemblage_ptr;
	//struct gas_phase *gas_phase_ptr;
	struct kinetics *kinetics_ptr;
	struct kinetics_comp *kinetics_comp_ptr;
	int found;
	LDBLE moles;
	char *ptr;
	/*
	 *  Cycle through tally table columns
	 */
	for (i = 0; i < count_tally_table_columns; i++)
	{
		switch (tally_table[i].type)
		{
		case Solution:
/*
 *   fill solution
 */
			if (n_user[Solution] < 0 || n_buffer == 0)
				break;
			if (i == 0)
			{
				solution_ptr = solution_bsearch(index_conservative, &n, TRUE);
			}
			else if (i == 1)
			{
				solution_ptr = solution_bsearch(n_user[Solution], &n, TRUE);
			}
			else
			{
				solution_ptr = NULL;
				error_msg
					("Solution is not in first two columns of tally_table",
					 STOP);
			}
			if (solution_ptr == NULL)
				break;
			xsolution_zero();
			add_solution(solution_ptr, 1.0, 1.0);
			count_elts = 0;
			paren_count = 0;
			for (j = 0; j < count_master; j++)
			{
				if (master[j]->total > 0.0)
				{
					char * temp_name = string_duplicate(master[j]->elt->primary->elt->name);
					ptr = temp_name;
					get_elts_in_species(&ptr, master[j]->total);
					free_check_null(temp_name);
				}
			}
			qsort(elt_list, (size_t) count_elts,
				  (size_t) sizeof(struct elt_list), elt_list_compare);
			elt_list_combine();
			elt_list_to_tally_table(tally_table[i].total[n_buffer]);
			break;
		case Reaction:
			/*
			 *   fill reaction
			 */
			if (n_user[Reaction] < 0)
				break;
			{
				cxxReaction *reaction_ptr = Utilities::Rxn_find(Rxn_reaction_map, n_user[Reaction]);
				if (reaction_ptr == NULL)
					break;
				count_elts = 0;
				paren_count = 0;
				if (n_buffer == 1)
				{
					moles = reaction_ptr->Get_steps()[0];
				}
				else
				{
					moles = 0.0;
				}
				reaction_calc(reaction_ptr);
				add_elt_list(reaction_ptr->Get_elementList(), moles);
				elt_list_to_tally_table(tally_table[i].total[n_buffer]);
			}
#ifdef SKIP
			irrev_ptr = irrev_bsearch(n_user[Reaction], &n);
			if (irrev_ptr == NULL)
				break;
			count_elts = 0;
			paren_count = 0;
			if (n_buffer == 1)
			{
				moles = irrev_ptr->steps[0];
			}
			else
			{
				moles = 0.0;
			}
			add_elt_list(irrev_ptr->elts, moles);
			elt_list_to_tally_table(tally_table[i].total[n_buffer]);
#endif
			break;
		case Pure_phase:
			/*
			 *   fill an equilibrium phase
			 */
			if (n_user[Pure_phase] < 0)
				break;
			{
				cxxPPassemblage * pp_assemblage_ptr = Utilities::Rxn_find(Rxn_pp_assemblage_map, n_user[Pure_phase]);
				//pp_assemblage_ptr = pp_assemblage_bsearch(n_user[Pure_phase], &n);
				if (pp_assemblage_ptr == NULL)
					break;
				//for (j = 0; j < pp_assemblage_ptr->count_comps; j++)
				std::map<std::string, cxxPPassemblageComp>::iterator it;
				it =  pp_assemblage_ptr->Get_pp_assemblage_comps().begin();
				for ( ; it != pp_assemblage_ptr->Get_pp_assemblage_comps().end(); it++)
				{
					if (string_hsave(it->second.Get_name().c_str()) ==
						tally_table[i].name)
						break;
					if (strcmp_nocase(it->second.Get_name().c_str(),
						tally_table[i].name) == 0)
						break;
				}
				if (it == pp_assemblage_ptr->Get_pp_assemblage_comps().end())
					break;
				count_elts = 0;
				paren_count = 0;
				moles = it->second.Get_moles();
				add_elt_list(tally_table[i].formula, moles);
				elt_list_to_tally_table(tally_table[i].total[n_buffer]);
			}
			break;
		case Exchange:
			{
				/*
				*   fill exchange
				*/
				if (n_user[Exchange] < 0)
					break;
				//exchange_ptr = exchange_bsearch(n_user[Exchange], &n);
				cxxExchange * exchange_ptr = Utilities::Rxn_find(Rxn_exchange_map, n_user[Exchange]);
				if (exchange_ptr == NULL)
					break;
				count_elts = 0;
				paren_count = 0;
				std::vector<cxxExchComp *> comps = exchange_ptr->Vectorize();
				//for (j = 0; j < exchange_ptr->count_comps; j++)
				for (j = 0; j < (int) comps.size(); j++)
				{
					add_elt_list(comps[j]->Get_totals(), 1.0);
				}
#ifdef SKIP
				for (j = 0; j < exchange_ptr->count_comps; j++)
				{
					add_elt_list(exchange_ptr->comps[j].totals, 1.0);
				}
#endif
				qsort(elt_list, (size_t) count_elts,
					(size_t) sizeof(struct elt_list), elt_list_compare);
				elt_list_combine();
				elt_list_to_tally_table(tally_table[i].total[n_buffer]);
			}
			break;
		case Surface:
			/*
			 *   fill surface
			 */
			if (n_user[Surface] < 0)
				break;
			surface_ptr = surface_bsearch(n_user[Surface], &n);
			if (surface_ptr == NULL)
				break;
			count_elts = 0;
			paren_count = 0;
			for (j = 0; j < surface_ptr->count_comps; j++)
			{
				add_elt_list(surface_ptr->comps[j].totals, 1.0);
			}
			qsort(elt_list, (size_t) count_elts,
				  (size_t) sizeof(struct elt_list), elt_list_compare);
			elt_list_combine();
			elt_list_to_tally_table(tally_table[i].total[n_buffer]);
			break;
		case Ss_phase:
			/*
			 *   fill an solid solution phase
			 */
			if (n_user[Ss_phase] < 0)
				break;
			ss_assemblage_ptr = ss_assemblage_bsearch(n_user[Ss_phase], &n);
			if (ss_assemblage_ptr == NULL)
				break;
			found = FALSE;
			moles = 0.0;
			for (j = 0; j < ss_assemblage_ptr->count_s_s; j++)
			{
				for (k = 0; k < ss_assemblage_ptr->s_s[j].count_comps; k++)
				{
					if (ss_assemblage_ptr->s_s[j].comps[k].phase->name ==
						tally_table[i].name)
						break;
					if (strcmp_nocase
						(ss_assemblage_ptr->s_s[j].comps[k].phase->name,
						 tally_table[i].name) == 0)
						break;
				}
				if (k < ss_assemblage_ptr->s_s[j].count_comps)
				{
					moles = ss_assemblage_ptr->s_s[j].comps[k].moles;
					found = TRUE;
					break;
				}
			}
			if (found == FALSE)
				break;
			count_elts = 0;
			paren_count = 0;
			add_elt_list(tally_table[i].formula, moles);
			elt_list_to_tally_table(tally_table[i].total[n_buffer]);
			break;
		case Gas_phase:
			/*
			 *   fill in gas phase
			 */
			if (n_user[Gas_phase] < 0)
				break;
			{
				//gas_phase_ptr = gas_phase_bsearch(n_user[Gas_phase], &n);
				cxxGasPhase * gas_phase_ptr = Utilities::Rxn_find(Rxn_gas_phase_map, n_user[Gas_phase]);
				if (gas_phase_ptr == NULL)
					break;
				count_elts = 0;
				paren_count = 0;
				const std::vector<cxxGasComp> *gc = &(gas_phase_ptr->Get_gas_comps());
				for (size_t l = 0; l < gc->size(); l++)
				{
					struct phase *phase_ptr = phase_bsearch((*gc)[l].Get_phase_name().c_str(), &k, FALSE);

					add_elt_list(phase_ptr->next_elt,
						(*gc)[l].Get_moles());
				}
				qsort(elt_list, (size_t) count_elts,
					(size_t) sizeof(struct elt_list), elt_list_compare);
				elt_list_combine();
				elt_list_to_tally_table(tally_table[i].total[n_buffer]);
				break;
			}
#ifdef SKIP
			/*
			 *   fill in gas phase
			 */
			if (n_user[Gas_phase] < 0)
				break;
			//gas_phase_ptr = gas_phase_bsearch(n_user[Gas_phase], &n);

			if (gas_phase_ptr == NULL)
				break;
			count_elts = 0;
			paren_count = 0;
			for (j = 0; j < gas_phase_ptr->count_comps; j++)
			{
				add_elt_list(gas_phase_ptr->comps[j].phase->next_elt,
							 gas_phase_ptr->comps[j].moles);
			}
			qsort(elt_list, (size_t) count_elts,
				  (size_t) sizeof(struct elt_list), elt_list_compare);
			elt_list_combine();
			elt_list_to_tally_table(tally_table[i].total[n_buffer]);
			break;
#endif
		case Kinetics:
			/*
			 *   fill in kinetics
			 */
			if (n_user[Kinetics] < 0)
				break;
			kinetics_ptr = kinetics_bsearch(n_user[Kinetics], &n);
			if (kinetics_ptr == NULL)
				break;
			kinetics_comp_ptr = NULL;
			for (j = 0; j < kinetics_ptr->count_comps; j++)
			{
				kinetics_comp_ptr = &kinetics_ptr->comps[j];
				if (kinetics_comp_ptr->rate_name == tally_table[i].name)
					break;
				if (strcmp_nocase
					(kinetics_comp_ptr->rate_name, tally_table[i].name) == 0)
					break;
			}
			if (j >= kinetics_ptr->count_comps)
				break;
			moles = kinetics_comp_ptr->m;
			count_elts = 0;
			paren_count = 0;
			add_elt_list(tally_table[i].formula, moles);
			elt_list_to_tally_table(tally_table[i].total[n_buffer]);
			break;
		case Mix:
			break;
		case Temperature:
		case Pressure:
			break;
		case UnKnown:
			break;
		}
#ifdef SKIP
		output_msg(sformatf( "Column %d\t%s\tType: %d\n", i,
				   tally_table[i].name, tally_table[i].type));
		for (j = 0; j < count_tally_table_rows; j++)
		{
			output_msg(sformatf( "\t%d\t%s\t%e\n", j,
					   tally_table[i].total[n_buffer][j].name,
					   (double) tally_table[i].total[n_buffer][j].moles));
		}
#endif
	}

	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
elt_list_to_tally_table(struct tally_buffer *buffer_ptr)
/* ---------------------------------------------------------------------- */
{
	int i, j;
	for (i = 0; i < count_tally_table_rows; i++)
	{
		buffer_ptr[i].moles = 0.0;
	}
	/*
	 * copy element list amounts to buffer in tally table
	 * for column number
	 */

	for (j = 0; j < count_elts; j++)
	{
		if (elt_list[j].elt->primary->s == s_h2o)
			continue;
		if (elt_list[j].elt->primary->s == s_hplus)
			continue;
		if (elt_list[j].elt->primary->s == s_h3oplus)
			continue;
		if (elt_list[j].elt->primary->type != AQ)
			continue;
		for (i = 0; i < count_tally_table_rows; i++)
		{
			if (elt_list[j].elt->primary ==
				buffer_ptr[i].master->elt->primary)
			{
				buffer_ptr[i].moles = elt_list[j].coef;
				break;
			}
		}
		if (i >= count_tally_table_rows)
		{
			error_msg("Should not be here in elt_list_to_tally_table", STOP);
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
build_tally_table(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Routine accumulates elements from all solutions, phases, gas phases,
 *   exchangers, and surfaces. Counts number of aqueous components
 *   to transport. Stores in global variable tally_count_component.
 *   Also calculates a number greater than all user numbers and
 *   stores in global variable first_user_number.
 */
	int i, j, k, l, n, p, save_print_use;
	int count_tt_reaction, count_tt_exchange, count_tt_surface,
		count_tt_gas_phase;
	int count_tt_pure_phase, count_tt_ss_phase, count_tt_kinetics;
	//struct pp_assemblage *pp_assemblage_ptr;
	//struct pure_phase *pure_phase_ptr;
	struct ss_assemblage *ss_assemblage_ptr;
	struct s_s *s_s_ptr;
	struct s_s_comp *s_s_comp_ptr;
	struct kinetics *kinetics_ptr;
	struct kinetics_comp *kinetics_comp_ptr;
	struct phase *phase_ptr;


	char token[MAX_LENGTH];
	char *ptr;
/*
 *  make list of all elements in all entitites
 *  defines the number of rows in the table
 */
	get_all_components();

	save_print_use = pr.use;
	pr.use = FALSE;
/*
 *  find nuber of columns
 */
	count_tally_table_columns = 0;
/*
 *   add one for conservative mixing
 */
	n = count_tally_table_columns;
	extend_tally_table();
	tally_table[n].name = string_hsave("Solution_conservative");
	tally_table[n].type = Solution;
/*
 *   add one for mixing plus reaction
 */
	n = count_tally_table_columns;
	extend_tally_table();
	tally_table[n].name = string_hsave("Solution_reaction");
	tally_table[n].type = Solution;
/*
 *   add one for reactions
 */
	//if (count_irrev > 0)
	if (Rxn_reaction_map.size() > 0)
	{
		count_tt_reaction = 1;
		n = count_tally_table_columns;
		extend_tally_table();
		tally_table[n].name = string_hsave("Reaction");
		tally_table[n].type = Reaction;
	}
	else
	{
		count_tt_reaction = 0;
	}
/*
 *   add one for exchangers
 */
	if (Rxn_exchange_map.size() > 0)
	{
		count_tt_exchange = 1;
		n = count_tally_table_columns;
		extend_tally_table();
		tally_table[n].name = string_hsave("Exchange");
		tally_table[n].type = Exchange;
	}
	else
	{
		count_tt_exchange = 0;
	}
#ifdef SKIP
	if (count_exchange > 0)
	{
		count_tt_exchange = 1;
		n = count_tally_table_columns;
		extend_tally_table();
		tally_table[n].name = string_hsave("Exchange");
		tally_table[n].type = Exchange;
	}
	else
	{
		count_tt_exchange = 0;
	}
#endif
/*
 *   add one for surface
 */
	if (count_surface > 0)
	{
		count_tt_surface = 1;
		n = count_tally_table_columns;
		extend_tally_table();
		tally_table[n].name = string_hsave("Surface");
		tally_table[n].type = Surface;
	}
	else
	{
		count_tt_surface = 0;
	}
/*
 *   add one for gases
 */
	//if (count_gas_phase > 0)
	if (Rxn_gas_phase_map.size() > 0)
	{
		count_tt_gas_phase = 1;
		n = count_tally_table_columns;
		extend_tally_table();
		tally_table[n].name = string_hsave("Gas_phase");
		tally_table[n].type = Gas_phase;
	}
	else
	{
		count_tt_gas_phase = 0;
	}
/*
 *   Count pure phases
 */
	count_tt_pure_phase = 0;
	//if (count_pp_assemblage > 0)
	if (Rxn_pp_assemblage_map.size() > 0)
	{
		/* 
		 * Go through all pure phases in pure phase assemblages
		 */
		//for (i = 0; i < count_pp_assemblage; i++)
		std::map<int, cxxPPassemblage>::iterator it;
		for (it = Rxn_pp_assemblage_map.begin(); it != Rxn_pp_assemblage_map.end(); it++)
		{
			//pp_assemblage_ptr = &pp_assemblage[i];
			cxxPPassemblage * pp_assemblage_ptr = &(it->second);
			std::map<std::string, cxxPPassemblageComp>::iterator jit;
			jit =  pp_assemblage_ptr->Get_pp_assemblage_comps().begin();
			for ( ; jit != pp_assemblage_ptr->Get_pp_assemblage_comps().end(); jit++)
			{
				cxxPPassemblageComp * comp_ptr = &(jit->second);
				int l;
				struct phase * phase_ptr = phase_bsearch(jit->first.c_str(), &l, FALSE);
				/* 
				 * check if already in tally_table
				 */
				for (k = 1; k < count_tally_table_columns; k++)
				{
					if (tally_table[k].type == Pure_phase &&
						tally_table[k].name == phase_ptr->name &&
						tally_table[k].add_formula ==
						string_hsave(comp_ptr->Get_add_formula().c_str()))
						break;
				}
				if (k < count_tally_table_columns)
					continue;
				/*
				 * Add to table
				 */
				count_tt_pure_phase++;
				n = count_tally_table_columns;
				extend_tally_table();
				tally_table[n].name = phase_ptr->name;
				tally_table[n].type = Pure_phase;
				tally_table[n].add_formula = string_hsave(comp_ptr->Get_add_formula().c_str());
				count_elts = 0;
				paren_count = 0;
				if (comp_ptr->Get_add_formula().size() > 0)
				{
					strcpy(token, comp_ptr->Get_add_formula().c_str());
					ptr = &(token[0]);
					get_elts_in_species(&ptr, 1.0);
				}
				else
				{
					strcpy(token, phase_ptr->formula);
					add_elt_list(phase_ptr->next_elt, 1.0);
				}
				qsort(elt_list, (size_t) count_elts,
					  (size_t) sizeof(struct elt_list), elt_list_compare);
				elt_list_combine();
				tally_table[n].formula = elt_list_save();
			}
		}
	}
#ifdef SKIP
	count_tt_pure_phase = 0;
	if (count_pp_assemblage > 0)
	{
		/* 
		 * Go through all pure phases in pure phase assemblages
		 */
		for (i = 0; i < count_pp_assemblage; i++)
		{
			pp_assemblage_ptr = &pp_assemblage[i];
			for (j = 0; j < pp_assemblage_ptr->count_comps; j++)
			{
				pure_phase_ptr = &pp_assemblage_ptr->pure_phases[j];
				/* 
				 * check if already in tally_table
				 */
				for (k = 1; k < count_tally_table_columns; k++)
				{
					if (tally_table[k].type == Pure_phase &&
						tally_table[k].name == pure_phase_ptr->phase->name &&
						tally_table[k].add_formula ==
						pure_phase_ptr->add_formula)
						break;
				}
				if (k < count_tally_table_columns)
					continue;
				/*
				 * Add to table
				 */
				count_tt_pure_phase++;
				n = count_tally_table_columns;
				extend_tally_table();
				tally_table[n].name = pure_phase_ptr->phase->name;
				tally_table[n].type = Pure_phase;
				tally_table[n].add_formula = pure_phase_ptr->add_formula;
				count_elts = 0;
				paren_count = 0;
				if (pure_phase_ptr->add_formula != NULL)
				{
					strcpy(token, pure_phase_ptr->add_formula);
					ptr = &(token[0]);
					get_elts_in_species(&ptr, 1.0);
				}
				else
				{
					strcpy(token, pure_phase_ptr->phase->formula);
					add_elt_list(pure_phase_ptr->phase->next_elt, 1.0);
				}
				qsort(elt_list, (size_t) count_elts,
					  (size_t) sizeof(struct elt_list), elt_list_compare);
				elt_list_combine();
				tally_table[n].formula = elt_list_save();
			}
		}
	}
#endif
/*
 *   Add solid-solution pure phases
 */
	count_tt_ss_phase = 0;
	if (count_ss_assemblage > 0)
	{
		/* 
		 * Go through all components of all solid solutions in solid-solution assemblages
		 */
		for (i = 0; i < count_ss_assemblage; i++)
		{
			ss_assemblage_ptr = &ss_assemblage[i];
			for (j = 0; j < ss_assemblage_ptr->count_s_s; j++)
			{
				s_s_ptr = &ss_assemblage_ptr->s_s[j];
				for (k = 0; k < s_s_ptr->count_comps; k++)
				{
					s_s_comp_ptr = &s_s_ptr->comps[k];
					/* 
					 * check if already in tally_table
					 */
					for (l = 1; l < count_tally_table_columns; l++)
					{
						if (tally_table[l].type == Ss_phase &&
							tally_table[l].name == s_s_comp_ptr->phase->name)
							break;
					}
					if (l < count_tally_table_columns)
						continue;
					/*
					 * Add to table
					 */
					count_tt_ss_phase++;
					n = count_tally_table_columns;
					extend_tally_table();
					tally_table[n].name = s_s_comp_ptr->phase->name;
					tally_table[n].type = Ss_phase;
					count_elts = 0;
					paren_count = 0;
					strcpy(token, s_s_comp_ptr->phase->formula);
					add_elt_list(s_s_comp_ptr->phase->next_elt, 1.0);
					qsort(elt_list, (size_t) count_elts,
						  (size_t) sizeof(struct elt_list), elt_list_compare);
					elt_list_combine();
					tally_table[n].formula = elt_list_save();
				}
			}
		}
	}
/*
 *   Add kinetic reactants
 */
	count_tt_kinetics = 0;
	if (count_kinetics > 0)
	{
		for (i = 0; i < count_kinetics; i++)
		{
			kinetics_ptr = &kinetics[i];
			for (j = 0; j < kinetics_ptr->count_comps; j++)
			{
				kinetics_comp_ptr = &kinetics_ptr->comps[j];
				/* 
				 * check if already in tally_table
				 */
				for (l = 1; l < count_tally_table_columns; l++)
				{
					if (tally_table[l].type == Kinetics &&
						tally_table[l].name == kinetics_comp_ptr->rate_name)
						break;
				}
				if (l < count_tally_table_columns)
					continue;
				/*
				 * Add to table
				 */
				count_tt_kinetics++;
				n = count_tally_table_columns;
				extend_tally_table();
				tally_table[n].name = kinetics_comp_ptr->rate_name;
				tally_table[n].type = Kinetics;
				/*
				 * get formula for kinetic component
				 */
				count_elts = 0;
				paren_count = 0;
				phase_ptr = NULL;
				if (kinetics_ptr->comps[j].count_list == 1)
				{
					strcpy(token, kinetics_ptr->comps[j].list[0].name);
					phase_ptr = phase_bsearch(token, &p, FALSE);
				}
				if (phase_ptr != NULL)
				{
					add_elt_list(phase_ptr->next_elt, 1.0);
				}
				else
				{
					for (k = 0; k < kinetics_ptr->comps[j].count_list; k++)
					{
						char * temp_name = string_duplicate(kinetics_ptr->comps[j].list[k].name);
						ptr = temp_name;
						get_elts_in_species(&ptr,
											1.0 *
											kinetics_ptr->comps[j].list[k].
											coef);
						free_check_null(temp_name);
					}
				}
				qsort(elt_list, (size_t) count_elts,
					  (size_t) sizeof(struct elt_list), elt_list_compare);
				elt_list_combine();
				tally_table[n].formula = elt_list_save();
			}
		}
	}
#ifdef SKIP
	/*
	 *  Debug print for table definition
	 */
	output_msg(sformatf( "List of rows for tally table\n"));
	for (i = 0; i < count_tally_table_rows; i++)
	{
		output_msg(sformatf( "\t%-s\n", buffer[i].name));
	}
	output_msg(sformatf( "\nList of columns for tally table\n"));
	for (i = 0; i < count_tally_table_columns; i++)
	{
		output_msg(sformatf( "\t%-20s\tType: %d\n",
				   tally_table[i].name, tally_table[i].type));
		if (tally_table[i].formula != NULL)
		{
			for (j = 0; tally_table[i].formula[j].elt != NULL; j++)
			{
				output_msg(sformatf( "\t\t%-10s\t%f\n",
						   tally_table[i].formula[j].elt->name,
						   (double) tally_table[i].formula[j].coef));
			}
		}
	}
#endif
	pr.use = save_print_use;
	return (OK);
}

/* ---------------------------------------------------------------------- */
void Phreeqc::
add_all_components_tally(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Routine accumulates elements from all solutions, phases, gas phases,
 *   exchangers, and surfaces. Counts number of aqueous components
 *   to transport. Stores in global variable tally_count_component.
 *   Also calculates a number greater than all user numbers and
 *   stores in global variable first_user_number.
 */
	int i, save_print_use;

	save_print_use = pr.use;
	pr.use = FALSE;
/*
 *   Delete solutions less than -1
 */
	while (count_solution > 0 && solution[0]->n_user < -1)
	{
		i = solution[0]->n_user;
		solution_delete(i);
	}
/*
 *   add all solutions
 */
	xsolution_zero();
	for (i = 0; i < count_solution; i++)
	{
		add_solution(solution[i], 1.0 / solution[i]->mass_water, 1.0);
	}
/*
 *   add all irrev reactions
 */
	{
		std::map<int, cxxReaction>::iterator it = Rxn_reaction_map.begin();
		for (; it != Rxn_reaction_map.end(); it++)
		{
			add_reaction(&it->second, 1, 1.0);
		}
	}
#ifdef SKIP
	for (i = 0; i < count_irrev; i++)
	{
		add_reaction(&irrev[i], 1, 1.0);
	}
#endif
/*
 *   Add pure phases
 */
	{
		std::map<int, cxxPPassemblage>::iterator it = Rxn_pp_assemblage_map.begin();
		for (; it != Rxn_pp_assemblage_map.end(); it++)
		{
			add_pp_assemblage(&(it->second));
		}
	}
#ifdef SKIP
	for (i = 0; i < count_pp_assemblage; i++)
	{
		add_pp_assemblage(&pp_assemblage[i]);
	}
#endif
/*
 *   Exchangers
 */
	{
		std::map<int, cxxExchange>::iterator it = Rxn_exchange_map.begin();
		for (; it != Rxn_exchange_map.end(); it++)
		{
			add_exchange(&it->second);
		}
	}
#ifdef SKIP
	for (i = 0; i < count_exchange; i++)
	{
		add_exchange(&exchange[i]);
	}
#endif
/*
 *   Surfaces
 */
	for (i = 0; i < count_surface; i++)
	{
		add_surface(&surface[i]);
	}
/*
 *   Gases
 */
	{
		std::map<int, cxxGasPhase>::iterator it = Rxn_gas_phase_map.begin();
		for ( ; it != Rxn_gas_phase_map.end(); it++)
		{
			add_gas_phase(&it->second);
		}
	}
#ifdef SKIP
	for (i = 0; i < count_gas_phase; i++)
	{
		add_gas_phase(&gas_phase[i]);
	}
#endif
/*
 *   Add solid-solution pure phases
 */
	for (i = 0; i < count_ss_assemblage; i++)
	{
		add_ss_assemblage(&ss_assemblage[i]);
	}
/*
 *   Add elements in kinetic reactions
 */
	for (i = 0; i < count_kinetics; i++)
	{
		calc_dummy_kinetic_reaction_tally(&kinetics[i]);
		add_kinetics(&kinetics[i]);
	}
/*
 *   reset pr.use
 */
	pr.use = save_print_use;
	return;
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
calc_dummy_kinetic_reaction_tally(struct kinetics *kinetics_ptr)
/* ---------------------------------------------------------------------- */
{
/*
 *    Go through kinetic components and add positive amount of each reactant
 */
	int i, j;
	LDBLE coef;
	char token[MAX_LENGTH];
	char *ptr;
	struct phase *phase_ptr;
/*
 *   Go through list and generate list of elements and
 *   coefficient of elements in reaction
 */
	free_check_null(kinetics_ptr->totals);
	count_elts = 0;
	paren_count = 0;
	for (i = 0; i < kinetics_ptr->count_comps; i++)
	{
		coef = 1.0;
/*
 *   Reactant is a pure phase, copy formula into token
 */
		phase_ptr = NULL;
		if (kinetics_ptr->comps[i].count_list == 1)
		{
			strcpy(token, kinetics_ptr->comps[i].list[0].name);
			phase_ptr = phase_bsearch(token, &j, FALSE);
		}
		if (phase_ptr != NULL)
		{
			add_elt_list(phase_ptr->next_elt, coef);
		}
		else
		{
			for (j = 0; j < kinetics_ptr->comps[i].count_list; j++)
			{
				char * temp_name = string_duplicate(kinetics_ptr->comps[i].list[j].name);
				ptr = temp_name;
				get_elts_in_species(&ptr, coef);
				free_check_null(temp_name);
			}
		}

	}
	kinetics_ptr->totals = elt_list_save();

	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
extend_tally_table(void)
/* ---------------------------------------------------------------------- */
{
	int i, j;
	/* 
	 * adds another column to tally_table
	 * increments number of columns
	 */
	tally_table =
		(struct tally *) PHRQ_realloc((void *) tally_table,
									  (size_t) (count_tally_table_columns +
												1) * sizeof(struct tally));
	if (tally_table == NULL)
		malloc_error();
	for (i = 0; i < 3; i++)
	{
		tally_table[count_tally_table_columns].total[i] =
			(struct tally_buffer *)
			PHRQ_malloc((size_t) (count_tally_table_rows) *
						sizeof(struct tally_buffer));
		if (tally_table[count_tally_table_columns].total[i] == NULL)
			malloc_error();
		for (j = 0; j < count_tally_table_rows; j++)
		{
			tally_table[count_tally_table_columns].total[i][j].name =
				t_buffer[j].name;
			tally_table[count_tally_table_columns].total[i][j].master =
				t_buffer[j].master;
		}
	}
	tally_table[count_tally_table_columns].name = NULL;
	tally_table[count_tally_table_columns].type = UnKnown;
	tally_table[count_tally_table_columns].add_formula = NULL;
	tally_table[count_tally_table_columns].moles = 0.0;
	tally_table[count_tally_table_columns].formula = NULL;
	count_tally_table_columns++;
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
set_reaction_moles(int n_user, LDBLE moles)
/* ---------------------------------------------------------------------- */
{
	cxxReaction *reaction_ptr = Utilities::Rxn_find(Rxn_reaction_map, n_user);
	if (reaction_ptr == NULL)
		return (ERROR);
	std::vector<LDBLE> v;
	v.push_back(moles);
	reaction_ptr->Set_steps(v);
	reaction_ptr->Set_countSteps(1);
	reaction_ptr->Set_equalIncrements(true);
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
set_reaction_temperature(int n_user, LDBLE tc)
/* ---------------------------------------------------------------------- */
{
	cxxTemperature * temperature_ptr = Utilities::Rxn_find(Rxn_temperature_map, n_user);
	if (temperature_ptr == NULL)
		return (ERROR);
	temperature_ptr->Get_temps().clear();
	temperature_ptr->Get_temps().push_back(tc);
	temperature_ptr->Set_equalIncrements(false);
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
set_kinetics_time(int n_user, LDBLE step)
/* ---------------------------------------------------------------------- */
{
	int n;
	struct kinetics *kinetics_ptr;

	kinetics_ptr = kinetics_bsearch(n_user, &n);
	if (kinetics_ptr == NULL)
		return (ERROR);
	kinetics_ptr->steps[0] = step;
	kinetics_ptr->count_steps = 1;
	return (OK);
}
