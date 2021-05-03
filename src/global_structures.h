#ifndef _INC_GLOBAL_STRUCTURES_H
#define _INC_GLOBAL_STRUCTURES_H
#include "Surface.h"
#include "GasPhase.h"
#include "Species.h"
#include "Element.h"
#include "Phase.h"
#include "Rate.h"
#include "Master.h"
/* ----------------------------------------------------------------------
 *   #define DEFINITIONS
 * ---------------------------------------------------------------------- */
#ifndef NAN
#   define NAN -99999999
#endif
#define MISSING -9999.999            
#include "NA.h"   /* NA = not available */

#define F_C_MOL 96493.5			/* C/mol or joule/volt-eq */
#define F_KJ_V_EQ  96.4935		/* kJ/volt-eq */
#define F_KCAL_V_EQ 23.0623		/* Logk::kcal/volt-eq */
#define R_LITER_ATM 0.0820597	/* L-atm/deg-mol */
#define R_KCAL_DEG_MOL 0.00198726	/* Logk::kcal/deg-mol */
#define R_KJ_DEG_MOL 0.00831470	/* kJ/deg-mol */
#define EPSILON 78.5			/* dialectric constant, dimensionless. Is calculated as eps_r(P, T) in calc_dielectrics. Update the code?? */
#define EPSILON_ZERO 8.854e-12	/* permittivity of free space, C/V-m = C**2/m-J */
#define JOULES_PER_CALORIE 4.1840
#define PASCAL_PER_ATM 1.01325E5 /* conversion from atm to Pa */
#define AVOGADRO 6.02252e23		/* atoms / mole */
#define pi 3.14159265358979
#define AH2O_FACTOR 0.017

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define STOP 1
#define CONTINUE 0

#define OPTION_EOF -1
#define OPTION_KEYWORD -2
#define OPTION_ERROR -3
#define OPTION_DEFAULT -4
#define OPT_1 -5

#define DISP 2
#define STAG 3
#define NOMIX 4

#define CONVERGED 2
#define MASS_BALANCE 3

#define REWRITE 2
#define INIT -1

/* check_line values, plus EMPTY, EOF, OK */
#define KEYWORD 3

/* copy_token values */
#define EMPTY 2
#define UPPER 4
#define LOWER 5
#define DIGIT 6
#define UNKNOWN 7
#define OPTION 8

/* species types */
#define AQ 0
#define HPLUS 1
#define H2O 2
#define EMINUS 3
#define SOLID 4
#define EX 5
#define SURF 6
#define SURF_PSI 7
#define SURF_PSI1 8
#define SURF_PSI2 9

/* unknown types */
#define MB 10
#define ALK 11
#define CB 12
#define SOLUTION_PHASE_BOUNDARY 13
#define MU 14
#define AH2O 15
#define MH 16
#define MH2O 17
#define PP 18
#define EXCH 19
#define SURFACE 20
#define SURFACE_CB 21
#define SURFACE_CB1 22
#define SURFACE_CB2 23
#define GAS_MOLES 24
#define SS_MOLES 25
#define PITZER_GAMMA 26
#define SLACK 28
/* state */
#define INITIALIZE	       0
#define INITIAL_SOLUTION   1
#define INITIAL_EXCHANGE   2
#define INITIAL_SURFACE 3
#define INITIAL_GAS_PHASE  4
#define REACTION		   5
#define INVERSE		 6
#define ADVECTION		 7
#define TRANSPORT		 8
#define PHAST		     9

/* constaints in mass balance */
#define EITHER 0
#define DISSOLVE 1
#define PRECIPITATE -1

/* gas phase type */
#define PRESSURE 1
#define VOLUME 2

#define MAX_PP_ASSEMBLAGE 10	/* default estimate of the number of phase assemblages */
#define MAX_ADD_EQUATIONS 20	/* maximum number of equations added together to reduce eqn to
								   master species */
#define MAX_ELEMENTS 50			/* default estimate of the number of elements */
#define MAX_LENGTH 256			/* maximum number of characters component name */
#define MAX_LINE 4096			/* estimate of maximum line length */
#define MAX_MASS_BALANCE 10		/* initial guess of number mass balance equations for a solution */
#define MAX_MASTER 50			/* default estimate of the number of master species */
#define MAX_ELTS 15				/* default estimate for maximum number of times elements occur in
								   an equation */
