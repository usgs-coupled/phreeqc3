// SurfaceCharge.cxx: implementation of the cxxSurfaceCharge class.
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
#include "SurfaceCharge.h"
#include "Dictionary.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include "output.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxSurfaceCharge::cxxSurfaceCharge()
//
// default constructor for cxxSurfaceCharge 
//
{
	specific_area = 0.0;
	grams = 0.0;
	charge_balance = 0.0;
	mass_water = 0.0;
	la_psi = 0.0;
	la_psi1 = 0.0;
	la_psi2 = 0.0;
	capacitance[0] = 1.0;
	capacitance[1] = 5.0;
	diffuse_layer_totals.type = cxxNameDouble::ND_ELT_MOLES;
}

cxxSurfaceCharge::cxxSurfaceCharge(struct surface_charge *surf_charge_ptr)
//
// constructor for cxxSurfaceCharge from struct surface_charge
//
:
diffuse_layer_totals(surf_charge_ptr->diffuse_layer_totals)
{
	this->set_name(surf_charge_ptr->name);
	specific_area = surf_charge_ptr->specific_area;
	grams = surf_charge_ptr->grams;
	charge_balance = surf_charge_ptr->charge_balance;
	mass_water = surf_charge_ptr->mass_water;
	la_psi = surf_charge_ptr->la_psi;
	la_psi1 = surf_charge_ptr->la_psi1;
	la_psi2 = surf_charge_ptr->la_psi2;
	capacitance[0] = surf_charge_ptr->capacitance[0];
	capacitance[1] = surf_charge_ptr->capacitance[1];
}

cxxSurfaceCharge::~cxxSurfaceCharge()
{
}

struct master *
cxxSurfaceCharge::get_psi_master()
{
	struct master *master_ptr = NULL;
	std::string str = this->name;

	str.append("_psi");
	master_ptr = master_bsearch(str.c_str());
	if (master_ptr == NULL)
	{
		std::ostringstream error_oss;
		error_oss << "Surface charge psi_master not found." << this->
			name << std::endl;
		//Utilities::error_msg(error_oss.str(), CONTINUE);
		error_msg(error_oss.str().c_str(), CONTINUE);
	}
	return (master_ptr);
}

struct surface_charge *
cxxSurfaceCharge::cxxSurfaceCharge2surface_charge(std::map < std::string, cxxSurfaceCharge > &el)
	//
	// Builds surface_charge structure from of cxxSurfaceCharge 
	//
{
	struct surface_charge *surf_charge_ptr =
		(struct surface_charge *)
		PHRQ_malloc((size_t) (el.size() * sizeof(struct surface_charge)));
	if (surf_charge_ptr == NULL)
		malloc_error();

	int i = 0;
	for (std::map < std::string, cxxSurfaceCharge >::iterator it = el.begin();
		it != el.end(); ++it)
	{
		surf_charge_ptr[i].name = string_hsave((*it).second.name.c_str());
		assert((*it).second.name.size() > 0);
		surf_charge_ptr[i].specific_area = (*it).second.specific_area;
		surf_charge_ptr[i].grams = (*it).second.grams;
		surf_charge_ptr[i].charge_balance = (*it).second.charge_balance;
		surf_charge_ptr[i].mass_water = (*it).second.mass_water;
		surf_charge_ptr[i].la_psi = (*it).second.la_psi;
		surf_charge_ptr[i].la_psi1 = (*it).second.la_psi1;
		surf_charge_ptr[i].la_psi2 = (*it).second.la_psi2;
		surf_charge_ptr[i].capacitance[0] = (*it).second.capacitance[0];
		surf_charge_ptr[i].capacitance[1] = (*it).second.capacitance[1];
		surf_charge_ptr[i].sigma0 = 0;
		surf_charge_ptr[i].sigma1 = 0;
		surf_charge_ptr[i].sigma2 = 0;
		surf_charge_ptr[i].sigmaddl = 0;
		surf_charge_ptr[i].diffuse_layer_totals = (*it).second.diffuse_layer_totals.elt_list();
		//surf_charge_ptr[i].psi_master           = it->get_psi_master();
		surf_charge_ptr[i].count_g = 0;
		surf_charge_ptr[i].g = NULL;
		i++;
	}
	return (surf_charge_ptr);
}

