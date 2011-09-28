// StorageBin.cxx: implementation of the cxxStorageBin class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#ifdef USE_MPI
//MPICH seems to require mpi.h to be first
#include <mpi.h>
#endif
#include <fstream>
#include <iostream>				// std::cout std::cerr
#include <cassert>				// assert
#include <algorithm>			// std::sort

#include "Utils.h"				// define first
#if !defined(PHREEQC_CLASS)
#define EXTERNAL extern
#include "global.h"
#else
#include "Phreeqc.h"
#endif
#include "NameDouble.h"
#include "StorageBin.h"
#include "SSassemblage.h"
#include "Solution.h"
#include "Exchange.h"
#include "GasPhase.h"
#include "cxxKinetics.h"
#include "PPassemblage.h"
#include "SSassemblageSS.h"
#include "SSassemblage.h"
#include "Surface.h"
#include "cxxMix.h"
#include "Reaction.h"
#include "Temperature.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include "output.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
cxxStorageBin::cxxStorageBin(PHRQ_io *io)
:
PHRQ_base(io)
{
	// default constructor for cxxStorageBin 
	this->system.Set_io(io);
}

cxxStorageBin::cxxStorageBin(PHREEQC_PTR_ARG_COMMA struct Use *use_ptr, PHRQ_io * io)
:
PHRQ_base(io)
{
	//Construct from use pointer

	int n;
	this->system.Set_io(io);
	if (use_ptr->mix_in == TRUE)
	{
		struct mix *struct_entity = P_INSTANCE_POINTER mix_bsearch(use_ptr->n_mix_user, &n);
		if (struct_entity != NULL)
		{
			cxxMix entity(struct_entity, this->Get_io());
			this->setMix(use_ptr->n_mix_user, &entity);

			std::map<int, double> c = (this->getMix(use_ptr->n_mix_user)->Get_mixComps());
			std::map<int, double>::iterator it;
			for (it = c.begin(); it != c.end(); it++)
			{
				struct solution *struct_entity1 = P_INSTANCE_POINTER solution_bsearch(it->first, &n, FALSE);
				if (struct_entity1 != NULL)
				{
					cxxSolution entity1(struct_entity1, this->Get_io());
					this->setSolution(it->first, &entity1);
				}
			}
		}
	}
	else if (use_ptr->solution_in == TRUE)
	{
		struct solution *struct_entity = P_INSTANCE_POINTER solution_bsearch(use_ptr->n_solution_user, &n, FALSE);
		if (struct_entity != NULL)
		{
			cxxSolution entity(struct_entity, this->Get_io());
			this->setSolution(use_ptr->n_solution_user, &entity);
		}
	}
	if (use_ptr->pp_assemblage_in == TRUE)
	{
		struct pp_assemblage *struct_entity = P_INSTANCE_POINTER pp_assemblage_bsearch(use_ptr->n_pp_assemblage_user, &n);
		if (struct_entity != NULL)
		{
			cxxPPassemblage entity(struct_entity, this->Get_io());
			this->setPPassemblage(use_ptr->n_pp_assemblage_user, &entity);
		}
	}
	if (use_ptr->exchange_in == TRUE)
	{
		struct exchange *struct_entity = P_INSTANCE_POINTER exchange_bsearch(use_ptr->n_exchange_user, &n);
		if (struct_entity != NULL)
		{
			cxxExchange entity(struct_entity, this->Get_io());
			this->setExchange(use_ptr->n_exchange_user, &entity);
		}
	}
	if (use_ptr->surface_in == TRUE)
	{
		struct surface *struct_entity = P_INSTANCE_POINTER surface_bsearch(use_ptr->n_surface_user, &n);
		if (struct_entity != NULL)
		{
			cxxSurface entity(struct_entity, this->Get_io());
			this->setSurface(use_ptr->n_surface_user, &entity);
		}
	}
	if (use_ptr->gas_phase_in == TRUE)
	{
		struct gas_phase *struct_entity = P_INSTANCE_POINTER gas_phase_bsearch(use_ptr->n_gas_phase_user, &n);
		if (struct_entity != NULL)
		{
			cxxGasPhase entity(struct_entity, this->Get_io());
			this->setGasPhase(use_ptr->n_gas_phase_user, &entity);
		}
	}
	if (use_ptr->s_s_assemblage_in == TRUE)
	{
		struct s_s_assemblage *struct_entity = P_INSTANCE_POINTER s_s_assemblage_bsearch(use_ptr->n_s_s_assemblage_user, &n);
		if (struct_entity != NULL)
		{
			cxxSSassemblage entity(struct_entity);
			this->setSSassemblage(use_ptr->n_s_s_assemblage_user, &entity);
		}
	}
	if (use_ptr->kinetics_in == TRUE)
	{
		struct kinetics *struct_entity = P_INSTANCE_POINTER kinetics_bsearch(use_ptr->n_kinetics_user, &n);
		if (struct_entity != NULL)
		{
			cxxKinetics entity(struct_entity, this->Get_io());
			this->setKinetics(use_ptr->n_kinetics_user, &entity);
		}
	}
	if (use_ptr->irrev_in == TRUE)
	{
		struct irrev *struct_entity = P_INSTANCE_POINTER irrev_bsearch(use_ptr->n_irrev_user, &n);
		if (struct_entity != NULL)
		{
			cxxReaction entity(struct_entity);
			this->setReaction(use_ptr->n_irrev_user, &entity);
		}
	}
	if (use_ptr->temperature_in == TRUE)
	{
		struct temperature *struct_entity = P_INSTANCE_POINTER temperature_bsearch(use_ptr->n_temperature_user, &n);
		if (struct_entity != NULL)
		{
			cxxTemperature entity(struct_entity, this->Get_io());
			this->setTemperature(use_ptr->n_temperature_user, &entity);
		}
	}
}

cxxStorageBin::~cxxStorageBin()
{
}
cxxSolution *
cxxStorageBin::getSolution(int n_user)
{
	if (this->Solutions.find(n_user) != this->Solutions.end())
	{
		return (&(this->Solutions.find(n_user)->second));
	}
	return (NULL);
}
void 
cxxStorageBin::setSolution(int n_user, cxxSolution * entity)
{
	if (entity == NULL)
		return;
	Solutions[n_user] = *entity;
}
void 
cxxStorageBin::setSolution(int n_user, cxxSolution & entity)
{
	Solutions[n_user] = entity;
}
void 
cxxStorageBin::removeSolution(int n_user)
{
	Solutions.erase(n_user);
}

cxxExchange *
cxxStorageBin::getExchange(int n_user)
{
	if (this->Exchangers.find(n_user) != this->Exchangers.end())
	{
		return (&(this->Exchangers.find(n_user)->second));
	}
	return (NULL);
}
void 
cxxStorageBin::setExchange(int n_user, cxxExchange * entity)
{
	if (entity == NULL)
		return;
	Exchangers[n_user] = *entity;
}
void 
cxxStorageBin::removeExchange(int n_user)
{
	Exchangers.erase(n_user);
}

