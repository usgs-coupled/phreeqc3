#include "trxn.h"
#include "Phreeqc.h"
bool reaction_temp::
phase_rxn_to_trxn(class phase* phase_ptr, CReaction& rxn_ref)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Copy reaction from reaction structure to
	 *   temp reaction structure.
	 */
	int l;
	const char* cptr;
	double l_z;
	this->token.resize(rxn_ref.size());
	this->token[0].Set_name(phase_ptr->formula);
	/* charge */
	cptr = phase_ptr->formula.c_str();
	{
		std::string token;
		phrq_ptr->get_token(&cptr, token, &l_z, &l);
	}
	this->token[0].z = l_z;
	this->token[0].Set_s(NULL);
	/*this->token[0].coef = -1.0; */
	/* check for leading coefficient of 1.0 for phase did not work */
	this->token[0].coef = phase_ptr->rxn.token[0].coef;
	for (size_t i = 1; !rxn_ref.token[i].Get_end(); i++)
	{
		this->token[i].Set_name(rxn_ref.token[i].Get_s()->name);
		this->token[i].z = rxn_ref.token[i].Get_s()->z;
		this->token[i].Set_s(NULL);
		this->token[i].coef = rxn_ref.token[i].coef;
		count_trxn = i + 1;
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
bool reaction_temp::
trxn_add(CReaction& r_ref, double coef, bool combine)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Adds reactions together.
	 *
	 *   Global variable count_trxn determines which position in trxn is used.
	 *      If count_trxn=0, then the equation effectively is copied into this->
	 *      If count_trxn>0, then new equation is added to existing equation.
	 *
	 *   Arguments:
	 *      *r_ptr	 points to rxn structure to add.
	 *
	 *       coef	  added equation is multiplied by coef.
	 *       combine       if TRUE, reaction is reaction is sorted and
	 *		     like terms combined.
	 */
	 /*
	  *   Accumulate log k for reaction
	  */

	if (count_trxn == 0)
	{
		for (int i = 0; i < Logk::MAX_LOG_K_INDICES; i++)
		{
			this->logk[i] = r_ref.logk_x[i];
		}
		for (int i = 0; i < 3; i++)	this->dz[i] = r_ref.Get_dz()[i];
	}
	else
	{
		for (int i = 0; i < Logk::MAX_LOG_K_INDICES; i++)
		{
			this->logk[i] += coef * r_ref.logk_x[i];
		}
		for (int i = 0; i < 3; i++) this->dz[i] += coef * r_ref.Get_dz()[i];
	}
	/*
	 *   Copy  equation into work space
	 */
	class rxn_token* next_token = &r_ref.token[0];
	while (!next_token->Get_end())
	{
		if (count_trxn + 1 > this->token.size())
			this->token.resize(count_trxn + 1);
		this->token[count_trxn].Set_name(next_token->Get_s()->name);
		this->token[count_trxn].Set_s(next_token->Get_s());
		this->token[count_trxn].coef = coef * next_token->coef;
		count_trxn++;
		next_token++;
	}
	if (combine == TRUE)
		trxn_combine();
	return (OK);
}
/* ---------------------------------------------------------------------- */
bool reaction_temp::
trxn_add_phase(CReaction& r_ref, double coef, bool combine)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Adds reactions together.
	 *
	 *   Global variable count_trxn determines which position in trxn is used.
	 *      If count_trxn=0, then the equation effectively is copied into this->
	 *      If count_trxn>0, then new equation is added to existing equation.
	 *
	 *   Arguments:
	 *      *r_ptr	 points to rxn structure to add.
	 *
	 *       coef	  added equation is multiplied by coef.
	 *       combine       if TRUE, reaction is reaction is sorted and
	 *		     like terms combined.
	 */
	int i;
	class rxn_token* next_token;
	/*
	 *   Accumulate log k for reaction
	 */
	if (count_trxn == 0)
	{
		for (i = 0; i < Logk::MAX_LOG_K_INDICES; i++)
		{
			this->logk[i] = r_ref.logk_x[i];
		}
	}
	else
	{
		for (i = 0; i < Logk::MAX_LOG_K_INDICES; i++)
		{
			this->logk[i] += coef * r_ref.logk_x[i];
		}
	}
	/*
	 *   Copy  equation into work space
	 */
	next_token = &r_ref.token[0];
	while (!next_token->Get_end() || next_token->Get_name().size() != 0)
	{
		if (count_trxn + 1 > this->token.size())
			this->token.resize(count_trxn + 1);
		if (!next_token->Get_end())
		{
			this->token[count_trxn].Set_name(next_token->Get_s()->name);
			this->token[count_trxn].Set_s(next_token->Get_s());
		}
		else
		{
			this->token[count_trxn].Set_name(next_token->Get_name());
			this->token[count_trxn].Set_s(NULL);
		}
		this->token[count_trxn].coef = coef * next_token->coef;
		count_trxn++;
		next_token++;
	}
	if (combine)
		trxn_combine();
	return (OK);
}

