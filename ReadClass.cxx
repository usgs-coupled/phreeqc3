#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include "Parser.h"
#include "Solution.h"
#include "Exchange.h"
#include "Surface.h"
#include "PPassemblage.h"
#include "cxxKinetics.h"
#include "SSassemblage.h"
#include "GasPhase.h"
#include "Reaction.h"
#include "cxxMix.h"
#include "Temperature.h"
#include "dumper.h"
#define EXTERNAL extern
#include "global.h"
#include "phqalloc.h"
#include "output.h"
#include "phrqproto.h"
#include <iostream>
#include <fstream>
#include <sstream>

static int streamify_to_next_keyword(std::istringstream & lines);
extern int reading_database(void);
extern int check_line(const char *string, int allow_empty, int allow_eof,
					  int allow_keyword, int print);
dumper dump_info;
/* ---------------------------------------------------------------------- */
int
read_solution_raw(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads SOLUTION_RAW data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;
	/*
	 *  Accumulate lines in std string
	 */
	std::string keywordLines("");

	keywordLines.append(line);
	keywordLines.append("\n");
/*
 *   Read additonal lines
 */
	int save_echo_input = pr.echo_input;
	pr.echo_input = FALSE;
	for (;;)
	{
		return_value = check_line("solution_raw", TRUE, TRUE, TRUE, FALSE);
		/* empty, eof, keyword, print */
		if (return_value == EOF || return_value == KEYWORD)
			break;
		keywordLines.append(line);
		keywordLines.append("\n");
	}
	pr.echo_input = save_echo_input;

	std::istringstream iss_in(keywordLines);
	std::ostringstream oss_out;
	std::ostringstream oss_err;

	CParser parser(iss_in, oss_out, oss_err);
	assert(!reading_database());
	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}
	//For testing, need to read line to get started
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);


	cxxSolution sol;
	sol.read_raw(parser);
	struct solution *soln_ptr = sol.cxxSolution2solution();
	int n;

	/*
	 *  This is not quite right, may not produce sort order, forced sort
	 */

	if (solution_bsearch(soln_ptr->n_user, &n, FALSE) != NULL)
	{
		solution_free(solution[n]);
		solution[n] = soln_ptr;
	}
	else
	{
		n = count_solution++;
		if (count_solution >= max_solution)
		{
			space((void **) ((void *) &(solution)), count_solution,
				  &max_solution, sizeof(struct solution *));
		}
		solution[n] = soln_ptr;
		solution_sort();
	}

	// Need to output the next keyword
	output_msg(OUTPUT_CHECKLINE, "\t%s\n", line);
	return (return_value);
}

/* ---------------------------------------------------------------------- */
int
read_exchange_raw(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads EXCHANGE_RAW data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;
	/*
	 *  Accumulate lines in std string
	 */
	std::string keywordLines("");

	keywordLines.append(line);
	keywordLines.append("\n");
/*
 *   Read additonal lines
 */
	int save_echo_input = pr.echo_input;
	pr.echo_input = FALSE;
	for (;;)
	{
		return_value = check_line("exchange_raw", TRUE, TRUE, TRUE, FALSE);
		/* empty, eof, keyword, print */
		if (return_value == EOF || return_value == KEYWORD)
			break;
		keywordLines.append(line);
		keywordLines.append("\n");
	}
	pr.echo_input = save_echo_input;

	std::istringstream iss_in(keywordLines);
	std::ostringstream oss_out;
	std::ostringstream oss_err;

	CParser parser(iss_in, oss_out, oss_err);
	assert(!reading_database());
	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}
	//For testing, need to read line to get started
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);

	cxxExchange ex;
	ex.read_raw(parser);
	struct exchange *exchange_ptr = ex.cxxExchange2exchange();
	int n;

	/*
	 *  This is not quite right, may not produce sort order, forced sort
	 */

	if (exchange_bsearch(exchange_ptr->n_user, &n) != NULL)
	{
		exchange_free(&exchange[n]);
		exchange_copy(exchange_ptr, &exchange[n], exchange_ptr->n_user);
	}
	else
	{
		n = count_exchange++;
		if (count_exchange >= max_exchange)
		{
			space((void **) ((void *) &(exchange)), count_exchange,
				  &max_exchange, sizeof(struct exchange));
		}
		exchange_copy(exchange_ptr, &exchange[n], exchange_ptr->n_user);
		exchange_sort();
	}
	exchange_free(exchange_ptr);
	free_check_null(exchange_ptr);

	// Need to output the next keyword
	output_msg(OUTPUT_CHECKLINE, "\t%s\n", line);
	return (return_value);
}

