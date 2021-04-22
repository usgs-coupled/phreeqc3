#if !defined(LOGK_H_INCLUDED)
#define LOGK_H_INCLUDED
//#include "global_structures.h"
#include <vector>
#include <string>
#include "Name_coef.h"
class Phreeqc;
class Logk
{
public:
	~Logk() {};
	Logk()
	{	/* Named log K's */
		//name.clear();		 // name of log_k 
		lk = 0.0;	         // log10 k at working temperature                   
		// log kt0, delh, 6 coefficients analalytical expression 
		logk_original.resize(MAX_LOG_K_INDICES, 0);
		logk_selected.resize(MAX_LOG_K_INDICES, 0);
		logk_x.resize(MAX_LOG_K_INDICES, 0);
		//add_logk.clear();
		// enum with original delta H units 
		original_units = Logk::kjoules;
		// enum with original delta V units 
		original_deltav_units = Logk::cm3_per_mol;
	}
	typedef enum { kcal, cal, kjoules, joules } DELTA_H_UNIT;
	typedef enum { cm3_per_mol, dm3_per_mol, m3_per_mol } DELTA_V_UNIT;
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


	void Set_name(const std::string& str) { this->name = str; }
	const std::string& Get_name() { return this->name; }
	void Set_logk_original(const std::vector<double>& k);
	void Set_selected();
	std::vector<double>& Get_logk_x() { return this->logk_x; }
	std::vector<double>& Get_logk_original() { return this->logk_original; }
	std::vector<double>& Get_logk_selected() { return this->logk_selected; }

	double Calc_Logk(double tempk, double presPa/*, Phreeqc* phrq_ptr*/);
	void tidy_logk(Phreeqc* phrq_ptr);
	std::string name;
	double lk;
	std::vector<double> logk_original;
	std::vector<double> logk_selected;
	std::vector<double> logk_x;
	std::vector<class name_coef> add_logk;
	DELTA_H_UNIT original_units;
	DELTA_V_UNIT original_deltav_units;
protected:
	void add_logks(std::vector<double>& lk, class Logk& next_Logk,
		double coef, size_t repeats, Phreeqc* phrq_ptr);
};
#endif // !defined(LOGK_H_INCLUDED)