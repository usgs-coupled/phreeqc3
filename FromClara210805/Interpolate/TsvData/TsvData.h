#pragma once
#include <iostream>
#include <set>
#include <vector>
#include <map>
class TsvData
{
public:
	TsvData(const char * file_name);
	~TsvData(void);
	static TsvData * GetInstance() {return TsvData::Instance;}
	int GetCountDataLines() {return (int) this->count_data_lines;}
	std::vector<int> & IxAllDataFortran() {return this->ix_all_data_fortran;}
	std::vector<int> & IyAllDataFortran() {return this->iy_all_data_fortran;}
	std::vector<int> & IzAllDataFortran() {return this->iz_all_data_fortran;}
	std::vector<int> & ItAllDataFortran() {return this->it_all_data_fortran;}
	
	std::set<double> & GetXLoc() {return this->x_loc;}
	std::set<double> & GetYLoc() {return this->y_loc;}
	std::set<double> & GetZLoc() {return this->z_loc;}
	std::set<double> & GetTLoc() {return this->t_loc;}
	int Interpolate(void);
	std::vector<double> & GetDataVector(std::string heading);
	int copy_token(std::string & token,
		std::string::iterator & begin,
		std::string::iterator & end);
protected:
	std::set<double>    x_loc,  y_loc,  z_loc,  t_loc;
	std::vector<std::string> headings;
	std::vector <std::vector <double> > all_data;
	std::map<double,int> fortran_x_index_map;
	std::map<double,int> fortran_y_index_map;
	std::map<double,int> fortran_z_index_map;
	std::map<double,int> fortran_t_index_map;
	std::vector<int> ix_all_data_fortran;
	std::vector<int> iy_all_data_fortran;
	std::vector<int> iz_all_data_fortran;
	std::vector<int> it_all_data_fortran;
	std::vector<double> data_vector;
	size_t count_xyz;
	size_t count_data_lines;
public:
	static TsvData *Instance;
};

