// Kinetics.cxx: implementation of the cxxKinetics class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include <cassert>				// assert
#include <algorithm>			// std::sort

#include "Utils.h"				// define first
#include "Phreeqc.h"

#include "cxxKinetics.h"
#include "cxxMix.h"

#include "phqalloc.h"

#include "PHRQ_io.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

cxxKinetics::cxxKinetics(PHRQ_io *io)
	//
	// default constructor for cxxKinetics 
	//
:	cxxNumKeyword(io)
{
	step_divide = 1.0;
	rk = 3;
	bad_step_max = 500;
	use_cvode = false;
	cvode_steps = 100;
	cvode_order = 5;
	totals.type = cxxNameDouble::ND_ELT_MOLES;
	equal_steps = 0;
}

cxxKinetics::cxxKinetics(struct kinetics *kinetics_ptr, PHRQ_io *io)
		//
		// constructor for cxxKinetics from struct kinetics
		//
	:
cxxNumKeyword(io),
totals(kinetics_ptr->totals)
{
	int i;

	this->Set_description(kinetics_ptr->description);
	n_user = kinetics_ptr->n_user;
	n_user_end = kinetics_ptr->n_user_end;
	step_divide = kinetics_ptr->step_divide;
	rk = kinetics_ptr->rk;
	bad_step_max = kinetics_ptr->bad_step_max;
	use_cvode = (kinetics_ptr->use_cvode == TRUE);
	cvode_steps = kinetics_ptr->cvode_steps;
	cvode_order = kinetics_ptr->cvode_order;

	// kinetics components
	for (i = 0; i < kinetics_ptr->count_comps; i++)
	{
		cxxKineticsComp ec(&(kinetics_ptr->comps[i]), this->Get_io());
		std::string str(ec.Get_rate_name());
		//this->kineticsComps[str] = ec;
		this->kineticsComps.push_back(ec);
	}

	// steps
	int n = kinetics_ptr->count_steps;
	this->equal_steps = 0;
	if (kinetics_ptr->count_steps < 0)
	{
		this->equal_steps = -kinetics_ptr->count_steps;
		n = 1;
	}
	for (i = 0; i < n; i++)
	{
		this->steps.push_back(kinetics_ptr->steps[i]);
	}
}
cxxKinetics::cxxKinetics(const std::map < int, cxxKinetics > &entities,
						 cxxMix & mix, int l_n_user, PHRQ_io *io):
cxxNumKeyword(io)
{
	this->n_user = this->n_user_end = l_n_user;
	step_divide = 1.0;
	rk = 3;
	bad_step_max = 500;
	use_cvode = false;
	cvode_steps = 100;
	cvode_order = 5;
	totals.type = cxxNameDouble::ND_ELT_MOLES;
//
//   Mix
//
	const std::map < int, LDBLE >&mixcomps = mix.Get_mixComps();
	std::map < int, LDBLE >::const_iterator it;
	for (it = mixcomps.begin(); it != mixcomps.end(); it++)
	{
		if (entities.find(it->first) != entities.end())
		{
			const cxxKinetics *entity_ptr =
				&(entities.find(it->first)->second);
			this->add(*entity_ptr, it->second);
		}
	}
}

cxxKinetics::~cxxKinetics()
{
}

#ifdef SKIP
void
cxxKinetics::dump_xml(std::ostream & s_oss, unsigned int indent) const const
{
	//const char    ERR_MESSAGE[] = "Packing kinetics message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// Kinetics element and attributes
	s_oss << indent0;
	s_oss << "<kinetics " << "\n";

	s_oss << indent1;
	s_oss << "pitzer_kinetics_gammas=\"" << this->
		pitzer_kinetics_gammas << "\"" << "\n";

	// components
	s_oss << indent1;
	s_oss << "<component " << "\n";
	for (std::list < cxxKineticsComp >::const_iterator it =
		 kineticsComps.begin(); it != kineticsComps.end(); ++it)
	{
		it->dump_xml(s_oss, indent + 2);
	}

	return;
}
#endif

