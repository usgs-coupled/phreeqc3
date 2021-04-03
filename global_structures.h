#ifndef _INC_GLOBAL_STRUCTURES_H
#define _INC_GLOBAL_STRUCTURES_H
#include "Surface.h"
#include "GasPhase.h"
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
#define F_KCAL_V_EQ 23.0623		/* kcal/volt-eq */
#define R_LITER_ATM 0.0820597	/* L-atm/deg-mol */
#define R_KCAL_DEG_MOL 0.00198726	/* kcal/deg-mol */
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

//
// Typedefs and structure definitions
//
typedef enum { kcal, cal, kjoules, joules } DELTA_H_UNIT;
typedef enum { cm3_per_mol, dm3_per_mol, m3_per_mol } DELTA_V_UNIT;
enum entity_type
{ Solution, Reaction, Exchange, Surface, Gas_phase, Pure_phase, Ss_phase,
	Kinetics, Mix, Temperature, Pressure, UnKnown
};

typedef enum {
	logK_T0,
	delta_h,
	T_A1,
	T_A2,
	T_A3,
	T_A4,
	T_A5,
	T_A6,
	delta_v,	/* set in calc_delta_v: calculated molar volume-change of the reaction */
	vm_tc,		/* set in calc_vm: calculated molal volume of the species at tc */
	vm0,		/* read: molar volume of a phase */
	vma1, vma2, vma3, vma4, /* read: a1..a4 from supcrt, see calc_vm */
	wref,       /* from supcrt */
	b_Av,		/* b in z^2 * A_v * log(1 + b * I^0.5) / (2 * b) */
	vmi1, vmi2, vmi3, vmi4, /* ionic strength terms: (i1 + i2/(TK - 228) + i3 * (TK - 228) ) * I^i4 */
	MAX_LOG_K_INDICES	/* Keep this definition at the end of the enum */
} LOG_K_INDICES;

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
	const char *comp_name = NULL;
	LDBLE fraction = 0.5;
	const char *new_comp_name = NULL;
	LDBLE new_Dw = 1e-9;
	int cell_no = 0;
	int next = 0;
};
/*----------------------------------------------------------------------
 *   CReaction
 *---------------------------------------------------------------------- */

class CReaction
{
public:
	CReaction(void);
	CReaction(size_t ntoken);
	~CReaction(void) {}
	double* Get_logk(void) { return this->logk; }
	void   Set_logk(double* d);
	double* Get_dz(void) { return this->dz; }
	void   Set_dz(double* d);
	size_t size() { return token.size(); }
	std::vector<struct rxn_token>& Get_tokens(void) { return this->token; }
	void Set_tokens(const std::vector<struct rxn_token>& t) { this->token = t; }

public:
	double logk[MAX_LOG_K_INDICES];
	double dz[3];
	std::vector<struct rxn_token> token;
};
struct rxn_token
{
	struct species* s = NULL;
	LDBLE coef = 0.0;
	const char* name = NULL;
};
struct save
{
	int solution = 0;
	int n_solution_user = 0;
	int n_solution_user_end = 0;
	int mix = 0;
	int n_mix_user = 0;
	int n_mix_user_end = 0;
	int reaction = 0;
	int n_reaction_user = 0;
	int n_reaction_user_end = 0;
	int pp_assemblage = 0;
	int n_pp_assemblage_user = 0;
	int n_pp_assemblage_user_end = 0;
	int exchange = 0;
	int n_exchange_user = 0;
	int n_exchange_user_end = 0;
	int kinetics = 0;
	int n_kinetics_user = 0;
	int n_kinetics_user_end = 0;
	int surface = 0;
	int n_surface_user = 0;
	int n_surface_user_end = 0;
	int gas_phase = 0;
	int n_gas_phase_user = 0;
	int n_gas_phase_user_end = 0;
	int ss_assemblage = 0;
	int n_ss_assemblage_user = 0;
	int n_ss_assemblage_user_end = 0;
};

