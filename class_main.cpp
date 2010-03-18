#include "Phreeqc.h"
#include "output.h"
#include "phrqproto.h"
#include "input.h"
#include "NameDouble.h"
#include "Solution.h"
#include "Reaction.h"
#include "PPassemblage.h"
#include "Exchange.h"
#include "Surface.h"
#include "GasPhase.h"
#include "SSassemblage.h"
#include "cxxKinetics.h"


/* ----------------------------------------------------------------------
 *   MAIN
 * ---------------------------------------------------------------------- */
int 
main(int argc, char *argv[])
/*
 *   Main program for PHREEQC
 */
{

	//int errors;
#if defined(WIN32_MEMORY_DEBUG)
	int tmpDbgFlag;

	/*
	 * Set the debug-heap flag to keep freed blocks in the
	 * heap's linked list - This will allow us to catch any
	 * inadvertent use of freed memory
	 */
	tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	//tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	///tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;
	_CrtSetDbgFlag(tmpDbgFlag);
	//_crtBreakAlloc = 9482;
#endif

	/*if (svnid == NULL)
		fprintf(stderr, " ");*/


	Phreeqc phreeqc_instance;
	phreeqc_instance.main_method(argc, argv);
	std::list<std::string> components;
	phreeqc_instance.list_components(components);
	std::list<std::string>::iterator it;
	std::cout << "Number of components: " << components.size() << std::endl;
	for (it = components.begin(); it != components.end(); it++)
	{
		std::cout << "   " << *it << std::endl;
	}
}


int CLASS_QUALIFIER
main_method(int argc, char *argv[])
/*
 *   Main program for PHREEQC
 */
{

	int errors;
	void *db_cookie = NULL;
	void *input_cookie = NULL;
#if defined(WIN32_MEMORY_DEBUG)
	int tmpDbgFlag;

	/*
	 * Set the debug-heap flag to keep freed blocks in the
	 * heap's linked list - This will allow us to catch any
	 * inadvertent use of freed memory
	 */
	tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	//tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	///tmpDbgFlag |= _CRTDBG_CHECK_ALWAYS_DF;
	_CrtSetDbgFlag(tmpDbgFlag);
	//_crtBreakAlloc = 9482;
#endif

	//if (svnid == NULL)
	//	fprintf(stderr, " ");
	phast = FALSE;
/*
 *   Add callbacks for error_msg and warning_msg
 */
	if (add_output_callback(phreeqc_handler, this) != OK)
	{
		fprintf(stderr, "ERROR: %s\n",
				"NULL pointer returned from malloc or realloc.");
		fprintf(stderr, "ERROR: %s\n", "Program terminating.");
		return -1;
	}

/*
 *   Open input/output files
 */
	errors = process_file_names(argc, argv, &db_cookie, &input_cookie, TRUE);
	if (errors != 0)
	{
		//clean_up();
		return errors;
	}
#ifdef DOS
	write_banner();
#endif

/*
 *   Initialize arrays
 */
	errors = do_initialize();
	if (errors != 0)
	{
		//clean_up();
		return errors;
	}
/*
 *   Load database into memory
 */
	errors = read_database(getc_callback, db_cookie);
	if (errors != 0)
	{
		//clean_up();
		return errors;
	}

/*
 *   Read input data for simulation
 */
	errors = run_simulations(getc_callback, input_cookie);
	if (errors != 0)
	{
		//clean_up();
		return errors;
	}


/*
 *   Display successful status
 */
	errors = do_status();
	if (errors != 0)
	{
		//clean_up();
		return errors;
	}


	//clean_up();
	//close_input_files();
	//close_output_files();

	return 0;
}


