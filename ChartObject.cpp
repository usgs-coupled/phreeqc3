// ChartObject.cpp: implementation of the ChartObject class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#ifdef MULTICHART
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include "Utils.h"
#include <iostream>
#include "ChartObject.h"
#include "Parser.h"
#include <fstream>
#include <math.h>

#ifdef PHREEQC_CLASS
#else
#include "phqalloc.h"
extern int error_msg(const char *err_str, const int stop, ...);
extern int warning_msg(const char *err_str, ...);
extern int rate_free(struct rate *rate_ptr);
#endif

#include "Form2.h"
using namespace zdg_ui2;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ChartObject::ChartObject()
	//
	// default constructor for cxxExchComp
	//
{
	new_ug = false;
	FirstCallToUSER_GRAPH = true;
	
	update_time_chart = 150;
	PanelHeight = 510;
	PanelWidth = 640;

	Symbol_map["Square"] = 0;
	Symbol_map["Diamond"] = 1;
	Symbol_map["Triangle"] = 2;
	Symbol_map["Circle"] = 3;
	Symbol_map["XCross"] = 4;
	Symbol_map["Plus"] = 5;
	Symbol_map["Star"] = 6;
	Symbol_map["TriangleDown"] = 7;
	Symbol_map["HDash"] = 8;
	Symbol_map["VDash"] = 9;
	Symbol_map["None"] = 10;

	Color_vector.push_back("Red");
	Color_vector.push_back("Green");
	Color_vector.push_back("Blue");
	Color_vector.push_back("Orange");
	Color_vector.push_back("Magenta");
	Color_vector.push_back("Yellow");
	Color_vector.push_back("Black");

	nCSV_headers = 0;
	user_graph_headings.clear();
	chart_title.clear();
	axis_titles.clear();
	
	int i;
	for (i = 0; i < 5; i++)
	{
		axis_scale_x[i] = NA;
		axis_scale_y[i] = NA;
		axis_scale_y2[i] = NA;
	}

	chart_type = 0;
	graph_initial_solutions = false;
	shifts_as_points = false;
	rownr = -1;
	colnr = 0;
	RowOffset = 0;
	ColumnOffset = 0;
	prev_advection_step = 0;
	prev_transport_step = 0;
	AddSeries = true;
	prev_sim_no = 0;
	x_filled = false;
	col_dwn = false;
	y_filled.clear();
	x_value = NA;
	bool all_points = false;
	bool end_timer = false;

	graph_initial_solutions = true;
	connect_simulations = true;

	ncurves_changed[0] = ncurves_changed[1] = ncurves_changed[2] = 0;

	user_graph.commands = NULL;
	user_graph.name = NULL;
	user_graph.new_def = 0;
	user_graph.linebase = user_graph.loopbase = user_graph.varbase = NULL;

	default_symbol = 0;
}

ChartObject::~ChartObject()
{
	// all data cleans itself up
}