void
cxxSurfaceCharge::dump_xml(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing surf_charge message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// Surf_Charge element and attributes

	s_oss << indent0 << "name=\"" << this->name << "\"" << std::endl;
	s_oss << indent0 << "specific_area=\"" << this->
		specific_area << "\"" << std::endl;
	s_oss << indent0 << "grams=\"" << this->grams << "\"" << std::endl;
	s_oss << indent0 << "charge_balance=\"" << this->
		charge_balance << "\"" << std::endl;
	s_oss << indent0 << "mass_water=\"" << this->
		mass_water << "\"" << std::endl;
	s_oss << indent0 << "la_psi=\"" << this->la_psi << "\"" << std::endl;
	s_oss << indent0 << "la_psi1=\"" << this->la_psi1 << "\"" << std::endl;
	s_oss << indent0 << "la_psi2=\"" << this->la_psi2 << "\"" << std::endl;
	s_oss << indent0 << "capacitance=\"" << this->
		capacitance[0] << " " << this->capacitance[0] << "\"" << std::endl;

	// totals
	s_oss << indent0;
	s_oss << "<diffuse_layer_totals " << std::endl;
	this->diffuse_layer_totals.dump_xml(s_oss, indent + 1);

}

void
cxxSurfaceCharge::dump_raw(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing surf_charge message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// Surf_Charge element and attributes

	s_oss << indent0 << "-name                  " << this->name << std::endl;
	s_oss << indent1 << "-specific_area         " << this->
		specific_area << std::endl;
	s_oss << indent1 << "-grams                 " << this->grams << std::endl;
	s_oss << indent1 << "-charge_balance        " << this->
		charge_balance << std::endl;
	s_oss << indent1 << "-mass_water            " << this->
		mass_water << std::endl;
	s_oss << indent1 << "-la_psi                " << this->
		la_psi << std::endl;
	s_oss << indent1 << "-la_psi1               " << this->
		la_psi1 << std::endl;
	s_oss << indent1 << "-la_psi2               " << this->
		la_psi2 << std::endl;
	s_oss << indent1 << "-capacitance0          " << this->
		capacitance[0] << std::endl;
	s_oss << indent1 << "-capacitance1          " << this->
		capacitance[1] << std::endl;

	// totals
	s_oss << indent1;
	s_oss << "-diffuse_layer_totals" << std::endl;
	this->diffuse_layer_totals.dump_raw(s_oss, indent + 2);

}