#ifdef DOS
/* ---------------------------------------------------------------------- */
int CLASS_QUALIFIER
write_banner(void)
/* ---------------------------------------------------------------------- */
{
	char buffer[80];
	int len, indent;
	output_msg(OUTPUT_SCREEN,
			   "              ÛßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßÛ\n");
	output_msg(OUTPUT_SCREEN,
			   "              º                                            º\n");

	/* version */
	len = sprintf(buffer, "* PHREEQC-%s *", "@VERSION@");
	indent = (44 - len) / 2;
	output_msg(OUTPUT_SCREEN, "%14cº%*c%s%*cº\n", ' ', indent, ' ', buffer,
			   44 - indent - len, ' ');

	output_msg(OUTPUT_SCREEN,
			   "              º                                            º\n");
	output_msg(OUTPUT_SCREEN,
			   "              º      A hydrogeochemical transport model    º\n");
	output_msg(OUTPUT_SCREEN,
			   "              º                                            º\n");
	output_msg(OUTPUT_SCREEN,
			   "              º                    by                      º\n");
	output_msg(OUTPUT_SCREEN,
			   "              º       D.L. Parkhurst and C.A.J. Appelo     º\n");
	output_msg(OUTPUT_SCREEN,
			   "              º                                            º\n");


	/* date */
	len = sprintf(buffer, "%s", "@VER_DATE@");
	indent = (44 - len) / 2;
	output_msg(OUTPUT_SCREEN, "%14cº%*c%s%*cº\n", ' ', indent, ' ', buffer,
			   44 - indent - len, ' ');

	output_msg(OUTPUT_SCREEN,
			   "              ÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛ\n\n");

	return 0;
}
#endif
size_t CLASS_QUALIFIER
list_components(std::list<std::string> &list_c)
/*
 *   Find all elements in any class definition
 */
{
	cxxNameDouble accumulator;
	accumulator.add("H", 1);
	accumulator.add("O", 1);

	int i;

	// solutions
	for (i = 0; i < count_solution; i++)
	{
		cxxSolution entity(solution[i]);
		accumulator.add_extensive(entity.get_totals(), 1.0);
	}

	// irreversible reactions
	for (i = 0; i < count_irrev; i++)
	{
		reaction_calc(&irrev[i]);
		cxxReaction entity(&irrev[i]);
		accumulator.add_extensive(entity.get_elementList(), 1.0);
	}

	// pure phases
	for (i = 0; i < count_pp_assemblage; i++)
	{
		cxxPPassemblage entity(&pp_assemblage[i]);
		entity.totalize(this);
		accumulator.add_extensive(entity.get_totals(), 1.0);
	}

	// exchangers
	for (i = 0; i < count_exchange; i++)
	{
		cxxExchange entity(&exchange[i]);
		entity.totalize();
		accumulator.add_extensive(entity.get_totals(), 1.0);
	}

	// surfaces
	for (i = 0; i < count_surface; i++)
	{
		cxxSurface entity(&surface[i]);
		entity.totalize();
		accumulator.add_extensive(entity.get_totals(), 1.0);
	}

	// gas phases
	for (i = 0; i < count_gas_phase; i++)
	{
		cxxGasPhase entity(&gas_phase[i]);
		entity.totalize(this);
		accumulator.add_extensive(entity.get_totals(), 1.0);
	}

	// solid-solutions
	for (i = 0; i < count_s_s_assemblage; i++)
	{
		cxxSSassemblage entity(&s_s_assemblage[i]);
		entity.totalize(this);
		accumulator.add_extensive(entity.get_totals(), 1.0);
	}

	// kinetics
	for (i = 0; i < count_kinetics; i++)
	{
		calc_dummy_kinetic_reaction_tally(&kinetics[i]);
		cxxKinetics entity(&kinetics[i]);
		accumulator.add_extensive(entity.get_totals(), 1.0);
	}

	// print list
	cxxNameDouble::iterator it;
	for (it = accumulator.begin(); it != accumulator.end(); it++)
	{
		struct master *master_ptr = master_bsearch(it->first.c_str());
		if (master_ptr == NULL) continue;
		if (master_ptr->type != AQ) continue;
		if (master_ptr->primary == 0) continue;
		list_c.push_back(it->first);
	}
	return(accumulator.size());
}