void
cxxKinetics::dump_raw(std::ostream & s_oss, unsigned int indent, int * n_out) const
{
	//const char    ERR_MESSAGE[] = "Packing kinetics message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// Kinetics element and attributes
	s_oss << indent0;
	int n_user_local = (n_out != NULL) ? *n_out : this->n_user;
	s_oss << "KINETICS_RAW       " << n_user_local << " " << this->description << "\n";

	s_oss << indent1;
	s_oss << "-step_divide       " << this->step_divide << "\n";

	s_oss << indent1;
	s_oss << "-rk                " << this->rk << "\n";

	s_oss << indent1;
	s_oss << "-bad_step_max      " << this->bad_step_max << "\n";

	s_oss << indent1;
	s_oss << "-use_cvode         " << this->use_cvode << "\n";

	s_oss << indent1;
	s_oss << "-cvode_steps       " << this->cvode_steps << "\n";

	s_oss << indent1;
	s_oss << "-cvode_order       " << this->cvode_order << "\n";

	// kineticsComps structures
	for (std::list < cxxKineticsComp >::const_iterator it =
		 kineticsComps.begin(); it != kineticsComps.end(); ++it)
	{
		s_oss << indent1;
		s_oss << "-component" << "\n";
		(*it).dump_raw(s_oss, indent + 2);
	}

	// totals
	s_oss << indent1;
	s_oss << "-totals         " << "\n";
	this->totals.dump_raw(s_oss, indent + 2);

	// equal_steps
	s_oss << indent1;
	s_oss << "-equal_steps    " << this->equal_steps << "\n";

	// steps
	s_oss << indent1;
	s_oss << "-steps         " << "\n";
	{
		int i = 0;
		s_oss << indent2;
		for (std::vector < LDBLE >::const_iterator it = this->steps.begin();
			 it != this->steps.end(); it++)
		{
			if (i++ == 5)
			{
				s_oss << "\n";
				s_oss << indent2;
				i = 0;
			}
			s_oss << *it << " ";
		}
		s_oss << "\n";
	}
	return;
}

