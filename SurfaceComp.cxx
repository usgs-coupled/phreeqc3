// SurfaceComp.cxx: implementation of the cxxSurfaceComp class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include <cassert>				// assert
#include <algorithm>			// std::sort

#include "Utils.h"				// define first
#if !defined(PHREEQC_CLASS)
#define EXTERNAL extern
#include "global.h"
#else
#include "Phreeqc.h"
#endif
#include "SurfaceComp.h"
#include "Dictionary.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include "output.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxSurfaceComp::cxxSurfaceComp(PHRQ_io *io)
:
PHRQ_base(io)
//
// default constructor for cxxSurfaceComp 
//
{
	formula_totals.type = cxxNameDouble::ND_ELT_MOLES;
	formula_z = 0.0;
	moles = 0.0;
	totals.type = cxxNameDouble::ND_ELT_MOLES;
	la = 0.0;
	//charge_number           = -99;
	charge_balance = 0;
	phase_proportion = 0.0;
	Dw = 0.0;
}

cxxSurfaceComp::cxxSurfaceComp(struct surface_comp *surf_comp_ptr, PHRQ_io *io)
//
// constructor for cxxSurfaceComp from struct surface_comp
//
:
PHRQ_base(io),
formula_totals(surf_comp_ptr->formula_totals),
totals(surf_comp_ptr->totals)
{
	this->set_formula(surf_comp_ptr->formula);
	formula_z = surf_comp_ptr->formula_z;
	moles = surf_comp_ptr->moles;
	la = surf_comp_ptr->la;
	//charge_number            = surf_comp_ptr->charge;
	charge_balance = surf_comp_ptr->cb;
	this->set_phase_name(surf_comp_ptr->phase_name);
	phase_proportion = surf_comp_ptr->phase_proportion;
	this->set_rate_name(surf_comp_ptr->rate_name);
	Dw = surf_comp_ptr->Dw;
}

cxxSurfaceComp::~cxxSurfaceComp()
{
}

struct master *
	cxxSurfaceComp::get_master(PHREEQC_PTR_ARG)
{
	struct master *master_ptr = NULL;
	//for (std::map <char *, double, CHARSTAR_LESS>::iterator it = totals.begin(); it != totals.end(); it++) {
	for (cxxNameDouble::iterator it = this->totals.begin();
		it != this->totals.end(); it++)
	{
		/* Find master species */
		char *eltName = P_INSTANCE_POINTER string_hsave(it->first.c_str());
		assert(it->first.size() > 0);
		struct element *elt_ptr = P_INSTANCE_POINTER element_store(eltName);
		if (elt_ptr->master == NULL)
		{
			std::ostringstream error_oss;
			error_oss << "Master species not in data base for " << elt_ptr->
				name << std::endl;
			//Utilities::error_msg(error_oss.str(), CONTINUE);
			P_INSTANCE_POINTER error_msg(error_oss.str().c_str(), CONTINUE);
			return (NULL);
		}
		if (elt_ptr->master->type != SURF)
			continue;
		master_ptr = elt_ptr->master;
		break;
	}
	if (master_ptr == NULL)
	{
		std::ostringstream error_oss;
		error_oss <<
			"Surface formula does not contain an surface master species, " <<
			this->formula << std::endl;
		//Utilities::error_msg(error_oss.str(), CONTINUE);
		P_INSTANCE_POINTER error_msg(error_oss.str().c_str(), CONTINUE);
	}
	return (master_ptr);
}