cxxPPassemblage *
cxxStorageBin::getPPassemblage(int n_user)
{
	if (this->PPassemblages.find(n_user) != this->PPassemblages.end())
	{
		return (&(this->PPassemblages.find(n_user)->second));
	}
	return (NULL);
}
void 
cxxStorageBin::setPPassemblage(int n_user, cxxPPassemblage * entity)
{
	if (entity == NULL)
		return;
	PPassemblages[n_user] = *entity;
}
void 
cxxStorageBin::setPPassemblage(int n_user, cxxPPassemblage & entity)
{
	PPassemblages[n_user] = entity;
}
void 
cxxStorageBin::removePPassemblage(int n_user)
{
	PPassemblages.erase(n_user);
}

cxxGasPhase *
cxxStorageBin::getGasPhase(int n_user)
{
	if (this->GasPhases.find(n_user) != this->GasPhases.end())
	{
		return (&(this->GasPhases.find(n_user)->second));
	}
	return (NULL);
}
void 
cxxStorageBin::setGasPhase(int n_user, cxxGasPhase * entity)
{
	if (entity == NULL)
		return;
	GasPhases[n_user] = *entity;
}
void 
cxxStorageBin::removeGasPhase(int n_user)
{
	GasPhases.erase(n_user);
}

cxxSSassemblage *
cxxStorageBin::getSSassemblage(int n_user)
{
	if (this->SSassemblages.find(n_user) != this->SSassemblages.end())
	{
		return (&(this->SSassemblages.find(n_user)->second));
	}
	return (NULL);
}
void 
cxxStorageBin::setSSassemblage(int n_user, cxxSSassemblage * entity)
{
	if (entity == NULL)
		return;
	SSassemblages[n_user] = *entity;
}
void 
cxxStorageBin::setSSassemblage(int n_user, cxxSSassemblage & entity)
{
	SSassemblages[n_user] = entity;
}
void 
cxxStorageBin::removeSSassemblage(int n_user)
{
	SSassemblages.erase(n_user);
}

cxxKinetics *
cxxStorageBin::getKinetics(int n_user)
{
	if (this->Kinetics.find(n_user) != this->Kinetics.end())
	{
		return (&(this->Kinetics.find(n_user)->second));
	}
	return (NULL);
}
void 
cxxStorageBin::setKinetics(int n_user, cxxKinetics * entity)
{
	if (entity == NULL)
		return;
	Kinetics[n_user] = *entity;
}
void 
cxxStorageBin::removeKinetics(int n_user)
{
	Kinetics.erase(n_user);
}

cxxSurface *
cxxStorageBin::getSurface(int n_user)
{
	if (this->Surfaces.find(n_user) != this->Surfaces.end())
	{
		return (&(this->Surfaces.find(n_user)->second));
	}
	return (NULL);
}
void 
cxxStorageBin::setSurface(int n_user, cxxSurface * entity)
{
	if (entity == NULL)
		return;
	Surfaces[n_user] = *entity;
}
void 
cxxStorageBin::removeSurface(int n_user)
{
	Surfaces.erase(n_user);
}

cxxMix *
cxxStorageBin::getMix(int n_user)
{
	if (this->Mixes.find(n_user) != this->Mixes.end())
	{
		return (&(this->Mixes.find(n_user)->second));
	}
	return (NULL);
}
void 
cxxStorageBin::setMix(int n_user, cxxMix * entity)
{
	if (entity == NULL)
		return;
	Mixes[n_user] = *entity;
}
void 
cxxStorageBin::removeMix(int n_user)
{
	Mixes.erase(n_user);
}

cxxReaction *
cxxStorageBin::getReaction(int n_user)
{
	if (this->Reactions.find(n_user) != this->Reactions.end())
	{
		return (&(this->Reactions.find(n_user)->second));
	}
	return (NULL);
}
void 
cxxStorageBin::setReaction(int n_user, cxxReaction * entity)
{
	if (entity == NULL)
		return;
	Reactions[n_user] = *entity;
}
void 
cxxStorageBin::removeReaction(int n_user)
{
	Reactions.erase(n_user);
}

cxxTemperature *
cxxStorageBin::getTemperature(int n_user)
{
	if (this->Temperatures.find(n_user) != this->Temperatures.end())
	{
		return (&(this->Temperatures.find(n_user)->second));
	}
	return (NULL);
}
void 
cxxStorageBin::setTemperature(int n_user, cxxTemperature * entity)
{
	if (entity == NULL)
		return;
	Temperatures[n_user] = *entity;
}
void 
cxxStorageBin::removeTemperature(int n_user)
{
	Temperatures.erase(n_user);
}

const std::map < int, cxxSolution > &
cxxStorageBin::getSolutions() const
{
	return this->Solutions;
}
const std::map < int, cxxExchange > &
cxxStorageBin::getExchangers() const
{
	return this->Exchangers;
}
const std::map < int, cxxGasPhase > &
cxxStorageBin::getGasPhases() const
{
	return this->GasPhases;
}
const std::map < int, cxxKinetics > &
cxxStorageBin::getKinetics() const
{
	return this->Kinetics;
}
const std::map < int, cxxPPassemblage > &
cxxStorageBin::getPPassemblages() const
{
	return this->PPassemblages;
}
const std::map < int, cxxSSassemblage > &
cxxStorageBin::getSSassemblages() const
{
	return this->SSassemblages;
}
const std::map < int, cxxSurface > &
cxxStorageBin::getSurfaces() const
{
	return this->Surfaces;
}
void
cxxStorageBin::import_phreeqc(PHREEQC_PTR_ARG)
		//
	// pull data out of c storage
		//
{
	int i;

	// Solutions
	for (i = 0; i < P_INSTANCE_POINTER count_solution; i++)
	{
		Solutions[P_INSTANCE_POINTER solution[i]->n_user] = cxxSolution(P_INSTANCE_POINTER solution[i], this->Get_io());
	}

	// Exchangers
	for (i = 0; i < P_INSTANCE_POINTER count_exchange; i++)
	{
		Exchangers[P_INSTANCE_POINTER exchange[i].n_user] = cxxExchange(&P_INSTANCE_POINTER exchange[i], this->Get_io());
	}

	// GasPhases
	for (i = 0; i < P_INSTANCE_POINTER count_gas_phase; i++)
	{
		GasPhases[P_INSTANCE_POINTER gas_phase[i].n_user] = cxxGasPhase(&P_INSTANCE_POINTER gas_phase[i], this->Get_io());
	}

	// Kinetics
	for (i = 0; i < P_INSTANCE_POINTER count_kinetics; i++)
	{
		Kinetics[P_INSTANCE_POINTER kinetics[i].n_user] = cxxKinetics(&P_INSTANCE_POINTER kinetics[i], this->Get_io());
	}

	// PPassemblages
	for (i = 0; i < P_INSTANCE_POINTER count_pp_assemblage; i++)
	{
		PPassemblages[P_INSTANCE_POINTER pp_assemblage[i].n_user] =
			cxxPPassemblage(&P_INSTANCE_POINTER pp_assemblage[i], this->Get_io());
	}

	// SSassemblages
	for (i = 0; i < P_INSTANCE_POINTER count_s_s_assemblage; i++)
	{
		SSassemblages[P_INSTANCE_POINTER s_s_assemblage[i].n_user] =
			cxxSSassemblage(&P_INSTANCE_POINTER s_s_assemblage[i]);
	}

	// Surfaces
	for (i = 0; i < P_INSTANCE_POINTER count_surface; i++)
	{
		Surfaces[P_INSTANCE_POINTER surface[i].n_user] = cxxSurface(&P_INSTANCE_POINTER surface[i], this->Get_io());
	}

	// Mixes
	for (i = 0; i < P_INSTANCE_POINTER count_mix; i++)
	{
		Mixes[P_INSTANCE_POINTER mix[i].n_user] = cxxMix(&P_INSTANCE_POINTER mix[i], this->Get_io());
	}

	// Reactions
	for (i = 0; i < P_INSTANCE_POINTER count_irrev; i++)
	{
		Reactions[P_INSTANCE_POINTER irrev[i].n_user] = cxxReaction(&P_INSTANCE_POINTER irrev[i]);
	}

	// Temperatures
	for (i = 0; i < P_INSTANCE_POINTER count_temperature; i++)
	{
		Temperatures[P_INSTANCE_POINTER temperature[i].n_user] = cxxTemperature(&P_INSTANCE_POINTER temperature[i], this->Get_io());
	}
}