void
cxxSurfaceCharge::read_raw(CParser & parser, bool check)
{
	std::string str;

	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(10);
		vopts.push_back("name");	// 0 
		vopts.push_back("specific_area");	// 1 
		vopts.push_back("grams");	// 2 
		vopts.push_back("charge_balance");	// 3 
		vopts.push_back("mass_water");	// 4 
		vopts.push_back("la_psi");	// 5 
		vopts.push_back("diffuse_layer_totals");	// 6 
		vopts.push_back("la_psi1");	// 7 
		vopts.push_back("la_psi2");	// 8 
		vopts.push_back("capacitance0");	// 9 
		vopts.push_back("capacitance1");	// 10 
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;

	opt_save = CParser::OPT_ERROR;
	bool name_defined(false);
	bool specific_area_defined(false);
	bool grams_defined(false);
	bool charge_balance_defined(false);
	bool mass_water_defined(false);
	bool la_psi_defined(false);
	bool la_psi1_defined(false);
	bool la_psi2_defined(false);
	bool capacitance0_defined(false);
	bool capacitance1_defined(false);

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
			//parser.error_msg("Unknown input in SURF_CHARGE read.", CParser::OT_CONTINUE);
			//parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
			break;

		case 0:				// name
			if (!(parser.get_iss() >> str))
			{
				this->name.clear();
				parser.incr_input_error();
				parser.error_msg("Expected string value for name.",
					CParser::OT_CONTINUE);
			}
			else
			{
				this->name = str;
			}
			name_defined = true;
			break;

		case 1:				// specific_area
			if (!(parser.get_iss() >> this->specific_area))
			{
				this->specific_area = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for specific_area.",
					CParser::OT_CONTINUE);
			}
			specific_area_defined = true;
			break;

		case 2:				// grams
			if (!(parser.get_iss() >> this->grams))
			{
				this->grams = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for grams.",
					CParser::OT_CONTINUE);
			}
			grams_defined = true;
			break;


		case 3:				// charge_balance
			if (!(parser.get_iss() >> this->charge_balance))
			{
				this->charge_balance = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for charge_balance.",
					CParser::OT_CONTINUE);
			}
			charge_balance_defined = true;
			break;

		case 4:				// mass_water
			if (!(parser.get_iss() >> this->mass_water))
			{
				this->mass_water = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for mass_water.",
					CParser::OT_CONTINUE);
			}
			mass_water_defined = true;
			break;


		case 5:				// la_psi
			if (!(parser.get_iss() >> this->la_psi))
			{
				this->la_psi = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for la_psi.",
					CParser::OT_CONTINUE);
			}
			la_psi_defined = true;
			break;


		case 6:				// diffuse_layer_totals
			if (this->diffuse_layer_totals.read_raw(parser, next_char) !=
				CParser::PARSER_OK)
			{
				parser.incr_input_error();
				parser.
					error_msg
					("Expected element name and molality for SurfaceCharge diffuse_layer_totals.",
					CParser::OT_CONTINUE);
			}
			opt_save = 6;
			break;

		case 7:				// la_psi1
			if (!(parser.get_iss() >> this->la_psi1))
			{
				this->la_psi1 = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for la_psi1.",
					CParser::OT_CONTINUE);
			}
			la_psi1_defined = true;
			break;

		case 8:				// la_psi2
			if (!(parser.get_iss() >> this->la_psi2))
			{
				this->la_psi2 = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for la_psi.",
					CParser::OT_CONTINUE);
			}
			la_psi2_defined = true;
			break;

		case 9:				// capacitance0
			if (!(parser.get_iss() >> this->capacitance[0]))
			{
				this->capacitance[0] = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for capacitance0.",
					CParser::OT_CONTINUE);
			}
			capacitance0_defined = true;
			break;

		case 10:				// capacitance1
			if (!(parser.get_iss() >> this->capacitance[1]))
			{
				this->capacitance[1] = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for capacitance1.",
					CParser::OT_CONTINUE);
			}
			capacitance1_defined = true;
			break;

		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	if (check)
	{
		// members that must be defined
		if (name_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Name not defined for SurfaceCharge input.",
				CParser::OT_CONTINUE);
		}
		if (specific_area_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Specific_area not defined for SurfaceCharge input.",
				CParser::OT_CONTINUE);
		}
		if (grams_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Grams not defined for SurfaceCharge input.",
				CParser::OT_CONTINUE);
		}
		if (charge_balance_defined == false)
		{
			parser.incr_input_error();
			parser.
				error_msg("Charge_balance not defined for SurfaceCharge input.",
				CParser::OT_CONTINUE);
		}
		if (mass_water_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Mass_water not defined for SurfaceCharge input.",
				CParser::OT_CONTINUE);
		}
		if (la_psi_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("La_psi not defined for SurfaceCharge input.",
				CParser::OT_CONTINUE);
		}
		if (la_psi1_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("La_psi1 not defined for SurfaceCharge input.",
				CParser::OT_CONTINUE);
		}
		if (la_psi2_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("La_psi2 not defined for SurfaceCharge input.",
				CParser::OT_CONTINUE);
		}
		if (capacitance0_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Capacitance0 not defined for SurfaceCharge input.",
				CParser::OT_CONTINUE);
		}
		if (capacitance1_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Capacitance1 not defined for SurfaceCharge input.",
				CParser::OT_CONTINUE);
		}
	}
}