bool 
ChartObject::Set_axis_scale(CParser & parser)
{
	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	double *scale_ptr = NULL;
	std::vector<std::string> string_vector;
	size_t j = 0;

	// rereads option
	parser.copy_token(token, next_char);

	// reads parameters
	while ((j < 5) && (parser.copy_token(token, next_char)) != CParser::TT_EMPTY)
	{
		string_vector.push_back(token);
		j++;
	}
	if (string_vector.size() == 0)
	{
		error_msg("No axis defined for scales", CParser::OT_CONTINUE);
		return false;
	}
	std::string str = string_vector[0];
	std::string type;
	// determine axis
	switch (str[0])
	{
	case 'X':
	case 'x':
		type = "x";
		scale_ptr = this->axis_scale_x;
		break;
	case 'Y':
	case 'y':
		type = "y";
		scale_ptr = this->axis_scale_y;
		break;
	case 'S':
	case 's':
		type = "sy";
		scale_ptr = this->axis_scale_y2;
		break;
	default:
		std::ostringstream estream;
		estream << "Found " << str;
		estream << ", but expect axis type \'x\', \'y\', or \'sy\'.";
		estream << std::endl;
		error_msg("No axis defined for scales", CParser::OT_CONTINUE);
		return false;
		break;
	}

	for (j = 1; j < string_vector.size() && j < 5; j++)
	{
		std::string s = string_vector[j];
		if (s[0] == 'a' || s[0] == 'A')
		{
			scale_ptr[j - 1] = NA;
		}
		else if (CParser::token_type(s) == CParser::TT_DIGIT)
		{
			scale_ptr[j - 1] = atof(s.c_str());
		}
		else
		{
			std::ostringstream estream;
			estream << "Found " << s;
			estream << ", but expect number or 'a(uto)'.";
			estream << std::endl;
			error_msg(estream.str().c_str(), CONTINUE);
			return false;
		}
	}
	if (string_vector.size() == 5)
	{
		std::string s = string_vector[4];
		if (s[0] != 't' || s[0] != 'T')
		{
			scale_ptr[4] = 10.0;
			if (((fabs(scale_ptr[0] - NA) > 1e-3) && scale_ptr[0] <=0) ||
				((fabs(scale_ptr[1] - NA) > 1e-3) && scale_ptr[1] <=0))
			{
				std::ostringstream estream;
				estream << "MIN and MAX must be > 0 for log " << type << "-scale.";
				estream << std::endl;
				error_msg(estream.str().c_str(), CONTINUE);
				return false;
			}

		}
	}
	if ((fabs(scale_ptr[0] - NA) > 1e-3) && (fabs(scale_ptr[1] - NA) > 1e-3))
	{
		if (scale_ptr[0] > scale_ptr[1])
		{
			std::ostringstream estream;
			estream << "Maximum must be larger than minimum of axis_scale " << type << "-scale." << std::endl;
			estream << "Switching values for MIN and MAX. " << std::endl;
			warning_msg(estream.str().c_str());
			double t;
			t = scale_ptr[0];
			scale_ptr[0] = scale_ptr[1];
			scale_ptr[1] = scale_ptr[0];
			return false;
		}
	}
	return true;
}

