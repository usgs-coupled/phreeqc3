#define EXTERNAL
#include "global.h"
#include "output.h"
#include "phrqproto.h"
#include "input.h"

/*#define PHREEQC_XML*/
#ifdef PHREEQC_XML
#include "SAXPhreeqc.h"
extern void SAX_cleanup(void);
#endif

static char const svnid[] = "$Id: main.c 715 2006-01-18 01:26:29Z dlpark $";

#ifdef DOS
static int write_banner(void);
#endif

/* ----------------------------------------------------------------------
 *   MAIN
 * ---------------------------------------------------------------------- */
int main(int argc, char *argv[])
/*
 *   Main program for PHREEQC
 */
{
	int errors;
	void *db_cookie = NULL;
	void *input_cookie = NULL;
	if (svnid == NULL) fprintf(stderr," ");
	phast = FALSE;
/*
 *   Add callbacks for error_msg and warning_msg
 */
	if (add_output_callback(phreeqc_handler, NULL) != OK) {
		fprintf(stderr, "ERROR: %s\n", "NULL pointer returned from malloc or realloc.");
		fprintf(stderr, "ERROR: %s\n", "Program terminating.");
		return -1;
	}

/*
 *   Open input/output files
 */
	errors = process_file_names(argc, argv, &db_cookie, &input_cookie, TRUE);
	if (errors != 0) {
		clean_up();
		return errors;
	}
#ifdef DOS
	write_banner(); 
#endif

/*
 *   Initialize arrays
 */
	errors = do_initialize();
	if (errors != 0) {
		clean_up();
		return errors;
	}

/*
 *   Load database into memory
 */
	errors = read_database(getc_callback, db_cookie);
	if (errors != 0) {
		clean_up();
		return errors;
	}

/*
 *   Read input data for simulation
 */
	errors = run_simulations(getc_callback, input_cookie);
	if (errors != 0) {
		clean_up();
		return errors;
	}


/*
 *   Display successful status
 */
	errors = do_status();
	if (errors != 0) {
		clean_up();
		return errors;
	}
#ifdef PHREEQC_XML
{
	 int n;
		SAX_StartSystem();
		for (n = 0; n < count_solution; ++n)
		{
		  SAX_AddSolution(solution[n]);
		}
		SAX_EndSystem();
		SAX_UnpackSolutions(SAX_GetXMLStr(), SAX_GetXMLLength());
 }
#endif

	clean_up();
	close_input_files();
	close_output_files();
#ifdef PHREEQC_XML
	SAX_cleanup();
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
int write_banner(void)
/* ---------------------------------------------------------------------- */
{
	char buffer[80];
	int len, indent;
	output_msg(OUTPUT_SCREEN, "              ÛßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßÛ\n");
	output_msg(OUTPUT_SCREEN, "              º                                            º\n");

	/* version */
	len = sprintf(buffer, "* PHREEQC-%s *", "@VERSION@");
	indent = (44 - len) / 2;
	output_msg(OUTPUT_SCREEN, "%14cº%*c%s%*cº\n", ' ', indent, ' ', buffer, 44 - indent - len, ' ');

	output_msg(OUTPUT_SCREEN, "              º                                            º\n");
	output_msg(OUTPUT_SCREEN, "              º      A hydrogeochemical transport model    º\n");
	output_msg(OUTPUT_SCREEN, "              º                                            º\n");
	output_msg(OUTPUT_SCREEN, "              º                    by                      º\n");
	output_msg(OUTPUT_SCREEN, "              º       D.L. Parkhurst and C.A.J. Appelo     º\n");
	output_msg(OUTPUT_SCREEN, "              º                                            º\n");


	/* date */
	len = sprintf(buffer, "%s", "@VER_DATE@");
	indent = (44 - len) / 2;
	output_msg(OUTPUT_SCREEN, "%14cº%*c%s%*cº\n", ' ', indent, ' ', buffer, 44 - indent - len, ' ');

	output_msg(OUTPUT_SCREEN, "              ÛÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÛ\n\n");

return 0;
}
