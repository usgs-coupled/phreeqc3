#include <time.h>
#include <assert.h>
#include "Utils.h"
#include "Phreeqc.h"
#include "phqalloc.h"
#include "PBasic.h"
#include "Temperature.h"
#include "Exchange.h"
#include "ExchComp.h"
#include "GasPhase.h"
#include "Reaction.h"
#include "PPassemblage.h"
#include "SSassemblage.h"

#if defined(WINDOWS) || defined(_WINDOWS)
#include <windows.h>
#endif

/* ---------------------------------------------------------------------- */
void Phreeqc::
initialize(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Initialize global variables
 */
	int i;
	struct logk *logk_ptr;
	char token[MAX_LENGTH];

	moles_per_kilogram_string = string_duplicate("Mol/kgw");
	pe_string = string_duplicate("pe");

	debug_model = FALSE;
	debug_prep = FALSE;
	debug_set = FALSE;
	debug_diffuse_layer = FALSE;
	debug_inverse = FALSE;
	itmax = 100;
	max_tries = 100;
#ifdef USE_LONG_DOUBLE
	/* from float.h, sets tolerance for cl1 routine */
	ineq_tol = pow((long double) 10, (long double) -LDBL_DIG);
#else
	ineq_tol = pow((double) 10, (double) -DBL_DIG);
#endif
	convergence_tolerance = 1e-8;
#ifdef USE_LONG_DOUBLE
	/* from float.h, sets tolerance for cl1 routine */
	inv_tol_default = pow((long double) 10, (long double) -LDBL_DIG + 5);
#else
	inv_tol_default = pow((double) 10, (double) -DBL_DIG + 5);
#endif
	step_size = 100.;
	pe_step_size = 10.;
	pp_scale = 1.0;
	pp_column_scale = 1.0;
	diagonal_scale = FALSE;
	censor = 0.0;
	mass_water_switch = FALSE;
	/*      mass_water_switch = TRUE; */
	delay_mass_water = FALSE;
	incremental_reactions = FALSE;
	aqueous_only = 0;
	negative_concentrations = FALSE;

	LOG_10 = log(10.0);
	/* Use space for all memory allocation */
	max_solution = MAX_SOLUTION;
	//max_pp_assemblage = MAX_PP_ASSEMBLAGE;
	//max_exchange = MAX_PP_ASSEMBLAGE;
	max_surface = MAX_PP_ASSEMBLAGE;
	//max_gas_phase = MAX_PP_ASSEMBLAGE;
	max_kinetics = MAX_PP_ASSEMBLAGE;
	//max_ss_assemblage = MAX_PP_ASSEMBLAGE;

	max_elements = MAX_ELEMENTS;
	max_elts = MAX_ELTS;
	max_line = MAX_LINE;
	max_master = MAX_MASTER;
	max_mb_unknowns = MAX_TRXN;
	max_phases = MAX_PHASES;
	max_s = MAX_S;
	max_strings = MAX_STRINGS;
	max_trxn = MAX_TRXN;
	max_logk = MAX_S;
	max_master_isotope = MAX_ELTS;

	count_solution = 0;
	//count_pp_assemblage = 0;
	//count_exchange = 0;
	count_surface = 0;
	//count_gas_phase = 0;
	count_kinetics = 0;
	//count_ss_assemblage = 0;

	count_elements = 0;
	//count_irrev = 0;
	count_master = 0;
	//count_mix = 0;
	count_phases = 0;
	count_s = 0;
	count_logk = 0;
	count_master_isotope = 0;
/*
 *   Initialize advection
 */
	count_ad_cells = 1;
	count_ad_shifts = 1;
	print_ad_modulus = 1;
	punch_ad_modulus = 1;
	advection_punch = (int *) PHRQ_malloc(sizeof(int));
	if (advection_punch == NULL)
		malloc_error();
	advection_punch[0] = TRUE;
	advection_kin_time = 0.0;
	advection_kin_time_defined = FALSE;
	advection_print = (int *) PHRQ_malloc(sizeof(int));
	if (advection_print == NULL)
		malloc_error();
	advection_print[0] = TRUE;
	advection_warnings = TRUE;
/*
 *   Initialize transport
 */
	count_cells = 1;
	count_shifts = 1;
	ishift = 1;
	bcon_first = bcon_last = 3;
	diffc = 0.3e-9;
	simul_tr = 0;
	tempr = 2.0;
	heat_diffc = -0.1;
	timest = 0.0;
	multi_Dflag = FALSE;
	interlayer_Dflag = FALSE;
	interlayer_tortf = 100.0;
	interlayer_Dpor = 0.1;
/* !!!!        count_stag = 0; */
	mcd_substeps = 1.0;
	print_modulus = 1;
	punch_modulus = 1;
	dump_modulus = 0;
	dump_in = FALSE;
	transport_warnings = TRUE;
/*
 *   Allocate space
 */
	//space((void **) ((void *) &pp_assemblage), INIT, &max_pp_assemblage,
	//	  sizeof(struct pp_assemblage));

	//space((void **) ((void *) &exchange), INIT, &max_exchange,
	//	  sizeof(struct exchange));

	space((void **) ((void *) &surface), INIT, &max_surface,
		  sizeof(struct surface));

	//space((void **) ((void *) &gas_phase), INIT, &max_gas_phase,
	//	  sizeof(struct gas_phase));

	space((void **) ((void *) &kinetics), INIT, &max_kinetics,
		  sizeof(struct kinetics));

	//space((void **) ((void *) &ss_assemblage), INIT, &max_ss_assemblage,
	//	  sizeof(struct ss_assemblage));

	space((void **) ((void *) &cell_data), INIT, &count_cells,
		  sizeof(struct cell_data));

	space((void **) ((void *) &elements), INIT, &max_elements,
		  sizeof(struct element *));

	space((void **) ((void *) &elt_list), INIT, &max_elts,
		  sizeof(struct elt_list));


	inverse = (struct inverse *) PHRQ_malloc((size_t) sizeof(struct inverse));
	if (inverse == NULL)
		malloc_error();
	count_inverse = 0;
	space((void **) ((void *) &line), INIT, &max_line, sizeof(char));

	space((void **) ((void *) &line_save), INIT, &max_line, sizeof(char));

	space((void **) ((void *) &master), INIT, &max_master,
		  sizeof(struct master *));

	space((void **) ((void *) &mb_unknowns), INIT, &max_mb_unknowns,
		  sizeof(struct unknown_list));

	//mix = (struct mix *) PHRQ_malloc((size_t) sizeof(struct mix));
	//if (mix == NULL)
	//	malloc_error();
	//count_mix = 0;
/* !!!! */
	stag_data = (struct stag_data *) PHRQ_calloc(1, sizeof(struct stag_data));
	if (stag_data == NULL)
		malloc_error();
	stag_data->count_stag = 0;
	stag_data->exch_f = 0;
	stag_data->th_m = 0;
	stag_data->th_im = 0;

	space((void **) ((void *) &phases), INIT, &max_phases,
		  sizeof(struct phase *));

	space((void **) ((void *) &trxn.token), INIT, &max_trxn,
		  sizeof(struct rxn_token_temp));

	space((void **) ((void *) &s), INIT, &max_s, sizeof(struct species *));

	space((void **) ((void *) &logk), INIT, &max_logk, sizeof(struct logk *));

	space((void **) ((void *) &master_isotope), INIT, &max_master_isotope,
		  sizeof(struct master_isotope *));

	solution =
		(struct solution **) PHRQ_malloc((size_t) MAX_SOLUTION *
										 sizeof(struct solution *));
	if (solution == NULL)
		malloc_error();

	title_x = NULL;
	pe_x = NULL;
	description_x = NULL;
	units_x = NULL;
	s_x = NULL;
/* SRC ADDED */
	sum_mb1 = NULL;
	sum_mb2 = NULL;
	sum_jacob0 = NULL;
	sum_jacob1 = NULL;
	sum_jacob2 = NULL;
	sum_delta = NULL;
/* SRC ADDED */
	count_isotopes_x = 0;
	isotopes_x =
		(struct isotope *) PHRQ_malloc((size_t) sizeof(struct isotope));
	if (isotopes_x == NULL)
		malloc_error();
	x = NULL;
	max_unknowns = 0;

	array = NULL;
	delta = NULL;
	residual = NULL;
	s_h2o = NULL;
	s_hplus = NULL;
	s_h3oplus = NULL;
	s_eminus = NULL;
	s_co3 = NULL;
	s_h2 = NULL;
	s_o2 = NULL;

	hcreate_multi((unsigned) max_logk, &logk_hash_table);
	hcreate_multi((unsigned) max_master_isotope, &master_isotope_hash_table);
/*
 *   Create hash tables
 */
	hcreate_multi((unsigned) max_elements, &elements_hash_table);
	hcreate_multi((unsigned) max_s, &species_hash_table);
	hcreate_multi((unsigned) max_phases, &phases_hash_table);
/*
 *  Initialize use pointers
 */
	//use_init();
/*
 *   Initialize punch
 */
	punch.in = FALSE;
	punch.new_def = FALSE;
	punch.count_totals = 0;
	punch.totals =
		(struct name_master *) PHRQ_malloc(sizeof(struct name_master));
	if (punch.totals == NULL)
		malloc_error();
	punch.count_molalities = 0;
	punch.molalities =
		(struct name_species *) PHRQ_malloc(sizeof(struct name_species));
	if (punch.molalities == NULL)
		malloc_error();
	punch.count_activities = 0;
	punch.activities =
		(struct name_species *) PHRQ_malloc(sizeof(struct name_species));
	if (punch.activities == NULL)
		malloc_error();
	punch.count_pure_phases = 0;
	punch.pure_phases =
		(struct name_phase *) PHRQ_malloc(sizeof(struct name_phase));
	if (punch.pure_phases == NULL)
		malloc_error();
	punch.count_si = 0;
	punch.si = (struct name_phase *) PHRQ_malloc(sizeof(struct name_phase));
	if (punch.si == NULL)
		malloc_error();
	punch.count_gases = 0;
	punch.gases =
		(struct name_phase *) PHRQ_malloc(sizeof(struct name_phase));
	if (punch.gases == NULL)
		malloc_error();
	punch.count_s_s = 0;
	punch.s_s = (struct name_phase *) PHRQ_malloc(sizeof(struct name_phase));
	if (punch.s_s == NULL)
		malloc_error();

	punch.count_kinetics = 0;
	punch.kinetics =
		(struct name_phase *) PHRQ_malloc(sizeof(struct name_phase));
	if (punch.kinetics == NULL)
		malloc_error();

	punch.count_isotopes = 0;
	punch.isotopes =
		(struct name_master *) PHRQ_malloc(sizeof(struct name_master));
	if (punch.isotopes == NULL)
		malloc_error();

	punch.count_calculate_values = 0;
	punch.calculate_values =
		(struct name_master *) PHRQ_malloc(sizeof(struct name_master));
	if (punch.calculate_values == NULL)
		malloc_error();

	count_save_values = 0;
	save_values =
		(struct save_values *) PHRQ_malloc(sizeof(struct save_values));
	if (save_values == NULL)
		malloc_error();

	punch.inverse = TRUE;

	punch.sim = TRUE;
	punch.state = TRUE;
	punch.soln = TRUE;
	punch.dist = TRUE;
	punch.time = TRUE;
	punch.step = TRUE;
	punch.rxn = FALSE;
	punch.temp = FALSE;
	punch.ph = TRUE;
	punch.pe = TRUE;
	punch.alk = FALSE;
	punch.mu = FALSE;
	punch.water = FALSE;
	punch.high_precision = FALSE;
	punch.user_punch = TRUE;
	punch.charge_balance = FALSE;
	punch.percent_error = FALSE;
/*
 *   last model
 */
	last_model.force_prep = TRUE;
	last_model.temperature = -100;
	last_model.pressure = 0;
	last_model.count_exchange = -1;
	last_model.exchange = NULL;
	last_model.count_kinetics = -1;
	last_model.kinetics = NULL;
	last_model.count_gas_phase = -1;
	last_model.gas_phase = NULL;
	last_model.count_ss_assemblage = -1;
	last_model.ss_assemblage = NULL;
	last_model.count_pp_assemblage = -1;
	last_model.pp_assemblage = NULL;
	last_model.add_formula = NULL;
	last_model.si = NULL;
	last_model.dl_type = NO_DL;
	last_model.surface_type = UNKNOWN_DL;
	last_model.only_counter_ions = FALSE;
	last_model.thickness = 1e-8;
	last_model.count_surface_comp = -1;
	last_model.surface_comp = NULL;
	last_model.count_surface_charge = -1;
	last_model.surface_charge = NULL;
/*
 *   rates
 */
	rates = (struct rate *) PHRQ_malloc(sizeof(struct rate));
	if (rates == NULL)
		malloc_error();
	count_rates = 0;
	initial_total_time = 0;
	rate_m = 0;
	rate_m0 = 0;
	rate_p = NULL;
	rate_time = 0;
	rate_sim_time_start = 0;
	rate_sim_time_end = 0;
	rate_sim_time = 0;
	rate_moles = 0;
/*
 *   user_print, user_punch
 */
	user_print = (struct rate *) PHRQ_malloc((size_t) sizeof(struct rate));
	if (user_print == NULL)
		malloc_error();
	user_print->commands = NULL;
	user_print->linebase = NULL;
	user_print->varbase = NULL;
	user_print->loopbase = NULL;
	user_punch = (struct rate *) PHRQ_malloc((size_t) sizeof(struct rate));
	if (user_punch == NULL)
		malloc_error();
	user_punch->commands = NULL;
	user_punch->linebase = NULL;
	user_punch->varbase = NULL;
	user_punch->loopbase = NULL;
	user_punch_headings = (const char **) PHRQ_malloc(sizeof(char *));
	if (user_punch_headings == NULL)
		malloc_error();
	user_punch_count_headings = 0;
#if defined PHREEQ98 
/*
 *   user_graph
 */
	user_graph = (struct rate *) PHRQ_malloc((size_t) sizeof(struct rate));
	if (user_graph == NULL)
		malloc_error();
	user_graph->commands = NULL;
	user_graph->linebase = NULL;
	user_graph->varbase = NULL;
	user_graph->loopbase = NULL;
	user_graph_headings = (char **) PHRQ_malloc(sizeof(char *));
	if (user_graph_headings == NULL)
		malloc_error();
	user_graph_count_headings = 0;
#endif
	/*
	   Initialize llnl aqueous model parameters
	 */
	llnl_temp = (LDBLE *) PHRQ_malloc(sizeof(LDBLE));
	if (llnl_temp == NULL)
		malloc_error();
	llnl_count_temp = 0;
	llnl_adh = (LDBLE *) PHRQ_malloc(sizeof(LDBLE));
	if (llnl_adh == NULL)
		malloc_error();
	llnl_count_adh = 0;
	llnl_bdh = (LDBLE *) PHRQ_malloc(sizeof(LDBLE));
	if (llnl_bdh == NULL)
		malloc_error();
	llnl_count_bdh = 0;
	llnl_bdot = (LDBLE *) PHRQ_malloc(sizeof(LDBLE));
	if (llnl_bdot == NULL)
		malloc_error();
	llnl_count_bdot = 0;
	llnl_co2_coefs = (LDBLE *) PHRQ_malloc(sizeof(LDBLE));
	if (llnl_co2_coefs == NULL)
		malloc_error();
	llnl_count_co2_coefs = 0;
/*
 *
 */
	basic_interpreter = new PBasic(this, phrq_io);

	change_surf =
		(struct Change_Surf *)
		PHRQ_malloc((size_t) (2 * sizeof(struct Change_Surf)));
	if (change_surf == NULL)
		malloc_error();
	change_surf[0].cell_no = -99;
	change_surf[0].next = TRUE;
	change_surf[1].cell_no = -99;
	change_surf[1].next = FALSE;
	change_surf_count = 0;


#if defined(WINDOWS) || defined(_WINDOWS)
	/* SRC pr.status = FALSE; */
#endif
	/* Initialize print here, not in global.h */
	pr.all = TRUE;
	pr.initial_solutions = TRUE;
	pr.initial_exchangers = TRUE;
	pr.reactions = TRUE;
	pr.gas_phase = TRUE;
	pr.ss_assemblage = TRUE;
	pr.pp_assemblage = TRUE;
	pr.surface = TRUE;
	pr.exchange = TRUE;
	pr.kinetics = TRUE;
	pr.totals = TRUE;
	pr.eh = TRUE;
	pr.species = TRUE;
	pr.saturation_indices = TRUE;
	pr.irrev = TRUE;
	pr.mix = TRUE;
	pr.reaction = TRUE;
	pr.use = TRUE;
	pr.logfile = FALSE;
	pr.punch = TRUE;
	if (phast == TRUE)
	{
		pr.status = FALSE;
	}
	else
	{
		pr.status = TRUE;
	}
	pr.inverse = TRUE;
	pr.dump = TRUE;
	pr.user_print = TRUE;
	pr.headings = TRUE;
	pr.user_graph = TRUE;
	pr.echo_input = TRUE;
	count_warnings = 0;
	pr.warnings = 100;
	pr.initial_isotopes = TRUE;
	pr.isotope_ratios = TRUE;
	pr.isotope_alphas = TRUE;
	pr.hdf = FALSE;
	pr.alkalinity = FALSE;
	species_list = NULL;

	user_database = NULL;
	first_read_input = TRUE;
	have_punch_name = FALSE;
	selected_output_file_name = NULL;
	dump_file_name = NULL;

#ifdef PHREEQCI_GUI
	g_spread_sheet.heading = NULL;
	g_spread_sheet.units = NULL;
	g_spread_sheet.count_rows = 0;
	g_spread_sheet.rows = NULL;
	g_spread_sheet.defaults.units = NULL;
	g_spread_sheet.defaults.count_iso = 0;
	g_spread_sheet.defaults.iso = NULL;
#endif
	/* calculate_value */
	max_calculate_value = MAX_ELTS;
	count_calculate_value = 0;
	space((void **) ((void *) &calculate_value), INIT, &max_calculate_value,
		  sizeof(struct calculate_value *));
	hcreate_multi((unsigned) max_calculate_value,
				  &calculate_value_hash_table);

	/* isotope_ratio */
	max_isotope_ratio = MAX_ELTS;
	count_isotope_ratio = 0;
	space((void **) ((void *) &isotope_ratio), INIT, &max_isotope_ratio,
		  sizeof(struct isotope_ratio *));
	hcreate_multi((unsigned) max_isotope_ratio, &isotope_ratio_hash_table);

	/* isotope_value */
	max_isotope_alpha = MAX_ELTS;
	count_isotope_alpha = 0;
	space((void **) ((void *) &isotope_alpha), INIT, &max_isotope_alpha,
		  sizeof(struct isotope_alpha *));
	hcreate_multi((unsigned) max_isotope_alpha, &isotope_alpha_hash_table);

	/* 
	 * define constant named log_k
	 */
	strcpy(token, "XconstantX");
	logk_ptr = logk_store(token, TRUE);
	strcpy(token, "1.0");
	read_log_k_only(token, &logk_ptr->log_k[0]);

	phreeqc_mpi_myself = 0;

	copier_init(&copy_solution);
	copier_init(&copy_pp_assemblage);
	copier_init(&copy_exchange);
	copier_init(&copy_surface);
	copier_init(&copy_ss_assemblage);
	copier_init(&copy_gas_phase);
	copier_init(&copy_kinetics);
	copier_init(&copy_mix);
	copier_init(&copy_reaction);
	copier_init(&copy_temperature);
	copier_init(&copy_pressure);

	set_forward_output_to_log(FALSE);
	simulation = 0;
	/*
	 *  cvode
	 */

	cvode_init();
	/*
	 *  Pitzer
	 */
	pitzer_init();
	/*
	 *  SIT
	 */
	sit_init();
	/*
	 * to facilitate debuging
	 */
	//dbg_use = &use;
	dbg_solution = solution;
	//dbg_exchange = exchange;
	dbg_surface = surface;
	//dbg_pp_assemblage = pp_assemblage;
	dbg_kinetics = kinetics;
	//dbg_irrev = irrev;
	//dbg_mix = mix;
	dbg_master = master;
	calculating_deriv = FALSE;
	numerical_deriv = FALSE;

	zeros = (LDBLE *) PHRQ_malloc(sizeof(LDBLE));
	if (zeros == NULL)
		malloc_error();
	zeros[0] = 0.0;
	zeros_max = 1;

	cell_pore_volume = 0;
	cell_volume = 0;
	cell_porosity = 0;
	cell_saturation = 0;

	charge_group = NULL;
	print_density = 0;


	same_model = FALSE;
	same_temperature = FALSE;
	same_pressure = FALSE;
	g_iterations = -1;
	G_TOL = 1e-8;
	save_init(-1);
	count_species_list = 0;
	max_species_list = 0;
	count_sum_jacob0 = 0;
	max_sum_jacob0 = 0;
	count_sum_mb1 = 0;
	max_sum_mb1 = 0;
	count_sum_jacob1 = 0;
	max_sum_jacob1 = 0;
	count_sum_mb2 = 0;
	max_sum_mb2 = 0;
	count_sum_jacob2 = 0;
	max_sum_jacob2 = 0;
	count_sum_delta = 0;
	max_sum_delta = 0;
	new_x = FALSE;
	tc_x = 0;
	tk_x = 0;
	ph_x = 0;
	solution_pe_x = 0;
	mu_x = 0;
	ah2o_x = 0;
	density_x = 0;
	total_h_x = 0;
	total_o_x = 0;
	cb_x = 0;
	total_ions_x = 0;
	mass_water_aq_x = 0;
	mass_water_surfaces_x = 0;
	mass_water_bulk_x = 0;
	default_pe_x = 0;
	dl_type_x = NO_DL;
	total_carbon = 0;
	total_co2 = 0;
	total_alkalinity = 0;
	gfw_water = 0;
	step_x = 0;
	kin_time_x = 0;
	correct_disp = FALSE;
	cell = 0;
	multi_Dflag = FALSE;
	interlayer_Dflag = FALSE;
	default_Dw = 0;
	multi_Dpor = 0;
	interlayer_Dpor = 0.1;
	multi_Dpor_lim = 0;
	interlayer_Dpor_lim = 0;
	multi_Dn = 0;
	interlayer_tortf = 100;
	cell_no = 0;
	new_model = TRUE;
	new_exchange = FALSE;
	new_pp_assemblage = FALSE;
	new_surface = FALSE;
	new_reaction = FALSE;
	new_temperature = FALSE;
	new_mix = FALSE;
	new_solution = FALSE;
	new_gas_phase = FALSE;
	new_inverse = FALSE;
	new_punch = FALSE;
	new_ss_assemblage = FALSE;
	new_kinetics = FALSE;
	new_copy = FALSE;
	new_pitzer = FALSE;
	element_h_one = NULL;
	count_elts = 0;
	count_s_x = 0;
	max_s_x = 0;
	count_unknowns = 0;
	ah2o_unknown = NULL;
	alkalinity_unknown = NULL;
	carbon_unknown = NULL;
	charge_balance_unknown = NULL;
	exchange_unknown = NULL;
	mass_hydrogen_unknown = NULL;
	mass_oxygen_unknown = NULL;
	mb_unknown = NULL;
	mu_unknown = NULL;
	pe_unknown = NULL;
	ph_unknown = NULL;
	pure_phase_unknown = NULL;
	solution_phase_boundary_unknown = NULL;
	surface_unknown = NULL;
	gas_unknown = NULL;
	ss_unknown = NULL;
	for (i = 0; i < MAX_LOG_K_INDICES; i++)
	{
		trxn.logk[i] = 0;
	}
	for (i = 0; i < 3; i++)
	{
		trxn.dz[i] = 0;
	}
	count_trxn = 0;
	count_mb_unknowns = 0;
	status_on = TRUE;
	status_timer = (float) clock();
	status_interval = 0;
	count_rate_p = 0;
	//strcpy(error_string,"");
	reaction_step = 0;
	transport_step = 0;
	transport_start = 0;
	advection_step = 0;
	stop_program = FALSE;
	count_strings = 0;
	input_error = 0;
	parse_error = 0;
	paren_count = 0;
	iterations = 0;
	gamma_iterations = 0;
	run_reactions_iterations = 0;
	step_size_now = step_size;
	dampen_ah2o = false;
	slack = false;
	numerical_fixed_volume = false;
	force_numerical_fixed_volume = false;
	switch_numerical = false;
	pe_step_size_now = pe_step_size;
	count_total_steps = 0;
	remove_unstable_phases = FALSE;
	for (i = 0; i < 50; i++)
	{
		match_tokens[i].name = NULL;
		match_tokens[i].coef = 0;
	}
	count_match_tokens = 0;
	initial_solution_isotopes = FALSE;
	full_pitzer = FALSE;
	always_full_pitzer = FALSE;
	IC = -1;
	COSMOT = 0;
	AW = 0;
	sys = NULL;
	count_sys = 0;
	max_sys = 0;
	sys_tot = 0;
	AA_basic = 0;
	BB_basic = 0;
	CC = 0;
	I_m = 0;
	rho_0 = 0;
	solution_mass = 0;
	solution_volume = 0;

	patm_x = 1;   /* Initialize pressure of component x to 1 atm */

	/* model_min_value = 0; */

	return;
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
set_use(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Structure "use" has list of solution, ex, surf, pp_assemblage, 
 *   gas_phase and solid solution to use in current calculations, 
 *   also mix, irrev, and temp. 
 *   This routine searches for the user numbers in each list
 *   (solution, ex, ...) and sets a pointer in structure use
 */

/*
 *   Initial solution case
 */
	use.Set_pp_assemblage_ptr(NULL);
	use.Set_mix_ptr(NULL);
	use.Set_reaction_ptr(NULL);
	use.Set_exchange_ptr(NULL);
	use.Set_kinetics_ptr(NULL);
	use.Set_surface_ptr(NULL);
	use.Set_temperature_ptr(NULL);
	use.Set_pressure_ptr(NULL);
	use.Set_gas_phase_ptr(NULL);
	use.Set_ss_assemblage_ptr(NULL);

	if (state < REACTION)
	{
		return (OK);
	}
/*
 *   Reaction case
 */
	if (use.Get_pp_assemblage_in() == FALSE &&
		use.Get_reaction_in() == FALSE &&
		use.Get_mix_in() == FALSE &&
		use.Get_exchange_in() == FALSE &&
		use.Get_kinetics_in() == FALSE &&
		use.Get_surface_in() == FALSE &&
		use.Get_temperature_in() == FALSE &&
		use.Get_pressure_in() == FALSE &&
		use.Get_gas_phase_in() == FALSE && use.Get_ss_assemblage_in() == FALSE)
	{
		return (FALSE);
	}
	if (use.Get_solution_in() == FALSE && use.Get_mix_in() == FALSE)
		return (FALSE);
/*
 *   Find solution
 */
	if (use.Get_solution_in() == TRUE)
	{
		int n_solution;
		use.Set_solution_ptr(
			solution_bsearch(use.Get_n_solution_user(), &n_solution, FALSE));
		//use.Set_n_solution(n_solution);
		if (use.Get_solution_ptr() == NULL)
		{
			error_string = sformatf( "Solution %d not found.",
					use.Get_n_solution_user());
			error_msg(error_string, STOP);
		}
	}
/*
 *   Find mixture
 */
	if (use.Get_mix_in() == TRUE)
	{
		use.Set_mix_ptr(Utilities::Rxn_find(Rxn_mix_map, use.Get_n_mix_user()));
		//use.Get_mix_ptr() =(void *) Utilities::Rxn_find(Rxn_mix_map, use.Get_n_mix_user());
		use.Set_n_mix_user_orig(use.Get_n_mix_user());
		if (use.Get_mix_ptr() == NULL)
		{
			error_string = sformatf( "Mix %d not found.",
					use.Get_n_mix_user());
			error_msg(error_string, STOP);
		}
	}
	else
	{
		use.Set_mix_ptr(NULL);
	}
/*
 *   Find pure phase assemblage
 */
	if (use.Get_pp_assemblage_in() == TRUE)
	{
		use.Set_pp_assemblage_ptr(Utilities::Rxn_find(Rxn_pp_assemblage_map, use.Get_n_pp_assemblage_user()));
		//use.Get_pp_assemblage_ptr() =
		//	pp_assemblage_bsearch(use.Get_n_pp_assemblage_user(),
		//						  &use.n_pp_assemblage);
		if (use.Get_pp_assemblage_ptr() == NULL)
		{
			error_string = sformatf( "Pure phase assemblage %d not found.",
					use.Get_n_pp_assemblage_user());
			error_msg(error_string, STOP);
		}
	}
	else
	{
		use.Set_pp_assemblage_ptr(NULL);
	}
/*
 *   Find irrev reaction
 */
	if (use.Get_reaction_in() == TRUE)
	{
		use.Set_reaction_ptr(Utilities::Rxn_find(Rxn_reaction_map, use.Get_n_reaction_user()));
		if (use.Get_reaction_ptr() == NULL)
		{
			error_string = sformatf( "Reaction %d not found.",
					use.Get_n_reaction_user());
			error_msg(error_string, STOP);
		}
	}
	else
	{
		use.Set_reaction_ptr(NULL);
	}
/*
 *   Find exchange
 */
	if (use.Get_exchange_in() == TRUE)
	{
		use.Set_exchange_ptr(Utilities::Rxn_find(Rxn_exchange_map, use.Get_n_exchange_user()));
		if (use.Get_exchange_ptr() == NULL)
		{
			error_string = sformatf( "Exchange %d not found.",
					use.Get_n_exchange_user());
			error_msg(error_string, STOP);
		}
	}
	else
	{
		use.Set_exchange_ptr(NULL);
	}
/*
 *   Find kinetics
 */
	if (use.Get_kinetics_in() == TRUE)
	{
		int n_kinetics;
		use.Set_kinetics_ptr(
			kinetics_bsearch(use.Get_n_kinetics_user(), &n_kinetics));
		//use.Set_n_kinetics(n_kinetics);
		if (use.Get_kinetics_ptr() == NULL)
		{
			error_string = sformatf( "Kinetics %d not found.",
					use.Get_n_kinetics_user());
			error_msg(error_string, STOP);
		}
	}
	else
	{
		use.Set_kinetics_ptr(NULL);
	}
/*
 *   Find surface
 */
	dl_type_x = NO_DL;
	if (use.Get_surface_in() == TRUE)
	{
		int n_surface;
		use.Set_surface_ptr(surface_bsearch(use.Get_n_surface_user(), &n_surface));
		//use.Set_n_surface(n_surface);
		if (use.Get_surface_ptr() == NULL)
		{
			error_string = sformatf( "Surface %d not found.",
					use.Get_n_surface_user());
			error_msg(error_string, STOP);
		}
	}
	else
	{
		use.Set_surface_ptr(NULL);
	}
/*
 *   Find temperature
 */
	if (use.Get_temperature_in() == TRUE)
	{
		use.Set_temperature_ptr(Utilities::Rxn_find(Rxn_temperature_map, use.Get_n_temperature_user()));
		if (use.Get_temperature_ptr() == NULL)
		{
			error_string = sformatf( "Temperature %d not found.",
					use.Get_n_temperature_user());
			error_msg(error_string, STOP);
		}
	}
	else
	{
		use.Set_temperature_ptr(NULL);
	}
/*
 *   Find pressure
 */
	if (use.Get_pressure_in() == TRUE)
	{
		use.Set_pressure_ptr(Utilities::Rxn_find(Rxn_pressure_map, use.Get_n_pressure_user()));
		if (use.Get_pressure_ptr() == NULL)
		{
			error_string = sformatf( "Pressure %d not found.",	use.Get_n_pressure_user());
			error_msg(error_string, STOP);
		}
	}
	else
	{
		use.Set_pressure_ptr(NULL);
	}
/*
 *   Find gas
 */
	if (use.Get_gas_phase_in() == TRUE)
	{
		//use.Get_gas_phase_ptr() =	gas_phase_bsearch(use.Get_n_gas_phase_user(), &use.n_gas_phase);
		use.Set_gas_phase_ptr(Utilities::Rxn_find(Rxn_gas_phase_map, use.Get_n_gas_phase_user()));
		if (use.Get_gas_phase_ptr() == NULL)
		{
			error_string = sformatf( "Gas_phase %d not found.",
					use.Get_n_gas_phase_user());
			error_msg(error_string, STOP);
		}
	}
	else
	{
		use.Set_gas_phase_ptr(NULL);
	}
/*
 *   Find ss_assemblage
 */
	if (use.Get_ss_assemblage_in() == TRUE)
	{
		//int n_ss_assemblage_user;
		//use.Set_ss_assemblage_ptr(
		//	ss_assemblage_bsearch(use.Get_n_ss_assemblage_user(),
		//						   &n_ss_assemblage_user));
		use.Set_ss_assemblage_ptr(Utilities::Rxn_find(Rxn_ss_assemblage_map, use.Get_n_ss_assemblage_user()));
		if (use.Get_ss_assemblage_ptr() == NULL)
		{
			error_string = sformatf( "ss_assemblage %d not found.",
					use.Get_n_ss_assemblage_user());
			error_msg(error_string, STOP);
		}
	}
	else
	{
		use.Set_ss_assemblage_ptr(NULL);
	}
	/*
	if (use.irrev_ptr != NULL && use.Get_kinetics_ptr() != NULL)
	{
		warning_msg("Should not use REACTION in same simulation with KINETICS.");
	}
	*/
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
initial_solutions(int print)
/* ---------------------------------------------------------------------- */
{
/*
 *   Go through list of solutions, make initial solution calculations
 *   for any marked "new".
 */
	int i, converge, converge1;
	int n, last, n_user, print1;
	char token[2 * MAX_LENGTH];

	state = INITIAL_SOLUTION;
	set_use();
	print1 = TRUE;
	dl_type_x = NO_DL;
	for (n = 0; n < count_solution; n++)
	{
		initial_solution_isotopes = FALSE;
		if (solution[n] != NULL && solution[n]->new_def == TRUE)
		{
			if (print1 == TRUE && print == TRUE)
			{
				dup_print("Beginning of initial solution calculations.",
						  TRUE);
				print1 = FALSE;
			}
			if (print == TRUE)
			{
				sprintf(token, "Initial solution %d.\t%.350s",
						solution[n]->n_user, solution[n]->description);
				dup_print(token, FALSE);
			}
			use.Set_solution_ptr(solution[n]);
			prep();
			k_temp(solution[n]->tc, solution[n]->patm);
			set(TRUE);
			always_full_pitzer = FALSE;
			converge = model();
			if (converge == ERROR && diagonal_scale == FALSE)
			{
				diagonal_scale = TRUE;
				always_full_pitzer = TRUE;
				set(TRUE);
				converge = model();
				diagonal_scale = FALSE;
			}
			converge1 = check_residuals();
			sum_species();
			add_isotopes(solution[n]);
			punch_all();
			print_all();
			/* free_model_allocs(); */
			if (converge == ERROR || converge1 == ERROR)
			{
				error_msg("Model failed to converge for initial solution.",
						  STOP);
			}
			n_user = solution[n]->n_user;
			last = solution[n]->n_user_end;
			/* copy isotope data */
			if (solution[n]->count_isotopes > 0)
			{
				count_isotopes_x = solution[n]->count_isotopes;
				isotopes_x =
					(struct isotope *) PHRQ_realloc(isotopes_x,
													(size_t) count_isotopes_x
													* sizeof(struct isotope));
				if (isotopes_x == NULL)
					malloc_error();
				memcpy(isotopes_x, solution[n]->isotopes,
					   (size_t) count_isotopes_x * sizeof(struct isotope));
			}
			else
			{
				count_isotopes_x = 0;
			}
			xsolution_save(n_user);
			for (i = n_user + 1; i <= last; i++)
			{
				solution_duplicate(n_user, i);
			}
		}
	}
	initial_solution_isotopes = FALSE;
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
initial_exchangers(int print)
/* ---------------------------------------------------------------------- */
{
/*
 *   Go through list of exchangers, make initial calculations
 *   for any marked "new" that are defined to be in equilibrium with a 
 *   solution.
 */
	int i, converge, converge1;
	int last, n_user, print1;
	char token[2 * MAX_LENGTH];

	state = INITIAL_EXCHANGE;
	set_use();
	print1 = TRUE;
	dl_type_x = NO_DL;
	std::map<int, cxxExchange>::iterator it = Rxn_exchange_map.begin();
	for ( ; it != Rxn_exchange_map.end(); it++)
	{
		if (!it->second.Get_new_def())
			continue;
		cxxExchange *exchange_ptr = &(it->second);
		n_user = exchange_ptr->Get_n_user();
		last = exchange_ptr->Get_n_user_end();
		exchange_ptr->Set_n_user_end(n_user);
		exchange_ptr->Set_new_def(false);
		if (exchange_ptr->Get_solution_equilibria())
		{
			if (print1 == TRUE && print == TRUE)
			{
				dup_print("Beginning of initial exchange"
						  "-composition calculations.", TRUE);
				print1 = FALSE;
			}
			if (print == TRUE)
			{
				sprintf(token, "Exchange %d.\t%.350s",
						exchange_ptr->Get_n_user(), exchange_ptr->Get_description().c_str());
				dup_print(token, FALSE);
			}
			use.Set_exchange_ptr(exchange_ptr);
			use.Set_solution_ptr(solution_bsearch(exchange_ptr->Get_n_solution(), &i, TRUE));
			if (use.Get_solution_ptr() == NULL)
			{
				error_msg
					("Solution not found for initial exchange calculation",
					 STOP);
			}

			prep();
			k_temp(use.Get_solution_ptr()->tc, use.Get_solution_ptr()->patm);
			set(TRUE);
			converge = model();
			converge1 = check_residuals();
			sum_species();
			species_list_sort();
			print_exchange();
			xexchange_save(n_user);
			punch_all();
			/* free_model_allocs(); */
			if (converge == ERROR || converge1 == ERROR)
			{
				error_msg
					("Model failed to converge for initial exchange calculation.",
					 STOP);
			}
		}
		for (i = n_user + 1; i <= last; i++)
		{
			//exchange_duplicate(n_user, i);
			Utilities::Rxn_copy(Rxn_exchange_map, n_user, i);
		}
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
initial_gas_phases(int print)
/* ---------------------------------------------------------------------- */
{
/*
 *   Go through list of gas_phases, make initial calculations
 *   for any marked "new" that are defined to be in equilibrium with a 
 *   solution.
 */
	int i, converge, converge1;
	int last, n_user, print1;
	char token[2 * MAX_LENGTH];
	struct phase *phase_ptr;
	struct rxn_token *rxn_ptr;
	LDBLE lp;
	bool PR = false;

	state = INITIAL_GAS_PHASE;
	set_use();
	print1 = TRUE;
	dl_type_x = NO_DL;
	std::map<int, cxxGasPhase>::iterator it = Rxn_gas_phase_map.begin();
	for ( ; it != Rxn_gas_phase_map.end(); it++)
	//for (n = 0; n < count_gas_phase; n++)
	{
		cxxGasPhase *gas_phase_ptr = &it->second;
		if (!gas_phase_ptr->Get_new_def())
			continue;
		n_user = gas_phase_ptr->Get_n_user();
		last = gas_phase_ptr->Get_n_user_end();
		gas_phase_ptr->Set_n_user_end(n_user);
		gas_phase_ptr->Set_new_def(false);
		if (gas_phase_ptr->Get_solution_equilibria())
		{
			if (print1 == TRUE && print == TRUE)
			{
				dup_print("Beginning of initial gas_phase"
						  "-composition calculations.", TRUE);
				print1 = FALSE;
			}
			if (print == TRUE)
			{
				sprintf(token, "Gas_Phase %d.\t%.350s",
						gas_phase_ptr->Get_n_user(), gas_phase_ptr->Get_description().c_str());
				dup_print(token, FALSE);
			}

			/* Try to obtain a solution pointer */ 
			use.Set_solution_ptr(
				solution_bsearch(gas_phase_ptr->Get_n_solution(), &i, TRUE));
			prep();
			k_temp(use.Get_solution_ptr()->tc, use.Get_solution_ptr()->patm);
			set(TRUE);
			converge = model();
			converge1 = check_residuals();
			if (converge == ERROR || converge1 == ERROR)
			{
				/* free_model_allocs(); */
				error_msg
					("Model failed to converge for initial gas phase calculation.",
					 STOP);
			}
			use.Set_gas_phase_ptr(gas_phase_ptr);
			gas_phase_ptr->Set_total_p(0);
			gas_phase_ptr->Set_total_moles(0);
			for (size_t i = 0; i < gas_phase_ptr->Get_gas_comps().size(); i++)
			//for (i = 0; i < use.Get_gas_phase_ptr()->count_comps; i++)
			{
				cxxGasComp * gc_ptr = &(gas_phase_ptr->Get_gas_comps()[i]);
				//gas_comp_ptr = &(use.Get_gas_phase_ptr()->comps[i]);
				//phase_ptr = gas_comp_ptr->phase;
				int k;
				phase_ptr = phase_bsearch(gc_ptr->Get_phase_name().c_str(), &k, FALSE);
				if (phase_ptr->in == TRUE)
				{
					lp = -phase_ptr->lk;
					for (rxn_ptr = phase_ptr->rxn_x->token + 1;
						 rxn_ptr->s != NULL; rxn_ptr++)
					{
						lp += rxn_ptr->s->la * rxn_ptr->coef;
					}
					phase_ptr->p_soln_x = exp(lp * LOG_10);
					gas_phase_ptr->Set_total_p(gas_phase_ptr->Get_total_p() + phase_ptr->p_soln_x);
					phase_ptr->moles_x = phase_ptr->p_soln_x *
						gas_phase_ptr->Get_volume() / (R_LITER_ATM * tk_x);
					gc_ptr->Set_moles(phase_ptr->moles_x);
					gas_phase_ptr->Set_total_moles(gas_phase_ptr->Get_total_moles() + phase_ptr->moles_x);
					if (phase_ptr->p_c || phase_ptr->t_c)
						PR = true;
				}
				else
				{
					phase_ptr->moles_x = 0;
				}
			}
			if (fabs(gas_phase_ptr->Get_total_p() - use.Get_solution_ptr()->patm) > 5)
			{
				sprintf(token, 
					"WARNING: While initializing gas phase composition by equilibrating:\n%s (%.2f atm) %s (%.2f atm).\n%s.",
					"         Gas phase pressure",
					(double) gas_phase_ptr->Get_total_p(),
					"is not equal to solution-pressure",
					(double) use.Get_solution_ptr()->patm,
					"         Pressure effects on solubility may be incorrect");
					dup_print(token, FALSE);
			}

			print_gas_phase();
 			if (PR /*&& use.Get_gas_phase_ptr()->total_p > 1.0*/)
 				warning_msg("While initializing gas phase composition by equilibrating:\n"
				"         Found definitions of gas' critical temperature and pressure.\n"
				"         Going to use Peng-Robinson in subsequent calculations.\n");
			xgas_save(n_user);
			punch_all();
			/* free_model_allocs(); */
		}
		Utilities::Rxn_copies(Rxn_gas_phase_map, n_user, last);
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
initial_surfaces(int print)
/* ---------------------------------------------------------------------- */
{
/*
 *   Go through list of surfaces, make initial calculations
 *   for any marked "new" that are defined to be in equilibrium with a 
 *   solution.
 */
	int i;
	int n, last, n_user, print1;
	char token[2 * MAX_LENGTH];

	state = INITIAL_SURFACE;
	set_use();
	print1 = TRUE;
	for (n = 0; n < count_surface; n++)
	{
		if (surface[n].new_def != TRUE)
			continue;
		n_user = surface[n].n_user;
		last = surface[n].n_user_end;
		surface[n].n_user_end = n_user;
		if (surface[n].solution_equilibria == TRUE)
		{
			if (print1 == TRUE && print == TRUE)
			{
				dup_print
					("Beginning of initial surface-composition calculations.",
					 TRUE);
				print1 = FALSE;
			}
			if (print == TRUE)
			{
				sprintf(token, "Surface %d.\t%.350s",
						surface[n].n_user, surface[n].description);
				dup_print(token, FALSE);
			}
			use.Set_surface_ptr(&(surface[n]));
			dl_type_x = use.Get_surface_ptr()->dl_type;
			use.Set_solution_ptr(
				solution_bsearch(surface[n].n_solution, &i, TRUE));
			if (use.Get_solution_ptr() == NULL)
			{
				error_msg
					("Solution not found for initial surface calculation",
					 STOP);
			}
			set_and_run_wrapper(-1, FALSE, FALSE, -1, 0.0);
			species_list_sort();
			print_surface();
			/*print_all(); */
			punch_all();
			xsurface_save(n_user);
			/* free_model_allocs(); */
		}
		for (i = n_user + 1; i <= last; i++)
		{
			surface_duplicate(n_user, i);
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
reactions(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Make all reaction calculation which could include:
 *      equilibrium with a pure-phase assemblage,
 *      equilibrium with an exchanger,
 *      equilibrium with an surface,
 *      equilibrium with a gas phase,
 *      equilibrium with a solid solution assemblage,
 *      kinetics,
 *      change of temperature,
 *      mixture,
 *      or irreversible reaction.
 */
	int count_steps, use_mix, m;
	char token[2 * MAX_LENGTH];
	struct save save_data;
	LDBLE kin_time;
	struct kinetics *kinetics_ptr;

	state = REACTION;
	/* last_model.force_prep = TRUE; */
	if (set_use() == FALSE)
		return (OK);
/*
 *   Find maximum number of steps
 */
	dup_print("Beginning of batch-reaction calculations.", TRUE);
	count_steps = 1;
	if (use.Get_reaction_in() == TRUE && use.Get_reaction_ptr() != NULL)
	{
		cxxReaction *reaction_ptr = (cxxReaction *) use.Get_reaction_ptr();
		if (reaction_ptr->Get_actualSteps() > count_steps)
			count_steps = reaction_ptr->Get_actualSteps();
	}
	if (use.Get_kinetics_in() == TRUE && use.Get_kinetics_ptr() != NULL)
	{
		if (abs(use.Get_kinetics_ptr()->count_steps) > count_steps)
			count_steps = abs(use.Get_kinetics_ptr()->count_steps);
	}
	if (use.Get_temperature_in() == TRUE && use.Get_temperature_ptr() != NULL)
	{
		int count = ((cxxTemperature *) use.Get_temperature_ptr())->Get_countTemps();
		if (count > count_steps)
		{
			count_steps = count;
		}
	}
	if (use.Get_pressure_in() == TRUE && use.Get_pressure_ptr() != NULL)
	{
		int count = ((cxxPressure *) use.Get_pressure_ptr())->Get_count();
		if (count > count_steps)
		{
			count_steps = count;
		}
	}
	count_total_steps = count_steps;
/*
 *  save data for saving solutions
 */
	memcpy(&save_data, &save, sizeof(struct save));
	/* 
	 *Copy everything to -2
	 */
	copy_use(-2);
	rate_sim_time_start = 0;
	rate_sim_time = 0;
	for (reaction_step = 1; reaction_step <= count_steps; reaction_step++)
	{
		sprintf(token, "Reaction step %d.", reaction_step);
		if (reaction_step > 1 && incremental_reactions == FALSE)
		{
			copy_use(-2);
		}
		set_initial_moles(-2);
		dup_print(token, FALSE);
/*
 *  Determine time step for kinetics
 */
		kin_time = 0.0;
		if (use.Get_kinetics_in() == TRUE)
		{
			kinetics_ptr = kinetics_bsearch(-2, &m);
			if (incremental_reactions == FALSE)
			{
				if (kinetics_ptr->count_steps > 0)
				{
					if (reaction_step > kinetics_ptr->count_steps)
					{
						kin_time =
							kinetics_ptr->steps[kinetics_ptr->count_steps -
												1];
					}
					else
					{
						kin_time = kinetics_ptr->steps[reaction_step - 1];
					}
				}
				else if (kinetics_ptr->count_steps < 0)
				{
					if (reaction_step > -kinetics_ptr->count_steps)
					{
						kin_time = kinetics_ptr->steps[0];
					}
					else
					{
						kin_time =
							reaction_step * kinetics_ptr->steps[0] /
							((LDBLE) (-kinetics_ptr->count_steps));
					}
				}
			}
			else
			{
				/* incremental reactions */
				if (kinetics_ptr->count_steps > 0)
				{
					if (reaction_step > kinetics_ptr->count_steps)
					{
						kin_time =
							kinetics_ptr->steps[kinetics_ptr->count_steps -
												1];
					}
					else
					{
						kin_time = kinetics_ptr->steps[reaction_step - 1];
					}
				}
				else if (kinetics_ptr->count_steps < 0)
				{
					if (reaction_step > -kinetics_ptr->count_steps)
					{
						kin_time = 0;
					}
					else
					{
						kin_time =
							kinetics_ptr->steps[0] /
							((LDBLE) (-kinetics_ptr->count_steps));
					}
				}
			}
		}
		if (incremental_reactions == FALSE ||
			(incremental_reactions == TRUE && reaction_step == 1))
		{
			use_mix = TRUE;
		}
		else
		{
			use_mix = FALSE;
		}
/*
 *   Run reaction step
 */
		run_reactions(-2, kin_time, use_mix, 1.0);
		if (incremental_reactions == TRUE)
		{
			rate_sim_time_start += kin_time;
			rate_sim_time = rate_sim_time_start;
		}
		else
		{
			rate_sim_time = kin_time;
		}
		if (state != ADVECTION)
		{
			punch_all();
			print_all();
		}
		/* saves back into -2 */
		if (reaction_step < count_steps)
		{
			saver();
		}
	}
/*
 *   save end of reaction
 */
	memcpy(&save, &save_data, sizeof(struct save));
	if (use.Get_kinetics_in() == TRUE)
	{
		kinetics_duplicate(-2, use.Get_n_kinetics_user());
	}
	saver();

	/* free_model_allocs(); */
	/* last_model.force_prep = TRUE; */
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
saver(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Save results of calcuations (data in variables with _x,
 *   in unknown structure x, in master, or s) into structure 
 *   arrays.  Structure "save" has info on whether to save
 *   data for each entity (solution, ex, surf, pp, gas, or s_s).
 *   Initial calculation may be saved into multiple "n_user"
 *   slots.
 */
	int i, n;
	char token[MAX_LENGTH];

	if (save.solution == TRUE)
	{
		sprintf(token, "Solution after simulation %d.", simulation);
		description_x = (char *) free_check_null(description_x);
		description_x = string_duplicate(token);
		n = save.n_solution_user;
		xsolution_save(n);
		for (i = save.n_solution_user + 1; i <= save.n_solution_user_end; i++)
		{
			solution_duplicate(n, i);
		}
	}
	if (save.pp_assemblage == TRUE)
	{
		n = save.n_pp_assemblage_user;
		xpp_assemblage_save(n);
		Utilities::Rxn_copies(Rxn_pp_assemblage_map, save.n_pp_assemblage_user, save.n_pp_assemblage_user_end);
		//for (i = save.n_pp_assemblage_user + 1;
		//	 i <= save.n_pp_assemblage_user_end; i++)
		//{
		//	pp_assemblage_duplicate(n, i);
		//}
	}
	if (save.exchange == TRUE)
	{
		n = save.n_exchange_user;
		xexchange_save(n);
		for (i = save.n_exchange_user + 1; i <= save.n_exchange_user_end; i++)
		{
			//exchange_duplicate(n, i);
			Utilities::Rxn_copy(Rxn_exchange_map, n, i);
		}
	}
	if (save.surface == TRUE)
	{
		n = save.n_surface_user;
		xsurface_save(n);
		for (i = save.n_surface_user + 1; i <= save.n_surface_user_end; i++)
		{
			surface_duplicate(n, i);
		}
	}
	if (save.gas_phase == TRUE)
	{
		n = save.n_gas_phase_user;
		xgas_save(n);
		for (i = save.n_gas_phase_user + 1; i <= save.n_gas_phase_user_end;
			 i++)
		{
			//gas_phase_duplicate(n, i);
			Utilities::Rxn_copy(Rxn_gas_phase_map, n, i);
		}
	}
	if (save.ss_assemblage == TRUE)
	{
		n = save.n_ss_assemblage_user;
		xss_assemblage_save(n);
		Utilities::Rxn_copies(Rxn_ss_assemblage_map, save.n_ss_assemblage_user, save.n_ss_assemblage_user_end);
		//for (i = save.n_ss_assemblage_user + 1;
		//	 i <= save.n_ss_assemblage_user_end; i++)
		//{
		//	ss_assemblage_duplicate(n, i);
		//}
	}
	if (save.kinetics == TRUE && use.Get_kinetics_in() == TRUE
	    /*&& use.Get_kinetics_ptr() != NULL */)
	{
		if (state == TRANSPORT || state == PHAST || state == ADVECTION)
		{
			use.Set_kinetics_ptr(kinetics_bsearch(use.Get_n_kinetics_user(), &i));
		}
		else if (use.Get_kinetics_in() != FALSE)
		{
			use.Set_kinetics_ptr(kinetics_bsearch(-2, &i));
		}
		if (use.Get_kinetics_ptr() != NULL)
		{
			n = use.Get_kinetics_ptr()->n_user;
			for (i = save.n_kinetics_user; i <= save.n_kinetics_user_end; i++)
			{
				kinetics_duplicate(n, i);
			}
		}
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
xexchange_save(int n_user)
/* ---------------------------------------------------------------------- */
{
/*
 *   Save exchanger assemblage into structure exchange with user 
 *   number n_user.
 */
	int i, j;
	char token[MAX_LENGTH];
	int count_comps;
	//struct exchange temp_exchange, *exchange_ptr;

	LDBLE charge;
	if (use.Get_exchange_ptr() == NULL)
		return (OK);

	cxxExchange temp_exchange = *((cxxExchange *) use.Get_exchange_ptr());
/*
 *   Store data for structure exchange
 */
	temp_exchange.Set_n_user(n_user);
	temp_exchange.Set_n_user_end(n_user);
	temp_exchange.Set_new_def(false);
	sprintf(token, "Exchange assemblage after simulation %d.", simulation);
	temp_exchange.Set_description(token);
	temp_exchange.Set_solution_equilibria(false);
	temp_exchange.Set_n_solution(-999);
	temp_exchange.Get_exchComps().clear();

/*
 *   Write exch_comp structure for each exchange component
 */
	count_comps = 0;
	for (i = 0; i < count_unknowns; i++)
	{
		if (x[i]->type == EXCH)
		{
			const cxxExchComp *comp_ptr = ((cxxExchange *) use.Get_exchange_ptr())->ExchComp_find(x[i]->exch_comp);
			if (!comp_ptr)
			{
				assert(false);
			}
			cxxExchComp xcomp = *comp_ptr;
			xcomp.Set_la(x[i]->master[0]->s->la);
			xcomp.Set_moles(0);
/*
 *   Save element concentrations on exchanger
 */
			count_elts = 0;
			paren_count = 0;
			charge = 0.0;
			for (j = 0; j < count_species_list; j++)
			{
				if (species_list[j].master_s == x[i]->master[0]->s)
				{
					add_elt_list(species_list[j].s->next_elt,
								 species_list[j].s->moles);
					charge += species_list[j].s->moles * species_list[j].s->z;
				}
			}
/*
 *   Keep exchanger related to phase even if none currently in solution
 */
			if (xcomp.Get_phase_name().size() != 0 && count_elts == 0)
			{
				add_elt_list(x[i]->master[0]->s->next_elt, 1e-20);
			}
/*
 *   Store list
 */
			xcomp.Set_charge_balance(charge);

			xcomp.Set_totals(elt_list_NameDouble());
/* debug
                        output_msg(sformatf( "Exchange charge_balance: %e\n", charge));
 */
			/* update unknown pointer */
			//x[i]->exch_comp = &(temp_exchange.comps[count_comps]);
			temp_exchange.Get_exchComps()[x[i]->exch_comp] = xcomp;
		}
	}
/*
 *   Finish up
 */
	Rxn_exchange_map[n_user] = temp_exchange; 

	use.Set_exchange_ptr(NULL);
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
xgas_save(int n_user)
/* ---------------------------------------------------------------------- */
{
/*
 *   Save gas composition into structure gas_phase with user 
 *   number n_user.
 */
	//int count_comps, n, i;
	//struct gas_phase temp_gas_phase, *gas_phase_ptr;
	char token[MAX_LENGTH];

	if (use.Get_gas_phase_ptr() == NULL)
		return (OK);
	cxxGasPhase *gas_phase_ptr = (cxxGasPhase *) use.Get_gas_phase_ptr();
	cxxGasPhase temp_gas_phase(*gas_phase_ptr);
/*
 *   Count gases
 */
	//size_t count_comps = gas_phase_ptr->Get_gasPhaseComps().size();
/*
 *   Store in gas_phase
 */

	temp_gas_phase.Set_n_user(n_user);
	temp_gas_phase.Set_n_user_end(n_user);
	sprintf(token, "Gas phase after simulation %d.", simulation);
	temp_gas_phase.Set_description(token);
	temp_gas_phase.Set_new_def(false);
	temp_gas_phase.Set_solution_equilibria(false);
	temp_gas_phase.Set_n_solution(-99);
/*
 *   Update amounts
 */
	for (size_t i = 0 ; i < temp_gas_phase.Get_gas_comps().size(); i++)
	{
		cxxGasComp * gc_ptr = &(temp_gas_phase.Get_gas_comps()[i]);
		int k;
		struct phase *phase_ptr = phase_bsearch(gc_ptr->Get_phase_name().c_str(), &k, FALSE);
		assert(phase_ptr);
		gc_ptr->Set_moles(phase_ptr->moles_x);
	}
	Rxn_gas_phase_map[n_user] = temp_gas_phase;

	use.Set_gas_phase_ptr(NULL);
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
xss_assemblage_save(int n_user)
/* ---------------------------------------------------------------------- */
{
/*
 *   Save ss_assemblage composition into structure ss_assemblage with user 
 *   number n_user.
 */
	//int i, j, n;
	//int count_comps, count_s_s;
	cxxSSassemblage temp_ss_assemblage;
	//char token[MAX_LENGTH];

	if (use.Get_ss_assemblage_ptr() == NULL)
		return (OK);
/*
 *   Set ss_assemblage
 */
	temp_ss_assemblage.Set_n_user(n_user);
	temp_ss_assemblage.Set_n_user_end(n_user);
	std::ostringstream msg;
	msg << "Solid solution assemblage after simulation " << simulation;
	temp_ss_assemblage.Set_description(msg.str().c_str());
	temp_ss_assemblage.Set_new_def(false);
	temp_ss_assemblage.Set_SSs(use.Get_ss_assemblage_ptr()->Get_SSs());

	std::vector<cxxSS *> ss_ptrs = temp_ss_assemblage.Vectorize();
	for (size_t i = 0; i < ss_ptrs.size(); i++)
	{
		cxxSS *ss_ptr = ss_ptrs[i];
		/* set initial moles for quick setup */
		for (size_t j = 0; j < ss_ptr->Get_ss_comps().size(); j++)
		{
			cxxSScomp *comp_ptr = &(ss_ptr->Get_ss_comps()[j]);
			comp_ptr->Set_initial_moles(comp_ptr->Get_moles());
		}
	}
/*
 *   Finish up
 */
	Rxn_ss_assemblage_map[n_user] = temp_ss_assemblage;

	use.Set_ss_assemblage_ptr(NULL);
	return (OK);
}
#ifdef SKIP
/* ---------------------------------------------------------------------- */
int Phreeqc::
xss_assemblage_save(int n_user)
/* ---------------------------------------------------------------------- */
{
/*
 *   Save ss_assemblage composition into structure ss_assemblage with user 
 *   number n_user.
 */
	int i, j, n;
	int count_comps, count_s_s;
	struct ss_assemblage temp_ss_assemblage, *ss_assemblage_ptr;
	char token[MAX_LENGTH];

	if (use.Get_ss_assemblage_ptr() == NULL)
		return (OK);
/*
 *   Set ss_assemblage
 */
	temp_ss_assemblage.n_user = n_user;
	temp_ss_assemblage.n_user_end = n_user;
	sprintf(token, "Solid solution assemblage after simulation %d.",
			simulation);
	temp_ss_assemblage.description = string_duplicate(token);
	temp_ss_assemblage.new_def = FALSE;
	count_s_s = use.Get_ss_assemblage_ptr()->count_s_s;
	temp_ss_assemblage.count_s_s = count_s_s;
/*
 *   Malloc space for solid solutions
 */
	/* ss_assemblage->s_s */
	temp_ss_assemblage.s_s =
		(struct s_s *) PHRQ_malloc((size_t) count_s_s * sizeof(struct s_s));
	if (temp_ss_assemblage.s_s == NULL)
		malloc_error();
	for (i = 0; i < count_s_s; i++)
	{
		memcpy(&(temp_ss_assemblage.s_s[i]),
			   &(use.Get_ss_assemblage_ptr()->s_s[i]), sizeof(struct s_s));
		/* 
		 * Malloc space for solid soution components
		 */
		count_comps = use.Get_ss_assemblage_ptr()->s_s[i].count_comps;
		temp_ss_assemblage.s_s[i].comps =
			(struct s_s_comp *) PHRQ_malloc((size_t) count_comps *
											sizeof(struct s_s_comp));
		if (temp_ss_assemblage.s_s[i].comps == NULL)
			malloc_error();
		memcpy((void *) temp_ss_assemblage.s_s[i].comps,
			   (void *) use.Get_ss_assemblage_ptr()->s_s[i].comps,
			   (size_t) count_comps * sizeof(struct s_s_comp));

		/* set initial moles for quick setup */
		for (j = 0; j < count_comps; j++)
		{
			temp_ss_assemblage.s_s[i].comps[j].initial_moles =
				temp_ss_assemblage.s_s[i].comps[j].moles;
		}
	}
/*
 *   Finish up
 */
	ss_assemblage_ptr = ss_assemblage_bsearch(n_user, &n);
	if (ss_assemblage_ptr == NULL)
	{
		space((void **) ((void *) &ss_assemblage), count_ss_assemblage,
			  &max_ss_assemblage, sizeof(struct ss_assemblage));
		n = count_ss_assemblage++;
	}
	else
	{
		ss_assemblage_free(&ss_assemblage[n]);
	}
	memcpy(&ss_assemblage[n], &temp_ss_assemblage,
		   sizeof(struct ss_assemblage));
	/* sort only if necessary */
	if (n == count_ss_assemblage - 1 && count_ss_assemblage > 1)
	{
		if (ss_assemblage[n].n_user < ss_assemblage[n - 1].n_user)
		{
			qsort(ss_assemblage,
				  (size_t) count_ss_assemblage,
				  (size_t) sizeof(struct ss_assemblage),
				  ss_assemblage_compare);
		}
	}
	use.Set_ss_assemblage_ptr(NULL);
	return (OK);
}
#endif
/* ---------------------------------------------------------------------- */
int Phreeqc::
xpp_assemblage_save(int n_user)
/* ---------------------------------------------------------------------- */
{
/*
 *   Save pure_phase assemblage into instance of cxxPPassemblage with user 
 *   number n_user.
 */
	std::string token;
	cxxPPassemblage * pp_assemblage_ptr = (cxxPPassemblage *) use.Get_pp_assemblage_ptr();
	if (use.Get_pp_assemblage_ptr() == NULL)
		return (OK);

	cxxPPassemblage temp_pp_assemblage(*pp_assemblage_ptr);

	temp_pp_assemblage.Set_n_user(n_user);
	temp_pp_assemblage.Set_n_user_end(n_user);
	std::ostringstream desc;
	desc << "Pure-phase assemblage after simulation " << simulation << ".";
	temp_pp_assemblage.Set_description(desc.str().c_str());
	temp_pp_assemblage.Set_new_def(false);
/*
 *   Update amounts
 */
	for (int j = 0; j < count_unknowns; j++)
	{
		if (x[j]->type != PP)
			continue;
		cxxPPassemblageComp *comp = temp_pp_assemblage.Find(x[j]->pp_assemblage_comp_name);
		comp->Set_moles(x[j]->moles);
		comp->Set_delta(0.0);
	}
/*
 *   Finish up
 */

	Rxn_pp_assemblage_map[n_user] = temp_pp_assemblage;
	use.Set_pp_assemblage_ptr(NULL);
	return (OK);
}
#ifdef SKIP
/* ---------------------------------------------------------------------- */
int Phreeqc::
xpp_assemblage_save(int n_user)
/* ---------------------------------------------------------------------- */
{
/*
 *   Save pure_phase assemblage into structure pp_assemblage with user 
 *   number n_user.
 */
	int count_comps, n, j, i;
	struct pp_assemblage temp_pp_assemblage, *pp_assemblage_ptr;
	char token[MAX_LENGTH];

	if (use.Get_pp_assemblage_ptr() == NULL)
		return (OK);
/*
 *   Count pure phases
 */
	count_comps = use.Get_pp_assemblage_ptr()->count_comps;

	temp_pp_assemblage.n_user = n_user;
	temp_pp_assemblage.n_user_end = n_user;
	sprintf(token, "Pure-phase assemblage after simulation %d.", simulation);
	temp_pp_assemblage.description = string_duplicate(token);
	temp_pp_assemblage.new_def = FALSE;
	temp_pp_assemblage.count_comps = count_comps;
	temp_pp_assemblage.next_elt =
		elt_list_dup(use.Get_pp_assemblage_ptr()->next_elt);
/*
 *   Malloc space and copy pure phase data
 */
	temp_pp_assemblage.pure_phases =
		(struct pure_phase *) PHRQ_malloc((size_t) count_comps *
										  sizeof(struct pure_phase));
	if (temp_pp_assemblage.pure_phases == NULL)
		malloc_error();
	memcpy((void *) temp_pp_assemblage.pure_phases,
		   (void *) use.Get_pp_assemblage_ptr()->pure_phases,
		   (size_t) count_comps * sizeof(struct pure_phase));
/*
 *   Update amounts
 */
	i = 0;
	for (j = 0; j < count_unknowns; j++)
	{
		if (x[j]->type != PP)
			continue;
		temp_pp_assemblage.pure_phases[i].moles = x[j]->moles;
		temp_pp_assemblage.pure_phases[i].delta = 0.0;

		/* update unknown ptr, old may be freed later */
		x[j]->pure_phase = &(temp_pp_assemblage.pure_phases[i]);
		i++;
	}
/*
 *   Finish up
 */
	pp_assemblage_ptr = pp_assemblage_bsearch(n_user, &n);
	if (pp_assemblage_ptr == NULL)
	{
		space((void **) ((void *) &pp_assemblage), count_pp_assemblage,
			  &max_pp_assemblage, sizeof(struct pp_assemblage));
		n = count_pp_assemblage++;
	}
	else
	{
		pp_assemblage_free(&pp_assemblage[n]);
	}
	memcpy(&pp_assemblage[n], &temp_pp_assemblage,
		   sizeof(struct pp_assemblage));
	/* sort only if necessary */
	if (n == count_pp_assemblage - 1 && count_pp_assemblage > 1)
	{
		if (pp_assemblage[n].n_user < pp_assemblage[n - 1].n_user)
		{
			qsort(pp_assemblage,
				  (size_t) count_pp_assemblage,
				  (size_t) sizeof(struct pp_assemblage),
				  pp_assemblage_compare);
		}
	}
	use.Get_pp_assemblage_ptr() = NULL;
	return (OK);
}
#endif
/* ---------------------------------------------------------------------- */
int Phreeqc::
xsolution_save(int n_user)
/* ---------------------------------------------------------------------- */
{
/*
 *   Save solution composition into structure solution with user number
 *   n_user.
 *
 *   input:  n_user is user solution number of target
 */
	int i, j, n;
	int count_mass_balance, count_master_activity;
	int max_mass_balance, max_master_activity;
	struct solution *solution_ptr;
	struct master *master_i_ptr, *master_ptr;
/*
 *   Malloc space for solution data
 */
	solution_ptr = solution_alloc();

	max_mass_balance = MAX_MASS_BALANCE;
	max_master_activity = MAX_MASS_BALANCE;

	solution_ptr->n_user = n_user;
	solution_ptr->n_user_end = n_user;
	solution_ptr->new_def = FALSE;
	solution_ptr->description = string_duplicate(description_x);
	solution_ptr->tc = tc_x;
	solution_ptr->patm = patm_x;
	solution_ptr->ph = ph_x;
	solution_ptr->solution_pe = solution_pe_x;
	solution_ptr->mu = mu_x;
	solution_ptr->ah2o = ah2o_x;
	solution_ptr->density = density_x;
	solution_ptr->total_h = total_h_x;
	solution_ptr->total_o = total_o_x;
	solution_ptr->cb = cb_x;	/* cb_x does not include surface charge sfter sum_species */
	/* does include surface charge after step */

	solution_ptr->mass_water = mass_water_aq_x;
	solution_ptr->total_alkalinity = total_alkalinity;
	/*solution_ptr->total_co2 = total_co2; */
	solution_ptr->units = moles_per_kilogram_string;
/*
 *   Copy pe data
 */

	pe_data_free(solution_ptr->pe);
	/*solution_ptr->pe = pe_data_dup(pe_x); */
	solution_ptr->pe = pe_data_alloc();
	solution_ptr->default_pe = 0;
	/*
	 * Add in minor isotopes if initial solution calculation
	 */
	if (initial_solution_isotopes == TRUE)
	{
		for (i = 0; i < count_master_isotope; i++)
		{
			if (master_isotope[i]->moles > 0)
			{
				master_i_ptr = master_bsearch(master_isotope[i]->name);
				master_ptr = master_isotope[i]->elt->master;
				if (master_isotope[i]->minor_isotope == TRUE)
				{
					master_i_ptr->total = master_isotope[i]->moles;
					if (master_ptr->total > 0)
					{
						master_i_ptr->s->la =
							master_ptr->s->la +
							log10(master_i_ptr->total / master_ptr->total);
					}
					else
					{
						master_i_ptr->s->la = master_ptr->s->la;
					}
				}
				else if (master_isotope[i]->minor_isotope == FALSE
						 && master_ptr->s != s_hplus
						 && master_ptr->s != s_h2o)
				{
					if (master_ptr->s->secondary != NULL)
					{
						master_ptr->s->secondary->total =
							master_isotope[i]->moles;
					}
					else
					{
						master_ptr->s->primary->total =
							master_isotope[i]->moles;
					}
				}
			}
		}
	}
/*
 *   Copy totals data
 */
	count_mass_balance = 0;
	count_master_activity = 0;
	for (i = 0; i < count_master; i++)
	{
		if (master[i]->in == FALSE)
			continue;
		if (master[i]->s->type == EX ||
			master[i]->s->type == SURF || master[i]->s->type == SURF_PSI)
			continue;
		if (master[i]->s == s_hplus)
			continue;
		if (master[i]->s == s_h2o)
			continue;
/*
 *   Save list of log activities
 */
		if (master[i]->in != FALSE)
		{
			count_master_activity++;
		}
	}
	solution_ptr->master_activity =
		(struct master_activity *) PHRQ_realloc(solution_ptr->
												master_activity,
												(size_t)
												(count_master_activity +
												 1) *
												sizeof(struct
													   master_activity));
	solution_ptr->count_master_activity = count_master_activity;
	count_master_activity = 0;
	for (i = 0; i < count_master; i++)
	{
		if (master[i]->s->type == EX ||
			master[i]->s->type == SURF || master[i]->s->type == SURF_PSI)
			continue;
		if (master[i]->s == s_hplus)
			continue;
		if (master[i]->s == s_h2o)
			continue;
/*
 *   Save list of log activities
 */
		if (master[i]->in != FALSE)
		{
			solution_ptr->master_activity[count_master_activity].description =
				master[i]->elt->name;
			solution_ptr->master_activity[count_master_activity++].la =
				master[i]->s->la;
			/*
			   if (count_master_activity + 2 >= max_master_activity) {
			   space ((void *) &(solution_ptr->master_activity), count_master_activity + 2,
			   &max_master_activity, sizeof (struct master_activity));
			   }
			 */
		}
		if (master[i]->total <= MIN_TOTAL)
		{
			master[i]->total = 0.0;
			master[i]->total_primary = 0.0;
			continue;
		}
/*              if (master[i]->total <= 0.0) continue;  */
/*
 *   Save list of concentrations
 */
		solution_ptr->totals[count_mass_balance].description =
			master[i]->elt->name;
		solution_ptr->totals[count_mass_balance].input_conc =
			master[i]->total;
		solution_ptr->totals[count_mass_balance].moles = master[i]->total;
		solution_ptr->totals[count_mass_balance].units = solution_ptr->units;
		solution_ptr->totals[count_mass_balance].equation_name = NULL;
		solution_ptr->totals[count_mass_balance].n_pe = 0;
		solution_ptr->totals[count_mass_balance].phase = NULL;
		solution_ptr->totals[count_mass_balance].phase_si = 0.0;
		solution_ptr->totals[count_mass_balance].as = NULL;
		solution_ptr->totals[count_mass_balance].gfw = master[i]->elt->primary->elt->gfw;
		count_mass_balance++;
/*
 *   Make space
 */
		if (count_mass_balance + 2 >= max_mass_balance)
		{
			space((void **) ((void *) &(solution_ptr->totals)),
				  count_mass_balance + 2, &max_mass_balance,
				  sizeof(struct conc));
		}
	}
	if (pitzer_model == TRUE || sit_model == TRUE)
	{
		i = 0;
		for (j = 0; j < count_s; j++)
		{
			if (s[j]->lg != 0.0)
				i++;
		}
		solution_ptr->species_gamma =
			(struct master_activity *) PHRQ_realloc(solution_ptr->
													species_gamma,
													(size_t) (i *
															  sizeof(struct
																	 master_activity)));
		i = 0;
		for (j = 0; j < count_s; j++)
		{
			if (s[j]->lg != 0.0)
			{
				solution_ptr->species_gamma[i].la = s[j]->lg;
				solution_ptr->species_gamma[i].description = s[j]->name;
				i++;
			}
		}
		solution_ptr->count_species_gamma = i;
	}
	else
	{
		solution_ptr->species_gamma = NULL;
		solution_ptr->count_species_gamma = 0;
	}
/*
 *   Mark end of totals
 */
	solution_ptr->totals[count_mass_balance].description = NULL;
	count_mass_balance++;
	solution_ptr->master_activity[count_master_activity].description = NULL;
	count_master_activity++;
	solution_ptr->totals =
		(struct conc *) PHRQ_realloc(solution_ptr->totals,
									 (size_t) count_mass_balance *
									 sizeof(struct conc));
	if (solution_ptr->totals == NULL)
		malloc_error();
	solution_ptr->master_activity =
		(struct master_activity *) PHRQ_realloc(solution_ptr->
												master_activity,
												(size_t)
												count_master_activity *
												sizeof(struct
													   master_activity));
	if (solution_ptr->master_activity == NULL)
		malloc_error();
	solution_ptr->count_master_activity = count_master_activity;
/*
 *   Save isotope data
 */
	if (count_isotopes_x > 0)
	{
		solution_ptr->count_isotopes = count_isotopes_x;
		solution_ptr->isotopes =
			(struct isotope *) PHRQ_realloc(solution_ptr->isotopes,
											(size_t) count_isotopes_x *
											sizeof(struct isotope));
		if (solution_ptr->isotopes == NULL)
			malloc_error();
		memcpy(solution_ptr->isotopes, isotopes_x,
			   (size_t) count_isotopes_x * sizeof(struct isotope));
		for (i = 0; i < count_isotopes_x; i++)
		{
			solution_ptr->isotopes[i].total =
				solution_ptr->isotopes[i].master->total;
			if (solution_ptr->isotopes[i].master == s_hplus->secondary)
			{
				solution_ptr->isotopes[i].total =
					2 * mass_water_aq_x / gfw_water;
			}
			if (solution_ptr->isotopes[i].master == s_h2o->secondary)
			{
				solution_ptr->isotopes[i].total = mass_water_aq_x / gfw_water;
			}
		}
	}
	else
	{
		solution_ptr->count_isotopes = 0;
		solution_ptr->isotopes =
			(struct isotope *) free_check_null(solution_ptr->isotopes);
		solution_ptr->isotopes = NULL;
	}
/*
 *   Save solution in solution
 */
	if (solution_bsearch(n_user, &n, FALSE) != NULL)
	{
		solution_free(solution[n]);
	}
	else
	{
		n = count_solution++;
		if (count_solution >= max_solution)
		{
			space((void **) ((void *) &(solution)), count_solution,
				  &max_solution, sizeof(struct solution *));
		}
	}
	solution[n] = solution_ptr;
	/* sort only if necessary */
	if (count_solution > 1
		&& (solution[count_solution - 1]->n_user <
			solution[count_solution - 2]->n_user))
	{
		solution_sort();
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
xsurface_save(int n_user)
/* ---------------------------------------------------------------------- */
{
/*
 *   Save surface data into structure surface with user 
 *   number n_user.
 */
	int i, j, n, last_charge;
	int count_comps, count_charge;
	char token[MAX_LENGTH];
	struct surface temp_surface, *surface_ptr;
	LDBLE charge;
	if (use.Get_surface_ptr() == NULL)
		return (OK);
/*
 *   Store data for structure surface
 */
	memcpy(&temp_surface, use.Get_surface_ptr(), sizeof(struct surface));
	temp_surface.n_user = n_user;
	temp_surface.n_user_end = n_user;
	temp_surface.new_def = FALSE;
	temp_surface.dl_type = dl_type_x;
	sprintf(token, "Surface assemblage after simulation %d.", simulation);
	temp_surface.description = string_duplicate(token);
	temp_surface.solution_equilibria = FALSE;
	temp_surface.n_solution = -10;
/*
 *   Allocate space to pointer comps
 */

	count_comps = use.Get_surface_ptr()->count_comps;
	count_charge = use.Get_surface_ptr()->count_charge;
	temp_surface.count_comps = count_comps;
	temp_surface.comps =
		(struct surface_comp *) PHRQ_malloc((size_t) (count_comps) *
											sizeof(struct surface_comp));
	if (temp_surface.comps == NULL)
		malloc_error();
	/*if (temp_surface.edl == FALSE) { */
	if (temp_surface.type == NO_EDL)
	{
		temp_surface.charge = NULL;
		temp_surface.count_charge = 0;
	}
	else
	{
		temp_surface.count_charge = count_charge;
		temp_surface.charge =
			(struct surface_charge *) PHRQ_malloc((size_t) (count_charge) *
												  sizeof(struct
														 surface_charge));
		if (temp_surface.charge == NULL)
			malloc_error();
	}
/*
 *   Write surface_comp structure for each surf component into comps_ptr
 */
	count_comps = 0;
	count_charge = 0;
	/*
	 *  Initial entry of surface sites is random
	 *  Charge balance numbering follows the initial entry
	 *  Surface sites are then sorted alphabetically
	 *  Now when we save, the site order differs from the charge order
	 *  last_charge sets up logic to renumber charge balance equations.
	 */
	last_charge = -1;
	for (i = 0; i < count_unknowns; i++)
	{
		if (x[i]->type == SURFACE)
		{
			memcpy(&temp_surface.comps[count_comps], x[i]->surface_comp,
				   sizeof(struct surface_comp));

			temp_surface.comps[count_comps].master = x[i]->master[0];
			temp_surface.comps[count_comps].la = x[i]->master[0]->s->la;
			/* temp_surface.comps[count_comps].formula = NULL; */
			temp_surface.comps[count_comps].moles = 0.;
			if (x[i]->surface_comp->charge == last_charge)
			{
				temp_surface.comps[count_comps].charge = count_charge - 1;
			}
			else
			{
				temp_surface.comps[count_comps].charge = count_charge;
			}
			last_charge = x[i]->surface_comp->charge;
/*
 *   Save element concentrations on surface
 */
			count_elts = 0;
			paren_count = 0;
			charge = 0.0;
			for (j = 0; j < count_species_list; j++)
			{
				if (species_list[j].master_s == x[i]->master[0]->s)
				{
					add_elt_list(species_list[j].s->next_elt,
								 species_list[j].s->moles);
					charge += species_list[j].s->moles * species_list[j].s->z;
				}
			}
			temp_surface.comps[count_comps].totals = elt_list_save();
			temp_surface.comps[count_comps].formula_totals =
				elt_list_dup(x[i]->surface_comp->formula_totals);
			temp_surface.comps[count_comps].cb = charge;

			/* update unknown pointer */
			x[i]->surface_comp = &(temp_surface.comps[count_comps]);
			count_comps++;
		}
		else if (x[i]->type == SURFACE_CB && use.Get_surface_ptr()->type == DDL)
		{
			memcpy(&temp_surface.charge[count_charge], x[i]->surface_charge,
				   sizeof(struct surface_charge));
			temp_surface.charge[count_charge].charge_balance = x[i]->f;
			temp_surface.charge[count_charge].mass_water =
				x[i]->surface_charge->mass_water;
			temp_surface.charge[count_charge].diffuse_layer_totals = NULL;
			temp_surface.charge[count_charge].count_g = 0;
			temp_surface.charge[count_charge].g = NULL;
/*
 *   Added code to save g
 */

			if (x[i]->surface_charge->count_g >
				0 /*&& use.Get_surface_ptr()->type != CD_MUSIC */ )
			{
				temp_surface.charge[count_charge].count_g =
					x[i]->surface_charge->count_g;
				temp_surface.charge[count_charge].g =
					(struct surface_diff_layer *) PHRQ_malloc((size_t) x[i]->
															  surface_charge->
															  count_g *
															  sizeof(struct
																	 surface_diff_layer));
				if (temp_surface.charge[count_charge].g == NULL)
					malloc_error();
				memcpy(temp_surface.charge[count_charge].g,
					   x[i]->surface_charge->g,
					   (size_t) x[i]->surface_charge->count_g *
					   sizeof(struct surface_diff_layer));
			}

			/*temp_surface.charge[count_charge].psi_master = x[i]->master[0]; */
			temp_surface.charge[count_charge].la_psi = x[i]->master[0]->s->la;
/*
 *   Store moles from diffuse_layer
 */
			if (dl_type_x != NO_DL)
			{
				sum_diffuse_layer(x[i]->surface_charge);
				temp_surface.charge[count_charge].diffuse_layer_totals =
					elt_list_save();
			}

			/* update unknown pointer */
			x[i]->surface_charge = &(temp_surface.charge[count_charge]);
			x[i]->surface_comp = x[i - 1]->surface_comp;

			count_charge++;
		}
		else if (x[i]->type == SURFACE_CB
				 && use.Get_surface_ptr()->type == CD_MUSIC)
		{
			memcpy(&temp_surface.charge[count_charge], x[i]->surface_charge,
				   sizeof(struct surface_charge));
			if (dl_type_x != NO_DL)
			{
				temp_surface.charge[count_charge].charge_balance =
					(x[i]->surface_charge->sigma0 +
					 x[i]->surface_charge->sigma1 +
					 x[i]->surface_charge->sigma2 +
					 x[i]->surface_charge->sigmaddl)
					* (x[i]->surface_charge->specific_area *
					   x[i]->surface_charge->grams) / F_C_MOL;
			}
			else
			{
				temp_surface.charge[count_charge].charge_balance =
					(x[i]->surface_charge->sigma0 +
					 x[i]->surface_charge->sigma1 +
					 x[i]->surface_charge->sigma2)
					* (x[i]->surface_charge->specific_area *
					   x[i]->surface_charge->grams) / F_C_MOL;
			}
			temp_surface.charge[count_charge].mass_water =
				x[i]->surface_charge->mass_water;
			temp_surface.charge[count_charge].diffuse_layer_totals = NULL;
			temp_surface.charge[count_charge].count_g = 0;
			temp_surface.charge[count_charge].g = NULL;
/*
 *   Added code to save g
 */

			if (x[i]->surface_charge->count_g > 0)
			{
				temp_surface.charge[count_charge].count_g =
					x[i]->surface_charge->count_g;
				temp_surface.charge[count_charge].g =
					(struct surface_diff_layer *) PHRQ_malloc((size_t) x[i]->
															  surface_charge->
															  count_g *
															  sizeof(struct
																	 surface_diff_layer));
				if (temp_surface.charge[count_charge].g == NULL)
					malloc_error();
				memcpy(temp_surface.charge[count_charge].g,
					   x[i]->surface_charge->g,
					   (size_t) x[i]->surface_charge->count_g *
					   sizeof(struct surface_diff_layer));
			}

			/*temp_surface.charge[count_charge].psi_master = x[i]->master[0]; */
			temp_surface.charge[count_charge].la_psi = x[i]->master[0]->s->la;
/*
 *   Store moles from diffuse_layer
 */
			if (dl_type_x != NO_DL)
			{
				sum_diffuse_layer(x[i]->surface_charge);
				temp_surface.charge[count_charge].diffuse_layer_totals =
					elt_list_save();
			}

			/* update unknown pointer */
			x[i]->surface_charge = &(temp_surface.charge[count_charge]);
			x[i]->surface_comp = x[i - 1]->surface_comp;

			count_charge++;
		}
	}
/*
 *   Finish up
 */
	surface_ptr = surface_bsearch(n_user, &n);
	if (surface_ptr == NULL)
	{
		n = count_surface++;
		space((void **) ((void *) &surface), count_surface, &max_surface,
			  sizeof(struct surface));
	}
	else
	{
		surface_free(&surface[n]);
	}
	memcpy(&surface[n], &temp_surface, sizeof(struct surface));
	if (n == count_surface - 1 && count_surface > 1)
	{
		if (surface[n].n_user < surface[n - 1].n_user)
		{
			qsort(surface,
				  (size_t) count_surface,
				  (size_t) sizeof(struct surface), surface_compare);
		}
	}
	use.Set_surface_ptr(NULL);
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
copy_use(int i)
/* ---------------------------------------------------------------------- */
{
/*
 *   Find mixture
 */
	if (use.Get_mix_in() == TRUE)
	{
		Utilities::Rxn_copy(Rxn_mix_map, use.Get_n_mix_user(), i);
	}
/*
 *   Find solution
 */
	if (use.Get_solution_in() == TRUE)
	{
		solution_duplicate(use.Get_n_solution_user(), i);
	}
/*
 *   Always save solution to i, mixing or not
 */
	save.solution = TRUE;
	save.n_solution_user = i;
	save.n_solution_user_end = i;
/*
 *   Find pure phase assemblage
 */
	if (use.Get_pp_assemblage_in() == TRUE)
	{
		//pp_assemblage_duplicate(use.Get_n_pp_assemblage_user(), i);
		Utilities::Rxn_copy(Rxn_pp_assemblage_map, use.Get_n_pp_assemblage_user(), i);
		save.pp_assemblage = TRUE;
		save.n_pp_assemblage_user = i;
		save.n_pp_assemblage_user_end = i;
	}
	else
	{
		save.pp_assemblage = FALSE;
	}
/*
 *   Find irrev reaction
 */
	if (use.Get_reaction_in() == TRUE)
	{
		Utilities::Rxn_copy(Rxn_reaction_map, use.Get_n_reaction_user(), i);
		save.reaction = TRUE;
		save.n_reaction_user = i;
		save.n_reaction_user_end = i;
	}
	else
	{
		save.reaction = FALSE;
	}
/*
 *   Find exchange
 */
	if (use.Get_exchange_in() == TRUE)
	{
		Utilities::Rxn_copy(Rxn_exchange_map, use.Get_n_exchange_user(), i);
		save.exchange = TRUE;
		save.n_exchange_user = i;
		save.n_exchange_user_end = i;
	}
	else
	{
		save.exchange = FALSE;
	}
/*
 *   Find kinetics
 */
	if (use.Get_kinetics_in() == TRUE)
	{
		kinetics_duplicate(use.Get_n_kinetics_user(), i);
		save.kinetics = TRUE;
		save.n_kinetics_user = i;
		save.n_kinetics_user_end = i;
	}
	else
	{
		save.kinetics = FALSE;
	}
/*
 *   Find surface
 */
	dl_type_x = NO_DL;
	if (use.Get_surface_in() == TRUE)
	{
		surface_duplicate(use.Get_n_surface_user(), i);
		save.surface = TRUE;
		save.n_surface_user = i;
		save.n_surface_user_end = i;
	}
	else
	{
		save.surface = FALSE;
	}
/*
 *   Find temperature
 */
	if (use.Get_temperature_in() == TRUE)
	{
		Utilities::Rxn_copy(Rxn_temperature_map, use.Get_n_temperature_user(), i);
	}
/*
 *   Find pressure
 */
	if (use.Get_pressure_in() == TRUE)
	{
		Utilities::Rxn_copy(Rxn_pressure_map, use.Get_n_pressure_user(), i);
	}
/*
 *   Find gas
 */
	if (use.Get_gas_phase_in() == TRUE)
	{
		//gas_phase_duplicate(use.Get_n_gas_phase_user(), i);
		Utilities::Rxn_copy(Rxn_gas_phase_map, use.Get_n_gas_phase_user(), i);
		save.gas_phase = TRUE;
		save.n_gas_phase_user = i;
		save.n_gas_phase_user_end = i;
	}
	else
	{
		save.gas_phase = FALSE;
	}
/*
 *   Find solid solution
 */
	if (use.Get_ss_assemblage_in() == TRUE)
	{
		//ss_assemblage_duplicate(use.Get_n_ss_assemblage_user(), i);
		Utilities::Rxn_copy(Rxn_ss_assemblage_map, use.Get_n_ss_assemblage_user(), i);
		save.ss_assemblage = TRUE;
		save.n_ss_assemblage_user = i;
		save.n_ss_assemblage_user_end = i;
	}
	else
	{
		save.ss_assemblage = FALSE;
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
int Phreeqc::
step_save_exch(int n_user)
/* ---------------------------------------------------------------------- */
{
/*
 *   Save exchange composition 
 *
 *   input:  n_user is user exchange number of target
 */
	//int i;
	bool found;

	if (use.Get_exchange_ptr() == NULL)
		return (OK);

	cxxExchange *temp_ptr = Utilities::Rxn_find(Rxn_exchange_map, use.Get_n_exchange_user());
	assert(temp_ptr);

	// Set all totals to 0.0
	cxxExchange temp_exchange = *temp_ptr;
	{
		std::map<std::string, cxxExchComp>::iterator it = temp_exchange.Get_exchComps().begin();
		for ( ; it != temp_exchange.Get_exchComps().end(); it++)
		{
			it->second.Get_totals().multiply(0.0);
		}
	}

	// Set exchange total in one component
	for (int i = 0; i < count_master; i++)
	{
		if (master[i]->s->type != EX)
			continue;
		found = false;
		std::string e(master[i]->elt->name);
		std::map<std::string, cxxExchComp>::iterator it = temp_exchange.Get_exchComps().begin();
		for ( ; it != temp_exchange.Get_exchComps().end(); it++)
		{
			cxxNameDouble *nd = &(it->second.Get_totals());
			cxxNameDouble::iterator nd_it = nd->find(e);
			if (nd_it != nd->end())
			{
				LDBLE coef;
				if (master[i]->total <= MIN_TOTAL)
				{
					coef = MIN_TOTAL;
				}
				else
				{
					coef = master[i]->total;
				}
				nd->insert(nd_it->first.c_str(), coef);
				break;
			}
		}
	}
	
	Rxn_exchange_map[n_user] = temp_exchange;
	return (OK);
}
#ifdef SKIP
/* ---------------------------------------------------------------------- */
int Phreeqc::
step_save_exch(int n_user)
/* ---------------------------------------------------------------------- */
{
/*
 *   Save exchange composition 
 *
 *   input:  n_user is user exchange number of target
 */
	//int i;
	bool found;

	if (use.Get_exchange_ptr() == NULL)
		return (OK);
	//exchange_duplicate(use.Get_exchange_ptr()->n_user, n_user);
	//exchange_ptr = exchange_bsearch(n_user, &n);
	cxxExchange *temp_ptr = Utilities::Rxn_find(Rxn_exchange_map, use.Get_n_exchange_user());
	assert(temp_ptr);
	cxxExchange temp_exchange = *temp_ptr;
	
	for (int i = 0; i < count_master; i++)
	{
		if (master[i]->s->type != EX)
			continue;
		found = false;
		std::string e(master[i]->elt->name);
		std::map<std::string, cxxExchComp>::iterator it = temp_exchange.Get_exchComps().begin();
		for ( ; it != temp_exchange.Get_exchComps().end(); it++)
		{
			cxxNameDouble nd = it->second.Get_totals();
			nd.multiply(0.0);
			cxxNameDouble::iterator nd_it =nd.find(e);
			if ((!found) && nd_it != nd.end())
			{
				found = true;
				LDBLE coef;
				if (master[i]->total <= MIN_TOTAL)
				{
					coef = MIN_TOTAL;
				}
				else
				{
					coef = master[i]->total;
				}
				nd[nd_it->first.c_str()] = coef;
			}
			it->second.Set_totals(nd);
		}
	}
	
	Rxn_exchange_map[n_user] = temp_exchange;
	return (OK);
}
#endif
/* ---------------------------------------------------------------------- */
int Phreeqc::
step_save_surf(int n_user)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Save surface for intermediate calculation
	 *   Amt of surface may have changed due to reaction or surface related
	 *   to kinetic reactant.
	 *
	 *   input:  n_user is user solution number of target
	 */
	int i, j, k, n, m;
	struct surface *surface_ptr;
	/*
	 *   Malloc space for solution data
	 */
	if (use.Get_surface_ptr() == NULL)
		return (OK);
	surface_duplicate(use.Get_surface_ptr()->n_user, n_user);
	surface_ptr = surface_bsearch(n_user, &n);
	for (i = 0; i < count_master; i++)
	{
		if (master[i]->s->type != SURF)
			continue;
		for (j = 0; j < surface_ptr->count_comps; j++)
		{
			for (k = 0; surface_ptr->comps[j].totals[k].elt != NULL; k++)
			{
				if (surface_ptr->comps[j].totals[k].elt == master[i]->elt)
				{
					if (master[i]->total <= MIN_TOTAL)
					{
						surface_ptr->comps[j].totals[k].coef = MIN_TOTAL;
					}
					else
					{
						surface_ptr->comps[j].totals[k].coef =
							master[i]->total;
					}
					break;
				}
			}
		}
	}
	/*
	 *   Update grams
	 */
	/*if (surface_ptr->edl == TRUE && surface_ptr->related_rate == TRUE && use.Get_kinetics_ptr() != NULL) { */
	if ((surface_ptr->type == DDL || surface_ptr->type == CD_MUSIC)
		&& surface_ptr->related_rate == TRUE && use.Get_kinetics_ptr() != NULL)
	{
		for (j = 0; j < surface_ptr->count_comps; j++)
		{
			if (surface_ptr->comps[j].rate_name != NULL)
			{
				for (m = 0; m < use.Get_kinetics_ptr()->count_comps; m++)
				{
					if (strcmp_nocase
						(use.Get_kinetics_ptr()->comps[m].rate_name,
						 surface_ptr->comps[j].rate_name) != 0)
						continue;
					surface_ptr->charge[surface_ptr->comps[j].charge].grams =
						use.Get_kinetics_ptr()->comps[m].m;
					break;
				}
			}
		}
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
copy_entities(void)
/* ---------------------------------------------------------------------- */
{
	int i, j, n, return_value;
	int verbose;

	verbose = FALSE;
	return_value = OK;
	if (copy_solution.count > 0)
	{
		for (j = 0; j < copy_solution.count; j++)
		{
			if (solution_bsearch(copy_solution.n_user[j], &n, FALSE) != NULL)
			{
				for (i = copy_solution.start[j]; i <= copy_solution.end[j];
					i++)
				{
					if (i == copy_solution.n_user[j])
						continue;
					solution_duplicate(copy_solution.n_user[j], i);
				}
			}
			else
			{
				if (verbose == TRUE)
				{
					warning_msg("SOLUTION to copy not found.");
					return_value = ERROR;
				}
			}
		}
	}
	if (copy_pp_assemblage.count > 0)
	{
		for (j = 0; j < copy_pp_assemblage.count; j++)
		{
			//if (pp_assemblage_bsearch(copy_pp_assemblage.n_user[j], &n) != NULL)
			if (Utilities::Rxn_find(Rxn_pp_assemblage_map, copy_pp_assemblage.n_user[j]) != NULL)
			{
				for (i = copy_pp_assemblage.start[j];
					i <= copy_pp_assemblage.end[j]; i++)
				{
					if (i == copy_pp_assemblage.n_user[j])
						continue;
					//pp_assemblage_duplicate(copy_pp_assemblage.n_user[j], i);
					Utilities::Rxn_copy(Rxn_pp_assemblage_map, copy_pp_assemblage.n_user[j], i);
				}
			}
			else
			{
				if (verbose == TRUE)
				{
					warning_msg("EQUILIBRIUM_PHASES to copy not found.");
					return_value = ERROR;
				}
			}
		}
	}
	if (copy_reaction.count > 0)
	{
		for (j = 0; j < copy_reaction.count; j++)
		{
			//if (irrev_bsearch(copy_irrev.n_user[j], &n) != NULL)
			if (Utilities::Rxn_find(Rxn_reaction_map, copy_reaction.n_user[j]) != NULL)
			{
				for (i = copy_reaction.start[j]; i <= copy_reaction.end[j]; i++)
				{
					if (i == copy_reaction.n_user[j])
						continue;
					//irrev_duplicate(copy_irrev.n_user[j], i);
					Utilities::Rxn_copy(Rxn_reaction_map, copy_reaction.n_user[j], i);
				}
			}
			else
			{
				if (verbose == TRUE)
				{
					warning_msg("REACTION to copy not found.");
					return_value = ERROR;
				}
			}
		}
	}
	if (copy_mix.count > 0)
	{
		for (j = 0; j < copy_mix.count; j++)
		{
			if (Utilities::Rxn_find(Rxn_mix_map, copy_mix.n_user[j]) != NULL)
			{
				for (i = copy_mix.start[j]; i <= copy_mix.end[j]; i++)
				{
					if (i != copy_mix.n_user[j]) 
					{
						Utilities::Rxn_copy(Rxn_mix_map, copy_mix.n_user[j], i);
					}
				}
			}
			else
			{
				if (verbose == TRUE)
				{
					warning_msg("Mix to copy not found.");
					return_value = ERROR;
				}
			}
		}
	}
	
	if (copy_exchange.count > 0)
	{
		for (j = 0; j < copy_exchange.count; j++)
		{
			if (Utilities::Rxn_find(Rxn_exchange_map, copy_exchange.n_user[j]) != NULL)
			{
				for (i = copy_exchange.start[j]; i <= copy_exchange.end[j];
					i++)
				{
					if (i == copy_exchange.n_user[j])
						continue;
					Utilities::Rxn_copy(Rxn_exchange_map, copy_exchange.n_user[j], i);
				}
			}
			else
			{
				if (verbose == TRUE)
				{
					warning_msg("EXCHANGE to copy not found.");
					return_value = ERROR;
				}
			}
		}
	}
	if (copy_surface.count > 0)
	{
		for (j = 0; j < copy_surface.count; j++)
		{
			if (surface_bsearch(copy_surface.n_user[j], &n) != NULL)
			{
				for (i = copy_surface.start[j]; i <= copy_surface.end[j]; i++)
				{
					if (i == copy_surface.n_user[j])
						continue;
					surface_duplicate(copy_surface.n_user[j], i);
				}
			}
			else
			{
				if (verbose == TRUE)
				{
					warning_msg("SURFACE to copy not found.");
					return_value = ERROR;
				}
			}
		}
	}

	if (copy_temperature.count > 0)
	{
		for (j = 0; j < copy_temperature.count; j++)
		{
			if (Utilities::Rxn_find(Rxn_temperature_map, copy_temperature.n_user[j]) != NULL)
			{
				for (i = copy_temperature.start[j]; i <= copy_temperature.end[j]; i++)
				{
					if (i != copy_temperature.n_user[j]) 
					{
						Utilities::Rxn_copy(Rxn_temperature_map, copy_temperature.n_user[j], i);
					}
				}
			}
			else
			{
				if (verbose == TRUE)
				{
					warning_msg("temperature to copy not found.");
					return_value = ERROR;
				}
			}
		}
	}
	if (copy_pressure.count > 0)
	{
		for (j = 0; j < copy_pressure.count; j++)
		{
			if (Utilities::Rxn_find(Rxn_pressure_map, copy_pressure.n_user[j]) != NULL)
			{
				for (i = copy_pressure.start[j]; i <= copy_pressure.end[j]; i++)
				{
					if (i != copy_pressure.n_user[j]) 
					{
						Utilities::Rxn_copy(Rxn_pressure_map, copy_pressure.n_user[j], i);
					}
				}
			}
			else
			{
				if (verbose == TRUE)
				{
					warning_msg("pressure to copy not found.");
					return_value = ERROR;
				}
			}
		}
	}
	if (copy_gas_phase.count > 0)
	{
		for (j = 0; j < copy_gas_phase.count; j++)
		{
			if (Utilities::Rxn_find(Rxn_gas_phase_map, copy_gas_phase.n_user[j]) != NULL)
			{
				for (i = copy_gas_phase.start[j]; i <= copy_gas_phase.end[j];
					i++)
				{
					if (i == copy_gas_phase.n_user[j])
						continue;
					Utilities::Rxn_copy(Rxn_gas_phase_map, copy_gas_phase.n_user[j], i);
				}
			}
			else
			{
				if (verbose == TRUE)
				{
					warning_msg("EXCHANGE to copy not found.");
					return_value = ERROR;
				}
			}
		}
	}
	if (copy_kinetics.count > 0)
	{
		for (j = 0; j < copy_kinetics.count; j++)
		{
			if (kinetics_bsearch(copy_kinetics.n_user[j], &n) != NULL)
			{
				for (i = copy_kinetics.start[j]; i <= copy_kinetics.end[j];
					i++)
				{
					if (i == copy_kinetics.n_user[j])
						continue;
					kinetics_duplicate(copy_kinetics.n_user[j], i);
				}
			}
			else
			{
				if (verbose == TRUE)
				{
					warning_msg("KINETICS to copy not found.");
					return_value = ERROR;
				}
			}
		}
	}
	if (copy_ss_assemblage.count > 0)
	{
		for (j = 0; j < copy_ss_assemblage.count; j++)
		{
			//if (ss_assemblage_bsearch(copy_ss_assemblage.n_user[j], &n) != NULL)
			if (Utilities::Rxn_find(Rxn_ss_assemblage_map, copy_ss_assemblage.n_user[j]) != NULL)
			{
				for (i = copy_ss_assemblage.start[j];
					i <= copy_ss_assemblage.end[j]; i++)
				{
					if (i == copy_ss_assemblage.n_user[j])
						continue;
					//ss_assemblage_duplicate(copy_ss_assemblage.n_user[j], i);
					Utilities::Rxn_copy(Rxn_ss_assemblage_map, copy_ss_assemblage.n_user[j], i);
				}
			}
			else
			{
				if (verbose == TRUE)
				{
					warning_msg("SOLID_SOLUTIONS to copy not found.");
					return_value = ERROR;
				}
			}
		}
	}
	copy_solution.count = 0;
	copy_pp_assemblage.count = 0;
	copy_exchange.count = 0;
	copy_surface.count = 0;
	copy_ss_assemblage.count = 0;
	copy_gas_phase.count = 0;
	copy_kinetics.count = 0;
	copy_mix.count = 0;
	copy_reaction.count = 0;
	copy_temperature.count = 0;
	copy_pressure.count = 0;
	new_copy = FALSE;
	return (OK);
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
read_database(void)
/* ---------------------------------------------------------------------- */
{
	simulation = 0;

/*
 *   Prepare error handling
 */
	try
	{
		set_reading_database(TRUE);
		dup_print("Reading data base.", TRUE);
		read_input();
		tidy_model();
		status(0, NULL);
	}
	catch (PhreeqcStop e)
	{
		return get_input_errors();
	}
	set_reading_database(FALSE);
	return 0;
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
run_simulations(void)
/* ---------------------------------------------------------------------- */
{
	char token[MAX_LENGTH];
#ifdef SKIP
#if defined(WIN32)
	unsigned int old_exponent_format;
	old_exponent_format = _set_output_format(_TWO_DIGIT_EXPONENT);
#endif
#endif
/*
 *   Prepare error handling
 */
	try
	{
/*
 *   Read input data for simulation
 */
		for (simulation = 1;; simulation++)
		{

#if defined PHREEQ98 
			AddSeries = !connect_simulations;
#endif
			sprintf(token, "Reading input data for simulation %d.", simulation);

			dup_print(token, TRUE);
			if (read_input() == EOF)
				break;

			if (title_x != NULL)
			{
				sprintf(token, "TITLE");
				dup_print(token, TRUE);
				if (pr.headings == TRUE)
					output_msg(sformatf( "%s\n\n", title_x));
			}
			tidy_model();
#ifdef PHREEQ98
			if (!phreeq98_debug)
			{
#endif

/*
 *   Calculate distribution of species for initial solutions
 */
			if (new_solution)
				initial_solutions(TRUE);
/*
 *   Calculate distribution for exchangers
 */
			if (new_exchange)
				initial_exchangers(TRUE);
/*
 *   Calculate distribution for surfaces
 */
			if (new_surface)
				initial_surfaces(TRUE);
/*
 *   Calculate initial gas composition
 */
			if (new_gas_phase)
				initial_gas_phases(TRUE);
/*
 *   Calculate reactions
 */
			reactions();
/*
 *   Calculate inverse models
 */
			inverse_models();
/*
 *   Calculate advection
 */
			if (use.Get_advect_in())
			{
				dup_print("Beginning of advection calculations.", TRUE);
				advection();
			}
/*
 *   Calculate transport
 */
			if (use.Get_trans_in())
			{
				dup_print("Beginning of transport calculations.", TRUE);
				transport();
			}
/*
 *   run
 */
			run_as_cells();
/*
 *   Copy
 */
			if (new_copy) copy_entities();
/*
 *   dump
 */
			dump_entities();
/*
 *   delete
 */
			delete_entities();
/*
 *   End of simulation
 */
			dup_print("End of simulation.", TRUE);
#ifdef PHREEQ98
		}						/* if (!phreeq98_debug) */
#endif
		}

	}
	catch (PhreeqcStop e)
	{
		return get_input_errors();
	}
	return 0;
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
do_initialize(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Prepare error handling
 */
	try {

		state = INITIALIZE;

		initialize();
	}
	catch (PhreeqcStop e)
	{
		return get_input_errors();
	}
	return 0;
}

/* ---------------------------------------------------------------------- */
int Phreeqc::
do_status(void)
/* ---------------------------------------------------------------------- */
{
/*
 *   Prepare error handling
 */
	try {

		if (pr.status == TRUE)
		{
#if defined(PHREEQCI_GUI)
		state = -1;
		status(0, "\r\nDone.");
#else
			status(0, "\nDone.");
#endif
			screen_msg("\n");
		}
		dup_print("End of run.", TRUE);
		screen_msg("\nEnd of Run.\n");
	}
	catch (PhreeqcStop e)
	{
		return get_input_errors();
	}
	return 0;
}
void Phreeqc::
save_init(int i)
{
	save.solution = i;
	save.n_solution_user = i;
	save.n_solution_user_end = i;
	save.mix = i;
	save.n_mix_user = i;
	save.n_mix_user_end = i;
	save.reaction = i;
	save.n_reaction_user = i;
	save.n_reaction_user_end = i;
	save.pp_assemblage = i;
	save.n_pp_assemblage_user = i;
	save.n_pp_assemblage_user_end = i;
	save.exchange = i;
	save.n_exchange_user = i;
	save.n_exchange_user_end = i;
	save.kinetics = i;
	save.n_kinetics_user = i;
	save.n_kinetics_user_end = i;
	save.surface = i;
	save.n_surface_user = i;
	save.n_surface_user_end = i;
	save.gas_phase = i;
	save.n_gas_phase_user = i;
	save.n_gas_phase_user_end = i;
	save.ss_assemblage = i;
	save.n_ss_assemblage_user = i;
	save.n_ss_assemblage_user_end = i;
}
#ifdef SKIP
void Phreeqc::
use_init(void)
{
	use.Get_solution_in() = FALSE;
	use.Get_n_solution_user()= -1;
	use.Get_n_solution()= -1;
	use.Get_solution_ptr() = NULL;

	use.Get_pp_assemblage_in() = FALSE;
	use.Get_n_pp_assemblage_user()= -1;
	//use.n_pp_assemblage= -1;
	use.Get_pp_assemblage_ptr() = NULL;

	use.Get_mix_in() = FALSE;
	use.Get_n_mix_user()= -1;
	//use.n_mix= -1;
	use.Get_mix_ptr() = NULL;
	use.Get_n_mix_user_orig()= -1;

	use.Get_reaction_in() = FALSE;
	use.Get_n_reaction_user()= -1;
	//use.n_irrev= -1;
	use.Get_reaction_ptr() = NULL;

	use.Get_exchange_in() = FALSE;
	use.Get_n_exchange_user()= -1;
	//use.n_exchange= -1;
	use.Get_exchange_ptr() = NULL;

	use.Get_kinetics_in() = FALSE;
	use.Get_n_kinetics_user()= -1;
	use.n_kinetics= -1;
	use.Get_kinetics_ptr() = NULL;

	use.Get_surface_in() = FALSE;
	use.Get_n_surface_user()= -1;
	use.n_surface= -1;
	use.Get_surface_ptr() = NULL;

	use.Get_temperature_in() = FALSE;
	use.Get_n_temperature_user()= -1;
	use.Get_temperature_ptr() = NULL;

	use.Get_pressure_in() = FALSE;
	use.Get_n_pressure_user()= -1;
	use.Get_pressure_ptr() = NULL;

	use.Get_inverse_in() = FALSE;
	use.Get_n_inverse_user()= -1;
	use.n_inverse= -1;
	use.Get_inverse_ptr() = NULL;

	use.Get_gas_phase_in() = FALSE;
	use.Get_n_gas_phase_user()= -1;
	//use.n_gas_phase= -1;
	use.Get_gas_phase_ptr() = NULL;

	use.Get_ss_assemblage_in() = FALSE;
	use.Get_n_ss_assemblage_user()= -1;
	use.n_ss_assemblage= -1;
	use.Get_ss_assemblage_ptr() = NULL;

	use.trans_in = FALSE;
	use.advect_in = FALSE;
}
#endif