/* ---------------------------------------------------------------------- */
int
read_surface_raw(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads SURFACE_RAW data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;
	/*
	 *  Accumulate lines in std string
	 */
	std::string keywordLines("");

	keywordLines.append(line);
	keywordLines.append("\n");
/*
 *   Read additonal lines
 */
	int save_echo_input = pr.echo_input;
	pr.echo_input = FALSE;
	for (;;)
	{
		return_value = check_line("surface_raw", TRUE, TRUE, TRUE, FALSE);
		/* empty, eof, keyword, print */
		if (return_value == EOF || return_value == KEYWORD)
			break;
		keywordLines.append(line);
		keywordLines.append("\n");
	}
	pr.echo_input = save_echo_input;

	std::istringstream iss_in(keywordLines);
	std::ostringstream oss_out;
	std::ostringstream oss_err;

	CParser parser(iss_in, oss_out, oss_err);
	assert(!reading_database());
	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}
	//For testing, need to read line to get started
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);

	cxxSurface ex;
	ex.read_raw(parser);
	struct surface *surface_ptr = ex.cxxSurface2surface();
	int n;

	/*
	 *  This is not quite right, may not produce sort order, forced sort
	 */

	if (surface_bsearch(surface_ptr->n_user, &n) != NULL)
	{
		surface_free(&surface[n]);
		surface_copy(surface_ptr, &surface[n], surface_ptr->n_user);
	}
	else
	{
		n = count_surface++;
		if (count_surface >= max_surface)
		{
			space((void **) ((void *) &(surface)), count_surface,
				  &max_surface, sizeof(struct surface));
		}
		surface_copy(surface_ptr, &surface[n], surface_ptr->n_user);
		surface_sort();
	}
	surface_free(surface_ptr);
	free_check_null(surface_ptr);

	// Need to output the next keyword
	output_msg(OUTPUT_CHECKLINE, "\t%s\n", line);
	return (return_value);
}

/* ---------------------------------------------------------------------- */
int
read_equilibrium_phases_raw(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads EQUILIBRIUM_PHASES_RAW data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;
	/*
	 *  Accumulate lines in std string
	 */
	std::string keywordLines("");

	keywordLines.append(line);
	keywordLines.append("\n");
/*
 *   Read additonal lines
 */
	int save_echo_input = pr.echo_input;
	pr.echo_input = FALSE;
	for (;;)
	{
		return_value =
			check_line("equilibrium_phases_raw", TRUE, TRUE, TRUE, FALSE);
		/* empty, eof, keyword, print */
		if (return_value == EOF || return_value == KEYWORD)
			break;
		keywordLines.append(line);
		keywordLines.append("\n");
	}
	pr.echo_input = save_echo_input;

	std::istringstream iss_in(keywordLines);
	std::ostringstream oss_out;
	std::ostringstream oss_err;

	CParser parser(iss_in, oss_out, oss_err);
	assert(!reading_database());
	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}
	//For testing, need to read line to get started
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);

	cxxPPassemblage ex;
	ex.read_raw(parser);
	struct pp_assemblage *pp_assemblage_ptr =
		ex.cxxPPassemblage2pp_assemblage();
	int n;

	/*
	 *  This is not quite right, may not produce sort order, forced sort
	 */

	if (pp_assemblage_bsearch(pp_assemblage_ptr->n_user, &n) != NULL)
	{
		pp_assemblage_free(&pp_assemblage[n]);
		pp_assemblage_copy(pp_assemblage_ptr, &pp_assemblage[n],
						   pp_assemblage_ptr->n_user);
	}
	else
	{
		n = count_pp_assemblage++;
		if (count_pp_assemblage >= max_pp_assemblage)
		{
			space((void **) ((void *) &(pp_assemblage)), count_pp_assemblage,
				  &max_pp_assemblage, sizeof(struct pp_assemblage));
		}
		pp_assemblage_copy(pp_assemblage_ptr, &pp_assemblage[n],
						   pp_assemblage_ptr->n_user);
		pp_assemblage_sort();
	}
	pp_assemblage_free(pp_assemblage_ptr);
	free_check_null(pp_assemblage_ptr);

	// Need to output the next keyword
	output_msg(OUTPUT_CHECKLINE, "\t%s\n", line);
	return (return_value);
}

