#include "CReaction.h"
#include "Species.h"
#include "Element.h"
#include "Phreeqc.h"
void CReaction::Initialize(bool is_phase)
{
	dz.clear();
	dz.resize(3, 0.0);
	tokens.clear();
	phase = is_phase;
	iap = 0;
	iap_la = 0;
	lk = 0;
	si = 0;
	Logk_cr.Initialize();
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
		for (size_t i = 1; this->tokens[i].Get_s(); i++)
		{
			if (this->tokens[i].Get_s() == NULL)
				continue;
			d_v += this->tokens[i].coef * this->tokens[i].Get_s()->rxn.Get_logk_original()[Logk::vm_tc];
		}
		d_v = d_v - this->Get_logk_x()[Logk::vm0];
		this->Get_logk_x()[Logk::delta_v] = d_v;
		this->Get_logk_original()[Logk::delta_v] = d_v;
	}
	else
	{
		for (size_t i = 0; this->tokens[i].Get_name().size() > 0; i++)
		{
			if (this->tokens[i].Get_s() == NULL)
				continue;
			d_v -= this->tokens[i].Get_s()->Get_rxn().Get_logk_original()[Logk::vm_tc]
				* this->tokens[i].coef;
		}
		this->Get_logk_x()[Logk::delta_v] = d_v;
		//this->logk_original[Logk::delta_v] = d_v;
	}
	return d_v;
}

double CReaction::
Calc_iap_la()
{
	this->iap_la = 0.0;
	for (rxn_token* rxn_ptr = &this->tokens[0] + 1;
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
	for (rxn_token* rxn_ptr = &this->tokens[0] + 1;
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
Calc_si_la(double tempk, double presPa)
{
	this->iap_la = Calc_iap_la();
	this->lk = Calc_dv_lk_x(tempk, presPa);
	this->si_la = this->iap_la - this->lk;
	return this->si_la;
}
double CReaction::
Calc_iap_dv_lk_si(double tempk, double presPa, double& iap_out, double& lk_out)
{
	iap_out = Calc_iap();
	lk_out = Calc_dv_lk_x(tempk, presPa);
	si = this->iap - this->lk;
	return this->si;
}
double CReaction::
Calc_dv_lk_x(double tempk, double presPa)
{
	Calc_delta_v();
	this->lk = Calc_lk_x(tempk, presPa);
	return lk;
}
double CReaction::
Calc_lk_x(double tempk, double presPa)
{
	this->lk = Logk_cr.Calc_lk_x(tempk, presPa);
	return lk;
}
double CReaction::
Calc_si_iap_only()
{
	double si_local = -this->lk;
	for (rxn_token* rxn_ptr = &this->tokens[0] + 1;
		!rxn_ptr->Get_end(); rxn_ptr++)
	{
		si_local += rxn_ptr->Get_s()->la * rxn_ptr->coef;
	}
	return si_local;
}