#ifdef SKIP
void
cxxStorageBin::dump_xml(std::ostream & s_oss, unsigned int indent) const const
{
	//const char    ERR_MESSAGE[] = "Packing mix message: %s, element not found\n";
	unsigned int i;
	s_oss.precision(DBL_DIG - 1);
	std::string indent0(""), indent1(""), indent2("");
	for (i = 0; i < indent; ++i)
		indent0.append(Utilities::INDENT);
	for (i = 0; i < indent + 1; ++i)
		indent1.append(Utilities::INDENT);
	for (i = 0; i < indent + 2; ++i)
		indent2.append(Utilities::INDENT);

	// StorageBin element and attributes
	s_oss << indent0;
	s_oss << "<mix " << std::endl;

	s_oss << indent1;
	s_oss << "pitzer_mix_gammas=\"" << this->
		pitzer_mix_gammas << "\"" << std::endl;

	// components
	s_oss << indent1;
	s_oss << "<component " << std::endl;
	for (std::list < cxxStorageBinComp >::const_iterator it =
		 mixComps.begin(); it != mixComps.end(); ++it)
	{
		it->dump_xml(s_oss, indent + 2);
	}

	return;
}
#endif

void
cxxStorageBin::dump_raw(std::ostream & s_oss, unsigned int indent) const
{
	// Dump all data

	//const char    ERR_MESSAGE[] = "Packing mix message: %s, element not found\n";
	s_oss.precision(DBL_DIG - 1);

	// Solutions
	Utilities::dump_raw(Solutions, s_oss, indent);

	// Exchange
	Utilities::dump_raw(Exchangers, s_oss, indent);

	// Gas Phases
	Utilities::dump_raw(GasPhases, s_oss, indent);

	// Kinetics
	Utilities::dump_raw(Kinetics, s_oss, indent);

	// PPassemblage
	Utilities::dump_raw(PPassemblages, s_oss, indent);

	// SSassemblage
	Utilities::dump_raw(SSassemblages, s_oss, indent);

	// Surface
	Utilities::dump_raw(Surfaces, s_oss, indent);

	// Mix
	Utilities::dump_raw(Mixes, s_oss, indent);

	// Reactions
	Utilities::dump_raw(Reactions, s_oss, indent);

	// Temperature
	Utilities::dump_raw(Temperatures, s_oss, indent);
}

void
cxxStorageBin::dump_raw(std::ostream & s_oss, int n, unsigned int indent)
{
	// Dump one user number

	//const char    ERR_MESSAGE[] = "Packing mix message: %s, element not found\n";
	s_oss.precision(DBL_DIG - 1);

	// Solutions
	if (this->getSolution(n) != NULL)
	{
		this->getSolution(n)->dump_raw(s_oss, indent);
	}

	// Exchange
	if (this->getExchange(n) != NULL)
	{
		this->getExchange(n)->dump_raw(s_oss, indent);
	}

	// Gas Phases
	if (this->getGasPhase(n) != NULL)
	{
		this->getGasPhase(n)->dump_raw(s_oss, indent);
	}

	// Kinetics
	if (this->getKinetics(n) != NULL)
	{
		this->getKinetics(n)->dump_raw(s_oss, indent);
	}

	// PPassemblage
	if (this->getPPassemblage(n) != NULL)
	{
		this->getPPassemblage(n)->dump_raw(s_oss, indent);
	}

	// SSassemblage
	if (this->getSSassemblage(n) != NULL)
	{
		this->getSSassemblage(n)->dump_raw(s_oss, indent);
	}

	// Surface
	if (this->getSurface(n) != NULL)
	{
		this->getSurface(n)->dump_raw(s_oss, indent);
	}
}

void
cxxStorageBin::read_raw(CParser & parser)
{
	CParser::LINE_TYPE i;
	while ((i =
			parser.check_line("StorageBin read_raw", false, true, true,
							  true)) != CParser::LT_KEYWORD)
	{
		if (i == CParser::LT_EOF)
			return;				// CParser::LT_EOF;
	}

	for (;;)
	{
		switch (parser.next_keyword())
		{
		case CParser::KT_END:
		case CParser::KT_EOF:
		case CParser::KT_NONE:
			goto END_OF_SIMULATION_INPUT;
			break;
			/*
			   KT_SOLUTION_RAW     =  5,
			   KT_EXCHANGE_RAW     =  6,
			   KT_GASPHASE_RAW     =  7,
			   KT_KINETICS_RAW     =  8,
			   KT_PPASSEMBLAGE_RAW =  9,
			   KT_SSASSEMBLAGE_RAW =  10,
			   KT_SURFACE_RAW      =  11,
			   KT_TEMPERATURE_RAW  =  12,
			   KT_REACTION_RAW     =  13
			 */
		case CParser::KT_SOLUTION_RAW:
			{
				cxxSolution entity(this->Get_io());
				entity.read_raw(parser);
				Solutions[entity.get_n_user()] = entity;
			}
			break;

		case CParser::KT_EXCHANGE_RAW:
			{
				cxxExchange entity(this->Get_io());
				entity.read_raw(parser);
				Exchangers[entity.get_n_user()] = entity;
			}
			break;

		case CParser::KT_GASPHASE_RAW:
			{
				cxxGasPhase entity(this->Get_io());
				entity.read_raw(parser);
				GasPhases[entity.get_n_user()] = entity;
			}
			break;

		case CParser::KT_KINETICS_RAW:
			{
				cxxKinetics entity(this->Get_io());
				entity.read_raw(parser);
				Kinetics[entity.get_n_user()] = entity;
			}
			break;

		case CParser::KT_PPASSEMBLAGE_RAW:
			{
				cxxPPassemblage entity(this->Get_io());
				entity.read_raw(parser);
				PPassemblages[entity.get_n_user()] = entity;
			}
			break;

		case CParser::KT_SSASSEMBLAGE_RAW:
			{
				cxxSSassemblage entity;
				entity.read_raw(parser);
				SSassemblages[entity.get_n_user()] = entity;
			}
			break;

		case CParser::KT_SURFACE_RAW:
			{
				cxxSurface entity(this->Get_io());
				entity.read_raw(parser);
				Surfaces[entity.get_n_user()] = entity;
			}
			break;

		case CParser::KT_TEMPERATURE_RAW:
			{
				cxxTemperature entity(this->Get_io());
				entity.read_raw(parser);
				Temperatures[entity.get_n_user()] = entity;
			}
			break;

		case CParser::KT_REACTION_RAW:
			{
				cxxReaction entity;
				entity.read_raw(parser, true);
				Reactions[entity.get_n_user()] = entity;
			}
			break;
		default:
			break;
		}
	}

  END_OF_SIMULATION_INPUT:
	return;						//CParser::LT_OK;
}

