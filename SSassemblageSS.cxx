// SSassemblageSS.cxx: implementation of the cxxSSassemblageSS class.
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
#include "SSassemblageSS.h"
#include "Dictionary.h"
#include "phqalloc.h"
#include "phrqproto.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxSSassemblageSS::cxxSSassemblageSS()
	//
	// default constructor for cxxSSassemblageSS 
	//
{
	//total_moles                    = 0;
	a0 = 0;
	a1 = 0;
	ag0 = 0;
	ag1 = 0;
	miscibility = false;
	//spinodal                       = false;
	//tk                             = 25.;
	xb1 = 0;
	xb2 = 0;
	//SS_PARAMETER_TYPE type         = SS_PARM_NONE;
	//double p[4];
}

cxxSSassemblageSS::cxxSSassemblageSS(struct s_s *s_s_ptr)
		//
		// constructor for cxxSSassemblageSS from struct s_s
		//
{
	this->set_name(s_s_ptr->name);
	//total_moles                              = s_s_ptr->total_moles;                            
	a0 = s_s_ptr->a0;
	a1 = s_s_ptr->a1;
	ag0 = s_s_ptr->ag0;
	ag1 = s_s_ptr->ag1;
	miscibility = (s_s_ptr->miscibility == TRUE);
	//spinodal                                   = (s_s_ptr->spinodal == TRUE);                     
	//tk                                         = s_s_ptr->tk;
	xb1 = s_s_ptr->xb1;
	xb2 = s_s_ptr->xb2;
	//type                                     = s_s_ptr->input_case;                            
	/*
	   for (i = 0; i < 4; i++) {
	   p[i]                               = s_s_ptr->p[i];
	   }
	 */
	int i;
	for (i = 0; i < s_s_ptr->count_comps; i++)
	{
		comps[s_s_ptr->comps[i].name] = s_s_ptr->comps[i].moles;
	}
}

cxxSSassemblageSS::~cxxSSassemblageSS()
{
}

struct s_s *
cxxSSassemblageSS::cxxSSassemblageSS2s_s(std::map < std::string, cxxSSassemblageSS > &el)
		//
		// Builds s_s structure from of cxxSSassemblageSS 
		//
{

	// 
	//  generate s_s structures
	//
	struct s_s *s_s_ptr =
		(struct s_s *) PHRQ_malloc((size_t) (el.size() * sizeof(struct s_s)));
	if (s_s_ptr == NULL)
		malloc_error();
	int j = 0;
	for (std::map < std::string, cxxSSassemblageSS >::iterator it = el.begin();
		 it != el.end(); ++it)
	{
		s_s_ptr[j].name = string_hsave((*it).second.name.c_str());
		assert((*it).second.name.size() > 0);
		//s_s_ptr[j].total_moles                                 = it->total_moles;
		s_s_ptr[j].total_moles = 0;
		s_s_ptr[j].dn = 0;
		s_s_ptr[j].a0 = (*it).second.a0;
		s_s_ptr[j].a1 = (*it).second.a1;
		s_s_ptr[j].ag0 = (*it).second.ag0;
		s_s_ptr[j].ag1 = (*it).second.ag1;
		//s_s_ptr[j].ag0                                         = 0;
		//s_s_ptr[j].ag1                                         = 0;
		s_s_ptr[j].s_s_in = TRUE;
		s_s_ptr[j].miscibility = (*it).second.miscibility;
		//s_s_ptr[j].spinodal                                    = it->spinodal;
		s_s_ptr[j].spinodal = FALSE;
		//s_s_ptr[j].tk                                          = it->tk;
		s_s_ptr[j].tk = 273.15;
		s_s_ptr[j].xb1 = (*it).second.xb1;
		s_s_ptr[j].xb2 = (*it).second.xb2;
		s_s_ptr[j].input_case = 0;
		s_s_ptr[j].p[0] = 0;
		s_s_ptr[j].p[1] = 0;
		s_s_ptr[j].p[2] = 0;
		s_s_ptr[j].p[3] = 0;
		// 
		//  generate s_s_comp structures
		//
		s_s_ptr[j].count_comps = (int) (*it).second.comps.size();
		s_s_ptr[j].comps = NULL;
		if ((*it).second.comps.size() > 0)
		{
			int i = 0;
			int n;
			struct s_s_comp *s_s_comp_ptr =
				(struct s_s_comp *)
				PHRQ_malloc((size_t)
							((*it).second.comps.size() * sizeof(struct s_s_comp)));
			if (s_s_comp_ptr == NULL)
				malloc_error();
			for (cxxNameDouble::iterator itc = (*it).second.comps.begin();
				 itc != (*it).second.comps.end(); ++itc)
			{
				s_s_comp_ptr[i].name = string_hsave(itc->first.c_str());
				assert(itc->first.size() > 0);
				s_s_comp_ptr[i].phase = phase_bsearch(itc->first.c_str(), &n, TRUE);
				s_s_comp_ptr[i].initial_moles = 0;
				s_s_comp_ptr[i].moles = itc->second;
				s_s_comp_ptr[i].init_moles = 0;
				s_s_comp_ptr[i].delta = 0;
				s_s_comp_ptr[i].fraction_x = 0;
				s_s_comp_ptr[i].log10_lambda = 0;
				s_s_comp_ptr[i].log10_fraction_x = 0;
				s_s_comp_ptr[i].dn = 0;
				s_s_comp_ptr[i].dnc = 0;
				s_s_comp_ptr[i].dnb = 0;
				i++;
			}
			s_s_ptr[j].comps = s_s_comp_ptr;
		}
		j++;
	}
	return (s_s_ptr);
}

