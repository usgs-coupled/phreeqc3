#if !defined(STORAGEBIN_H_INCLUDED)
#define STORAGEBIN_H_INCLUDED

#include "Utils.h"
#include "Parser.h"
#include "Solution.h"
#include "Exchange.h"
#include "GasPhase.h"
#include "cxxKinetics.h"
#include "PPassemblage.h"
//#include "SSassemblage.h"
class NameDouble;
class cxxSSassemblageSS;
#include "Surface.h"
#include "System.h"
//#include "cxxMix.h"
class cxxMix;
#include "Reaction.h"
#include "Temperature.h"

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

class cxxStorageBin
{

  public:
	cxxStorageBin();
	cxxStorageBin(struct Use *use_ptr);
	 ~cxxStorageBin();

	void import_phreeqc(void);

	void cxxStorageBin2phreeqc(int n);

	void phreeqc2cxxStorageBin(int n);

	void remove(int n);

	cxxSolution *getSolution(int n_user)
	{
		if (this->Solutions.find(n_user) != this->Solutions.end())
		{
			return (&(this->Solutions.find(n_user)->second));
		}
		return (NULL);
	}
	void setSolution(int n_user, cxxSolution * entity)
	{
		if (entity == NULL)
			return;
		Solutions[n_user] = *entity;
	}
	void removeSolution(int n_user)
	{
		Solutions.erase(n_user);
	}

	cxxExchange *getExchange(int n_user)
	{
		if (this->Exchangers.find(n_user) != this->Exchangers.end())
		{
			return (&(this->Exchangers.find(n_user)->second));
		}
		return (NULL);
	}
	void setExchange(int n_user, cxxExchange * entity)
	{
		if (entity == NULL)
			return;
		Exchangers[n_user] = *entity;
	}
	void removeExchange(int n_user)
	{
		Exchangers.erase(n_user);
	}

	cxxPPassemblage *getPPassemblage(int n_user)
	{
		if (this->PPassemblages.find(n_user) != this->PPassemblages.end())
		{
			return (&(this->PPassemblages.find(n_user)->second));
		}
		return (NULL);
	}
	void setPPassemblage(int n_user, cxxPPassemblage * entity)
	{
		if (entity == NULL)
			return;
		PPassemblages[n_user] = *entity;
	}
	void removePPassemblage(int n_user)
	{
		PPassemblages.erase(n_user);
	}

	cxxGasPhase *getGasPhase(int n_user)
	{
		if (this->GasPhases.find(n_user) != this->GasPhases.end())
		{
			return (&(this->GasPhases.find(n_user)->second));
		}
		return (NULL);
	}
	void setGasPhase(int n_user, cxxGasPhase * entity)
	{
		if (entity == NULL)
			return;
		GasPhases[n_user] = *entity;
	}
	void removeGasPhase(int n_user)
	{
		GasPhases.erase(n_user);
	}

	cxxSSassemblage *getSSassemblage(int n_user)
	{
		if (this->SSassemblages.find(n_user) != this->SSassemblages.end())
		{
			return (&(this->SSassemblages.find(n_user)->second));
		}
		return (NULL);
	}
	void setSSassemblage(int n_user, cxxSSassemblage * entity)
	{
		if (entity == NULL)
			return;
		SSassemblages[n_user] = *entity;
	}
	void removeSSassemblage(int n_user)
	{
		SSassemblages.erase(n_user);
	}

	cxxKinetics *getKinetics(int n_user)
	{
		if (this->Kinetics.find(n_user) != this->Kinetics.end())
		{
			return (&(this->Kinetics.find(n_user)->second));
		}
		return (NULL);
	}
	void setKinetics(int n_user, cxxKinetics * entity)
	{
		if (entity == NULL)
			return;
		Kinetics[n_user] = *entity;
	}
	void removeKinetics(int n_user)
	{
		Kinetics.erase(n_user);
	}