/* ---------------------------------------------------------------------- */
int
read_kinetics_raw(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads KINETICS_RAW data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;
	/*
	 *  Accumulate lines in std string
	 */
	std::string keywordLines("");

	keywordLines.append(line);
	keywordLines.append("\n");
/*
 *   Read additonal lines
 */
	int save_echo_input = pr.echo_input;
	pr.echo_input = FALSE;
	for (;;)
	{
		return_value = check_line("kinetics_raw", TRUE, TRUE, TRUE, FALSE);
		/* empty, eof, keyword, print */
		if (return_value == EOF || return_value == KEYWORD)
			break;
		keywordLines.append(line);
		keywordLines.append("\n");
	}
	pr.echo_input = save_echo_input;

	std::istringstream iss_in(keywordLines);
	std::ostringstream oss_out;
	std::ostringstream oss_err;

	CParser parser(iss_in, oss_out, oss_err);
	assert(!reading_database());
	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}
	//For testing, need to read line to get started
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);

	cxxKinetics ex;
	ex.read_raw(parser);
	struct kinetics *kinetics_ptr = ex.cxxKinetics2kinetics();
	int n;

	/*
	 *  This is not quite right, may not produce sort order, forced sort
	 */

	if (kinetics_bsearch(kinetics_ptr->n_user, &n) != NULL)
	{
		kinetics_free(&kinetics[n]);
		kinetics_copy(kinetics_ptr, &kinetics[n], kinetics_ptr->n_user);
	}
	else
	{
		n = count_kinetics++;
		if (count_kinetics >= max_kinetics)
		{
			space((void **) ((void *) &(kinetics)), count_kinetics,
				  &max_kinetics, sizeof(struct kinetics));
		}
		kinetics_copy(kinetics_ptr, &kinetics[n], kinetics_ptr->n_user);
		kinetics_sort();
	}
	kinetics_free(kinetics_ptr);
	free_check_null(kinetics_ptr);

	// Need to output the next keyword
	output_msg(OUTPUT_CHECKLINE, "\t%s\n", line);
	return (return_value);
}

