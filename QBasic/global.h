#ifndef _INC_GLOBAL_H
#define _INC_GLOBAL_H

 PHRQ_io *phrq_io;

/* ----------------------------------------------------------------------
 *   STRUCTURES
 * ---------------------------------------------------------------------- */

 struct model last_model;
 int same_model;
 int same_temperature;
 struct punch punch;
/* ----------------------------------------------------------------------
 *   Temperatures
 * ---------------------------------------------------------------------- */

 struct temperature *temperature;
 int count_temperature;
/* ----------------------------------------------------------------------
 *   Surface
 * --------------------------------------------------------------------- */

 int g_iterations;
 LDBLE G_TOL;
 struct surface *surface;
 struct surface *dbg_surface;
 int count_surface;
 int max_surface;
 struct Charge_Group *charge_group;
 int change_surf_count;
 struct Change_Surf *change_surf;

/* ----------------------------------------------------------------------
 *   Exchange
 * ---------------------------------------------------------------------- */

 struct exchange *exchange;
 struct exchange *dbg_exchange;
 int count_exchange;
 int max_exchange;
/* ----------------------------------------------------------------------
 *   Kinetics
 * ---------------------------------------------------------------------- */

 struct kinetics *kinetics;
 struct kinetics *dbg_kinetics;
 int count_kinetics;
 int max_kinetics;

 int count_save_values;
 struct save_values *save_values;

/*----------------------------------------------------------------------
 *   Save
 *---------------------------------------------------------------------- */

 struct save save;
/*----------------------------------------------------------------------
 *   Use
 *---------------------------------------------------------------------- */

 struct Use use;
 struct Use *dbg_use;
/*----------------------------------------------------------------------
 *   Copy
 *---------------------------------------------------------------------- */

 struct copier copy_solution;
 struct copier copy_pp_assemblage;
 struct copier copy_exchange;
 struct copier copy_surface;
 struct copier copy_s_s_assemblage;
 struct copier copy_gas_phase;
 struct copier copy_kinetics;
 struct copier copy_mix;
 struct copier copy_irrev;
 struct copier copy_temperature;


/*----------------------------------------------------------------------
 *   Inverse
 *---------------------------------------------------------------------- */

 struct inverse *inverse;
 int count_inverse;

/*----------------------------------------------------------------------
 *   Mix
 *---------------------------------------------------------------------- */

 struct mix *mix;
 struct mix *dbg_mix;
 int count_mix;
/*----------------------------------------------------------------------
 *   Irreversible reaction
 *---------------------------------------------------------------------- */

 struct irrev *irrev;
 struct irrev *dbg_irrev;
 int count_irrev;
/*----------------------------------------------------------------------
 *   Gas phase
 *---------------------------------------------------------------------- */

 int count_gas_phase;
 int max_gas_phase;
 struct gas_phase *gas_phase;
/*----------------------------------------------------------------------
 *   Solid solution
 *---------------------------------------------------------------------- */

 int count_s_s_assemblage;
 int max_s_s_assemblage;
 struct s_s_assemblage *s_s_assemblage;
/*----------------------------------------------------------------------
 *   Pure-phase assemblage
 *---------------------------------------------------------------------- */

 int count_pp_assemblage;
 int max_pp_assemblage;
 struct pp_assemblage *pp_assemblage;
 struct pp_assemblage *dbg_pp_assemblage;
/*----------------------------------------------------------------------
 *   Species_list
 *---------------------------------------------------------------------- */

 int count_species_list;
 int max_species_list;
 struct species_list *species_list;
