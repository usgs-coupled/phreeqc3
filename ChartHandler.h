#if !defined(CHARTHANDLER_H_INCLUDED)
#define CHARTHANDLER_H_INCLUDED
#if defined MULTICHART
#include <vector>
#include <map>
#include <string>
#include "Parser.h"
#include "ChartObject.h"
#include "PHRQ_base.h"

class ChartHandler: public PHRQ_base
{

public:
	ChartHandler(PHRQ_io *io = NULL);
	virtual ~ChartHandler();

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
	bool Read(Phreeqc * phreeqc_ptr, CParser &parser);
	void Punch_user_graph(Phreeqc * phreeqc_ptr);
	bool End_timer();
	bool dump(std::ostream & oss, unsigned int indent);
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
