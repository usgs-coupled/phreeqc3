#include "Serializer.h"
#include "Phreeqc.h"
#include "Utils.h"
#include "Solution.h"
#include "Exchange.h"
#include "Temperature.h"
#include "GasPhase.h"
#include "cxxKinetics.h"
#include "PPassemblage.h"
#include "SSassemblage.h"
#include "Surface.h"
Serializer::Serializer(void)
{
}
Serializer::~Serializer(void)
{
}
bool Serializer::Serialize(Phreeqc &phreeqc_ref, int start, int end, bool include_t, bool include_p)
{
	this->ints.push_back(end - start + 1);
	for (int i = start; i <= end; i++)
	{
		cxxSolution *soln_ptr = Utilities::Rxn_find(phreeqc_ref.Get_Rxn_solution_map(), i);
		if (soln_ptr)
		{
			ints.push_back(1);
			soln_ptr->mpi_pack(this->dictionary, this->ints, this->doubles);
		}
		else
		{
			ints.push_back(0);
		}
		// Exchangers
		{
		cxxExchange *entity_ptr = Utilities::Rxn_find(phreeqc_ref.Get_Rxn_exchange_map(), i);
		if (entity_ptr)
		{
			ints.push_back(1);
			entity_ptr->mpi_pack(this->dictionary, this->ints, this->doubles);
		}
		else
		{
			ints.push_back(0);
		}
		}
		// GasPhases
		{
		cxxGasPhase *entity_ptr = Utilities::Rxn_find(phreeqc_ref.Get_Rxn_gas_phase_map(), i);
		if (entity_ptr)
		{
			ints.push_back(1);
			entity_ptr->mpi_pack(this->dictionary, this->ints, this->doubles);
		}
		else
		{
			ints.push_back(0);
		}
		}
		// Kinetics
		{
		cxxKinetics *entity_ptr = Utilities::Rxn_find(phreeqc_ref.Get_Rxn_kinetics_map(), i);
		if (entity_ptr)
		{
			ints.push_back(1);
			entity_ptr->mpi_pack(this->dictionary, this->ints, this->doubles);
		}
		else
		{
			ints.push_back(0);
		}
		}
		// PPassemblages
		{
		cxxPPassemblage *entity_ptr = Utilities::Rxn_find(phreeqc_ref.Get_Rxn_pp_assemblage_map(), i);
		if (entity_ptr)
		{
			ints.push_back(1);
			entity_ptr->mpi_pack(this->dictionary, this->ints, this->doubles);
		}
		else
		{
			ints.push_back(0);
		}
		}
		// SSassemblages
		{
		cxxSSassemblage *entity_ptr = Utilities::Rxn_find(phreeqc_ref.Get_Rxn_ss_assemblage_map(), i);
		if (entity_ptr)
		{
			ints.push_back(1);
			entity_ptr->mpi_pack(this->dictionary, this->ints, this->doubles);
		}
		else
		{
			ints.push_back(0);
		}
		}
		// Surfaces
		{
		cxxSurface *entity_ptr = Utilities::Rxn_find(phreeqc_ref.Get_Rxn_surface_map(), i);
		if (entity_ptr)
		{
			ints.push_back(1);
			entity_ptr->mpi_pack(this->dictionary, this->ints, this->doubles);
		}
		else
		{
			ints.push_back(0);
		}
		}
		// Temperature
		if (include_t)
		{		
			cxxTemperature *entity_ptr = Utilities::Rxn_find(phreeqc_ref.Get_Rxn_temperature_map(), i);
			if (entity_ptr)
			{
				ints.push_back(1);
				entity_ptr->mpi_pack(this->dictionary, this->ints, this->doubles);
			}
			else
			{
				ints.push_back(0);
			}
		}			
		else
		{
			ints.push_back(0);
		}
		// Pressure
		if (include_p)
		{		
			cxxPressure *entity_ptr = Utilities::Rxn_find(phreeqc_ref.Get_Rxn_pressure_map(), i);
			if (entity_ptr)
			{
				ints.push_back(1);
				entity_ptr->mpi_pack(this->dictionary, this->ints, this->doubles);
			}
			else
			{
				ints.push_back(0);
			}
		}			
		else
		{
			ints.push_back(0);
		}
	}	
	return true;
}

bool 
Serializer::Deserialize(Phreeqc &phreeqc_ref, Dictionary &dictionary, std::vector<int> &ints, std::vector<double> &doubles)
{
	int ii = 0;
	int dd = 0;
	int count = ints[ii++];
	for (int i = 0; i < count; i++)
	{
		// Solution
		if (ints[ii++] == 1)
		{
			cxxSolution soln;
			soln.mpi_unpack(this->dictionary, ints, doubles, ii, dd);
			int n_user = soln.Get_n_user();
			phreeqc_ref.Get_Rxn_solution_map()[n_user] = soln;
		}
		
		// Exchanger
		if (ints[ii++] == 1)
		{
			cxxExchange entity;
			entity.mpi_unpack(this->dictionary, ints, doubles, ii, dd);
			int n_user = entity.Get_n_user();
			phreeqc_ref.Get_Rxn_exchange_map()[n_user] = entity;
		}
		// GasPhase
		if (ints[ii++] == 1)
		{
			cxxGasPhase entity;
			entity.mpi_unpack(this->dictionary, ints, doubles, ii, dd);
			int n_user = entity.Get_n_user();
			phreeqc_ref.Get_Rxn_gas_phase_map()[n_user] = entity;
		}
		// Kinetics
		if (ints[ii++] == 1)
		{
			cxxKinetics entity;
			entity.mpi_unpack(this->dictionary, ints, doubles, ii, dd);
			int n_user = entity.Get_n_user();
			phreeqc_ref.Get_Rxn_kinetics_map()[n_user] = entity;
		}
		// PPassemblages
		if (ints[ii++] == 1)
		{
			cxxPPassemblage entity;
			entity.mpi_unpack(this->dictionary, ints, doubles, ii, dd);
			int n_user = entity.Get_n_user();
			phreeqc_ref.Get_Rxn_pp_assemblage_map()[n_user] = entity;
		}
		// SSassemblages
		if (ints[ii++] == 1)
		{
			cxxSSassemblage entity;
			entity.mpi_unpack(this->dictionary, ints, doubles, ii, dd);
			int n_user = entity.Get_n_user();
			phreeqc_ref.Get_Rxn_ss_assemblage_map()[n_user] = entity;
		}
		// Surfaces
		if (ints[ii++] == 1)
		{
			cxxSurface entity;
			entity.mpi_unpack(this->dictionary, ints, doubles, ii, dd);
			int n_user = entity.Get_n_user();
			phreeqc_ref.Get_Rxn_surface_map()[n_user] = entity;
		}
		// Temperature
		if (ints[ii++] == 1)
		{
			cxxTemperature entity;
			entity.mpi_unpack(this->dictionary, ints, doubles, ii, dd);
			int n_user = entity.Get_n_user();
			phreeqc_ref.Get_Rxn_temperature_map()[n_user] = entity;
		}
		// Pressure
		if (ints[ii++] == 1)
		{
			cxxPressure entity;
			entity.mpi_unpack(this->dictionary, ints, doubles, ii, dd);
			int n_user = entity.Get_n_user();
			phreeqc_ref.Get_Rxn_pressure_map()[n_user] = entity;
		}
	}
	return true;
}