#ifdef USE_MPI
void
cxxSurfaceCharge::mpi_pack(std::vector < int >&ints,
						   std::vector < double >&doubles)
{
	extern cxxDictionary dictionary;

	ints.push_back(dictionary.string2int(this->name));
	doubles.push_back(this->specific_area);
	doubles.push_back(this->grams);
	doubles.push_back(this->charge_balance);
	doubles.push_back(this->mass_water);
	doubles.push_back(this->la_psi);
	doubles.push_back(this->la_psi1);
	doubles.push_back(this->la_psi2);
	doubles.push_back(this->capacitance[0]);
	doubles.push_back(this->capacitance[1]);
	this->diffuse_layer_totals.mpi_pack(ints, doubles);
}

void
cxxSurfaceCharge::mpi_unpack(int *ints, int *ii, double *doubles, int *dd)
{
	extern cxxDictionary dictionary;
	int i = *ii;
	int d = *dd;
	this->name = dictionary.int2stdstring(ints[i++]);
	this->specific_area = doubles[d++];
	this->grams = doubles[d++];
	this->charge_balance = doubles[d++];
	this->mass_water = doubles[d++];
	this->la_psi = doubles[d++];
	this->la_psi1 = doubles[d++];
	this->la_psi2 = doubles[d++];
	this->capacitance[0] = doubles[d++];
	this->capacitance[1] = doubles[d++];
	this->diffuse_layer_totals.mpi_unpack(ints, &i, doubles, &d);
	*ii = i;
	*dd = d;
}
#endif
void
cxxSurfaceCharge::add(const cxxSurfaceCharge & addee, double extensive)
{
	if (extensive == 0.0)
		return;
	//char * name;
	//double specific_area;
	//double grams;
	//double charge_balance;
	//double mass_water;
	//double la_psi, la_psi1, la_psi2;
	//double capacitance[2];

	//char * name;
	if (this->name.size() == 0 && addee.name.size() == 0)
	{
		return;
	}
	assert(this->name == addee.name);

	double ext1, ext2, f1, f2;
	ext1 = this->specific_area * this->grams;
	ext2 = addee.specific_area * addee.grams * extensive;
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

	//double specific_area;
	this->specific_area = f1 * this->specific_area + f2 * addee.specific_area;
	//double grams;
	this->grams += addee.grams * extensive;
	//double charge_balance;
	this->charge_balance += addee.charge_balance * extensive;
	//double mass_water;
	this->mass_water += addee.mass_water * extensive;
	//double la_psi, la_psi1, la_psi2;
	this->la_psi = this->la_psi * f1 + addee.la_psi * f2;
	this->la_psi1 = this->la_psi1 * f1 + addee.la_psi1 * f2;
	this->la_psi2 = this->la_psi2 * f1 + addee.la_psi2 * f2;
	//double capacitance[2];  
	this->capacitance[0] =
		this->capacitance[0] * f1 + this->capacitance[0] * f2;
	this->capacitance[1] =
		this->capacitance[1] * f1 + this->capacitance[1] * f2;
}

void
cxxSurfaceCharge::multiply(double extensive)
{
	//char * name;
	//double specific_area;
	//double grams;
	this->grams *= extensive;
	//double charge_balance;
	this->charge_balance *= extensive;
	//double mass_water;
	this->mass_water *= extensive;
	//double la_psi, la_psi1, la_psi2;
	//double capacitance[2];
	//cxxNameDouble diffuse_layer_totals; 
	this->diffuse_layer_totals.multiply(extensive);
}
