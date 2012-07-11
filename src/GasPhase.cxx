// GasPhase.cxx: implementation of the cxxGasPhase class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include <cassert>				// assert
#include <algorithm>			// std::sort
#include <float.h>

#include "Utils.h"				// define first
#include "Phreeqc.h"
#include "GasPhase.h"
#include "cxxMix.h"
#include "phqalloc.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxGasPhase::cxxGasPhase(PHRQ_io * io)
	//
	// default constructor for cxxGasPhase 
	//
:	cxxNumKeyword(io)
{
	new_def = false;
	solution_equilibria = false;
	n_solution = -999;
	type = cxxGasPhase::GP_PRESSURE;
	total_p = 1.0;
	total_moles = 0.0;
	volume = 1.0;
	v_m = 0;
	pr_in = false;
	temperature = 298.15;
}

cxxGasPhase::cxxGasPhase(std::map < int, cxxGasPhase > &entity_map,
		cxxMix & mx, int l_n_user, PHRQ_io * io)
: cxxNumKeyword(io)
{
	this->n_user = this->n_user_end = l_n_user;
	total_p = 0;
	volume = 0;
	v_m = 0;
	pr_in = false;
	bool first = true;
//
//   Mix
//
	// accumulate in map
	std::map<std::string, cxxGasComp> comp_map;
	std::map<std::string, cxxGasComp>::iterator comp_it;

	const std::map < int, LDBLE > & mixcomps = mx.Get_mixComps();
	std::map < int, LDBLE >::const_iterator it;
	for (it = mixcomps.begin(); it != mixcomps.end(); it++)
	{
		const cxxGasPhase *entity_ptr =	&(entity_map.find(it->first)->second);
		if (first)
		{
			this->new_def = entity_ptr->new_def;
			this->solution_equilibria = entity_ptr->solution_equilibria;
			this->n_solution = entity_ptr->n_solution;
			this->type = entity_ptr->type;
			this->total_p = entity_ptr->total_p * it->second;
			this->total_moles = entity_ptr->total_moles * it->second;
			this->volume = entity_ptr->volume * it->second;
			this->v_m = entity_ptr->v_m * it->second;
			this->pr_in = entity_ptr->pr_in;
			this->temperature = entity_ptr->temperature;
			first = false;
		}
		else
		{
			if (this->type != entity_ptr->type)
			{
				std::ostringstream oss;
				oss << "Can not mix two gas_phases with differing types.";
				error_msg(oss.str().c_str(), CONTINUE);
				return;
			}

			this->total_p += entity_ptr->total_p * it->second;
			this->volume += entity_ptr->volume * it->second;
			this->v_m += entity_ptr->v_m * it->second;
		}
		cxxGasPhase *gas_phase_ptr = Utilities::Rxn_find(entity_map, it->first);
		if (gas_phase_ptr)
		{
			std::vector<cxxGasComp> add_comps = gas_phase_ptr->Get_gas_comps();
			for (size_t i = 0; i < add_comps.size(); i++)
			{
				comp_it = comp_map.find(add_comps[i].Get_phase_name());
				if (comp_it != comp_map.end())
				{
					comp_it->second.add(add_comps[i], it->second);
				}
				else
				{
					cxxGasComp gc(add_comps[i]);
					gc.multiply(it->second);
					comp_map[add_comps[i].Get_phase_name()] = gc;
				}
			}

		}
	}

	// put map into vector
	this->gas_comps.clear();
	std::vector<cxxGasComp> gc;
	for (comp_it = comp_map.begin(); comp_it != comp_map.end(); comp_it++)
	{
		this->gas_comps.push_back(comp_it->second);
	}
}
#ifdef SKIP
cxxGasPhase::cxxGasPhase(const std::map < int, cxxGasPhase > &entities,
						 cxxMix & mix, int l_n_user, PHRQ_io * io):