int
cxxStorageBin::read_raw_keyword(CParser & parser)
{
	CParser::LINE_TYPE i;
	int entity_number = -999;

	switch (parser.next_keyword())
	{
	case CParser::KT_NONE:
	case CParser::KT_END:
		while ((i =
				parser.check_line("StorageBin read_raw_keyword", false, true,
								  true, true)) != CParser::LT_KEYWORD)
		{
			if (i == CParser::LT_EOF)
				break;			// CParser::LT_EOF;
		}
		break;
	case CParser::KT_EOF:
		break;
		/*
		   KT_SOLUTION_RAW     =  5,
		   KT_EXCHANGE_RAW     =  6,
		   KT_GASPHASE_RAW     =  7,
		   KT_KINETICS_RAW     =  8,
		   KT_PPASSEMBLAGE_RAW =  9,
		   KT_SSASSEMBLAGE_RAW =  10,
		   KT_SURFACE_RAW      =  11,
		   KT_TEMPERATURE_RAW  =  12,
		   KT_REACTION_RAW     =  13
		 */
	case CParser::KT_SOLUTION_RAW:
		{
			cxxSolution entity(this->Get_io());
			entity.read_raw(parser);
			Solutions[entity.get_n_user()] = entity;
			entity_number = entity.get_n_user();
		}
		break;

	case CParser::KT_EXCHANGE_RAW:
		{
			cxxExchange entity(this->Get_io());
			entity.read_raw(parser);
			Exchangers[entity.get_n_user()] = entity;
			entity_number = entity.get_n_user();
		}
		break;

	case CParser::KT_GASPHASE_RAW:
		{
			cxxGasPhase entity(this->Get_io());
			entity.read_raw(parser);
			GasPhases[entity.get_n_user()] = entity;
			entity_number = entity.get_n_user();
		}
		break;

	case CParser::KT_KINETICS_RAW:
		{
			cxxKinetics entity(this->Get_io());
			entity.read_raw(parser);
			Kinetics[entity.get_n_user()] = entity;
			entity_number = entity.get_n_user();
		}
		break;

	case CParser::KT_PPASSEMBLAGE_RAW:
		{
			cxxPPassemblage entity(this->Get_io());
			entity.read_raw(parser);
			PPassemblages[entity.get_n_user()] = entity;
			entity_number = entity.get_n_user();
		}
		break;

	case CParser::KT_SSASSEMBLAGE_RAW:
		{
			cxxSSassemblage entity;
			entity.read_raw(parser);
			SSassemblages[entity.get_n_user()] = entity;
			entity_number = entity.get_n_user();
		}
		break;

	case CParser::KT_SURFACE_RAW:
		{
			cxxSurface entity(this->Get_io());
			entity.read_raw(parser);
			Surfaces[entity.get_n_user()] = entity;
			entity_number = entity.get_n_user();
		}
		break;

	case CParser::KT_TEMPERATURE_RAW:
		{
			cxxTemperature entity(this->Get_io());
			entity.read_raw(parser);
			Temperatures[entity.get_n_user()] = entity;
			entity_number = entity.get_n_user();
		}
		break;

	case CParser::KT_REACTION_RAW:
		{
			cxxReaction entity;
			entity.read_raw(parser, true);
			Reactions[entity.get_n_user()] = entity;
			entity_number = entity.get_n_user();
		}
		break;

	default:
		break;
	}
	return (entity_number);		//CParser::LT_OK;
}

void
cxxStorageBin::add(struct system *system_ptr)
		//
		// add data from a system structure
		//
{

	// Solutions
	if (system_ptr->solution != NULL)
	{
		this->Solutions[system_ptr->solution->n_user] =
			cxxSolution(system_ptr->solution, this->Get_io());
	}

	// Exchangers
	if (system_ptr->exchange != NULL)
	{
		this->Exchangers[system_ptr->exchange->n_user] =
			cxxExchange(system_ptr->exchange, this->Get_io());
	}

	// GasPhases
	if (system_ptr->gas_phase != NULL)
	{
		this->GasPhases[system_ptr->gas_phase->n_user] =
			cxxGasPhase(system_ptr->gas_phase, this->Get_io());
	}

	// Kinetics
	if (system_ptr->kinetics != NULL)
	{
		this->Kinetics[system_ptr->kinetics->n_user] =
			cxxKinetics(system_ptr->kinetics, this->Get_io());
	}

	// PPassemblages
	if (system_ptr->pp_assemblage != NULL)
	{
		this->PPassemblages[system_ptr->pp_assemblage->n_user] =
			cxxPPassemblage(system_ptr->pp_assemblage, this->Get_io());
	}

	// SSassemblages
	if (system_ptr->s_s_assemblage != NULL)
	{
		this->SSassemblages[system_ptr->s_s_assemblage->n_user] =
			cxxSSassemblage(system_ptr->s_s_assemblage);
	}

	// Surfaces
	if (system_ptr->surface != NULL)
	{
		this->Surfaces[system_ptr->surface->n_user] =
			cxxSurface(system_ptr->surface, this->Get_io());
	}
}

