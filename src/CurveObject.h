#if !defined(CURVEOBJECT_H_INCLUDED)
#define CURVEOBJECT_H_INCLUDED
#include <vector>
#include <string>

class CurveObject
{

public:
	CurveObject();
	~CurveObject();
	void Set_id(std::string s)
	{
		this->id = s;
	}
	std::string &Get_id(void)
	{
		return this->id;
	}
	void Set_color(std::string s)
	{
		this->color = s;
	}
	std::string &Get_color(void)
	{
		return this->color;
	}
	void Set_symbol(std::string s)
	{
		this->symbol = s;
	}
	std::string &Get_symbol(void)
	{
		return this->symbol;
	}
	void Set_symbol_size(double f)
	{
		this->symbol_size = f;
	}
	double Get_symbol_size(void)
	{
		return this->symbol_size;
	}
	void Set_line_w(double f)
	{
		this->line_w = f;
	}
	double Get_line_w(void)
	{
		return this->line_w;
	}
	void Set_y_axis(int f)
	{
		this->y_axis = f;
	}
	std::vector<double> & Get_x()
	{
		return this->x;
	}
	std::vector<double> & Get_y()
	{
		return this->y;
	}
	int Get_y_axis()
	{
		return this->y_axis;
	}
	void Set_npoints_plot(int f)
	{
		this->npoints_plot = f;
	}
	int Get_npoints_plot(void)
	{
		return this->npoints_plot;
	}

protected:
	//float *x, *y;
	std::vector<double> x, y;
	int nxy, npoints, npoints_plot, prev_npoints;

	std::string id, color, symbol;
	int y_axis; 
	double line_w, symbol_size;

public:

};

#endif // !defined(CURVEOBJECT_H_INCLUDED)
