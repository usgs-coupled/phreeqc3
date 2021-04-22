#include "CReaction.h"
#include "Species.h"
#include "Element.h"
#include "Phreeqc.h"
void CReaction::Initialize(bool is_phase)
{
	logk_cr.clear();
	logk_cr.resize(Logk::MAX_LOG_K_INDICES, 0.0);
	dz.clear();
	dz.resize(3, 0.0);
	token.clear();
	phase = is_phase;
	tc_last = -99;
	p_atm_last = -99;
	mu_last = -99;
}
double CReaction::
Calc_delta_v()
/* ---------------------------------------------------------------------- */
{
	/* calculate Logk::delta_v from molar volumes */
	double d_v = 0.0;
	if (this->phase)
	{
		/* for phases: reactants have coef's < 0, products have coef's > 0, v.v. for species */
		for (size_t i = 1; this->token[i].Get_s(); i++)
		{
			if (this->token[i].Get_s() == NULL)
				continue;
			d_v += this->token[i].coef * this->token[i].Get_s()->logk[Logk::vm_tc];
		}
		d_v = d_v - this->logk_x[Logk::vm0];
		this->logk_x[Logk::delta_v] = d_v;
	}
	else
	{
		for (size_t i = 0; this->token[i].Get_name().size() > 0; i++)
		{
			if (this->token[i].Get_s() == NULL)
				continue;
			d_v -= this->token[i].Get_s()->Get_rxn().Get_logk_original()[Logk::vm_tc]
				* this->token[i].coef;
		}
	}
	return d_v;
}

double CReaction::
Calc_iap()
{
	this->iap = 0.0;
	for (rxn_token* rxn_ptr = &this->token[0] + 1;
		!rxn_ptr->Get_end(); rxn_ptr++)
	{
		if (rxn_ptr->Get_s()->name != "e-")
		{
			this->iap += (rxn_ptr->Get_s()->lm + rxn_ptr->Get_s()->lg) * rxn_ptr->coef;
		}
		else
		{
			this->iap += rxn_ptr->Get_s()->la * rxn_ptr->coef;
		}
	}
	return this->iap;
}
double CReaction::
Calc_si(double tempk, double presPa/*, Phreeqc* phrq_ptr*/)
{
	Calc_delta_v();
	this->si = Calc_iap();
	this->si -= Calc_Logk(tempk, presPa/*, phrq_ptr*/);
	return si;
}
double CReaction::
Calc_lk(double tempk, double presPa/*, Phreeqc* phrq_ptr*/)
{
	Calc_delta_v();
	return Calc_Logk(tempk, presPa/*, phrq_ptr*/);
}

void CReaction::Set_logk_cr(const std::vector<double>& k)
{
	std::vector<double> k_copy = k;
	k_copy.resize(Logk::MAX_LOG_K_INDICES);
	logk_cr = k_copy;
}
void   CReaction::Set_dz(const std::vector<double>& d)
{
	std::vector<double> d_copy;
	d_copy.resize(3);
	dz = d_copy;
}