struct surface_comp *
	cxxSurfaceComp::cxxSurfaceComp2surface_comp(PHREEQC_PTR_ARG_COMMA std::map < std::string, cxxSurfaceComp > &el)
	//
	// Builds surface_comp structure from of cxxSurfaceComp 
	//
{
	struct surface_comp *surf_comp_ptr =
		(struct surface_comp *)
		P_INSTANCE_POINTER PHRQ_malloc((size_t) (el.size() * sizeof(struct surface_comp)));
	if (surf_comp_ptr == NULL)
		P_INSTANCE_POINTER malloc_error();

	int i = 0;
	for (std::map < std::string, cxxSurfaceComp >::iterator it = el.begin();
		it != el.end(); ++it)
	{
		surf_comp_ptr[i].formula = P_INSTANCE_POINTER string_hsave((*it).second.formula.c_str());
		assert((*it).second.formula.size() > 0);
		surf_comp_ptr[i].formula_totals = (*it).second.formula_totals.elt_list(P_INSTANCE);
		surf_comp_ptr[i].formula_z = (*it).second.formula_z;
		surf_comp_ptr[i].moles = (*it).second.moles;
		surf_comp_ptr[i].master = (*it).second.get_master(P_INSTANCE);
		surf_comp_ptr[i].totals = (*it).second.totals.elt_list(P_INSTANCE);
		surf_comp_ptr[i].la = (*it).second.la;
		//surf_comp_ptr[i].charge                 =  it->charge_number;
		surf_comp_ptr[i].cb = (*it).second.charge_balance;
		if ((*it).second.phase_name.size() == 0)
			surf_comp_ptr[i].phase_name = NULL;
		else
			surf_comp_ptr[i].phase_name = P_INSTANCE_POINTER string_hsave((*it).second.phase_name.c_str());
		surf_comp_ptr[i].phase_proportion = (*it).second.phase_proportion;
		if ((*it).second.rate_name.size() == 0)
			surf_comp_ptr[i].rate_name = NULL;
		else
			surf_comp_ptr[i].rate_name = P_INSTANCE_POINTER string_hsave((*it).second.rate_name.c_str());
		surf_comp_ptr[i].Dw = (*it).second.Dw;
		surf_comp_ptr[i].master = (*it).second.get_master(P_INSTANCE);
		i++;
	}
	return (surf_comp_ptr);
}

void
cxxSurfaceComp::dump_xml(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing surf_comp message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// Surf_Comp element and attributes

	s_oss << indent0 << "formula=\"" << this->formula << "\"" << std::endl;
	s_oss << indent0 << "formula_z=\"" << this->
		formula_z << "\"" << std::endl;
	s_oss << indent0 << "moles=\"" << this->moles << "\"" << std::endl;
	s_oss << indent0 << "la=\"" << this->la << "\"" << std::endl;
	//s_oss << indent0 << "charge_number=\"" << this->charge_number   << "\"" << std::endl;
	s_oss << indent0 << "charge_balance=\"" << this->
		charge_balance << "\"" << std::endl;
	if (this->phase_name.size() != 0)
	{
		s_oss << indent0 << "phase_name=\"" << this->
			phase_name << "\"" << std::endl;
	}
	if (this->rate_name.size() != 0)
	{
		s_oss << indent0 << "rate_name=\"" << this->
			rate_name << "\"" << std::endl;
	}
	s_oss << indent0 << "phase_proportion=\"" << this->
		phase_proportion << "\"" << std::endl;
	s_oss << indent0 << "Dw=\"" << this->Dw << "\"" << std::endl;

	// formula_totals
	s_oss << indent0;
	s_oss << "<formula_totals " << std::endl;
	this->formula_totals.dump_xml(s_oss, indent + 1);

	// totals
	s_oss << indent0;
	s_oss << "<totals " << std::endl;
	this->totals.dump_xml(s_oss, indent + 1);

}

void
cxxSurfaceComp::dump_raw(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing surf_comp message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// Surf_Comp element and attributes

	s_oss << indent0 << "-formula               " << this->
		formula << std::endl;
	s_oss << indent1 << "-formula_z             " << this->
		formula_z << std::endl;
	s_oss << indent1 << "-moles                 " << this->moles << std::endl;
	s_oss << indent1 << "-la                    " << this->la << std::endl;
	//s_oss << indent1 << "-charge_number         " << this->charge_number  << std::endl;
	s_oss << indent1 << "-charge_balance        " << this->
		charge_balance << std::endl;
	if (this->phase_name.size() != 0)
	{
		s_oss << indent1 << "-phase_name            " << this->
			phase_name << std::endl;
	}
	if (this->rate_name.size() != 0)
	{
		s_oss << indent1 << "-rate_name             " << this->
			rate_name << std::endl;
	}
	s_oss << indent1 << "-phase_proportion      " << this->
		phase_proportion << std::endl;
	s_oss << indent1 << "-Dw                    " << this->Dw << std::endl;

	// formula_totals
	s_oss << indent1;
	s_oss << "-formula_totals" << std::endl;
	this->formula_totals.dump_raw(s_oss, indent + 2);

	// totals
	s_oss << indent1;
	s_oss << "-totals" << std::endl;
	this->totals.dump_raw(s_oss, indent + 2);

}

