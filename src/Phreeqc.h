#ifndef _INC_PHREEQC_H
#define _INC_PHREEQC_H
#if defined(WIN32)
#include <windows.h>
#endif
/* ----------------------------------------------------------------------
 *   INCLUDE FILES
 * ---------------------------------------------------------------------- */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>
#include <float.h>
#include <setjmp.h>
#include "phrqtype.h"
#include "sundialstypes.h"
#include "nvector.h"
#include "cvdense.h"	
#include "nvector_serial.h"	/* definitions of type N_Vector and macro         */
							/* NV_Ith_S, prototypes for N_VNew, N_VFree       */
#include "dense.h"			/* definitions of type DenseMat, macro DENSE_ELEM */
#include "nvector.h"
#include "runner.h"
#include "dumper.h"
#include "StorageBinList.h"

#define STATIC
#define EXTERNAL
#define CLASS_QUALIFIER Phreeqc::
#define CLASS_STATIC static
#define extern
#include "p2c.h"
#undef extern 
#include "global_structures.h"
#include "pitzer_structures.h"
class Phreeqc
{
public:
	Phreeqc(void);
	~Phreeqc(void);


//private:
//
//struct _generic_N_Vector;
//struct calculate_value;
//struct conc;
//struct CVodeMemRec;
//struct element;
//struct exchange;
//struct exch_comp;
//struct elt_list;
//struct gas_phase;
//struct gas_comp;
//struct inverse;
//struct inv_elts;
//struct inv_phases;
//struct inv_isotope;
//struct irrev;
//struct isotope;
//struct kinetics;
//struct kinetics_comp;
//struct LOC_exec;
//struct master;
//struct master_activity;
//struct master_isotope;
//struct mix;
//struct mix_comp;
//struct name_coef;
//struct pe_data;
//struct phase;
//struct PHRQMemHeader;
//struct pitz_param;
//struct pp_assemblage;
//struct pure_phase;
//struct reaction;
//struct reaction_temp;
//struct rxn_token;
//struct rxn_token_temp;
//struct solution;
//struct species;
//struct s_s;
//struct s_s_assemblage;
//struct s_s_comp;
//struct species_diff_layer;
//struct surface;
//struct surface_comp;
//struct surface_charge;
//struct surface_diff_layer;
//struct theta_param;
//struct tokenrec;
//struct varrec;
//struct unknown;

#define PITZER_EXTERNAL 
#include "pitzer.h"
#include "input.h"
#define KINETICS_EXTERNAL 
#include "sundialstypes.h"		/* definitions of types realtype and              */
							    /* integertype, and the constant FALSE            */
public:
#include "output.h"
public:
#include "global.h"
/*
  All functions are included as methods here
*/
#define _INC_PHRQPROTO_H

// advection.c -------------------------------

int advection(void);

// basic.c -------------------------------

int basic_main(char *commands);
void cmd_initialize(void);
void cmd_free(void);
int basic_compile(char *commands, void **lnbase, void **vbase, void **lpbase);
int basic_run(char *commands, void *lnbase, void *vbase, void *lpbase);
int basic_init(void);
#ifdef PHREEQ98
void GridChar(char *s, char *a);
#endif
int sget_logical_line(char **ptr, int *l, char *return_line);
int free_dim_stringvar(varrec *varbase);
void exec(void);
int basic_renumber(char *commands, void **lnbase, void **vbase, void **lpbase);
void restoredata(void);
void clearloops(void);
void clearvar(varrec * v);
void clearvars(void);
Char * numtostr(Char * Result, LDBLE n);
void parse(Char * inbuf, tokenrec ** buf);
void listtokens(FILE * f, tokenrec * buf);
void disposetokens(tokenrec ** tok);
void parseinput(tokenrec ** buf);
void errormsg(const Char * s);
void snerr(const Char * s);
void tmerr(const Char * s);
void badsubscr(void);
LDBLE realfactor(struct LOC_exec *LINK);
Char * strfactor(struct LOC_exec * LINK);
Char *stringfactor(Char * Result, struct LOC_exec *LINK);
long intfactor(struct LOC_exec *LINK);
LDBLE realexpr(struct LOC_exec *LINK);
Char * strexpr(struct LOC_exec * LINK);
Char * stringexpr(Char * Result, struct LOC_exec * LINK);
long intexpr(struct LOC_exec *LINK);
void require(int k, struct LOC_exec *LINK);
void skipparen(struct LOC_exec *LINK);
varrec * findvar(struct LOC_exec *LINK);
valrec factor(struct LOC_exec *LINK);
valrec upexpr(struct LOC_exec * LINK);
valrec term(struct LOC_exec * LINK);
valrec sexpr(struct LOC_exec * LINK);
valrec relexpr(struct LOC_exec * LINK);
valrec andexpr(struct LOC_exec * LINK);
valrec expr(struct LOC_exec *LINK);
void checkextra(struct LOC_exec *LINK);
boolean iseos(struct LOC_exec *LINK);
void skiptoeos(struct LOC_exec *LINK);
linerec * findline(long n);
linerec * mustfindline(long n);
void cmdend(struct LOC_exec *LINK);
void cmdnew(struct LOC_exec *LINK);
void cmdlist(struct LOC_exec *LINK);
void cmdload(boolean merging, Char * name, struct LOC_exec *LINK);
void cmdrun(struct LOC_exec *LINK);
void cmdsave(struct LOC_exec *LINK);
void cmdput(struct LOC_exec *LINK);
void cmdchange_por(struct LOC_exec *LINK);
void cmdchange_surf(struct LOC_exec *LINK);
void cmdbye(void);
void cmddel(struct LOC_exec *LINK);
void cmdrenum(struct LOC_exec *LINK);
void cmdprint(struct LOC_exec *LINK);
void cmdpunch(struct LOC_exec *LINK);
#ifdef PHREEQ98
void cmdgraph_x(struct LOC_exec *LINK);
void cmdgraph_y(struct LOC_exec *LINK);
void cmdgraph_sy(struct LOC_exec *LINK);
#endif
void cmdlet(boolean implied, struct LOC_exec *LINK);
void cmdgoto(struct LOC_exec *LINK);
void cmdif(struct LOC_exec *LINK);
void cmdelse(struct LOC_exec *LINK);
boolean skiploop(int up, int dn, struct LOC_exec *LINK);
void cmdfor(struct LOC_exec *LINK);
void cmdnext(struct LOC_exec *LINK);
void cmdwhile(struct LOC_exec *LINK);
void cmdwend(struct LOC_exec *LINK);
void cmdgosub(struct LOC_exec *LINK);
void cmdreturn(struct LOC_exec *LINK);
void cmdread(struct LOC_exec *LINK);
void cmddata(struct LOC_exec *LINK);
void cmdrestore(struct LOC_exec *LINK);
void cmdgotoxy(struct LOC_exec *LINK);
void cmdon(struct LOC_exec *LINK);
void cmddim(struct LOC_exec *LINK);
void cmdpoke(struct LOC_exec *LINK);

// basicsubs.c -------------------------------

LDBLE activity(const char *species_name);
LDBLE activity_coefficient(const char *species_name);
LDBLE log_activity_coefficient(const char *species_name);
LDBLE calc_SC(void);
/* VP: Density Start */
LDBLE calc_dens(void);
/* VP: Density End */
LDBLE calc_logk_n(const char *name);
LDBLE calc_logk_p(const char *name);
LDBLE calc_logk_s(const char *name);
LDBLE calc_surface_charge(const char *surface_name);
LDBLE diff_layer_total(const char *total_name, const char *surface_name);
LDBLE equi_phase(const char *phase_name);
LDBLE find_gas_comp(const char *gas_comp_name);
LDBLE find_misc1(const char *s_s_name);
LDBLE find_misc2(const char *s_s_name);
LDBLE find_s_s_comp(const char *s_s_comp_name);
LDBLE get_calculate_value(const char *name);
char * iso_unit(const char *total_name);
LDBLE iso_value(const char *total_name);
LDBLE kinetics_moles(const char *kinetics_name);
LDBLE log_activity(const char *species_name);
LDBLE log_molality(const char *species_name);
LDBLE molality(const char *species_name);
LDBLE saturation_ratio(const char *phase_name);
int saturation_index(const char *phase_name, LDBLE * iap, LDBLE * si);
LDBLE solution_sum_secondary(const char *total_name);
LDBLE sum_match_gases(const char *stemplate, const char *name);
LDBLE sum_match_species(const char *stemplate, const char *name);
LDBLE sum_match_s_s(const char *stemplate, const char *name);
int match_elts_in_species(const char *name, const char *stemplate);
int extract_bracket(char **string, char *bracket_string);
LDBLE surf_total(const char *total_name, const char *surface_name);
CLASS_STATIC int system_species_compare(const void *ptr1, const void *ptr2);
LDBLE system_total(const char *total_name, LDBLE * count, char ***names,
				   char ***types, LDBLE ** moles);
int system_total_elements(void);
int system_total_si(void);
int system_total_aq(void);
int system_total_ex(void);
int system_total_surf(void);
int system_total_gas(void);
int system_total_s_s(void);
int system_total_elt(const char *total_name);
int system_total_elt_secondary(const char *total_name);
LDBLE total(const char *total_name);
LDBLE total_mole(const char *total_name);
int system_total_solids(struct exchange *exchange_ptr,
					struct pp_assemblage *pp_assemblage_ptr,
					struct gas_phase *gas_phase_ptr,
					struct s_s_assemblage *s_s_assemblage_ptr,
					struct surface *surface_ptr);

static LDBLE f_rho(LDBLE rho_old, void *cookie);

// cl1.c -------------------------------
int cl1(int k, int l, int m, int n,
		int nklmd, int n2d,
		LDBLE * q,
		int *kode, LDBLE toler,
		int *iter, LDBLE * x, LDBLE * res, LDBLE * error,
		LDBLE * cu, int *iu, int *s, int check);
void cl1_space(int check, int n2d, int klm, int nklmd);

// cl1mp.c -------------------------------
int cl1mp(int k, int l, int m, int n,
		  int nklmd, int n2d,
		  LDBLE * q_arg,
		  int *kode, LDBLE toler,
		  int *iter, LDBLE * x_arg, LDBLE * res_arg, LDBLE * error,
		  LDBLE * cu_arg, int *iu, int *s, int check, LDBLE censor_arg);

// class_main.c -------------------------------
#ifdef DOS
int write_banner(void);
#endif

/* default.c */
public:
int close_input_files(void);
int close_output_files(void);
CLASS_STATIC int getc_callback(void *cookie);
int process_file_names(int argc, char *argv[], void **db_cookie,
					   void **input_cookie, int log);

// dw.c -------------------------------
int BB(LDBLE T);
LDBLE PS(LDBLE T);
LDBLE VLEST(LDBLE T);
int DFIND(LDBLE * DOUT, LDBLE P, LDBLE D, LDBLE T);
int QQ(LDBLE T, LDBLE D);
LDBLE BASE(LDBLE D);

// input.c -------------------------------

int reading_database(void);
struct read_callback s_read_callback;
int check_line(const char *string, int allow_empty, int allow_eof,
		   int allow_keyword, int print);

// integrate.c -------------------------------

int calc_all_g(void);
int calc_init_g(void);
int initial_surface_water(void);
int sum_diffuse_layer(struct surface_charge *surface_charge_ptr1);
int calc_all_donnan(void);
int calc_init_donnan(void);
LDBLE g_function(LDBLE x_value);
LDBLE midpnt(LDBLE x1, LDBLE x2, int n);
void polint(LDBLE * xa, LDBLE * ya, int n, LDBLE xv, LDBLE * yv,
				   LDBLE * dy);
LDBLE qromb_midpnt(LDBLE x1, LDBLE x2);
LDBLE calc_psi_avg(LDBLE surf_chrg_eq);
int calc_all_donnan_music(void);
int calc_init_donnan_music(void);

// inverse.c -------------------------------

int inverse_models(void);
int add_to_file(const char *filename, char *string);
int bit_print(unsigned long bits, int l);
int carbon_derivs(struct inverse *inv_ptr);
int check_isotopes(struct inverse *inv_ptr);
int check_solns(struct inverse *inv_ptr);
int count_isotope_unknowns(struct inverse *inv_ptr,
								  struct isotope **isotope_unknowns);
struct isotope *get_isotope(struct solution *solution_ptr, const char *elt);
struct conc *get_inv_total(struct solution *solution_ptr, const char *elt);
int isotope_balance_equation(struct inverse *inv_ptr, int row, int n);
int post_mortem(void);
unsigned long get_bits(unsigned long bits, int position, int number);
unsigned long minimal_solve(struct inverse *inv_ptr,
								   unsigned long minimal_bits);
void dump_netpath(struct inverse *inv_ptr);
int dump_netpath_pat(struct inverse *inv_ptr);
int next_set_phases(struct inverse *inv_ptr, int first_of_model_size,
						   int model_size);
int phase_isotope_inequalities(struct inverse *inv_ptr);
int print_model(struct inverse *inv_ptr);
int punch_model_heading(struct inverse *inv_ptr);
int punch_model(struct inverse *inv_ptr);
void print_isotope(FILE * netpath_file, struct solution *solution_ptr,
				   const char *elt, const char *string);
void print_total(FILE * netpath_file, struct solution *solution_ptr,
				 const char *elt, const char *string);
void print_total_multi(FILE * netpath_file, struct solution *solution_ptr,
					   const char *string, const char *elt0,
					   const char *elt1, const char *elt2, const char *elt3,
					   const char *elt4);
void print_total_pat(FILE * netpath_file, const char *elt,
					 const char *string);
int range(struct inverse *inv_ptr, unsigned long cur_bits);
int save_bad(unsigned long bits);
int save_good(unsigned long bits);
int save_minimal(unsigned long bits);
unsigned long set_bit(unsigned long bits, int position, int value);
int setup_inverse(struct inverse *inv_ptr);
int set_initial_solution(int n_user_old, int n_user_new);
int set_ph_c(struct inverse *inv_ptr,
					int i,
					struct solution *soln_ptr_orig,
					int n_user_new,
					LDBLE d_alk, LDBLE ph_factor, LDBLE alk_factor);
int shrink(struct inverse *inv_ptr, LDBLE * array_in,
				  LDBLE * array_out, int *k, int *l, int *m, int *n,
				  unsigned long cur_bits, LDBLE * delta_l, int *col_back_l,
				  int *row_back_l);
int solve_inverse(struct inverse *inv_ptr);
int solve_with_mask(struct inverse *inv_ptr, unsigned long cur_bits);
int subset_bad(unsigned long bits);
int subset_minimal(unsigned long bits);
int superset_minimal(unsigned long bits);
int write_optimize_names(struct inverse *inv_ptr);

// isotopes.c -------------------------------

int add_isotopes(struct solution *solution_ptr);
int calculate_values(void);
int calculate_isotope_moles(struct element *elt_ptr,
							struct solution *solution_ptr, LDBLE total_moles);
LDBLE convert_isotope(struct master_isotope *master_isotope_ptr, LDBLE ratio);
int from_pcil(struct master_isotope *master_isotope_ptr);
int from_permil(struct master_isotope *master_isotope_ptr, LDBLE major_total);
int from_pct(struct master_isotope *master_isotope_ptr, LDBLE major_total);
int from_tu(struct master_isotope *master_isotope_ptr);
struct calculate_value *calculate_value_alloc(void);
int calculate_value_free(struct calculate_value *calculate_value_ptr);
struct calculate_value *calculate_value_search(const char *name);
struct calculate_value *calculate_value_store(const char *name,
											  int replace_if_found);
struct isotope_alpha *isotope_alpha_alloc(void);
struct isotope_alpha *isotope_alpha_search(const char *name);
struct isotope_alpha *isotope_alpha_store(const char *name,
										  int replace_if_found);
struct isotope_ratio *isotope_ratio_alloc(void);
struct isotope_ratio *isotope_ratio_search(const char *name);
struct isotope_ratio *isotope_ratio_store(const char *name,
										  int replace_if_found);
struct master_isotope *master_isotope_store(const char *name,
											int replace_if_found);
struct master_isotope *master_isotope_alloc(void);
struct master_isotope *master_isotope_search(const char *name);
int print_initial_solution_isotopes(void);
int print_isotope_ratios(void);
int print_isotope_alphas(void);
int punch_isotopes(void);
int punch_calculate_values(void);
int read_calculate_values(void);
int read_isotopes(void);
int read_isotope_ratios(void);
int read_isotope_alphas(void);
int calculate_value_init(struct calculate_value *calculate_value_ptr);
int isotope_alpha_init(struct isotope_alpha *isotope_alpha_ptr);
int isotope_ratio_init(struct isotope_ratio *isotope_ratio_ptr);
int master_isotope_init(struct master_isotope *master_isotope_ptr);

// kinetics.c -------------------------------

void cvode_init(void);
int run_reactions(int i, LDBLE kin_time, int use_mix, LDBLE step_fraction);
int set_and_run(int i, int use_mix, int use_kinetics, int nsaver,
				LDBLE step_fraction);
int set_and_run_wrapper(int i, int use_mix, int use_kinetics, int nsaver,
						LDBLE step_fraction);
int set_advection(int i, int use_mix, int use_kinetics, int nsaver);
int free_cvode(void);
public:
static void f(integertype N, realtype t, N_Vector y, N_Vector ydot,
			  void *f_data);
static void Jac(integertype N, DenseMat J, RhsFn f, void *f_data, realtype t,
				N_Vector y, N_Vector fy, N_Vector ewt, realtype h,
				realtype uround, void *jac_data, long int *nfePtr,
				N_Vector vtemp1, N_Vector vtemp2, N_Vector vtemp3);

int calc_final_kinetic_reaction(struct kinetics *kinetics_ptr);
int calc_kinetic_reaction(struct kinetics *kinetics_ptr,
								 LDBLE time_step);
int rk_kinetics(int i, LDBLE kin_time, int use_mix, int nsaver,
					   LDBLE step_fraction);
int set_reaction(int i, int use_mix, int use_kinetics);
int set_transport(int i, int use_mix, int use_kinetics, int nsaver);
int store_get_equi_reactants(int k, int kin_end);

// mainsubs  -------------------------------

FILE *file_open(char *query, char *default_name, const char *status,
				int batch);
int copy_entities(void);
void initialize(void);
int initial_exchangers(int print);
int initial_gas_phases(int print);
int initial_solutions(int print);
int step_save_exch(int n_user);
int step_save_surf(int n_user);
int initial_surfaces(int print);
int reactions(void);
int saver(void);
int xsolution_save(int k_user);
int xexchange_save(int n_user);
int xgas_save(int n_user);
int xpp_assemblage_save(int n_user);
int xs_s_assemblage_save(int n_user);
int xsurface_save(int n_user);
int do_initialize(void);
int do_status(void);

int copy_use(int i);
int set_use(void);

// model.c -------------------------------

int check_residuals(void);
int free_model_allocs(void);
int ineq(int kode);
int model(void);
int jacobian_sums(void);
int mb_gases(void);
int mb_s_s(void);
int mb_sums(void);
int molalities(int allow_overflow);
int reset(void);
int residuals(void);
int set(int initial);
int sum_species(void);
int surface_model(void);

LDBLE s_s_root(LDBLE a0, LDBLE a1, LDBLE kc, LDBLE kb, LDBLE xcaq,
					  LDBLE xbaq);
LDBLE s_s_halve(LDBLE a0, LDBLE a1, LDBLE x0, LDBLE x1, LDBLE kc,
					   LDBLE kb, LDBLE xcaq, LDBLE xbaq);
LDBLE s_s_f(LDBLE xb, LDBLE a0, LDBLE a1, LDBLE kc, LDBLE kb,
				   LDBLE xcaq, LDBLE xbaq);
int numerical_jacobian(void);
void set_inert_moles(void);
void unset_inert_moles(void);
#ifdef SLNQ
int add_trivial_eqns(int rows, int cols, LDBLE * matrix);
int slnq(int n, LDBLE * a, LDBLE * delta, int ncols, int print);
#endif
int calc_gas_pressures(void);
int calc_s_s_fractions(void);
int gammas(LDBLE mu);
int initial_guesses(void);
int revise_guesses(void);
int s_s_binary(struct s_s *s_s_ptr);
int s_s_ideal(struct s_s *s_s_ptr);
//int remove_unstable_phases;
int gas_in;
void ineq_init(int max_row_count, int max_column_count);

// output.c -------------------------------

int output_message(const int type, const char *err_str, const int stop,
			   const char *format, va_list args);

// parse.c -------------------------------

int check_eqn(int association);
int get_charge(char *charge, LDBLE * z);
int get_elt(char **t_ptr, char *element, int *i);
int get_elts_in_species(char **t_ptr, LDBLE coef);
int get_num(char **t_ptr, LDBLE * num);
int get_secondary_in_species(char **t_ptr, LDBLE coef);
int parse_eq(char *eqn, struct elt_list **elt_ptr, int association);

int get_coef(LDBLE * coef, char **eqnaddr);
int get_secondary(char **t_ptr, char *element, int *i);
int get_species(char **ptr);

// phqalloc.c -------------------------------
public:
#if !defined(NDEBUG)
void *PHRQ_malloc(size_t, const char *, int);
void *PHRQ_calloc(size_t, size_t, const char *, int);
void *PHRQ_realloc(void *, size_t, const char *, int);
#else
void *PHRQ_malloc(size_t);
void *PHRQ_calloc(size_t, size_t);
void *PHRQ_realloc(void *, size_t);
#endif
void PHRQ_free(void *ptr);

void PHRQ_free_all(void);
protected:

// phreeqc_files.c -------------------------------

#ifdef PHREEQ98
void check_line_breaks(char *s);
char *prefix_database_dir(char *s);
void show_progress(const int type, char *s);
#endif
public:
int fileop_handler(const int type, int (*PFN) (FILE *));
int open_handler(const int type, const char *file_name);
int output_handler(const int type, const char *err_str,
						  const int stop, void *cookie, const char *format,
						  va_list args);
static int rewind_wrapper(FILE * file_ptr);
Void PASCAL_MAIN(int argc, Char **argv);
long my_labs(long x);
Anyptr my_memmove(Anyptr d, Const Anyptr s, size_t n);
Anyptr my_memcpy(Anyptr d, Const Anyptr s, size_t n);
int my_memcmp(Const Anyptr s1, Const Anyptr s2, size_t n);
Anyptr my_memset(Anyptr d, int c, size_t n);
int my_toupper(int c);
int my_tolower(int c);
long ipow(long a, long b);
char * strsub(register char *ret, register char *s, register int pos,
	   register int len);
int strpos2(char *s, register char *pat, register int pos);
int strcicmp(register char *s1, register char *s2);
char * strltrim(register char *s);
char * strrtrim(register char *s);
void strmove(register int len, register char *s, register int spos,
		register char *d, register int dpos);
void strinsert(register char *src, register char *dst, register int pos);
int P_peek(FILE * f);
int P_eof(void);
int P_eoln(FILE * f);
Void P_readpaoc(FILE * f, char *s, int len);
Void P_readlnpaoc(FILE * f, char *s, int len);
long P_maxpos(FILE * f);
Char * P_trimname(register Char * fn, register int len);
long memavail(void);
long maxavail(void);
long * P_setunion(register long *d, register long *s1, register long *s2);
long * P_setint(register long *d, register long *s1, register long *s2);
long * P_setdiff(register long *d, register long *s1, register long *s2);
long * P_setxor(register long *d, register long *s1, register long *s2);
long * P_addset(register long *s, register unsigned val);
long * P_addsetr(register long *s, register unsigned v1, register unsigned v2);
long * P_remset(register long *s, register unsigned val);
int P_setequal(register long *s1, register long *s2);
int P_subset(register long *s1, register long *s2);
long * P_setcpy(register long *d, register long *s);
long * P_expset(register long *d, register long s);
long P_packset(register long *s);	
int _OutMem(void);
int _CaseCheck(void);
int _NilCheck(void);
static char * _ShowEscape(char *buf, int code, int ior, char *prefix);
int _Escape(int code);
int _EscIO(int code);

// pitzer.c -------------------------------

int gammas_pz(void);
int model_pz(void);
int pitzer(void);
int pitzer_clean_up(void);
int pitzer_init(void);
int pitzer_tidy(void);
int read_pitzer(void);
int set_pz(int initial);

int calc_pitz_param(struct pitz_param *pz_ptr, LDBLE TK, LDBLE TR);
int check_gammas_pz(void);
int ISPEC(char *name);
LDBLE G(LDBLE Y);
LDBLE GP(LDBLE Y);
int ETHETAS(LDBLE ZJ, LDBLE ZK, LDBLE I, LDBLE * etheta,
				   LDBLE * ethetap);
int BDK(LDBLE X);
int pitzer_initial_guesses(void);
int pitzer_revise_guesses(void);
int pitzer_remove_unstable_phases;
int PTEMP(LDBLE TK);
LDBLE JAY(LDBLE X);
LDBLE JPRIME(LDBLE Y);
int jacobian_pz(void);

// pitzer_structures.c -------------------------------

struct pitz_param *pitz_param_alloc(void);
int pitz_param_init(struct pitz_param *pitz_param_ptr);
struct pitz_param *pitz_param_duplicate(struct pitz_param *old_ptr);
int pitz_param_copy(struct pitz_param *old_ptr,
						   struct pitz_param *new_ptr);

// pitzer_structures.c -------------------------------

int add_potential_factor(void);
int add_cd_music_factors(int n);
int add_surface_charge_balance(void);
int add_cd_music_charge_balances(int i);
int build_gas_phase(void);
int build_jacobian_sums(int k);
int build_mb_sums(void);
int build_min_exch(void);
int build_model(void);
int build_pure_phases(void);
int build_s_s_assemblage(void);
int build_solution_phase_boundaries(void);
int build_species_list(int n);
int build_min_surface(void);
int change_hydrogen_in_elt_list(LDBLE charge);
int clear(void);
int convert_units(struct solution *solution_ptr);
struct unknown *find_surface_charge_unknown(char *str_ptr, int plane);
struct master **get_list_master_ptrs(char *ptr,
											struct master *master_ptr);
int inout(void);
int is_special(struct species *spec);
int mb_for_species_aq(int n);
int mb_for_species_ex(int n);
int mb_for_species_surf(int n);
int quick_setup(void);
int resetup_master(void);
int save_model(void);
int setup_exchange(void);
int setup_gas_phase(void);
int setup_master_rxn(struct master **master_ptr_list,
							struct reaction **pe_rxn);
int setup_pure_phases(void);
int setup_related_surface(void);
int setup_s_s_assemblage(void);
int setup_solution(void);
int setup_surface(void);
int setup_unknowns(void);
int store_dn(int k, LDBLE * source, int row, LDBLE coef_in,
					LDBLE * gamma_source);
int store_jacob(LDBLE * source, LDBLE * target, LDBLE coef);
int store_jacob0(int row, int column, LDBLE coef);
int store_mb(LDBLE * source, LDBLE * target, LDBLE coef);
int store_mb_unknowns(struct unknown *unknown_ptr, LDBLE * LDBLE_ptr,
							 LDBLE coef, LDBLE * gamma_ptr);
int store_sum_deltas(LDBLE * source, LDBLE * target, LDBLE coef);
int tidy_redox(void);
struct master **unknown_alloc_master(void);
int write_mb_eqn_x(void);
int write_mb_for_species_list(int n);
int write_mass_action_eqn_x(int stop);

/* prep.c */

int check_same_model(void);
int k_temp(LDBLE tc);
LDBLE k_calc(LDBLE * logk, LDBLE tempk);
int prep(void);
int reprep(void);
int rewrite_master_to_secondary(struct master *master_ptr1,
								struct master *master_ptr2);
int switch_bases(void);
int write_phase_sys_total(int n);

// print.c -------------------------------

int fpunchf(const char *name, const char *format, ...);
int fpunchf_user(int user_index, const char *format, ...);
int fpunchf_end_row(const char *format, ...);
char *sformatf(const char *format, ...);
int array_print(LDBLE * array_l, int row_count, int column_count,
				int max_column_count);
int print_all(void);
int print_exchange(void);
int print_gas_phase(void);
int print_master_reactions(void);
int print_reaction(struct reaction *rxn_ptr);
int print_species(void);
int print_surface(void);
int print_user_print(void);
int punch_all(void);

int print_alkalinity(void);
int print_diffuse_layer(struct surface_charge *surface_charge_ptr);
int print_eh(void);
int print_irrev(void);
int print_kinetics(void);
int print_mix(void);
int print_pp_assemblage(void);
int print_s_s_assemblage(void);
int print_saturation_indices(void);
int print_surface_cd_music(void);
int print_totals(void);
int print_using(void);
/*int print_user_print(void);*/
int punch_gas_phase(void);
int punch_identifiers(void);
int punch_kinetics(void);
int punch_molalities(void);
int punch_activities(void);
int punch_pp_assemblage(void);
int punch_s_s_assemblage(void);
int punch_saturation_indices(void);
int punch_totals(void);
int punch_user_punch(void);
#ifdef PHREEQ98
int punch_user_graph(void);
#endif

// read.c -------------------------------

int read_input(void);
int read_conc(int n, int count_mass_balance, char *str);
int *read_list_ints_range(char **ptr, int *count_ints, int positive,
						  int *int_list);
int read_log_k_only(char *ptr, LDBLE * log_k);
int read_number_description(char *ptr, int *n_user, int *n_user_end,
							char **description);
int check_key(char *str);
int check_units(char *tot_units, int alkalinity, int check_compatibility,
				const char *default_units, int print);
int find_option(char *item, int *n, const char **list, int count_list,
				int exact);
int get_option(const char **opt_list, int count_opt_list, char **next_char);
int get_true_false(char *string, int default_value);

int add_psi_master_species(char *token);
int read_advection(void);
int read_analytical_expression_only(char *ptr, LDBLE * log_k);
/* VP: Density Start */
int read_millero_abcdef (char *ptr, LDBLE * abcdef);
/* VP: Density End */
int read_copy(void);
int read_debug(void);
int read_delta_h_only(char *ptr, LDBLE * delta_h,
							 DELTA_H_UNIT * units);
int read_llnl_aqueous_model_parameters(void);
int read_exchange(void);
int read_exchange_master_species(void);
int read_exchange_species(void);
int read_gas_phase(void);
int read_incremental_reactions(void);
int read_inverse(void);
int read_inv_balances(struct inverse *inverse_ptr, char *next_char);
int read_inv_isotopes(struct inverse *inverse_ptr, char *ptr);
int read_inv_phases(struct inverse *inverse_ptr, char *next_char);
int read_kinetics(void);
int read_line_doubles(char *next_char, LDBLE ** d, int *count_d,
							 int *count_alloc);
int read_lines_doubles(char *next_char, LDBLE ** d, int *count_d,
							  int *count_alloc, const char **opt_list,
							  int count_opt_list, int *opt);
LDBLE *read_list_doubles(char **ptr, int *count_doubles);
int *read_list_ints(char **ptr, int *count_ints, int positive);
int *read_list_t_f(char **ptr, int *count_ints);
int read_master_species(void);
int read_mix(void);
int read_named_logk(void);
int read_phases(void);
int read_print(void);
int read_pure_phases(void);
int read_rates(void);
int read_reaction(void);
int read_reaction_reactants(struct irrev *irrev_ptr);
int read_reaction_steps(struct irrev *irrev_ptr);
int read_solid_solutions(void);
int read_temperature(void);
int read_reaction_temps(struct temperature *temperature_ptr);
int read_save(void);
int read_selected_output(void);
int read_solution(void);
int read_species(void);
int read_surf(void);
int read_surface_master_species(void);
int read_surface_species(void);
int read_use(void);
int read_title(void);
int read_user_print(void);
int read_user_punch(void);
#ifdef PHREEQ98
int read_user_graph(void);
int copy_title(char *token_ptr, char **ptr, int *length);
int OpenCSVFile(char file_name[MAX_LENGTH]);
void GridHeadings(char *s, int i);
void SetAxisTitles(char *s, int i);
void SetAxisScale(char *a, int c, char *v, int l);
void SetChartTitle(char *s);
#endif
int next_keyword_or_option(const char **opt_list, int count_opt_list);

// ReadClass.cxx
 int read_solution_raw(void);
 int read_exchange_raw(void);
 int read_surface_raw(void);
 int read_equilibrium_phases_raw(void);
 int read_kinetics_raw(void);
 int read_solid_solutions_raw(void);
 int read_gas_phase_raw(void);
 int read_reaction_raw(void);
 int read_mix_raw(void);
 int read_temperature_raw(void);
 int read_dump(void);
 int read_solution_modify(void);
 int read_equilibrium_phases_modify(void);
 int read_exchange_modify(void);
 int read_surface_modify(void);
 int read_solid_solutions_modify(void);
 int read_gas_phase_modify(void);
 int read_kinetics_modify(void);
 int read_delete(void);
 int read_run_cells(void);
 int streamify_to_next_keyword(std::istringstream & lines);
 int dump_entities(void);
 int delete_entities(void);
 int run_as_cells(void);
 void dump_ostream(std::ostream& os);


// readtr.c -------------------------------

int read_transport(void);
int dump(void);
int dump_exchange(int k);
int dump_gas_phase(int k);
int dump_kinetics(int k);
int dump_mix(int k);
int dump_pp_assemblage(int k);
int dump_reaction(int k);
int dump_s_s_assemblage(int k);
int dump_solution(int k);
int dump_surface(int k);
int dump_cpp(void);

int read_line_LDBLEs(char *next_char, LDBLE ** d, int *count_d,
							int *count_alloc);

// sit.c -------------------------------

int gammas_sit(void);
int model_sit(void);
int sit(void);
int sit_clean_up(void);
int sit_init(void);
int sit_tidy(void);
int read_sit(void);
int set_sit(int initial);

int calc_sit_param(struct pitz_param *pz_ptr, LDBLE TK, LDBLE TR);
int check_gammas_sit(void);
int sit_ISPEC(char *name);
/*int DH_AB (LDBLE TK, LDBLE *A, LDBLE *B);*/
int sit_initial_guesses(void);
int sit_revise_guesses(void);
int sit_remove_unstable_phases;
int PTEMP_SIT(LDBLE tk);
int jacobian_sit(void);

// spread.c -------------------------------

int read_solution_spread(void);

int copy_token_tab(char *token_ptr, char **ptr, int *length);
int get_option_string(const char **opt_list, int count_opt_list,
							 char **next_char);
int spread_row_free(struct spread_row *spread_row_ptr);
int spread_row_to_solution(struct spread_row *heading,
								  struct spread_row *units,
								  struct spread_row *data,
								  struct defaults defaults);
struct spread_row *string_to_spread_row(char *string);
#ifdef PHREEQCI_GUI
void add_row(struct spread_row *spread_row_ptr);
void copy_defaults(struct defaults *dest_ptr,
						  struct defaults *src_ptr);
void free_spread(void);
struct spread_row *copy_row(struct spread_row *spread_row_ptr);
#endif

// step.c -------------------------------

int step(LDBLE step_fraction);
int xsolution_zero(void);
int add_exchange(struct exchange *exchange_ptr);
int add_gas_phase(struct gas_phase *gas_phase_ptr);
int add_kinetics(struct kinetics *kinetics_ptr);
int add_mix(struct mix *mix_ptr);
int add_pp_assemblage(struct pp_assemblage *pp_assemblage_ptr);
int add_reaction(struct irrev *irrev_ptr, int step_number,
				 LDBLE step_fraction);
int add_s_s_assemblage(struct s_s_assemblage *s_s_assemblage_ptr);
int add_solution(struct solution *solution_ptr, LDBLE extensive,
				 LDBLE intensive);
int add_surface(struct surface *surface_ptr);
int add_temperature(struct temperature *temperature_ptr, int step_number);

int check_pp_assemblage(struct pp_assemblage *pp_assemblage_ptr);
int gas_phase_check(struct gas_phase *gas_phase_ptr);
int pp_assemblage_check(struct pp_assemblage *pp_assemblage_ptr);
int reaction_calc(struct irrev *irrev_ptr);
int solution_check(void);
int s_s_assemblage_check(struct s_s_assemblage *s_s_assemblage_ptr);

// structures.c -------------------------------

int clean_up(void);
int reinitialize(void);
int copier_add(struct copier *copier_ptr, int n_user, int start, int end);
int copier_free(struct copier *copier_ptr);
int copier_init(struct copier *copier_ptr);
CLASS_STATIC int element_compare(const void *ptr1, const void *ptr2);
public:
	struct element *element_store(const char *element);
	int elt_list_combine(void);
	CLASS_STATIC int elt_list_compare(const void *ptr1, const void *ptr2);
private:
struct elt_list *elt_list_dup(struct elt_list *elt_list_ptr_old);
int elt_list_print(struct elt_list *elt_list_ptr);
struct elt_list *elt_list_save(void);
public:
	struct exchange *exchange_alloc(void);
	struct exchange *exchange_bsearch(int k, int *n);
private:
int exchange_comp_compare(const void *ptr1, const void *ptr2);
void exchange_comp_init(struct exch_comp *exch_comp_ptr);
public:
	int exchange_copy(struct exchange *exchange_old_ptr,
		struct exchange *exchange_new_ptr, int n_user_new);
private:
CLASS_STATIC int exchange_compare(const void *ptr1, const void *ptr2);
int exchange_copy_to_last(int n, int n_user);
int exchange_delete(int n_user_old);
int exchange_duplicate(int n_user_old, int n_user_new);
int exchange_init(struct exchange *exchange_ptr, int n_user, int n_user_end,
				  const char *description);
public:
	int exchange_free(struct exchange *exchange_ptr);
private:
int exchange_ptr_to_user(struct exchange *exchange_old_ptr, int n_user_new);
struct exchange *exchange_replicate(struct exchange *exchange_old_ptr,
									int n_user_new);
struct exchange *exchange_search(int n_user, int *n, int print);
int exchange_sort(void);
CLASS_STATIC int gas_comp_compare(const void *ptr1, const void *ptr2);
public:
	struct gas_phase *gas_phase_alloc(void);
	struct gas_phase *gas_phase_bsearch(int k, int *n);
private:
CLASS_STATIC int gas_phase_compare(const void *ptr1, const void *ptr2);
public:
int gas_phase_copy(struct gas_phase *gas_phase_old_ptr,
				   struct gas_phase *gas_phase_new_ptr, int n_user_new);
private:
int gas_phase_copy_to_last(int n, int n_user);
int gas_phase_delete(int n_user_old);
int gas_phase_duplicate(int n_user_old, int n_user_new);
int gas_phase_init(struct gas_phase *gas_phase_ptr, int n_user,
				   int n_user_end, char *description);
public:
int gas_phase_free(struct gas_phase *gas_phase_ptr);
private:
int gas_phase_ptr_to_user(struct gas_phase *gas_phase_ptr_old,
						  int n_user_new);
struct gas_phase *gas_phase_replicate(struct gas_phase *gas_phase_old_ptr,
									  int n_user_new);
struct gas_phase *gas_phase_search(int n_user, int *n);
int gas_phase_sort(void);
enum entity_type get_entity_enum(char *name);
struct inverse *inverse_alloc(void);
int inverse_delete(int i);
CLASS_STATIC int inverse_isotope_compare(const void *ptr1, const void *ptr2);
struct inverse *inverse_search(int n_user, int *n);
int inverse_sort(void);
struct irrev *irrev_bsearch(int k, int *n);
int irrev_copy(struct irrev *irrev_old_ptr, struct irrev *irrev_new_ptr,
			   int n_user_new);
int irrev_delete(int n_user_old);
int irrev_duplicate(int n_user_old, int n_user_new);
int irrev_free(struct irrev *irrev_ptr);
struct irrev *irrev_search(int n_user, int *n);
int irrev_sort(void);
public:
	struct kinetics *kinetics_alloc(void);
	struct kinetics *kinetics_bsearch(int k, int *n);
private:
int kinetics_delete(int n_user_old);
int kinetics_comp_duplicate(struct kinetics_comp *kinetics_comp_new_ptr,
							struct kinetics_comp *kinetics_comp_old_ptr);
CLASS_STATIC int kinetics_compare(const void *ptr1, const void *ptr2);
public:
int kinetics_copy(struct kinetics *kinetics_old_ptr,
				  struct kinetics *kinetics_new_ptr, int n_user_new);
private:				 
int kinetics_copy_to_last(int n, int n_user);
int kinetics_duplicate(int n_user_old, int n_user_new);
int kinetics_init(struct kinetics *kinetics_ptr, int n_user, int n_user_end,
				  char *description);
public:
int kinetics_free(struct kinetics *kinetics_ptr);
private:
int kinetics_ptr_to_user(struct kinetics *kinetics_ptr_old, int n_user_new);
struct kinetics *kinetics_replicate(struct kinetics *kinetics_old_ptr,
									int n_user_new);
struct kinetics *kinetics_search(int n_user, int *n, int print);
int kinetics_sort(void);
struct logk *logk_alloc(void);
int logk_copy2orig(struct logk *logk_ptr);
struct logk *logk_store(char *name, int replace_if_found);
struct logk *logk_search(char *name);
struct master *master_alloc(void);
CLASS_STATIC int master_compare(const void *ptr1, const void *ptr2);
int master_delete(char *ptr);
public:
struct master *master_bsearch(const char *ptr);
struct master *master_bsearch_primary(char *ptr);
private:
struct master *master_search(char *ptr, int *n);
struct mix *mix_bsearch(int k, int *n);
int mix_copy(struct mix *mix_old_ptr,
			 struct mix *mix_new_ptr, int n_user_new);
int mix_delete(int n_user_old);
int mix_duplicate(int n_user_old, int n_user_new);
int mix_free(struct mix *mix_ptr);
struct mix *mix_search(int n_user, int *n, int print);
int mix_sort(void);
struct pe_data *pe_data_alloc(void);
public:
	struct pe_data *pe_data_dup(struct pe_data *pe_ptr_old);
	struct pe_data *pe_data_free(struct pe_data *pe_data_ptr);
private:
int pe_data_store(struct pe_data **pe, const char *token);
public:
struct phase *phase_bsearch(const char *ptr, int *j, int print);
private:
CLASS_STATIC int phase_compare(const void *ptr1, const void *ptr2);
int phase_delete(int i);
struct phase *phase_store(char *name);
public:
struct pp_assemblage *pp_assemblage_alloc(void);
struct pp_assemblage *pp_assemblage_bsearch(int k, int *n);
private:
CLASS_STATIC int pp_assemblage_compare(const void *ptr1, const void *ptr2);
public:
int pp_assemblage_copy(struct pp_assemblage *pp_assemblage_old_ptr,
					   struct pp_assemblage *pp_assemblage_new_ptr,
					   int n_user_new);
private:
int pp_assemblage_copy_to_last(int n, int n_user);
int pp_assemblage_delete(int n_user_old);
int pp_assemblage_duplicate(int n_user_old, int n_user_new);
public:
int pp_assemblage_free(struct pp_assemblage *pp_assemblage_ptr);
private:
int pp_assemblage_init(struct pp_assemblage *pp_assemblage_ptr, int n_user,
					   int n_user_end, char *description);
int pp_assemblage_ptr_to_user(struct pp_assemblage *pp_assemblage_ptr_old,
							  int n_user_new);
struct pp_assemblage *pp_assemblage_replicate(struct pp_assemblage
											  *pp_assemblage_old_ptr,
											  int n_user_new);
struct pp_assemblage *pp_assemblage_search(int n_user, int *n);
int pp_assemblage_sort(void);
CLASS_STATIC int pure_phase_compare(const void *ptr1, const void *ptr2);
struct rate *rate_bsearch(char *ptr, int *j);
int rate_free(struct rate *rate_ptr);
struct rate *rate_search(char *name, int *n);
int rate_sort(void);
struct reaction *rxn_alloc(int ntokens);
struct reaction *rxn_dup(struct reaction *rxn_ptr_old);
LDBLE rxn_find_coef(struct reaction *r_ptr, const char *str);
int rxn_free(struct reaction *rxn_ptr);
int rxn_print(struct reaction *rxn_ptr);
CLASS_STATIC int s_compare(const void *ptr1, const void *ptr2);
int s_delete(int i);
struct species *s_search(const char *name);
struct species *s_store(char *name, LDBLE z, int replace_if_found);
public:
	struct s_s_assemblage *s_s_assemblage_alloc(void);
	struct s_s_assemblage *s_s_assemblage_bsearch(int k, int *n);
private:
CLASS_STATIC int s_s_assemblage_compare(const void *ptr1, const void *ptr2);
public:
	int s_s_assemblage_copy(struct s_s_assemblage *s_s_assemblage_old_ptr,
						struct s_s_assemblage *s_s_assemblage_new_ptr,
						int n_user_new);
private:
int s_s_assemblage_copy_to_last(int n, int n_user);
int s_s_assemblage_duplicate(int n_user_old, int n_user_new);
int s_s_assemblage_delete(int n_user_old);
public:
	int s_s_assemblage_free(struct s_s_assemblage *s_s_assemblage_ptr);
private:
int s_s_assemblage_init(struct s_s_assemblage *s_s_assemblage_ptr,
						int n_user, int n_user_end, char *description);
int s_s_assemblage_ptr_to_user(struct s_s_assemblage *s_s_assemblage_ptr_old,
							   int n_user_new);
struct s_s_assemblage *s_s_assemblage_replicate(struct s_s_assemblage
												*s_s_assemblage_old_ptr,
												int n_user_new);
struct s_s_assemblage *s_s_assemblage_search(int n_user, int *n);
int s_s_assemblage_sort(void);
CLASS_STATIC int s_s_compare(const void *ptr1, const void *ptr2);
struct save_values *save_values_bsearch(struct save_values *k, int *n);
CLASS_STATIC int save_values_compare(const void *ptr1, const void *ptr2);
int save_values_sort(void);
int save_values_store(struct save_values *s_v);
CLASS_STATIC int conc_compare(const void *ptr1, const void *ptr2);
int conc_init(struct conc *conc_ptr);
CLASS_STATIC int isotope_compare(const void *ptr1, const void *ptr2);
public:
	struct solution *solution_alloc(void);
	struct solution *solution_bsearch(int k, int *n, int print);
private:
	struct solution *solution_copy(struct solution *solution_old_ptr,
							   int n_user_new);
int solution_copy_to_last(int n, int n_user_new);
int solution_duplicate(int n_user_old, int n_user_new);
int solution_delete(int n_user_old);
int solution_delete_n(int n);
int solution_free(struct solution *solution_ptr);
int solution_ptr_to_user(struct solution *solution_old_ptr, int n_user_new);
struct solution *solution_replicate(struct solution *solution_old_ptr,
									int n_user_new);
int solution_sort(void);
CLASS_STATIC int species_list_compare_alk(const void *ptr1, const void *ptr2);
CLASS_STATIC int species_list_compare_master(const void *ptr1, const void *ptr2);
int species_list_sort(void);
struct Change_Surf *change_surf_alloc(int count);
public:
	struct surface *surface_alloc(void);
	struct surface *surface_bsearch(int k, int *n);
private:
struct master *surface_get_psi_master(const char *name, int plane);
CLASS_STATIC int surface_comp_compare(const void *ptr1, const void *ptr2);
CLASS_STATIC int surface_charge_compare(const void *ptr1, const void *ptr2);
struct surface_charge * surface_charge_duplicate(struct surface_charge *charge_old_ptr);
int surface_charge_free(struct surface_charge *charge);
CLASS_STATIC int surface_compare(const void *ptr1, const void *ptr2);
public:
	int surface_copy(struct surface *surface_old_ptr,
				 struct surface *surface_new_ptr, int n_user_new);
private:
int surface_copy_to_last(int n, int n_user);
int surface_delete(int n_user_old);
int surface_duplicate(int n_user_old, int n_user_new);
public:
	int surface_free(struct surface *surface_ptr);
protected:
int surface_init(struct surface *surface_ptr, int n_user, int n_user_end,
				 char *description);
int surface_ptr_to_user(struct surface *surface_ptr_old, int n_user_new);
struct surface *surface_replicate(struct surface *surface_old_ptr,
								  int n_user_new);
struct surface *surface_search(int n_user, int *n, int print);
int surface_sort(void);
int system_duplicate(int i, int save_old);
struct temperature *temperature_bsearch(int k, int *n);
int temperature_copy(struct temperature *temperature_old_ptr,
					 struct temperature *temperature_new_ptr, int n_user_new);
int temperature_delete(int n_user_old);
int temperature_duplicate(int n_user_old, int n_user_new);
int temperature_free(struct temperature *temperature_ptr);
struct temperature *temperature_search(int n_user, int *n);
int temperature_sort(void);
int trxn_add(struct reaction *r_ptr, LDBLE coef, int combine);
int trxn_add_phase(struct reaction *r_ptr, LDBLE coef, int combine);
int trxn_combine(void);
int trxn_copy(struct reaction *rxn_ptr);
LDBLE trxn_find_coef(const char *str, int start);
int trxn_print(void);
int trxn_reverse_k(void);
int trxn_sort(void);
int trxn_swap(const char *token);
struct unknown *unknown_alloc(void);
int unknown_delete(int i);
int unknown_free(struct unknown *unknown_ptr);
int entity_exists(char *name, int n_user);

static int exchange_compare_int(const void *ptr1, const void *ptr2);
static int gas_phase_compare_int(const void *ptr1, const void *ptr2);
static int inverse_compare(const void *ptr1, const void *ptr2);
int inverse_free(struct inverse *inverse_ptr);
static int irrev_compare(const void *ptr1, const void *ptr2);
static int irrev_compare_int(const void *ptr1, const void *ptr2);
static int kinetics_compare_int(const void *ptr1, const void *ptr2);
int logk_init(struct logk *logk_ptr);
static int master_compare_string(const void *ptr1, const void *ptr2);
int master_free(struct master *master_ptr);
static int mix_compare(const void *ptr1, const void *ptr2);
static int mix_compare_int(const void *ptr1, const void *ptr2);
struct phase *phase_alloc(void);
static int phase_compare_string(const void *ptr1, const void *ptr2);
int phase_free(struct phase *phase_ptr);
int phase_init(struct phase *phase_ptr);
static int pp_assemblage_compare_int(const void *ptr1, const void *ptr2);
static int rate_compare(const void *ptr1, const void *ptr2);
static int rate_compare_string(const void *ptr1, const void *ptr2);
struct species *s_alloc(void);
int s_free(struct species *s_ptr);
int s_init(struct species *s_ptr);
static int s_s_assemblage_compare_int(const void *ptr1, const void *ptr2);
static int solution_compare(const void *ptr1, const void *ptr2);
static int solution_compare_int(const void *ptr1, const void *ptr2);
static int species_list_compare(const void *ptr1, const void *ptr2);
static int surface_compare_int(const void *ptr1, const void *ptr2);
static int temperature_compare(const void *ptr1, const void *ptr2);
static int temperature_compare_int(const void *ptr1, const void *ptr2);
static int rxn_token_temp_compare(const void *ptr1, const void *ptr2);
int trxn_multiply(LDBLE coef);
#ifdef PHREEQCI_GUI
extern void free_spread(void);
#endif
#if defined(USE_MPI) && defined(HDF5_CREATE) && defined(MERGE_FILES)
extern void MergeFinalize(void);
#endif

/* tally.c */

void add_all_components_tally(void);
int build_tally_table(void);
int calc_dummy_kinetic_reaction_tally(struct kinetics *kinetics_ptr);
int diff_tally_table(void);
int extend_tally_table(void);
int free_tally_table(void);
int fill_tally_table(int *n_user, int index_conservative, int n_buffer);
int get_tally_table_rows_columns(int *rows, int *columns);
int get_tally_table_column_heading(int column, int *type, char *string);
int get_tally_table_row_heading(int column, char *string);
int store_tally_table(LDBLE * array, int row_dim, int col_dim,
					  LDBLE fill_factor);
int zero_tally_table(void);

int elt_list_to_tally_table(struct tally_buffer *buffer_ptr);
int get_all_components(void);
int print_tally_table(void);
int set_reaction_moles(int n_user, LDBLE moles);
int set_reaction_temperature(int n_user, LDBLE tc);
int set_kinetics_time(int n_user, LDBLE step);

// tidy.c -------------------------------

int add_other_logk(LDBLE * source_k, int count_add_logk,
				   struct name_coef *add_logk);
int add_logks(struct logk *logk_ptr, int repeats);
LDBLE halve(LDBLE f(LDBLE x, void *), LDBLE x0, LDBLE x1, LDBLE tol);
int replace_solids_gases(void);
int s_s_prep(LDBLE t, struct s_s *s_s_ptr, int print);
int select_log_k_expression(LDBLE * source_k, LDBLE * target_k);
int slnq(int n, LDBLE * a, LDBLE * delta, int ncols, int print);
public:
int tidy_punch(void);
int tidy_model(void);

int check_species_input(void);
LDBLE coef_in_master(struct master *master_ptr);
int phase_rxn_to_trxn(struct phase *phase_ptr,
							 struct reaction *rxn_ptr);
int reset_last_model(void);
int rewrite_eqn_to_primary(void);
int rewrite_eqn_to_secondary(void);
int species_rxn_to_trxn(struct species *s_ptr);
int tidy_logk(void);
int tidy_exchange(void);
int tidy_min_exchange(void);
int tidy_kin_exchange(void);
int tidy_gas_phase(void);
int tidy_inverse(void);
int tidy_isotopes(void);
int tidy_isotope_ratios(void);
int tidy_isotope_alphas(void);
int tidy_kin_surface(void);
int tidy_master_isotope(void);
int tidy_min_surface(void);
int tidy_phases(void);
int tidy_pp_assemblage(void);
int tidy_solutions(void);
int tidy_s_s_assemblage(void);
int tidy_species(void);
int tidy_surface(void);
int scan(LDBLE f(LDBLE x, void *), LDBLE * xx0, LDBLE * xx1);
static LDBLE f_spinodal(LDBLE x, void *);
int solve_misc(LDBLE * xxc1, LDBLE * xxc2, LDBLE tol);
int s_s_calc_a0_a1(struct s_s *s_s_ptr);

// transport.c -------------------------------

int transport(void);
int set_initial_moles(int i);
int sum_surface_comp(struct surface *source1, LDBLE f1,
					 struct surface *source2, int k, LDBLE f2,
					 struct surface *target, LDBLE new_Dw);
int reformat_surf(char *comp_name, LDBLE fraction, char *new_comp_name,
				  LDBLE new_Dw, int cell);
LDBLE viscosity(void);

int multi_D(LDBLE DDt, int mobile_cell, int stagnant);
int find_J(int icell, int jcell, LDBLE mixf, LDBLE DDt, int stagnant);
int fill_spec(int cell_no);
int fill_m_s(struct J_ij *J_ij, int J_ij_count_spec);
static int sort_species_name(const void *ptr1, const void *ptr2);
int disp_surf(LDBLE stagkin_time);
int diff_stag_surf(int mobile_cell);
int check_surfaces(struct surface *surface_ptr1,
						  struct surface *surface_ptr2);
int mobile_surface_copy(struct surface *surface_old_ptr,
							   struct surface *surf_ptr1, int n_user_new,
							   int move_old);
int init_mix(void);
int init_heat_mix(int nmix);
int heat_mix(int heat_nmix);
int mix_stag(int i, LDBLE stagkin_time, int punch,
					LDBLE step_fraction_kin);


// utilities.c -------------------------------
public:
int add_elt_list(struct elt_list *elt_list_ptr, LDBLE coef);
private:
int backspace_screen(int spaces);
LDBLE calc_alk(struct reaction *rxn_ptr);
public:
	int compute_gfw(const char *string, LDBLE * gfw);
int copy_token(char *token_ptr, char **ptr, int *length);
int dup_print(const char *ptr, int emphasis);
int equal(LDBLE a, LDBLE b, LDBLE eps);
public:
	void *free_check_null(void *ptr);
private:
void free_hash_strings(HashTable * Table);
int get_token(char **eqnaddr, char *string, LDBLE * z, int *l);
int hcreate_multi(unsigned Count, HashTable ** HashTable_ptr);
void hdestroy_multi(HashTable * HashTable_ptr);
ENTRY *hsearch_multi(HashTable * Table, ENTRY item, ACTION action);
int islegit(const char c);
public:
void malloc_error(void);
private:
int parse_couple(char *token);
int print_centered(const char *string);
public:
CLASS_STATIC int replace(const char *str1, const char *str2, char *str);
private:
void space(void **ptr, int i, int *max, int struct_size);
void squeeze_white(char *s_l);
int status(int count, const char *str);
void str_tolower(char *str);
void str_toupper(char *str);
CLASS_STATIC int strcmp_nocase(const char *str1, const char *str2);
int strcmp_nocase_arg1(const char *str1, const char *str2);
public:
	char *string_duplicate(const char *token);
	char *string_hsave(const char *str);
private:
char *string_pad(char *str, int i);
int string_trim(char *str);
int string_trim_right(char *str);
int string_trim_left(char *str);
CLASS_STATIC LDBLE under(LDBLE xval);
void zero_double(LDBLE * target, int n);

#ifdef PHREEQ98
void AddToCEntry(char *a, int l, int i);
void ApplicationProcessMessages(void);
int copy_title(char *token_ptr, char **ptr, int *length);
extern int clean_up_null(void);
#endif
int isamong(char c, const char *s_l);
Address Hash_multi(HashTable * Table, char *Key);
void ExpandTable_multi(HashTable * Table);

public:        // public methods for PHREEQC_CLASS
	int main_method(int argc, char *argv[]);
	void set_phast(int);
	size_t list_components(std::list<std::string> &list_c);
protected:
	void init(void);
};
#endif /* _INC_PHREEQC_H */