/*----------------------------------------------------------------------
 *   Jacobian and Mass balance lists
 *---------------------------------------------------------------------- */

 int count_sum_jacob0;	/* number of elements in sum_jacob0 */
 int max_sum_jacob0;	/* calculated maximum number of elements in sum_jacob0 */
 struct list0 *sum_jacob0;	/* array of pointers to targets and coefficients for array */

 int count_sum_mb1;		/* number of elements in sum_mb1 */
 int max_sum_mb1;		/* calculated maximum number of elements in sum_mb1 */
 struct list1 *sum_mb1;	/* array of pointers to sources and targets for mass
								   balance summations with coef = 1.0 */
 int count_sum_jacob1;	/* number of elements in sum_jacob1 */
 int max_sum_jacob1;	/* calculated maximum number of elements in sum_jacob1 */
 struct list1 *sum_jacob1;	/* array of pointers to sources and targets for array
									   equations with coef = 1.0 */
 int count_sum_mb2;		/* number of elements in sum_mb2 */
 int max_sum_mb2;		/* calculated maximum number of elements in sum_mb2 */
 struct list2 *sum_mb2;	/* array of coefficients and pointers to sources and
								   targets for mass balance summations with coef != 1.0 */
 int count_sum_jacob2;	/* number of elements in sum_jacob2 */
 int max_sum_jacob2;	/* calculated maximum number of elements in sum_jacob2 */
 struct list2 *sum_jacob2;	/* array of coefficients and pointers to sources and
									   targets, coef != 1.0 */
 int count_sum_delta;	/* number of elements in sum_delta */
 int max_sum_delta;		/* calculated maximum number of elements in sum_delta */
 struct list2 *sum_delta;	/* array of pointers to sources, targets and coefficients for
									   summing deltas for mass balance equations */
/*----------------------------------------------------------------------
 *   Solution
 *---------------------------------------------------------------------- */

 struct solution **solution;
 struct solution **dbg_solution;
 int count_solution;
 int max_solution;

struct iso *iso_defaults;
int count_iso_defaults;

/*----------------------------------------------------------------------
 *   Global solution
 *---------------------------------------------------------------------- */
 char *title_x;
 int new_x;
 char *description_x;
 LDBLE tc_x;
 LDBLE tk_x;
 LDBLE ph_x;
 LDBLE solution_pe_x;
 LDBLE mu_x;
 LDBLE ah2o_x;
 LDBLE density_x;
 LDBLE total_h_x;
 LDBLE total_o_x;
 LDBLE cb_x;
 LDBLE total_ions_x;
 LDBLE mass_water_aq_x;
 LDBLE mass_water_surfaces_x;
 LDBLE mass_water_bulk_x;
 char *units_x;
 struct pe_data *pe_x;
 int count_isotopes_x;
 struct isotope *isotopes_x;
 int default_pe_x;
/* int diffuse_layer_x;*/
 enum DIFFUSE_LAYER_TYPE dl_type_x;
 LDBLE total_carbon;
 LDBLE total_co2;
 LDBLE total_alkalinity;
 LDBLE gfw_water;
 LDBLE step_x;
 LDBLE kin_time_x;
/*----------------------------------------------------------------------
 *   Transport data
 *---------------------------------------------------------------------- */
 int count_cells;
 int count_shifts;
 int ishift;
 int bcon_first;
 int bcon_last;
 int correct_disp;
 LDBLE tempr;
 LDBLE timest;
 int simul_tr;
 LDBLE diffc;
 LDBLE heat_diffc;
 int cell;
 LDBLE mcd_substeps;
 struct stag_data *stag_data;
 int print_modulus;
 int punch_modulus;
 int dump_in;
 int dump_modulus;
 int transport_warnings;
 struct cell_data *cell_data;
 int multi_Dflag;		/* signals calc'n of multicomponent diffusion */
 int interlayer_Dflag;	/* multicomponent diffusion and diffusion through interlayer porosity */
 LDBLE default_Dw;		/* default species diffusion coefficient in water at 25oC, m2/s */
 LDBLE multi_Dpor;		/* uniform porosity of free porewater in solid medium */
 LDBLE interlayer_Dpor;	/* uniform porosity of interlayer space of montmorillonite in solid medium */
 LDBLE multi_Dpor_lim;	/* limiting free porewater porosity where transport stops */
 LDBLE interlayer_Dpor_lim;	/* limiting interlayer porosity where transport stops */
 LDBLE multi_Dn;		/* exponent to calculate pore water diffusion coefficient,
								   Dp = Dw * (multi_Dpor)^multi_Dn */
 LDBLE interlayer_tortf;	/* tortuosity_factor in interlayer porosity,
									   Dpil = Dw / interlayer_tortf */

 int cell_no;
