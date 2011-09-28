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
	//cxxStorageBin(struct Use *use_ptr);
	cxxStorageBin(PHREEQC_PTR_ARG_COMMA struct Use *use_ptr, PHRQ_io *io=NULL);
	 ~cxxStorageBin();

	void import_phreeqc(PHREEQC_PTR_ARG);

	void cxxStorageBin2phreeqc(PHREEQC_PTR_ARG_COMMA int n);

	void phreeqc2cxxStorageBin(PHREEQC_PTR_ARG_COMMA int n);

	void remove(int n);

	cxxSolution *getSolution(int n_user);
	void setSolution(int n_user, cxxSolution * entity);
	void setSolution(int n_user, cxxSolution & entity);
	void removeSolution(int n_user);

	cxxExchange *getExchange(int n_user);
	void setExchange(int n_user, cxxExchange * entity);
	void removeExchange(int n_user);

	cxxPPassemblage *getPPassemblage(int n_user);
	void setPPassemblage(int n_user, cxxPPassemblage * entity);
	void setPPassemblage(int n_user, cxxPPassemblage & entity);
	void removePPassemblage(int n_user);

	cxxGasPhase *getGasPhase(int n_user);
	void setGasPhase(int n_user, cxxGasPhase * entity);
	void removeGasPhase(int n_user);

	cxxSSassemblage *getSSassemblage(int n_user);
	void setSSassemblage(int n_user, cxxSSassemblage * entity);
	void setSSassemblage(int n_user, cxxSSassemblage & entity);
	void removeSSassemblage(int n_user);

	cxxKinetics *getKinetics(int n_user);
	void setKinetics(int n_user, cxxKinetics * entity);
	void removeKinetics(int n_user);

	cxxSurface *getSurface(int n_user);
	void setSurface(int n_user, cxxSurface * entity);
	void removeSurface(int n_user);

	cxxMix *getMix(int n_user);
	void setMix(int n_user, cxxMix * entity);
	void removeMix(int n_user);

	cxxReaction *getReaction(int n_user);
	void setReaction(int n_user, cxxReaction * entity);
	void removeReaction(int n_user);

	cxxTemperature *getTemperature(int n_user);
	void setTemperature(int n_user, cxxTemperature * entity);
	void removeTemperature(int n_user);

	cxxSystem &getSystem(void);
	void setSystem(struct Use *use_ptr);
	void setSystem(int i);

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void dump_raw(std::ostream & s_oss, int i, unsigned int indent);

	void read_raw(CParser & parser);
	int read_raw_keyword(CParser & parser);

	void add(struct system *sys_ptr);

	struct system *cxxStorageBin2system(PHREEQC_PTR_ARG_COMMA int i);

	//cxxSolution *mix_cxxSolutions(cxxMix &mixmap);
	cxxExchange *mix_cxxExchange(cxxMix & mixmap);

	const std::map < int, cxxSolution > &getSolutions() const;
	const std::map < int, cxxExchange > &getExchangers() const;
	const std::map < int, cxxGasPhase > &getGasPhases() const;
	const std::map < int, cxxKinetics > &getKinetics() const;
	const std::map < int, cxxPPassemblage > &getPPassemblages() const;
	const std::map < int, cxxSSassemblage > &getSSassemblages() const;
	const std::map < int, cxxSurface > &getSurfaces() const;

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
