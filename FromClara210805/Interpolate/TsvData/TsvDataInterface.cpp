#include "TsvData.h"
#include "TsvDataInterface.h"
#include <stdlib.h>
#include <fstream>
#include <string>
#include <sstream>
int
main(int argc, char *argv[])
/*
 *   Main program for PHREEQC
 */
{
	// Get the model results
	std::string file_name = "01_1D.txt";
	TSV_Create(file_name.c_str());

	// Interpolate observations
	TSV_interpolate();
	std::cerr << "Interpolation success." << std::endl;

}
int
TSV_GetAllDataIndices(int *ix_all_data_fortran, int *iy_all_data_fortran, int *iz_all_data_fortran, int *it_all_data_fortran)
/* ---------------------------------------------------------------------- */
{
	TsvData *tsv_ptr = TsvData::GetInstance();
	for(int i = 0; i < tsv_ptr->GetCountDataLines(); i++)
	{
		ix_all_data_fortran[i] = tsv_ptr->IxAllDataFortran()[i];
		iy_all_data_fortran[i] = tsv_ptr->IyAllDataFortran()[i];
		iz_all_data_fortran[i] = tsv_ptr->IzAllDataFortran()[i];
		it_all_data_fortran[i] = tsv_ptr->ItAllDataFortran()[i];
	}
	return 0;
}
int
TSV_GetCoordinateCounts(int *ix, int *iy, int *iz, int *it, int * count_data_lines)
/* ---------------------------------------------------------------------- */
{
	TsvData *tsv_ptr = TsvData::GetInstance();
	*ix = (int) tsv_ptr->GetXLoc().size();
	*iy = (int) tsv_ptr->GetYLoc().size();
	*iz = (int) tsv_ptr->GetZLoc().size();
	*it = (int) tsv_ptr->GetTLoc().size();
	*count_data_lines = (int) tsv_ptr->GetCountDataLines();
	return 0;
}
int
TSV_GetCoordinateLocations(double *x_loc, double *y_loc, double *z_loc, double *t_loc)
/* ---------------------------------------------------------------------- */
{
	TsvData *tsv_ptr = TsvData::GetInstance();
	{
		std::set<double>::iterator it = tsv_ptr->GetXLoc().begin();
		size_t i = 0;
		for(; it !=  tsv_ptr->GetXLoc().end(); it++)
		{
			x_loc[i++] = *it;
		}	
	}
	{
		std::set<double>::iterator it = tsv_ptr->GetYLoc().begin();
		size_t i = 0;
		for(; it !=  tsv_ptr->GetYLoc().end(); it++)
		{
			y_loc[i++] = *it;
		}
	}	
	{
		std::set<double>::iterator it = tsv_ptr->GetZLoc().begin();
		size_t i = 0;
		for(; it !=  tsv_ptr->GetZLoc().end(); it++)
		{
			z_loc[i++] = *it;
		}
	}		
	{
		std::set<double>::iterator it = tsv_ptr->GetTLoc().begin();
		size_t i = 0;
		for(; it !=  tsv_ptr->GetTLoc().end(); it++)
		{
			t_loc[i++] = *it;
		}
	}	
	return 0;
}
int
TSV_GetParameter(char *name, double *all_data_column)
/* ---------------------------------------------------------------------- */
{
	TsvData *tsv_ptr = TsvData::GetInstance();
	std::vector<double> & dv = tsv_ptr->GetDataVector(name);
	double max = 0;
	for (size_t i = 0; i < tsv_ptr->GetCountDataLines(); i++)
	{
		all_data_column[i] = dv[i];
		if (all_data_column[i] > max)
		{
			max = all_data_column[i];
			//			std::cerr << "Max value: " << i << "   " << all_data_column[i] << std::endl;
			//			std::cerr << "\t" << tsv_ptr->IxAllDataFortran()[i] << "   " 
			//				<< tsv_ptr->IyAllDataFortran()[i] << "   "
			//				<< tsv_ptr->IzAllDataFortran()[i] << "   "
			//				<< tsv_ptr->ItAllDataFortran()[i] << std::endl;
		}
	}
	return 0;
}
int
TSV_Create(const char *file_name)
/* ---------------------------------------------------------------------- */
{
	if (TsvData::Instance == NULL)
	{
		TsvData::Instance = new TsvData(file_name);
	}
	return 0;
}
void
TSV_interpolate()
{
	if (TsvData::Instance != NULL)
	{
		TsvData *tsv_ptr = TsvData::GetInstance();
		tsv_ptr->Interpolate();
	}
	return;
}