void
cxxStorageBin::cxxStorageBin2phreeqc(PHREEQC_PTR_ARG_COMMA int n)
		//
		// copy data fromphreeqc storage to storage bin
		//
{
	//std::ostringstream oss;

	// Solutions
	{

		std::map < int, cxxSolution >::iterator it = this->Solutions.find(n);
		if (it != this->Solutions.end())
		{
			//P_INSTANCE_POINTER solution[0] = (it->second).cxxSolution2solution(P_INSTANCE);
			P_INSTANCE_POINTER solution[0] = P_INSTANCE_POINTER cxxSolution2solution(&(it->second));
			P_INSTANCE_POINTER solution[0]->n_user = n;
			P_INSTANCE_POINTER solution[0]->n_user_end = n;
			P_INSTANCE_POINTER count_solution++;
		}
		else
		{
			error_msg("cxxSolution not found in system2phreeqc", STOP);
		}
	}

	// Exchangers
	{
		std::map < int, cxxExchange >::iterator it = this->Exchangers.find(n);
		if (it != this->Exchangers.end())
		{
			//struct exchange *exchange_ptr =
			//	(it->second).cxxExchange2exchange(P_INSTANCE);
			struct exchange *exchange_ptr = P_INSTANCE_POINTER cxxExchange2exchange(&it->second);

			P_INSTANCE_POINTER exchange_copy(exchange_ptr, &P_INSTANCE_POINTER exchange[0], n);
			P_INSTANCE_POINTER count_exchange++;
			P_INSTANCE_POINTER exchange_free(exchange_ptr);
			exchange_ptr = (struct exchange *) P_INSTANCE_POINTER free_check_null(exchange_ptr);
		}
	}

	// GasPhases
	{
		std::map < int, cxxGasPhase >::iterator it = this->GasPhases.find(n);
		if (it != this->GasPhases.end())
		{
			//struct gas_phase *gas_phase_ptr =
			//	(it->second).cxxGasPhase2gas_phase(P_INSTANCE);
			struct gas_phase *gas_phase_ptr = P_INSTANCE_POINTER cxxGasPhase2gas_phase(&(it->second));
			P_INSTANCE_POINTER gas_phase_copy(gas_phase_ptr, &P_INSTANCE_POINTER gas_phase[0], n);
			P_INSTANCE_POINTER count_gas_phase++;
			P_INSTANCE_POINTER gas_phase_free(gas_phase_ptr);
			gas_phase_ptr =
				(struct gas_phase *) P_INSTANCE_POINTER free_check_null(gas_phase_ptr);
		}
	}

	// Kinetics
	{
		std::map < int, cxxKinetics >::iterator it = this->Kinetics.find(n);
		if (it != this->Kinetics.end())
		{
			//struct kinetics *kinetics_ptr =
			//	(it->second).cxxKinetics2kinetics(P_INSTANCE);
			struct kinetics *kinetics_ptr =  P_INSTANCE_POINTER cxxKinetics2kinetics(&(it->second));
			P_INSTANCE_POINTER kinetics_copy(kinetics_ptr, &P_INSTANCE_POINTER kinetics[0], n);
			P_INSTANCE_POINTER count_kinetics++;
			P_INSTANCE_POINTER kinetics_free(kinetics_ptr);
			kinetics_ptr = (struct kinetics *) P_INSTANCE_POINTER free_check_null(kinetics_ptr);
		}
	}

	// PPassemblages
	{
		std::map < int, cxxPPassemblage >::iterator it =
			this->PPassemblages.find(n);
		if (it != this->PPassemblages.end())
		{
			//struct pp_assemblage *pp_assemblage_ptr =
			//	(it->second).cxxPPassemblage2pp_assemblage(P_INSTANCE);
			struct pp_assemblage *pp_assemblage_ptr = P_INSTANCE_POINTER cxxPPassemblage2pp_assemblage(&(it->second));
			P_INSTANCE_POINTER pp_assemblage_copy(pp_assemblage_ptr, &P_INSTANCE_POINTER pp_assemblage[0], n);
			P_INSTANCE_POINTER count_pp_assemblage++;
			P_INSTANCE_POINTER pp_assemblage_free(pp_assemblage_ptr);
			pp_assemblage_ptr =
				(struct pp_assemblage *) P_INSTANCE_POINTER free_check_null(pp_assemblage_ptr);
		}
	}

	// SSassemblages
	{
		std::map < int, cxxSSassemblage >::iterator it =
			this->SSassemblages.find(n);
		if (it != this->SSassemblages.end())
		{
			//struct s_s_assemblage *s_s_assemblage_ptr =
			//	(it->second).cxxSSassemblage2s_s_assemblage(P_INSTANCE);
			struct s_s_assemblage *s_s_assemblage_ptr =
				P_INSTANCE_POINTER cxxSSassemblage2s_s_assemblage(&(it->second));

			P_INSTANCE_POINTER s_s_assemblage_copy(s_s_assemblage_ptr, &P_INSTANCE_POINTER s_s_assemblage[0], n);
			P_INSTANCE_POINTER count_s_s_assemblage++;
			P_INSTANCE_POINTER s_s_assemblage_free(s_s_assemblage_ptr);
			s_s_assemblage_ptr =
				(struct s_s_assemblage *) P_INSTANCE_POINTER free_check_null(s_s_assemblage_ptr);
		}
	}

	// Surfaces
	{
		std::map < int, cxxSurface >::iterator it = this->Surfaces.find(n);
		if (it != this->Surfaces.end())
		{
			//struct surface *surface_ptr = (it->second).cxxSurface2surface(P_INSTANCE);
			struct surface *surface_ptr = P_INSTANCE_POINTER cxxSurface2surface(&(it->second));
			P_INSTANCE_POINTER surface_copy(surface_ptr, &P_INSTANCE_POINTER surface[0], n);
			P_INSTANCE_POINTER count_surface++;
			P_INSTANCE_POINTER surface_free(surface_ptr);
			surface_ptr = (struct surface *) P_INSTANCE_POINTER free_check_null(surface_ptr);
		}
	}
	//std::cerr << oss.str();

}
void
cxxStorageBin::phreeqc2cxxStorageBin(PHREEQC_PTR_ARG_COMMA int n)
		//
		// copy data fromphreeqc storage to storage bin
		//
{
	int pos;

	// Solutions
	{
		P_INSTANCE_POINTER solution_bsearch(n, &pos, TRUE);
		this->Solutions[n] = cxxSolution(P_INSTANCE_POINTER solution[pos], this->Get_io());
	}

	// Exchangers
	{
		if (P_INSTANCE_POINTER exchange_bsearch(n, &pos) != NULL)
		{
			this->Exchangers[n] = cxxExchange(&(P_INSTANCE_POINTER exchange[pos]), this->Get_io());
		}
	}

	// GasPhases
	{
		if (P_INSTANCE_POINTER gas_phase_bsearch(n, &pos) != NULL)
		{
			this->GasPhases[n] = cxxGasPhase(&(P_INSTANCE_POINTER gas_phase[pos]), this->Get_io());
		}
	}

	// Kinetics
	{
		if (P_INSTANCE_POINTER kinetics_bsearch(n, &pos) != NULL)
		{
			this->Kinetics[n] = cxxKinetics(&(P_INSTANCE_POINTER kinetics[pos]), this->Get_io());
		}
	}

	// PPassemblages
	{
		if (P_INSTANCE_POINTER pp_assemblage_bsearch(n, &pos) != NULL)
		{
			this->PPassemblages[n] = cxxPPassemblage(&(P_INSTANCE_POINTER pp_assemblage[pos]), this->Get_io());
		}
	}

	// SSassemblages
	{
		if (P_INSTANCE_POINTER s_s_assemblage_bsearch(n, &pos) != NULL)
		{
			this->SSassemblages[n] = cxxSSassemblage(&(P_INSTANCE_POINTER s_s_assemblage[pos]));
		}
	}

	// Surfaces
	{
		if (P_INSTANCE_POINTER surface_bsearch(n, &pos) != NULL)
		{
			this->Surfaces[n] = cxxSurface(&(P_INSTANCE_POINTER surface[pos]), this->Get_io());
		}
	}
}

void
cxxStorageBin::remove(int n)
{
	// Solution
	this->Solutions.erase(n);

	// Exchanger
	this->Exchangers.erase(n);

	// GasPhase
	this->GasPhases.erase(n);

	// Kinetics
	this->Kinetics.erase(n);

	// PPassemblage
	this->PPassemblages.erase(n);

	// SSassemblage
	this->SSassemblages.erase(n);

	// Surface
	this->Surfaces.erase(n);
}

