// ChartHandler.cpp: implementation of the ChartHandler class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include "ChartHandler.h"
#include <iostream>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ChartHandler::ChartHandler()
	//
	// default constructor for ChartHandler
	//
{
	current_chart = NULL;
	current_chart_n_user = -1000;
	u_g_defined = false;
}

ChartHandler::~ChartHandler()
{
}

bool
ChartHandler::Read(CParser &parser)
{
	int n_user;
	std::string token;

	// reads line, next character is after keyword
	parser.check_line("ChartHandler", true, false, true, false);

	std::istringstream iss(parser.line());
	// keyword
	iss >> token;
	// number
	if (!(iss >> n_user))
	{
		n_user = 1;
	}

	// makes new ChartObject if necessary
	std::map<int, ChartObject>::iterator it = this->chart_map.find(n_user);
	if (it == this->chart_map.end())
	{
		ChartObject chart_obj;
		chart_map[n_user] = chart_obj;
		it = this->chart_map.find(n_user);
	}

	// Read/update ChartObject
	it->second.Read(parser);
	current_chart_n_user = n_user;
	current_chart = &it->second;
	u_g_defined = true;

	return true;
}