#define MAX_PHASES 500			/* initial guess of number of phases defined */
#define MAX_S 500				/* default estimate for maximum number of species in aqueous model */
#define MAX_SUM_JACOB0 50		/* list used to calculate jacobian */
#define MAX_SUM_JACOB1 500		/* list used to calculate jacobian */
#define MAX_SUM_JACOB2 500		/* list used to calculate jacobian */
#define MAX_SUM_MB 500			/* list used to calculate mass balance sums */
#define MAX_TRXN 16				/* default estimate for maximum number of components in an eqn */
#define MAX_UNKNOWNS 15			/* default estimate for maximum number of unknowns in model */
#define TOL 1e-9				/* tolerance for comparisons of double numbers */
#define MAX_LM 3.0				/* maximum log molality allowed in intermediate iterations */
#define MAX_M 1000.0
#ifdef USE_DECIMAL128
// #define MIN_LM -80.0			/* minimum log molality allowed before molality set to zero */
// #define LOG_ZERO_MOLALITY -80	/* molalities <= LOG_ZERO_MOLALITY are considered equal to zero */
// #define MIN_TOTAL 1e-60
// #define MIN_TOTAL_SS MIN_TOTAL/100
// #define MIN_RELATED_SURFACE MIN_TOTAL*100
// #define MIN_RELATED_LOG_ACTIVITY -60
#else
// #define MIN_LM -30.0			/* minimum log molality allowed before molality set to zero */
// #define LOG_ZERO_MOLALITY -30	/* molalities <= LOG_ZERO_MOLALITY are considered equal to zero */
// #define MIN_TOTAL 1e-25
// #define MIN_TOTAL_SS MIN_TOTAL/100
// #define MIN_RELATED_SURFACE MIN_TOTAL*100
// #define MIN_RELATED_LOG_ACTIVITY -30
#endif
#define REF_PRES_PASCAL 1.01325E5   /* Reference pressure: 1 atm */
#define MAX_P_NONLLNL 1500.0

enum entity_type
{ Solution, Reaction, Exchange, Surface, Gas_phase, Pure_phase, Ss_phase,
	Kinetics, Mix, Temperature, Pressure, UnKnown
};

typedef struct PHRQMemHeader
{
	struct PHRQMemHeader *pNext;	/* memory allocated just after this one */
	struct PHRQMemHeader *pPrev;	/* memory allocated just prior to this one */
	size_t size;				/* memory request + sizeof(PHRQMemHeader) */
#if !defined(NDEBUG)
	char *szFileName;			/* file name */
	int nLine;					/* line number */
	int dummy;					/* alignment */
#endif
} PHRQMemHeader;

struct Change_Surf
{
	const char *comp_name;
	LDBLE fraction;
	const char *new_comp_name;
	LDBLE new_Dw;
	int cell_no;
	int next;
};

class save
{
public:
	~save() {};
	save()
	{
		solution = 0;
		n_solution_user = 0;
		n_solution_user_end = 0;
		mix = 0;
		n_mix_user = 0;
		n_mix_user_end = 0;
		reaction = 0;
		n_reaction_user = 0;
		n_reaction_user_end = 0;
		pp_assemblage = 0;
		n_pp_assemblage_user = 0;
		n_pp_assemblage_user_end = 0;
		exchange = 0;
		n_exchange_user = 0;
		n_exchange_user_end = 0;
		kinetics = 0;
		n_kinetics_user = 0;
		n_kinetics_user_end = 0;
		surface = 0;
		n_surface_user = 0;
		n_surface_user_end = 0;
		gas_phase = 0;
		n_gas_phase_user = 0;
		n_gas_phase_user_end = 0;
		ss_assemblage = 0;
		n_ss_assemblage_user = 0;
		n_ss_assemblage_user_end = 0;
	}
	int solution;
	int n_solution_user;
	int n_solution_user_end;
	int mix;
	int n_mix_user;
	int n_mix_user_end;
	int reaction;
	int n_reaction_user;
	int n_reaction_user_end;
	int pp_assemblage;
	int n_pp_assemblage_user;
	int n_pp_assemblage_user_end;
	int exchange;
	int n_exchange_user;
	int n_exchange_user_end;
	int kinetics;
	int n_kinetics_user;
	int n_kinetics_user_end;
	int surface;
	int n_surface_user;
	int n_surface_user_end;
	int gas_phase;
	int n_gas_phase_user;
	int n_gas_phase_user_end;
	int ss_assemblage;
	int n_ss_assemblage_user;
	int n_ss_assemblage_user_end;
};

