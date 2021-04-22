#include "Logk.h"
#include "Phreeqc.h"
/* ---------------------------------------------------------------------- */
void Logk::
Set_logk_original(const std::vector<double>& k)
/* ---------------------------------------------------------------------- */
{
	this->logk_original = k;
	this->logk_original.resize(Logk::MAX_LOG_K_INDICES, 0);
	this->Set_selected();
	return;
}
/* ---------------------------------------------------------------------- */
void Logk::
Set_selected()
/* ---------------------------------------------------------------------- */
{
	this->logk_selected = logk_original;
	bool analytic = false;
	for (size_t j = Logk::T_A1; j <= Logk::T_A6; j++)
	{
		if (logk_original[j] != 0.0)
		{
			analytic = true;
			break;
		}
	}
	if (analytic)
	{
		logk_selected[Logk::logK_T0] = 0.0;
		logk_selected[Logk::delta_h] = 0.0;
	}
	return;
}
void Logk::
tidy_logk(Phreeqc* phrq_ptr)
{
	//logk_x.clear();
	//logk_x.resize(Logk::MAX_LOG_K_INDICES, 0.0);
	logk_x = logk_selected;
	add_logks(logk_x, *this, 1.0, 0, phrq_ptr);
}
/* ---------------------------------------------------------------------- */
double Logk::
Calc_Logk(double tempk, double presPa/*, Phreeqc* phrq_ptr*/)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Calculates log k at specified temperature and pressure
	 *   Returns calculated log k.
	 *
	 *   Logk::delta_v is in cm3/mol.
	 */
	//logk = logk_selected;
	//this->add_other_logk(phrq_ptr);
	double LOG_10 = log(10.0);
	 /* Molar energy */
	LDBLE me = tempk * R_KJ_DEG_MOL;

	/* Pressure difference */
	LDBLE delta_p = presPa - REF_PRES_PASCAL;

	/* Calculate new log k value for this temperature and pressure */
	LDBLE lk = logk_x[Logk::logK_T0]
		- logk_x[Logk::delta_h] * (298.15 - tempk) / (LOG_10 * me * 298.15)
		+ logk_x[Logk::T_A1]
		+ logk_x[Logk::T_A2] * tempk
		+ logk_x[Logk::T_A3] / tempk
		+ logk_x[Logk::T_A4] * log10(tempk)
		+ logk_x[Logk::T_A5] / (tempk * tempk)
		+ logk_x[Logk::T_A6] * tempk * tempk;
	if (delta_p > 0)
		/* cm3 * J /mol = 1e-9 m3 * kJ /mol */
		lk -= logk_x[Logk::delta_v] * 1E-9 * delta_p / (LOG_10 * me);
	return lk;
}
#ifdef SKIP
/* ---------------------------------------------------------------------- */
void Logk::
add_logks(std::vector<double>& lk, class Logk& next_Logk, double coef, size_t repeats, Phreeqc* phrq_ptr)
/* ---------------------------------------------------------------------- */
{
	if (repeats > 15)
	{
		std::ostringstream oss;
		oss << "Circular definition of named_logk? " << this->name;
		phrq_ptr->error_msg(oss.str().c_str(), CONTINUE);
		return;
	}
	for (size_t j = 0; j < Logk::MAX_LOG_K_INDICES; j++)
	{
		lk[j] += next_Logk.logk_selected[j] * coef;
	}
	for (size_t i = 0; i < next_Logk.add_logk.size(); i++)
	{
		double coef_add = next_Logk.add_logk[i].coef * coef;
		std::string token = next_Logk.add_logk[i].name;
		phrq_ptr->str_tolower(token);
		std::map<std::string, class Logk>::iterator it = 
			phrq_ptr->Get_Logk_map().find(token);
		if (it == phrq_ptr->Get_Logk_map().end())
		{
			std::ostringstream oss;
			oss << "Could not find named temperature expression, " << next_Logk.name;
			phrq_ptr->error_msg(oss.str().c_str(), CONTINUE);
			return;
		}
		for (size_t j = 0; j < Logk::MAX_LOG_K_INDICES; j++)
		{
			lk[j] += next_Logk.logk_selected[j] * coef_add;
			//lk[j] += it->second.logk_x[j] * coef_add;
		}
		add_logks(lk, it->second, coef_add, ++repeats, phrq_ptr);
	}
}
#endif
/* ---------------------------------------------------------------------- */
void Logk::
add_logks(std::vector<double>& lk, class Logk& next_Logk, double coef, size_t repeats, Phreeqc* phrq_ptr)
/* ---------------------------------------------------------------------- */
{
	if (repeats > 15)
	{
		std::ostringstream oss;
		oss << "Circular definition of named_logk? " << this->name;
		phrq_ptr->error_msg(oss.str().c_str(), CONTINUE);
		return;
	}
	//for (size_t j = 0; j < Logk::MAX_LOG_K_INDICES; j++)
	//{
	//	lk[j] += next_Logk.logk_selected[j] * coef;
	//}
	for (size_t i = 0; i < next_Logk.add_logk.size(); i++)
	{
		double coef_add = next_Logk.add_logk[i].coef * coef;
		std::string token = next_Logk.add_logk[i].name;
		phrq_ptr->str_tolower(token);
		std::map<std::string, class Logk>::iterator it =
			phrq_ptr->Get_Logk_map().find(token);
		if (it == phrq_ptr->Get_Logk_map().end())
		{
			std::ostringstream oss;
			oss << "Could not find named temperature expression, " << next_Logk.name;
			phrq_ptr->error_msg(oss.str().c_str(), CONTINUE);
			return;
		}
		for (size_t j = 0; j < Logk::MAX_LOG_K_INDICES; j++)
		{
			//lk[j] += next_Logk.logk_selected[j] * coef_add;
			lk[j] += it->second.logk_selected[j] * coef_add;
		}
		add_logks(lk, it->second, coef_add, ++repeats, phrq_ptr);
	}
}