/* ---------------------------------------------------------------------- */
int reaction_temp::
trxn_combine(void)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Combines coefficients of tokens that are equal in temporary
	 *   reaction structure, this->
	 */
	int j, k;
	/*
	 *   Sort trxn species
	 */
	trxn_sort();
	/*
	 *   Combine trxn tokens
	 */
	j = 1;
	for (k = 2; k < count_trxn; k++)
	{
		if (this->token[k].Get_s() != NULL)
		{
			if ((j > 0) && (this->token[k].Get_s() == this->token[j].Get_s()))
			{
				this->token[j].coef += this->token[k].coef;
				//if (equal(this->token[j].coef, 0.0, 1e-5))
				if (fabs(this->token[j].coef) <= 1e-5)
					j--;
			}
			else
			{
				j++;
				if (k != j)
				{
					this->token[j].Set_name(this->token[k].Get_name());
					this->token[j].Set_s(this->token[k].Get_s());
					this->token[j].coef = this->token[k].coef;
				}
			}
		}
		else
		{
			if ((j > 0) && (this->token[k].Get_s() == this->token[j].Get_s())
				&& (this->token[k].Get_name() == this->token[j].Get_name()))
			{
				this->token[j].coef += this->token[k].coef;
				//if (equal(this->token[j].coef, 0.0, 1e-5))
				if (fabs(this->token[j].coef) <= 1e-5)
					j--;
			}
			else
			{
				j++;
				if (k != j)
				{
					this->token[j].Set_name(this->token[k].Get_name());
					this->token[j].Set_s(this->token[k].Get_s());
					this->token[j].coef = this->token[k].coef;
				}
			}
		}
	}
	count_trxn = j + 1;			/* number excluding final NULL */
	return (OK);
}
/* ---------------------------------------------------------------------- */
int reaction_temp::
trxn_compare(const void* ptr1, const void* ptr2)
/* ---------------------------------------------------------------------- */
{
	const class rxn_token_temp* rxn_token_temp_ptr1, * rxn_token_temp_ptr2;
	rxn_token_temp_ptr1 = (const class rxn_token_temp*)ptr1;
	rxn_token_temp_ptr2 = (const class rxn_token_temp*)ptr2;
	return (strcmp(rxn_token_temp_ptr1->Get_name().c_str(),
		rxn_token_temp_ptr2->Get_name().c_str()));
}
/* ---------------------------------------------------------------------- */
bool reaction_temp::
trxn_copy(CReaction& rxn_ref)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Copies trxn to a reaction structure.
	 *
	 *   Input: rxn_ptr, pointer to reaction structure to copy trxn to.
	 *
	 */
	int i;

	/*
	 *   Copy logk data
	 */
	for (i = 0; i < Logk::MAX_LOG_K_INDICES; i++)
	{
			rxn_ref.logk_x[i] = this->logk[i];
			rxn_ref.logk_cr[i] = this->logk[i];
	}
	/*
	 *   Copy dz data
	 */
	for (i = 0; i < 3; i++)
	{
		rxn_ref.dz[i] = this->dz[i];
	}
	/*
	 *   Copy tokens
	 */
	rxn_ref.Get_tokens().resize(count_trxn + 1);
	for (size_t i = 0; i < count_trxn; i++)
	{
		rxn_ref.Get_tokens()[i].Set_s(this->token[i].Get_s());
		rxn_ref.Get_tokens()[i].Set_name(this->token[i].Get_name());
		rxn_ref.Get_tokens()[i].coef = this->token[i].coef;
	}
	rxn_ref.token[count_trxn].Set_s(NULL);
	rxn_ref.token[count_trxn].Set_name("");
	rxn_ref.token[count_trxn].Set_end(true);
	return (OK);
}
/* ---------------------------------------------------------------------- */
bool reaction_temp::
trxn_copy(CReaction& rxn_ref, std::vector<double>& target_logk)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Copies trxn to a reaction structure.
	 *
	 *   Input: rxn_ptr, pointer to reaction structure to copy trxn to.
	 *
	 */
	int i;
	/*
	 *   Copy logk data
	 */
	target_logk.resize(Logk::MAX_LOG_K_INDICES);
	for (i = 0; i < Logk::MAX_LOG_K_INDICES; i++)
	{
		target_logk[i] = this->logk[i];
	}
	/*
	 *   Copy dz data
	 */
	rxn_ref.dz.resize(3);
	for (i = 0; i < 3; i++)
	{
		rxn_ref.dz[i] = this->dz[i];
	}
	/*
	 *   Copy tokens
	 */
	rxn_ref.Get_tokens().resize(count_trxn + 1);
	for (size_t i = 0; i < count_trxn; i++)
	{
		rxn_ref.Get_tokens()[i].Set_s(this->token[i].Get_s());
		rxn_ref.Get_tokens()[i].Set_name(this->token[i].Get_name());
		rxn_ref.Get_tokens()[i].coef = this->token[i].coef;
	}
	rxn_ref.token[count_trxn].Set_s(NULL);
	rxn_ref.token[count_trxn].Set_name("");
	rxn_ref.token[count_trxn].Set_end(true);
	return (OK);
}
/* ---------------------------------------------------------------------- */
double reaction_temp::
trxn_find_coef(const char* str, int start)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Finds coefficient of specified token in this->
	 *   Input: str, token name in reaction.
	 *
	 *   Return: 0.0, if token not found.
	 *	   coefficient of token, if token found.
	 */
	int i;
	double coef;

	coef = 0.0;
	for (i = start; i < count_trxn; i++)
	{
		if (strcmp(this->token[i].Get_s()->name.c_str(), str) == 0)
		{
			coef = this->token[i].coef;
			break;
		}
	}
	return (coef);
}