void
cxxSurfaceComp::read_raw(PHREEQC_PTR_ARG_COMMA CParser & parser, bool check)
{
	std::string str;

	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(10);
		vopts.push_back("formula");	// 0 
		vopts.push_back("moles");	// 1
		vopts.push_back("la");	// 2 
		vopts.push_back("charge_number");	// 3 
		vopts.push_back("charge_balance");	// 4
		vopts.push_back("phase_name");	// 5 
		vopts.push_back("rate_name");	// 6 
		vopts.push_back("phase_proportion");	// 7 
		vopts.push_back("totals");	// 8
		vopts.push_back("formula_z");	// 9
		vopts.push_back("formula_totals");	// 10
		vopts.push_back("dw");	// 11
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;

	opt_save = CParser::OPT_ERROR;
	bool formula_defined(false);
	bool moles_defined(false);
	bool la_defined(false);
	//bool charge_number_defined(false); 
	bool charge_balance_defined(false);
	bool formula_z_defined(false);
	bool Dw_defined(false);

	for (;;)
	{
		int opt = parser.get_option(vopts, next_char);
		if (opt == CParser::OPT_DEFAULT)
		{
			opt = opt_save;
		}

		switch (opt)
		{
		case CParser::OPT_EOF:
			break;
		case CParser::OPT_KEYWORD:
			break;
		case CParser::OPT_DEFAULT:
		case CParser::OPT_ERROR:
			opt = CParser::OPT_KEYWORD;
			// Allow return to Surface for more processing
			//parser.error_msg("Unknown input in SURF_COMP read.", CParser::OT_CONTINUE);
			//parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
			break;

		case 0:				// formula
			if (!(parser.get_iss() >> str))
			{
				this->formula.clear();
				parser.incr_input_error();
				parser.error_msg("Expected string value for formula.",
					CParser::OT_CONTINUE);
			}
			else
			{
				this->formula = str;
			}
			formula_defined = true;
			break;

		case 1:				// moles
			if (!(parser.get_iss() >> this->moles))
			{
				this->moles = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for moles.",
					CParser::OT_CONTINUE);
			}
			moles_defined = true;
			break;

		case 2:				// la
			if (!(parser.get_iss() >> this->la))
			{
				this->la = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for la.",
					CParser::OT_CONTINUE);
			}
			la_defined = true;
			break;
#ifdef SKIP
		case 3:				// charge_number
			if (!(parser.get_iss() >> this->charge_number))
			{
				this->charge_number = 0;
				parser.incr_input_error();
				parser.error_msg("Expected integer value for charge_number.",
					CParser::OT_CONTINUE);
			}
			charge_number_defined = true;
			break;
#endif
		case 4:				// charge_balance
			if (!(parser.get_iss() >> this->charge_balance))
			{
				this->charge_balance = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for charge_balance.",
					CParser::OT_CONTINUE);
			}
			charge_balance_defined = true;
			break;

		case 5:				// phase_name
			if (!(parser.get_iss() >> str))
			{
				this->phase_name.clear();
				parser.incr_input_error();
				parser.error_msg("Expected string value for phase_name.",
					CParser::OT_CONTINUE);
			}
			else
			{
				this->phase_name = str;
			}
			break;

		case 6:				// rate_name
			if (!(parser.get_iss() >> str))
			{
				this->rate_name.clear();
				parser.incr_input_error();
				parser.error_msg("Expected string value for rate_name.",
					CParser::OT_CONTINUE);
			}
			else
			{
				this->rate_name = str;
			}
			break;

		case 7:				// phase_proportion
			if (!(parser.get_iss() >> this->phase_proportion))
			{
				this->phase_proportion = 0;
				parser.incr_input_error();
				parser.
					error_msg("Expected numeric value for phase_proportion.",
					CParser::OT_CONTINUE);
			}
			break;

		case 8:				// totals
			if (this->totals.read_raw(P_INSTANCE_COMMA parser, next_char) !=
				CParser::PARSER_OK)
			{
				parser.incr_input_error();
				parser.
					error_msg
					("Expected element name and molality for SurfaceComp totals.",
					CParser::OT_CONTINUE);
			}
			opt_save = 8;
			break;

		case 9:				// formula_z
			if (!(parser.get_iss() >> this->formula_z))
			{
				this->formula_z = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for formula_z.",
					CParser::OT_CONTINUE);
			}
			formula_z_defined = true;
			break;

		case 10:				// formula_totals
			if (this->formula_totals.read_raw(P_INSTANCE_COMMA parser, next_char) !=
				CParser::PARSER_OK)
			{
				parser.incr_input_error();
				parser.
					error_msg
					("Expected element name and molality for SurfaceComp formula totals.",
					CParser::OT_CONTINUE);
			}
			opt_save = 10;
			break;

		case 11:				// Dw
			if (!(parser.get_iss() >> this->Dw))
			{
				this->Dw = 0.0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for Dw.",
					CParser::OT_CONTINUE);
			}
			Dw_defined = true;
			break;
		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	if (check)
	{
		// members that must be defined
		if (formula_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Formula not defined for SurfaceComp input.",
				CParser::OT_CONTINUE);
		}
		if (formula_z_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Formula_z not defined for ExchComp input.",
				CParser::OT_CONTINUE);
		}
		if (moles_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Moles not defined for SurfaceComp input.",
				CParser::OT_CONTINUE);
		}
		if (la_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("La not defined for SurfaceComp input.",
				CParser::OT_CONTINUE);
		}