#ifdef SKIP
cxxSolution *
cxxStorageBin::mix_cxxSolutions(cxxMix & mixmap)
{
/*
 *   mixes solutions based on cxxMix structure, returns new solution
 *   return solution must be freed by calling method
 */
	double intensive, extensive;
	cxxSolution *cxxsoln_ptr, *cxxsoln_ptr1;
/*
 *   Zero out global solution data
 */
	cxxsoln_ptr = new cxxSolution(0.0);
/*
 *   Determine sum of mixing fractions
 */
	extensive = 0.0;

	std::map < int, double >*mixcomps = mixmap.comps();

	std::map < int, double >::const_iterator it;
	for (it = mixcomps->begin(); it != mixcomps->end(); it++)
	{
		extensive += it->second;
	}
/*
 *   Add solutions 
 */
	for (it = mixcomps->begin(); it != mixcomps->end(); it++)
	{
		cxxsoln_ptr1 = &((this->Solutions.find(it->first))->second);
		if (cxxsoln_ptr1 == NULL)
		{
			sprintf(error_string,
					"Solution %d not found in mix_cxxSolutions.", it->first);
			error_msg(error_string, CONTINUE);
			P_INSTANCE_POINTER input_error++;
			return (NULL);
		}
		intensive = it->second / extensive;
		cxxsoln_ptr->add(*cxxsoln_ptr1, intensive, it->second);
	}
	return (cxxsoln_ptr);
}
#endif
struct system *
cxxStorageBin::cxxStorageBin2system(PHREEQC_PTR_ARG_COMMA int n)
		//
		// make a system from storagebin
		//
{
	struct system *system_ptr =
		(struct system *) P_INSTANCE_POINTER PHRQ_malloc(sizeof(struct system));
	if (system_ptr == NULL)
		P_INSTANCE_POINTER malloc_error();

	// Solutions

	if (this->getSolution(n) != NULL)
	{
		//system_ptr->solution = (this->getSolution(n))->cxxSolution2solution(P_INSTANCE);
		system_ptr->solution = P_INSTANCE_POINTER cxxSolution2solution(this->getSolution(n));
	}
	else
	{
		system_ptr->solution = NULL;
	}

	// Exchangers
	if (this->getExchange(n) != NULL)
	{
		//system_ptr->exchange = (this->getExchange(n))->cxxExchange2exchange(P_INSTANCE);
		system_ptr->exchange = P_INSTANCE_POINTER cxxExchange2exchange(this->getExchange(n));
	}
	else
	{
		system_ptr->exchange = NULL;
	}

	// GasPhases
	if (this->getGasPhase(n) != NULL)
	{
		//system_ptr->gas_phase = (this->getGasPhase(n))->cxxGasPhase2gas_phase(P_INSTANCE);
		system_ptr->gas_phase = P_INSTANCE_POINTER cxxGasPhase2gas_phase(this->getGasPhase(n));
	}
	else
	{
		system_ptr->gas_phase = NULL;
	}

	// Kinetics
	if (this->getKinetics(n) != NULL)
	{
		//system_ptr->kinetics = (this->getKinetics(n))->cxxKinetics2kinetics(P_INSTANCE);
		system_ptr->kinetics = P_INSTANCE_POINTER cxxKinetics2kinetics(this->getKinetics(n));
		
	}
	else
	{
		system_ptr->kinetics = NULL;
	}

	// PPassemblages
	if (this->getPPassemblage(n) != NULL)
	{
		//system_ptr->pp_assemblage =
		//	(this->getPPassemblage(n))->cxxPPassemblage2pp_assemblage(P_INSTANCE);
		system_ptr->pp_assemblage =
			P_INSTANCE_POINTER cxxPPassemblage2pp_assemblage(this->getPPassemblage(n));
	}
	else
	{
		system_ptr->pp_assemblage = NULL;
	}

	// SSassemblages
	if (this->getSSassemblage(n) != NULL)
	{
		//system_ptr->s_s_assemblage =
		//	(this->getSSassemblage(n))->cxxSSassemblage2s_s_assemblage(P_INSTANCE);
		system_ptr->s_s_assemblage =
			P_INSTANCE_POINTER cxxSSassemblage2s_s_assemblage((this->getSSassemblage(n)));
	}
	else
	{
		system_ptr->s_s_assemblage = NULL;
	}

	// Surfaces
	if (this->getSurface(n) != NULL)
	{
		//system_ptr->surface = (this->getSurface(n))->cxxSurface2surface(P_INSTANCE);
		system_ptr->surface = P_INSTANCE_POINTER cxxSurface2surface((this->getSurface(n)));
	}
	else
	{
		system_ptr->surface = NULL;
	}
	return system_ptr;
}

#ifdef USE_MPI
void
cxxStorageBin::mpi_send(int n, int task_number)
{
	//
	// Send data for system n to task_number
	//
	std::vector < int >ints;
	std::vector < double >doubles;

	// Solution
	if (this->getSolution(n) != NULL)
	{
		ints.push_back(1);
		this->getSolution(n)->mpi_pack(ints, doubles);
	}
	else
	{
		ints.push_back(0);
	}
	//std::cerr << "Packed Solution" << std::endl;

	// Exchanger
	if (this->getExchange(n) != NULL)
	{
		ints.push_back(1);
		this->getExchange(n)->mpi_pack(ints, doubles);
	}
	else
	{
		ints.push_back(0);
	}
	//std::cerr << "Packed Exchange" << std::endl;

	// GasPhase
	if (this->getGasPhase(n) != NULL)
	{
		ints.push_back(1);
		this->getGasPhase(n)->mpi_pack(ints, doubles);
	}
	else
	{
		ints.push_back(0);
	}
	//std::cerr << "Packed GasPhase" << std::endl;

	// Kinetics
	if (this->getKinetics(n) != NULL)
	{
		ints.push_back(1);
		this->getKinetics(n)->mpi_pack(ints, doubles);
	}
	else
	{
		ints.push_back(0);
	}
	//std::cerr << "Packed Kinetics" << std::endl;

	// PPassemblages
	if (this->getPPassemblage(n) != NULL)
	{
		ints.push_back(1);
		this->getPPassemblage(n)->mpi_pack(ints, doubles);
	}
	else
	{
		ints.push_back(0);
	}
	//std::cerr << "Packed PPassemblage" << std::endl;

	// SSassemblages
	if (this->getSSassemblage(n) != NULL)
	{
		ints.push_back(1);
		this->getSSassemblage(n)->mpi_pack(ints, doubles);
	}
	else
	{
		ints.push_back(0);
	}
	//std::cerr << "Packed SSassemblage" << std::endl;

	// Surfaces
	if (this->getSurface(n) != NULL)
	{
		ints.push_back(1);
		this->getSurface(n)->mpi_pack(ints, doubles);
	}
	else
	{
		ints.push_back(0);
	}
	//std::cerr << "Packed Surface" << std::endl;

	// Pack data
	int max_size = 0;
	int member_size = 0;
	MPI_Pack_size((int) ints.size() + 10, MPI_INT, MPI_COMM_WORLD,
				  &member_size);
	max_size += member_size;
	MPI_Pack_size((int) doubles.size(), MPI_DOUBLE, MPI_COMM_WORLD,
				  &member_size);
	max_size += member_size + 10;
	void *buffer = P_INSTANCE_POINTER PHRQ_malloc(max_size);
	if (buffer == NULL)
		malloc_error();

	// Convert to arrays
	int i = (int) ints.size();
	//int int_array[i];
	int d = (int) doubles.size();
	//double double_array[d];
	/*
	   for (int j = 0; j < i; j++) {
	   int_array[j] = ints[j];
	   //std::cerr << "Sending ints " << j << " value " << ints[j] << std::endl;
	   }
	   for (int j = 0; j < d; j++) {
	   double_array[j] = doubles[j];
	   //std::cerr << "Sending doubles " << j << " value " << doubles[j] << std::endl;
	   }
	 */
	/*
	 *   Send message to processor
	 */
	int position = 0;
	MPI_Send(&max_size, 1, MPI_INT, task_number, 0, MPI_COMM_WORLD);
	MPI_Pack(&i, 1, MPI_INT, buffer, max_size, &position, MPI_COMM_WORLD);
	//MPI_Pack(&int_array, i, MPI_INT, buffer, max_size, &position, MPI_COMM_WORLD);
	MPI_Pack(&(ints.front()), i, MPI_INT, buffer, max_size, &position,
			 MPI_COMM_WORLD);
	MPI_Pack(&d, 1, MPI_INT, buffer, max_size, &position, MPI_COMM_WORLD);
	//MPI_Pack(&double_array, d, MPI_DOUBLE, buffer, max_size, &position, MPI_COMM_WORLD);
	if (d > 0)
	{
		MPI_Pack(&(doubles.front()), d, MPI_DOUBLE, buffer, max_size,
				 &position, MPI_COMM_WORLD);
	}
	MPI_Send(buffer, position, MPI_PACKED, task_number, 0, MPI_COMM_WORLD);

	buffer = (void *) free_check_null(buffer);
}