#ifdef SKIP
void
cxxSSassemblageSS::dump_xml(std::ostream & s_oss, unsigned int indent) const const
{
	//const char    ERR_MESSAGE[] = "Packing s_s message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// S_S element and attributes

	s_oss << indent0 << "name=\"" << this->name << "\"" << std::endl;
	s_oss << indent0 << "add_formula=\"" << this->
		add_formula << "\"" << std::endl;
	s_oss << indent0 << "si=\"" << this->si << "\"" << std::endl;
	s_oss << indent0 << "moles=\"" << this->moles << "\"" << std::endl;
	s_oss << indent0 << "delta=\"" << this->delta << "\"" << std::endl;
	s_oss << indent0 << "initial_moles=\"" << this->
		initial_moles << "\"" << std::endl;
	s_oss << indent0 << "dissolve_only=\"" << this->
		dissolve_only << "\"" << std::endl;

}
#endif
void
cxxSSassemblageSS::dump_raw(std::ostream & s_oss, unsigned int indent) const
{
	//const char    ERR_MESSAGE[] = "Packing s_s message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	// S_S element and attributes

	s_oss << indent0 << "-name                  " << this->name << std::endl;
	//s_oss << indent0 << "-total_moles           " << this->total_moles    << std::endl;
	s_oss << indent1 << "-a0                    " << this->a0 << std::endl;
	s_oss << indent1 << "-a1                    " << this->a1 << std::endl;
	s_oss << indent1 << "-ag0                   " << this->ag0 << std::endl;
	s_oss << indent1 << "-ag1                   " << this->ag1 << std::endl;
	s_oss << indent1 << "-miscibility           " << this->miscibility << std::endl;
	//s_oss << indent0 << "-spinodal              " << this->spinodal << std::endl;
	//s_oss << indent0 << "-tk                    " << this->tk << std::endl;
	s_oss << indent1 << "-xb1                   " << this->xb1 << std::endl;
	s_oss << indent1 << "-xb2                   " << this->xb2 << std::endl;
	s_oss << indent1 << "-component             " << std::endl;
	this->comps.dump_raw(s_oss, indent + 2);
}