/* ---------------------------------------------------------------------- */
int reaction_temp::
trxn_multiply(double coef)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Multiplies temporary reaction, trxn,  by a constant
	 *
	 *   Arguments:
	 *       input: coef	  multiplier.
	 */
	int i;
	/*
	 *   Multiply log k for reaction
	 */

	for (i = 0; i < Logk::MAX_LOG_K_INDICES; i++)
	{
		this->logk[i] *= coef;
	}
	/*
	 *   Multiply dz for reaction
	 */
	for (i = 0; i < 3; i++)
	{
		this->dz[i] *= coef;
	}
	/*
	 *   Multiply coefficients of reaction
	 */
	for (i = 0; i < count_trxn; i++)
	{
		this->token[i].coef *= coef;
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int reaction_temp::
trxn_print(void)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Prints trxn
	 */
	int i;
	/*
	 *   Print log k for reaction
	 */
	std::ostringstream oss;
	oss << "\tlog k data:\n";
	//output_msg(sformatf("\tlog k data:\n"));
	for (i = 0; i < Logk::MAX_LOG_K_INDICES; i++)
	{
		oss << "\t\t" << this->logk[i] << "\n";
		//output_msg(sformatf("\t\t%f\n", (double)this->logk[i]));
	}

	/*
	 *   Print dz for reaction
	 */
	oss << "\tdz data:\n";
	//output_msg(sformatf("\tdz data:\n"));
	for (i = 0; i < 3; i++)
	{
		oss << "\t\t" << this->dz[i] << "\n";
		//output_msg(sformatf("\t\t%f\n", (double)this->dz[i]));
	}
	/*
	 *   Print stoichiometry
	 */
	oss << "\tReaction stoichiometry\n";
	//output_msg(sformatf("\tReaction stoichiometry\n"));
	for (i = 0; i < count_trxn; i++)
	{
		oss << "\t\t" << this->token[i].Get_name() << "\t" << this->token[i].coef << "\n";
		//output_msg(sformatf("\t\t%-20s\t%10.2f\n", this->token[i].Get_name().c_str(),
		//	(double)this->token[i].coef));
	}
	oss << "\n";
	phrq_ptr->output_msg(oss.str().c_str());
	//output_msg(sformatf("\n"));
	return (OK);
}

/* ---------------------------------------------------------------------- */
int reaction_temp::
trxn_reverse_k(void)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Changes K from dissociation to association and back
	 */
	int i;
	/*
	 *   Accumulate log k for reaction
	 */
	for (i = 0; i < Logk::MAX_LOG_K_INDICES; i++)
	{
		this->logk[i] = -this->logk[i];
	}
	for (i = 0; i < 3; i++)
	{
		this->dz[i] = -this->dz[i];
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int reaction_temp::
trxn_sort(void)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Compare names in tokens in trxn array for sorting
	 */
	if (count_trxn - 1 > 1)
	{
		std::sort(this->token.begin() + 1, this->token.begin() + count_trxn);
	}
	return (OK);
}