/* ---------------------------------------------------------------------- */
void
cxxStorageBin::mpi_recv(int task_number)
/* ---------------------------------------------------------------------- */
{
	MPI_Status mpi_status;
/*
 *   Malloc space for a buffer
 */
	int max_size;
	// buffer size

	MPI_Recv(&max_size, 1, MPI_INT, task_number, 0, MPI_COMM_WORLD,
			 &mpi_status);
	void *buffer = P_INSTANCE_POINTER PHRQ_malloc(max_size);
	if (buffer == NULL)
		malloc_error();
	/*
	 *   Recieve system
	 */
	MPI_Recv(buffer, max_size, MPI_PACKED, task_number, 0, MPI_COMM_WORLD,
			 &mpi_status);
	int position = 0;
	int msg_size;
	MPI_Get_count(&mpi_status, MPI_PACKED, &msg_size);

	/* Unpack ints */
	int count_ints;
	MPI_Unpack(buffer, msg_size, &position, &count_ints, 1, MPI_INT,
			   MPI_COMM_WORLD);
	int *ints = new int[count_ints];
	MPI_Unpack(buffer, msg_size, &position, ints, count_ints, MPI_INT,
			   MPI_COMM_WORLD);

	/* Unpack doubles */
	int count_doubles;
	MPI_Unpack(buffer, msg_size, &position, &count_doubles, 1, MPI_INT,
			   MPI_COMM_WORLD);
	double *doubles = new double[count_doubles];
	if (count_doubles > 0)
	{
		MPI_Unpack(buffer, msg_size, &position, doubles, count_doubles,
				   MPI_DOUBLE, MPI_COMM_WORLD);
	}
	buffer = free_check_null(buffer);
	/*
	 *   Make list of list of ints and doubles from solution structure
	 *   This list is not the complete structure, but only enough
	 *   for batch-reaction, advection, and transport calculations
	 */
	int i = 0;
	int d = 0;

	// Solution
	if (ints[i++] != 0)
	{
		cxxSolution entity;
		entity.mpi_unpack(ints, &i, doubles, &d);
		this->setSolution(entity.get_n_user(), &entity);
	}
	//std::cerr << "Unpacked Solution" << std::endl;

	// Exchanger
	if (ints[i++] != 0)
	{
		cxxExchange entity;
		entity.mpi_unpack(ints, &i, doubles, &d);
		this->setExchange(entity.get_n_user(), &entity);
	}
	//std::cerr << "Unpacked Exchange" << std::endl;

	// GasPhase
	if (ints[i++] != 0)
	{
		cxxGasPhase entity;
		entity.mpi_unpack(ints, &i, doubles, &d);
		this->setGasPhase(entity.get_n_user(), &entity);
	}
	//std::cerr << "Unpacked GasPhase" << std::endl;

	// Kinetics
	if (ints[i++] != 0)
	{
		cxxKinetics entity;
		entity.mpi_unpack(ints, &i, doubles, &d);
		this->setKinetics(entity.get_n_user(), &entity);
	}
	//std::cerr << "Unpacked Kinetics" << std::endl;

	// PPassemblage
	if (ints[i++] != 0)
	{
		cxxPPassemblage entity;
		entity.mpi_unpack(ints, &i, doubles, &d);
		this->setPPassemblage(entity.get_n_user(), &entity);
	}
	//std::cerr << "Unpacked PPassemblage" << std::endl;

	// SSassemblage
	if (ints[i++] != 0)
	{
		cxxSSassemblage entity;
		entity.mpi_unpack(ints, &i, doubles, &d);
		this->setSSassemblage(entity.get_n_user(), &entity);
	}
	//std::cerr << "Unpacked SSassemblage" << std::endl;

	// Surfaces
	if (ints[i++] != 0)
	{
		cxxSurface entity;
		entity.mpi_unpack(ints, &i, doubles, &d);
		this->setSurface(entity.get_n_user(), &entity);
	}
	//std::cerr << "Unpacked Surface" << std::endl;

	delete[]ints;
	delete[]doubles;
}
#endif
#ifdef SKIP
cxxExchange *
cxxStorageBin::mix_cxxExchange(cxxMix & mixmap)
{
/*
 *   mixes exchangers based on cxxMix structure, returns new exchanger
 *   return exchanger must be freed by calling method
 */
	cxxExchange *new_exch_ptr, *old_exch_ptr;
/*
 *   Zero out global solution data
 */
	new_exch_ptr = new cxxExchange();

	std::map < int, double >::const_iterator it_mix;
	std::map < int, double >*mixcomps = mixmap.comps();

	// Pitzer_exchange_gammas
	it_mix = mixcomps->begin();
	old_exch_ptr = &((this->Exchangers.find(it_mix->first))->second);
	if (old_exch_ptr == NULL)
	{
		sprintf(error_string, "Exchange %d not found in mix_cxxExchange.",
				it_mix->first);
		error_msg(error_string, CONTINUE);
		P_INSTANCE_POINTER input_error++;
		return (NULL);
	}
	new_exch_ptr->set_pitzer_exchange_gammas(old_exch_ptr->
											 get_pitzer_exchange_gammas());
/*
 *   Make list of ExchComps
 */
	std::vector < cxxExchComp > ec_vector;
	std::vector < double >f_vector;
	//
	// make list of all exchange components and their mix fractions
	//
	for (it_mix = mixcomps->begin(); it_mix != mixcomps->end(); it_mix++)
	{
		old_exch_ptr = &((this->Exchangers.find(it_mix->first))->second);
		if (old_exch_ptr == NULL)
		{
			sprintf(error_string, "Exchange %d not found in mix_cxxExchange.",
					it_mix->first);
			error_msg(error_string, CONTINUE);
			P_INSTANCE_POINTER input_error++;
			return (NULL);
		}
		//  Add exchange components to vector ec_vector
		std::list < cxxExchComp >::const_iterator it_ec;
		std::list < cxxExchComp > &eclist = old_exch_ptr->get_exchComps();
		for (it_ec = eclist.begin(); it_ec != eclist.end(); it_ec++)
		{
			f_vector.push_back(it_mix->second);
			//cxxExchComp ec = *it_ec;
			//ec_vector.push_back(ec);
			ec_vector.push_back(*it_ec);
		}
	}
	//
	// Process list to make mixture
	//
	char *current_formula = ec_vector.begin()->get_formula();
	while (current_formula != NULL)
	{

		std::vector < cxxExchComp > ec_subvector;
		std::vector < double >f_subvector;
		std::vector < cxxExchComp >::iterator it_ec = ec_vector.begin();
		std::vector < double >::iterator it_f = f_vector.begin();
		current_formula = NULL;
		for (; it_ec != ec_vector.end(); it_ec++)
		{
			if (*it_f != 0)
			{
				if (current_formula == NULL)
					current_formula = it_ec->get_formula();
				if (it_ec->get_formula() == current_formula)
				{
					ec_subvector.push_back(*it_ec);
					f_subvector.push_back(*it_f);
					*it_f = 0;
					//ec_vector.erase(it_ec);
					//f_vector.erase(it_f);
				}
			}
			it_f++;
		}
		//
		//  mix ec_subvector to make
		// one exchange component
		//
		if (current_formula != NULL)
		{
			cxxExchComp new_comp(ec_subvector, f_subvector);
			new_exch_ptr->get_exchComps().push_back(new_comp);
		}
	}
	/*
	   std::ostringstream oss;
	   new_exch_ptr->dump_raw(oss, 0);
	   std::cerr << oss.str();
	 */
	return (new_exch_ptr);
}
#endif