bool
ChartObject::Read(CParser & parser)
{

	static std::vector < std::string > vopts;
	if (vopts.empty())
	{
		vopts.reserve(15);
		vopts.push_back("start");	// 0 
		vopts.push_back("end");	// 1 
		vopts.push_back("heading");	// 2 
		vopts.push_back("headings");	// 3 
		vopts.push_back("chart_title");	// 4 
		vopts.push_back("axis_titles");	// 5 
		vopts.push_back("axis_scale");	// 6 
		vopts.push_back("initial_solutions");	// 7 
		vopts.push_back("plot_concentration_vs");	// 8 
		vopts.push_back("shifts_as_points");	// 9 
		vopts.push_back("grid_offset");	// 10
		vopts.push_back("connect_simulations");	// 11
		vopts.push_back("plot_csv_file");	// 12		
	}
	std::istream::pos_type ptr;
	std::istream::pos_type next_char;
	std::string token;
	int opt_save;
	bool useLastLine(false);
	if (this->Curves.size() > 0)
	{
		this->ncurves_changed[0] = 1;
		this->ColumnOffset = ncurves_changed[2];
		this->new_ug = true;
	}


	// Read number and description
	{
		this->read_number_description(parser);
	}

	opt_save = CParser::OPT_DEFAULT;

	for (;;)
	{
		int opt;
		if (useLastLine == false)
		{
			opt = parser.get_option(vopts, next_char);
		}
		else
		{
			opt = parser.getOptionFromLastLine(vopts, next_char);
		}
		if (opt == CParser::OPT_DEFAULT)
		{
			opt = opt_save;
		}
		switch (opt)
		{
		case CParser::OPT_EOF:
			break;
		case CParser::OPT_KEYWORD:
			break;
		case CParser::OPT_ERROR:
			opt = CParser::OPT_EOF;
			error_msg("Unknown input in USER_GRAPH keyword.", CONTINUE);
			error_msg(parser.line().c_str(), CONTINUE);
			useLastLine = false;
			break;
		case 0:				/* start */
			opt_save = CParser::OPT_DEFAULT;
			break;
		case 1:				/* end */
			opt_save = CParser::OPT_DEFAULT;
			break;
		case 2:				/* headings */
		case 3:				/* heading */
			while (parser.copy_token(token, next_char) != CParser::TT_EMPTY)
			{
				this->user_graph_headings.push_back(token);
			}
			break;
		case 4:				/* chart title */
			{
				std::string l = parser.line();
				std::string tok;
				std::string::iterator b = l.begin();
				std::string::iterator e = l.end();
				parser.copy_title(tok,  b, e);
				this->chart_title = tok;
			}
			break;
		case 5:	/* axis titles */
			{
				std::string l; 
				parser.get_rest_of_line(l);
				std::string tok;
				std::string::iterator b = l.begin();
				std::string::iterator e = l.end();
				this->axis_titles.clear();
				while(parser.copy_title(tok,  b, e) != CParser::TT_EMPTY)
				{
					this->axis_titles.push_back(tok);
				}
			}
			break;
		case 6:	/* axis scales */
			{ 
				this->Set_axis_scale(parser);
			}
			break;
		case 7: /* initial_solutions */
			this->graph_initial_solutions = parser.get_true_false(next_char, FALSE);
			break;
		case 8: /* plot_concentration_vs */
			//prev_next_char = next_char;
			parser.copy_token(token, next_char);
			Utilities::str_tolower(token);
			if (token[0] == 'x' || token[0] == 'd')
				chart_type = 0;
			else if (token[0] == 't')
				chart_type = 1;
			else
			{
				std::ostringstream estream;
				estream << "Found " << token << ", but expect plot type: (\'x\' or \'dist\') for distance, (\'t\') for time.";
				error_msg(estream.str().c_str(), CONTINUE);			
			}
			break;
		case 9:  /* shifts_as_points */
			this->shifts_as_points = parser.get_true_false(next_char, true);
			if (this->shifts_as_points)
				this->chart_type = 0;
			else
				this->chart_type = 1;
			break;
		case 10: /* grid_offset */
#ifdef PHREEQ98
			/*
			i = copy_token(token, &next_char, &l);
			str_tolower(token);
			if (i == DIGIT)
				sscanf(token, "%d", &RowOffset);
			i = copy_token(token, &next_char, &l);
			str_tolower(token);
			if (i == DIGIT)
				sscanf(token, "%d", &ColumnOffset);
			*/
#endif
			break;
		case 11: /* connect_simulations */
			this->connect_simulations = parser.get_true_false(next_char, true);
			break;
		case 12: /* plot_csv_file */
			{
				std::string file_name;
				parser.get_rest_of_line(file_name);
				file_name = trim(file_name);
				this->OpenCSVFile(file_name);
			}
			break;
			/* End of modifications */
		case CParser::OPT_DEFAULT:	// Read Basic commands
			{
				this->rate_new_def = true;
				/* read command */
				std::string cmd(parser.line());
				std::string cmd_lower = cmd;
				Utilities::str_tolower(cmd_lower);
				if ((cmd_lower.find("graph_y") != std::string::npos) || 
					(cmd_lower.find("graph_sy") != std::string::npos))
				{
					CurveInfonr++;
				}
				if (cmd_lower.find("plot_xy") != std::string::npos)
				{
					ExtractCurveInfo(cmd); // truncates cmd
					CurveInfonr++;
				}
				this->rate_command_list.push_back(cmd);
			}
			opt_save = CParser::OPT_DEFAULT;
			break;
		}
		if (opt == CParser::OPT_EOF || opt == CParser::OPT_KEYWORD)
			break;
	}
	
	this->Set_rate_struct();
	return true;
}

