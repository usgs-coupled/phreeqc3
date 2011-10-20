#include "Phreeqc.h"

//#include "input.h"
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

	Phreeqc phreeqc_instance;
	phreeqc_instance.main_method(argc, argv);
}


int Phreeqc::
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

	phast = FALSE;
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
	this->set_cookie((std::ifstream *) db_cookie);
	errors = read_database(PHRQ_io::istream_getc, db_cookie);
	this->clear_cookie();

	if (errors != 0)
	{
		//clean_up();
		return errors;
	}

/*
 *   Read input data for simulation
 */

	this->set_cookie((std::ifstream *)input_cookie);
	errors = run_simulations(PHRQ_io::istream_getc, input_cookie);
	this->clear_cookie();

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
int Phreeqc::
write_banner(void)
/* ---------------------------------------------------------------------- */
{
	char buffer[80];
	int len, indent;
	screen_msg(
			   "              €ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ€\n");
	screen_msg(
			   "              ∫                                            ∫\n");

	/* version */
	len = sprintf(buffer, "* PHREEQC-%s *", "@VERSION@");
	indent = (44 - len) / 2;
	screen_msg(sformatf("%14c∫%*c%s%*c∫\n", ' ', indent, ' ', buffer,
			   44 - indent - len, ' '));

	screen_msg(
			   "              ∫                                            ∫\n");
	screen_msg(
			   "              ∫      A hydrogeochemical transport model    ∫\n");
	screen_msg(
			   "              ∫                                            ∫\n");
	screen_msg(
			   "              ∫                    by                      ∫\n");
	screen_msg(
			   "              ∫       D.L. Parkhurst and C.A.J. Appelo     ∫\n");
	screen_msg(
			   "              ∫                                            ∫\n");


	/* date */
	len = sprintf(buffer, "%s", "@VER_DATE@");
	indent = (44 - len) / 2;
	screen_msg(sformatf("%14c∫%*c%s%*c∫\n", ' ', indent, ' ', buffer,
			   44 - indent - len, ' '));

	screen_msg(
			   "              €‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹€\n\n");

	return 0;
}
#endif
