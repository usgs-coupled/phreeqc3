// ChartHandler.cpp: implementation of the ChartHandler class.
//
//////////////////////////////////////////////////////////////////////
#if defined MULTICHART
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include "ChartHandler.h"
#include "phreeqc_class.h"
#if defined PHREEQC_CLASS
#include "phreeqc.h"
#else
extern int punch_user_graph(void);
extern int error_msg(const char *err_str, const int stop, ...);
#endif
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
ChartHandler::Punch_user_graph(PHREEQC_PTR_ARG)
{
	std::map<int, ChartObject *>::iterator it = this->chart_map.begin();
	for ( ; it != chart_map.end(); it++)
	{
		if (it->second->Get_active())
		{
			while (0 != System::Threading::Interlocked::Exchange(it->second->usingResource, 1))
				System::Threading::Thread::Sleep(1);
			this->current_chart = it->second;
			P_INSTANCE_POINTER punch_user_graph();
			System::Threading::Interlocked::Exchange(it->second->usingResource, 0);
		}
	}
}

bool
ChartHandler::Read(PHREEQC_PTR_ARG_COMMA CParser &parser)
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
		chart_map[n_user] = new ChartObject();
		it = this->chart_map.find(n_user);
#ifdef PHREEQC_CLASS
		it->second->Set_phreeqc(P_INSTANCE);
#endif
	}

	// Read/update ChartObject
	while (0 != System::Threading::Interlocked::Exchange(it->second->usingResource, 1)) 
		System::Threading::Thread::Sleep(1);
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

	// Release lock
	System::Threading::Interlocked::Exchange(it->second->usingResource, 0);

	// if detached, wait for thread to acknowledge and then erase chart
	if (it->second->Get_detach())
	{
		while (it->second->Get_form_started() && it->second->Get_done() != true) 
		{
			System::Threading::Thread::Sleep(1);
		}
		delete it->second;
		this->chart_map.erase(it);
	}	
	return true;
}
bool
ChartHandler::End_timer(PHREEQC_PTR_ARG)
{
	
	size_t max_tries = 1000; 
	std::map<int, ChartObject *>::iterator it = this->chart_map.begin();
	
	for  ( ; it != chart_map.end(); it++)
	{
		//{
		//	std::vector<CurveObject *>::iterator cit = it->second->Get_Curves().begin();
		//	size_t i;
		//	for (i = 0; i <  it->second->Get_Curves().size(); i ++)
		//	{
		//		std::cerr << "Curve " << i << ", number of points " << it->second->Get_Curves()[i]->Get_x().size() << std::endl;
		//	}
		//}
		size_t i = 0;
		it->second->Rate_free();
		if (it->second->Get_form_started())
		{
			while (0 != System::Threading::Interlocked::Exchange(it->second->usingResource, 1) && i < max_tries) 
			{
				i++;
				System::Threading::Thread::Sleep(1);
			}
			it->second->Set_end_timer(true);
			System::Threading::Interlocked::Exchange(it->second->usingResource, 0);

			size_t i2 = 0;
			while (it->second->Get_done() != true && i2 < max_tries) 
			{
				i2++;
				System::Threading::Thread::Sleep(1);
			}
			if (i >= max_tries || i2 >= max_tries)
			{
				P_INSTANCE_POINTER error_msg("Chart did not respond.", CONTINUE);
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