/* ---------------------------------------------------------------------- */
int
read_solid_solutions_raw(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads SOLID_SOLUTION_RAW data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;
	/*
	 *  Accumulate lines in std string
	 */
	std::string keywordLines("");

	keywordLines.append(line);
	keywordLines.append("\n");
/*
 *   Read additonal lines
 */
	int save_echo_input = pr.echo_input;
	pr.echo_input = FALSE;
	for (;;)
	{
		return_value =
			check_line("solid_solution_raw", TRUE, TRUE, TRUE, FALSE);
		/* empty, eof, keyword, print */
		if (return_value == EOF || return_value == KEYWORD)
			break;
		keywordLines.append(line);
		keywordLines.append("\n");
	}
	pr.echo_input = save_echo_input;

	std::istringstream iss_in(keywordLines);
	std::ostringstream oss_out;
	std::ostringstream oss_err;

	CParser parser(iss_in, oss_out, oss_err);
	assert(!reading_database());
	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}
	//For testing, need to read line to get started
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);

	cxxSSassemblage ex;
	ex.read_raw(parser);
	struct s_s_assemblage *s_s_assemblage_ptr =
		ex.cxxSSassemblage2s_s_assemblage();
	int n;

	/*
	 *  This is not quite right, may not produce sort order, forced sort
	 */

	if (s_s_assemblage_bsearch(s_s_assemblage_ptr->n_user, &n) != NULL)
	{
		s_s_assemblage_free(&s_s_assemblage[n]);
		s_s_assemblage_copy(s_s_assemblage_ptr, &s_s_assemblage[n],
							s_s_assemblage_ptr->n_user);
	}
	else
	{
		n = count_s_s_assemblage++;
		if (count_s_s_assemblage >= max_s_s_assemblage)
		{
			space((void **) ((void *) &(s_s_assemblage)),
				  count_s_s_assemblage, &max_s_s_assemblage,
				  sizeof(struct s_s_assemblage));
		}
		s_s_assemblage_copy(s_s_assemblage_ptr, &s_s_assemblage[n],
							s_s_assemblage_ptr->n_user);
		s_s_assemblage_sort();
	}
	s_s_assemblage_free(s_s_assemblage_ptr);
	free_check_null(s_s_assemblage_ptr);

	// Need to output the next keyword
	output_msg(OUTPUT_CHECKLINE, "\t%s\n", line);
	return (return_value);
}

/* ---------------------------------------------------------------------- */
int
read_gas_phase_raw(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads GAS_PHASE_RAW data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;
	/*
	 *  Accumulate lines in std string
	 */
	std::string keywordLines("");

	keywordLines.append(line);
	keywordLines.append("\n");
/*
 *   Read additonal lines
 */
	int save_echo_input = pr.echo_input;
	pr.echo_input = FALSE;
	for (;;)
	{
		return_value = check_line("gas_phase_raw", TRUE, TRUE, TRUE, FALSE);
		/* empty, eof, keyword, print */
		if (return_value == EOF || return_value == KEYWORD)
			break;
		keywordLines.append(line);
		keywordLines.append("\n");
	}
	pr.echo_input = save_echo_input;

	std::istringstream iss_in(keywordLines);
	std::ostringstream oss_out;
	std::ostringstream oss_err;

	CParser parser(iss_in, oss_out, oss_err);
	assert(!reading_database());
	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}
	//For testing, need to read line to get started
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);

	cxxGasPhase ex;
	ex.read_raw(parser);
	struct gas_phase *gas_phase_ptr = ex.cxxGasPhase2gas_phase();
	int n;

	/*
	 *  This is not quite right, may not produce sort order, forced sort
	 */

	if (gas_phase_bsearch(gas_phase_ptr->n_user, &n) != NULL)
	{
		gas_phase_free(&gas_phase[n]);
		gas_phase_copy(gas_phase_ptr, &gas_phase[n], gas_phase_ptr->n_user);
	}
	else
	{
		n = count_gas_phase++;
		if (count_gas_phase >= max_gas_phase)
		{
			space((void **) ((void *) &(gas_phase)), count_gas_phase,
				  &max_gas_phase, sizeof(struct gas_phase));
		}
		gas_phase_copy(gas_phase_ptr, &gas_phase[n], gas_phase_ptr->n_user);
		gas_phase_sort();
	}
	gas_phase_free(gas_phase_ptr);
	free_check_null(gas_phase_ptr);

	// Need to output the next keyword
	output_msg(OUTPUT_CHECKLINE, "\t%s\n", line);
	return (return_value);
}