void
cxxKinetics::read_raw(CParser & parser, bool check)
{

	LDBLE d;
	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(15);
		vopts.push_back("step_divide");
		vopts.push_back("rk");
		vopts.push_back("bad_step_max");
		vopts.push_back("use_cvode");
		vopts.push_back("component");
		vopts.push_back("totals");
		vopts.push_back("steps");
		vopts.push_back("cvode_steps");
		vopts.push_back("cvode_order");
		vopts.push_back("equal_steps");
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;
	bool useLastLine(false);
	std::vector < LDBLE > temp_steps;

	// Read kinetics number and description
	this->read_number_description(parser);

	opt_save = CParser::OPT_ERROR;
	bool step_divide_defined(false);
	bool rk_defined(false);
	bool bad_step_max_defined(false);
	bool use_cvode_defined(false);
	bool cvode_steps_defined(false);
	bool cvode_order_defined(false);
	bool steps_defined(false);
	bool equal_steps_defined(false);

	for (;;)
	{
		int opt;
		if (useLastLine == false)
		{
			opt = parser.get_option(vopts, next_char);
		}
		else
		{
			opt = parser.getOptionFromLastLine(vopts, next_char);
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
			parser.error_msg("Unknown input in KINETICS_COMP_RAW keyword.",
							 PHRQ_io::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), PHRQ_io::OT_CONTINUE);
			useLastLine = false;
			break;

		case 0:				// step_divide
			if (!(parser.get_iss() >> this->step_divide))
			{
				this->step_divide = 1.0;
				parser.incr_input_error();
				parser.error_msg("Expected numeric value for step_divide.",
								 PHRQ_io::OT_CONTINUE);
			}
			step_divide_defined = true;
			useLastLine = false;
			break;

		case 1:				// rk
			if (!(parser.get_iss() >> this->rk))
			{
				this->rk = 3;
				parser.incr_input_error();
				parser.error_msg("Expected integer value for rk.",
								 PHRQ_io::OT_CONTINUE);
			}
			rk_defined = true;
			useLastLine = false;
			break;

		case 2:				// bad_step_max
			if (!(parser.get_iss() >> this->bad_step_max))
			{
				this->bad_step_max = 500;
				parser.incr_input_error();
				parser.error_msg("Expected integer value for bad_step_max.",
								 PHRQ_io::OT_CONTINUE);
			}
			bad_step_max_defined = true;
			useLastLine = false;
			break;

		case 3:				// use_cvode
			if (!(parser.get_iss() >> this->use_cvode))
			{
				this->use_cvode = false;
				parser.incr_input_error();
				parser.error_msg("Expected boolean value for use_cvode.",
								 PHRQ_io::OT_CONTINUE);
			}
			use_cvode_defined = true;
			useLastLine = false;
			break;

		case 4:				// component
			{
				cxxKineticsComp ec(this->Get_io());

				// preliminary read
				parser.set_accumulate(true);
				ec.read_raw(parser, false);
				parser.set_accumulate(false);
				std::istringstream is(parser.get_accumulated());
				CParser reread(is, this->Get_io());
				reread.set_echo_file(CParser::EO_NONE);
				reread.set_echo_stream(CParser::EO_NONE);

				std::list < cxxKineticsComp >::iterator kit;
				bool found = false;
				for (kit = this->kineticsComps.begin(); kit != this->kineticsComps.end(); kit++)
				{
					if (kit->Get_rate_name() == ec.Get_rate_name())
					{
						found = true;
						break;
					}
				}
				if (found)
				{
					kit->read_raw(reread, false);
				}
				else
				{
					cxxKineticsComp ec1(this->Get_io());
					ec1.read_raw(reread, false);
					std::string str(ec1.Get_rate_name());
					this->kineticsComps.push_back(ec1);
				}
				/*
				if (this->kineticsComps.find(ec.get_rate_name()) != this->kineticsComps.end())
				{
					cxxKineticsComp & comp = this->kineticsComps.find(ec.get_rate_name())->second;
					comp.read_raw(reread, false);
				}
				else
				{
					cxxKineticsComp ec1;
					ec1.read_raw(reread, false);
					std::string str(ec1.get_rate_name());
					this->kineticsComps[str] = ec1;
				}
				*/
			}
			useLastLine = true;
			break;

		case 5:				// totals
			if (this->totals.read_raw(parser, next_char) !=
				CParser::PARSER_OK)
			{
				parser.incr_input_error();
				parser.
					error_msg
					("Expected element name and molality for KineticsComp totals.",
					 PHRQ_io::OT_CONTINUE);
			}
			opt_save = 5;
			useLastLine = false;
			break;

		case 6:				// steps
			while (parser.copy_token(token, next_char) == CParser::TT_DIGIT)
			{
				//sscanf(token.c_str(), "%lf", &d);
				//this->steps.push_back(d);
				std::istringstream iss(token);
				if (!(iss >> d))
				{
					parser.incr_input_error();
					parser.error_msg("Expected numeric value for steps.",
									 PHRQ_io::OT_CONTINUE);
				}
				else
				{
					temp_steps.push_back(d);
					steps_defined = true;
				}
			}
			opt_save = 6;
			useLastLine = false;
			break;

		case 7:				// cvode_steps
			if (!(parser.get_iss() >> this->cvode_steps))
			{
				this->cvode_steps = 100;
				parser.incr_input_error();
				parser.error_msg("Expected integer value for cvode_steps.",
								 PHRQ_io::OT_CONTINUE);
			}
			cvode_steps_defined = true;
			useLastLine = false;
			break;

		case 8:				// cvode_order
			if (!(parser.get_iss() >> this->cvode_order))
			{
				this->cvode_order = 5;
				parser.incr_input_error();
				parser.error_msg("Expected integer value for cvode_order.",
								 PHRQ_io::OT_CONTINUE);
			}
			cvode_order_defined = true;
			useLastLine = false;
			break;
		case 9:				// equal steps
			if (!(parser.get_iss() >> this->equal_steps))
			{
				this->equal_steps = 0;
				parser.incr_input_error();
				parser.error_msg("Expected integer value for equal_steps.",
								 PHRQ_io::OT_CONTINUE);
			}
			equal_steps_defined = true;
			useLastLine = false;
			break;

		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	if (steps_defined)
	{
		this->steps = temp_steps;
	}
	if (check)
	{
		// members that must be defined
		if (step_divide_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Step_divide not defined for KINETICS_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (rk_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Rk not defined for KINETICS_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (bad_step_max_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Bad_step_max not defined for KINETICS_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (use_cvode_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Use_cvode not defined for KINETICS_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (cvode_steps_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Cvode_steps not defined for KINETICS_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		if (cvode_order_defined == false)
		{
			parser.incr_input_error();
			parser.error_msg("Cvode_order not defined for KINETICS_RAW input.",
				PHRQ_io::OT_CONTINUE);
		}
		// disable to remain backward compatible with PHAST restart files?
		//if (equal_steps_defined == false)
		//{
		//	parser.incr_input_error();
		//	parser.error_msg("Equal_steps not defined for KINETICS_RAW input.",
		//		PHRQ_io::OT_CONTINUE);
		//}
	}
}

#ifdef USE_MPI
void
cxxKinetics::mpi_pack(std::vector < int >&ints,
					  std::vector < LDBLE >&doubles)
{
	ints.push_back(this->n_user);
	ints.push_back((int) this->kineticsComps.size());
	std::list < cxxKineticsComp >::iterator it;
	for (it = this->kineticsComps.begin(); it != this->kineticsComps.end(); it++)
	{
		(*it).mpi_pack(ints, doubles);
	}
	/*
	for (std::map < std::string, cxxKineticsComp >::iterator it =
		 this->kineticsComps.begin(); it != this->kineticsComps.end(); it++)
	{
		(*it).second.mpi_pack(ints, doubles);
	}
	*/
	ints.push_back((int) this->steps.size());
	for (std::vector < LDBLE >::iterator it = this->steps.begin();
		 it != this->steps.end(); it++)
	{
		doubles.push_back(*it);
	}
	doubles.push_back(this->step_divide);
	ints.push_back(this->rk);
	ints.push_back(this->bad_step_max);
	ints.push_back(this->use_cvode);
	ints.push_back(this->cvode_steps);
	ints.push_back(this->cvode_order);
	ints.push_back(this->equal_steps);
}

void
cxxKinetics::mpi_unpack(int *ints, int *ii, LDBLE *doubles, int *dd)
{
	int i = *ii;
	int d = *dd;
	this->n_user = ints[i++];
	this->n_user_end = this->n_user;
	this->description = " ";

	int n = ints[i++];
	this->kineticsComps.clear();
	for (int j = 0; j < n; j++)
	{
		cxxKineticsComp kc;
		kc.mpi_unpack(ints, &i, doubles, &d);
		//std::string str(kc.get_rate_name());
		//this->kineticsComps[str] = kc;
		this->kineticsComps.push_back(kc);
	}
	n = ints[i++];
	this->steps.clear();
	for (int j = 0; j < n; j++)
	{
		this->steps.push_back(doubles[d++]);
	}
	this->step_divide = doubles[d++];
	this->rk = ints[i++];
	this->bad_step_max = ints[i++];
	this->use_cvode = (ints[i++] == TRUE);
	this->cvode_steps = ints[i++];
	this->cvode_order = ints[i++];
	this->equal_steps = ints[i++];
	*ii = i;
	*dd = d;
}
#endif
void
cxxKinetics::add(const cxxKinetics & addee, LDBLE extensive)
		//
		// Add to existing ppassemblage to "this" ppassemblage
		//
{
	if (extensive == 0.0)
		return;
	//std::map < std::string, cxxKineticsComp> kineticsComps;
	for (std::list < cxxKineticsComp >::const_iterator itadd =
		 addee.kineticsComps.begin(); itadd != addee.kineticsComps.end();
		 ++itadd)
	{
		bool found(false);
		std::list <  cxxKineticsComp >::iterator it;
		for (it = this->kineticsComps.begin(); it != this->kineticsComps.end(); it++)
		{
			if ((*it).Get_rate_name() == (*itadd).Get_rate_name())
			{
				found = true;
				break;
			}
		}

		//std::map < std::string, cxxKineticsComp >::iterator it = this->kineticsComps.find((*itadd).first);
		if (found)
		{
			(*it).add((*itadd), extensive);
		}
		else
		{
			cxxKineticsComp entity = (*itadd);
			entity.multiply(extensive);
			this->kineticsComps.push_back(entity);
		}
	}
	//std::vector<LDBLE> steps;
	this->steps = addee.steps;
	//cxxNameDouble totals;
	//LDBLE step_divide;
	this->step_divide = addee.step_divide;
	//int rk;
	this->rk = addee.rk;
	//int bad_step_max;
	this->bad_step_max = addee.bad_step_max;
	//bool use_cvode;
	this->use_cvode = addee.use_cvode;
	this->cvode_steps = addee.cvode_steps;
	this->cvode_order = addee.cvode_order;
	this->equal_steps = addee.equal_steps;
}
//cxxNameDouble & cxxKinetics::get_totals(void)
//{
//	return this->totals;
//}
