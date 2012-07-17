#if !defined(STORAGEBINLIST_H_INCLUDED)
#define STORAGEBINLIST_H_INCLUDED
#include <set>					// std::set
#include <string>				// std::string
#include <list>					// std::list
#include <vector>					// std::vector
#include "PHRQ_base.h"
class CParser;

class StorageBinListItem
{
public:
	StorageBinListItem(void);
	StorageBinListItem(CParser & parser);
	~StorageBinListItem(void);
	void Set_defined(bool tf) { this->defined = tf; };
	bool Get_defined(void) { return(this->defined); };
	void Augment(std::string token);
	void Augment(int i);
	std::set < int > &Get_numbers(void) { return(this->numbers); };
	void Clear(void) { this->numbers.clear(); };
protected:
	std::set < int > numbers;
	bool defined;
};
class StorageBinList: public PHRQ_base
{
public:
	StorageBinList(PHRQ_io *io=NULL);
	StorageBinList(CParser & parser, PHRQ_io *io=NULL);
	virtual ~StorageBinList(void);
	bool Read(CParser & parser);
	void SetAll(bool tf);
	void TransferAll(StorageBinListItem &source);

	StorageBinListItem & Get_solution(void) { return(this->solution); };
	StorageBinListItem & Get_pp_assemblage(void) { return(this->pp_assemblage); };
	StorageBinListItem & Get_exchange(void) { return(this->exchange); };
	StorageBinListItem & Get_surface(void) { return(this->surface); };
	StorageBinListItem & Get_ss_assemblage(void) { return(this->ss_assemblage); };
	StorageBinListItem & Get_gas_phase(void) { return(this->gas_phase); };
	StorageBinListItem & Get_kinetics(void) { return(this->kinetics); };
	StorageBinListItem & Get_mix(void) { return(this->mix); };
	StorageBinListItem & Get_reaction(void) { return(this->reaction); };
	StorageBinListItem & Get_temperature(void) { return(this->temperature); };
	StorageBinListItem & Get_pressure(void) { return(this->pressure); };
protected:
	StorageBinListItem solution;
	StorageBinListItem pp_assemblage;
	StorageBinListItem exchange;
	StorageBinListItem surface;
	StorageBinListItem ss_assemblage;
	StorageBinListItem gas_phase;
	StorageBinListItem kinetics;
	StorageBinListItem mix;
	StorageBinListItem reaction;
	StorageBinListItem temperature;
	StorageBinListItem pressure;
	const static std::vector < std::string > vopts;
};


#endif // !defined(STORAGEBINLIST_H_INCLUDED)