/*----------------------------------------------------------------------
 *   Copy
 *---------------------------------------------------------------------- */
struct copier
{
	std::vector<int> n_user, start, end;
};

/*----------------------------------------------------------------------
 *   Inverse
 *---------------------------------------------------------------------- */
struct inverse
{
	int n_user        = -1;
	char *description = NULL;
	int new_def       = FALSE;
	int minimal       = FALSE;
	int range         = FALSE;
	int mp            = FALSE;
	LDBLE mp_censor   = 1e-20;
	LDBLE range_max   = 1000.0;
	LDBLE tolerance   = 1e-10;
	LDBLE mp_tolerance = 1e-12;
	std::vector<double> uncertainties;
	std::vector<double> ph_uncertainties;
	LDBLE water_uncertainty = 0.0;
	int mineral_water = TRUE;
	int carbon        = TRUE;
	std::vector<double> dalk_dph;
	std::vector<double> dalk_dc;
	size_t count_solns = 0;
	std::vector<int> solns;
	std::vector<bool> force_solns;
	std::vector<struct inv_elts> elts;
	std::vector<struct inv_phases> phases;
	size_t count_redox_rxns = 0;
	std::vector<struct inv_isotope> isotopes;
	std::vector<struct inv_isotope> i_u;
	std::vector<struct isotope> isotope_unknowns;
	const char *netpath = NULL;
	const char *pat = NULL;
};
struct inv_elts
{
	const char *name = NULL;
	struct master *master = NULL;
	size_t row = 0;
	std::vector<double> uncertainties;
};
struct inv_isotope
{
	const char *isotope_name = NULL;
	LDBLE isotope_number = 0;
	const char *elt_name = NULL;
	std::vector<double> uncertainties;
};
struct inv_phases
{
	const char *name = NULL;
	struct phase *phase = NULL;
	int column = 0;
	int constraint = EITHER;
	int force = FALSE;
	std::vector<struct isotope> isotopes;
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

	bool force_prep = false;
	bool numerical_fixed_volume = false;
	cxxGasPhase::GP_TYPE gas_phase_type = cxxGasPhase::GP_UNKNOWN;
	std::vector<struct phase*> gas_phase;
	std::vector<const char*> ss_assemblage;
	std::vector<struct phase*> pp_assemblage;
	std::vector<double> si;
	std::vector<const char*> add_formula;
	cxxSurface::DIFFUSE_LAYER_TYPE dl_type = cxxSurface::NO_DL;
	cxxSurface::SURFACE_TYPE surface_type = cxxSurface::UNKNOWN_DL;
	std::vector<const char*> surface_comp;
	std::vector<const char*> surface_charge;
};

struct name_coef
{
	const char* name = NULL;
	LDBLE coef = 0;
};
/*----------------------------------------------------------------------
 *   Species_list
 *---------------------------------------------------------------------- */
struct species_list
{
	struct species *master_s = NULL;
	struct species *s = NULL;
	LDBLE coef = 0;
};

/*----------------------------------------------------------------------
 *   Jacobian and Mass balance lists
 *---------------------------------------------------------------------- */
struct list0
{
	LDBLE *target = NULL;
	LDBLE coef = 0;
};
struct list1
{
	LDBLE *source = NULL;
	LDBLE *target = NULL;
};
struct list2
{
	LDBLE *source = NULL;
	LDBLE *target = NULL;
	LDBLE coef = 0;
};

 struct isotope
 {
 	LDBLE isotope_number = 0;
 	const char *elt_name = NULL;
 	const char *isotope_name = NULL;
 	LDBLE total = 0;
 	LDBLE ratio = 0;
 	LDBLE ratio_uncertainty = 0;
 	LDBLE x_ratio_uncertainty = 0;
 	struct master *master = NULL;
 	struct master *primary = NULL;
 	LDBLE coef = 0;					/* coefficient of element in phase */
};
struct iso
{
	const char *name = NULL;
	LDBLE value = 0;
	LDBLE uncertainty = 0.05;
};
/*----------------------------------------------------------------------
 *   Transport data
 *---------------------------------------------------------------------- */