/*----------------------------------------------------------------------
 *   Copy
 *---------------------------------------------------------------------- */
class copier
{
public:
	~copier() {};
	copier()
	{}
	std::vector<int> n_user;
	std::vector<int> start;
	std::vector<int> end;
};
/*----------------------------------------------------------------------
 *   Jacobian and Mass balance lists
 *---------------------------------------------------------------------- */
class Model
{
public:
	Model()
	{
		force_prep = true;
		gas_phase_type = cxxGasPhase::GP_UNKNOWN;
		numerical_fixed_volume = false;
		dl_type = cxxSurface::NO_DL;
		surface_type = cxxSurface::UNKNOWN_DL;
	};
	~Model()
	{
	};
	bool force_prep;
	bool numerical_fixed_volume;
	cxxGasPhase::GP_TYPE gas_phase_type;
	std::vector<std::string> gas_phase;
	std::vector<std::string> ss_assemblage;
	std::vector<class phase*> pp_assemblage;
	std::vector<double> si;
	std::vector<std::string> add_formula;
	cxxSurface::DIFFUSE_LAYER_TYPE dl_type;
	cxxSurface::SURFACE_TYPE surface_type;
	std::vector<std::string> surface_comp;
	std::vector<std::string> surface_charge;
};
/*----------------------------------------------------------------------
 *   Species_list
 *---------------------------------------------------------------------- */
class species_list
{
public:
	~species_list() {};
	species_list()
	{
		master_s = NULL;
		s = NULL;
		coef = 0;
	}
	class species* master_s;
	class species* s;
	LDBLE coef;
};
/*----------------------------------------------------------------------
 *   Jacobian and Mass balance lists
 *---------------------------------------------------------------------- */
class list0
{
public:
	~list0() {};
	list0()
	{
		target = NULL;
		coef = 0;
	}
	LDBLE* target;
	LDBLE coef;
};
class list1
{
public:
	~list1() {};
	list1()
	{
		source = NULL;
		target = NULL;
	}
	LDBLE* source;
	LDBLE* target;
};
class list2
{
public:
	~list2() {};
	list2()
	{
		source = NULL;
		target = NULL;
		coef = 0;
	}
	LDBLE* source;
	LDBLE* target;
	LDBLE coef;
};
class isotope
{
public:
	~isotope() {};
	isotope()
	{
		isotope_number = 0;
		//elt_name.clear();
		//isotope_name.clear();
		total = 0;
		ratio = 0;
		ratio_uncertainty = 0;
		x_ratio_uncertainty = 0;
		master = NULL;
		primary = NULL;
		coef = 0;					/* coefficient of element in phase */
	}
	LDBLE isotope_number;
	std::string elt_name;
	std::string isotope_name;
	LDBLE total;
	LDBLE ratio;
	LDBLE ratio_uncertainty;
	LDBLE x_ratio_uncertainty;
	class master* master;
	class master* primary;
	LDBLE coef;
};
class const_iso
{
public:
	~const_iso() {};
	const_iso()
	{
		//name.clear();
		value = 0;
		uncertainty = 0;
	}
	const_iso(const std::string n, LDBLE v, LDBLE u)
	{
		name = n;
		value = v;
		uncertainty = u;
	}
	std::string name;
	LDBLE value;
	LDBLE uncertainty;
};
class iso
{
public:
	~iso() {};
	iso()
	{
		//name.clear();
		value = 0;
		uncertainty = 0.05;
	}
	iso(const class const_iso& ci)
	{
		name = ci.name;
		value = ci.value;
		uncertainty = ci.uncertainty;
	}
	std::string name;
	LDBLE value;
	LDBLE uncertainty;
};
/*----------------------------------------------------------------------
 *   Transport data
 *---------------------------------------------------------------------- */
class stag_data
{
public:
	~stag_data() {};
	stag_data()
	{
		count_stag = 0;
		exch_f = 0;
		th_m = 0;
		th_im = 0;
	}
	int count_stag;
	LDBLE exch_f;
	LDBLE th_m;
	LDBLE th_im;
};
class cell_data
{
public:
	~cell_data() {};
	cell_data()
	{
		length = 1;
		mid_cell_x = 1.;
		disp = 1.0;
		temp = 25.;
		// free (uncharged) porewater porosities
		por = 0.1;
		// interlayer water porosities
		por_il = 0.01;
		// potential (V)
		potV = 0;
		punch = FALSE;
		print = FALSE;
		same_model = FALSE;
	}
	LDBLE length;
	LDBLE mid_cell_x;
	LDBLE disp;
	LDBLE temp;
	LDBLE por;
	LDBLE por_il;
	LDBLE potV;
	int punch;
	int print;
	int same_model;
};

