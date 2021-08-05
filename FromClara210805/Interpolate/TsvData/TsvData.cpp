#include "TsvData.h"
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#define MISSING_VALUE 1.23456e+99

TsvData *TsvData::Instance=NULL;

TsvData::TsvData(const char * file_name)
{
	std::string line;

	std::ifstream xyz_file;
	xyz_file.open(file_name);
	if (!xyz_file.is_open())
	{
		std::cerr << "Could not open file. " << file_name << std::endl;
	}

	// read header
	std::getline(xyz_file, line);
	{
		std::istringstream strm_line(line);
		std::string heading;
		while (strm_line >> heading)
		{
			headings.push_back(heading);
		}
	}

	// read the file and get dimensions
	this->count_data_lines = 0;
	std::vector<double> temp_v;
	temp_v.resize(headings.size(), MISSING_VALUE);

	int file_line = 0;
	// read lines
	while (std::getline(xyz_file, line))
	{
		file_line++;
		// Skip lines
		if (file_line > 1 && file_line < 24) continue;
		{
			std::string::iterator begin = line.begin();
			std::string::iterator end = line.end();
			std::string token;

			for(int i=0; i < headings.size(); i++)
			{
				temp_v[i] = MISSING_VALUE;
			}
			size_t j = 0;
			while(copy_token(token, begin, end))
			{
				sscanf(token.c_str(), "%lf", &(temp_v[j++]));
			}
			if (temp_v[0] < 0) temp_v[0] = 0.0;
			all_data.push_back(temp_v);
		}
		this->count_data_lines++;
	}
}
std::vector<double> &
TsvData::GetDataVector(std::string heading)
{

	int j = -1;
	for (size_t i = 0; i < headings.size(); i++)
	{
		if (this->headings[i] == heading)
		{
			j = (int) i;
			break;
		}
	}
	if (j < 0)
	{
		std::cerr << "Could not find heading " << heading << std::endl;
		data_vector.clear();
		exit(4);
	}
	else
	{
		this->data_vector.resize(this->count_data_lines);
		for (size_t i = 0; i < this->all_data.size(); i++)
		{
			data_vector[i] = all_data[i][j];
		}
	}
	return data_vector;
}
int
TsvData::copy_token(std::string & token,
										std::string::iterator & begin,
										std::string::iterator & end)
{
	token = "";
	if (begin != end)
	{
		std::string::iterator b = begin;
		for (; b < end &&::isspace(*b); ++b);

		begin = b;
		for (; begin < end && !::isspace(*begin); ++begin);

		token.assign(b, begin);
	}
	return (int) token.size();
}
TsvData::~TsvData(void)
{
}
int
TsvData::Interpolate()
{
	TsvData *tsv_ptr = TsvData::GetInstance();
	// control file
	std::string file_name = "interpolator.control";
	std::ifstream control_file;
	control_file.open(file_name);
	if (!control_file.is_open())
	{
		std::cerr << "Could not open interpolator.control." << std::endl;
		exit(4);
	}
	// pst file
	std::string pst_file_name = "all.pst";
	std::ofstream pst_file;
	pst_file.open(pst_file_name);
	if (!pst_file.is_open())
	{
		std::cerr << "Could not open pst file." << std::endl;
		exit(4);
	}
	// read file name of obs and interpolate for .calc file
	
	// read obs file names
	std::string obs_file_name;
	std::string line;
	double wt;
	while (std::getline(control_file, line))
	{
		// read obs_file_name
		std::istringstream str(line);
		str >> obs_file_name;
		str >> wt;
		// open obs file
		std::ifstream obs_file;
		obs_file.open(obs_file_name);
		if (!obs_file.is_open())
		{
			std::cerr << "Could not open " << obs_file_name << std::endl;
			exit(4);
		}
		// Locate column
		std::size_t pos = obs_file_name.find(".");
		std::string analyte = obs_file_name.substr(0,pos);
		// open calc file
		std::ofstream calc_file;
		std::string calc_file_name = analyte + ".calc";
		calc_file.open(calc_file_name);
		if (!calc_file.is_open())
		{
			std::cerr << "Could not open " << calc_file_name << std::endl;
			exit(4);
		}
		// open ins file
		std::ofstream ins_file;
		std::string ins_file_name = analyte + ".ins";
		ins_file.open(ins_file_name);
		if (!ins_file.is_open())
		{
			std::cerr << "Could not open " << ins_file_name << std::endl;
			exit(4);
		}
		ins_file << "pif $" << std::endl;
		size_t i;
		for (i = 0; i < this->headings.size();  i++)
		{
			if (analyte == headings[i]) break;
		}
		if (i == this->headings.size())
		{
			std::cerr << "Could not find analyte " << analyte << std::endl;
			exit(4);
		}
		// interpolate and write files
		while (std::getline(obs_file, line))
		{
			std::istringstream str(line);
			if (line.size() == 0) continue;
			// read obs file line
			std::string name;
			str >> name;
			double x;
			str >> x;
			double v_obs;
			str >> v_obs;
			// interpolate to obs x
			size_t j;
			double x1,x2,v1,v2,v;
			for (j = 0; j < this->all_data.size() - 1; j++)
			{
				if (all_data[j][0] <= x && all_data[j+1][0] >= x) 
				{
					x1 = all_data[j][0];
					v1 = all_data[j][i];
					x2 = all_data[j+1][0]; 
					v2 = all_data[j+1][i];
					v = (v1*(x2-x) + v2*(x-x1)) / (x2-x1);
					break;
				}
			}
			if (j == this->all_data.size())
			{
				std::cerr << "Could not find bracket x " << x << std::endl;
				exit(4);
			}
			// calc file
			calc_file << std::setw(20) << v << "  " << std::setw(10) << x << "  " << name << std::endl;
			// pst file
			pst_file << std::setw(10) << name << "  " << std::setw(20) <<  v_obs << "  " <<std::setw(10) << wt << "  " << analyte << std::endl;
			// ins file
			ins_file << "l1 [" << name << "]1:20" << std::endl;
		}
		calc_file.close();
		ins_file.close();
	}
	pst_file.close();
	control_file.close();
	return 1;
}