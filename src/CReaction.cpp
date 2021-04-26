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
	iap = 0;
	iap_la = 0;
	lk = 0;
	si = 0;
	Logk::Initialize();
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
			d_v += this->token[i].coef * this->token[i].Get_s()->rxn.logk_original[Logk::vm_tc];
		}
		d_v = d_v - this->logk_x[Logk::vm0];
		this->logk_x[Logk::delta_v] = d_v;
		this->logk_original[Logk::delta_v] = d_v;
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
		this->logk_x[Logk::delta_v] = d_v;
		//this->logk_original[Logk::delta_v] = d_v;
	}
	return d_v;
}

double CReaction::
Calc_iap_la()
{
	this->iap_la = 0.0;
	for (rxn_token* rxn_ptr = &this->token[0] + 1;
		!rxn_ptr->Get_end(); rxn_ptr++)
	{
		this->iap_la += rxn_ptr->Get_s()->la * rxn_ptr->coef;
	}
	return this->iap_la;
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
Calc_si(double tempk, double presPa)
{
	this->iap = Calc_iap();
	this->lk = Calc_lk(tempk, presPa);
	this->si = this->iap - this->lk;
	return this->si;
}
double CReaction::
Calc_si_la(double tempk, double presPa)
{
	this->iap_la = Calc_iap_la();
	this->lk = Calc_lk(tempk, presPa);
	this->si_la = this->iap_la - this->lk;
	return this->si_la;
}
double CReaction::
Calc_si(double tempk, double presPa, double& iap_out, double& lk_out)
{
	iap_out = Calc_iap();
	lk_out = Calc_lk(tempk, presPa);
	si = this->iap - this->lk;
	return this->si;
}
double CReaction::
Calc_lk(double tempk, double presPa)
{
	double d_v = Calc_delta_v();
	this->lk = Calc_Logk(tempk, presPa);
	return lk;
}

void CReaction::Set_logk_cr(const std::vector<double>& k)
{
	if (k.size() != Logk::MAX_LOG_K_INDICES)
	{
		std::vector<double> k_copy = k;
		k_copy.resize(Logk::MAX_LOG_K_INDICES);
		logk_cr = k_copy;
	}
	else
	{
		logk_cr = k;
	}
}
void   CReaction::Set_dz(const std::vector<double>& d)
{
	if (d.size() != 3)
	{
		std::vector<double> d_copy;
		d_copy.resize(3);
		dz = d_copy;
	}
	else
	{
		dz = d;
	}
}