/*----------------------------------------------------------------------
 *   Unknowns
 *---------------------------------------------------------------------- */
class unknown
{
public:
	~unknown() {};
	unknown()
	{
		type = 0;
		moles = 0;
		ln_moles = 0;
		f = 0;
		sum = 0;
		delta = 0;
		la = 0;
		number = 0;
		//description.clear();
		//master.clear();
		phase = NULL;
		si = 0;
		n_gas_phase_user = 0;
		s = NULL;
		//exch_comp.clear();
		//pp_assemblage_comp_name.clear();
		pp_assemblage_comp_ptr = NULL;
		//ss_name.clear();
		ss_ptr = NULL;
		//ss_comp_name.clear();
		ss_comp_ptr = NULL;
		ss_comp_number = 0;
		ss_in = FALSE;
		//surface_comp.clear();
		//surface_charge.clear();
		related_moles = 0;
		potential_unknown = NULL;
		potential_unknown1 = NULL;
		potential_unknown2 = NULL;
		// list for CD_MUSIC of comps that contribute to 0 plane mass-balance term
		//comp_unknowns.clear();
		phase_unknown = NULL;
		mass_water = 1;
		dissolve_only = FALSE;
		inert_moles = 0;
		V_m = 0;
		pressure = 1;
		mb_number = 0;
		iteration = 0;
	}
	int type;
	LDBLE moles;
	LDBLE ln_moles;
	LDBLE f;
	LDBLE sum;
	LDBLE delta;
	LDBLE la;
	size_t number;
	std::string description;
	std::vector<class master*> master;
	class phase* phase;
	LDBLE si;
	int n_gas_phase_user;
	class species* s;
	std::string exch_comp;
	std::string pp_assemblage_comp_name;
	void* pp_assemblage_comp_ptr;
	std::string ss_name;
	void* ss_ptr;
	std::string ss_comp_name;
	void* ss_comp_ptr;
	int ss_comp_number;
	int ss_in;
	std::string surface_comp;
	std::string surface_charge;
	LDBLE related_moles;
	class unknown* potential_unknown;
	class unknown* potential_unknown1;
	class unknown* potential_unknown2;
	std::vector<class unknown*> comp_unknowns;
	class unknown* phase_unknown;
	LDBLE mass_water;
	int dissolve_only;
	LDBLE inert_moles;
	LDBLE V_m;
	LDBLE pressure;
	int mb_number;
	int iteration;
};
/*----------------------------------------------------------------------
 *   Reaction work space
 *---------------------------------------------------------------------- */

class unknown_list
{
public:
	~unknown_list() {};
	unknown_list()
	{
		unknown = NULL;
		source = NULL;
		gamma_source = NULL;
		coef = 0;
	}
	class unknown* unknown;
	LDBLE* source;
	LDBLE* gamma_source;
	LDBLE coef;
};
/* ----------------------------------------------------------------------
 *   Print
 * ---------------------------------------------------------------------- */
class prints
{
public:
	~prints() {};
	prints()
	{
		all = 0;
		initial_solutions = 0;
		initial_exchangers = 0;
		reactions = 0;
		gas_phase = 0;
		ss_assemblage = 0;
		pp_assemblage = 0;
		surface = 0;
		exchange = 0;
		kinetics = 0;
		totals = 0;
		eh = 0;
		species = 0;
		saturation_indices = 0;
		irrev = 0;
		mix = 0;
		reaction = 0;
		use = 0;
		logfile = 0;
		punch = 0;
		status = 0;
		inverse = 0;
		dump = 0;
		user_print = 0;
		headings = 0;
		user_graph = 0;
		echo_input = 0;
		warnings = 0;
		initial_isotopes = 0;
		isotope_ratios = 0;
		isotope_alphas = 0;
		hdf = 0;
		alkalinity = 0;
	}
	int all;
	int initial_solutions;
	int initial_exchangers;
	int reactions;
	int gas_phase;
	int ss_assemblage;
	int pp_assemblage;
	int surface;
	int exchange;
	int kinetics;
	int totals;
	int eh;
	int species;
	int saturation_indices;
	int irrev;
	int mix;
	int reaction;
	int use;
	int logfile;
	int punch;
	int status;
	int inverse;
	int dump;
	int user_print;
	int headings;
	int user_graph;
	int echo_input;
	int warnings;
	int initial_isotopes;
	int isotope_ratios;
	int isotope_alphas;
	int hdf;
	int alkalinity;
};
/* ----------------------------------------------------------------------
 *   GLOBAL DECLARATIONS
 * ---------------------------------------------------------------------- */