/*----------------------------------------------------------------------
 *   Advection data
 *---------------------------------------------------------------------- */
 int count_ad_cells;
 int count_ad_shifts;
 int print_ad_modulus;
 int punch_ad_modulus;
 int *advection_punch, *advection_print;
 LDBLE advection_kin_time;
 LDBLE advection_kin_time_defined;
 int advection_warnings;

/*----------------------------------------------------------------------
 *   Keywords
 *---------------------------------------------------------------------- */
struct const_key *keyword;
int NKEYS;

 struct key *keyword_hash;
 int new_model, new_exchange, new_pp_assemblage, new_surface,
	new_reaction, new_temperature, new_mix, new_solution, new_gas_phase,
	new_inverse, new_punch, new_s_s_assemblage, new_kinetics, new_copy,
	new_pitzer;
/*----------------------------------------------------------------------
 *   Elements
 *---------------------------------------------------------------------- */

 struct element **elements;
 int count_elements;
 int max_elements;
 struct element *element_h_one;

/*----------------------------------------------------------------------
 *   Element List
 *---------------------------------------------------------------------- */

 struct elt_list *elt_list;	/* structure array of working space while reading equations
									   names are in "strings", initially in input order */
 int count_elts;		/* number of elements in elt_list = position of next */
 int max_elts;
/*----------------------------------------------------------------------
 *   Reaction
 *---------------------------------------------------------------------- */

/*----------------------------------------------------------------------
 *   Species
 *---------------------------------------------------------------------- */

 struct logk **logk;
 int count_logk;
 int max_logk;

 char *moles_per_kilogram_string;
 char *pe_string;

 struct species **s;
 int count_s;
 int max_s;

 struct species **s_x;
 int count_s_x;
 int max_s_x;

 struct species *s_h2o;
 struct species *s_hplus;
 struct species *s_h3oplus;
 struct species *s_eminus;
 struct species *s_co3;
 struct species *s_h2;
 struct species *s_o2;
/*----------------------------------------------------------------------
 *   Phases
 *---------------------------------------------------------------------- */

 struct phase **phases;
 int count_phases;
 int max_phases;
/*----------------------------------------------------------------------
 *   Master species
 *---------------------------------------------------------------------- */

 struct master **master;	/* structure array of master species */
 struct master **dbg_master;
 int count_master;
 int max_master;
/*----------------------------------------------------------------------
 *   Unknowns
 *---------------------------------------------------------------------- */

 struct unknown **x;
 int count_unknowns;
 int max_unknowns;

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
 struct unknown *s_s_unknown;
/*----------------------------------------------------------------------
 *   Reaction work space
 *---------------------------------------------------------------------- */

 struct reaction_temp trxn;	/* structure array of working space while reading equations
									   species names are in "temp_strings" */
 int count_trxn;		/* number of reactants in trxn = position of next */
 int max_trxn;

 struct unknown_list *mb_unknowns;
 int count_mb_unknowns;
 int max_mb_unknowns;
/* ----------------------------------------------------------------------
 *   Print
 * ---------------------------------------------------------------------- */

 struct prints pr;
 int status_on, status_interval;
 float status_timer;
 int count_warnings;

/* ----------------------------------------------------------------------
 *   RATES
 * ---------------------------------------------------------------------- */

 struct rate *rates;
 int count_rates;
 LDBLE rate_m, rate_m0, *rate_p, rate_time, rate_sim_time_start,
	rate_sim_time_end, rate_sim_time, rate_moles, initial_total_time;
 int count_rate_p;
/* ----------------------------------------------------------------------
 *   USER PRINT COMMANDS
 * ---------------------------------------------------------------------- */
 struct rate *user_print;
 struct rate *user_punch;
 char **user_punch_headings;
 int user_punch_count_headings;
#if defined PHREEQ98 
 struct rate *user_graph;
 char **user_graph_headings;
 int user_graph_count_headings;
#endif
#if defined MULTICHART
 ChartHandler chart_handler;
#endif