#ifdef ORCHESTRA
void
cxxStorageBin::ORCH_write(std::ostream & chemistry_dat,
						  std::ostream & input_dat, std::ostream & output_dat)
{
	//std::ostringstream oss;

	// Liter concentrations
	this->system.ORCH_write(chemistry_dat, input_dat, output_dat);

}
#endif
cxxSystem &
cxxStorageBin::getSystem(void)
{
	return this->system;
}

void
cxxStorageBin::setSystem(struct Use *use_ptr)
{
	// Initialize
	this->system.Initialize();
	// Solution
	if (use_ptr->solution_ptr != NULL)
	{
		std::map < int, cxxSolution >::iterator it =
			this->Solutions.find(use_ptr->n_solution_user);
		if (it != this->Solutions.end())
		{
			this->system.setSolution(&(it->second));
		}
	}
	// Exchange
	if (use_ptr->exchange_ptr != NULL)
	{
		std::map < int, cxxExchange >::iterator it =
			this->Exchangers.find(use_ptr->n_exchange_user);
		if (it != this->Exchangers.end())
		{
			this->system.setExchange(&(it->second));
		}
	}
	// gas_phase
	if (use_ptr->gas_phase_ptr != NULL)
	{
		std::map < int, cxxGasPhase >::iterator it =
			this->GasPhases.find(use_ptr->n_gas_phase_user);
		if (it != this->GasPhases.end())
		{
			this->system.setGasPhase(&(it->second));
		}
	}
	// kinetics
	if (use_ptr->kinetics_ptr != NULL)
	{
		std::map < int, cxxKinetics >::iterator it =
			this->Kinetics.find(use_ptr->n_kinetics_user);
		if (it != this->Kinetics.end())
		{
			this->system.setKinetics(&(it->second));
		}
	}
	// pp_assemblage
	if (use_ptr->pp_assemblage_ptr != NULL)
	{
		std::map < int, cxxPPassemblage >::iterator it =
			this->PPassemblages.find(use_ptr->n_pp_assemblage_user);
		if (it != this->PPassemblages.end())
		{
			this->system.setPPassemblage(&(it->second));
		}
	}
	// s_s_assemblage
	if (use_ptr->s_s_assemblage_ptr != NULL)
	{
		std::map < int, cxxSSassemblage >::iterator it =
			this->SSassemblages.find(use_ptr->n_s_s_assemblage_user);
		if (it != this->SSassemblages.end())
		{
			this->system.setSSassemblage(&(it->second));
		}
	}
	// surface
	if (use_ptr->surface_ptr != NULL)
	{
		std::map < int, cxxSurface >::iterator it =
			this->Surfaces.find(use_ptr->n_surface_user);
		if (it != this->Surfaces.end())
		{
			this->system.setSurface(&(it->second));
		}
	}
	// mix
	if (use_ptr->mix_ptr != NULL)
	{
		std::map < int, cxxMix >::iterator it =
			this->Mixes.find(use_ptr->n_mix_user);
		if (it != this->Mixes.end())
		{
			this->system.setMix(&(it->second));
		}
	}
	// reaction
	if (use_ptr->irrev_ptr != NULL)
	{
		std::map < int, cxxReaction >::iterator it =
			this->Reactions.find(use_ptr->n_irrev_user);
		if (it != this->Reactions.end())
		{
			this->system.setReaction(&(it->second));
		}
	}
	// reaction temperature
	if (use_ptr->temperature_ptr != NULL)
	{
		std::map < int, cxxTemperature >::iterator it =
			this->Temperatures.find(use_ptr->n_temperature_user);
		if (it != this->Temperatures.end())
		{
			this->system.setTemperature(&(it->second));
		}
	}
}
void
cxxStorageBin::setSystem(int i)
{
	// Initialize
	this->system.Initialize();
	// Solution
	{
		std::map < int, cxxSolution >::iterator it = this->Solutions.find(i);
		if (it != this->Solutions.end())
		{
			this->system.setSolution(&(it->second));
		}
	}

	// Exchange
	{
		std::map < int, cxxExchange >::iterator it = this->Exchangers.find(i);
		if (it != this->Exchangers.end())
		{
			this->system.setExchange(&(it->second));
		}
	}

	// gas_phase
	{
		std::map < int, cxxGasPhase >::iterator it = this->GasPhases.find(i);
		if (it != this->GasPhases.end())
		{
			this->system.setGasPhase(&(it->second));
		}
	}
	// kinetics
	{
		std::map < int, cxxKinetics >::iterator it = this->Kinetics.find(i);
		if (it != this->Kinetics.end())
		{
			this->system.setKinetics(&(it->second));
		}
	}
	// pp_assemblage
	{
		std::map < int, cxxPPassemblage >::iterator it = this->PPassemblages.find(i);
		if (it != this->PPassemblages.end())
		{
			this->system.setPPassemblage(&(it->second));
		}
	}
	// s_s_assemblage
	{
		std::map < int, cxxSSassemblage >::iterator it = this->SSassemblages.find(i);
		if (it != this->SSassemblages.end())
		{
			this->system.setSSassemblage(&(it->second));
		}
	}
	// surface
	{
		std::map < int, cxxSurface >::iterator it = this->Surfaces.find(i);
		if (it != this->Surfaces.end())
		{
			this->system.setSurface(&(it->second));
		}
	}
	// mix
	{
		std::map < int, cxxMix >::iterator it =	this->Mixes.find(i);
		if (it != this->Mixes.end())
		{
			this->system.setMix(&(it->second));
		}
	}
	// reaction
	{
		std::map < int, cxxReaction >::iterator it = this->Reactions.find(i);
		if (it != this->Reactions.end())
		{
			this->system.setReaction(&(it->second));
		}
	}
	// reaction temperature
	{
		std::map < int, cxxTemperature >::iterator it = this->Temperatures.find(i);
		if (it != this->Temperatures.end())
		{
			this->system.setTemperature(&(it->second));
		}
	}
}