struct stag_data
{
	int count_stag = 0;
	LDBLE exch_f = 0;
	LDBLE th_m = 0;
	LDBLE th_im = 0;
};
struct cell_data
{
	LDBLE length = 1;
	LDBLE mid_cell_x = 1.;
	LDBLE disp = 1.0;
	LDBLE temp = 25.;
	// free (uncharged) porewater porosities
	LDBLE por = 0.1;
	// interlayer water porosities
	LDBLE por_il = 0.01;
	// potential (V)
	LDBLE potV = 0;
	int punch = FALSE;
	int print = FALSE;
	int same_model = FALSE;
};

/*----------------------------------------------------------------------
 *   Keywords
 *---------------------------------------------------------------------- */
 struct key
 {
 	char *name = NULL;
 	int keycount = 0;
 };
 struct const_key
 {
 	const char *name = NULL;
 	int keycount = 0;
};

/*----------------------------------------------------------------------
 *   Elements
 *---------------------------------------------------------------------- */
struct element
{
	// element name
	const char *name = NULL;					
	/*    int in; */
	struct master *master = NULL;
	struct master *primary = NULL;
	LDBLE gfw = 0;
};
/*----------------------------------------------------------------------
 *   Element List
 *---------------------------------------------------------------------- */
struct elt_list
{								/* list of name and number of elements in an equation */
	struct element *elt = NULL;	/* pointer to element structure */
	LDBLE coef = 0.0;			/* number of element e's in eqn */
};

/*----------------------------------------------------------------------
 *   Species
 *---------------------------------------------------------------------- */