/* ----------------------------------------------------------------------
 *   GLOBAL DECLARATIONS
 * ---------------------------------------------------------------------- */
 char error_string[10 * MAX_LENGTH];
 int simulation;
 int state;
 int reaction_step;
 int transport_step;
 int transport_start;
 int advection_step;
 int stop_program;
 int incremental_reactions;

 int count_strings;
 int max_strings;

 LDBLE *array;
 LDBLE *delta;
 LDBLE *residual;

 int input_error;

 int next_keyword;
 int parse_error;
 int paren_count;
 int iterations;
 int gamma_iterations;
 int run_reactions_iterations;

 int max_line;
 char *line;
 char *line_save;

 LDBLE LOG_10;

 int debug_model;
 int debug_prep;
 int debug_set;
 int debug_diffuse_layer;
 int debug_inverse;

 LDBLE inv_tol_default;
 int itmax;
 LDBLE ineq_tol;
 LDBLE convergence_tolerance;
 LDBLE step_size;
 LDBLE pe_step_size;
 LDBLE step_size_now;
 LDBLE pe_step_size_now;
 LDBLE pp_scale;
 LDBLE pp_column_scale;
 int diagonal_scale;	/* 0 not used, 1 used */
 int mass_water_switch;
 int delay_mass_water;
 LDBLE censor;
 int aqueous_only;
 int negative_concentrations;
 int calculating_deriv;
 int numerical_deriv;

 int count_total_steps;
 int phast;
 LDBLE *llnl_temp, *llnl_adh, *llnl_bdh, *llnl_bdot, *llnl_co2_coefs;
 int llnl_count_temp, llnl_count_adh, llnl_count_bdh, llnl_count_bdot,
	llnl_count_co2_coefs;

 char *selected_output_file_name;
 char *dump_file_name;
 int remove_unstable_phases;

#ifdef PHREEQCI_GUI
 struct spread_sheet g_spread_sheet;
#endif

/* ---------------------------------------------------------------------- */
/*
 *   Hash definitions
 */

 HashTable *strings_hash_table;
 HashTable *elements_hash_table;
 HashTable *species_hash_table;
 HashTable *phases_hash_table;
 HashTable *keyword_hash_table;
 HashTable *logk_hash_table;
 HashTable *master_isotope_hash_table;

#if defined(PHREEQCI_GUI)
#include "../../phreeqci_gui.h"
#endif /* defined(PHREEQCI_GUI) */
/* ----------------------------------------------------------------------
 *   ISOTOPES
 * ---------------------------------------------------------------------- */
 struct name_coef match_tokens[50];
 int count_match_tokens;

 int count_master_isotope;
 struct master_isotope **master_isotope;
 int max_master_isotope;
 int initial_solution_isotopes;

#define OPTION_EOF -1
#define OPTION_KEYWORD -2
#define OPTION_ERROR -3
#define OPTION_DEFAULT -4
#define OPT_1 -5


 int count_calculate_value;
 struct calculate_value **calculate_value;
 int max_calculate_value;
 HashTable *calculate_value_hash_table;


 int count_isotope_ratio;
 struct isotope_ratio **isotope_ratio;
 int max_isotope_ratio;
 HashTable *isotope_ratio_hash_table;

 int count_isotope_alpha;
 struct isotope_alpha **isotope_alpha;
 int max_isotope_alpha;
 HashTable *isotope_alpha_hash_table;

 int phreeqc_mpi_myself;

 int first_read_input;
 char *user_database;
 int pitzer_model, sit_model, pitzer_pe;
 int full_pitzer, always_full_pitzer, ICON, IC;
 LDBLE COSMOT;
 LDBLE AW;
 int have_punch_name;
/* VP: Density Start */
 int print_density;
/* VP: Density End */

 jmp_buf mark;
 LDBLE *zeros;
 int zeros_max;
#if defined(WIN32)
#include <windows.h>
#endif

#if defined(WIN32_MEMORY_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

 LDBLE cell_pore_volume;
 LDBLE cell_porosity;
 LDBLE cell_volume;
 LDBLE cell_saturation;

 struct system_species *sys;
 int count_sys, max_sys;

 LDBLE sys_tot;
 LDBLE AA_basic, BB_basic, CC, I_m, rho_0;
 LDBLE solution_mass, solution_volume;
 LDBLE f_rho(LDBLE rho_old);

