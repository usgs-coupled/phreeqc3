#include <algorithm>			// std::replace
#include "StorageBinList.h"
#include "Parser.h"

StorageBinListItem::StorageBinListItem(void)
{
	this->defined = false;
}

StorageBinListItem::~StorageBinListItem(void)
{
}
StorageBinListItem::StorageBinListItem(CParser & parser)
{
	this->Clear();
	// Read list of numbers or number ranges 
	for (;;)
	{
		//read lines
		PHRQ_io::LINE_TYPE l = parser.check_line("read StorageBinListLtem", false, true, true, true);
		std::istream::pos_type next_char = 0;
		if (l == PHRQ_io::LT_EOF) break;
		for (;;)
		{ 
			std::string token;
			CParser::TOKEN_TYPE j = parser.copy_token(token, next_char);
			if (j == CParser::TT_DIGIT)
			{
				this->Augment(token);
			}
			else if (j == CParser::TT_EMPTY)
			{
				break;
			}
		}
	}
}
void StorageBinListItem::Augment(std::string token)
{
	this->defined = true;
	if (token.size() == 0) return;

	// split string accounting for possible negative numbers
	size_t pos;
	if ((pos = token.find("--")) != std::string::npos)
	{
		token.replace(pos,2," &");
	}
	std::replace(token.begin() + 1, token.end(), '-', ' ');
	std::replace(token.begin() + 1, token.end(), '&', '-');

	// parse string into 1 or 2 numbers
	std::istringstream iss(token);
	std::set < int > temp_set;
	int i;
	if (iss >> i)
	{
		// add first 
		temp_set.insert(i);
		if (iss >> i)
		{
			// add second if defined
			temp_set.insert(i);
		}
	}

	// add single number or range to StorageBinListItem
	if (temp_set.size() == 1)
	{
		this->numbers.insert(*(temp_set.begin()));
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
			this->numbers.insert(i);
		}
	}
}
void StorageBinListItem::Augment(int i)
{
	// Skip if all are defined
	if (this->defined == true && this->numbers.size() ==  0) return;
	this->defined = true;
	this->numbers.insert(i);
}
//
//Class definitions for StorageBinList
//
StorageBinList::StorageBinList(PHRQ_io *io)
:
PHRQ_base(io)
{
}
StorageBinList::StorageBinList(CParser & parser, PHRQ_io *io)
:
PHRQ_base(io)
{
	this->Read(parser);
}
StorageBinList::~StorageBinList(void)
{
}

void StorageBinList::SetAll(bool tf)
{
	this->solution.Clear();
	this->pp_assemblage.Clear();
	this->exchange.Clear();
	this->surface.Clear();
	this->ss_assemblage.Clear();
	this->gas_phase.Clear();
	this->kinetics.Clear();
	this->mix.Clear();
	this->reaction.Clear();
	this->temperature.Clear();
	this->pressure.Clear();

	this->solution.Set_defined(tf);
	this->pp_assemblage.Set_defined(tf);
	this->exchange.Set_defined(tf);
	this->surface.Set_defined(tf);
	this->ss_assemblage.Set_defined(tf);
	this->gas_phase.Set_defined(tf);
	this->kinetics.Set_defined(tf);
	this->mix.Set_defined(tf);
	this->reaction.Set_defined(tf);
	this->temperature.Set_defined(tf);
	this->pressure.Set_defined(tf);
}

bool StorageBinList::Read(CParser & parser)
{
	bool return_value(true);
#if defined(STATIC_VOPTS)
	static std::vector < std::string > vopts;
#else
	std::vector < std::string > vopts;
#endif
	if (vopts.empty())
	{
		vopts.reserve(20);
		vopts.push_back("solution");
		vopts.push_back("pp_assemblage");
		vopts.push_back("equilibrium_phases");
		vopts.push_back("exchange");
		vopts.push_back("surface");
		vopts.push_back("ss_assemblage");
		vopts.push_back("solid_solution");
		vopts.push_back("solid_solutions");
		vopts.push_back("gas_phase");
		vopts.push_back("kinetics");
		vopts.push_back("mix");
		vopts.push_back("reaction");
		vopts.push_back("temperature");	
		vopts.push_back("all");    // 13
		vopts.push_back("cell");
		vopts.push_back("cells");  // 15
		vopts.push_back("reaction_temperature");
		vopts.push_back("pressure");			//17	
		vopts.push_back("reaction_pressure");	//18	
	}

	std::istream::pos_type next_char;
	std::string token;
	int opt_save;

	bool useLastLine(false);
	opt_save = CParser::OPT_DEFAULT;

	for (;;)
	{
		int opt;
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
		StorageBinListItem cell_list;
		switch (opt)
		{
		case 0:
			item = &(this->Get_solution());
			break;
		case 1:
		case 2:
			item = &(this->Get_pp_assemblage());
			break;
		case 3:
			item = &(this->Get_exchange());
			break;
		case 4:
			item = &(this->Get_surface());
			break;
		case 5:
		case 6:
		case 7:
			item = &(this->Get_ss_assemblage());
			break;
		case 8:
			item = &(this->Get_gas_phase());
			break;
		case 9:
			item = &(this->Get_kinetics());
			break;
		case 10:
			item = &(this->Get_mix());
			break;
		case 11:
			item = &(this->Get_reaction());
			break;
		case 12:
		case 16:
			item = &(this->Get_temperature());
			break;
		case 14:
		case 15:
			item = &cell_list;
			break;
		case 17:
		case 18:
			item = &(this->Get_pressure());
			break;
		default:
			break;
		}

		// Read dump entity list of numbers or number ranges for line, store in item
		if ((opt >= 0 && opt <= 12) || (opt >= 14))
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
						PHRQ_io::OT_CONTINUE);
					break;
				}
			}
		}

		// Process other identifiers
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
		case 16:
		case 17:
		case 18:
			break;
		case 13:			//all
			this->SetAll(true);
			break;
		case 14:
		case 15:
			this->TransferAll(cell_list);
			break;
		default:
		case CParser::OPT_DEFAULT:
		case CParser::OPT_ERROR:
			opt = CParser::OPT_EOF;
			parser.error_msg("Unknown input reading DELETE definition.",
							 PHRQ_io::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), PHRQ_io::OT_CONTINUE);
			useLastLine = false;
			return_value = false;
			break;
		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	return(return_value);
}

void StorageBinList::TransferAll(StorageBinListItem &source)
{
	std::set < int >::iterator it;
	for (it = source.Get_numbers().begin(); it != source.Get_numbers().end(); it++)
	{
		this->solution.Augment(*it);
		this->pp_assemblage.Augment(*it);
		this->exchange.Augment(*it);
		this->surface.Augment(*it);
		this->ss_assemblage.Augment(*it);
		this->gas_phase.Augment(*it);
		this->kinetics.Augment(*it);
		this->mix.Augment(*it);
		this->reaction.Augment(*it);
		this->temperature.Augment(*it);
		this->pressure.Augment(*it);
	}
}
