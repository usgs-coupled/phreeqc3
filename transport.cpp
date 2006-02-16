#define  EXTERNAL extern
#include "global.h"
#include "phqalloc.h"
#include "output.h"
#include "phrqproto.h"

static char const svnid[] = "$Id: transport.c 78 2005-02-01 22:47:12Z dlpark $";

static int init_mix(void);
static int init_heat_mix(int nmix);
static int heat_mix(int heat_nmix);
static int mix_stag(int i, LDBLE stagkin_time, int punch, LDBLE step_fraction_kin);


LDBLE *heat_mix_array;
LDBLE *temp1, *temp2;
int heat_nmix;
LDBLE heat_mix_f_imm, heat_mix_f_m;

/* ---------------------------------------------------------------------- */
int transport(void)
/* ---------------------------------------------------------------------- */
{
        int i, j, k, n;
	int j_imm, n_m, n_imm;
	LDBLE b, f, mix_f_m, mix_f_imm;
	LDBLE water_m, water_imm;
        int first_c, last_c, b_c;
        int nmix;
        int max_iter;
        char token[MAX_LENGTH];
        LDBLE kin_time, stagkin_time, kin_time_save;
	struct mix *mix_ptr;
	int punch_boolean;
	LDBLE step_fraction;
	if (svnid == NULL) fprintf(stderr," ");
	
        state = TRANSPORT;
/*      mass_water_switch = TRUE; */
/*
 *   Check existence of solutions
 */
	j = -1;
	/* check column solutions */
        for (i = 1; i <= count_cells; i++) {
                use.solution_ptr = solution_bsearch(i, &n, TRUE);
                if (use.solution_ptr == NULL) {
                        input_error++;
                        sprintf(error_string, "Solution %d is needed for transport, but is not defined.", i);
                        error_msg(error_string, CONTINUE);
                } else {
                        cell_data[i-1].temp = use.solution_ptr->tc;
                }
        }

	/* check solution 0 */
        if (ishift == 1) {
                if (solution_bsearch(0, &n, TRUE) == NULL) {
                        input_error++;
                        sprintf(error_string, "Solution 0 is needed for transport, but is not defined.");
                        error_msg(error_string, CONTINUE);
                }
        }

	/* check solution count_cells */
        if (ishift == -1) {
                if (solution_bsearch(count_cells+1, &n, TRUE) == NULL) {
                        input_error++;
                        sprintf(error_string, "Solution %d is needed for transport, but is not defined.", count_cells+1);
                        error_msg(error_string, CONTINUE);
                }
        }
/*
 *   Stop if error
 */
        if (input_error > 0) {
                error_msg("Program terminating due to input errors.", STOP);
        }
/*
 *   Initialize temperature in stagnant cells ...
 */
        for (n=1; n<=stag_data->count_stag; n++) {
                for (i=1; i <= count_cells; i++) {
                        k = i+1+n*count_cells;
                        use.solution_ptr = solution_bsearch(k, &use.n_solution, FALSE);
                        if (use.solution_ptr != NULL) {
                                cell_data[k-1].temp = use.solution_ptr->tc;
                        }
                }
        }

        if (solution_bsearch(0, &n, FALSE) == NULL) solution_duplicate(1,0);
        if (solution_bsearch(count_cells+1, &n, FALSE) == NULL) solution_duplicate(count_cells, count_cells+1);

/*
 *  Initialize mixing factors, define kinetics times
 */
	if ((stag_data->exch_f > 0) && (stag_data->count_stag == 1)) {
		mix_ptr = &mix[0];
		for (i = 0; i < count_mix; i++) mix_free(mix_ptr++);
		count_mix = 2*count_cells;
		mix = (struct mix *) PHRQ_realloc(mix, (size_t) count_mix * sizeof(struct mix));
		if (mix == NULL) malloc_error();
		memset(mix, 0, sizeof(struct mix) * count_mix);
	}
        nmix = init_mix();
	heat_nmix = init_heat_mix(nmix);
        if (nmix < 2) stagkin_time = timest; else stagkin_time = timest/nmix;
        if (ishift != 0 ) kin_time = timest/(1+nmix); else kin_time = stagkin_time;
	kin_time_save = kin_time;

/* Reaction defined for a shift... */
	step_fraction = 1.0/(1.0+nmix);

/*
 *   Set boundary conditions, transport direction
 */
        last_model.force_prep = TRUE;
        if ((ishift == 0) || (bcon_first == 1) || (bcon_last == 1)) {
                b_c = 1;
        } else {
		b_c = 0;
	}
        if (ishift >= 0) {
                last_c = count_cells;
                first_c = 1;
        } else  {
                last_c = 1;
		first_c = count_cells;
        }
/*
 * First equilibrate solutions
 */
        dup_print("Equilibrating initial solutions", TRUE);
        transport_step = 0;
        for (i=1; i <= count_cells; i++) {
		set_initial_moles(i);
		cell_no = i;
		set_and_run_wrapper(i, NOMIX, FALSE, i, 0.0);
                if (cell_data[i-1].punch == TRUE)  punch_all();
                if (cell_data[i-1].print == TRUE) print_all();
                saver();
        }
/*
 * Also stagnant cells
 */
        for (n=1; n<=stag_data->count_stag; n++) {
                for (i=1; i <= count_cells; i++) {
                        k = i+1+n*count_cells;
			cell_no = k;
                        if (solution_bsearch(k, &use.n_solution, FALSE) != 0 ) {
				set_initial_moles(k);
				set_and_run_wrapper(k, NOMIX, FALSE, k, 0.0);
                                if ((cell_data[k-1].punch == TRUE))  punch_all();
				if ((cell_data[k-1].print == TRUE) && (transport_step % print_modulus == 0)) print_all();
                                saver();
                        }
                }
        }
/*
 * Define stagnant/mobile mix structure, if not read explicitly.
 *
 * With count_stag = 1, mix factors are calculated from exchange factor à
 * (= exch_f), mobile é_m (= th_m) and immobile é_im (= th_im) porosity.
 * These variables are read under keyword TRANSPORT, after stagnant, in
 * structure stag_data.
 * MIX 'cell_no' in input file can be an alternative for the calculation here.
 */

	if ((stag_data->exch_f > 0) && (stag_data->count_stag == 1)) {
	
		b = stag_data->th_m / (stag_data->th_m + stag_data->th_im);
		f = exp( - stag_data->exch_f * stagkin_time / (b * stag_data->th_im) );
		mix_f_imm = b - b * f;
		mix_f_m = mix_f_imm * stag_data->th_im / stag_data->th_m;

		n = 0;
		for (i=1; i <= count_cells; i++) {
			j = i;
			j_imm = j + (1 + count_cells);
			if (solution_bsearch(j, &n_m, TRUE) == NULL) {
				error_msg("Could not find mobile cell solution in TRANSPORT.", STOP);
			}
			if (solution_bsearch(j_imm, &n_imm, TRUE) == NULL) {
				error_msg("Could not find immobile cell solution in TRANSPORT.", STOP);
			}
			water_m = solution[n_m]->mass_water;
			water_imm = solution[n_imm]->mass_water;
/*
 * Define C_m = (1 - mix_f_m) * C_m0  +  mix_f_m) * C_im0
 */
			mix[n].comps = (struct mix_comp *) PHRQ_malloc((size_t) 2*sizeof(struct mix_comp));
			if (mix[n].comps == NULL) malloc_error();
			mix[n].count_comps = 2;
			mix[n].description = string_duplicate(" ");
			mix[n].n_user = j;
			mix[n].n_user_end = j;
			mix[n].comps[0].n_solution = j;
			mix[n].comps[0].fraction = 1 - mix_f_m;
			mix[n].comps[1].n_solution = j_imm;
			mix[n].comps[1].fraction = mix_f_m*water_m/water_imm;
			n++;
/*
 * Define C_im = mix_f_imm * C_m0  +  (1 - mix_f_imm) * C_im0,  or...
 */
			mix[n].comps = (struct mix_comp *) PHRQ_malloc((size_t) 2*sizeof(struct mix_comp));
			if (mix[n].comps == NULL) malloc_error();
			mix[n].count_comps = 2;
			mix[n].description = string_duplicate(" ");
			mix[n].n_user = j_imm;
			mix[n].n_user_end = j_imm;
			mix[n].comps[0].n_solution = j_imm;
			mix[n].comps[0].fraction = 1 - mix_f_imm;
			mix[n].comps[1].n_solution = j;
			mix[n].comps[1].fraction = mix_f_imm*water_imm/water_m;
			n++;
		}

		if (heat_nmix > 0) {
/*
 * Assumption: D_e used for calculating exch_f in input file equals diffc
 */
	 		f = stag_data->exch_f * (heat_diffc - diffc) / diffc / tempr;
			f = exp( - f * stagkin_time / (b * stag_data->th_im) );
			heat_mix_f_imm = b - b * f;
			heat_mix_f_m = heat_mix_f_imm * stag_data->th_im / stag_data->th_m;
		}
	}
/*        
 * Now transport 
 */
        max_iter = 0;
        for (transport_step = transport_start; transport_step <= count_shifts; transport_step++) {
		/*
		 *  Set initial moles of phases
		 */
		for (i = 1; i <= count_cells; i++) {
			set_initial_moles(i);
		}

/*
 * Start diffusing if boundary cond = 1, (fixed c, or closed)
 */
                if (b_c == 1) {

 			/* For half of mixing steps */

			for (j = 1; j <= floor(nmix/2); j++) {
				rate_sim_time_start = (transport_step - 1) * timest + (j-1) * kin_time;
				rate_sim_time = rate_sim_time_start + kin_time;

				sprintf(token, "Transport step %3d. Mixrun %3d.", transport_step, j);
				dup_print(token, FALSE);

				if (heat_nmix > 0) {
					heat_mix(heat_nmix);
				/* equilibrate again ... */
				        for (i=1; i <= count_cells; i++) {
						cell_no = i;
						set_and_run_wrapper(i, NOMIX, FALSE, i, 0.0);
				                saver();
        				}
				}

				/* Go through cells */

                                for (i = 1; i <= count_cells; i++) {

#ifdef SKIP
/* this is somewhere lost; quickened in set_and_run ... */
                                	if (i == 1) {
						last_model.force_prep = TRUE;
					}
#endif
                                        if (iterations > max_iter) max_iter = iterations;
					sprintf(token, "Transport step %3d. Mixrun %3d. Cell %3d. (Max. iter %3d)", transport_step, j, i, max_iter);
					status(0, token);

					cell_no = i;
					run_reactions(i, kin_time, DISP, step_fraction);

					/* punch and output file */
                                        if ((ishift == 0) && (j == nmix) &&
					    ((stag_data->count_stag == 0) ||
					     solution_bsearch(i+1+count_cells, &use.n_solution, FALSE) == 0)){
						if ((cell_data[i-1].punch == TRUE) &&
						    (transport_step % punch_modulus == 0)) punch_all();
						if ((cell_data[i-1].print == TRUE) &&
						    (transport_step % print_modulus == 0)) print_all();
					}
                                        if (i > 1) solution_duplicate(-2, i-1);
                                        saver();
                                }
				solution_duplicate(-2, count_cells);

				/* Stagnant zone mixing after completion of each
				   diffusive/dispersive step ...  */

				rate_sim_time_start = (transport_step - 1)*timest + (j-1)*stagkin_time;
				rate_sim_time = rate_sim_time_start + stagkin_time;

				if (stag_data->count_stag > 0) {
					if ((ishift == 0) && (j == nmix)) punch_boolean = TRUE; else punch_boolean = FALSE;
					for (i = 1; i <= count_cells; i++) mix_stag(i, stagkin_time, punch_boolean, step_fraction);
				}
			}
                }
/*
 * Advective transport
 */
                if (ishift !=0) {
			sprintf(token, "Transport step %3d.", transport_step);
                        dup_print(token, FALSE);
			if (b_c == 1) {
				rate_sim_time_start = (transport_step -1)*timest + (j-1)*kin_time;
			} else {
				rate_sim_time_start = (transport_step -1)*timest;
			}
			rate_sim_time = rate_sim_time_start + kin_time;

/* halftime kinetics for resident water in first cell ... */
                        if (kinetics_bsearch(first_c, &i) != NULL && count_cells > 1) {
                        	cell_no = first_c;
                                kin_time = kin_time_save / 2;
                                run_reactions(first_c, kin_time, NOMIX, 0.0);
                                saver();
				kin_time = kin_time_save;
                        }

			/* for each cell in column */
/* Begin revision Dec 7, 1999 */
                        for (i = last_c; i != (first_c-ishift); i-=ishift) {
                                solution_duplicate(i-ishift, i);
			}
/*
 * thermal diffusion when nmix = 0...
 */
			if ((nmix == 0) && (heat_nmix > 0)) {
				heat_mix(heat_nmix);
			/* equilibrate again ... */
			        for (i=1; i <= count_cells; i++) {
					cell_no = i;
					set_and_run_wrapper(i, NOMIX, FALSE, i, 0.0);
			                saver();
        			}
			}

                        for (i = 1; i <= count_cells; i++) {
				if (i == first_c && count_cells > 1) kin_time /= 2;
				sprintf(token, "Transport step %3d. Mixrun %3d. Cell %3d. (Max. iter %3d)", transport_step, 0, i, max_iter);
				status(0, token);
				cell_no = i;
				run_reactions(i, kin_time, NOMIX, step_fraction);
                                if (iterations > max_iter) max_iter = iterations;
/* end revision Dec 7, 1999 */

				if ((nmix == 0) &&
				    ((stag_data->count_stag == 0) ||
				     (solution_bsearch(i+1+count_cells, &use.n_solution, FALSE) == 0))) {
					if ((cell_data[i-1].punch == TRUE) &&
					    (transport_step % punch_modulus == 0)) punch_all();
					if ((cell_data[i-1].print == TRUE) &&
					    (transport_step % print_modulus == 0)) print_all();
				}
				if (i == first_c && count_cells > 1) kin_time = kin_time_save;
                                saver();

				/* If nmix is zero, stagnant zone mixing after
				   advective step ... */

                                if ((nmix == 0) && (stag_data->count_stag > 0)) mix_stag(i, stagkin_time, TRUE, step_fraction);
                        }
		}

/*
 * Further dispersive and diffusive transport
 */
		if (b_c != 1) j = 1;
		for (j = j; j <= nmix; j++) {
			sprintf(token, "Transport step %3d. Mixrun %3d.", transport_step, j);
			dup_print(token, FALSE);
			rate_sim_time_start = (transport_step - 1)*timest + (j - 1) * kin_time;
			if (ishift != 0) rate_sim_time_start += kin_time;
			rate_sim_time = rate_sim_time_start + kin_time;

			if (heat_nmix > 0) {
				heat_mix(heat_nmix);
			/* equilibrate again ... */
			        for (i=1; i <= count_cells; i++) {
					cell_no = i;
					set_and_run_wrapper(i, NOMIX, FALSE, i, 0.0);
			                saver();
       				}
			}

			/* for each cell in column */

			for (i = 1; i <= count_cells; i++) {
				if (iterations > max_iter) max_iter = iterations;
				sprintf(token, "Transport step %3d. Mixrun %3d. Cell %3d. (Max. iter %3d)", transport_step, j, i, max_iter);
				status(0, token);
				cell_no = i;
				run_reactions(i, kin_time, DISP, step_fraction);

				if ((j == nmix) &&
				    ((stag_data->count_stag == 0) ||
				     (solution_bsearch(i+1+count_cells, &use.n_solution, FALSE) == 0))) {
					if ((cell_data[i-1].punch == TRUE) &&
					    (transport_step % punch_modulus == 0))  punch_all();
					if ((cell_data[i-1].print == TRUE) &&
					    (transport_step % print_modulus == 0)) {
						print_all();
					}
				}
				if (i > 1) solution_duplicate(-2, i-1);
				saver();
			}
			solution_duplicate(-2, count_cells);
			/* Stagnant zone mixing after completion of each 
			   diffusive/dispersive step ... */

			rate_sim_time_start = (transport_step - 1)*timest + (j - 1) * stagkin_time;
			rate_sim_time = rate_sim_time_start + stagkin_time;

			if (stag_data->count_stag > 0) {
				if (j == nmix) punch_boolean = TRUE; else punch_boolean = FALSE;
				for (i = 1; i <= count_cells; i++) mix_stag(i, stagkin_time, punch_boolean, step_fraction);
			}
		}
		if (dump_modulus != 0 && (transport_step % dump_modulus) == 0) dump();
	}
#ifdef DOS
        output_msg(OUTPUT_SCREEN, "\n");
#else
	output_msg(OUTPUT_SCREEN, "%s%-80s", "\n", " ");
#endif
        /* free_model_allocs(); */
/*
 * free mix structures
 */
	if ((stag_data->exch_f > 0) && (stag_data->count_stag == 1)) {
		mix_ptr = &mix[0];
		for (i = 0; i < count_mix; i++) mix_free(mix_ptr++);
		count_mix =0;
	} else {
		if (nmix > 0) {
			mix_ptr = &mix[count_mix - count_cells];
			for (i = count_mix - count_cells; i < count_mix; i++) {
				mix_free(mix_ptr++);
			}
			count_mix -= count_cells;
			mix = (struct mix *) PHRQ_realloc(mix, (size_t) (count_mix + 1)*sizeof(struct mix));
			if (mix == NULL) malloc_error();
		}
	}
        if (heat_nmix > 0) {
	        free_check_null(heat_mix_array);
        	free_check_null(temp1);
                free_check_null(temp2);
        }

	initial_total_time += rate_sim_time;
	rate_sim_time = 0;
        mass_water_switch = FALSE;
        return(OK);
}
/* ---------------------------------------------------------------------- */
int init_mix(void)
/* ---------------------------------------------------------------------- */
{
        LDBLE dav, lav, mixf, maxmix, corr_disp;
        int i, n, nmix, count_comps, max_mix;
        LDBLE *m;

        m = (LDBLE *) PHRQ_malloc((count_cells+1)*sizeof(LDBLE));
        if (m == NULL) malloc_error();

/*
 * Define mixing factors among inner cells
 */
	corr_disp = 1.;
	if (correct_disp == TRUE && ishift != 0) {
		if (bcon_first == 3) corr_disp += 1. / count_cells;
		if (bcon_last == 3) corr_disp += 1. / count_cells;
	}
        maxmix = 0.0;
        for (i=1; i < count_cells; i++) {
                lav = (cell_data[i-1].length + cell_data[i].length)/2;
                if (ishift != 0) {
			dav = (cell_data[i-1].disp + cell_data[i].disp)/2;
		} else {
			dav = 0;
		}
                mixf = (diffc*timest/lav + dav) * corr_disp / lav;

                if (mixf > maxmix) maxmix = mixf;
                m[i] = mixf;           /* m[i] has mixf with lower cell */
        }
/*
 * Also for boundary cells
 */
        if (bcon_first == 1) {
                lav = cell_data[0].length;
                if (ishift != 0) {
			dav = cell_data[0].disp;
		} else {
			dav = 0;
		}
                mixf = (diffc*timest/lav + dav)/lav;
                if (mixf > maxmix) maxmix = mixf;
                m[0] = 2*mixf;
        } else {
		m[0] = 0;
	}
        if (bcon_last == 1) {
                lav = cell_data[count_cells-1].length;
                if (ishift != 0) {
			dav = cell_data[count_cells-1].disp;
		} else {
			dav = 0;
		}
                mixf = (diffc*timest/lav + dav)/lav;
                if (mixf > maxmix) maxmix = mixf;
                m[count_cells] = 2*mixf;
        } else {
		m[count_cells] = 0;
        }
/*
 * Find number of mixes
 */
          if (maxmix == 0) {
                nmix = 0;
          } else {
		if ((bcon_first == 1) || (bcon_last == 1)) {
 			nmix = 1 + (int) floor(4.5 * maxmix);
		} else {
 			nmix = 1 + (int) floor(3.0 * maxmix);
		}
		if ((ishift != 0) && ((bcon_first == 1) || (bcon_last == 1))) {
			if (nmix < 2) nmix = 2;
		}
		for (i=0; i<=count_cells; i++) m[i] /= nmix;
          }
 /*
  * Fill mix structure
  */
        if (nmix != 0) {
                mix = (struct mix *) PHRQ_realloc(mix, (size_t) (count_mix + count_cells)*sizeof(struct mix));
                if (mix == NULL) malloc_error();
                count_mix += count_cells;
                for (n = count_mix-count_cells; n<count_mix; n++) {
			mix[n].description = NULL;
                        mix[n].count_comps = 3;
                        mix[n].comps = (struct mix_comp *) PHRQ_malloc((size_t) 3*sizeof(struct mix_comp));
                        if (mix[n].comps == NULL) malloc_error();
                }

                n = count_mix - count_cells;
/*
 * max_mix brings n_user outside range of active cells
 * mix[n].n_user = mix[n].n_user_end = -999 has same effect
 * but max_mix keeps mix in sort order in case mix_bsearch
 * is used
 */
		if (n - 1 <= 0) {
			max_mix = 1;
		} else {
			max_mix = mix[n-1].n_user + 1;
		}
		if (max_mix < count_cells * (stag_data->count_stag + 1) + 1) {
			max_mix = count_cells * (stag_data->count_stag + 1) + 1;
		}
                for (i=1; i<=count_cells; i++) {
                        dav = 0;
                        count_comps = 0;
			mix[n].description = (char *) free_check_null(mix[n].description);
                        mix[n].description = string_duplicate(" ");
/*
 * max_mix brings n_user outside range of active cells
 * mix[n].n_user = mix[n].n_user_end = -999 has same effect
 * but max_mix keeps mix in sort order in case mix_bsearch
 * is used
 */
                        mix[n].n_user = max_mix + i ;
                        mix[n].n_user_end = max_mix + i ;
			
                        mix[n].comps[count_comps].n_solution = i-1;
                        mix[n].comps[count_comps].fraction = m[i-1];
                        dav += m[i-1];
                        count_comps++;
                        mix[n].comps[count_comps].n_solution = i+1;
                        mix[n].comps[count_comps].fraction = m[i];
                        dav += m[i];
                        count_comps++;
                        mix[n].comps[count_comps].n_solution = i;
                        mix[n].comps[count_comps].fraction = 1.0 - dav;

                        n++;
                }
        }
        m = (LDBLE *) free_check_null(m);
        return(nmix);
}
/* ---------------------------------------------------------------------- */
int mix_stag(int i, LDBLE kin_time, int punch, LDBLE step_fraction)
/* ---------------------------------------------------------------------- */
{
        int n, k, l;
        LDBLE t_imm;
        struct solution *ptr_imm, *ptr_m;
#ifdef SKIP
	char str[MAX_LENGTH];	
#endif
/*
 * Kinetics in transport cell is done while transporting
 */
        for (n=1; n<=stag_data->count_stag; n++) {
                k = i+1+n*count_cells;
                if ((ptr_imm = solution_bsearch(k, &use.n_solution, FALSE)) != NULL ) {
                        if (n == 1) {
                        	if (heat_nmix > 0) {
                                	ptr_m = solution_bsearch(i, &use.n_solution, FALSE);
                                	t_imm = heat_mix_f_imm * ptr_m->tc + (1 - heat_mix_f_imm) * ptr_imm->tc;
                                        ptr_m->tc = heat_mix_f_m * ptr_imm->tc + (1 - heat_mix_f_m) * ptr_m->tc;
                                        cell_data[i-1].temp = ptr_m->tc;
                                        cell_data[k-1].temp = ptr_imm->tc = t_imm;
				/* equilibrate again ... */
					cell_no = i;
					set_and_run_wrapper(i, NOMIX, FALSE, i, 0.0);
					saver();
					cell_no = k;
					set_and_run_wrapper(k, NOMIX, FALSE, k, 0.0);
					saver();
                                }
/*
 * Mobile cell, kinetics already done ...
 */
				cell_no = i;
				set_and_run_wrapper(i, STAG, FALSE, -2, 0.0);
				if ((use.kinetics_ptr = kinetics_bsearch(i, &l)) != NULL) {
					use.n_kinetics_user = i;
					use.kinetics_in = TRUE;
				}
				if ((punch == TRUE) && 
				    (cell_data[i-1].punch == TRUE) &&
				    (transport_step % punch_modulus == 0)) punch_all();
				if ((punch == TRUE) && 
				    (cell_data[i-1].print == TRUE) && 
				    (transport_step % print_modulus == 0)) {
					print_all();
				}
                                saver();
                        }
			cell_no = k;
			run_reactions(k, kin_time, STAG, step_fraction);
			if ((cell_data[k-1].punch == TRUE) &&
			    (punch == TRUE) && 
			    (transport_step % punch_modulus == 0)) punch_all();
			if ((cell_data[k-1].print == TRUE) &&
			    (punch == TRUE) &&
			    (transport_step % print_modulus == 0)) {
				print_all();
			}
                        saver();
                }
        }
        for (n=1; n<=stag_data->count_stag; n++) {
                k = i+1+n*count_cells;
                if (solution_bsearch(k, &use.n_solution, FALSE) != 0 ) {
                        solution_duplicate(-2-k, k);
                        if (n == 1) solution_duplicate(-2, i);
                }
        }
        return(OK);
}
/* ---------------------------------------------------------------------- */
int init_heat_mix(int nmix)
/* ---------------------------------------------------------------------- */
{
        LDBLE lav, mixf, maxmix, corr_disp;
        int i, j, k, n;
        int heat_nmix;
        LDBLE t0;
/*
 * Check for need to model thermal diffusion...
 */
	if (heat_diffc <= diffc) return(0);
        if (count_cells < 2) return(0);

	heat_nmix = 0;
	t0 = solution_bsearch(0, &n, FALSE)->tc;
	for (i = 0; i < count_cells; i++) {
        	if (fabs(cell_data[i].temp - t0) > 1.0) {
                	heat_nmix = 1;
                        break;
                }
        }
        if (heat_nmix == 0) {
        	if (fabs(solution_bsearch(count_cells+1, &n, FALSE)->tc - t0) > 1.0) heat_nmix = 1;
		for (n = 1; n <= stag_data->count_stag; n++) {
                	for (i = 1; i < count_cells; i++) {
				k = i+1+n*count_cells;
                		if (solution_bsearch(k, &j, FALSE) != 0 ) {
        				if (fabs(cell_data[k-1].temp - t0) > 1.0) {
                				heat_nmix = 1;
                        			break;
                                        }
		                }
                	}
                }
        }
        if (heat_nmix == 0) return(0);
/*
 * Initialize arrays...
 */
        heat_mix_array = (LDBLE *) PHRQ_malloc((count_cells+2)*sizeof(LDBLE));
        if (heat_mix_array == NULL) malloc_error();
        temp1 = (LDBLE *) PHRQ_malloc((count_cells + 2)*sizeof(LDBLE));
        if (temp1 == NULL) malloc_error();
        temp2 = (LDBLE *) PHRQ_malloc((count_cells + 2)*sizeof(LDBLE));
        if (temp2 == NULL) malloc_error();
/*
 * Define mixing factors among inner cells...
 */
	corr_disp = 1.;
	if (correct_disp == TRUE && ishift != 0) {
		if (bcon_first == 3) corr_disp += 1. / count_cells;
		if (bcon_last == 3) corr_disp += 1. / count_cells;
	}
	if (nmix > 0) corr_disp /= nmix;
        maxmix = 0.0;
        for (i=1; i < count_cells; i++) {
                lav = (cell_data[i-1].length + cell_data[i].length)/2;
                mixf = (heat_diffc - diffc) * timest * corr_disp / tempr / (lav *lav);
                if (mixf > maxmix) maxmix = mixf;
                heat_mix_array[i+1] = mixf;           /* m[i] has mixf with lower cell */
        }
/*
 * Also for boundary cells
 */
        if (bcon_first == 1) {
                lav = cell_data[0].length;
                mixf = (heat_diffc - diffc) * timest * corr_disp / tempr /(lav * lav);
                if (2*mixf > maxmix) maxmix = 2*mixf;
                heat_mix_array[1] = 2*mixf;
        } else {
		heat_mix_array[1] = 0;
	}
        if (bcon_last == 1) {
                lav = cell_data[count_cells-1].length;
                mixf = (heat_diffc - diffc) * timest * corr_disp / tempr /(lav * lav);
                if (2*mixf > maxmix) maxmix = 2*mixf;
                heat_mix_array[count_cells+1] = 2*mixf;
        } else {
		heat_mix_array[count_cells+1] = 0;
        }
/*
 * Find number of mixes
 */
          if (maxmix == 0) {
                  heat_nmix = 0;
          } else {
 		heat_nmix = 1 + (int) floor(3.0 * maxmix);
		for (i=1; i <= count_cells + 1; i++) heat_mix_array[i] /= heat_nmix;
          }

        return(heat_nmix);
}
/* ---------------------------------------------------------------------- */
int heat_mix(int heat_nmix)
/* ---------------------------------------------------------------------- */
{
	int i, j;

        for (i = 1; i <= count_cells; i++) temp1[i] = solution_bsearch(i, &j, FALSE)->tc;
        temp1[0] = solution_bsearch(0, &j, FALSE)->tc;
        temp1[count_cells+1] = solution_bsearch((count_cells+1), &j, FALSE)->tc;

        for (i = 1; i <= heat_nmix; i++) {
		for (j = 1; j <= count_cells; j++) {
                	temp2[j] = heat_mix_array[j] * temp1[j-1] +
				   heat_mix_array[j+1] * temp1[j+1] +
                                   (1 - heat_mix_array[j] - heat_mix_array[j+1]) * temp1[j];
               }
		for (j = 1; j <= count_cells; j++) temp1[j] = temp2[j];
        }

        for (i = 1; i <= count_cells; i++) {
        	cell_data[i-1].temp = temp1[i];
                solution_bsearch(i, &j, FALSE)->tc = temp1[i];
        }

        return(OK);
}
/* ---------------------------------------------------------------------- */
int set_initial_moles(int i)
/* ---------------------------------------------------------------------- */
{
	struct pp_assemblage *pp_assemblage_ptr;
	struct gas_phase *gas_phase_ptr;
	struct kinetics *kinetics_ptr;
	struct s_s_assemblage *s_s_assemblage_ptr;
	int j, k, n;
	/*
	 *   Pure phase assemblage
	 */
	pp_assemblage_ptr = pp_assemblage_bsearch (i, &n);
	if (pp_assemblage_ptr != NULL) {
		for (j = 0; j < pp_assemblage_ptr->count_comps; j++) {
			pp_assemblage_ptr->pure_phases[j].initial_moles = pp_assemblage_ptr->pure_phases[j].moles;
			if (pp_assemblage_ptr->pure_phases[j].initial_moles < 0) pp_assemblage_ptr->pure_phases[j].initial_moles = 0;
		}
	}
	/*
	 *   Gas phase
	 */
	gas_phase_ptr = gas_phase_bsearch (i, &n);
	if (gas_phase_ptr != NULL) {
		for (j = 0; j < gas_phase_ptr->count_comps; j++) {
			gas_phase_ptr->comps[j].initial_moles = gas_phase_ptr->comps[j].moles;
		}
	}
	/*
	 *   Kinetics
	 */
	kinetics_ptr = kinetics_bsearch (i, &n);
	if (kinetics_ptr != NULL) {
		for (j = 0; j < kinetics_ptr->count_comps; j++) {
			kinetics_ptr->comps[j].initial_moles = kinetics_ptr->comps[j].m;
		}
	}
	/*
	 *   Solid solutions
	 */
	s_s_assemblage_ptr = s_s_assemblage_bsearch (i, &n);
	if (s_s_assemblage_ptr != NULL) {
		for (k = 0; k < s_s_assemblage_ptr->count_s_s; k++) {
			for (j = 0; j < s_s_assemblage_ptr->s_s[k].count_comps; j++) {
				s_s_assemblage_ptr->s_s[k].comps[j].init_moles = s_s_assemblage_ptr->s_s[k].comps[j].moles;
			}
		}
	}
	return(OK);
}