void
cxxSSassemblageSS::read_raw(CParser & parser, bool check)
{
	std::string str;

	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(10);
		vopts.push_back("name");	// 0                                   
		vopts.push_back("total_moles");	// 1   
		vopts.push_back("a0");	// 2   
		vopts.push_back("a1");	// 3
		vopts.push_back("components");	// 4
		vopts.push_back("miscibility");	// 5
		vopts.push_back("spinodal");	// 6
		vopts.push_back("tk");	// 7
		vopts.push_back("xb1");	// 8
		vopts.push_back("xb2");	// 9
		vopts.push_back("ag0");	// 10
		vopts.push_back("ag1");	// 11
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;

	opt_save = CParser::OPT_ERROR;
	bool name_defined(false);
	//bool total_moles_defined(false);
	bool a0_defined(false);
	bool a1_defined(false);
	bool ag0_defined(false);
	bool ag1_defined(false);
	bool miscibility_defined(false);
	//bool spinodal_defined(false); 
	//bool tk_defined(false); 
	bool xb1_defined(false);
	bool xb2_defined(false);

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
			//parser.error_msg("Unknown input in S_S read.", CParser::OT_CONTINUE);
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
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 1:				// total_moles
			/*
			   if (!(parser.get_iss() >> this->total_moles))
			   {
			   this->total_moles = 0;
			   parser.incr_input_error();
			   parser.error_msg("Expected numeric value for total_moles.", CParser::OT_CONTINUE);
			   }
			   total_moles_defined = true;
			 */
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 2:				// a0
			if (!(parser.get_iss() >> this->a0))
			{
				this->a0 = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for a0.",
								 CParser::OT_CONTINUE);
			}
			a0_defined = true;
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 3:				// a1
			if (!(parser.get_iss() >> this->a1))
			{
				this->a1 = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for a1.",
								 CParser::OT_CONTINUE);
			}
			a1_defined = true;
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 4:				// components
			if (this->comps.read_raw(parser, next_char) != CParser::PARSER_OK)
			{
				parser.incr_input_error();
				parser.error_msg("Expected phase name and moles for comps.",
								 CParser::OT_CONTINUE);
			}
			opt_save = 4;
			break;

		case 5:				// miscibility
			if (!(parser.get_iss() >> this->miscibility))
			{
				this->miscibility = 0;
				parser.incr_input_error();
				parser.error_msg("Expected boolean value for miscibility.",
								 CParser::OT_CONTINUE);
			}
			miscibility_defined = true;
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 6:				// spinodal
			/*
			   if (!(parser.get_iss() >> this->spinodal))
			   {
			   this->spinodal = 0;
			   parser.incr_input_error();
			   parser.error_msg("Expected boolean value for spinodal.", CParser::OT_CONTINUE);
			   }
			   spinodal_defined = true;
			 */
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 7:				// tk
			/*
			   if (!(parser.get_iss() >> this->tk))
			   {
			   this->tk = 0;
			   parser.incr_input_error();
			   parser.error_msg("Expected numeric value for tk.", CParser::OT_CONTINUE);
			   }
			   tk_defined = true;
			 */
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 8:				// xb1
			if (!(parser.get_iss() >> this->xb1))
			{
				this->xb1 = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for xb1.",
								 CParser::OT_CONTINUE);
			}
			xb1_defined = true;
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 9:				// xb2
			if (!(parser.get_iss() >> this->xb2))
			{
				this->xb2 = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for xb2.",
								 CParser::OT_CONTINUE);
			}
			xb2_defined = true;
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 10:				// ag0
			if (!(parser.get_iss() >> this->ag0))
			{
				this->ag0 = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for ag0.",
								 CParser::OT_CONTINUE);
			}
			ag0_defined = true;
			opt_save = CParser::OPT_DEFAULT;
			break;

		case 11:				// ag1
			if (!(parser.get_iss() >> this->ag1))
			{
				this->ag1 = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for ag1.",
								 CParser::OT_CONTINUE);
			}
			ag1_defined = true;
			opt_save = CParser::OPT_DEFAULT;
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
			parser.error_msg("Name not defined for SSassemblageSS input.",
				CParser::OT_CONTINUE);
		}
		/*
		if (total_moles_defined == false) {
		parser.incr_input_error();
		parser.error_msg("Total_moles not defined for SSassemblageSS input.", CParser::OT_CONTINUE);
		}
		*/
		if (a0_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("A0 not defined for SSassemblageSS input.",
				CParser::OT_CONTINUE);
		}
		if (a1_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("A1 not defined for SSassemblageSS input.",
				CParser::OT_CONTINUE);
		}
		if (ag0_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Ag0 not defined for SSassemblageSS input.",
				CParser::OT_CONTINUE);
		}
		if (ag1_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Ag1 not defined for SSassemblageSS input.",
				CParser::OT_CONTINUE);
		}
		if (miscibility_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Miscibility not defined for SSassemblageSS input.",
				CParser::OT_CONTINUE);
		}
		/*
		if (spinodal_defined == false) {
		parser.incr_input_error();
		parser.error_msg("Spinodal not defined for SSassemblageSS input.", CParser::OT_CONTINUE);
		}
		if (tk_defined == false) {
		parser.incr_input_error();
		parser.error_msg("Tk not defined for SSassemblageSS input.", CParser::OT_CONTINUE);
		}
		*/
		if (xb1_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Xb1 not defined for SSassemblageSS input.",
				CParser::OT_CONTINUE);
		}
		if (xb2_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Xb2 not defined for SSassemblageSS input.",
				CParser::OT_CONTINUE);
		}
	}
}

