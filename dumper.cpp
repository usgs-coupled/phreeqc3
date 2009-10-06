#include "dumper.h"
#include <algorithm>			// std::replace

dumper::dumper(void)
{
	this->file_name = "dump.out";
	this->append = false;
	this->dump_solution = false;
	this->dump_pp_assemblage = false;
	this->dump_exchange = false;
	this->dump_surface = false;
	this->dump_s_s_assemblage = false;
	this->dump_gas_phase = false;
	this->dump_kinetics = false;
}
dumper::dumper(CParser & parser)
{
	this->file_name = "dump.out";
	this->append = false;

	this->dump_solution = false;
	this->dump_pp_assemblage = false;
	this->dump_exchange = false;
	this->dump_surface = false;
	this->dump_s_s_assemblage = false;
	this->dump_gas_phase = false;
	this->dump_kinetics = false;
	this->Read(parser);
}

dumper::~dumper(void)
{
}

void dumper::DumpAll(bool tf)
{
	this->solution.clear();
	this->pp_assemblage.clear();
	this->exchange.clear();
	this->surface.clear();
	this->s_s_assemblage.clear();
	this->gas_phase.clear();
	this->kinetics.clear();

	if (tf)
	{
		this->dump_solution = true;
		this->dump_pp_assemblage = true;
		this->dump_exchange = true;
		this->dump_surface = true;
		this->dump_s_s_assemblage = true;
		this->dump_gas_phase = true;
		this->dump_kinetics = true;
	}
	else
	{
		this->dump_solution = false;
		this->dump_pp_assemblage = false;
		this->dump_exchange = false;
		this->dump_surface = false;
		this->dump_s_s_assemblage = false;
		this->dump_gas_phase = false;
		this->dump_kinetics = false;
	}
}
bool dumper::Read(CParser & parser)
{

	bool return_value(true);
	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(15);
		vopts.push_back("solution");
		vopts.push_back("pp_assemblage");
		vopts.push_back("equilibrium_phases");
		vopts.push_back("exchange");
		vopts.push_back("surface");
		vopts.push_back("s_s_assemblage");
		vopts.push_back("gas_phase");
		vopts.push_back("kinetics");
		vopts.push_back("file");
		vopts.push_back("append");
		vopts.push_back("all");		
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;
	bool useLastLine(false);

	// Read mix number and description
	//this->read_number_description(parser);

	opt_save = CParser::OPT_DEFAULT;

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
		else
		{
			opt_save = opt;
		}
		std::set < int > accumulator;

		// Read list of numbers or number ranges for line
		if (opt >= 0 && opt <= 7)
		{
			for (;;)
			{ 
				CParser::TOKEN_TYPE j = parser.copy_token(token, next_char);
				std::set < int > temp_set;
				int i; 
				if (j == CParser::TT_EMPTY)
				{
					break;
				}
				else if (j == CParser::TT_DIGIT)
				{
					std::replace(token.begin(), token.end(), '-', ' ');
					//if (token.find_last_of("-") != token.end())
					//{
					//	token.replace(token.find_last_of("-"), token.find_last_of("-") + 1, " ");
					//}

					std::istringstream iss(token);
					if (iss >> i)
					{
						temp_set.insert(i);
						if (iss >> i)
						{
							temp_set.insert(i);
						}
					}
					if (temp_set.size() == 1)
					{
						accumulator.insert(*(temp_set.begin()));
					}
					else if (temp_set.size() == 2)
					{
						int i1, i2;
						std::set <int>::iterator it;
						it = temp_set.begin();
						i1 = *it;
						it++;
						i2 = *it;
						for (i = i1; i <= i2; i++)
						{
							accumulator.insert(i);
						}
					}
					else
					{
						parser.error_msg("Expected positive integers for dump range.",
							CParser::OT_CONTINUE);
					}
				}
				else
				{
					parser.error_msg("Expected single number or range of numbers.",
						CParser::OT_CONTINUE);
				}
			}
		}

		// Process identifiers
		std::set < int >::iterator it;
		switch (opt)
		{
		case CParser::OPT_EOF:
			break;
		case CParser::OPT_KEYWORD:
			break;
		case CParser::OPT_DEFAULT:
		case CParser::OPT_ERROR:
			opt = CParser::OPT_EOF;
			parser.error_msg("Unknown input reading DUMP definition.",
							 CParser::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
			useLastLine = false;
			return_value = false;
			break;
		case 0:
			for (it = accumulator.begin(); it != accumulator.end(); it++)
			{
				this->solution.insert(*it);
			}
			this->dump_solution = true;
			break;
		case 1:
		case 2:
			for (it = accumulator.begin(); it != accumulator.end(); it++)
			{
				this->pp_assemblage.insert(*it);
			}
			this->dump_pp_assemblage = true;
			break;
		case 3:
			for (it = accumulator.begin(); it != accumulator.end(); it++)
			{
				this->exchange.insert(*it);
			}
			this->dump_exchange = true;
			break;
		case 4:
			for (it = accumulator.begin(); it != accumulator.end(); it++)
			{
				this->surface.insert(*it);
			}
			this->dump_surface = true;
			break;
		case 5:
			for (it = accumulator.begin(); it != accumulator.end(); it++)
			{
				this->s_s_assemblage.insert(*it);
			}			this->dump_s_s_assemblage = true;
			break;
		case 6:
			for (it = accumulator.begin(); it != accumulator.end(); it++)
			{
				this->gas_phase.insert(*it);
			}			this->dump_gas_phase = true;
			break;
		case 7:
			for (it = accumulator.begin(); it != accumulator.end(); it++)
			{
				this->kinetics.insert(*it);
			}
			this->dump_kinetics = true;
			break;
		case 8:				//file
			std::getline(parser.get_iss(), this->file_name);
			this->file_name = trim(this->file_name, " \t");
			if (this->file_name.size() == 0)
			{
				this->file_name = "dump.out";
			}

			break;
		case 9:				//append
			{
				CParser::TOKEN_TYPE j = parser.copy_token(token, next_char);
				//if (!(parser.get_iss() >> this->append))
				this->append = true;
				if (token.c_str()[0] == 'f' || token.c_str()[0] == 'F')
				{
					this->append = false;
				}
			}
			break;
		case 10:			//all
			this->DumpAll(true);
			break;
		}

		
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	return(return_value);
}
