#if !defined(LOGK_H_INCLUDED)
#define LOGK_H_INCLUDED
#include "global_structures.h"
#include <vector>
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
		logk.resize(MAX_LOG_K_INDICES, 0);
		done = FALSE;
		//add_logk.clear();
		// enum with original delta H units 
		original_units = kjoules;
		// enum with original delta V units 
		original_deltav_units = cm3_per_mol;
	}
	void Set_name(const std::string& str) { this->name = str; }
	const std::string& Get_name() { return this->name; }
	void Set_logk(const std::vector<double>& k);
	void Set_selected();
	const std::vector<double>& Get_logk() { return this->logk; }
	const std::vector<double>& Get_logk_original() { return this->logk_original; }
	const std::vector<double>& Get_logk_selected() { return this->logk_selected; }

	double k_calc(double tempk, double presPa, Phreeqc* phrq_ptr);
	class logk* Newlogk();
	void tidy_logk(Phreeqc* phrq_ptr);
	std::string name;
	double lk;
	std::vector<double> logk_original;
	std::vector<double> logk_selected;
	std::vector<double> logk;
	int done;
	std::vector<class name_coef> add_logk;
	DELTA_H_UNIT original_units;
	DELTA_V_UNIT original_deltav_units;
protected:
	//void add_other_logk(Phreeqc* phrq_ptr);
	//void add_other_logk(size_t repeat, Phreeqc* phrq_ptr);
	void add_logks(std::vector<double>& lk, class Logk& next_Logk, 
		double coef, size_t repeats, Phreeqc* phrq_ptr);
};
#endif // !defined(LOGK_H_INCLUDED)