bool
ChartObject::OpenCSVFile(std::string file_name)
{
	std::string token;

	std::ifstream f_csv(file_name.c_str(), std::ifstream::in);

	if (!f_csv.is_open())
	{
		std::ostringstream estream;
		estream << "Could not open csv file for USER_GRAPH " << file_name;
		error_msg(estream.str().c_str(), CONTINUE);
		return false;
	}

	std::ostringstream oss_out;
	std::ostringstream oss_err;
	CParser parser(f_csv, oss_out, oss_err);

	/* Get lines */
	int linenr = 0; 
	std::vector<std::string> headings;
	std::vector<CurveObject> csv_curves;
	while (parser.check_line("cvs file", false, true, true, false) != CParser::LT_EOF)
	{
		// Headings line
		if (linenr == 0)
		{
			// Skip x in 1st column
			parser.get_iss() >> token;
			while (parser.get_iss() >> token)
			{
				headings.push_back(token);
			}
			nCSV_headers = (int) headings.size();
			this->ColumnOffset = nCSV_headers;

			// add new headers to beginning of list
			std::vector<std::string> old_headings = this->user_graph_headings;
			this->user_graph_headings.clear();
			this->user_graph_headings = headings;
			std::vector<std::string>::iterator it = old_headings.begin();
			for (; it != old_headings.end(); it++)
			{
				this->user_graph_headings.push_back(*it); 
			}

			// add new curves
			size_t i; 
			for (i = 0; i < headings.size(); i++)
			{
				CurveObject c;
				c.Set_id(headings[i]);
				csv_curves.push_back(c);
			}

			linenr++;
			continue;
		}

		token = parser.line();
		std::string tok1;
		CParser::TOKEN_TYPE tt = CParser::parse_delimited(token, tok1, "\t");

		// Curve properties lines
		if (linenr < 6 && tt != CParser::TT_DIGIT)
		{
			Utilities::str_tolower(tok1);
			std::string tok2;
			size_t i=0;
			while (token.size() != 0 && i < csv_curves.size())
			{
				CParser::parse_delimited(token, tok2, "\t");
				if (!strncmp(tok1.c_str(), "color", 5))
				{
					csv_curves[i].Set_color(tok2);
				}
				else if (!strncmp(tok1.c_str(), "symbol", 5) && (strstr(tok1.c_str(), "_si") == NULL)
					&& (strstr(tok1.c_str(), "-si") == NULL))
				{
					csv_curves[i].Set_symbol(tok2);
				}
				else if (!strncmp(tok1.c_str(), "symbol_size", 8) || !strncmp(tok1.c_str(), "symbol-size", 8))
				{
					csv_curves[i].Set_symbol_size(atof(tok2.c_str()));
				}
				else if (!strncmp(tok1.c_str(), "line_w", 5) || !strncmp(tok1.c_str(), "line-w", 5))
				{
					csv_curves[i].Set_line_w(atof(tok2.c_str()));
				}
				else if (!strncmp(tok1.c_str(), "y_axis", 5) || !strncmp(tok1.c_str(), "y-axis", 5))
				{
					csv_curves[i].Set_y_axis(atoi(tok2.c_str()));
				}
				i++;
			}
			linenr++;
			continue;
		}

		// Curve data
		if (linenr < 6)	linenr = 6;
		if (tt != CParser::TT_DIGIT) 
		{
			linenr++;	
			continue;
		} 

		// x value for all curves
		double x_value = atof(tok1.c_str());

		// y values for curves
		std::string tok2;
		size_t i=0;
		while (token.size() != 0 && i < csv_curves.size())
		{
			CParser::parse_delimited(token, tok2, "\t");
			Utilities::squeeze_white(tok2);
			if (tok2.size() == 0)
			{
				csv_curves[i].Get_x().push_back(NA);
				csv_curves[i].Get_x().push_back(NA);
			}
			else
			{
				csv_curves[i].Get_x().push_back(x_value);
				csv_curves[i].Get_y().push_back(atof(tok2.c_str()));
			}
			i++;
		}
		linenr++;
	}

	// Prepend new curves
	std::vector<CurveObject> old_curves = this->Curves;
	this->Curves.clear();
	this->Curves = csv_curves;

	std::vector<CurveObject>::iterator it = old_curves.begin();
	for (; it != old_curves.end(); it++)
	{
		this->Curves.push_back(*it);
	}
	return true;
}

