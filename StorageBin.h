#if !defined(STORAGEBIN_H_INCLUDED)
#define STORAGEBIN_H_INCLUDED
#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

#include "System.h"
#include "PHRQ_io.h"
#include "PHRQ_base.h"
class cxxSolution;
class cxxExchange;
class cxxGasPhase;
class cxxKinetics;
class cxxPPassemblage;
class cxxSSassemblage;
class cxxSurface;
class cxxReaction;
class cxxTemperature;

class cxxStorageBin: public PHRQ_base
{

  public:
	cxxStorageBin(PHRQ_io *io=NULL);
	~cxxStorageBin();

	void Remove(int n);
	void Clear(void);

	cxxSolution *Get_Solution(int n_user);
	void Set_Solution(int n_user, cxxSolution * entity);
	void Set_Solution(int n_user, cxxSolution & entity);
	void Remove_Solution(int n_user);

	cxxExchange *Get_Exchange(int n_user);
	void Set_Exchange(int n_user, cxxExchange * entity);
	void Remove_Exchange(int n_user);

	cxxPPassemblage *Get_PPassemblage(int n_user);
	void Set_PPassemblage(int n_user, cxxPPassemblage * entity);
	void Set_PPassemblage(int n_user, cxxPPassemblage & entity);
	void Remove_PPassemblage(int n_user);

	cxxGasPhase *Get_GasPhase(int n_user);
	void Set_GasPhase(int n_user, cxxGasPhase * entity);
	void Remove_GasPhase(int n_user);

	cxxSSassemblage *Get_SSassemblage(int n_user);
	void Set_SSassemblage(int n_user, cxxSSassemblage * entity);
	void Set_SSassemblage(int n_user, cxxSSassemblage & entity);
	void Remove_SSassemblage(int n_user);

	cxxKinetics *Get_Kinetics(int n_user);
	void Set_Kinetics(int n_user, cxxKinetics * entity);
	void Remove_Kinetics(int n_user);

	cxxSurface *Get_Surface(int n_user);
	void Set_Surface(int n_user, cxxSurface * entity);
	void Remove_Surface(int n_user);

	cxxMix *Get_Mix(int n_user);
	void Set_Mix(int n_user, cxxMix * entity);
	void Remove_Mix(int n_user);

	cxxReaction *Get_Reaction(int n_user);
	void Set_Reaction(int n_user, cxxReaction * entity);
	void Remove_Reaction(int n_user);

	cxxTemperature *Get_Temperature(int n_user);
	void Set_Temperature(int n_user, cxxTemperature * entity);
	void Remove_Temperature(int n_user);

	cxxSystem &Get_System(void);
	void Set_System(struct Use *use_ptr);
	void Set_System(int i);

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void dump_raw(std::ostream & s_oss, int i, unsigned int indent, int *n_out=NULL);

	void read_raw(CParser & parser);
	int read_raw_keyword(CParser & parser);

	//void add(struct system *sys_ptr);

	//cxxSolution *mix_cxxSolutions(cxxMix &mixmap);
	cxxExchange *mix_cxxExchange(cxxMix & mixmap);

	const std::map < int, cxxSolution > &Get_Solutions() const;
	const std::map < int, cxxExchange > &Get_Exchangers() const;
	const std::map < int, cxxGasPhase > &Get_GasPhases() const;
	const std::map < int, cxxKinetics > &Get_Kinetics() const;
	const std::map < int, cxxPPassemblage > &Get_PPassemblages() const;
	const std::map < int, cxxSSassemblage > &Get_SSassemblages() const;
	const std::map < int, cxxSurface > &Get_Surfaces() const;
	const std::map < int, cxxMix > &Get_Mixes() const;
	const std::map < int, cxxReaction > &Get_Reactions() const;
	const std::map < int, cxxTemperature > &Get_Temperatures() const;

	cxxSystem & Get_system(void) {return system;};
#ifdef USE_MPI
	void mpi_send(int n, int task_number);
	void mpi_recv(int task_number);
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