#ifdef SKIP
		if (charge_number_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Charge_number not defined for SurfaceComp input.",
				CParser::OT_CONTINUE);
		}
#endif
		if (charge_balance_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Charge_balance not defined for SurfaceComp input.",
				CParser::OT_CONTINUE);
		}
		if (Dw_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Dw not defined for SurfaceComp input.",
				CParser::OT_CONTINUE);
		}
	}
}

#ifdef USE_MPI
void
cxxSurfaceComp::mpi_pack(std::vector < int >&ints,
						 std::vector < double >&doubles)
{
	extern cxxDictionary dictionary;

	ints.push_back(dictionary.string2int(this->formula));
	doubles.push_back(this->formula_z);
	this->formula_totals.mpi_pack(ints, doubles);
	doubles.push_back(this->moles);
	this->totals.mpi_pack(ints, doubles);
	doubles.push_back(this->la);
	//ints.push_back(charge_number);
	doubles.push_back(this->charge_balance);
	ints.push_back(dictionary.string2int(this->phase_name));
	doubles.push_back(this->phase_proportion);
	ints.push_back(dictionary.string2int(this->rate_name));
	doubles.push_back(this->Dw);
}

void
cxxSurfaceComp::mpi_unpack(int *ints, int *ii, double *doubles, int *dd)
{
	extern cxxDictionary dictionary;
	int i = *ii;
	int d = *dd;
	this->formula = dictionary.int2stdstring(ints[i++]);
	this->formula_z = doubles[d++];
	this->formula_totals.mpi_unpack(ints, &i, doubles, &d);
	this->moles = doubles[d++];
	this->totals.mpi_unpack(ints, &i, doubles, &d);
	this->la = doubles[d++];
	//this->charge_number = ints[i++];
	this->charge_balance = doubles[d++];
	this->phase_name = dictionary.int2stdstring(ints[i++]);
	this->phase_proportion = doubles[d++];
	this->rate_name = dictionary.int2stdstring(ints[i++]);
	this->Dw = doubles[d++];
	*ii = i;
	*dd = d;
}
#endif
void
cxxSurfaceComp::add(PHREEQC_PTR_ARG_COMMA const cxxSurfaceComp & addee, double extensive)
{
	if (extensive == 0.0)
		return;
	if (addee.formula.size() == 0)
		return;

	//char * formula;
	//cxxNameDouble formula_totals; 
	if (this->formula.size() == 0 && addee.formula.size() == 0)
	{
		return;
	}
	assert(this->formula == addee.formula);
	assert(this->formula_z == addee.formula_z);
	if (this->formula.size() == 0 && addee.formula.size() != 0)
	{
		this->formula = addee.formula;
		this->formula_totals = addee.formula_totals;
	}

	// this and addee must have same formula
	// otherwise generate a new exchcomp with multiply
	double ext1, ext2, f1, f2;
	ext1 = this->moles;
	ext2 = addee.moles * extensive;
	if (ext1 + ext2 != 0)
	{
		f1 = ext1 / (ext1 + ext2);
		f2 = ext2 / (ext1 + ext2);
	}
	else
	{
		f1 = 0.5;
		f2 = 0.5;
	}
	//double formula_z;

	//double moles;
	this->moles += addee.moles * extensive;
	//cxxNameDouble totals; 
	this->totals.add_extensive(addee.totals, extensive);
	//double la;
	this->la = f1 * this->la + f2 * addee.la;
	//int charge_number;
	//double charge_balance;
	this->charge_balance += addee.charge_balance * extensive;
	//char   *phase_name;

	if (this->phase_name != addee.phase_name)
	{
		std::ostringstream oss;
		oss <<
			"Can not mix two Surface components with same formula and different related phases, "
			<< this->formula;
		P_INSTANCE_POINTER error_msg(oss.str().c_str(), CONTINUE);
		P_INSTANCE_POINTER input_error++;
		return;
	}
	else if (this->phase_name.size() != 0)
	{
		this->phase_proportion =
			this->phase_proportion * f1 + addee.phase_proportion * f2;
	}

	//char   *rate_name;
	if (this->rate_name != addee.rate_name)
	{
		std::ostringstream oss;
		oss <<
			"Can not mix two exchange components with same formula and different related kinetics, "
			<< this->formula;
		P_INSTANCE_POINTER error_msg(oss.str().c_str(), CONTINUE);
		P_INSTANCE_POINTER input_error++;
		return;
	}
	else if (this->rate_name.size() != 0)
	{
		//double phase_proportion;
		this->phase_proportion =
			this->phase_proportion * f1 + addee.phase_proportion * f2;
	}
	if ((this->rate_name.size() != 0 && addee.phase_name.size() != 0) ||
		(this->phase_name.size() != 0 && addee.rate_name.size() != 0))
	{
		std::ostringstream oss;
		oss <<
			"Can not mix exchange components related to phase with exchange components related to kinetics, "
			<< this->formula;
		P_INSTANCE_POINTER error_msg(oss.str().c_str(), CONTINUE);
		P_INSTANCE_POINTER input_error++;
		return;
	}
	//double Dw;

}
void
cxxSurfaceComp::multiply(double extensive)
{
	//char * formula;
	//cxxNameDouble formula_totals; 
	//double formula_z;
	//double moles;
	this->moles *= extensive;
	//cxxNameDouble totals; 
	this->totals.multiply(extensive);
	//double la;
	//int charge_number;
	//double charge_balance;
	this->charge_balance *= extensive;
	//char   *phase_name;
	//double phase_proportion;
	//char   *rate_name;
	//double Dw;
}
const std::string &
cxxSurfaceComp::get_phase_name() const
{
	return this->phase_name;
}
void 
cxxSurfaceComp::set_phase_name(char * f) 
{
	if (f != NULL)
		this->phase_name = std::string(f);
	else
		this->phase_name.clear();
}
const std::string &
cxxSurfaceComp::get_rate_name() const
{
	return this->rate_name;
}
void 
cxxSurfaceComp::set_rate_name(char * f) 
{
	if (f != NULL)
		this->rate_name = std::string(f);
	else
		this->rate_name.clear();
}
const std::string &
cxxSurfaceComp::get_formula() const
{
	return this->formula;
}
void 
cxxSurfaceComp::set_formula(char * f) 
{
	if (f != NULL)
		this->formula = std::string(f);
	else
		this->formula.clear();
}
double 
cxxSurfaceComp::get_charge_balance() const
{
	return this->charge_balance;
}
void 
cxxSurfaceComp::set_charge_balance(double d)
{
	this->charge_balance = d;
}
const cxxNameDouble & 
cxxSurfaceComp::get_totals() const
{
	return (this->totals);
}
std::string 
cxxSurfaceComp::charge_name(PHREEQC_PTR_ARG)
{
	char * str = P_INSTANCE_POINTER string_hsave(this->formula.c_str());
	return (get_charge_name(P_INSTANCE_COMMA str));
}
std::string 
cxxSurfaceComp::get_charge_name(PHREEQC_PTR_ARG_COMMA char *token)
{
	char name[100];
	int l;
	char *ptr1 = token;
	P_INSTANCE_POINTER get_elt(&ptr1, name, &l);
	ptr1 = strchr(name, '_');
	if (ptr1 != NULL)
	{
		ptr1[0] = '\0';
	}
	return (std::string(name));
}
