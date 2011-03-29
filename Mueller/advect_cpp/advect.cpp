#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <IPhreeqc.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <time.h>

void EHandler(int id);
void initial_conditions(int id, int ncells);
void make_selected_output(int id, std::vector<std::string> components, std::vector<std::string> &headings);
void extract_selected_output(int id, std::vector<std::vector<double>> &conc);
void write_outflow(int id, std::vector<std::vector<double>> &conc, std::ofstream &selected_file);
void advect_step(int id, int cells, std::vector<std::vector<double>> &conc, std::vector<std::string> &headings);

int main(void)
{
	int ncells=40;
	int shifts=120;
	std::ofstream selected_file("out.txt");

	// Problem definition
	std::cout << "Dimensions" << std::endl;
    std::cout << "==========" << std::endl;
    std::cout << "number of cells:   " << ncells << std::endl;
    std::cout << "number of shifts   " << shifts << std::endl;

	// Begin timing
	double run_time = clock();

	// create IPhreeqc object and load database
	int id = CreateIPhreeqc();
	if (LoadDatabase(id, "phreeqc.dat") != 0) EHandler(id);
	SetOutputFileOn(id, 1);

	// run initial conditions, copy to column
	initial_conditions(id, ncells);

	// Define SELECTED_OUTPUT
	std::vector<std::string> components, headings;
	make_selected_output(id, components, headings);

	// Run initial conditions
	std::ostringstream run_stream;
	run_stream << "RUN_CELLS" << std::endl << "-cells 1-" << ncells << std::endl;
	if (RunString(id, run_stream.str().c_str()) != 0) EHandler(id);
	std::vector<std::vector<double>> conc;  // conc has all selected output values
	extract_selected_output(id, conc);
	write_outflow(id, conc, selected_file);

	// advect for number of shifts
	int i;
	for (i = 0; i < shifts; i++)
	{
		advect_step(id, ncells, conc, headings);
		extract_selected_output(id, conc);
		write_outflow(id, conc, selected_file);
	}

	run_time = (clock() - run_time) / CLOCKS_PER_SEC;
    std::cout << "run time: " << run_time  << " sec" << std::endl;
    std::cout << "Finished simulation" << std::endl;
}

void EHandler(int id)
{
	OutputErrorString(id);
	exit(EXIT_FAILURE);	
}

