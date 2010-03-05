#include "Phreeqc.h"
//class Phreeqc
//{
//	Phreeqc(void);
//	~Phreeqc(void);
//};
#include <algorithm>			// std::replace

Phreeqc::Phreeqc(void)
{
	phast = FALSE;
	s_pTail = NULL;
	user_database = NULL;
	output = NULL;
	log_file = NULL;
	punch_file = NULL;
	dump_file = NULL;
	error_file = NULL;
	database_file = NULL;
	input_file = NULL;
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
	{"equilibrium_phase"}
#ifdef PHREEQC_CPP
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
	{"run_cells", 0}
#endif
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
		{"and", tokand},
		{"or", tokor},
		{"xor", tokxor},
		{"not", toknot},
		{"mod", tokmod},
		{"sqr", toksqr},
		{"sqrt", toksqrt},
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
		{"asc", tokasc},
		{"len", toklen},
		{"mid$", tokmid_},
		{"peek", tokpeek},
		{"let", toklet},
		{"print", tokprint},
		{"punch", tokpunch},
#ifdef PHREEQ98
		{"graph_x", tokgraph_x},
		{"graph_y", tokgraph_y},
		{"graph_sy", tokgraph_sy},
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
		{"rho", tokrho}
		/* VP: Density End */
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
	output_callbacks = new Phreeqc::output_callback[MAX_CALLBACKS];
	count_output_callback = 0;
	forward_output_to_log = 0;

	// phqalloc.c
	//s_pTail = NULL;

	// transport.c
	J_ij = NULL;
	J_ij_il = NULL;
	m_s = NULL;

	default_data_base = string_duplicate("phreeqc.dat");

//FILE *input_file = NULL;
//FILE *database_file = NULL;
//FILE *output = NULL;		/* OUTPUT_MESSAGE */
//FILE *log_file = NULL;	/* OUTPUT_LOG */
//FILE *punch_file = NULL;	/* OUTPUT_PUNCH */
//FILE *error_file = NULL;	/* OUTPUT_ERROR */
//FILE *dump_file = NULL;	/* OUTPUT_DUMP */
}

Phreeqc::~Phreeqc(void)
{

	clean_up();
	close_input_files();
	close_output_files();

	int i;
	for (i = 0; i < count_iso_defaults; i++)
	{
		iso_defaults[i].name = (char *) free_check_null((void *) iso_defaults[i].name);
	}
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

	PHRQ_free_all();
}
void Phreeqc::set_phast(int tf)
{
	this->phast = tf;
}
