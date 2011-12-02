#include <algorithm>			// std::replace

#include "dumper.h"
#include "Parser.h"

dumper::dumper(PHRQ_io *io)
:
PHRQ_base(io)
{
	this->file_name = "dump.out";
	this->append = false;
	this->on = false;
}
dumper::dumper(CParser & parser, PHRQ_io *io)
:
PHRQ_base(io)
{
	this->file_name = "dump.out";
	this->append = false;
	this->Read(parser);
}

dumper::~dumper(void)
{
}

void dumper::SetAll(bool tf)
{
	this->binList.SetAll(tf);

}bool dumper::Read(CParser & parser)
{
	bool return_value(true);
	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(20);
		vopts.push_back("file");				// 0
		vopts.push_back("append");				// 1
		vopts.push_back("all");					// 2
		vopts.push_back("cell");				// 3
		vopts.push_back("cells");				// 4
		vopts.push_back("solution");			// 5
		vopts.push_back("solutions");			// 6
		vopts.push_back("pp_assemblage");
		vopts.push_back("pp_assemblages");
		vopts.push_back("equilibrium_phase");
		vopts.push_back("equilibrium_phases");	// 10
		vopts.push_back("exchange");
		vopts.push_back("surface");
		vopts.push_back("s_s_assemblage");
		vopts.push_back("solid_solution");
		vopts.push_back("solid_solutions");		// 15
		vopts.push_back("gas_phase");
		vopts.push_back("gas_phases");
		vopts.push_back("kinetics");			// 18
		vopts.push_back("mix");					// 19
		vopts.push_back("reaction");			// 20
		vopts.push_back("reactions");			// 21
		vopts.push_back("temperature");			// 22
		vopts.push_back("reaction_temperature");	// 23
		vopts.push_back("reaction_temperatures");   // 24
		vopts.push_back("pressure");			// 25
		vopts.push_back("reaction_pressure");	// 26
		vopts.push_back("reaction_pressures");	// 27
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
		StorageBinListItem cells;
		opt = parser.get_option(vopts, next_char);
		if (opt == CParser::OPT_DEFAULT)
		{
			opt = opt_save;
		}
		else
		{
			opt_save = opt;
		}

		// Select StorageBinListItem
		StorageBinListItem *item = NULL;
		switch (opt)
		{
		case 3:					// cell
		case 4:					// cells
			item = &cells;
			break;
		case 5:
		case 6:
			item = &(this->binList.Get_solution());
			break;
		case 7:
		case 8:
		case 9:
		case 10:
			item = &(this->binList.Get_pp_assemblage());
			break;
		case 11:
			item = &(this->binList.Get_exchange());
			break;
		case 12:
			item = &(this->binList.Get_surface());
			break;
		case 13:
		case 14:
		case 15:
			item = &(this->binList.Get_s_s_assemblage());
			break;
		case 16:
		case 17:
			item = &(this->binList.Get_gas_phase());
			break;
		case 18:
			item = &(this->binList.Get_kinetics());
			break;

		case 19:	// mix
			item = &(this->binList.Get_mix());
			break;
		case 20:	// reaction
		case 21:	// reactions
			item = &(this->binList.Get_reaction());
			break;
		case 22:	// temperature
		case 23:	// reaction_temperature
		case 24:    // reaction_temperatures
			item = &(this->binList.Get_temperature());
			break;
		case 25:	// pressure
		case 26:	// reaction_pressure
		case 27:    // reaction_pressures
			item = &(this->binList.Get_pressure());
			break;
		default:
			break;
		}

		// Read dump entity list of numbers or number ranges for line, store in item
		if ((opt > 2))
		{
			for (;;)
			{ 
				CParser::TOKEN_TYPE j = parser.copy_token(token, next_char);
				if (j == CParser::TT_DIGIT)
				{
					item->Augment(token);
				}
				else if (j == CParser::TT_EMPTY)
				{
					item->Augment(token);
					break;
				}
				else
				{
					parser.error_msg("Expected single number or range of numbers.",
						CParser::OT_CONTINUE);
				}
			}
		}

		if (opt == 3 || opt == 4)
		{
			this->binList.TransferAll(cells);
		}
		// Process other identifiers
		std::set < int >::iterator it;
		switch (opt)
		{
		case CParser::OPT_EOF:
			break;
		case CParser::OPT_KEYWORD:
			break;
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
			break;
		case 0:				//file
			std::getline(parser.get_iss(), this->file_name);
			this->file_name = trim(this->file_name, " \t");
			if (this->file_name.size() == 0)
			{
				this->file_name = "dump.out";
			}

			break;
		case 1:				//append
			{
				parser.copy_token(token, next_char);
				//if (!(parser.get_iss() >> this->append))
				this->append = true;
				if (token.c_str()[0] == 'f' || token.c_str()[0] == 'F')
				{
					this->append = false;
				}
			}
			break;
		case 2:			//all
			this->SetAll(true);
			break;
		default:
		case CParser::OPT_DEFAULT:
		case CParser::OPT_ERROR:
			opt = CParser::OPT_EOF;
			parser.error_msg("Unknown input reading DUMP definition.",
							 CParser::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
			useLastLine = false;
			return_value = false;
			break;
		}
		
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	return(return_value);
}

#ifdef SKIP
bool dumper::Read(CParser & parser)
{

	bool return_value(true);
	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(20);
		vopts.push_back("solution");
		vopts.push_back("solutions");
		vopts.push_back("pp_assemblage");
		vopts.push_back("pp_assemblages");
		vopts.push_back("equilibrium_phase");
		vopts.push_back("equilibrium_phases");
		vopts.push_back("exchange");
		vopts.push_back("surface");
		vopts.push_back("s_s_assemblage");
		vopts.push_back("solid_solution");
		vopts.push_back("solid_solutions");
		vopts.push_back("gas_phase");
		vopts.push_back("gas_phases");
		vopts.push_back("kinetics");
		vopts.push_back("file");
		vopts.push_back("append");
		vopts.push_back("all");	
		vopts.push_back("cell");
		vopts.push_back("cells");			// 18
		vopts.push_back("mix");				// 19
		vopts.push_back("reaction");		// 20
		vopts.push_back("reactions");		// 21
		vopts.push_back("temperature");		// 22
		vopts.push_back("reaction_temperature");	// 23
		vopts.push_back("reaction_temperatures");   // 24
		vopts.push_back("pressure");		// 25
		vopts.push_back("reaction_pressure");	// 26
		vopts.push_back("reaction_pressures");   // 27
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
		StorageBinListItem cells;
		opt = parser.get_option(vopts, next_char);
		if (opt == CParser::OPT_DEFAULT)
		{
			opt = opt_save;
		}
		else
		{
			opt_save = opt;
		}

		// Select StorageBinListItem
		StorageBinListItem *item = NULL;
		switch (opt)
		{
		case 0:
		case 1:
			item = &(this->binList.Get_solution());
			break;
		case 2:
		case 3:
		case 4:
		case 5:
			item = &(this->binList.Get_pp_assemblage());
			break;
		case 6:
			item = &(this->binList.Get_exchange());
			break;
		case 7:
			item = &(this->binList.Get_surface());
			break;
		case 8:
		case 9:
		case 10:
			item = &(this->binList.Get_s_s_assemblage());
			break;
		case 11:
		case 12:
			item = &(this->binList.Get_gas_phase());
			break;
		case 13:
			item = &(this->binList.Get_kinetics());
			break;
		case 17:	// cell
		case 18:	// cells
			item = &cells;
			break;
		case 19:	// mix
			item = &(this->binList.Get_mix());
			break;
		case 20:	// reaction
		case 21:	// reactions
			item = &(this->binList.Get_reaction());
			break;
		case 22:	// temperature
		case 23:	// reaction_temperature
		case 24:    // reaction_temperatures
			item = &(this->binList.Get_temperature());
			break;
		case 25:	// pressure
		case 26:	// reaction_pressure
		case 27:    // reaction_pressures
			item = &(this->binList.Get_pressure());
			break;
		default:
			break;
		}

		// Read dump entity list of numbers or number ranges for line, store in item
		if ((opt >= 0 && opt <= 13) || (opt >= 17 && opt <= 28))
		{
			for (;;)
			{ 
				CParser::TOKEN_TYPE j = parser.copy_token(token, next_char);
				if (j == CParser::TT_DIGIT)
				{
					item->Augment(token);
				}
				else if (j == CParser::TT_EMPTY)
				{
					item->Augment(token);
					break;
				}
				else
				{
					parser.error_msg("Expected single number or range of numbers.",
						CParser::OT_CONTINUE);
				}
			}
		}

		if (opt == 17 || opt == 18)
		{
			this->binList.TransferAll(cells);
		}
		// Process other identifiers
		std::set < int >::iterator it;
		switch (opt)
		{
		case CParser::OPT_EOF:
			break;
		case CParser::OPT_KEYWORD:
			break;

		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
			break;
		case 14:				//file
			std::getline(parser.get_iss(), this->file_name);
			this->file_name = trim(this->file_name, " \t");
			if (this->file_name.size() == 0)
			{
				this->file_name = "dump.out";
			}

			break;
		case 15:				//append
			{
				parser.copy_token(token, next_char);
				//if (!(parser.get_iss() >> this->append))
				this->append = true;
				if (token.c_str()[0] == 'f' || token.c_str()[0] == 'F')
				{
					this->append = false;
				}
			}
			break;
		case 16:			//all
			this->SetAll(true);
			break;
		default:
		case CParser::OPT_DEFAULT:
		case CParser::OPT_ERROR:
			opt = CParser::OPT_EOF;
			parser.error_msg("Unknown input reading DUMP definition.",
							 CParser::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
			useLastLine = false;
			return_value = false;
			break;
		}

		
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	return(return_value);
}
#endif
bool dumper::Get_bool_any(void)
{
	return (
		Get_bool_solution()			||
		Get_bool_pp_assemblage()	||
		Get_bool_exchange()			||
		Get_bool_surface()			||
		Get_bool_s_s_assemblage()	||
		Get_bool_gas_phase()		||
		Get_bool_kinetics()			||
		Get_bool_mix()				||
		Get_bool_reaction()			||
		Get_bool_temperature()		||
		Get_bool_pressure()
		);
}
