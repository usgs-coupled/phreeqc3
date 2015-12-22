#include "Parallelizer.h"
#include "IPhreeqcPhast.h"
#include "IPhreeqc.h"
#include "PHRQ_io.h"
#include "StorageBin.h"
#include "Phreeqc.h"
#include "Solution.h"
#include "Exchange.h"
#include "GasPhase.h"
#include "cxxKinetics.h"
#include "PPassemblage.h"
#include "SSassemblage.h"
#include "Surface.h"
#include "Temperature.h"
#include "Pressure.h"
#include <assert.h>
#ifdef USE_OPENMP
#include <omp.h>
#endif
#ifdef USE_MPI
#include "Serializer.h"
#endif

Parallelizer::Parallelizer(int nxyz_arg, MP_TYPE data_for_parallel_processing, PHRQ_io *io)
: PhreeqcRM(nxyz_arg, data_for_parallel_processing, io)
{
	this->phreeqc_ptr = NULL;
}
Parallelizer::~Parallelizer(void)
{

}

IRM_RESULT Parallelizer::Initialize()
{
	this->LoadDatabase(this->phreeqc_ptr->user_database); 
	this->SetRebalanceFraction(0.0);
	this->SetScreenOn(false);
	std::vector<int> mask;
	for (int i = 0; i < this->count_chemistry; i++)
	{
		mask.push_back(phreeqc_ptr->cell_data[i].print);
	}
	this->SetPrintChemistryMask(mask);
	return IRM_OK;
}
#ifdef USE_MPI
//#define USE_DUMP
#ifdef USE_DUMP
IRM_RESULT Parallelizer::Phreeqc2RM(Phreeqc *phreeqc_ptr)
{
    int method_number = 1000;
	if (this->GetMpiMyself() == 0)
	{
		MPI_Bcast(&method_number, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
	}
	for (int i = 0; i < this->GetMpiTasks(); i++)
	{
		cxxStorageBin sb;
		if (this->GetMpiMyself() == 0)
		{
			for (int j = this->start_cell[i]; j <= this->end_cell[i]; j++)
			{
				phreeqc_ptr->phreeqc2cxxStorageBin(sb, j);
			}
		}
		if (this->GetMpiMyself() == i && i == 0)
		{
			this->workers[0]->Get_PhreeqcPtr()->cxxStorageBin2phreeqc(sb);
		}
		else if (this->GetMpiMyself() == 0)
		{
			// make raw_stream for transfer
			std::ostringstream raw_stream;
			sb.dump_raw(raw_stream, 0);
			size_t string_size = raw_stream.str().size() + 1;
			MPI_Send(&string_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send((void *) raw_stream.str().c_str(), (int) string_size, MPI_CHAR, i, 0, MPI_COMM_WORLD);
		}
		else if (this->GetMpiMyself() == i)
		{
			MPI_Status mpi_status;
			int string_size;;
			MPI_Recv(&string_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &mpi_status);
			char *string_buffer = new char[string_size];
			MPI_Recv((void *) string_buffer, string_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &mpi_status);
			IPhreeqcPhast * phast_iphreeqc_worker = this->workers[0];
			int status = phast_iphreeqc_worker->RunString(string_buffer);
			if (status != 0)
			{
				this->ErrorMessage(phast_iphreeqc_worker->GetErrorString());
			}
			delete [] string_buffer;
		}
	}
	return IRM_OK;
}
#else   
//Mpi_pack
IRM_RESULT Parallelizer::Phreeqc2RM(Phreeqc *phreeqc_ptr)
{
    int method_number = 1000;
	if (this->GetMpiMyself() == 0)
	{
		MPI_Bcast(&method_number, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
	}
	for (int i = 0; i < this->GetMpiTasks(); i++)
	{
		if (this->GetMpiMyself() == 0 || this->GetMpiMyself() == i)
		{

			if (this->GetMpiMyself() == i && i == 0)
			{
				Serializer serial;
				serial.Serialize(*phreeqc_ptr, this->start_cell[i], this->end_cell[i], false, false);
				serial.Deserialize(*this->GetWorkers()[0]->Get_PhreeqcPtr(), serial.GetDictionary(), serial.GetInts(), serial.GetDoubles());
				continue;
			}
			if (this->GetMpiMyself() == 0)
			{
#define PACK
#if !defined(PACK)
				Serializer serial;
				serial.Serialize(*phreeqc_ptr, this->start_cell[i], this->end_cell[i], false, false);
				int sizes[3];
				sizes[0] = (int) serial.GetDictionary().GetDictionaryOss().str().size();
				sizes[1] = (int) serial.GetInts().size();
				sizes[2] = (int) serial.GetDoubles().size();
				MPI_Send(&sizes[0], 3, MPI_INT, i, 0, MPI_COMM_WORLD);
				MPI_Send(serial.GetDictionary().GetDictionaryOss().str().c_str(), sizes[0], MPI_CHAR, i, 0, MPI_COMM_WORLD);
				MPI_Send(&serial.GetInts()[0], sizes[1], MPI_INT, i, 0, MPI_COMM_WORLD);
				MPI_Send(&serial.GetDoubles()[0], sizes[2], MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
#else
				Serializer serial;
				serial.Serialize(*phreeqc_ptr, this->start_cell[i], this->end_cell[i], false, false);
				int sizes[4];
				
				int total_size = (int) (serial.GetDictionary().GetDictionaryOss().str().size() * sizeof(char));
				total_size += (int) (serial.GetInts().size() * sizeof(int));
				total_size += (int) (serial.GetDoubles().size() * sizeof(double));
				total_size += 1000;
				int position=0;
				char *buffer = new char[total_size];

				MPI_Pack(serial.GetDictionary().GetDictionaryOss().str().c_str(), (int) serial.GetDictionary().GetDictionaryOss().str().size(), 
					MPI_CHAR, buffer, total_size, &position, MPI_COMM_WORLD);
				MPI_Pack(&(serial.GetInts()[0]), (int) serial.GetInts().size(), 
					MPI_INT, buffer, total_size, &position, MPI_COMM_WORLD);
				MPI_Pack(&(serial.GetDoubles()[0]), (int) serial.GetDoubles().size(), 
					MPI_DOUBLE, buffer, total_size, &position, MPI_COMM_WORLD);

				sizes[0] = position;
				sizes[1] = (int) serial.GetDictionary().GetDictionaryOss().str().size();
				sizes[2] = (int) serial.GetInts().size();
				sizes[3] = (int) serial.GetDoubles().size();
				MPI_Send(&sizes, 4, MPI_INT, i, 0, MPI_COMM_WORLD);
				MPI_Send(buffer, position, MPI_PACKED, i, 0, MPI_COMM_WORLD);
				delete [] buffer;
#endif
			}
			else if (this->GetMpiMyself() == i)
			{
#if !defined(PACK)
				MPI_Status mpi_status;
				int sizes[3];
				MPI_Recv(&sizes[0], 3, MPI_INT, 0, 0, MPI_COMM_WORLD, &mpi_status);
				Serializer serial;
				std::string  string_buffer;
				string_buffer.resize(sizes[0]);
				MPI_Recv((void *) &string_buffer[0], sizes[0], MPI_CHAR, 0, 0, MPI_COMM_WORLD, &mpi_status);
				Dictionary dictionary(string_buffer);
				std::vector<int> ints;
				ints.resize(sizes[1], 0);
				MPI_Recv((void *) &(ints[0]), sizes[1] + 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &mpi_status);
				std::vector<double> doubles;
				doubles.resize(sizes[2],0.0);
				MPI_Recv((void *) &doubles[0], sizes[2], MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &mpi_status);
				IPhreeqcPhast * phast_iphreeqc_worker = this->workers[0];
				serial.Deserialize(*phast_iphreeqc_worker->Get_PhreeqcPtr(), dictionary, ints, doubles);
#else
				MPI_Status mpi_status;
				int sizes[4];
				MPI_Recv(&sizes[0], 4, MPI_INT, 0, 0, MPI_COMM_WORLD, &mpi_status);

				Serializer serial;
				std::string  string_buffer;
				string_buffer.resize(sizes[1]);
				std::vector<int> ints;
				ints.resize(sizes[2], 0);
				std::vector<double> doubles;
				doubles.resize(sizes[3],0.0);
				char *buffer = new char[sizes[0]];
				MPI_Recv((void *) buffer, sizes[0], MPI_PACKED, 0, 0, MPI_COMM_WORLD, &mpi_status);

				int position = 0;
				MPI_Unpack(buffer, sizes[0], &position, &(string_buffer[0]), sizes[1],
					MPI_CHAR, MPI_COMM_WORLD);
				MPI_Unpack(buffer, sizes[0], &position, &(ints[0]), sizes[2],
					MPI_INT, MPI_COMM_WORLD);
				MPI_Unpack(buffer, sizes[0], &position, &(doubles[0]), sizes[3],
					MPI_DOUBLE, MPI_COMM_WORLD);

				Dictionary dictionary(string_buffer);
				IPhreeqcPhast * phast_iphreeqc_worker = this->workers[0];
				serial.Deserialize(*phast_iphreeqc_worker->Get_PhreeqcPtr(), dictionary, ints, doubles);
				delete [] buffer;
#endif
			}
		}
	}
	return IRM_OK;
}
#endif
#else
IRM_RESULT Parallelizer::Phreeqc2RM(Phreeqc *phreeqc_ptr)
{
	// move data to workers
#ifdef USE_OPENMP
		omp_set_num_threads(this->GetThreadCount());
#pragma omp parallel
#pragma omp for
#endif
	for (int ii = 0; ii < this->GetThreadCount(); ii++)
	{
		Phreeqc * rmphreeqc_ptr = this->GetWorkers()[ii]->Get_PhreeqcPtr();
		for (int jj = this->start_cell[ii]; jj <= this->end_cell[ii]; jj++)
		{
			{
				cxxSolution *entity_ptr = Utilities::Rxn_find(phreeqc_ptr->Get_Rxn_solution_map(), jj);
				rmphreeqc_ptr->Get_Rxn_solution_map()[jj] = *entity_ptr;
			}

			// Exchangers
			{
				cxxExchange *entity_ptr = Utilities::Rxn_find(phreeqc_ptr->Get_Rxn_exchange_map(), jj);
				if (entity_ptr != NULL)
				{
					this->GetWorkers()[ii]->Get_PhreeqcPtr()->Get_Rxn_exchange_map()[jj] = *entity_ptr;
				}
			}

			// GasPhases
			{
				cxxGasPhase *entity_ptr = Utilities::Rxn_find(phreeqc_ptr->Get_Rxn_gas_phase_map(), jj);
				if (entity_ptr != NULL)
				{
					this->GetWorkers()[ii]->Get_PhreeqcPtr()->Get_Rxn_gas_phase_map()[jj] = *entity_ptr;
				}
			}

			// Kinetics
			{
				cxxKinetics *entity_ptr = Utilities::Rxn_find(phreeqc_ptr->Get_Rxn_kinetics_map(), jj);
				if (entity_ptr != NULL)
				{
					this->GetWorkers()[ii]->Get_PhreeqcPtr()->Get_Rxn_kinetics_map()[jj] = *entity_ptr;
				}
			}
			// PPassemblages
			{
				cxxPPassemblage *entity_ptr = Utilities::Rxn_find(phreeqc_ptr->Get_Rxn_pp_assemblage_map(), jj);
				if (entity_ptr != NULL)
				{
					this->GetWorkers()[ii]->Get_PhreeqcPtr()->Get_Rxn_pp_assemblage_map()[jj] = *entity_ptr;
				}
			}
			// SSassemblages
			{
				cxxSSassemblage *entity_ptr = Utilities::Rxn_find(phreeqc_ptr->Get_Rxn_ss_assemblage_map(), jj);
				if (entity_ptr != NULL)
				{
					this->GetWorkers()[ii]->Get_PhreeqcPtr()->Get_Rxn_ss_assemblage_map()[jj] = *entity_ptr;
				}
			}
			// Surfaces
			{
				cxxSurface *entity_ptr = Utilities::Rxn_find(phreeqc_ptr->Get_Rxn_surface_map(), jj);
				if (entity_ptr != NULL)
				{
					this->GetWorkers()[ii]->Get_PhreeqcPtr()->Get_Rxn_surface_map()[jj] = *entity_ptr;
				}
			}
			// Temperature
			{
				cxxTemperature *entity_ptr = Utilities::Rxn_find(phreeqc_ptr->Get_Rxn_temperature_map(), jj);
				if (entity_ptr != NULL)
				{
					this->GetWorkers()[ii]->Get_PhreeqcPtr()->Get_Rxn_temperature_map()[jj] = *entity_ptr;
				}
			}
			// Pressure
			{
				cxxPressure *entity_ptr = Utilities::Rxn_find(phreeqc_ptr->Get_Rxn_pressure_map(), jj);
				if (entity_ptr != NULL)
				{
					this->GetWorkers()[ii]->Get_PhreeqcPtr()->Get_Rxn_pressure_map()[jj] = *entity_ptr;
				}
			}
		}
	}
	return IRM_OK;
}


#endif
#ifdef USE_MPI
IRM_RESULT Parallelizer::RM2Phreeqc(Phreeqc *phreeqc_ptr)
//Serialize
{
    int method_number = 1001;
	if (this->GetMpiMyself() == 0)
	{
		MPI_Bcast(&method_number, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
	}
	for (int i = 0; i < this->GetMpiTasks(); i++)
	{
		if (this->GetMpiMyself() == 0 || this->GetMpiMyself() == i)
		{

			if (this->GetMpiMyself() == i && i == 0)
			{
				Serializer serial;
				serial.Serialize(*this->GetWorkers()[0]->Get_PhreeqcPtr(), this->start_cell[i], this->end_cell[i], false, false);
				serial.Deserialize(*phreeqc_ptr, serial.GetDictionary(), serial.GetInts(), serial.GetDoubles());
				continue;
			}
			if (this->GetMpiMyself() == 0)
			{
				MPI_Status mpi_status;
				int sizes[4];
				MPI_Recv(&sizes[0], 4, MPI_INT, i, 0, MPI_COMM_WORLD, &mpi_status);

				Serializer serial;
				std::string  string_buffer;
				string_buffer.resize(sizes[1]);
				std::vector<int> ints;
				ints.resize(sizes[2], 0);
				std::vector<double> doubles;
				doubles.resize(sizes[3],0.0);
				char *buffer = new char[sizes[0]];
				MPI_Recv((void *) buffer, sizes[0], MPI_PACKED, i, 0, MPI_COMM_WORLD, &mpi_status);

				int position = 0;
				MPI_Unpack(buffer, sizes[0], &position, &(string_buffer[0]), sizes[1],
					MPI_CHAR, MPI_COMM_WORLD);
				MPI_Unpack(buffer, sizes[0], &position, &(ints[0]), sizes[2],
					MPI_INT, MPI_COMM_WORLD);
				MPI_Unpack(buffer, sizes[0], &position, &(doubles[0]), sizes[3],
					MPI_DOUBLE, MPI_COMM_WORLD);

				Dictionary dictionary(string_buffer);
				serial.Deserialize(*phreeqc_ptr, dictionary, ints, doubles);
				delete [] buffer;
			}
			else if (this->GetMpiMyself() == i)
			{
				Serializer serial;
				serial.Serialize(*this->GetWorkers()[0]->Get_PhreeqcPtr(), this->start_cell[i], this->end_cell[i], false, false);
				int sizes[4];
				int total_size = (int) (serial.GetDictionary().GetDictionaryOss().str().size() * sizeof(char));
				total_size += (int) (serial.GetInts().size() * sizeof(int));
				total_size += (int) (serial.GetDoubles().size() * sizeof(double));
				total_size += 1000;
				int position=0;
				char *buffer = new char[total_size];

				MPI_Pack(serial.GetDictionary().GetDictionaryOss().str().c_str(), (int) serial.GetDictionary().GetDictionaryOss().str().size(), 
					MPI_CHAR, buffer, total_size, &position, MPI_COMM_WORLD);
				MPI_Pack(&(serial.GetInts()[0]), (int) serial.GetInts().size(), 
					MPI_INT, buffer, total_size, &position, MPI_COMM_WORLD);
				MPI_Pack(&(serial.GetDoubles()[0]), (int) serial.GetDoubles().size(), 
					MPI_DOUBLE, buffer, total_size, &position, MPI_COMM_WORLD);

				sizes[0] = position;
				sizes[1] = (int) serial.GetDictionary().GetDictionaryOss().str().size();
				sizes[2] = (int) serial.GetInts().size();
				sizes[3] = (int) serial.GetDoubles().size();
				MPI_Send(&sizes, 4, MPI_INT, 0, 0, MPI_COMM_WORLD);
				MPI_Send(buffer, position, MPI_PACKED, 0, 0, MPI_COMM_WORLD);
				delete [] buffer;
			}
		}
	}
	return IRM_OK;
}
#else
IRM_RESULT Parallelizer::RM2Phreeqc(Phreeqc *phreeqc_ptr)
{
	// move data to workers
#ifdef USE_OPENMP
		omp_set_num_threads(this->GetThreadCount());
#pragma omp parallel
#pragma omp for
#endif
	for (int ii = 0; ii < this->GetThreadCount(); ii++)
	{
		for (int jj = this->start_cell[ii]; jj <= this->end_cell[ii]; jj++)
		{
			{
				cxxSolution *entity_ptr = &this->GetWorkers()[ii]->Get_PhreeqcPtr()->Get_Rxn_solution_map()[jj];
				phreeqc_ptr->Get_Rxn_solution_map() [jj] = *entity_ptr;
			}

			// Exchangers
			{
				cxxExchange *entity_ptr = Utilities::Rxn_find(phreeqc_ptr->Get_Rxn_exchange_map(), jj);
				if (entity_ptr != NULL)
				{
					this->GetWorkers()[ii]->Get_PhreeqcPtr()->Get_Rxn_exchange_map()[jj] = *entity_ptr;
				}
			}

			// GasPhases
			{
				cxxGasPhase *entity_ptr = Utilities::Rxn_find(phreeqc_ptr->Get_Rxn_gas_phase_map(), jj);
				if (entity_ptr != NULL)
				{
					this->GetWorkers()[ii]->Get_PhreeqcPtr()->Get_Rxn_gas_phase_map()[jj] = *entity_ptr;
				}
			}

			// Kinetics
			{
				cxxKinetics *entity_ptr = Utilities::Rxn_find(phreeqc_ptr->Get_Rxn_kinetics_map(), jj);
				if (entity_ptr != NULL)
				{
					this->GetWorkers()[ii]->Get_PhreeqcPtr()->Get_Rxn_kinetics_map()[jj] = *entity_ptr;
				}
			}
			// PPassemblages
			{
				cxxPPassemblage *entity_ptr = Utilities::Rxn_find(phreeqc_ptr->Get_Rxn_pp_assemblage_map(), jj);
				if (entity_ptr != NULL)
				{
					this->GetWorkers()[ii]->Get_PhreeqcPtr()->Get_Rxn_pp_assemblage_map()[jj] = *entity_ptr;
				}
			}
			// SSassemblages
			{
				cxxSSassemblage *entity_ptr = Utilities::Rxn_find(phreeqc_ptr->Get_Rxn_ss_assemblage_map(), jj);
				if (entity_ptr != NULL)
				{
					this->GetWorkers()[ii]->Get_PhreeqcPtr()->Get_Rxn_ss_assemblage_map()[jj] = *entity_ptr;
				}
			}
			// Surfaces
			{
				cxxSurface *entity_ptr = Utilities::Rxn_find(phreeqc_ptr->Get_Rxn_surface_map(), jj);
				if (entity_ptr != NULL)
				{
					this->GetWorkers()[ii]->Get_PhreeqcPtr()->Get_Rxn_surface_map()[jj] = *entity_ptr;
				}
			}
			// Temperature
			{
				cxxTemperature *entity_ptr = Utilities::Rxn_find(phreeqc_ptr->Get_Rxn_temperature_map(), jj);
				if (entity_ptr != NULL)
				{
					this->GetWorkers()[ii]->Get_PhreeqcPtr()->Get_Rxn_temperature_map()[jj] = *entity_ptr;
				}
			}
			// Pressure
			{
				cxxPressure *entity_ptr = Utilities::Rxn_find(phreeqc_ptr->Get_Rxn_pressure_map(), jj);
				if (entity_ptr != NULL)
				{
					this->GetWorkers()[ii]->Get_PhreeqcPtr()->Get_Rxn_pressure_map()[jj] = *entity_ptr;
				}
			}
		}
	}
	return IRM_OK;
}
#endif

IRM_RESULT Parallelizer::RunCellsParallel()
{

	bool punch = (this->phreeqc_ptr->transport_step % this->phreeqc_ptr->punch_modulus == 0);
	this->SetSelectedOutputOn(punch);
	bool print = (this->phreeqc_ptr->transport_step % this->phreeqc_ptr->print_modulus == 0);
	this->SetPrintChemistryOn(print, print, print);
	this->RunCells();			
	if (punch)
	{		
		for (int isel = 0; isel < this->GetSelectedOutputCount(); isel++)
		{
			// Loop through possible multiple selected output definitions
			int n_user = this->GetNthSelectedOutputUserNumber(isel);
			int status = this->SetCurrentSelectedOutputUserNumber(n_user);
			phreeqc_ptr->current_selected_output = &phreeqc_ptr->SelectedOutput_map[n_user];
			if (phreeqc_ptr->pr.punch == FALSE ||
				phreeqc_ptr->current_selected_output == NULL ||
				!phreeqc_ptr->current_selected_output->Get_active() )
			{
				continue;
			}
			phreeqc_ptr->phrq_io->Set_punch_ostream(phreeqc_ptr->current_selected_output->Get_punch_ostream());
			std::vector<double> so;	
			int col = this->GetSelectedOutputColumnCount();
			status = this->GetSelectedOutput(so);
			for (int i = 0; i < this->GetSelectedOutputRowCount(); i++)
			{
				if (this->phreeqc_ptr->cell_data[i].punch != 0)
				{
					std::ostringstream os;
					for(int j = 0; j < this->GetSelectedOutputColumnCount(); j++)
					{
						os << so[j*this->GetSelectedOutputRowCount() + i] << " ";
					}
					os << "\n";
					this->phreeqcrm_io->punch_msg(os.str().c_str());
				}
			}
		}
	}
	return IRM_OK;
}