	cxxSurface *getSurface(int n_user)
	{
		if (this->Surfaces.find(n_user) != this->Surfaces.end())
		{
			return (&(this->Surfaces.find(n_user)->second));
		}
		return (NULL);
	}
	void setSurface(int n_user, cxxSurface * entity)
	{
		if (entity == NULL)
			return;
		Surfaces[n_user] = *entity;
	}
	void removeSurface(int n_user)
	{
		Surfaces.erase(n_user);
	}

	cxxMix *getMix(int n_user)
	{
		if (this->Mixes.find(n_user) != this->Mixes.end())
		{
			return (&(this->Mixes.find(n_user)->second));
		}
		return (NULL);
	}
	void setMix(int n_user, cxxMix * entity)
	{
		if (entity == NULL)
			return;
		Mixes[n_user] = *entity;
	}
	void removeMix(int n_user)
	{
		Mixes.erase(n_user);
	}

	cxxReaction *getReaction(int n_user)
	{
		if (this->Reactions.find(n_user) != this->Reactions.end())
		{
			return (&(this->Reactions.find(n_user)->second));
		}
		return (NULL);
	}
	void setReaction(int n_user, cxxReaction * entity)
	{
		if (entity == NULL)
			return;
		Reactions[n_user] = *entity;
	}
	void removeReaction(int n_user)
	{
		Reactions.erase(n_user);
	}

	cxxTemperature *getTemperature(int n_user)
	{
		if (this->Temperatures.find(n_user) != this->Temperatures.end())
		{
			return (&(this->Temperatures.find(n_user)->second));
		}
		return (NULL);
	}
	void setTemperature(int n_user, cxxTemperature * entity)
	{
		if (entity == NULL)
			return;
		Temperatures[n_user] = *entity;
	}
	void removeTemperature(int n_user)
	{
		Temperatures.erase(n_user);
	}

	void setSystem(struct Use *use_ptr);

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void dump_raw(std::ostream & s_oss, int i, unsigned int indent);

	void read_raw(CParser & parser);
	int read_raw_keyword(CParser & parser);

	void add(struct system *sys_ptr);

	struct system *cxxStorageBin2system(int i);

	//cxxSolution *mix_cxxSolutions(cxxMix &mixmap);
	cxxExchange *mix_cxxExchange(cxxMix & mixmap);

	const std::map < int, cxxSolution > &getSolutions() const
	{
		return this->Solutions;
	};
	const std::map < int, cxxExchange > &getExchangers() const
	{
		return this->Exchangers;
	};
	const std::map < int, cxxGasPhase > &getGasPhases() const
	{
		return this->GasPhases;
	};
	const std::map < int, cxxKinetics > &getKinetics() const
	{
		return this->Kinetics;
	};
	const std::map < int, cxxPPassemblage > &getPPassemblages() const
	{
		return this->PPassemblages;
	};
	const std::map < int, cxxSSassemblage > &getSSassemblages() const
	{
		return this->SSassemblages;
	};
	const std::map < int, cxxSurface > &getSurfaces() const
	{
		return this->Surfaces;
	};

#ifdef USE_MPI
	void mpi_send(int n, int task_number);
	void mpi_recv(int task_number);
#endif
#ifdef ORCHESTRA
	void ORCH_write(std::ostream & chemistry_dat, std::ostream & input_dat,
					std::ostream & output_dat);
#endif
  protected:
	// Tidied classes
	std::map < int, cxxSolution > Solutions;
	std::map < int, cxxExchange > Exchangers;
	std::map < int, cxxGasPhase > GasPhases;
	std::map < int, cxxKinetics > Kinetics;
	std::map < int, cxxPPassemblage > PPassemblages;
	std::map < int, cxxSSassemblage > SSassemblages;
	std::map < int, cxxSurface > Surfaces;

	//bool b = exists (Solutions, 5);
	// Initial classes
	//std::map<int, cxxISolution>     ISolutions;

	// Reaction classes
	std::map < int, cxxMix > Mixes;
	std::map < int, cxxReaction > Reactions;
	std::map < int, cxxTemperature > Temperatures;
	cxxSystem system;
  public:
	//static std::map<int, cxxStorageBin>& map;

};

#endif // !defined(STORAGEBIN_H_INCLUDED)