cxxNumKeyword(io)
{
	this->n_user = this->n_user_end = l_n_user;
	gasPhaseComps.type = cxxNameDouble::ND_NAME_COEF;
	total_p = 0;
	volume = 0;
	v_m = 0;
	pr_in = false;
	bool first = true;
//
//   Mix
//
	//cxxNameDouble gasPhaseComps;
	const std::map < int, LDBLE > & mixcomps = mix.Get_mixComps();
	std::map < int, LDBLE >::const_iterator it;
	for (it = mixcomps.begin(); it != mixcomps.end(); it++)
	{
		if (entities.find(it->first) != entities.end())
		{
			const cxxGasPhase *entity_ptr =
				&(entities.find(it->first)->second);
			this->gasPhaseComps.add_extensive(entity_ptr->gasPhaseComps,
											  it->second);
			//GP_TYPE type;
			//LDBLE total_p;
			//LDBLE volume;
			if (first)
			{
				this->type = entity_ptr->type;
				this->total_p = entity_ptr->total_p * it->second;
				this->volume = entity_ptr->volume * it->second;
				this->v_m = entity_ptr->v_m * it->second;
				this->pr_in = entity_ptr->pr_in;
				first = false;
			}
			else
			{
				if (this->type != entity_ptr->type)
				{
					std::ostringstream oss;
					oss << "Can not mix two gas_phases with differing types.";
					error_msg(oss.str().c_str(), CONTINUE);
					//input_error++;
					return;
				}

				this->total_p += entity_ptr->total_p * it->second;
				this->volume += entity_ptr->volume * it->second;
				this->v_m += entity_ptr->v_m * it->second;
			}
		}
	}
}
#endif
cxxGasPhase::~cxxGasPhase()
{
}

#ifdef SKIP
void
cxxGasPhase::dump_xml(std::ostream & s_oss, unsigned int indent) const const
{
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// GasPhase element and attributes
	s_oss << indent0;
	s_oss << "<gas_phase " << "\n";

	s_oss << indent1;
	s_oss << "pitzer_gas_phase_gammas=\"" << this->
		pitzer_gas_phase_gammas << "\"" << "\n";

	// components
	s_oss << indent1;
	s_oss << "<component " << "\n";
	for (std::list < cxxGasPhaseComp >::const_iterator it =
		 gas_phaseComps.begin(); it != gas_phaseComps.end(); ++it)
	{
		it->dump_xml(s_oss, indent + 2);
	}

	return;
}
#endif

void
cxxGasPhase::dump_raw(std::ostream & s_oss, unsigned int indent, int *n_out) const
{
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// GasPhase element and attributes
	s_oss << indent0;
	int n_user_local = (n_out != NULL) ? *n_out : this->n_user;
	s_oss << "GAS_PHASE_RAW                " << n_user_local << " " << this->description << "\n";

	s_oss << indent1 << "# GAS_PHASE_MODIFY candidate identifiers #\n";
	s_oss << indent1;
	s_oss << "-type                      " << this->type << "\n";
	s_oss << indent1;
	s_oss << "-total_p                   " << this->total_p << "\n";
	s_oss << indent1;
	s_oss << "-volume                    " << this->volume << "\n";
	// gasPhaseComps 
	for (size_t i = 0 ; i < this->gas_comps.size(); i++)
	{
		s_oss << indent1;
		s_oss << "-component                 " << this->gas_comps[i].Get_phase_name() << "\n";
		this->gas_comps[i].dump_raw(s_oss, indent + 2);
	}

	s_oss << indent1 << "# GAS_PHASE_MODIFY candidate identifiers with new_def=true #\n";
	s_oss << indent1;
	s_oss << "-new_def                   " << this->new_def << "\n";
	s_oss << indent1;
	s_oss << "-solution_equilibria       " << this->solution_equilibria << "\n";
	s_oss << indent1;
	s_oss << "-n_solution                " << this->n_solution << "\n";
	s_oss << indent1;
	s_oss << "-temperature               " << this->temperature << "\n";

	s_oss << indent1 << "# GasPhase workspace variables #\n";
	s_oss << indent1;
	s_oss << "-total_moles               " << this->total_moles << "\n";
	s_oss << indent1;
	s_oss << "-v_m                       " << this->v_m << "\n";
	s_oss << indent1;
	s_oss << "-pr_in                     " << (this->pr_in ? 1 : 0) << "\n";
	s_oss << indent1;
	s_oss << "-totals                    " << "\n";
	this->totals.dump_raw(s_oss, indent + 2);
}

