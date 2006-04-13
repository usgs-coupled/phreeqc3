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

	struct cxxSolution *get_solution(int n_user) { 
		if (this->Solutions.find(n_user) != this->Solutions.end()) {
			return(&(this->Solutions.find(n_user)->second));
		} 
		return (NULL);
	}
	void set_solution(int n_user, cxxSolution soln) { 
		Solutions[n_user] = soln;
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
