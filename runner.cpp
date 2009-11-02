#include "runner.h"

runner::runner(void)
{
	this->time_step = 0;
	this->start_time = 0;
}
runner::runner(CParser & parser)
{
	this->time_step = 0;
	this->start_time = 0;
	this->Read(parser);
}

runner::~runner(void)
{
}
bool runner::Read(CParser & parser)
{

	bool return_value(true);
	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(20);
		vopts.push_back("cell");
		vopts.push_back("cells");
		vopts.push_back("start_time");
		vopts.push_back("time_step");
	}

	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;
	bool useLastLine(false);

	this->Get_cells().Set_defined(true);
	opt_save = CParser::OPT_DEFAULT;

	StorageBinListItem item;
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



		// Read dump entity list of numbers or number ranges for line, store in item
		if (opt >= 0 && opt <= 1)
		{

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
			for (;;)
			{ 
				CParser::TOKEN_TYPE j = parser.copy_token(token, next_char);
				if (j == CParser::TT_DIGIT)
				{
					item.Augment(token);
				}
				else if (j == CParser::TT_EMPTY)
				{
					item.Augment(token);
					break;
				}
				else
				{
					parser.error_msg("Expected single number or range of numbers.",
						CParser::OT_CONTINUE);
				}
			}			
			break;
		case 2:				//start_time
			if (!(parser.get_iss() >> this->start_time))
			{
				parser.error_msg("Expected start_time for RUN_CELLS.",
					CParser::OT_CONTINUE);
				parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
			}
			break;
		case 3:				//time_step
			if (!(parser.get_iss() >> this->time_step))
			{
				parser.error_msg("Expected time_step for RUN_CELLS.",
					CParser::OT_CONTINUE);
				parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
			}
			break;
		default:
		case CParser::OPT_DEFAULT:
		case CParser::OPT_ERROR:
			opt = CParser::OPT_EOF;
			parser.error_msg("Unknown input reading RUN_CELLS definition.",
							 CParser::OT_CONTINUE);
			parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
			useLastLine = false;
			return_value = false;
			break;
		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	if (item.Get_numbers().size() > 0)
	{
		this->cells = item;
	}
	return(return_value);
}
