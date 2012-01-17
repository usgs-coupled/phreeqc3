// SScomp.cxx: implementation of the cxxSScomp class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include <cassert>				// assert
#include <algorithm>			// std::sort

#include "Utils.h"				// define first
#include "Phreeqc.h"
#include "SScomp.h"
//#include "Dictionary.h"
#include "phqalloc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxSScomp::cxxSScomp(PHRQ_io *io)
:
PHRQ_base(io)
	//
	// default constructor for cxxSScomp 
	//
{
	name = "";
	initial_moles = 0;
	moles = 0;
	init_moles = 0;
	delta = 0;
	fraction_x = 0;
	log10_lambda = 0;
	log10_fraction_x = 0;
	dn = 0;
	dnc = 0; 
	dnb = 0;
}
#ifdef SKIP
cxxSScomp::cxxSScomp(struct pure_phase * pure_phase_ptr, PHRQ_io *io)
:
PHRQ_base(io)
	//
	// constructor for cxxSScomp from struct pure_phase
	//
{
	this->Set_name(pure_phase_ptr->name);
	this->Set_add_formula(pure_phase_ptr->add_formula);
	si = pure_phase_ptr->si;
	si_org = pure_phase_ptr->si_org;
	moles = pure_phase_ptr->moles;
	delta = pure_phase_ptr->delta;
	initial_moles = pure_phase_ptr->initial_moles;
	force_equality = (pure_phase_ptr->force_equality == TRUE);
	dissolve_only = (pure_phase_ptr->dissolve_only == TRUE);
	precipitate_only = (pure_phase_ptr->precipitate_only == TRUE);
}
#endif
cxxSScomp::~cxxSScomp()
{
}

void
cxxSScomp::dump_raw(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing pure_phase message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// Pure_Phase element and attributes

	//if (this->name.size() != 0)
	//	s_oss << indent0 << "-name                  " << this->name << "\n";
	//if (this->add_formula.size() != 0)
	//	s_oss << indent1 << "-add_formula           " << this->add_formula << "\n";
	s_oss << indent0 << "-initial_moles         " << this->initial_moles << "\n";
	s_oss << indent0 << "-moles                 " << this->moles << "\n";
	s_oss << indent0 << "-init_moles            " << this->init_moles << "\n";
	s_oss << indent0 << "-delta                 " << this->delta << "\n";
	s_oss << indent0 << "-fraction_x            " << this->fraction_x << "\n";
	s_oss << indent0 << "-log10_lambda          " << this->log10_lambda << "\n";
	s_oss << indent0 << "-log10_fraction_x      " << this->log10_fraction_x << "\n";
	//s_oss << indent0 << "-dn                    " << this->dn << "\n";
	//s_oss << indent0 << "-dnc                   " << this->dnc << "\n";
	//s_oss << indent0 << "-dnb                   " << this->dnb << "\n";
}

void
cxxSScomp::read_raw(CParser & parser, bool check)
{
	std::string str;

	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(10);
		vopts.push_back("name_not_used");	// 0                 
		vopts.push_back("initial_moles");	// 1
		vopts.push_back("moles");	// 2
		vopts.push_back("init_moles");	// 3
		vopts.push_back("delta");	// 4
		vopts.push_back("fraction_x");	// 5     
		vopts.push_back("log10_lambda");	// 6
		vopts.push_back("log10_fraction_x");	// 7
		vopts.push_back("dn_not_used");	// 8
		vopts.push_back("dnc_not_used");	// 9
		vopts.push_back("dnb_not_used");	// 10
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;

	opt_save = CParser::OPT_ERROR;
	bool name_defined(false);
	bool initial_moles_defined(false);
	bool moles_defined(false);
	//bool init_moles_defined(false);
	//bool delta_defined(false);

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
			// Allow return to Exchange for more processing
			//parser.error_msg("Unknown input in PURE_PHASE read.", PHRQ_io::OT_CONTINUE);
			//parser.error_msg(parser.line().c_str(), PHRQ_io::OT_CONTINUE);
			break;

		case 0:				// name
			if (!(parser.get_iss() >> str))
			{
				this->name.clear();
				parser.incr_input_error();
				parser.error_msg("Expected string value for name.",
								 PHRQ_io::OT_CONTINUE);
			}
			else
			{
				this->name = str;
			}
			name_defined = true;
			break;

		case 1:				// initial_moles
			if (!(parser.get_iss() >> this->initial_moles))
			{
				this->initial_moles = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for initial_moles.",
								 PHRQ_io::OT_CONTINUE);
			}
			initial_moles_defined = true;
			break;

		case 2:				// moles
			if (!(parser.get_iss() >> this->moles))
			{
				this->moles = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for moles.",
								 PHRQ_io::OT_CONTINUE);
			}
			moles_defined = true;
			break;

		case 3:				// init_moles
			if (!(parser.get_iss() >> this->init_moles))
			{
				this->init_moles = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for init_moles.",
								 PHRQ_io::OT_CONTINUE);
			}
			//init_moles_defined = true;
			break;

		case 4:				// delta
			if (!(parser.get_iss() >> this->delta))
			{
				this->delta = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for delta.",
								 PHRQ_io::OT_CONTINUE);
			}
			//delta_defined = true;
			break;

		case 5:				// fraction_x
			if (!(parser.get_iss() >> this->fraction_x))
			{
				this->fraction_x = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for fraction_x.",
								 PHRQ_io::OT_CONTINUE);
			}
			//initial_moles_defined = true;
			break;


		case 6:				// log10_lambda
			if (!(parser.get_iss() >> this->log10_lambda))
			{
				this->log10_lambda = 0;
				parser.incr_input_error();
				parser.error_msg("Expected boolean value for log10_lambda.",
								 PHRQ_io::OT_CONTINUE);
			}
			break;

		case 7:				// log10_fraction_x
			if (!(parser.get_iss() >> this->log10_fraction_x))
			{
				this->log10_fraction_x = 0;
				parser.incr_input_error();
				parser.error_msg("Expected boolean value for log10_fraction_x.",
								 PHRQ_io::OT_CONTINUE);
			}
			break;

		case 8:				// dn
			if (!(parser.get_iss() >> this->dn))
			{
				this->dn = 0;
				parser.incr_input_error();
				parser.error_msg("Expected boolean value for dn.",
								 PHRQ_io::OT_CONTINUE);
			}
			break;
		case 9:				// dnc
			if (!(parser.get_iss() >> this->dnc))
			{
				this->dnc = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for dnc.",
								 PHRQ_io::OT_CONTINUE);
			}
			//dnc_defined = true;
			break;
		case 10:				// dnb
			if (!(parser.get_iss() >> this->dnb))
			{
				this->dnb = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for dnb.",
								 PHRQ_io::OT_CONTINUE);
			}
			//dnc_defined = true;
			break;
		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	// members that must be defined
	if (check)
	{
		if (name_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Name not defined for PPassemblageComp input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (moles_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Moles not defined for PPassemblageComp input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (initial_moles_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Initial_moles not defined for PPassemblageComp input.",
				PHRQ_io::OT_CONTINUE);
		}
	}
}

