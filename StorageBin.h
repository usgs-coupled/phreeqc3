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
        enum SB_CONSTRUCTOR {
                SB_GLOBAL       = 1
        };

        cxxStorageBin();
        cxxStorageBin(SB_CONSTRUCTOR flag);
        ~cxxStorageBin();

        //void dump_xml(std::ostream& os, unsigned int indent = 0)const;

	struct cxxSolution *get_solution(int i) { 
		//cxxSolution *solution_ptr = Utilities::get_entity(this->Solutions, i);
		return(Utilities::get_entity(this->Solutions, i));
		//if (this->Solutions.find(i) != this->Solutions.end()) {
		//return(&(this->Solutions.find(i)->second));
		//} else {
		//return(NULL);
		//}
	}

        void dump_raw(std::ostream& s_oss, unsigned int indent)const;

        void read_raw(CParser& parser);

	void add(struct system *sys_ptr);

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