void 
ChartObject::ExtractCurveInfo(std::string & cmd_line)
{
	/* plot_xy x, tot("Cl"), color = Red, symbol = Circle, symbol_size = 0.0, line_w = 1.0, y_axis = 2 */

	// Make copy of cmd_line
	int curvenr = (int) this->Curves.size();
	std::string str_line = cmd_line;

	// find command part of cmd_line
	Utilities::replace(",","#",cmd_line);
	size_t pos = cmd_line.find(",");
	if (pos != std::string::npos)
	{
		cmd_line = cmd_line.substr(0, pos);
		Utilities::replace("#",",",cmd_line);
		str_line.erase(0, pos + 1);
	}
	else
	{
		error_msg("Did not find x and y expressions for plot_xy command in USER_GRAPH", STOP);
	}

	// new curve
	CurveObject new_curve;

	// process plot_xy options
	while (Utilities::replace(" ","#",str_line));
	while (Utilities::replace("\t","#",str_line));
	std::string tok;
	std::string::iterator b = str_line.begin();
	std::string::iterator e = str_line.end();

	while (CParser::copy_title(tok, b, e) != CParser::TT_EMPTY)
	{
		while (Utilities::replace("#"," ",tok));
		while (Utilities::replace("="," ",tok));
		std::string::iterator b1 = tok.begin();
		std::string::iterator e1 = tok.end();
		std::string tok1, tok2;
		CParser::copy_token(tok1, b1, e1);
		Utilities::str_tolower(tok1);
		CParser::copy_token(tok2, b1, e1);
		if (!strncmp(tok1.c_str(), "color", 5))
		{
			//Curves[curvenr + ColumnOffset].Set_color(tok2);
			new_curve.Set_color(tok2);
			continue;
		}
		else if (!strncmp(tok1.c_str(), "symbol", 5) && (strstr(tok1.c_str(), "_si") == NULL)
			&& (strstr(tok1.c_str(), "-si") == NULL))
		{
			new_curve.Set_symbol(tok2);
			continue;
		}
		else if (!strncmp(tok1.c_str(), "symbol_size", 8) || !strncmp(tok1.c_str(), "symbol-size", 8))
		{
			new_curve.Set_symbol_size(atof(tok2.c_str()));
			continue;
		}
		else if (!strncmp(tok1.c_str(), "line_w", 5) || !strncmp(tok1.c_str(), "line-w", 5))
		{
			new_curve.Set_line_w(atof(tok2.c_str()));
			continue;
		}
		else if (!strncmp(tok1.c_str(), "y_axis", 5) || !strncmp(tok1.c_str(), "y-axis", 5))
		{
			new_curve.Set_y_axis(atoi(tok2.c_str()));
			continue;
		}
		else
		{
			std::ostringstream estream;
			estream << "Unknown input for plot_xy in USER_GRAPH " << tok << std::endl;
			warning_msg(estream.str().c_str());
			continue;
		}
	}
	
	// Add new curve to chart
	this->Curves.push_back(new_curve);
}
void 
ChartObject::Set_rate_struct(void)
{
	rate_free(&user_graph);

	std::list<std::string>::iterator it = rate_command_list.begin();
	std::ostringstream oss;
	for (; it != rate_command_list.end(); it++)
	{
		oss << *it << std::endl;
	}
	this->user_graph.commands = (char *) PHRQ_malloc((strlen(oss.str().c_str()) + 2) * sizeof(char));
	::strcpy(this->user_graph.commands, oss.str().c_str());
	this->user_graph.new_def = this->rate_new_def;
	this->user_graph.loopbase = NULL;
	this->user_graph.varbase = NULL;
	this->user_graph.linebase = NULL;
	this->user_graph.name = NULL;
}
ZedGraph::SymbolType 
ChartObject::Return_SymbolType(const std::string sym)
{
	int i;
	std::map<std::string, int>::iterator it;
	if (this->Symbol_map.find(sym) != this->Symbol_map.end())
	{
		i = it->second;
	}
	else
	{
		i = default_symbol++;
		default_symbol = default_symbol % this->Symbol_map.size();
	}

	switch (i)
	{
		case 0:
			return SymbolType::Square;
			break;
		case 1:
			return SymbolType::Diamond;
			break;
		case 2:
			return SymbolType::Triangle;
			break;
		case 3:
			return SymbolType::Circle;
			break;
		case 4:
			return SymbolType::XCross;
			break;
		case 5:
			return SymbolType::Plus;
			break;
		case 6:
			return SymbolType::Star;
			break;
		case 7:
			return SymbolType::TriangleDown;
			break;
		case 8:
			return SymbolType::HDash;
			break;
		case 9:
			return SymbolType::VDash;
			break;
		case 10:
			return SymbolType::None;
			break;
		default:
			return SymbolType::Default;
			break;
	}
}
void 
ChartObject::SaveCurvesToFile(std::string &)
{
}
void 
ChartObject::PlotXY(std::string x, std::string y)
{
	/* Attribute values from *x and *y to Curves(*x, *y) */
	int curvenr = colnr + ColumnOffset;

#ifdef SKIP	   
	int i, i2, i3;
	bool new_sim = false, new_trans = false;
	if ((state == TRANSPORT && transport_step != prev_transport_step) ||
		(state == ADVECTION && advection_step != prev_advection_step))
		new_trans = true;
	if (FirstCallToUSER_GRAPH && colnr == 0)
		prev_sim_no = simulation;
	else
		if (!rownr && (simulation != prev_sim_no || new_trans))
		{
			new_sim = true;
			if (!connect_simulations)
				AddSeries = TRUE;
		}
	prev_sim_no = simulation;

	int curvenr = colnr + ColumnOffset;

	if (curvenr >= ncurves)
		ReallocCurves(0);
	if (curvenr + 1 > ncurves_changed[2]) /* timer must recall DefineCurves in Form */
	{
		ncurves_changed[0] = 1;
		ncurves_changed[1] = ncurves_changed[2];
		ncurves_changed[2] = curvenr + 1;
	}
	if (x_filled && user_graph_count_headings > curvenr + ColumnOffset)
	{
		PHRQ_free(Curves[curvenr + ColumnOffset].id);
		Curves[curvenr + ColumnOffset].id =
			string_duplicate(user_graph_headings[curvenr + ColumnOffset]);
	}

	/* If a new simulation, create new set of curves,
	   define identifiers, y axis from values set in ExtractCurveInfo... */
	if (rownr == 0 && colnr == 0)
	{
		if (new_sim && AddSeries && (!connect_simulations || new_ug))
		{ /* step to new curveset... */
			if (Curves[ncurves - 1].npoints)
				ReallocCurves(ncurves * 2);
			for (i = curvenr; i < ncurves; i++)
			{
				if (Curves[i].npoints)
					continue;
				else
				{
				/* curve i is free... */
					i2 = i3 = ColumnOffset;
					ColumnOffset = curvenr = i;
					break;
				}
			}
			if (new_trans && !new_ug) i3 = 0;
			if (new_ug) i2 = 0;
			/* fill in curve properties... */
			for (i = ColumnOffset; i < ColumnOffset + (ColumnOffset - i2); i++)
			{
				if (i >= ncurves)
					ReallocCurves(0);
				/* define the new curve... */
				if (i3 < user_graph_count_headings)
				{
					PHRQ_free(Curves[i].id);
					Curves[i].id = string_duplicate(user_graph_headings[i3]);
				}
				//Curves[i].color = Curves[i3].color;
				//Curves[i].line_w = Curves[i3].line_w;
				//Curves[i].symbol = Curves[i3].symbol;
				//Curves[i].symbol_size = Curves[i3].symbol_size;
				Curves[i].y_axis = Curves[i3].y_axis;
				i3++;
			}
		}
		/* Or, add all to existing set... */
		else if (new_sim)
		{
			RowOffset = 1;
			for (i = 0; i < ncurves; i++) Curves[i].prev_npoints = Curves[i].npoints;
		}
		new_ug = false;
	}
#endif
	/* return if x or y is a zero... */
	if (x.size() == 0 || y.size() == 0) return;

	/* fill in Curves(x, y)... */
	//if (Curves[curvenr].npoints >= Curves[curvenr].nxy)
	//	ReallocCurveXY(curvenr);
	Curves[curvenr].Get_x().push_back(atof(x.c_str()));
	Curves[curvenr].Get_y().push_back(atof(y.c_str()));
	//Curves[curvenr].npoints++;

}


bool
ChartObject::start_chart(void)
{
	//if (end)
	//	goto end_chart;
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(true); 

#if defined(PHREEQC_CLASS)
	Thread ^t = gcnew Thread(
                gcnew ParameterizedThreadStart(Form1::ThreadForm));
#else 
	Thread ^t = gcnew Thread(
                gcnew ParameterizedThreadStart(Form1::ThreadForm));
#endif
//#else
//	Thread ^t = gcnew Thread( gcnew ThreadStart( &Form1::ThreadForm));
//#endif
	t->SetApartmentState(ApartmentState::STA);
	t->IsBackground = false;
	t->Priority = ThreadPriority::Normal;
#if PHREEQC_CLASS
	PhreeqcObj ^p = gcnew PhreeqcObj(this);
	t->Start(p);
#else
	t->Start();
#endif
	//Thread::Sleep( 1 ); /* this when debugging... */
	//_beginthread(void (Form1::ThreadForm), 0, NULL);
	return true;
}
#endif // MULTICHART