class spread_row
{
public:
	~spread_row() {};
	spread_row()
	{
		count = 0;
		empty = 0, string = 0, number = 0;
		//char_vector.clear();
		//d_vector.clear();
		//type_vector.clear();
	}
	size_t count;
	size_t empty, string, number;
	std::vector<std::string> str_vector;
	std::vector<int> type_vector;
};
class defaults
{
public:
	~defaults() {};
	defaults()
	{
		temp = 25;
		density = 1;
		calc_density = false;
		units = NULL;
		redox = NULL;
		ph = 7;
		pe = 4;
		water = 1;
		//iso.clear();
		pressure = 1;	/* pressure in atm */
	}
	LDBLE temp;
	LDBLE density;
	bool calc_density;
	const char* units;
	const char* redox;
	LDBLE ph;
	LDBLE pe;
	LDBLE water;
	std::vector<class iso> iso;
	LDBLE pressure;
};
class spread_sheet
{
public:
	~spread_sheet() {};
	spread_sheet()
	{
		heading = NULL;
		units = NULL;
		//class defaults defaults;
	}
	class spread_row* heading;
	class spread_row* units;
	std::vector<class spread_row*> rows;
	class defaults defaults;
};
/* ----------------------------------------------------------------------
 *   ISOTOPES
 * ---------------------------------------------------------------------- */
class master_isotope
{
public:
	~master_isotope() {};
	master_isotope()
	{
		//name.clear();
		master = NULL;
		elt = NULL;
		//units.clear();
		standard = 0;
		ratio = 0;
		moles = 0;
		total_is_major = 0;
		minor_isotope = 0;
	}
	std::string name;
	class master* master;
	class element* elt;
	std::string units;
	LDBLE standard;
	LDBLE ratio;
	LDBLE moles;
	int total_is_major;
	int minor_isotope;
};
class calculate_value
{
public:
	~calculate_value() {};
	calculate_value()
	{
		//name.clear();
		value = 0;
		//commands.clear();
		new_def = 0;
		calculated = 0;
		linebase = NULL;
		varbase = NULL;
		loopbase = NULL;
	}
	std::string name;
	LDBLE value;
	std::string commands;
	int new_def;
	int calculated;
	void* linebase;
	void* varbase;
	void* loopbase;
};
class isotope_ratio
{
public:
	isotope_ratio()
	{
		//name.clear();
		//isotope_name.clear();
		ratio = 0;
		converted_ratio = 0;
	}
	~isotope_ratio() {};

	std::string name;
	std::string isotope_name;
	LDBLE ratio;
	LDBLE converted_ratio;
};
class isotope_alpha
{
public:
	isotope_alpha()
	{
		//name.clear();
		//named_logk.clear();
		value = 0;
	}
	~isotope_alpha() {};
	std::string name;
	std::string named_logk;
	LDBLE value;
};
class system_species
{
public:
	~system_species() {};
	system_species()
	{
		name = NULL;
		type = NULL;
		moles = 0;
	}
	char* name;
	char* type;
	LDBLE moles;
};
/* tally.c ------------------------------- */
class tally_buffer
{
public:
	~tally_buffer() {};
	tally_buffer()
	{
		//name.clear();
		master = NULL;
		moles = 0;
		gfw = 0;
	}
	//const char* name;
	std::string name;
	class master* master;
	LDBLE moles;
	LDBLE gfw;
};
class tally
{
public:
	~tally() {};
	tally()
	{
		//name.clear();
		type = UnKnown;
		//add_formula.clear();
		moles = 0;
		//formula.clear();
		/*
		 * first total is initial
		 * second total is final
		 * third total is difference (final - initial)
		 */
		for(size_t i = 0; i < 3; i++) total[i]= NULL;
	}
	//const char* name;
	std::string name;
	enum entity_type type;
	//const char* add_formula;
	std::string add_formula;
	LDBLE moles;
	std::vector<class elt_list> formula;
	/*
	 * first total is initial
	 * second total is final
	 * third total is difference (final - initial)
	 */
	class tally_buffer* total[3];
};
/* transport.c ------------------------------- */
class spec
{
public:
	~spec() {};
	spec()
	{
		// name of species
		name = NULL;
		// name of aqueous species in EX species
		aq_name = NULL;
		// type: AQ or EX
		type = 0;
		// activity
		a = 0;
		// log(concentration)
		lm = 0;
		// log(gamma)
		lg = 0;
		// concentration for AQ, equivalent fraction for EX
		c = 0;
		// charge number
		z = 0;
		// temperature corrected free water diffusion coefficient, m2/s
		Dwt = 0;
		// temperature factor for Dw
		dw_t = 0;
		// enrichment factor in ddl
		erm_ddl = 0;
	}
	const char* name;
	const char* aq_name;
	int type;
	LDBLE a;
	LDBLE lm;
	LDBLE lg;
	LDBLE c;
	LDBLE z;
	LDBLE Dwt;
	LDBLE dw_t;
	LDBLE erm_ddl;
};