struct species
{								/* all data pertinent to an aqueous species */
	// name of species 
	const char *name = NULL;	
	// formula for mole balance 
	const char *mole_balance = NULL;
	// set internally if species in model 
	int in = FALSE;						
	int number = 0;
	// points to master species list, NULL if not primary master
	struct master *primary = NULL;	
	// points to master species list, NULL if not secondary master
	struct master *secondary = NULL;
	// gram formula wt of species
	LDBLE gfw = 0;	
	// charge of species
	LDBLE z = 0;
	// tracer diffusion coefficient in water at 25oC, m2/s
	LDBLE dw = 0;
	// correct Dw for temperature: Dw(TK) = Dw(298.15) * exp(dw_t / TK - dw_t / 298.15)
	LDBLE dw_t = 0;
	// parms for calc'ng SC = SC0 * exp(-dw_a * z * mu^0.5 / (1 + DH_B * dw_a2 * mu^0.5))
	LDBLE dw_a = 0;
	LDBLE dw_a2 = 0;
	// viscosity correction of SC
	LDBLE dw_a_visc = 0;
	// contribution to SC, for calc'ng transport number with BASIC
	LDBLE dw_t_SC = 0;
	// dw corrected for TK and mu
	LDBLE dw_corr = 0;
	// enrichment factor in DDL
	LDBLE erm_ddl = 0;
	// equivalents in exchange species
	LDBLE equiv = 0;
	// alkalinity of species, used for cec in exchange
	LDBLE alk = 0;
	// stoichiometric coefficient of carbon in species
	LDBLE carbon = 0;
	// stoichiometric coefficient of C(4) in species
	LDBLE co2 = 0;
	// stoichiometric coefficient of H in species
	LDBLE h = 0;
	// stoichiometric coefficient of O in species
	LDBLE o = 0;
	// WATEQ Debye Huckel a and b-dot; active_fraction coef for exchange species
	LDBLE dha = 0, dhb = 0, a_f = 0;
	// log10 k at working temperature
	LDBLE lk = 0;
	// log kt0, delh, 6 coefficients analytical expression + volume terms
	LDBLE logk[MAX_LOG_K_INDICES] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	// 7 coefficients analytical expression for B, D, anion terms and pressure in Jones_Dole viscosity eqn
	LDBLE Jones_Dole[10] = { 0,0,0,0,0,0,0,0,0,0 };
/* VP: Density Start */
	// regression coefficients to calculate temperature dependent phi_0and b_v of Millero density model
	LDBLE millero[7] = { 0,0,0,0,0,0,0 };
	/* VP: Density End */
	// enum with original delta H units
	DELTA_H_UNIT original_units = kjoules;
	std::vector<struct name_coef> add_logk;
	// log10 activity coefficient, gamma
	LDBLE lg = 0;
	// log10 activity coefficient, from pitzer calculation
	LDBLE lg_pitzer = 0;
	// log10 molality
	LDBLE lm = 0;
	// log10 activity
	LDBLE la = 0;
	// gamma term for jacobian
	LDBLE dg = 0;
	LDBLE dg_total_g = 0;
	// moles in solution; moles/mass_water = molality
	LDBLE moles = 0;
	// flag indicating presence in model and types of equations
	int type = 0;
	// flag for preferred activity coef eqn
	int gflag = 0;
	// flag for preferred activity coef eqn
	int exch_gflag = 0;
	// vector of elements
	std::vector<struct elt_list> next_elt; 
	std::vector<struct elt_list> next_secondary;
	std::vector<struct elt_list> next_sys_total;
	// switch to check equation for charge and element balance
	int check_equation = TRUE;
	// data base reaction
	CReaction rxn;
	// reaction converted to secondary and primary master species
	CReaction rxn_s;
	// reaction to be used in model
	CReaction rxn_x;
	// (1 + sum(g)) * moles
	LDBLE tot_g_moles = 0;
	// sum(moles*g*Ws/Waq)
	LDBLE tot_dh2o_moles = 0;
	LDBLE cd_music[5] = { 0,0,0,0,0 };
	LDBLE dz[3] = { 0,0,0 };
	DELTA_V_UNIT original_deltav_units = cm3_per_mol;
};
struct logk
{								/* Named log K's */
	const char *name = NULL;		           // name of species 
	LDBLE lk = 0.0;	                           // log10 k at working temperature 
	LDBLE log_k[MAX_LOG_K_INDICES] =		   // log kt0, delh, 6 coefficients analalytical expression 
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	DELTA_H_UNIT original_units = kjoules;	   // enum with original delta H units 
	int done = FALSE;
	std::vector<struct name_coef> add_logk;
	LDBLE log_k_original[MAX_LOG_K_INDICES] =  // log kt0, delh, 5 coefficients analalytical expression */
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	DELTA_V_UNIT original_deltav_units = cm3_per_mol;
};

/*----------------------------------------------------------------------
 *   Phases
 *---------------------------------------------------------------------- */
