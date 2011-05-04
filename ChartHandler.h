#if !defined(CHARTHANDLER_H_INCLUDED)
#define CHARTHANDLER_H_INCLUDED
#if defined MULTICHART
#include <vector>
#include <map>
#include <string>
#include "Parser.h"
#include "ChartObject.h"

class ChartHandler
{

public:
	ChartHandler();
	~ChartHandler();

	size_t Get_chart_count()
	{
		return this->chart_map.size();
	}
	ChartObject * Get_current_chart()
	{
		return this->current_chart;
	}
	bool Get_timer()
	{
		return timer;
	}
	bool Read(PHREEQC_PTR_ARG_COMMA CParser &parser);
	void Punch_user_graph(PHREEQC_PTR_ARG);
	bool End_timer(PHREEQC_PTR_ARG);

protected:
	std::map<int, ChartObject *> chart_map;
	int current_chart_n_user;
	ChartObject * current_chart;
	bool u_g_defined;
	bool timer;

public:

};
#endif // MULTICHART
#endif // !defined(CHARTHANDLER_H_INCLUDED)