/* ---------------------------------------------------------------------- */
int
read_reaction_raw(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads REACTION_RAW data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;
	/*
	 *  Accumulate lines in std string
	 */
	std::string keywordLines("");

	keywordLines.append(line);
	keywordLines.append("\n");
/*
 *   Read additonal lines
 */
	int save_echo_input = pr.echo_input;
	pr.echo_input = FALSE;
	for (;;)
	{
		return_value = check_line("reaction_raw", TRUE, TRUE, TRUE, FALSE);
		/* empty, eof, keyword, print */
		if (return_value == EOF || return_value == KEYWORD)
			break;
		keywordLines.append(line);
		keywordLines.append("\n");
	}
	pr.echo_input = save_echo_input;

	std::istringstream iss_in(keywordLines);
	std::ostringstream oss_out;
	std::ostringstream oss_err;

	CParser parser(iss_in, oss_out, oss_err);
	assert(!reading_database());
	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}
	//For testing, need to read line to get started
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);

	cxxReaction ex;
	ex.read_raw(parser);
	struct irrev *irrev_ptr = ex.cxxReaction2irrev();
	int n;

	/*
	 *  This is not quite right, may not produce sort order, forced sort
	 */

	if (irrev_bsearch(irrev_ptr->n_user, &n) != NULL)
	{
		irrev_free(&irrev[n]);
		irrev_copy(irrev_ptr, &irrev[n], irrev_ptr->n_user);
	}
	else
	{
		n = count_irrev++;
		irrev =
			(struct irrev *) PHRQ_realloc(irrev,
										  (size_t) count_irrev *
										  sizeof(struct irrev));
		if (irrev == NULL)
			malloc_error();
		irrev_copy(irrev_ptr, &irrev[n], irrev_ptr->n_user);
		irrev_sort();
	}
	irrev_free(irrev_ptr);
	free_check_null(irrev_ptr);

	// Need to output the next keyword
	output_msg(OUTPUT_CHECKLINE, "\t%s\n", line);
	return (return_value);
}

/* ---------------------------------------------------------------------- */
int
read_mix_raw(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads MIX (_RAW) data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;
	/*
	 *  Accumulate lines in std string
	 */
	std::string keywordLines("");

	keywordLines.append(line);
	keywordLines.append("\n");
/*
 *   Read additonal lines
 */
	int save_echo_input = pr.echo_input;
	pr.echo_input = FALSE;
	for (;;)
	{
		return_value = check_line("mix_raw", TRUE, TRUE, TRUE, FALSE);
		/* empty, eof, keyword, print */
		if (return_value == EOF || return_value == KEYWORD)
			break;
		keywordLines.append(line);
		keywordLines.append("\n");
	}
	pr.echo_input = save_echo_input;

	std::istringstream iss_in(keywordLines);
	std::ostringstream oss_out;
	std::ostringstream oss_err;

	CParser parser(iss_in, oss_out, oss_err);
	assert(!reading_database());
	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}
	//For testing, need to read line to get started
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);

	cxxMix ex;
	ex.read_raw(parser);
	struct mix *mix_ptr = ex.cxxMix2mix();
	int n;

	/*
	 *  This is not quite right, may not produce sort order, forced sort
	 */

	if (mix_bsearch(mix_ptr->n_user, &n) != NULL)
	{
		mix_free(&mix[n]);
		mix_copy(mix_ptr, &mix[n], mix_ptr->n_user);
	}
	else
	{
		n = count_mix++;
		mix =
			(struct mix *) PHRQ_realloc(mix,
										(size_t) count_mix *
										sizeof(struct mix));
		if (mix == NULL)
			malloc_error();
		mix_copy(mix_ptr, &mix[n], mix_ptr->n_user);
		mix_sort();
	}
	mix_free(mix_ptr);
	free_check_null(mix_ptr);

	// Need to output the next keyword
	output_msg(OUTPUT_CHECKLINE, "\t%s\n", line);
	return (return_value);
}

