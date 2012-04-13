#include "Phreeqc.h"
#include <algorithm>			// std::replace

#include "NameDouble.h"
#include "Solution.h"
#include "Reaction.h"
#include "PPassemblage.h"
#include "Exchange.h"
#include "Surface.h"
#include "GasPhase.h"
#include "SSassemblage.h"
#include "cxxKinetics.h"
#include "phqalloc.h"
#include "PBasic.h"
#include "Temperature.h"
#include "SSassemblage.h"

const struct const_iso Phreeqc::iso_defaults[] = {
	{"13C", -10, 1},
	{"13C(4)", -10, 1},
	{"13C(-4)", -50, 5},
	{"34S", 10, 1},
	{"34S(6)", 10, 1},
	{"34S(-2)", -30, 5},
	{"2H", -28, 1},
	{"18O", -5, .1},
	{"87Sr", .71, .01},
	{"11B", 20, 5}
};

const int Phreeqc::count_iso_defaults = (sizeof(iso_defaults) / sizeof(struct const_iso));

Phreeqc::Phreeqc(PHRQ_io *io)
{
	if (io)
	{
		phrq_io = io;
	}
	else
	{
		phrq_io = &this->ioInstance;
	}

	phast = FALSE;
	s_pTail = NULL;
	user_database = NULL;
	rates = NULL;
	tally_table = NULL;
	spec = NULL;
	cations = NULL;
	anions = NULL;
	neutrals = NULL;
	IPRSNT = NULL;
	M = NULL;
	LGAMMA = NULL;
	sit_params = NULL;
	sit_LGAMMA = NULL;
	sit_IPRSNT = NULL;
	sit_M = NULL;

	// counters for enum KEYWORDS
	for (int i = 0; i < Keywords::KEY_COUNT_KEYWORDS; i++)
	{
		keycount.push_back(0);
	}

	// basic.c
	basic_interpreter = NULL;

	//cl1.c
	x_arg = NULL, res_arg = NULL, scratch = NULL;
	x_arg_max = 0, res_arg_max = 0, scratch_max = 0;

	// dw.c
	GASCON = 0.461522e0;
	TZ = 647.073e0;
	AA = 1.e0;
	G1 = 11.e0;
	G2 = 44.333333333333e0;
	GF = 3.5e0;

	// model.c
	min_value = 1e-10;
	normal = NULL;
	ineq_array = NULL;
	res = NULL;
	cu = NULL;
	zero =	NULL;
	delta1 = NULL;
	iu = NULL;
	is = NULL;
	back_eq = NULL;
	normal_max = 0;
	ineq_array_max = 0;
	res_max = 0;
	cu_max = 0;
	zero_max = 0;
	delta1_max = 0;
	iu_max = 0;
	is_max = 0;
	back_eq_max = 0;

	// output.c
	forward_output_to_log = 0;

	// phqalloc.c

	// print.c
	sformatf_buffer = (char *) PHRQ_malloc(256 * sizeof(char));
	if (sformatf_buffer == NULL) 
		malloc_error();
	sformatf_buffer_size = 256;

	// transport.c
	J_ij = NULL;
	J_ij_il = NULL;
	m_s = NULL;

	default_data_base = string_duplicate("phreeqc.dat");

	init();
}

Phreeqc::~Phreeqc(void)
{

	clean_up();

	free_check_null(default_data_base);
	free_check_null(sformatf_buffer);

	PHRQ_free_all();
	if (phrq_io == &ioInstance)
	{
		this->phrq_io->clear_istream();
		this->phrq_io->close_ostreams();
	}
}

