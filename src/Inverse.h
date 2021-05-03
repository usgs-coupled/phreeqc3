#if !defined(INVERSE_H_INCLUDED)
#define INVERSE_H_INCLUDED
#include <string>
#include <vector>

#define TRUE 1
#define FALSE 0
/* constaints in mass balance */
#define EITHER 0
#define DISSOLVE 1
#define PRECIPITATE -1
/*----------------------------------------------------------------------
 *   Inverse
 *---------------------------------------------------------------------- */
class inverse
{
public:
	~inverse() {};
	inverse()
	{
		n_user = -1;
		new_def = FALSE;
		minimal = FALSE;
		range = FALSE;
		mp = FALSE;
		mp_censor = 1e-20;
		range_max = 1000.0;
		tolerance = 1e-10;
		mp_tolerance = 1e-12;
		//uncertainties.clear();
		//ph_uncertainties.clear();
		water_uncertainty = 0.0;
		mineral_water = TRUE;
		carbon = TRUE;
		//dalk_dph.clear();
		//dalk_dc.clear();
		count_solns = 0;
		//solns.clear();
		//force_solns.clear();
		//elts.clear();
		//phases.clear();
		count_redox_rxns = 0;
		//isotopes.clear();
		//i_u.clear();
		//isotope_unknowns.clear();
		//netpath.clear();
		//pat.clear();
	}
	int n_user;
	std::string description;
	int new_def;
	int minimal;
	int range;
	int mp;
	double mp_censor;
	double range_max;
	double tolerance;
	double mp_tolerance;
	std::vector<double> uncertainties;
	std::vector<double> ph_uncertainties;
	double water_uncertainty;
	int mineral_water;
	int carbon;
	std::vector<double> dalk_dph;
	std::vector<double> dalk_dc;
	size_t count_solns;
	std::vector<int> solns;
	std::vector<bool> force_solns;
	std::vector<class inv_elts> elts;
	std::vector<class inv_phases> phases;
	size_t count_redox_rxns;
	std::vector<class inv_isotope> isotopes;
	std::vector<class inv_isotope> i_u;
	std::vector<class isotope> isotope_unknowns;
	std::string netpath;
	std::string pat;
};
class inv_elts
{
public:
	~inv_elts() {};
	inv_elts()
	{
		//name.clear();
		master = NULL;
		row = 0;
		//uncertainties.clear();
	}
	std::string name;
	class master* master;
	size_t row;
	std::vector<double> uncertainties;
};
class inv_isotope
{
public:
	~inv_isotope() {};
	inv_isotope()
	{
		//isotope_name.clear();
		isotope_number = 0;
		//elt_name.clear();
		//uncertainties.clear();
	}
	std::string isotope_name;
	double isotope_number;
	std::string elt_name;
	std::vector<double> uncertainties;
};
class inv_phases
{
public:
	~inv_phases() {};
	inv_phases()
	{
		//phase_name.clear(); // from phase_name_wrapper
		phase = NULL;
		column = 0;
		constraint = EITHER;
		force = FALSE;
		//isotopes.clear();
	}
	int& Get_constraint() { return this->constraint; }
	int& Get_force() { return this->force; }
	std::vector<class isotope>& Get_isotopes() { return this->isotopes; }
	class phase* Get_phase() { return this->phase; }
	std::string& Get_phase_name() { return this->phase_name; }
	void Set_phase_name(const std::string& name) { this->phase_name = name; }
	void Set_phase(class phase* p) { this->phase = p; }

protected:
	std::string phase_name;
	class phase* phase;
	int column;
	int constraint;
	int force;
	std::vector<class isotope> isotopes;
};
#endif // !defined(INVERSE_H_INCLUDED)