void
cxxGasPhase::read_raw(CParser & parser, bool check)
{

	int i;
#if defined(NO_STATIC_VOPTS)
	std::vector < std::string > vopts;
#else
	static std::vector < std::string > vopts;
#endif
	if (vopts.empty())
	{
		vopts.reserve(15);
		vopts.push_back("type");				//0
		vopts.push_back("total_p");				//1
		vopts.push_back("volume");				//2
		vopts.push_back("v_m");					//3
		vopts.push_back("component");			//4
		vopts.push_back("pressure");			//5
		vopts.push_back("pr_in");				//6
		vopts.push_back("new_def");				//7
		vopts.push_back("solution_equilibria");	//8
		vopts.push_back("n_solution");			//9
		vopts.push_back("total_moles");			//10
		vopts.push_back("temperature");			//11
		vopts.push_back("totals");              //12
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;
	bool useLastLine(false);

	// Read gas_phase number and description
	this->read_number_description(parser);

	opt_save = CParser::OPT_ERROR;
	bool type_defined(false);
	bool total_p_defined(false);
	bool volume_defined(false);
	bool v_m_defined(false);
	bool pr_in_defined(false);

	for (;;)
	{
		int opt;
		if (useLastLine == false)
		{
			opt = parser.get_option(vopts, next_char);
		}
		else
		{
			CParser::ECHO_OPTION eo = parser.get_echo_file();
			parser.set_echo_file(CParser::EO_NONE);
			opt = parser.getOptionFromLastLine(vopts, next_char, true);
			parser.set_echo_file(eo);
		}
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
			opt = CParser::OPT_EOF;
			parser.error_msg("Unknown input in GAS_PHASE_COMP_RAW keyword.",
							 PHRQ_io::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), PHRQ_io::OT_CONTINUE);
			useLastLine = false;
			break;

		case 0:				// type
			if (!(parser.get_iss() >> i))
			{
				this->type = cxxGasPhase::GP_PRESSURE;
				parser.incr_input_error();
				parser.error_msg("Expected enum for type.",
								 PHRQ_io::OT_CONTINUE);
			}
			else
			{
				this->type = (cxxGasPhase::GP_TYPE) i;
			}
			type_defined = true;
			useLastLine = false;
			break;

		case 1:				// total_p
		case 5:				// pressure
			if (!(parser.get_iss() >> this->total_p))
			{
				this->total_p = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for total_p.",
								 PHRQ_io::OT_CONTINUE);
			}
			total_p_defined = true;
			useLastLine = false;
			break;

		case 2:				// volume
			if (!(parser.get_iss() >> this->volume))
			{
				this->volume = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for volume.",
								 PHRQ_io::OT_CONTINUE);
			}
			volume_defined = true;
			useLastLine = false;
			break;

		case 3:				// v_m
			if (!(parser.get_iss() >> this->v_m))
			{
				this->v_m = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for v_m.",
								 PHRQ_io::OT_CONTINUE);
			}
			v_m_defined = true;
			useLastLine = false;
			break;
		case 4:				// component
			{
				std::string str;
				if (!(parser.get_iss() >> str))
				{
					parser.incr_input_error();
					parser.error_msg("Expected string value for component name.",
									 PHRQ_io::OT_CONTINUE);
				}
				else
				{
					cxxGasComp temp_comp(io);
					temp_comp.Set_phase_name(str);
					cxxGasComp * comp_ptr = this->Find_comp(str.c_str());
					if (comp_ptr)
					{
						temp_comp = *comp_ptr;	
					}
					temp_comp.read_raw(parser, false);
					if (comp_ptr)
					{
						for (size_t j = 0; j < this->gas_comps.size(); j++)
						{
							if (Utilities::strcmp_nocase(this->gas_comps[j].Get_phase_name().c_str(), str.c_str()) == 0)
							{
								this->gas_comps[j] = temp_comp;
							}
						}
					}
					else
					{
						this->gas_comps.push_back(temp_comp);
					}
					useLastLine = true;
				}
			}
			
			opt_save = CParser::OPT_DEFAULT;
			break;
		case 6:				// pr_in
			if (!(parser.get_iss() >> i))
			{
				parser.incr_input_error();
				parser.error_msg("Expected 0/1 for pr_in.", PHRQ_io::OT_CONTINUE);
			}
			else
			{
				this->pr_in = (i == 0) ? false : true;
			}
			pr_in_defined = true;
			useLastLine = false;
			break;
		case 7:				// new_def
			if (!(parser.get_iss() >> i))
			{
				parser.incr_input_error();
				parser.error_msg("Expected 0/1 for new_def.", PHRQ_io::OT_CONTINUE);
			}
			else
			{
				this->new_def = (i == 0) ? false : true;
			}
			useLastLine = false;
			break;
		case 8:				// solution_equilibria
			if (!(parser.get_iss() >> i))
			{
				parser.incr_input_error();
				parser.error_msg("Expected 0/1 for solution_equilibria.", PHRQ_io::OT_CONTINUE);
			}
			else
			{
				this->solution_equilibria = (i == 0) ? false : true;
			}
			useLastLine = false;
			break;
		case 9:				// n_solution
			if (!(parser.get_iss() >> this->n_solution))
			{
				parser.incr_input_error();
				parser.error_msg("Expected integer for n_solution.", PHRQ_io::OT_CONTINUE);
			}
			useLastLine = false;
			break;
		case 10:				// total_moles
			if (!(parser.get_iss() >> this->total_moles))
			{
				this->total_moles = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for total_moles.",
								 PHRQ_io::OT_CONTINUE);
			}
			useLastLine = false;
			break;
		case 11:				// temperature
			if (!(parser.get_iss() >> this->temperature))
			{
				this->temperature = 0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for temperature.",
								 PHRQ_io::OT_CONTINUE);
			}
			useLastLine = false;
			break;
		case 12:				// totals
			if (this->totals.read_raw(parser, next_char) !=
				CParser::PARSER_OK)
			{
				parser.incr_input_error();
				parser.
					error_msg
					("Expected element name and molality for GasPhase totals.",
					 PHRQ_io::OT_CONTINUE);
			}
			opt_save = 12;
			break;
		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	if (check)
	{
		// members that must be defined
		if (type_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Type not defined for GAS_PHASE_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (total_p_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Total_p not defined for GAS_PHASE_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (volume_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Volume not defined for GAS_PHASE_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
	}
}

#ifdef USE_MPI
void
cxxGasPhase::mpi_pack(std::vector < int >&ints,
					  std::vector < LDBLE >&doubles)
{
	ints.push_back(this->n_user);
	this->gasPhaseComps.mpi_pack(ints, doubles);
	if (this->type == cxxGasPhase::GP_PRESSURE)
	{
		ints.push_back(0);
	}
	else
	{
		ints.push_back(1);
	}
	doubles.push_back(this->total_p);
	doubles.push_back(this->volume);
	doubles.push_back(this->v_m);
	ints.push_back((pr_in) ? 1 : 0);
}

void
cxxGasPhase::mpi_unpack(int *ints, int *ii, LDBLE *doubles, int *dd)
{
	int i = *ii;
	int d = *dd;
	this->n_user = ints[i++];
	this->n_user_end = this->n_user;
	this->description = " ";
	this->gasPhaseComps.mpi_unpack(ints, &i, doubles, &d);
	int n = ints[i++];
	if (n == 0)
	{
		this->type = cxxGasPhase::GP_PRESSURE;
	}
	else
	{
		this->type = cxxGasPhase::GP_VOLUME;
	}
	this->total_p = doubles[d++];
	this->volume = doubles[d++];
	this->v_m = doubles[d++];
	n = ints[i++];
	this->pr_in = (n == 1) ? true : false;
	*ii = i;
	*dd = d;
}
#endif
void
cxxGasPhase::totalize(Phreeqc * phreeqc_ptr)
{
	this->totals.clear();
	// component structures
	for (size_t i = 0; i < this->gas_comps.size(); i++)
	{
		struct phase *phase_ptr;
		int l;
		phase_ptr = phreeqc_ptr-> phase_bsearch(this->gas_comps[i].Get_phase_name().c_str(), &l, FALSE);
		if (phase_ptr != NULL)
		{
			cxxNameDouble phase_formula(phase_ptr->next_elt);
			this->totals.add_extensive(phase_formula, this->gas_comps[i].Get_moles());
		}
		else
		{
			assert(false);
		}
	}
	return;
}
LDBLE cxxGasPhase::Calc_total_moles(void)const
{
	LDBLE tot = 0.0;
	for (size_t i = 0; i < this->gas_comps.size(); i++)
	{
		tot += gas_comps[i].Get_moles();
	}
	return tot;
}
cxxGasComp *
cxxGasPhase::Find_comp(const char * comp_name)
{
	for (size_t i = 0; i < this->gas_comps.size(); i++)
	{
		if (Utilities::strcmp_nocase(this->gas_comps[i].Get_phase_name().c_str(), comp_name) == 0)
		{
			return &(this->gas_comps[i]);
		}
	}
	return NULL;
}