#ifdef USE_MPI
void
cxxSSassemblageSS::mpi_pack(std::vector < int >&ints,
							std::vector < double >&doubles)
{
	extern cxxDictionary dictionary;
	ints.push_back(dictionary.string2int(this->name));
	this->comps.mpi_pack(ints, doubles);
	doubles.push_back(this->a0);
	doubles.push_back(this->a1);
	doubles.push_back(this->ag0);
	doubles.push_back(this->ag1);
	ints.push_back((int) this->miscibility);
	doubles.push_back(this->xb1);
	doubles.push_back(this->xb2);
}

void
cxxSSassemblageSS::mpi_unpack(int *ints, int *ii, double *doubles, int *dd)
{
	extern cxxDictionary dictionary;
	int i = *ii;
	int d = *dd;
	this->name = dictionary.int2stdstring(ints[i++]);
	this->comps.mpi_unpack(ints, &i, doubles, &d);
	this->a0 = doubles[d++];
	this->a1 = doubles[d++];
	this->ag0 = doubles[d++];
	this->ag1 = doubles[d++];
	this->miscibility = (ints[i++] != 0);
	this->xb1 = doubles[d++];
	this->xb2 = doubles[d++];
	*ii = i;
	*dd = d;
}
#endif

void
cxxSSassemblageSS::totalize()
{
	this->totals.clear();
	// component structures
	for (cxxNameDouble::const_iterator it = this->comps.begin();
		 it != this->comps.end(); it++)
	{
		struct phase *phase_ptr;
		int l;
		phase_ptr = phase_bsearch(it->first.c_str(), &l, FALSE);
		if (phase_ptr != NULL)
		{
			cxxNameDouble phase_formula(phase_ptr->next_elt);
			this->totals.add_extensive(phase_formula, it->second);
		}
		else
		{
			assert(false);
		}
	}
	return;
}

void
cxxSSassemblageSS::add(const cxxSSassemblageSS & addee, double extensive)
{
	if (extensive == 0.0)
		return;
	if (addee.name.size() == 0)
		return;
	// this and addee must have same name
	// otherwise generate a new PPassemblagComp with multiply

	//char *name;
	//cxxNameDouble comps;
	this->comps.add_extensive(addee.comps, extensive);
	//double a0, a1;
	//double ag0, ag1;
	//bool miscibility;
	//double xb1, xb2;
}

void
cxxSSassemblageSS::multiply(double extensive)
{
	//char *name;
	//cxxNameDouble comps;
	this->comps.multiply(extensive);
	//double a0, a1;
	//double ag0, ag1;
	//bool miscibility;
	//double xb1, xb2;
}