void initial_conditions(int id, int ncells)
{
	//Specify initial conditions data blocks.
	//Uniform initial conditions are assumed.
	std::ostringstream ic;
	ic << "TITLE Example 11.--Transport and ion exchange." << std::endl;
	ic << "SOLUTION 0  CaCl2" << std::endl;
	ic << "    units            mmol/kgw" << std::endl;
	ic << "    temp             25.0" << std::endl;
	ic << "    pH               7.0     charge" << std::endl;
	ic << "    pe               12.5    O2(g)   -0.68" << std::endl;
	ic << "    Ca               0.6" << std::endl;
	ic << "    Cl               1.2" << std::endl;
	ic << "SOLUTION 1  Initial solution for column" << std::endl;
	ic << "    units            mmol/kgw" << std::endl;
	ic << "    temp             25.0" << std::endl;
	ic << "    pH               7.0     charge" << std::endl;
 	ic << "   pe               12.5    O2(g)   -0.68" << std::endl;
	ic << "    Na               1.0" << std::endl;
	ic << "    K                0.2" << std::endl;
	ic << "    N(5)             1.2" << std::endl;
	ic << "    END" << std::endl;
	ic << "EXCHANGE 1" << std::endl;
	ic << "    equilibrate 1" << std::endl;
	ic << "    X                0.0011" << std::endl;
	ic << "END" << std::endl;
	ic << "COPY cells 1 1-" << ncells << std::endl;
	//std::cout << ic.str().c_str();
	if (RunString(id, ic.str().c_str()) != 0) EHandler(id);
}
void make_selected_output(int id, std::vector<std::string> components, std::vector<std::string> &headings)
{
	// list of components
	int i;
	for (i = 0; i < GetComponentCount(id); i++)
	{
		components.push_back(GetComponent(id, i));
	}

	std::ostringstream sel;
	// SELECTED_OUTPUT, reset false
	sel << "SELECTED_OUTPUT" << std::endl;
	sel << "   -reset false" << std::endl;

	// USER_PUNCH
	sel << "USER_PUNCH" << std::endl;

	// headings
	sel << "   -headings  cb  H  O  ";
	headings.push_back("cb");
	headings.push_back("H");
	headings.push_back("O");	
	std::vector<std::string>::iterator it;
	for (it = components.begin(); it  != components.end(); it++)
	{
		sel << *it << " ";
		headings.push_back(*it);
	}
	sel << std::endl;

	// PUNCH cb, H, O
	sel << "10 w = TOT(\"water\")" << std::endl;
	sel << "20 PUNCH CHARGE_BALANCE, TOTMOLE(\"H\"), TOTMOLE(\"O\")" << std::endl;

	// PUNCH each element
	int lino=30;
	for (it = components.begin(); it  != components.end(); it++)
	{
		sel << lino << " PUNCH w*TOT(\"" << *it << "\")" << std::endl;
		lino += 10;
	}
	if (RunString(id, sel.str().c_str()) != 0) EHandler(id);

}
void extract_selected_output(int id, std::vector<std::vector<double>> &conc)
{
	// assumes all selected output data are double
	VAR v;
	VarInit(&v);

	int i, j;

	// clear conc
	conc.clear();

	// collect selected output values in conc (skip headings)
	int max_row = GetSelectedOutputRowCount(id);
	int max_column = GetSelectedOutputColumnCount(id);
	for (i = 1; i < max_row; i++)
	{
		std::vector<double> row_conc;
		for(j = 0; j < max_column; j++)
		{
			if (GetSelectedOutputValue(id, i, j, &v) == VR_OK) {
				switch (v.type) 
				{
				case TT_DOUBLE:
					row_conc.push_back(v.dVal);
					break;
				case TT_LONG:
				case TT_STRING:
					std::cerr << "Long and string selected output not implemented";
					break;
				}
			} 
			else
			{
				EHandler(id);
			}
			VarClear(&v);
		}
		conc.push_back(row_conc);
    }
}
void write_outflow(int id, std::vector<std::vector<double>> &conc, std::ofstream &selected_file)
{
	// assumes all selected output data are double
	VAR v;
	VarInit(&v);
	static bool write_headings=true;

	int i, j;

	// write headings once, skip cb, H, O
	if (write_headings)
	{
		i = 0;
		for(j = 3; j < GetSelectedOutputColumnCount(id); j++)
		{
			GetSelectedOutputValue(id, i, j, &v);
			selected_file << std::setw(20) << v.sVal;
			VarClear(&v);
		}
		selected_file << std::endl;
		write_headings = false;
	}

	// write last row to selected_file, skip cb, H, O
	size_t ui, uj;
	ui = conc.size() - 1;
	{
		for (uj = 3; uj < conc[uj].size(); uj++)
		{
			selected_file << std::setw(20) << std::setprecision(12) << conc[ui][uj];
		}
		selected_file << std::endl;
	}
}
void advect_step(int id, int cells, std::vector<std::vector<double>> &conc, std::vector<std::string> &headings )
{

	std::ostringstream adv;

	// Copy infilling solution to cell 1
	adv << "COPY solution 0 1" << std::endl;

	// Modify solutions 2 through cells
	size_t i, j;
	for (i = 2; i <= (size_t) cells; i++)
	{
		adv << "SOLUTION_MODIFY " << i << std::endl;
		adv << "\t-cb      " << conc[i-2][0] << std::endl;
		adv << "\t-total_h " << conc[i-2][1] << std::endl;
		adv << "\t-total_o " << conc[i-2][2] << std::endl;
		adv << "\t-totals " << std::endl;
		for (j = 3; j < conc[i-2].size(); j++)
		{
			adv << "\t\t" << headings[j] << "\t" << conc[i-2][j] << std::endl;
		}
	}
	adv << "RUN_CELLS; -cells 1-" << cells << std::endl;
	//std::cout << adv.str().c_str();
	RunString(id, adv.str().c_str());
}