class sol_D
{
public:
	~sol_D() {};
	sol_D()
	{
		// number of aqueous + exchange species
		count_spec = 0;
		// number of exchange species
		count_exch_spec = 0;
		// total moles of X-, max X- in transport step in sol_D[1], tk
		exch_total = 0, x_max = 0, tk_x = 0;
		// (tk_x * viscos_0_25) / (298 * viscos) 
		viscos_f = 0;
		spec = NULL;
		spec_size = 0;
	}
	int count_spec;
	int count_exch_spec;
	LDBLE exch_total, x_max, tk_x;
	LDBLE viscos_f;
	class spec* spec;
	int spec_size;
};
class J_ij
{
public:
	~J_ij() {};
	J_ij()
	{
		name = NULL;
		// species change in cells i and j
		tot1 = 0;
		tot2 = 0;
		tot_stag = 0;
		charge = 0;
	}
	const char* name;
	LDBLE tot1, tot2, tot_stag, charge;
};
class M_S
{
public:
	~M_S() {};
	M_S()
	{
		name = NULL;
		// master species transport in cells i and j 
		tot1 = 0;
		tot2 = 0;
		tot_stag = 0;
		charge = 0;
	}
	const char* name;
	LDBLE tot1, tot2, tot_stag, charge;
};
// Pitzer definitions
typedef enum
{ TYPE_B0, TYPE_B1, TYPE_B2, TYPE_C0, TYPE_THETA, TYPE_LAMDA, TYPE_ZETA,
  TYPE_PSI, TYPE_ETHETA, TYPE_ALPHAS, TYPE_MU, TYPE_ETA, TYPE_Other,
  TYPE_SIT_EPSILON, TYPE_SIT_EPSILON_MU, TYPE_APHI
} pitz_param_type;
class pitz_param
{
public:
	~pitz_param() {};
	pitz_param()
	{
		//for(size_t i = 0; i < 3; i++) species[i].clear();
		for (size_t i = 0; i < 3; i++) ispec[i] = -1;
		type = TYPE_Other;
		p = 0;
		U.b0 = 0;
		for (size_t i = 0; i < 6; i++) a[i] = 0;
		alpha = 0;
		os_coef = 0;
		for (size_t i = 0; i < 3; i++) ln_coef[i] = 0;
		thetas = NULL;
	}
	std::string species[3];
	int ispec[3];
	pitz_param_type type;
	LDBLE p;
	union
	{
		LDBLE b0;
		LDBLE b1;
		LDBLE b2;
		LDBLE c0;
		LDBLE theta;
		LDBLE lamda;
		LDBLE zeta;
		LDBLE psi;
		LDBLE alphas;
		LDBLE mu;
		LDBLE eta;
		LDBLE eps;
		LDBLE eps1;
		LDBLE aphi;
	} U;
	LDBLE a[6];
	LDBLE alpha;
	LDBLE os_coef;
	LDBLE ln_coef[3];
	const class theta_param* thetas;
};
class theta_param
{
public:
	~theta_param() {};
	theta_param()
	{
		zj = 0;
		zk = 0;
		etheta = 0;
		ethetap = 0;
	}
	LDBLE zj;
	LDBLE zk;
	LDBLE etheta;
	LDBLE ethetap;
};


#endif /* _INC_GLOBAL_STRUCTURES_H  */