/* phqalloc.c ------------------------------- */

 PHRQMemHeader *s_pTail;
#include <sstream>
	std::stringstream merged_database_stream;
	std::stringstream merged_input_stream;
/* Collect all statics for PHREEQC_CLASS */

/* basic.c ------------------------------- */

#ifdef PHREEQ98
int colnr, rownr;
#endif

int n_user_punch_index;
//Char *inbuf;
//linerec *linebase;
//varrec *varbase;
//looprec *loopbase;
//long curline;
//linerec *stmtline, *dataline;
//tokenrec *stmttok, *datatok, *buf;
//boolean exitflag;
//long EXCP_LINE;
//HashTable *command_hash_table;
//struct const_key *command;
//int NCMDS;

/* cl1.c ------------------------------- */

LDBLE *x_arg, *res_arg, *scratch;
int x_arg_max, res_arg_max, scratch_max;

/* dw.c ------------------------------- */

/* COMMON /QQQQ/ */
LDBLE Q0, Q5;
LDBLE GASCON, TZ, AA;
LDBLE Z, DZ, Y;
LDBLE G1, G2, GF;
LDBLE B1, B2, B1T, B2T, B1TT, B2TT;

/* input.cpp ------------------------------- */

int check_line_return;  

/* integrate.cpp ------------------------------- */

LDBLE midpoint_sv;
LDBLE z, xd, alpha;
struct surface_charge *surface_charge_ptr;
int max_row_count, max_column_count;
int carbon;
char **col_name, **row_name;
int count_rows, count_optimize;
int col_phases, col_redox, col_epsilon, col_ph, col_water,
	col_isotopes, col_phase_isotopes;
int row_mb, row_fract, row_charge, row_carbon, row_isotopes,
	row_epsilon, row_isotope_epsilon, row_water;
LDBLE *inv_zero, *array1, *res, *inv_delta1, *delta2, *delta3, *inv_cu,
	*delta_save;
LDBLE *min_delta, *max_delta;
int *iu, *is;
int klmd, nklmd, n2d, kode, iter;
LDBLE toler, error, max_pct, scaled_error;
struct master *master_alk;
int *row_back, *col_back;
unsigned long *good, *bad, *minimal;
int max_good, max_bad, max_minimal;
int count_good, count_bad, count_minimal, count_calls;
unsigned long soln_bits, phase_bits, current_bits, temp_bits;

/* inverse.c ------------------------------- */

FILE *netpath_file;
int count_inverse_models, count_pat_solutions;
int min_position[32], max_position[32], now[32];

/* kinetics.c ------------------------------- */

public:
	void *cvode_kinetics_ptr;
	int cvode_test;
	int cvode_error;
	int cvode_n_user;
	int cvode_n_reactions;
	realtype cvode_step_fraction;
	realtype cvode_rate_sim_time;
	realtype cvode_rate_sim_time_start;
	realtype cvode_last_good_time;
	realtype cvode_prev_good_time;
	N_Vector cvode_last_good_y;
	N_Vector cvode_prev_good_y;
	M_Env kinetics_machEnv;
	N_Vector kinetics_y, kinetics_abstol;
	void *kinetics_cvode_mem;
	struct pp_assemblage *cvode_pp_assemblage_save;
	struct s_s_assemblage *cvode_s_s_assemblage_save;
	LDBLE *m_original;
	LDBLE *m_temp;

/* model.c ------------------------------- */

LDBLE min_value;
/* LDBLE model_min_value; */
LDBLE *normal, *ineq_array, *inv_res, *cu, *zero, *delta1;
int *inv_iu, *inv_is, *back_eq;
int normal_max, ineq_array_max, res_max, cu_max, zero_max,
	delta1_max, iu_max, is_max, back_eq_max;

/* phrq_io_output.c ------------------------------- */
int forward_output_to_log;

/* phreeqc_files.c ------------------------------- */

