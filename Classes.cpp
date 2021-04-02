#include "Phreeqc.h"
#include "CReaction.h"
/* ---------------------------------------------------------------------- */
double Phreeqc::
calc_delta_v(CReaction& r_ref, bool phase)
/* ---------------------------------------------------------------------- */
{
	/* calculate delta_v from molar volumes */
	double d_v = 0.0;
	if (phase)
	{
		/* for phases: reactants have coef's < 0, products have coef's > 0, v.v. for species */
		for (size_t i = 1; r_ref.Get_tokens()[i].s; i++)
		{
			if (!r_ref.Get_tokens()[i].s)
				continue;
			d_v += r_ref.Get_tokens()[i].coef * r_ref.Get_tokens()[i].s->logk[vm_tc];
		}
	}
	else
	{
		for (size_t i = 0; r_ref.token[i].name /*|| r_ptr->token[i].s*/; i++)
		{
			if (!r_ref.Get_tokens()[i].s)
				continue;
			d_v -= r_ref.Get_tokens()[i].coef * r_ref.Get_tokens()[i].s->logk[vm_tc];
		}
	}
	return d_v;
}
/* ---------------------------------------------------------------------- */
bool Phreeqc::
trxn_add(CReaction& r_ref, double coef, bool combine)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Adds reactions together.
	 *
	 *   Global variable count_trxn determines which position in trxn is used.
	 *      If count_trxn=0, then the equation effectively is copied into trxn.
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
		for (int i = 0; i < MAX_LOG_K_INDICES; i++) trxn.logk[i] = r_ref.Get_logk()[i];
		for (int i = 0; i < 3; i++)	trxn.dz[i] = r_ref.Get_dz()[i];
	}
	else
	{
		for (int i = 0; i < MAX_LOG_K_INDICES; i++) trxn.logk[i] += coef * r_ref.Get_logk()[i];
		for (int i = 0; i < 3; i++) trxn.dz[i] += coef * r_ref.Get_dz()[i];
	}
	/*
	 *   Copy  equation into work space
	 */
	struct rxn_token* next_token = &r_ref.token[0];
	while (next_token->s != NULL)
	{
		if (count_trxn + 1 > trxn.token.size())
			trxn.token.resize(count_trxn + 1);
		trxn.token[count_trxn].name = next_token->s->name;
		trxn.token[count_trxn].s = next_token->s;
		trxn.token[count_trxn].coef = coef * next_token->coef;
		count_trxn++;
		next_token++;
	}
	if (combine == TRUE)
		trxn_combine();
	return (OK);
}


/* ---------------------------------------------------------------------- */
bool Phreeqc::
trxn_add_phase(CReaction& r_ref, double coef, bool combine)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Adds reactions together.
	 *
	 *   Global variable count_trxn determines which position in trxn is used.
	 *      If count_trxn=0, then the equation effectively is copied into trxn.
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
	struct rxn_token* next_token;
	/*
	 *   Accumulate log k for reaction
	 */
	if (count_trxn == 0)
	{
		memcpy((void*)trxn.logk, (void*)r_ref.Get_logk(),
			(size_t)MAX_LOG_K_INDICES * sizeof(double));
	}
	else
	{
		for (i = 0; i < MAX_LOG_K_INDICES; i++)	trxn.logk[i] += coef * r_ref.Get_logk()[i];
	}
	/*
	 *   Copy  equation into work space
	 */
	next_token = &r_ref.token[0];
	while (next_token->s != NULL || next_token->name != NULL)
	{
		if (count_trxn + 1 > trxn.token.size())
			trxn.token.resize(count_trxn + 1);
		if (next_token->s != NULL)
		{
			trxn.token[count_trxn].name = next_token->s->name;
			trxn.token[count_trxn].s = next_token->s;
		}
		else
		{
			trxn.token[count_trxn].name = next_token->name;
			trxn.token[count_trxn].s = NULL;
		}
		trxn.token[count_trxn].coef = coef * next_token->coef;
		count_trxn++;
		next_token++;
	}
	if (combine)
		trxn_combine();
	return (OK);
}

