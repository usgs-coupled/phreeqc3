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

Phreeqc::Phreeqc(void)
{
	phrq_io = new PHRQ_io;

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

	struct const_iso
	{
		const char *name;
		LDBLE value;
		LDBLE uncertainty;
	};


	struct const_iso temp_iso_defaults[] = {
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
	int temp_count_iso_defaults =
		(sizeof(temp_iso_defaults) / sizeof(struct const_iso));

	count_iso_defaults = temp_count_iso_defaults;
	iso_defaults = new iso[count_iso_defaults];

	int i;
	for (i = 0; i < temp_count_iso_defaults; i++)
	{
		iso_defaults[i].name = string_duplicate(temp_iso_defaults[i].name);
		iso_defaults[i].value = temp_iso_defaults[i].value;
		iso_defaults[i].uncertainty = temp_iso_defaults[i].uncertainty;
	}


	struct const_key keyword_temp[] = {
	{"eof", 0},
	{"end", 0},
	{"solution_species", 0},
	{"solution_master_species", 0},
	{"solution", 0},
	{"phases", 0},
	{"pure_phases", 0},
	{"reaction", 0},
	{"mix", 0},
	{"use", 0},
	{"save", 0},
	{"exchange_species", 0},
	{"exchange_master_species", 0},
	{"exchange", 0},
	{"surface_species", 0},
	{"surface_master_species", 0},
	{"surface", 0},
	{"reaction_temperature", 0},
	{"inverse_modeling", 0},
	{"gas_phase", 0},
	{"transport", 0},
	{"debug", 0},
	{"selected_output", 0},
	{"select_output", 0},
	{"knobs", 0},
	{"print", 0},
	{"equilibrium_phases", 0},
	{"equilibria", 0},
	{"equilibrium", 0},
	{"pure", 0},
	{"title", 0},
	{"comment", 0},
	{"advection", 0},
	{"kinetics", 0},
	{"incremental_reactions", 0},
	{"incremental", 0},
	{"rates", 0},
	{"solution_s", 0},
	{"user_print", 0},
	{"user_punch", 0},
	{"solid_solutions", 0},
	{"solid_solution", 0},
	{"solution_spread", 0},
	{"spread_solution", 0},
	{"selected_out", 0},
	{"select_out", 0},
	{"user_graph", 0},
	{"llnl_aqueous_model_parameters", 0},
	{"llnl_aqueous_model", 0},
	{"database", 0},
	{"named_analytical_expression", 0},
	{"named_analytical_expressions", 0},
	{"named_expressions", 0},
	{"named_log_k", 0},
	{"isotopes", 0},
	{"calculate_values", 0},
	{"isotope_ratios", 0},
	{"isotope_alphas", 0},
	{"copy", 0},
	{"pitzer", 0},
	{"sit", 0},
	{"equilibrium_phase", 0}
	,
	{"solution_raw", 0},
	{"exchange_raw", 0},
	{"surface_raw", 0},
	{"equilibrium_phases_raw", 0},
	{"kinetics_raw", 0},
	{"solid_solutions_raw", 0},
	{"gas_phase_raw", 0},
	{"reaction_raw", 0},
	{"mix_raw", 0},
	{"reaction_temperature_raw", 0},
	{"dump", 0},
	{"solution_modify", 0},
	{"equilibrium_phases_modify", 0},
	{"exchange_modify", 0},
	{"surface_modify", 0},
	{"solid_solutions_modify", 0},
	{"gas_phase_modify", 0},
	{"kinetics_modify", 0},
	{"delete", 0},
	{"run_cells", 0},
	{"reaction_modify", 0},
	{"reaction_temperature_modify", 0},
	{"solid_solution_modify", 0}
	};
	NKEYS = (sizeof(keyword_temp) / sizeof(struct const_key));	/* Number of valid keywords */


	//keyword = (struct const_key *) PHRQ_malloc((size_t) (NKEYS * sizeof(const_key)));
	keyword = new const_key[NKEYS];
	for (i = 0; i < NKEYS; i++)
	{
		keyword[i].name = string_duplicate(keyword_temp[i].name);
		keyword[i].keycount = 0;
	}

	// basic.c
	struct const_key command_temp[] = {
		{"+", tokplus},
		{"-", tokminus},
		{"*", toktimes},
		{"/", tokdiv},
		{"^", tokup},
		{"( or [", toklp},
		{") or ]", tokrp},
		{",", tokcomma},
		{";", toksemi},
		{":", tokcolon},
		{"=", tokeq},
		{"<", toklt},
		{"<=", tokle},
		{">", tokgt},
		{">=", tokge},
		{"and", tokand},
		{"or", tokor},
		{"xor", tokxor},
		{"not", toknot},
		{"mod", tokmod},
		{"sqr", toksqr},
		{"sqrt", toksqrt},
		{"ceil", tokceil},
		{"floor", tokfloor},
		{"sin", toksin},
		{"cos", tokcos},
		{"tan", toktan},
		{"arctan", tokarctan},
		{"log", toklog},
		{"exp", tokexp},
		{"abs", tokabs},
		{"sgn", toksgn},
		{"str$", tokstr_},
		{"val", tokval},
		{"chr$", tokchr_},
		{"eol$", tokeol_},
		{"asc", tokasc},
		{"len", toklen},
		{"mid$", tokmid_},
		{"peek", tokpeek},
		{"let", toklet},
		{"print", tokprint},
		{"punch", tokpunch},
	#if defined PHREEQ98 || defined MULTICHART
		{"graph_x", tokgraph_x},
		{"graph_y", tokgraph_y},
		{"graph_sy", tokgraph_sy},
	#endif
	#if defined MULTICHART
		{"plot_xy", tokplot_xy},
	#endif
		{"input", tokinput},
		{"goto", tokgoto},
		{"go to", tokgoto},
		{"if", tokif},
		{"end", tokend},
		{"stop", tokstop},
		{"for", tokfor},
		{"next", toknext},
		{"while", tokwhile},
		{"wend", tokwend},
		{"gosub", tokgosub},
		{"return", tokreturn},
		{"read", tokread},
		{"data", tokdata},
		{"restore", tokrestore},
		{"gotoxy", tokgotoxy},
		{"on", tokon},
		{"dim", tokdim},
		{"poke", tokpoke},
		{"list", toklist},
		{"run", tokrun},
		{"new", toknew},
		{"load", tokload},
		{"merge", tokmerge},
		{"save", toksave},
		{"bye", tokbye},
		{"quit", tokbye},
		{"del", tokdel},
		{"renum", tokrenum},
		{"then", tokthen},
		{"else", tokelse},
		{"to", tokto},
		{"step", tokstep},
		{"tc", toktc},
		{"tk", toktk},
		{"time", toktime},
		{"sim_time", toksim_time},
		{"total_time", toktotal_time},
		{"m0", tokm0},
		{"m", tokm},
		{"parm", tokparm},
		{"act", tokact},
		{"edl", tokedl},
		{"surf", toksurf},
		{"equi", tokequi},
		{"kin", tokkin},
		{"gas", tokgas},
		{"s_s", toks_s},
		{"misc1", tokmisc1},
		{"misc2", tokmisc2},
		{"mu", tokmu},
		{"osmotic", tokosmotic},
		{"alk", tokalk},
		{"lk_species", toklk_species},
		{"lk_named", toklk_named},
		{"lk_phase", toklk_phase},
		{"sum_species", toksum_species},
		{"sum_gas", toksum_gas},
		{"sum_s_s", toksum_s_s},
		{"calc_value", tokcalc_value},
		{"description", tokdescription},
		{"sys", toksys},
		{"instr", tokinstr},
		{"ltrim", tokltrim},
		{"rtrim", tokrtrim},
		{"trim", toktrim},
		{"pad", tokpad},
		{"rxn", tokrxn},
		{"dist", tokdist},
		{"mol", tokmol},
		{"la", tokla},
		{"lm", toklm},
		{"sr", toksr},
		{"si", toksi},
		{"step_no", tokstep_no},
		{"cell_no", tokcell_no},
		{"sim_no", toksim_no},
		{"tot", toktot},
		{"log10", toklog10},
		{"charge_balance", tokcharge_balance},
		{"percent_error", tokpercent_error},
		{"put", tokput},
		{"get", tokget},
		{"exists", tokexists},
		{"rem", tokrem},
		{"change_por", tokchange_por},
		{"get_por", tokget_por},
		{"change_surf", tokchange_surf},
		{"porevolume", tokporevolume},
		{"sc", toksc},
		{"gamma", tokgamma},
	/* VP: Density Start */
		{"lg", toklg},
		{"rho", tokrho},
	/* VP: Density End */
		{"cell_volume", tokcell_volume},
		{"cell_pore_volume", tokcell_pore_volume},
		{"cell_porosity", tokcell_porosity},
		{"cell_saturation", tokcell_saturation},
		{"totmole", toktotmole},
		{"totmol", toktotmol},
		{"totmoles", toktotmoles},
		{"iso", tokiso},
		{"iso_unit", tokiso_unit},
		{"phase_formula", tokphase_formula},
		{"list_s_s", toklist_s_s},
		{"pr_p", tokpr_p},
 		{"pr_phi", tokpr_phi},
 		{"gas_p", tokgas_p},
 		{"gas_vm", tokgas_vm}
		};
	NCMDS = (sizeof(command_temp) / sizeof(struct const_key));
	command = new const_key[NCMDS];
	for (i = 0; i < NCMDS; i++)
	{
		command[i].name = string_duplicate(command_temp[i].name);
		command[i].keycount = command_temp[i].keycount;
	}

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
	this->phrq_io->close_input_files();
	this->phrq_io->close_output_files();

	int i;
	for (i = 0; i < count_iso_defaults; i++)
	{
		iso_defaults[i].name = (char *) free_check_null((void *) iso_defaults[i].name);
	}
	delete[] iso_defaults;
	for (i = 0; i < NKEYS; i++)
	{
		keyword[i].name = (char *) free_check_null((void *) keyword[i].name);
	}
	delete[] keyword;
	for (i = 0; i < NCMDS; i++)
	{
		command[i].name = (char *) free_check_null((void *) command[i].name);
	}
	delete[] command;

	free_check_null(default_data_base);
	free_check_null(sformatf_buffer);

	PHRQ_free_all();
	delete phrq_io;
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

	int i;

	// solutions
	for (i = 0; i < count_solution; i++)
	{
		cxxSolution entity(solution[i], phrq_io);
		accumulator.add_extensive(entity.Get_totals(), 1.0);
	}

	// irreversible reactions
	for (i = 0; i < count_irrev; i++)
	{
		reaction_calc(&irrev[i]);
		cxxReaction entity(&irrev[i]);
		accumulator.add_extensive(entity.Get_elementList(), 1.0);
	}

	// pure phases
	for (i = 0; i < count_pp_assemblage; i++)
	{
		cxxPPassemblage entity(&pp_assemblage[i], phrq_io);
		entity.totalize(this);
		accumulator.add_extensive(entity.Get_totals(), 1.0);
	}

	// exchangers
	for (i = 0; i < count_exchange; i++)
	{
		cxxExchange entity(&exchange[i], phrq_io);
		entity.totalize();
		accumulator.add_extensive(entity.Get_totals(), 1.0);
	}

	// surfaces
	for (i = 0; i < count_surface; i++)
	{
		cxxSurface entity(&surface[i], phrq_io);
		entity.totalize();
		accumulator.add_extensive(entity.Get_totals(), 1.0);
	}

	// gas phases
	for (i = 0; i < count_gas_phase; i++)
	{
		cxxGasPhase entity(&gas_phase[i], phrq_io);
		entity.totalize(this);
		accumulator.add_extensive(entity.Get_totals(), 1.0);
	}

	// solid-solutions
	for (i = 0; i < count_s_s_assemblage; i++)
	{
		cxxSSassemblage entity(&s_s_assemblage[i]);
		entity.totalize(this);
		accumulator.add_extensive(entity.Get_totals(), 1.0);
	}

	// kinetics
	for (i = 0; i < count_kinetics; i++)
	{
		calc_dummy_kinetic_reaction_tally(&kinetics[i]);
		cxxKinetics entity(&kinetics[i], phrq_io);
		accumulator.add_extensive(entity.Get_totals(), 1.0);
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
		//std::string name(master_ptr->elt->name);
		//char
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

	max_solution			= MAX_SOLUTION;
	max_pp_assemblage		= MAX_PP_ASSEMBLAGE;
	max_exchange			= MAX_PP_ASSEMBLAGE;
	max_surface				= MAX_PP_ASSEMBLAGE;
	max_gas_phase			= MAX_PP_ASSEMBLAGE;
	max_kinetics			= MAX_PP_ASSEMBLAGE;
	max_s_s_assemblage		= MAX_PP_ASSEMBLAGE;

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

	count_solution			 = 0;
	count_pp_assemblage	= 0;
	count_exchange			 = 0;
	count_surface				= 0;
	count_gas_phase			= 0;
	count_kinetics			 = 0;
	count_s_s_assemblage = 0;

	count_elements			 = 0;
	count_irrev					= 0;
	count_master				 = 0;
	count_mix						= 0;
	count_phases				 = 0;
	count_s							= 0;
	count_temperature		= 0;
	count_logk					 = 0;
	count_master_isotope = 0;
/*
 *	 Initialize advection
 */
	count_ad_cells	 = 1;
	count_ad_shifts	= 1;
	print_ad_modulus = 1;
	punch_ad_modulus = 1;

	advection_punch						= 0;
	advection_kin_time				 = 0.0;
	advection_kin_time_defined = FALSE;
	advection_print						= 0;
	advection_warnings				 = TRUE;
/*
 *	 Initialize transport
 */
	count_cells			= 1;
	count_shifts		 = 1;
	ishift					 = 1;
	bcon_first			 = bcon_last = 3;
	diffc						= 0.3e-9;
	simul_tr				 = 0;
	tempr						= 2.0;
	heat_diffc			 = -0.1;
	timest					 = 0.0;
	multi_Dflag			= FALSE;
	interlayer_Dflag = FALSE;
	interlayer_tortf = 100.0;
	interlayer_Dpor	= 0.1;
/* !!!!				count_stag = 0; */
	mcd_substeps			 = 1.0;
	print_modulus			= 1;
	punch_modulus			= 1;
	dump_modulus			 = 0;
	dump_in						= FALSE;
	transport_warnings = TRUE;

	pp_assemblage	= 0;
	exchange			 = 0;
	surface				= 0;
	gas_phase			= 0;
	kinetics			 = 0;
	s_s_assemblage = 0;
	cell_data			= 0;
	elements			 = 0;
	elt_list			 = 0;


	inverse			 = 0;
	count_inverse = 0;

	irrev = 0;

	line = 0;
	line_save = 0;

	master = 0;

	mb_unknowns = 0;

	mix			 = 0;
	count_mix = 0;
/* !!!! */
	stag_data = 0;

	phases = 0;

	trxn.token = 0;

	s = 0;

	logk = 0;

	master_isotope = 0;

	solution = 0;

	temperature = 0;

	title_x			 = NULL;
	pe_x					= NULL;
	description_x = NULL;
	units_x			 = NULL;
	s_x					 = NULL;

	sum_mb1		= NULL;
	sum_mb2		= NULL;
	sum_jacob0 = NULL;
	sum_jacob1 = NULL;
	sum_jacob2 = NULL;
	sum_delta	= NULL;

	isotopes_x = 0;

	x						= NULL;
	max_unknowns = 0;

	array		 = NULL;
	delta		 = NULL;
	residual	= NULL;
	s_h2o		 = NULL;
	s_hplus	 = NULL;
	s_h3oplus = NULL;
	s_eminus	= NULL;
	s_co3		 = NULL;
	s_h2			= NULL;
	s_o2			= NULL;

	logk_hash_table					 = 0;
	master_isotope_hash_table = 0;
	strings_hash_table				= 0;
	elements_hash_table			 = 0;
	species_hash_table				= 0;
	phases_hash_table				 = 0;
	keyword_hash_table				= 0;
/*
 *	Initialize use pointers
 */
	use.solution_in			= FALSE;
	use.pp_assemblage_in = FALSE;
	use.mix_in					 = FALSE;
	use.irrev_in				 = FALSE;
/*
 *	 Initialize punch
 */
	punch.in							 = FALSE;
	punch.count_totals		 = 0;
	punch.totals					 = 0;
	punch.count_molalities = 0;

	punch.molalities			 = 0;
	punch.count_activities = 0;

	punch.activities				= 0;
	punch.count_pure_phases = 0;

	punch.pure_phases = 0;
	punch.count_si		= 0;

	punch.si					= 0;
	punch.count_gases = 0;

	punch.gases		 = 0;
	punch.count_s_s = 0;
	punch.s_s = 0;

	punch.count_kinetics = 0;
	punch.kinetics = 0;

	punch.count_isotopes = 0;
	punch.isotopes			 = 0;

	punch.count_calculate_values = 0;
	punch.calculate_values			 = 0;

	count_save_values = 0;
	save_values			 = 0;


	punch.inverse = TRUE;

	punch.sim						= TRUE;
	punch.state					= TRUE;
	punch.soln					 = TRUE;
	punch.dist					 = TRUE;
	punch.time					 = TRUE;
	punch.step					 = TRUE;
	punch.rxn						= FALSE;
	punch.temp					 = FALSE;
	punch.ph						 = TRUE;
	punch.pe						 = TRUE;
	punch.alk						= FALSE;
	punch.mu						 = FALSE;
	punch.water					= FALSE;
	punch.high_precision = FALSE;
	punch.user_punch		 = TRUE;
	punch.charge_balance = FALSE;
	punch.percent_error	= FALSE;
/*
 *	 last model
 */
	last_model.exchange			 = NULL;
	last_model.gas_phase			= NULL;
	last_model.s_s_assemblage = NULL;
	last_model.kinetics			 = NULL;
	last_model.pp_assemblage	= NULL;
	last_model.add_formula		= NULL;
	last_model.si						 = NULL;
	last_model.surface_comp	 = NULL;
	last_model.surface_charge = NULL;
/*
 *	 Update hash table
 */
	keyword_hash = 0;
/*
 *	 rates
 */
	rates = 0;
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
 *	 user_print, user_punch
 */
	user_print = 0;
	user_punch = 0;
	user_punch_headings = 0;
	user_punch_count_headings = 0;
	/*
		 Initialize llnl aqueous model parameters
	 */
	llnl_temp = 0;
	llnl_count_temp = 0;

	llnl_adh = 0;
	llnl_count_adh = 0;

	llnl_bdh = 0;
	llnl_count_bdh = 0;

	llnl_bdot = 0;
	llnl_count_bdot = 0;

	llnl_co2_coefs = 0;
	llnl_count_co2_coefs = 0;
/*
 *
 */
	command_hash_table = 0;

	change_surf			 = 0;
	change_surf_count = 0;

	/* Initialize print here, not in global.h */
	pr.all					= TRUE;
	pr.initial_solutions	= TRUE;
	pr.initial_exchangers	= TRUE;
	pr.reactions			= TRUE;
	pr.gas_phase			= TRUE;
	pr.s_s_assemblage		= TRUE;
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
	max_calculate_value = MAX_ELTS;
	count_calculate_value = 0;

	calculate_value = 0;
	calculate_value_hash_table = 0;

	/* isotope_ratio */
	max_isotope_ratio = MAX_ELTS;
	count_isotope_ratio = 0;
	isotope_ratio = 0;
	isotope_ratio_hash_table = 0;

	/* isotope_value */
	max_isotope_alpha = MAX_ELTS;
	count_isotope_alpha = 0;
	isotope_alpha = 0;
	isotope_alpha_hash_table = 0;

	phreeqc_mpi_myself = 0;

	copy_solution.n_user		= copy_solution.start		= copy_solution.end			= 0;
	copy_pp_assemblage.n_user	= copy_pp_assemblage.start	= copy_pp_assemblage.end	= 0;
	copy_exchange.n_user		= copy_exchange.start		= copy_exchange.end			= 0;
	copy_surface.n_user			= copy_surface.start		= copy_surface.end			= 0;
	copy_s_s_assemblage.n_user	= copy_s_s_assemblage.start = copy_s_s_assemblage.end	= 0;
	copy_gas_phase.n_user		= copy_gas_phase.start		= copy_gas_phase.end		= 0;
	copy_kinetics.n_user		= copy_kinetics.start		= copy_kinetics.end			= 0;
	copy_mix.n_user				= copy_mix.start			= copy_mix.end				= 0;
	copy_irrev.n_user			= copy_irrev.start			= copy_irrev.end			= 0;
	copy_temperature.n_user		= copy_temperature.start	= copy_temperature.end		= 0;

	set_forward_output_to_log(FALSE);
	simulation = 0;
	/*
	 *	cvode
	 */

	cvode_init();
	/*
	 *	Pitzer
	 */
	pitzer_model = FALSE;
	max_pitz_param = 100;
	count_pitz_param = 0;
	use_etheta = TRUE;
	pitz_params = 0;

	max_theta_param = 100;
	count_theta_param = 0;
	theta_params = 0;

	ICON = TRUE;
	OTEMP = 0.0;
	for (i = 0; i < 23; i++)
	{
		BK[i] = 0.0;
		DK[i] = 0.0;
	}
	pitzer_pe = FALSE;


	/*
	 *	SIT
	 */
	sit_model			 = FALSE;
	max_sit_param	 = 100;
	count_sit_param = 0;
	sit_params			= 0;

	/*
	 * to facilitate debuging
	 */
	dbg_use				= &use;
	dbg_solution		= solution;
	dbg_exchange		= exchange;
	dbg_surface			= surface;
	dbg_pp_assemblage	= pp_assemblage;
	dbg_kinetics		= kinetics;
	dbg_irrev			= irrev;
	dbg_mix				= mix;
	dbg_master			= master;
	calculating_deriv	= FALSE;
	numerical_deriv		= FALSE;

	zeros				= 0;
	zeros_max			= 1;

	cell_pore_volume	= 0;
	cell_volume			= 0;
	cell_porosity		= 0;
	cell_saturation		= 0;

	charge_group		= NULL;
	print_density		= 0;

	//
	//	Non-class statics
	//
	/* basic.c */
	n_user_punch_index = 0;
	inbuf = NULL;
	linebase = NULL;
	varbase = NULL;
	loopbase = NULL;
	curline = 0;
	stmtline = NULL;
	dataline = NULL;
	stmttok = NULL;
	datatok = NULL;
	buf = NULL;
	exitflag = FALSE;
	EXCP_LINE = 0;
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
	/* integrate.c */
	z = 0;
	xd = 0;
	alpha = 0;
	surface_charge_ptr = NULL;
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
	/* p2clib.c */
	P_argc = 0;
	P_argv = NULL;
	P_escapecode = 0;
	P_ioresult = 0;
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

	this->clear_cookie();

	return;
}


