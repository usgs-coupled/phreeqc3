#if !defined(CLASSES_H_INCLUDED)
#define CLASSES_H_INCLUDED
#include <vector>
#include "GasPhase.h"
#include "Surface.h"
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
	std::vector<struct phase*> gas_phase;
	std::vector<const char*> ss_assemblage;
	std::vector<struct phase*> pp_assemblage;
	std::vector<double> si;
	std::vector<const char*> add_formula;
	cxxSurface::DIFFUSE_LAYER_TYPE dl_type;
	cxxSurface::SURFACE_TYPE surface_type;
	std::vector<const char*> surface_comp;
	std::vector<const char*> surface_charge;
};
#endif // !defined(CLASSES_H_INCLUDED)