/* ---------------------------------------------------------------------- */
bool Phreeqc::
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
	for (i = 0; i < MAX_LOG_K_INDICES; i++)
	{
		rxn_ref.logk[i] = trxn.logk[i];
	}
	/*
	 *   Copy dz data
	 */
	for (i = 0; i < 3; i++)
	{
		rxn_ref.dz[i] = trxn.dz[i];
	}
	/*
	 *   Copy tokens
	 */
	rxn_ref.Get_tokens().resize(count_trxn + 1);
	for (size_t i = 0; i < count_trxn; i++)
	{
		rxn_ref.Get_tokens()[i].s = trxn.token[i].s;
		rxn_ref.Get_tokens()[i].name = trxn.token[i].name;
		rxn_ref.Get_tokens()[i].coef = trxn.token[i].coef;
	}
	rxn_ref.token[count_trxn].s = NULL;
	rxn_ref.token[count_trxn].name = NULL;
	return (OK);
}
/* ---------------------------------------------------------------------- */
bool Phreeqc::
phase_rxn_to_trxn(struct phase* phase_ptr, CReaction& rxn_ref)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Copy reaction from reaction structure to
	 *   temp reaction structure.
	 */
	int l;
	const char* cptr;
	LDBLE l_z;
	trxn.token.resize(rxn_ref.size());
	trxn.token[0].name = phase_ptr->formula;
	/* charge */
	cptr = phase_ptr->formula;
	{
		std::string token;
		get_token(&cptr, token, &l_z, &l);
	}
	trxn.token[0].z = l_z;
	trxn.token[0].s = NULL;
	trxn.token[0].unknown = NULL;
	/*trxn.token[0].coef = -1.0; */
	/* check for leading coefficient of 1.0 for phase did not work */
	trxn.token[0].coef = phase_ptr->rxn.token[0].coef;
	for (size_t i = 1; rxn_ref.token[i].s != NULL; i++)
	{
		trxn.token[i].name = rxn_ref.token[i].s->name;
		trxn.token[i].z = rxn_ref.token[i].s->z;
		trxn.token[i].s = NULL;
		trxn.token[i].unknown = NULL;
		trxn.token[i].coef = rxn_ref.token[i].coef;
		count_trxn = i + 1;
	}
	return (OK);
}
/* ---------------------------------------------------------------------- */
double Phreeqc::
rxn_find_coef(CReaction& r_ref, const char* str)
/* ---------------------------------------------------------------------- */
{
	/*
	 *   Finds coefficient of token in reaction.
	 *   input: r_ptr, pointer to a reaction structure
	 *	  str, string to find as reaction token
	 *
	 *   Return: 0.0, if token not found
	 *	   coefficient of token, if found.
	 */
	struct rxn_token* r_token;
	LDBLE coef;

	r_token = &r_ref.token[1];
	coef = 0.0;
	while (r_token->s != NULL)
	{
		if (strcmp(r_token->s->name, str) == 0)
		{
			coef = r_token->coef;
			break;
		}
		r_token++;
	}
	return (coef);
}
/* ---------------------------------------------------------------------- */
double Phreeqc::
calc_alk(CReaction& rxn_ref)
/* ---------------------------------------------------------------------- */
{
	LDBLE return_value;
	struct master* master_ptr;

	return_value = 0.0;
	struct rxn_token* r_token = &rxn_ref.token[1];
	while (r_token->s != NULL)
	{
		master_ptr = r_token->s->secondary;
		if (master_ptr == NULL)
		{
			master_ptr = r_token->s->primary;
		}
		if (master_ptr == NULL)
		{
			error_string = sformatf(
				"Non-master species in secondary reaction, %s.",
				rxn_ref.token[0].s->name);
			error_msg(error_string, CONTINUE);
			input_error++;
			break;
		}
		return_value += r_token->coef * master_ptr->alk;
		r_token++;
	}
	return (return_value);
}
CReaction Phreeqc::CReaction_internal_copy(CReaction& rxn_ref)
{
	CReaction rxn;
	for (size_t i = 0; i < MAX_LOG_K_INDICES; i++) rxn.logk[i] = rxn_ref.logk[i];
	for (size_t i = 0; i < 3; i++) rxn.dz[i] = rxn_ref.dz[i];
	rxn.Get_tokens().resize(rxn_ref.Get_tokens().size());
	for (size_t i = 0; i < rxn_ref.Get_tokens().size(); i++)
	{
		rxn.token[i].s = (rxn_ref.token[i].s == NULL) ? NULL :
			s_search(rxn_ref.token[i].s->name);
		rxn.token[i].coef = rxn_ref.token[i].coef;
		rxn.token[i].name = (rxn_ref.token[i].s == NULL) ? NULL :
			string_hsave(rxn_ref.token[i].name);
	}
	return rxn;
}
	