/* ---------------------------------------------------------------------- */
int
read_temperature_raw(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads TEMPERATURE (_RAW) data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;
	/*
	 *  Accumulate lines in std string
	 */
	std::string keywordLines("");

	keywordLines.append(line);
	keywordLines.append("\n");
/*
 *   Read additonal lines
 */
	int save_echo_input = pr.echo_input;
	pr.echo_input = FALSE;
	for (;;)
	{
		return_value = check_line("temperature_raw", TRUE, TRUE, TRUE, FALSE);
		/* empty, eof, keyword, print */
		if (return_value == EOF || return_value == KEYWORD)
			break;
		keywordLines.append(line);
		keywordLines.append("\n");
	}
	pr.echo_input = save_echo_input;

	std::istringstream iss_in(keywordLines);
	std::ostringstream oss_out;
	std::ostringstream oss_err;

	CParser parser(iss_in, oss_out, oss_err);
	assert(!reading_database());

	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}
	//For testing, need to read line to get started
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);

	cxxTemperature ex;
	ex.read_raw(parser);
	struct temperature *temperature_ptr = ex.cxxTemperature2temperature();
	int n;

	/*
	 *  This is not quite right, may not produce sort order, forced sort
	 */

	if (temperature_bsearch(temperature_ptr->n_user, &n) != NULL)
	{
		temperature_free(&temperature[n]);
		temperature_copy(temperature_ptr, &temperature[n],
						 temperature_ptr->n_user);
	}
	else
	{
		n = count_temperature++;
		temperature =
			(struct temperature *) PHRQ_realloc(temperature,
												(size_t) count_temperature *
												sizeof(struct temperature));
		if (temperature == NULL)
			malloc_error();
		temperature_copy(temperature_ptr, &temperature[n],
						 temperature_ptr->n_user);
		temperature_sort();
	}
	temperature_free(temperature_ptr);
	free_check_null(temperature_ptr);

	// Need to output the next keyword
	output_msg(OUTPUT_CHECKLINE, "\t%s\n", line);
	return (return_value);
}
/* ---------------------------------------------------------------------- */
int
read_dump(void)
/* ---------------------------------------------------------------------- */
{
/*
 *      Reads DUMP data block
 *
 *      Arguments:
 *         none
 *
 *      Returns:
 *         KEYWORD if keyword encountered, input_error may be incremented if
 *                    a keyword is encountered in an unexpected position
 *         EOF     if eof encountered while reading mass balance concentrations
 *         ERROR   if error occurred reading data
 *
 */
	int return_value;
	/*
	 *  Make parser
	 */
	std::istringstream iss_in;
	return_value = streamify_to_next_keyword(iss_in);
	std::ostringstream oss_out;
	std::ostringstream oss_err;
	CParser parser(iss_in, oss_out, oss_err);
	assert(!reading_database());

	//For testing, need to read line to get started
	parser.set_echo_file(CParser::EO_NONE);
	std::vector < std::string > vopts;
	std::istream::pos_type next_char;
	parser.get_option(vopts, next_char);

	if (pr.echo_input == FALSE)
	{
		parser.set_echo_file(CParser::EO_NONE);
	}
	else
	{
		parser.set_echo_file(CParser::EO_NOKEYWORDS);
	}

	dumper dmp(parser);
	dump_info = dmp;


	// Need to output the next keyword
	output_msg(OUTPUT_CHECKLINE, "\t%s\n", line);
	return (return_value);
}
#ifdef SKIP
/* ---------------------------------------------------------------------- */
int
streamify_to_next_keyword(std::istringstream & lines)
/* ---------------------------------------------------------------------- */
{
/*
 *   Reads to next keyword or eof
 *
 *   Returns:
 *       KEYWORD
 *       OPTION
 *       EOF
 */
	const char *opt_list[] = {
		"none"
	};
	int count_opt_list = 0;
	int opt;
	char *next_char;

	// Handle echo
	int save_echo_input = pr.echo_input;
	pr.echo_input = FALSE;

	std::string accumulate(line);
	accumulate.append("\n");
	for (;;)
	{
		opt = get_option(opt_list, count_opt_list, &next_char);
		if (opt == OPTION_EOF)
		{						/* end of file */
			break;
		}
		else if (opt == OPTION_KEYWORD)
		{						/* keyword */
			break;
		}
		else
		{
			accumulate.append(line);
			accumulate.append("\n");
		}
	}
	pr.echo_input = save_echo_input;
	lines.str(accumulate);
	return (opt);
}
#endif
/* ---------------------------------------------------------------------- */
int
streamify_to_next_keyword(std::istringstream & lines)
/* ---------------------------------------------------------------------- */
{
/*
 *   Reads to next keyword or eof
 *
 *   Returns:
 *       OPTION_KEYWORD
 *       OPTION_EOF
 *       
 */
	// Handle echo
	int save_echo_input = pr.echo_input;
	pr.echo_input = FALSE;

	std::string accumulate(line);
	accumulate.append("\n");
	int j;
	for (;;)
	{
		j = check_line("Streamify", FALSE, TRUE, TRUE, FALSE);
		if (j == EOF)
		{						/* end of file */
			break;
		}
		else if (j == KEYWORD)
		{						/* keyword */
			break;
		}
		else
		{
			accumulate.append(line);
			accumulate.append("\n");
		}
	}

	lines.str(accumulate);
	//std::cerr << accumulate;
	pr.echo_input = save_echo_input;
	if (j == EOF) return (OPTION_EOF);
	if (j == KEYWORD) return (OPTION_KEYWORD);


	return (OPTION_ERROR);
}
/* ---------------------------------------------------------------------- */
int
dump_entities(void)
/* ---------------------------------------------------------------------- */
{
	int i, n, return_value;
	return_value = OK;
	if (!dump_info.get_dump_solution() &&
		!dump_info.get_dump_pp_assemblage() &&
		!dump_info.get_dump_exchange() &&
		!dump_info.get_dump_surface() &&
		!dump_info.get_dump_s_s_assemblage() &&
		!dump_info.get_dump_gas_phase() &&
		!dump_info.get_dump_kinetics()) 
	{
		return(OK);
	}


	std::ofstream dump_stream;
	if (dump_info.get_append())
	{
		//dump_stream.open(dump_info.get_file_name(), std::ios_base::app);
		dump_stream.open(dump_info.get_file_name().c_str(), std::ios_base::app);
	}
	else
	{
		dump_stream.open(dump_info.get_file_name().c_str());
	}
	// solutions
	if (dump_info.get_dump_solution())
	{
		if (dump_info.get_solution().size() == 0)
		{
			for (i = 0; i < count_solution; i++)
			{
					cxxSolution cxxsoln(solution[i]);
					cxxsoln.dump_raw(dump_stream,0);
			}
		}
		else
		{
			std::set < int >::iterator it;
			for (it = dump_info.get_solution().begin(); it != dump_info.get_solution().end(); it++)
			{
				if (solution_bsearch(*it, &n, FALSE) != NULL)
				{
					cxxSolution cxxsoln(solution[n]);
					cxxsoln.dump_raw(dump_stream,0);
				}
			}
		}
	}

	// pp_assemblages
	if (dump_info.get_dump_pp_assemblage())
	{
		if (dump_info.get_pp_assemblage().size() == 0)
		{
			for (i = 0; i < count_pp_assemblage; i++)
			{
					cxxPPassemblage cxxentity(&pp_assemblage[i]);
					cxxentity.dump_raw(dump_stream,0);
			}
		}
		else
		{
			std::set < int >::iterator it;
			for (it = dump_info.get_pp_assemblage().begin(); it != dump_info.get_pp_assemblage().end(); it++)
			{

				if (pp_assemblage_bsearch(*it, &n) != NULL)
				{
					cxxPPassemblage cxxentity(&pp_assemblage[n]);
					cxxentity.dump_raw(dump_stream,0);
				}
			}
		}
	}

	// exchanges
	if (dump_info.get_dump_exchange())
	{
		if (dump_info.get_exchange().size() == 0)
		{
			for (i = 0; i < count_exchange; i++)
			{
					cxxExchange cxxentity(&exchange[i]);
					cxxentity.dump_raw(dump_stream,0);
			}
		}
		else
		{
			std::set < int >::iterator it;
			for (it = dump_info.get_exchange().begin(); it != dump_info.get_exchange().end(); it++)
			{

				if (exchange_bsearch(*it, &n) != NULL)
				{
					cxxExchange cxxentity(&exchange[n]);
					cxxentity.dump_raw(dump_stream,0);
				}
			}
		}
	}

	// surfaces
	if (dump_info.get_dump_surface())
	{
		if (dump_info.get_surface().size() == 0)
		{
			for (i = 0; i < count_surface; i++)
			{
					cxxSurface cxxentity(&surface[i]);
					cxxentity.dump_raw(dump_stream,0);
			}
		}
		else
		{
			std::set < int >::iterator it;
			for (it = dump_info.get_surface().begin(); it != dump_info.get_surface().end(); it++)
			{

				if (surface_bsearch(*it, &n) != NULL)
				{
					cxxSurface cxxentity(&surface[n]);
					cxxentity.dump_raw(dump_stream,0);
				}
			}
		}
	}

	// s_s_assemblages
	if (dump_info.get_dump_s_s_assemblage())
	{
		if (dump_info.get_s_s_assemblage().size() == 0)
		{
			for (i = 0; i < count_s_s_assemblage; i++)
			{
					cxxSSassemblage cxxentity(&s_s_assemblage[i]);
					cxxentity.dump_raw(dump_stream,0);
			}
		}
		else
		{
			std::set < int >::iterator it;
			for (it = dump_info.get_s_s_assemblage().begin(); it != dump_info.get_s_s_assemblage().end(); it++)
			{

				if (s_s_assemblage_bsearch(*it, &n) != NULL)
				{
					cxxSSassemblage cxxentity(&s_s_assemblage[n]);
					cxxentity.dump_raw(dump_stream,0);
				}
			}
		}
	}

	// gas_phases
	if (dump_info.get_dump_gas_phase())
	{
		if (dump_info.get_gas_phase().size() == 0)
		{
			for (i = 0; i < count_gas_phase; i++)
			{
					cxxGasPhase cxxentity(&gas_phase[i]);
					cxxentity.dump_raw(dump_stream,0);
			}
		}
		else
		{
			std::set < int >::iterator it;
			for (it = dump_info.get_gas_phase().begin(); it != dump_info.get_gas_phase().end(); it++)
			{

				if (gas_phase_bsearch(*it, &n) != NULL)
				{
					cxxGasPhase cxxentity(&gas_phase[n]);
					cxxentity.dump_raw(dump_stream,0);
				}
			}
		}
	}

	// kineticss
	if (dump_info.get_dump_kinetics())
	{
		if (dump_info.get_kinetics().size() == 0)
		{
			for (i = 0; i < count_kinetics; i++)
			{
					cxxKinetics cxxentity(&kinetics[i]);
					cxxentity.dump_raw(dump_stream,0);
			}
		}
		else
		{
			std::set < int >::iterator it;
			for (it = dump_info.get_kinetics().begin(); it != dump_info.get_kinetics().end(); it++)
			{

				if (kinetics_bsearch(*it, &n) != NULL)
				{
					cxxKinetics cxxentity(&kinetics[n]);
					cxxentity.dump_raw(dump_stream,0);
				}
			}
		}
	}

	dump_info.DumpAll(false);


	return (OK);
}