#ifdef USE_MPI
void
cxxSScomp::mpi_pack(std::vector < int >&ints,
							  std::vector < double >&doubles)
{
	extern cxxDictionary dictionary;

	ints.push_back(dictionary.string2int(this->name));
	ints.push_back(dictionary.string2int(this->add_formula));
	doubles.push_back(this->si);
	doubles.push_back(this->si_org);
	doubles.push_back(this->moles);
	doubles.push_back(this->delta);
	doubles.push_back(this->initial_moles);
	ints.push_back((int) this->force_equality);
	ints.push_back((int) this->dissolve_only);
	ints.push_back((int) this->precipitate_only);
}

void
cxxSScomp::mpi_unpack(int *ints, int *ii, double *doubles, int *dd)
{
	extern cxxDictionary dictionary;
	int i = *ii;
	int d = *dd;
	this->name = dictionary.int2stdstring(ints[i++]);
	this->add_formula = dictionary.int2stdstring(ints[i++]);
	this->si = doubles[d++];
	this->si_org = doubles[d++];
	this->moles = doubles[d++];
	this->delta = doubles[d++];
	this->initial_moles = doubles[d++];
	this->force_equality = (ints[i++] != 0);
	this->dissolve_only = (ints[i++] != 0);
	this->precipitate_only = (ints[i++] != 0);
	*ii = i;
	*dd = d;
}
#endif
#ifdef SKIP
void
cxxSScomp::totalize(PHREEQC_PTR_ARG)
{
	this->totals.clear();
	// component structures
	if (this->add_formula.size() != 0)
		return;
	struct phase *phase_ptr;
	int l;
	phase_ptr = P_INSTANCE_POINTER phase_bsearch(this->name.c_str(), &l, FALSE);
	if (phase_ptr != NULL)
	{
		cxxNameDouble phase_formula(phase_ptr->next_elt);
		this->totals.add_extensive(phase_formula, this->moles);
	}
	else
	{
		assert(false);
	}
	return;
}
#endif
#ifdef SKIP
void
cxxSScomp::add(const cxxSScomp & addee, double extensive)
{
	double ext1, ext2, f1, f2;
	if (extensive == 0.0)
		return;
	if (addee.name.size() == 0)
		return;
	// this and addee must have same name
	// otherwise generate a new PPassemblagComp with multiply

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
	//char * name;
	//char *add_formula;

	if (this->name.size() == 0 && addee.name.size() == 0)
	{
		return;
	}
	assert(this->name == addee.name);
	if (this->add_formula != addee.add_formula)
	{
		std::ostringstream oss;
		oss <<
			"Can not mix two Equilibrium_phases with differing add_formulae., "
			<< this->name;
		error_msg(oss.str().c_str(), CONTINUE);
		return;
	}
	//double si;
	this->si = this->si * f1 + addee.si * f2;
	//double si_org;
	this->si_org = this->si_org * f1 + addee.si_org * f2;
	//double moles;
	this->moles += addee.moles * extensive;
	//double delta;
	this->delta += addee.delta * extensive;
	//double initial_moles;
	this->initial_moles += addee.initial_moles * extensive;
	//bool force_equality;
	//bool dissolve_only;

}
#endif
void
cxxSScomp::multiply(double extensive)
{
	//char * name;
	//char *add_formula;
	//double si;
	//double moles;
	this->moles *= extensive;
	//double delta;
	this->delta *= extensive;
	//double initial_moles;
	this->initial_moles *= extensive;
	//bool force_equality;
	//bool dissolve_only;
}
