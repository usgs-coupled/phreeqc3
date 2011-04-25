#if !defined(CHARTHANDLER_H_INCLUDED)
#define CHARTHANDLER_H_INCLUDED
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
	bool Read(CParser &parser);

protected:
	std::map<int,ChartObject> chart_map;
	int current_chart_n_user;
	ChartObject * current_chart;
	bool u_g_defined;

public:

};

#endif // !defined(CHARTHANDLER_H_INCLUDED)
