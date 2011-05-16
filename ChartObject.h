#if !defined(CHARTOBJECT_H_INCLUDED)
#define CHARTOBJECT_H_INCLUDED
#if defined MULTICHART
#include <vector>
#include <list>
#include <string>
#include <sstream>
#include "CurveObject.h"
#if defined PHREEQC_CLASS

#else
#include "global_structures.h"
#endif
#include "NumKeyword.h"

#include <float.h>
class Phreeqc;

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

	// int colnr;
	void Set_colnr(int i)
	{
		this->colnr = i;
	}
	int Get_colnr()
	{
		return (this->colnr);
	}
	// int ColumnOffset;
	void Set_ColumnOffset(int i)
	{
		this->ColumnOffset = i;
	}
	int Get_ColumnOffset()
	{
		return (this->ColumnOffset);
	}	
	// bool AddSeries;
	void Set_AddSeries(bool b)
	{
		this->AddSeries = b;
	}
	bool Get_AddSeries()
	{
		return this->AddSeries;
	}
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
	// int prev_sim_no;
	void Set_prev_sim_no(int i)
	{
		this->prev_sim_no = i;
	}
	int Get_prev_sim_no(void)
	{
		return this->prev_sim_no;
	}
	// bool end_timer;
	void Set_end_timer(bool b)
	{
		this->end_timer = b;
	}
	bool Get_end_timer()
	{
		return this->end_timer;
	}
	// bool end_timer;
	void Set_done(bool b)
	{
		this->done = b;
	}
	bool Get_done()
	{
		return this->done;
	}
	// std::vector<CurveObject *> CurvesCSV;
	std::vector<CurveObject *> &Get_CurvesCSV()
	{
		return this->CurvesCSV;
	}
	// std::vector<CurveObject *> Curves;
	std::vector<CurveObject *> &Get_Curves()
	{
		return this->Curves;
	}
	// bool curve_added;
	void Set_curve_added(bool tf)
	{
		this->curve_added = tf;
	}
	bool Get_curve_added()
	{
		return this->curve_added;
	}
	// bool point_added;
	void Set_point_added(bool tf)
	{
		this->point_added = tf;
	}
	bool Get_point_added()
	{
		return this->point_added;
	}
	// struct rate user_graph;
	struct rate *Get_user_graph()
	{
		return this->user_graph;
	}
	// C++ for rate struct
	// std::string rate_name;
	// std::list<std::string> rate_command_list;
	std::list<std::string> &Get_rate_command_list()
	{
		return this->rate_command_list;
	}
	// bool rate_new_def;
	void Set_rate_new_def(bool tf);

	bool Get_rate_new_def()
	{
		return this->rate_new_def;
	}
	// int default_symbol;
	//double graph_x;
	void Set_graph_x(double d)
	{
		this->graph_x = d;
	}
	double Get_graph_x()
	{
		return this->graph_x;
	}
	//std::map<int, double> graph_y;
	std::map<int, double> &Get_graph_y()
	{
		return this->graph_y;
	}
	//std::map<int, bool> secondary_y;
	std::map<int, bool> &Get_secondary_y()
	{
		return this->secondary_y;
	}
	//std::vector<CurveObject> new_plotxy_curves;
	std::vector<CurveObject> &Get_new_plotxy_curves()
	{
		return this->new_plotxy_curves;
	}
	//std::vector<std::string> new_headings;
	std::vector<std::string> &Get_new_headings()
	{
		return this->new_headings;
	}
	// bool active;
	void Set_active(bool tf)
	{
		this->active = tf;
	}
	bool Get_active()
	{
		return this->active;
	}
	// bool detach;
	void Set_detach(bool tf)
	{
		this->detach = tf;
	}
	bool Get_detach()
	{
		return this->detach;
	}
	// bool chart_started;
	bool Get_form_started()
	{
		return this->form_started;
	}
#if defined PHREEQC_CLASS
	void Set_phreeqc(Phreeqc * ptr)
	{
		this->p_instance1 = ptr;
	}
	Phreeqc * Get_phreeqc()
	{
		return this->p_instance1;
	}
#endif

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
	void Rate_free(void);
	void Initialize_graph_pts(void);
	void Finalize_graph_pts(void);
	void Get_legal_symbol(std::string &sym);
	void Get_legal_symbol_csv(std::string &sym);
	void Get_color_string(std::string &color);
	void Get_color_string_csv(std::string &color);
	void Add_new_series(void);
	void Add_curve(bool plotxy, std::string id = "", 
					   double line_width = 1.0, 
					   std::string symbol = "",
					   double symbol_size = 6.0, 
					   int y_axis = 1,
					   std::string color = "");
  protected:

	bool new_ug;
	bool FirstCallToUSER_GRAPH;

	int update_time_chart;			/* milliseconds, maybe read */
	int PanelHeight;
	int PanelWidth;
	std::map<std::string, int> Symbol_map;
	std::vector<std::string> Color_vector; 
	std::string chart_title;
	std::vector<std::string> axis_titles;
	double axis_scale_x[5];
	double axis_scale_y[5];
	double axis_scale_y2[5];

	int chart_type;
	bool graph_initial_solutions;
	bool connect_simulations;
	int shifts_as_points;
	int colnr;
	int ColumnOffset;
	bool AddSeries;
	
	int prev_advection_step;
	int prev_transport_step;

	int prev_sim_no;

	bool end_timer;
	bool done;

	std::vector<CurveObject *> CurvesCSV;
	std::vector<CurveObject *> Curves;
	bool curve_added;
	bool point_added;
	
	struct rate *user_graph;
	// C++ for rate struct
	std::string rate_name;
	std::list<std::string> rate_command_list;
	bool rate_new_def;

	int default_symbol;
	int default_symbol_csv;
	int default_color;
	int default_color_csv;

	// temporary storage before stored graph_x/y/sy data are stored in curves
	// Initialize_graph_pts and Finalize_graph_pts use this storage.
	double graph_x;
	std::map<int, double> graph_y;
	std::map<int, bool> secondary_y;

	// temporary plotxy curve definitions before stored in curves
	// a plotxy curve is copied to Curves when cmdplotxy is encountered
	// this keeps order correct between plotxy and graph_x/y/sy
	std::vector<CurveObject> new_plotxy_curves;

	// temporary headings until stored during basic_run
	std::vector<std::string> new_headings;
	bool active;
	bool detach;
	bool form_started;
#if defined PHREEQC_CLASS
	class Phreeqc * p_instance1;
#endif

  public:
	int usingResource;


};
#endif // MULTICHART
#endif // !defined(CHARTOBJECT_H_INCLUDED)