char *default_data_base;
#ifdef PHREEQ98
int outputlinenr;
char *LogFileNameC;
char progress_str[512];
#endif
//Anyptr __MallocTemp__;
//int P_argc;
//char **P_argv;
//int P_escapecode;
//int P_ioresult;
//__p2c_jmp_buf *__top_jb;

/* pitzer.c ------------------------------- */

LDBLE A0;
struct species **spec, **cations, **anions, **neutrals;
int count_cations, count_anions, count_neutrals;
int MAXCATIONS, FIRSTANION, MAXNEUTRAL;
struct pitz_param *mcb0, *mcb1, *mcc0;
int *IPRSNT;
LDBLE *M, *LGAMMA;
LDBLE BK[23], DK[23];
#ifdef PHREEQ98
int connect_simulations, graph_initial_solutions;
int shifts_as_points;
int chart_type;
int ShowChart;
int RowOffset, ColumnOffset;
#endif
LDBLE dummy;

/* print.c ------------------------------- */

#ifdef PHREEQ98
int colnr, rownr;
int graph_initial_solutions;
int prev_advection_step, prev_transport_step;	/*, prev_reaction_step */
/* int shifts_as_points; */
int chart_type;
int AddSeries;
int FirstCallToUSER_GRAPH;
#endif
/* read.c */
char *prev_next_char;
#if defined PHREEQ98 
int shifts_as_points;
#endif

/* read_class.cxx */
dumper dump_info;
StorageBinList delete_info;
runner run_info;
char * sformatf_buffer;
size_t sformatf_buffer_size;

/* readtr.c */

std::string dump_file_name_cpp;

/* sit.c ------------------------------- */

LDBLE sit_A0;
int sit_count_cations, sit_count_anions, sit_count_neutrals;
int sit_MAXCATIONS, sit_FIRSTANION, sit_MAXNEUTRAL;
int *sit_IPRSNT;
LDBLE *sit_M, *sit_LGAMMA;

/* tidy.c ------------------------------- */

LDBLE a0, a1, kc, kb;

/* tally.c ------------------------------- */


struct tally_buffer *t_buffer;
int tally_count_component;

struct tally *tally_table;
int count_tally_table_columns;
int count_tally_table_rows;

/* transport.c ------------------------------- */


struct sol_D *sol_D;
struct sol_D *sol_D_dbg;
struct J_ij *J_ij, *J_ij_il;
int J_ij_count_spec;

struct M_S *m_s;
int count_m_s;
LDBLE tot1_h, tot1_o, tot2_h, tot2_o;
LDBLE diffc_max, diffc_tr, J_ij_sum;
int transp_surf;
LDBLE *heat_mix_array;
LDBLE *temp1, *temp2;
int nmix, heat_nmix;
LDBLE heat_mix_f_imm, heat_mix_f_m;
int warn_MCD_X, warn_fixed_Surf;

/* utilities.c ------------------------------- */

#ifdef PHREEQ98
int AutoLoadOutputFile, CreateToC;
int ProcessMessages, ShowProgress, ShowProgressWindow, ShowChart;
int outputlinenr;
int stop_calculations;
char err_str98[80];
#endif


#endif /* _INC_GLOBAL_H  */

/*********************************
    isfinite handling
    (Note: Should NOT be guarded)
**********************************/

#if defined (PHREEQ98) || defined (_MSC_VER)
#  define HAVE_FINITE
#  define finite _finite
#else  /*defined (PHREEQ98) || defined (_MSC_VER)*/
#  if defined(DJGPP)
#    define HAVE_FINITE
#  endif
#endif /*defined (PHREEQ98) || defined (_MSC_VER)*/

#if defined(HAVE_ISFINITE)
#  define PHR_ISFINITE(x) isfinite(x)
#elif defined(HAVE_FINITE)
#  define PHR_ISFINITE(x) finite(x)
#elif defined(HAVE_ISNAN)
#  define PHR_ISFINITE(x) ( ((x) == 0.0) || ((!isnan(x)) && ((x) != (2.0 * (x)))) )
#else
#  define PHR_ISFINITE(x) ( ((x) == 0.0) || (((x) == (x)) && ((x) != (2.0 * (x)))) )
#endif
