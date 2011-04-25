#if !defined(CHARTOBJECT_H_INCLUDED)
#define CHARTOBJECT_H_INCLUDED
#include <vector>
#include <list>
#include <string>
#include <sstream>
#include "CurveObject.h"
#include "global_structures.h"
#include "NumKeyword.h"
//#include "Parser.h"
#include <float.h>

//#define NA (float) -9.9999	            /* NA = not available */
class ChartObject:public cxxNumKeyword
{

  public:
	ChartObject();
	ChartObject(int i);
	~ChartObject();

	// new_ug
	bool Get_new_ug()
	{
		return this->new_ug;
	}
	void Set_new_ug(bool b)
	{
		this->new_ug = b;
	}
	// bool FirstCallToUSER_GRAPH;
	void Set_FirstCallToUSER_GRAPH(bool b)
	{
		this->FirstCallToUSER_GRAPH = b;
	}
	bool Get_FirstCallToUSER_GRAPH()
	{
		return this->FirstCallToUSER_GRAPH;
	}
	// int update_time_chart;
	int Get_update_time_chart()
	{
		return (this->update_time_chart);
	}
	// PanelHeight
	int Get_PanelHeight()
	{
		return (this->PanelHeight);
	}
	// PanelWidth
	int Get_PanelWidth()
	{
		return (this->PanelWidth);
	}
	// Symbol_map
	// std::vector<std::string> Color_vector; 
	// int nCSV_headers;
	// std::vector<std::string> user_graph_headings;
	std::vector<std::string> &Get_user_graph_headings()
	{
		return this->user_graph_headings;
	}
	// std::string chart_title;
	std::string &Get_chart_title()
	{
		return this->chart_title;
	}
	// std::vector<std::string> axis_titles;
	std::vector<std::string> &Get_axis_titles()
	{
		return this->axis_titles;
	}
	// double axis_scale_x[5];
	double *Get_axis_scale_x()
	{
		return this->axis_scale_x;
	}
	// double axis_scale_y[5];
	double *Get_axis_scale_y()
	{
		return this->axis_scale_y;
	}
	// double axis_scale_y2[5];
	double *Get_axis_scale_y2()
	{
		return this->axis_scale_y2;
	}
	// int chart_type;
	int Get_chart_type()
	{
		return this->chart_type;
	}
	// bool graph_initial_solutions;
	bool Get_graph_initial_solutions()
	{
		return this->graph_initial_solutions;
	}
	// bool connect_simulations;
	bool Get_connect_simulations()
	{
		return this->connect_simulations;
	}
	// int shifts_as_points;
	// int rownr;
	void Set_rownr(int i)
	{
		this->rownr = i;
	}
	int Get_rownr()
	{
		return (this->rownr);
	}
	// int colnr;
	void Set_colnr(int i)
	{
		this->colnr = i;
	}
	int Get_colnr()
	{
		return (this->colnr);
	}
	// int RowOffset;
	// int ColumnOffset;
	
	// int prev_advection_step;
	void Set_prev_advection_step(int i)
	{
		this->prev_advection_step = i;
	}
	int Get_prev_advection_step()
	{
		return (this->prev_advection_step);
	}
	// int prev_transport_step;
	void Set_prev_transport_step(int i)
	{
		this->prev_transport_step = i;
	}
	int Get_prev_transport_step()
	{
		return (this->prev_transport_step);
	}
	// bool AddSeries;
	void Set_AddSeries(bool b)
	{
		this->AddSeries = b;
	}
	bool Get_AddSeries(void)
	{
		return this->AddSeries;
	}
	// int prev_sim_no;
	// bool x_filled;
	// bool col_dwn;
	// std::vector<bool> y_filled;
	// double x_value;
	// bool all_points;
	void Set_all_points(bool tf)
	{
		this->all_points = tf;
	}
	bool Get_all_points()
	{
		return this->all_points;
	}
	// bool end_timer;
	bool Get_end_timer()
	{
		return this->end_timer;
	}
	// std::vector<CurveObject> Curves;
	std::vector<CurveObject> &Get_Curves()
	{
		return this->Curves;
	}
	// int ncurves_changed[3];
	int *Get_ncurves_changed()
	{
		return (this->ncurves_changed);
	}
	void Set_ncurves_changed_0(int i)
	{
		this->ncurves_changed[0] = i;
	}
	// int CurveInfonr;
	
	// struct rate user_graph;
	struct rate *Get_user_graph()
	{
		return &this->user_graph;
	}
	// C++ for rate struct
	// std::string rate_name;
	// std::list<std::string> rate_command_list;
	std::list<std::string> &Get_rate_command_list()
	{
		return this->rate_command_list;
	}
	// bool rate_new_def;
	void Set_rate_new_def(bool tf)
	{
		this->rate_new_def = tf;
		if (tf)
		{
			this->user_graph.new_def = 1;
		}
		else
		{
			this->user_graph.new_def = 0;
		}
	}
	bool Get_rate_new_def()
	{
		return this->rate_new_def;
	}
	// int default_symbol;




	bool Set_axis_scale(std::vector<std::string>, std::vector<int> types, std::ostringstream &);
	bool Set_axis_scale(CParser & parser);
	bool Read(CParser & parser);
	bool OpenCSVFile(std::string fn);
	void ExtractCurveInfo(std::string & str_line);
	void Set_rate_struct(void);
	void PlotXY(std::string x, std::string y);
    bool start_chart(void);
	ZedGraph::SymbolType Return_SymbolType(std::string);
	void SaveCurvesToFile(std::string &);
  protected:

	bool new_ug;
	bool FirstCallToUSER_GRAPH;

	int update_time_chart;			/* milliseconds, maybe read */
	int PanelHeight;
	int PanelWidth;
	std::map<std::string, int> Symbol_map;
	std::vector<std::string> Color_vector; 
	int nCSV_headers;
	std::vector<std::string> user_graph_headings;
	std::string chart_title;
	std::vector<std::string> axis_titles;
	double axis_scale_x[5];
	double axis_scale_y[5];
	double axis_scale_y2[5];

	int chart_type;
	bool graph_initial_solutions;
	bool connect_simulations;
	int shifts_as_points;
	int rownr;
	int colnr;
	int RowOffset;
	int ColumnOffset;
	
	int prev_advection_step;
	int prev_transport_step;
	bool AddSeries;

	int prev_sim_no;
	bool x_filled;
	bool col_dwn;
	std::vector<bool> y_filled;
	double x_value;
	bool all_points;
	bool end_timer;

	std::vector<CurveObject> Curves;
	int ncurves_changed[3];	
	int CurveInfonr;
	
	struct rate user_graph;
	// C++ for rate struct
	std::string rate_name;
	std::list<std::string> rate_command_list;
	bool rate_new_def;

	int default_symbol;

  public:

};

#endif // !defined(CHARTOBJECT_H_INCLUDED)