void Phreeqc::set_phast(int tf)
{
	this->phast = tf;
}
size_t Phreeqc::list_components(std::list<std::string> &list_c)
/*
 *	 Find all elements in any class definition
 */
{
	cxxNameDouble accumulator;
	//accumulator.add("H", 1);
	//accumulator.add("O", 1);

	// solutions
	{
		std::map<int, cxxSolution>::const_iterator cit = Rxn_solution_map.begin();
		for (; cit !=  Rxn_solution_map.end(); cit++)
		{
			cxxSolution entity(cit->second);
			accumulator.add_extensive(entity.Get_totals(), 1.0);
		}
	}

	// irreversible reactions
	{
		std::map<int, cxxReaction>::const_iterator cit = Rxn_reaction_map.begin();
		for (; cit !=  Rxn_reaction_map.end(); cit++)
		{
			cxxReaction r_ptr(cit->second);
			reaction_calc(&r_ptr);
			accumulator.add_extensive(r_ptr.Get_elementList(), 1.0);
		}
	}

	// pure phases
	{
		std::map<int, cxxPPassemblage>::const_iterator cit = Rxn_pp_assemblage_map.begin();
		for (; cit !=  Rxn_pp_assemblage_map.end(); cit++)
		{
			cxxPPassemblage entity = cit->second;
			entity.totalize(this);
			accumulator.add_extensive(entity.Get_eltList(), 1.0);
		}
	}
	// exchangers
	{
		std::map<int, cxxExchange>::const_iterator cit = Rxn_exchange_map.begin();
		for (; cit !=  Rxn_exchange_map.end(); cit++)
		{
			cxxExchange entity = cit->second;
			entity.totalize();
			accumulator.add_extensive(entity.Get_totals(), 1.0);
		}
	}

	// surfaces
	{
		std::map<int, cxxSurface>::const_iterator cit = Rxn_surface_map.begin();
		for (; cit !=  Rxn_surface_map.end(); cit++)
		{
			cxxSurface entity = cit->second;
			entity.totalize();
			accumulator.add_extensive(entity.Get_totals(), 1.0);
		}
	}
	// gas phases
	{
		std::map<int, cxxGasPhase>::const_iterator cit = Rxn_gas_phase_map.begin();
		for (; cit !=  Rxn_gas_phase_map.end(); cit++)
		{
			cxxGasPhase entity = cit->second;
			entity.totalize(this);
			accumulator.add_extensive(entity.Get_totals(), 1.0);
		}
	}

	// solid-solutions
	{
		std::map<int, cxxSSassemblage>::const_iterator cit = Rxn_ss_assemblage_map.begin();
		for (; cit !=  Rxn_ss_assemblage_map.end(); cit++)
		{
			cxxSSassemblage entity = cit->second;
			entity.totalize(this);
			accumulator.add_extensive(entity.Get_totals(), 1.0);
		}
	}
	// kinetics
	{
		std::map<int, cxxKinetics>::iterator it = Rxn_kinetics_map.begin();
		for (; it !=  Rxn_kinetics_map.end(); it++)
		{
			calc_dummy_kinetic_reaction_tally(&(it->second));
			cxxKinetics entity = it->second;
			accumulator.add_extensive(entity.Get_totals(), 1.0);
		}
	}
	// Put in all primaries
	cxxNameDouble::iterator it;
	for (it = accumulator.begin(); it != accumulator.end(); it++)
	{
		if (it->first == "Charge") continue;
		char string[MAX_LENGTH];
		strcpy(string, it->first.c_str());
		struct master *master_ptr = master_bsearch_primary(string);
		if (master_ptr == NULL) continue;
		if (master_ptr->type != AQ) continue;
		accumulator.add(master_ptr->elt->name, 1);
	}
	// print list
	for (it = accumulator.begin(); it != accumulator.end(); it++)
	{
		struct master *master_ptr = master_bsearch(it->first.c_str());
		if (master_ptr == NULL) continue;
		if (master_ptr->type != AQ) continue;
		if (master_ptr->primary == 0) continue;
		if (it->first == "Charge") continue;
		if (it->first == "O") continue;
		if (it->first == "H") continue;
		list_c.push_back(it->first);
	}
	return(list_c.size());
}
void Phreeqc::init(void)
{
	int i;

	moles_per_kilogram_string = 0;
	pe_string = 0;

	debug_model = FALSE;
	debug_prep = FALSE;
	debug_set = FALSE;
	debug_diffuse_layer = FALSE;
	debug_inverse = FALSE;
	itmax = 100;
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
	step_size				= 100.;
	pe_step_size			= 10.;
	pp_scale				= 1.0;
	pp_column_scale			= 1.0;
	diagonal_scale			= FALSE;
	censor					= 0.0;
	mass_water_switch		= FALSE;
	delay_mass_water		= FALSE;
	incremental_reactions	= FALSE;
	aqueous_only			= 0;
	negative_concentrations = FALSE;

	LOG_10 = log(10.0);

	max_elements			= MAX_ELEMENTS;
	max_elts				= MAX_ELTS;
	max_line				= MAX_LINE;
	max_master				= MAX_MASTER;
	max_mb_unknowns			= MAX_TRXN;
	max_phases				= MAX_PHASES;
	max_s					= MAX_S;
	max_strings				= MAX_STRINGS;
	max_trxn				= MAX_TRXN;
	max_logk				= MAX_S;
	max_master_isotope		= MAX_ELTS;

	count_elements			= 0;
	count_master			= 0;
	count_phases			= 0;
	count_s					= 0;
	count_logk				= 0;
	count_master_isotope	= 0;
/*
 *	 Initialize advection
 */
	count_ad_cells			= 1;
	count_ad_shifts			= 1;
	print_ad_modulus		= 1;
	punch_ad_modulus		= 1;

	advection_punch			= 0;
	advection_kin_time		= 0.0;
	advection_kin_time_defined = FALSE;
	advection_print			= 0;
	advection_warnings		= TRUE;
/*
 *	 Initialize transport
 */
	count_cells				= 1;
	count_shifts			= 1;
	ishift					 = 1;
	bcon_first				= bcon_last = 3;
	diffc					= 0.3e-9;
	simul_tr				= 0;
	tempr					= 2.0;
	heat_diffc				= -0.1;
	timest					= 0.0;
	multi_Dflag				= FALSE;
	interlayer_Dflag		= FALSE;
	interlayer_tortf		= 100.0;
	interlayer_Dpor			= 0.1;
/* !!!!				count_stag = 0; */
	mcd_substeps			= 1.0;
	print_modulus			= 1;
	punch_modulus			= 1;
	dump_modulus			= 0;
	dump_in					= FALSE;
	transport_warnings		= TRUE;

	cell_data				= 0;
	elements				= 0;
	elt_list				= 0;

	inverse					= 0;
	count_inverse			= 0;
	line					= 0;
	line_save				= 0;

	master					= 0;

	mb_unknowns				= 0;
/* !!!! */
	stag_data				= 0;
	phases					= 0;
	trxn.token				= 0;
	s						= 0;
	logk					= 0;
	master_isotope			= 0;

	title_x					= NULL;
	//pe_x					= NULL;
	description_x			= NULL;
	units_x					= NULL;
	s_x						= NULL;

	sum_mb1					= NULL;
	sum_mb2					= NULL;
	sum_jacob0				= NULL;
	sum_jacob1				= NULL;
	sum_jacob2				= NULL;
	sum_delta				= NULL;

	//isotopes_x				= 0;

	x						= NULL;
	max_unknowns = 0;

	array					= NULL;
	delta					= NULL;
	residual				= NULL;
	s_h2o					= NULL;
	s_hplus					= NULL;
	s_h3oplus				= NULL;
	s_eminus				= NULL;
	s_co3					= NULL;
	s_h2					= NULL;
	s_o2					= NULL;

	logk_hash_table			= 0;
	master_isotope_hash_table = 0;
	elements_hash_table		= 0;
	species_hash_table		= 0;
	phases_hash_table		= 0;
/*
 *	 Initialize punch
 */
	punch.in				= FALSE;
	punch.count_totals		= 0;
	punch.totals			= 0;
	punch.count_molalities	= 0;

	punch.molalities		= 0;
	punch.count_activities	= 0;

	punch.activities		= 0;
	punch.count_pure_phases = 0;

	punch.pure_phases		= 0;
	punch.count_si			= 0;

	punch.si				= 0;
	punch.count_gases		= 0;

	punch.gases				= 0;
	punch.count_s_s			= 0;
	punch.s_s = 0;

	punch.count_kinetics	= 0;
	punch.kinetics			= 0;

	punch.count_isotopes	= 0;
	punch.isotopes			= 0;

	punch.count_calculate_values = 0;
	punch.calculate_values	= 0;

	count_save_values		= 0;
	save_values				= 0;


	punch.inverse			= TRUE;
	punch.sim				= TRUE;
	punch.state				= TRUE;
	punch.soln				= TRUE;
	punch.dist				= TRUE;
	punch.time				= TRUE;
	punch.step				= TRUE;
	punch.rxn				= FALSE;
	punch.temp				= FALSE;
	punch.ph				= TRUE;
	punch.pe				= TRUE;
	punch.alk				= FALSE;
	punch.mu				= FALSE;
	punch.water				= FALSE;
	punch.high_precision	= FALSE;
	punch.user_punch		= TRUE;
	punch.charge_balance	= FALSE;
	punch.percent_error		= FALSE;
/*
 *	 last model
 */
	last_model.exchange		= NULL;
	last_model.gas_phase	= NULL;
	last_model.ss_assemblage = NULL;
	last_model.kinetics		= NULL;
	last_model.pp_assemblage = NULL;
	last_model.add_formula	= NULL;
	last_model.si			= NULL;
	last_model.surface_comp	= NULL;
	last_model.surface_charge = NULL;
/*
 *	 rates
 */
	rates					= 0;
	count_rates				= 0;
	initial_total_time		= 0;
	rate_m					= 0;
	rate_m0					= 0;
	rate_time				= 0;
	rate_sim_time_start		= 0;
	rate_sim_time_end		= 0;
	rate_sim_time			= 0;
	rate_moles				= 0;

/*
 *	 user_print, user_punch
 */
	user_print				= 0;
	user_punch				= 0;
	user_punch_headings		= 0;
	user_punch_count_headings = 0;
	n_user_punch_index      = 0;
	/*
		 Initialize llnl aqueous model parameters
	 */
	llnl_temp				= 0;
	llnl_count_temp			= 0;

	llnl_adh				= 0;
	llnl_count_adh			= 0;

	llnl_bdh				= 0;
	llnl_count_bdh			= 0;

	llnl_bdot				= 0;
	llnl_count_bdot			= 0;

	llnl_co2_coefs			= 0;
	llnl_count_co2_coefs	= 0;


	change_surf				= 0;
	change_surf_count		= 0;

	/* Initialize print here, not in global.h */
	pr.all					= TRUE;
	pr.initial_solutions	= TRUE;
	pr.initial_exchangers	= TRUE;
	pr.reactions			= TRUE;
	pr.gas_phase			= TRUE;
	pr.ss_assemblage		= TRUE;
	pr.pp_assemblage		= TRUE;
	pr.surface				= TRUE;
	pr.exchange				= TRUE;
	pr.kinetics				= TRUE;
	pr.totals				= TRUE;
	pr.eh					= TRUE;
	pr.species				= TRUE;
	pr.saturation_indices	= TRUE;
	pr.irrev				= TRUE;
	pr.mix					= TRUE;
	pr.reaction				= TRUE;
	pr.use					= TRUE;
	pr.logfile				= FALSE;
	pr.punch				= TRUE;
	if (phast == TRUE)
	{
		pr.status			= FALSE;
	}
	else
	{
		pr.status			= TRUE;
	}
	pr.inverse				= TRUE;
	pr.dump					= TRUE;
	pr.user_print			= TRUE;
	pr.headings				= TRUE;
	pr.user_graph			= TRUE;
	pr.echo_input			= TRUE;
	count_warnings			= 0;
	pr.warnings				= 100;
	pr.initial_isotopes		= TRUE;
	pr.isotope_ratios		= TRUE;
	pr.isotope_alphas		= TRUE;
	pr.hdf					= FALSE;
	pr.alkalinity			= FALSE;
	species_list			= NULL;
	user_database			= NULL;
	first_read_input		= TRUE;
	have_punch_name			= FALSE;
	selected_output_file_name = NULL;
	dump_file_name			= NULL;

	/* calculate_value */
	max_calculate_value		= MAX_ELTS;
	count_calculate_value	= 0;

	calculate_value			= 0;
	calculate_value_hash_table = 0;

	/* isotope_ratio */
	max_isotope_ratio		= MAX_ELTS;
	count_isotope_ratio		= 0;
	isotope_ratio			= 0;
	isotope_ratio_hash_table = 0;

	/* isotope_value */
	max_isotope_alpha		= MAX_ELTS;
	count_isotope_alpha		= 0;
	isotope_alpha			= 0;
	isotope_alpha_hash_table = 0;

	phreeqc_mpi_myself		= 0;

	copy_solution.n_user		= copy_solution.start		= copy_solution.end			= 0;
	copy_pp_assemblage.n_user	= copy_pp_assemblage.start	= copy_pp_assemblage.end	= 0;
	copy_exchange.n_user		= copy_exchange.start		= copy_exchange.end			= 0;
	copy_surface.n_user			= copy_surface.start		= copy_surface.end			= 0;
	copy_ss_assemblage.n_user	= copy_ss_assemblage.start = copy_ss_assemblage.end		= 0;
	copy_gas_phase.n_user		= copy_gas_phase.start		= copy_gas_phase.end		= 0;
	copy_kinetics.n_user		= copy_kinetics.start		= copy_kinetics.end			= 0;
	copy_mix.n_user				= copy_mix.start			= copy_mix.end				= 0;
	copy_reaction.n_user		= copy_reaction.start		= copy_reaction.end			= 0;
	copy_temperature.n_user		= copy_temperature.start	= copy_temperature.end		= 0;
	copy_pressure.n_user		= copy_pressure.start		= copy_pressure.end			= 0;

	set_forward_output_to_log(FALSE);
	simulation				= 0;
	/*
	 *	cvode
	 */

	cvode_init();
	/*
	 *	Pitzer
	 */
	pitzer_model			= FALSE;
	max_pitz_param			= 100;
	count_pitz_param		= 0;
	use_etheta				= TRUE;
	pitz_params				= 0;

	max_theta_param			= 100;
	count_theta_param		= 0;
	theta_params			= 0;

	ICON					= TRUE;
	OTEMP					= 0.0;
	for (i = 0; i < 23; i++)
	{
		BK[i]				= 0.0;
		DK[i]				= 0.0;
	}
	pitzer_pe				= FALSE;

	count_pp = count_pg = count_ss = 0; // used in store_get_equi_reactants
	x0_moles = NULL;
	/*
	 *	SIT
	 */
	sit_model				= FALSE;
	max_sit_param			= 100;
	count_sit_param			= 0;
	sit_params				= 0;

	/*
	 * to facilitate debuging
	 */
	dbg_master				= master;
	calculating_deriv		= FALSE;
	numerical_deriv			= FALSE;

	zeros				= 0;
	zeros_max			= 1;

	cell_pore_volume	= 0;
	cell_volume			= 0;
	cell_porosity		= 0;
	cell_saturation		= 0;

	print_density		= 0;

	/* basic.c */

	/* dw.c */
	Q0 = 0;
	Q5 = 0;
	Z = 0;
	DZ = 0;
	Y = 0;
	B1 = 0;
	B2 = 0;
	B1T = 0;
	B2T = 0;
	B1TT = 0;
	B2TT = 0;

	/* input.c */
	reading_db = FALSE;

	/* integrate.c */
	z_global = 0;
	xd_global = 0;
	alpha_global = 0;

	/* inverse.c */
	max_row_count = 50;
	max_column_count = 50;
	carbon = FALSE;
	col_name = NULL;
	row_name = NULL;
	count_rows = 0;
	count_optimize = 0;
	col_phases = 0;
	col_redox = 0;
	col_epsilon = 0;
	col_ph = 0;
	col_water = 0;
	col_isotopes = 0;
	col_phase_isotopes = 0;
	row_mb = 0;
	row_fract = 0;
	row_charge = 0;
	row_carbon = 0;
	row_isotopes = 0;
	row_epsilon = 0;
	row_isotope_epsilon = 0;
	row_water = 0;
	inv_zero = NULL;
	array1 = 0;
	inv_delta1 = NULL;
	delta2 = NULL;
	delta3 = NULL;
	inv_cu = NULL;
	delta_save = NULL;
	min_delta = NULL;
	max_delta = NULL;
	klmd = 0;
	nklmd = 0;
	n2d = 0;
	kode = 0;
	iter = 0;
	toler = 0;
	error = 0;
	max_pct = 0;
	scaled_error = 0;
	master_alk = NULL;
	row_back = NULL;
	col_back = NULL;
	good = NULL;
	bad = NULL;
	minimal = NULL;
	max_good = 0;
	max_bad = 0;
	max_minimal = 0;
	count_good = 0;
	count_bad = 0;
	count_minimal = 0;
	count_calls = 0;
	soln_bits = 0;
	phase_bits = 0;
	current_bits = 0;
	temp_bits = 0;
	netpath_file = NULL;
	count_inverse_models = 0;
	count_pat_solutions = 0;
	inv_res = NULL;
	inv_iu = NULL;
	inv_is = NULL;
	/* kinetics.c */
	m_original = NULL;
	m_temp = NULL;
	rk_moles = NULL;

	/* pitzer.c */
	A0 = 0;
	count_cations = 0;
	count_anions = 0;
	count_neutrals = 0;
	MAXCATIONS = 0;
	FIRSTANION = 0;
	MAXNEUTRAL = 0;
	mcb0 = NULL;
	mcb1 = NULL;
	mcc0 = NULL;
	/* read.c */
	dummy = 0;
	prev_next_char = NULL;
	/* sit.c */
	sit_A0 = 0;
	sit_count_cations = 0;
	sit_count_anions = 0;
	sit_count_neutrals = 0;
	sit_MAXCATIONS = 0;
	sit_FIRSTANION = 0;
	sit_MAXNEUTRAL = 0;
	/* tidy.c */
	a0 = 0;
	a1 = 0;
	kc = 0;
	kb = 0;
	/* tally.c */
	t_buffer = NULL;
	tally_count_component = 0;
	count_tally_table_columns = 0;
	count_tally_table_rows = 0;
	/* transport.c */
	sol_D = NULL;
	sol_D_dbg = NULL;
	J_ij_count_spec = 0;
	count_m_s = 0;
	tot1_h = 0;
	tot1_o = 0;
	tot2_h = 0;
	tot2_o = 0;
	diffc_max = 0;
	diffc_tr = 0;
	J_ij_sum = 0;
	transp_surf = FALSE;
	heat_mix_array = NULL;
	temp1 = NULL;
	temp2 = NULL;
	nmix = 0;
	heat_nmix = 0;
	heat_mix_f_imm = 0;
	heat_mix_f_m = 0;
	warn_MCD_X = 0;
	warn_fixed_Surf = 0;
	/* model.c */
	gas_in = FALSE;

	count_strings = 0;
#ifdef MULTICHART
	chart_handler.Set_io(phrq_io);
#endif
	run_info.Set_io(phrq_io);

	phrq_io->clear_istream();

	return;
}