struct phase
{		/* all data pertinent to a pure solid phase */
	const char *name = NULL;                //name of species 
	const char *formula = NULL;				// chemical formula 
	int in = FALSE;						    // species used in model if TRUE 
	LDBLE lk = 0;					        // log10 k at working temperature 
	LDBLE logk[MAX_LOG_K_INDICES] =			// log kt0, delh, 6 coefficients analalytical expression
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	DELTA_H_UNIT original_units = kjoules;	// enum with original delta H units 
	DELTA_V_UNIT original_deltav_units = cm3_per_mol;
	std::vector<struct name_coef> add_logk;
	LDBLE moles_x = 0;
	LDBLE delta_max = 0;
	LDBLE p_soln_x = 0;
	LDBLE fraction_x = 0;
	LDBLE log10_lambda = 0;
	LDBLE log10_fraction_x = 0;
	LDBLE dn = 0, dnb = 0, dnc = 0;
	LDBLE gn = 0, gntot = 0;
	LDBLE gn_n = 0, gntot_n = 0;
	LDBLE t_c = 0, p_c = 0, omega = 0;          // gas: critical TK, critical P(atm), Pitzer acentric coeff 
	LDBLE pr_a = 0, pr_b = 0, pr_alpha = 0;		// Peng-Robinson parm's
	LDBLE pr_tk = 0, pr_p = 0;			        // Temperature (K), Pressure (atm)
	LDBLE pr_phi = 0;				            // fugacity coefficient (-) 
	LDBLE pr_aa_sum2 = 0;			            // for calculating multicomponent phi 
	LDBLE delta_v[9] = { 0,0,0,0,0,0,0,0,0 };   // delta_v[0] = [1] + [2]*T + [3]/T + [4]*log10(T) + [5]/T^2 + [6]*T^2 + [7]*P 
	LDBLE pr_si_f = 0;				            // si adapter: log10(phi) - delta_v[0] * (P - 1) /RT
	bool pr_in = false;					        // Peng-Robinson in the calc's, or not
	int type = SOLID;					        // flag indicating presence in model and types of equations
	std::vector<struct elt_list> next_elt;	    // list of elements in phase 
	std::vector<struct elt_list> next_sys_total;
	int check_equation = TRUE;			        // switch to check equation for charge and element balance
	CReaction rxn;		                        // pointer to data base reaction
	CReaction rxn_s;		                    // pointer to reaction converted to secondary and primary master species
	CReaction rxn_x;		                    // reaction to be used in model
	int replaced = FALSE;                       // equation contains solids or gases
	int in_system = FALSE;
};
/*----------------------------------------------------------------------
 *   Master species
 *---------------------------------------------------------------------- */
 struct master
 {		// list of name and number of elements in an equation	
	// TRUE if in model, FALSE if out, REWRITE if other mb eq
 	int in = 0;	
	// sequence number in list of masters
 	size_t number = 0;
	// saved to determine if model has changed
 	int last_model = FALSE;	
	// AQ or EX
 	int type = 0;
	// TRUE if master species is primary
 	int primary = FALSE;
	// coefficient of element in master species
 	LDBLE coef = 0;
	// total concentration for element or valence state
 	LDBLE total = 0;
 	LDBLE isotope_ratio = 0;
 	LDBLE isotope_ratio_uncertainty = 0;
 	int isotope = 0;
 	LDBLE total_primary = 0;
	// element structure
 	struct element *elt = NULL;
	// alkalinity of species
 	LDBLE alk = 0;
	// default gfw for species
 	LDBLE gfw = 1;
	// formula from which to calcuate gfw
 	const char *gfw_formula = NULL;
	// pointer to unknown structure
 	struct unknown *unknown = NULL;
	// pointer to species structure
 	struct species *s = NULL;
	// reaction writes master species in terms of primary  master species
	CReaction rxn_primary;
	// reaction writes master species in terms of secondary master species
	CReaction rxn_secondary;
	const char * pe_rxn = NULL;
 	int minor_isotope = FALSE;
};
/*----------------------------------------------------------------------
 *   Unknowns
 *---------------------------------------------------------------------- */
struct unknown
{
	int type = 0;
	LDBLE moles = 0;
	LDBLE ln_moles = 0;
	LDBLE f = 0;
	LDBLE sum = 0;
	LDBLE delta = 0;
	LDBLE la = 0;
	size_t number = 0;
	const char *description = NULL;
	std::vector<struct master*> master;
	struct phase *phase = NULL;
	LDBLE si = 0;
	int n_gas_phase_user = 0;
	struct species *s = NULL;
	const char * exch_comp = NULL;
	const char *pp_assemblage_comp_name = NULL;
	void *pp_assemblage_comp_ptr = NULL;
	const char * ss_name = NULL;
	void *ss_ptr = NULL;
	const char * ss_comp_name = NULL;
	void *ss_comp_ptr = NULL;
	int ss_comp_number = 0;
	int ss_in = FALSE;
	const char *surface_comp = NULL;
	const char *surface_charge = NULL;
	LDBLE related_moles = 0;
	struct unknown *potential_unknown = NULL, *potential_unknown1 = NULL,
		*potential_unknown2 = NULL;
	std::vector<struct unknown*> comp_unknowns; /* list for CD_MUSIC of comps that contribute to 0 plane mass-balance term */
	struct unknown *phase_unknown = NULL;
	LDBLE mass_water = 1;
	int dissolve_only = FALSE;
	LDBLE inert_moles = 0;
	LDBLE V_m = 0;
	LDBLE pressure = 1;
	int mb_number = 0;
	int iteration = 0;
};

