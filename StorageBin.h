#if !defined(STORAGEBIN_H_INCLUDED)
#define STORAGEBIN_H_INCLUDED

#include "Utils.h"
#include "Parser.h"
#include "Solution.h"
#include "Exchange.h"
#include "GasPhase.h"
#include "KineticsCxx.h"
#include "PPassemblage.h"
#include "SSassemblage.h"
#include "Surface.h"
#include "Mix.h"
#include "Reaction.h"
#include "Temperature.h"

#include <cassert> // assert
#include <map>     // std::map
#include <string>  // std::string
#include <list>    // std::list
#include <vector>  // std::vector

class cxxStorageBin 
{

public:
        cxxStorageBin();

        ~cxxStorageBin();

        void import_phreeqc(void);

	void cxxStorageBin2phreeqc(int n);

	void phreeqc2cxxStorageBin(int n);

	struct cxxSolution *getSolution(int n_user) { 
		if (this->Solutions.find(n_user) != this->Solutions.end()) {
			return(&(this->Solutions.find(n_user)->second));
		} 
		return (NULL);
	}
	void setSolution(int n_user, cxxSolution *entity) { 
		Solutions[n_user] = *entity;
	}

	struct cxxExchange *getExchange(int n_user) { 
		if (this->Exchangers.find(n_user) != this->Exchangers.end()) {
			return(&(this->Exchangers.find(n_user)->second));
		} 
		return (NULL);
	}
	void setExchange(int n_user, cxxExchange *entity) { 
		Exchangers[n_user] = *entity;
	}

	struct cxxPPassemblage *getPPassemblage(int n_user) { 
		if (this->PPassemblages.find(n_user) != this->PPassemblages.end()) {
			return(&(this->PPassemblages.find(n_user)->second));
		} 
		return (NULL);
	}
	void setPPassemblage(int n_user, cxxPPassemblage *entity) { 
		PPassemblages[n_user] = *entity;
	}
	struct cxxGasPhase *getGasPhase(int n_user) { 
		if (this->GasPhases.find(n_user) != this->GasPhases.end()) {
			return(&(this->GasPhases.find(n_user)->second));
		} 
		return (NULL);
	}
	void setGasPhase(int n_user, cxxGasPhase *entity) { 
		GasPhases[n_user] = *entity;
	}

	struct cxxSSassemblage *getSSassemblage(int n_user) { 
		if (this->SSassemblages.find(n_user) != this->SSassemblages.end()) {
			return(&(this->SSassemblages.find(n_user)->second));
		} 
		return (NULL);
	}
	void setSSassemblage(int n_user, cxxSSassemblage *entity) { 
		SSassemblages[n_user] = *entity;
	}

	struct cxxKinetics *getKinetics(int n_user) { 
		if (this->Kinetics.find(n_user) != this->Kinetics.end()) {
			return(&(this->Kinetics.find(n_user)->second));
		} 
		return (NULL);
	}
	void setKinetics(int n_user, cxxKinetics *entity) { 
		Kinetics[n_user] = *entity;
	}

	struct cxxSurface *getSurface(int n_user) { 
		if (this->Surfaces.find(n_user) != this->Surfaces.end()) {
			return(&(this->Surfaces.find(n_user)->second));
		} 
		return (NULL);
	}
	void setSurface(int n_user, cxxSurface *entity) { 
		Surfaces[n_user] = *entity;
	}

        void dump_raw(std::ostream& s_oss, unsigned int indent)const;

        void read_raw(CParser& parser);

	void add(struct system *sys_ptr);

	cxxSolution *mix_cxxSolutions(cxxMix &mixmap);

protected:
	// Tidied classes
	std::map<int, cxxSolution>      Solutions;
	std::map<int, cxxExchange>      Exchangers;
	std::map<int, cxxGasPhase>      GasPhases;
	std::map<int, cxxKinetics>      Kinetics;
	std::map<int, cxxPPassemblage>  PPassemblages;
	std::map<int, cxxSSassemblage>  SSassemblages;
	std::map<int, cxxSurface>       Surfaces;

	//bool b = exists (Solutions, 5);
	// Initial classes
	//std::map<int, cxxISolution>     ISolutions;

	// Reaction classes
	std::map<int, cxxMix>           Mixes;
	std::map<int, cxxReaction>      Reactions;
	std::map<int, cxxTemperature>   Temperatures;

public:
        //static std::map<int, cxxStorageBin>& map;

};

#endif // !defined(STORAGEBIN_H_INCLUDED)