/* ---------------------------------------------------------------------- */
int reaction_temp::
trxn_swap(const char* token)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Moves specified token to initial position in reaction.
	 *   Input: token, token name to move to initial position.
	 *
	 *   Return: ERROR, if token not found.
	 *	   OK, if token moved to initial position.
	 */
	int i, j;
	double coef;
	/*
	 *   Locate token
	 */
	for (j = 0; j < count_trxn; j++)
	{
		if (strcmp(this->token[j].Get_s()->name.c_str(), token) == 0)
			break;
	}
	if (j >= count_trxn)
	{
		std::ostringstream oss;
		oss << "Could not find token in equation " << token << "." << std::endl;

		for (i = 0; i < count_trxn; i++)
		{
			oss << this->token[i].coef << "\t" << this->token[i].Get_name() << "\n";
		}
		phrq_ptr->error_msg(oss.str().c_str(), CONTINUE);
		return (ERROR);
	}
	/*
	 *   Swap token to first position
	 */
	this->token[count_trxn].Set_name(this->token[0].Get_name());
	this->token[count_trxn].Set_s(this->token[0].Get_s());
	this->token[count_trxn].coef = this->token[0].coef;

	this->token[0].Set_name(this->token[j].Get_name());
	this->token[0].Set_s(this->token[j].Get_s());
	this->token[0].coef = this->token[j].coef;

	this->token[j].Set_name(this->token[count_trxn].Get_name());
	this->token[j].Set_s(this->token[count_trxn].Get_s());
	this->token[j].coef = this->token[count_trxn].coef;
	/*
	 *   Make coefficient of token -1.0
	 */
	coef = -1.0 / this->token[0].coef;
	trxn_multiply(coef);
	return (OK);
}
/* ---------------------------------------------------------------------- */
int reaction_temp::
rewrite_master_to_secondary(class master* master_ptr1,
	class master* master_ptr2)
	/* ---------------------------------------------------------------------- */
{
	/*
	 *   Write equation for secondary master species in terms of another secondary master species
	 *   Store result in rxn_secondary of master_ptr.
	 */
	LDBLE coef1, coef2;
	class master* master_ptr_p1, * master_ptr_p2;
	/*
	 *   Check that the two master species have the same primary master species
	 */
	master_ptr_p1 = master_ptr1->elt->primary;
	master_ptr_p2 = master_ptr2->elt->primary;
	if (master_ptr_p1 != master_ptr_p2 || master_ptr_p1 == NULL)
	{
		std::ostringstream oss;
		oss << "All redox states must be for the same element. " <<
			master_ptr1->elt->name << "\t" << master_ptr2->elt->name << ".\n";
		//error_string = sformatf(
		//	"All redox states must be for the same element. %s\t%s.",
		//	master_ptr1->elt->name.c_str(), master_ptr2->elt->name.c_str());
		phrq_ptr->error_msg(oss.str().c_str(), CONTINUE);
		//input_error++;
		return (ERROR);
	}
	/*
	 *   Find coefficient of primary master in reaction
	 */
	coef1 = phrq_ptr->rxn_find_coef(master_ptr1->rxn_primary, master_ptr_p1->s->name.c_str());
	coef2 = phrq_ptr->rxn_find_coef(master_ptr2->rxn_primary, master_ptr_p1->s->name.c_str());
	if (phrq_ptr->equal(coef1, 0.0, TOL) == TRUE || phrq_ptr->equal(coef2, 0.0, TOL) == TRUE)
	{
		std::ostringstream oss;
		oss << "One of these equations does not contain master species for element, "
			<< master_ptr1->s->name << " or " << master_ptr2->s->name << ".\n";
		phrq_ptr->error_msg(oss.str().c_str(), CONTINUE);
		//error_string = sformatf(
		//	"One of these equations does not contain master species for element, %s or %s.",
		//	master_ptr1->s->name, master_ptr2->s->name);
		//error_msg(error_string, CONTINUE);
		//input_error++;
		return (ERROR);
	}
	/*
	 *   Rewrite equation to secondary master species
	 */
	this->Set_count_trxn(0);
	bool x_on_save = x_on;
	x_on = false;
	this->trxn_add(master_ptr1->rxn_primary, 1.0, false);
	this->trxn_add(master_ptr2->rxn_primary, -coef1 / coef2, true);
	x_on = x_on_save;
	return (OK);
}
/* ---------------------------------------------------------------------- */
int reaction_temp::
species_rxn_to_trxn(class species* s_ptr)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Copy reaction from reaction structure to
	 *   temp reaction structure.
	 */
	if (this->token.size() <= s_ptr->rxn.token.size())
	{
		this->token.resize(s_ptr->rxn.token.size());
	}
	count_trxn = 0;;
	for (size_t i = 0; !s_ptr->rxn.token[i].Get_end(); i++)
	{
		this->token[i].Set_name(s_ptr->rxn.token[i].Get_s()->name);
		this->token[i].z = s_ptr->rxn.token[i].Get_s()->z;
		this->token[i].Set_s(s_ptr->rxn.token[i].Get_s());
		this->token[i].coef = s_ptr->rxn.token[i].coef;
		count_trxn = i + 1;
		if (count_trxn + 1 > this->token.size())
			this->token.resize(count_trxn + 1);
	}
	return (OK);
}