/*----------------------------------------------------------------------
 *   Reaction work space
 *---------------------------------------------------------------------- */
struct reaction_temp
{
	LDBLE logk[MAX_LOG_K_INDICES] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	LDBLE dz[3] = { 0,0,0 };
	std::vector<struct rxn_token_temp> token;
};
struct rxn_token_temp
{								/* data for equations, aq. species or minerals */
	const char *name = NULL;					/* pointer to a species name (formula) */
	LDBLE z = 0;					/* charge on species */
	struct species *s = NULL;
	struct unknown *unknown = NULL;
	LDBLE coef = 0;					/* coefficient of species name */
};
struct unknown_list
{
	struct unknown *unknown = NULL;
	LDBLE *source = NULL;
	LDBLE *gamma_source = NULL;
	/*    int row; */
	/*    int col; */
	LDBLE coef = 0;
};
/* ----------------------------------------------------------------------
 *   Print
 * ---------------------------------------------------------------------- */
struct prints
{
	int all = 0;
	int initial_solutions = 0;
	int initial_exchangers = 0;
	int reactions = 0;
	int gas_phase = 0;
	int ss_assemblage = 0;
	int pp_assemblage = 0;
	int surface = 0;
	int exchange = 0;
	int kinetics = 0;
	int totals = 0;
	int eh = 0;
	int species = 0;
	int saturation_indices = 0;
	int irrev = 0;
	int mix = 0;
	int reaction = 0;
	int use = 0;
	int logfile = 0;
	int punch = 0;
	int status = 0;
	int inverse = 0;
	int dump = 0;
	int user_print = 0;
	int headings = 0;
	int user_graph = 0;
	int echo_input = 0;
	int warnings = 0;
	int initial_isotopes = 0;
	int isotope_ratios = 0;
	int isotope_alphas = 0;
	int hdf = 0;
	int alkalinity = 0;
};
/* ----------------------------------------------------------------------
 *   RATES
 * ---------------------------------------------------------------------- */
struct rate
{
	const char *name = NULL;
	std::string commands;
	int new_def = 0;
	void *linebase = NULL;
	void *varbase = NULL;
	void *loopbase = NULL;
};
/* ----------------------------------------------------------------------
 *   GLOBAL DECLARATIONS
 * ---------------------------------------------------------------------- */
struct spread_row
{
	int count = 0;
	int empty = 0, string = 0, number = 0;
	std::vector<char*> char_vector;
	std::vector<double> d_vector;
	std::vector<int> type_vector;
};
struct defaults
{
	LDBLE temp = 0;
	LDBLE density = 0;
	bool calc_density = 0;
	const char *units = NULL;
	const char *redox = NULL;
	LDBLE ph = 7;
	LDBLE pe = 4;
	LDBLE water = 1;
	std::vector<struct iso> iso;
	LDBLE pressure = 1;	/* pressure in atm */
};
struct spread_sheet
{
	struct spread_row *heading = NULL;
	struct spread_row *units = NULL;
	int count_rows = 0;
	struct spread_row **rows = NULL;
	struct defaults defaults;
};
/* ----------------------------------------------------------------------
 *   ISOTOPES
 * ---------------------------------------------------------------------- */
struct master_isotope
{
	const char *name = NULL;
	struct master *master = NULL;
	struct element *elt = NULL;
	const char *units = NULL;
	LDBLE standard = 0;
	LDBLE ratio = 0;
	LDBLE moles = 0;
	int total_is_major = 0;
	int minor_isotope = 0;
};
struct calculate_value
{
	const char *name = NULL;
	LDBLE value = 0;
	std::string commands;
	int new_def = 0;
	int calculated = 0;
	void *linebase = NULL;
	void *varbase = NULL;
	void *loopbase = NULL;
};
struct isotope_ratio
{
	const char *name = NULL;
	const char *isotope_name = NULL;
	LDBLE ratio = 0;
	LDBLE converted_ratio = 0;
};
struct isotope_alpha
{
	const char *name = NULL;
	const char *named_logk = NULL;
	LDBLE value = 0;
};
struct system_species
{
	char *name = NULL;
	char *type = NULL;
	LDBLE moles = 0;
};

