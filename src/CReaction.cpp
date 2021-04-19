#include "CReaction.h"
//#include "Species.h"
#include "Element.h"

//double CReaction::
//Calc_delta_v(bool phase)
///* ---------------------------------------------------------------------- */
//{
//	/* calculate Logk::delta_v from molar volumes */
//	double d_v = 0.0;
//
//	if (phase)
//	{
//		/* for phases: reactants have coef's < 0, products have coef's > 0, v.v. for species */
//		for (size_t i = 1; this->Get_tokens()[i].Get_s(); i++)
//		{
//			if (!this->Get_tokens()[i].Get_s())
//				continue;
//			d_v += this->Get_tokens()[i].coef * this->Get_tokens()[i].Get_s()->logk[Logk::Logk::vm_tc];
//		}
//	}
//	else
//	{
//		for (size_t i = 0; this->token[i].Get_name().size() > 0; i++)
//		{
//			if (!this->Get_tokens()[i].Get_s())
//				continue;
//			d_v -= this->Get_tokens()[i].coef * this->Get_tokens()[i].Get_s()->logk[Logk::Logk::vm_tc];
//		}
//	}
//	this->logk_original[Logk::Logk::delta_v] = d_v;
//	this->logk_selected[Logk::Logk::delta_v] = d_v;
//	return d_v;
//}


