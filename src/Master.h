#if !defined(MASTER_H_INCLUDED)
#define MASTER_H_INCLUDED
#define TRUE 1
#define FALSE 0
#include <string>
#include "CReaction.h"
/*----------------------------------------------------------------------
 *   Master species
 *---------------------------------------------------------------------- */
class master
{
public:
	~master() {};
	master()
	{
		// TRUE if in model, FALSE if out, REWRITE if other mb eq
		in = 0;
		// sequence number in list of masters
		number = 0;
		// saved to determine if model has changed
		last_model = FALSE;
		// AQ or EX
		type = 0;
		// TRUE if master species is primary
		primary = FALSE;
		// coefficient of element in master species
		coef = 0;
		// total concentration for element or valence state
		total = 0;
		isotope_ratio = 0;
		isotope_ratio_uncertainty = 0;
		isotope = 0;
		total_primary = 0;
		// element structure
		elt = NULL;
		// alkalinity of species
		alk = 0;
		// default gfw for species
		gfw = 1;
		// formula from which to calcuate gfw
		//gfw_formula.clear();
		// pointer to unknown structure
		unknown = NULL;
		// pointer to species structure
		s = NULL;
		// reaction writes master species in terms of primary  master species
		rxn_primary.Initialize(false);
		// reaction writes master species in terms of secondary master species
		rxn_secondary.Initialize(false);
		//pe_rxn.clear();
		minor_isotope = FALSE;
	}
	int in;
	size_t number;
	int last_model;
	int type;
	int primary;
	double coef;
	double total;
	double isotope_ratio;
	double isotope_ratio_uncertainty;
	int isotope;
	double total_primary;
	class element* elt;
	double alk;
	double gfw;
	std::string gfw_formula;
	class unknown* unknown;
	class species* s;
	CReaction rxn_primary;
	CReaction rxn_secondary;
	std::string pe_rxn;
	int minor_isotope;
};
#endif // !defined(MASTER_H_INCLUDED)