/* tally.c ------------------------------- */
struct tally_buffer
{
	const char *name = NULL;
	struct master *master = NULL;
	LDBLE moles = 0;
	LDBLE gfw = 0;
};
struct tally
{
	const char *name = NULL;
	enum entity_type type = UnKnown;
	const char *add_formula = NULL;
	LDBLE moles = 0;
	std::vector<struct elt_list> formula;
	/*
	 * first total is initial
	 * second total is final
	 * third total is difference (final - initial)
	 */
	struct tally_buffer* total[3] = { NULL, NULL, NULL };
};

/* transport.c ------------------------------- */
struct spec
{
	// name of species
	const char *name = NULL;
	// name of aqueous species in EX species
	const char *aq_name = NULL;
	// type: AQ or EX
	int type = 0;
	// activity
	LDBLE a = 0;
	// log(concentration)
	LDBLE lm = 0;
	// log(gamma)
	LDBLE lg = 0;
	// concentration for AQ, equivalent fraction for EX
	LDBLE c = 0;
	// charge number
	LDBLE z = 0;
	// temperature corrected free water diffusion coefficient, m2/s
	LDBLE Dwt = 0;
	// temperature factor for Dw
	LDBLE dw_t = 0;
	// enrichment factor in ddl
	LDBLE erm_ddl = 0;
};
struct sol_D
{
	// number of aqueous + exchange species
	int count_spec = 0;
	// number of exchange species
	int count_exch_spec = 0;
	// total moles of X-, max X- in transport step in sol_D[1], tk
	LDBLE exch_total = 0, x_max = 0, tk_x = 0;	
	// (tk_x * viscos_0_25) / (298 * viscos) 
	LDBLE viscos_f = 0;
	struct spec *spec = NULL;
	int spec_size = 0;
};
struct J_ij
{
	const char *name = NULL;
	// species change in cells i and j
	LDBLE tot1 = 0, tot2 = 0, tot_stag = 0, charge = 0;  
};
struct M_S
{
	const char *name = NULL;
	// master species transport in cells i and j 
	LDBLE tot1 = 0, tot2 = 0, tot_stag = 0, charge = 0; 
};
// Pitzer definitions
typedef enum
{ TYPE_B0, TYPE_B1, TYPE_B2, TYPE_C0, TYPE_THETA, TYPE_LAMDA, TYPE_ZETA,
  TYPE_PSI, TYPE_ETHETA, TYPE_ALPHAS, TYPE_MU, TYPE_ETA, TYPE_Other,
  TYPE_SIT_EPSILON, TYPE_SIT_EPSILON_MU, TYPE_APHI
} pitz_param_type;

struct pitz_param
{
	const char* species[3] = { NULL,NULL,NULL };
	int ispec[3] = { 0,0,0 };
	pitz_param_type type = TYPE_B0;
	LDBLE p = 0;
	union
	{
		LDBLE b0 = 0;
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
	LDBLE a[6] = { 0,0,0,0,0,0 };
	LDBLE alpha = 0;
	LDBLE os_coef = 0;
	LDBLE ln_coef[3] = { 0,0,0 };
	struct theta_param *thetas = NULL;
};

struct theta_param
{
	LDBLE zj = 0;
	LDBLE zk = 0;
	LDBLE etheta = 0;
	LDBLE ethetap = 0;
};

struct const_iso
{
	const char *name = NULL;
	LDBLE value = 0;
	LDBLE uncertainty = 0;
};

#endif /* _INC_GLOBAL_STRUCTURES_H  */

