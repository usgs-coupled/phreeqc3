// ChartHandler.cpp: implementation of the ChartHandler class.
//
//////////////////////////////////////////////////////////////////////
#if defined MULTICHART
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include "ChartHandler.h"
#include "phreeqc.h"
#include <iostream>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ChartHandler::ChartHandler(PHRQ_io *io)
:
PHRQ_base(io)
	//
	// default constructor for ChartHandler
	//
{
	current_chart = NULL;
	current_chart_n_user = -1000;
	u_g_defined = false;
	timer = true;
}

ChartHandler::~ChartHandler()
{
	std::map<int, ChartObject *>::iterator it;
	for (it = this->chart_map.begin(); it != chart_map.end(); it++)
	{
		delete it->second;
	}
}
void
ChartHandler::Punch_user_graph(Phreeqc * phreeqc_ptr)
{
	std::map<int, ChartObject *>::iterator it = this->chart_map.begin();
	for ( ; it != chart_map.end(); it++)
	{
		if (it->second->Get_active())
		{
#if defined(__cplusplus_cli)
			while (0 != System::Threading::Interlocked::Exchange(it->second->usingResource, 1))
				System::Threading::Thread::Sleep(1);
#endif
			this->current_chart = it->second;
			phreeqc_ptr-> punch_user_graph();
#if defined(__cplusplus_cli)
			System::Threading::Interlocked::Exchange(it->second->usingResource, 0);
#endif
		}
	}
}

bool
ChartHandler::Read(Phreeqc * phreeqc_ptr, CParser &parser)
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
	std::map<int, ChartObject *>::iterator it = this->chart_map.find(n_user);
	if (it == this->chart_map.end())
	{
		chart_map[n_user] = new ChartObject(this->Get_io());
		it = this->chart_map.find(n_user);
		it->second->Set_phreeqc(phreeqc_ptr);
	}

	// Read/update ChartObject
#if defined(__cplusplus_cli)
	while (0 != System::Threading::Interlocked::Exchange(it->second->usingResource, 1)) 
		System::Threading::Thread::Sleep(1);
#endif
	{
		it->second->Read(parser);
		current_chart_n_user = n_user;
		current_chart = it->second;
		u_g_defined = true;
	}

	// if detached, set timer_end and free
	if (it->second->Get_detach() && it->second->Get_form_started())
	{
		it->second->Set_end_timer(true);
		it->second->Rate_free();
	}

#if defined(__cplusplus_cli)
	// Release lock
	System::Threading::Interlocked::Exchange(it->second->usingResource, 0);
#endif

	// if detached, wait for thread to acknowledge and then erase chart
	if (it->second->Get_detach())
	{
		while (it->second->Get_form_started() && it->second->Get_done() != true) 
		{
#if defined(__cplusplus_cli)
			System::Threading::Thread::Sleep(1);
#endif
		}
		delete it->second;
		this->chart_map.erase(it);
	}	
	return true;
}
bool
ChartHandler::End_timer()
{
	
	size_t max_tries = 1000; 
	std::map<int, ChartObject *>::iterator it = this->chart_map.begin();
	
	for  ( ; it != chart_map.end(); it++)
	{
		size_t i = 0;
		it->second->Rate_free();
		if (it->second->Get_form_started())
		{
#if defined(__cplusplus_cli)
			while (0 != System::Threading::Interlocked::Exchange(it->second->usingResource, 1) && i < max_tries) 
			{
				i++;
				System::Threading::Thread::Sleep(1);
			}
#endif
			it->second->Set_end_timer(true);
#if defined(__cplusplus_cli)
			System::Threading::Interlocked::Exchange(it->second->usingResource, 0);
#endif

			size_t i2 = 0;
			while (it->second->Get_done() != true && i2 < max_tries) 
			{
				i2++;
#if defined(__cplusplus_cli)
				System::Threading::Thread::Sleep(1);
#endif
			}
			if (i >= max_tries || i2 >= max_tries)
			{
				error_msg("Chart did not respond.", CONTINUE);
			}
		}
	}
	this->timer = false;

	return true;
}
bool
ChartHandler::dump(std::ostream & oss, unsigned int indent)
{
	std::map<int, ChartObject *>::iterator it = this->chart_map.begin();
	for  ( ; it != chart_map.end(); it++)
	{
		size_t i = 0;
		it->second->dump(oss, indent);
	}
	return true;
}
#endif

