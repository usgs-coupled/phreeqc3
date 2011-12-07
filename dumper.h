#if !defined(DUMPER_H_INCLUDED)
#define DUMPER_H_INCLUDED
#include <set>					// std::set
#include <string>				// std::string
#include <list>					// std::list
#include <vector>					// std::vector
#include "StorageBinList.h"
class CParser;

class dumper: public PHRQ_base
{
public:
	dumper(PHRQ_io *io=NULL);
	dumper(CParser & parser, PHRQ_io *io=NULL);
	~dumper(void);
	bool Read(CParser & parser);
	void SetAll(bool tf);
	std::string Get_file_name(void)		{ return(this->file_name); };
	bool Get_append(void)				{ return(this->append); };
	bool Get_bool_solution(void)		{ return(this->binList.Get_solution().Get_defined()); };
	bool Get_bool_pp_assemblage(void)	{ return(this->binList.Get_pp_assemblage().Get_defined()); };
	bool Get_bool_exchange(void)		{ return(this->binList.Get_exchange().Get_defined()); };
	bool Get_bool_surface(void)			{ return(this->binList.Get_surface().Get_defined()); };
	bool Get_bool_s_s_assemblage(void)	{ return(this->binList.Get_s_s_assemblage().Get_defined()); };
	bool Get_bool_gas_phase(void)		{ return(this->binList.Get_gas_phase().Get_defined()); };
	bool Get_bool_kinetics(void)		{ return(this->binList.Get_kinetics().Get_defined()); };
	bool Get_bool_mix(void)				{ return(this->binList.Get_mix().Get_defined()); };
	bool Get_bool_reaction(void)		{ return(this->binList.Get_reaction().Get_defined()); };
	bool Get_bool_temperature(void)		{ return(this->binList.Get_temperature().Get_defined()); };
	bool Get_bool_pressure(void)		{ return(this->binList.Get_pressure().Get_defined()); };
	bool Get_bool_any(void);

	std::set < int > & Get_solution(void)		{ return(this->binList.Get_solution().Get_numbers()); };
	std::set < int > & Get_pp_assemblage(void)	{ return(this->binList.Get_pp_assemblage().Get_numbers()); };
	std::set < int > & Get_exchange(void)		{ return(this->binList.Get_exchange().Get_numbers()); };
	std::set < int > & Get_surface(void)		{ return(this->binList.Get_surface().Get_numbers()); };
	std::set < int > & Get_s_s_assemblage(void) { return(this->binList.Get_s_s_assemblage().Get_numbers()); };
	std::set < int > & Get_gas_phase(void)		{ return(this->binList.Get_gas_phase().Get_numbers()); };
	std::set < int > & Get_kinetics(void)		{ return(this->binList.Get_kinetics().Get_numbers()); };
	std::set < int > & Get_mix(void)			{ return(this->binList.Get_mix().Get_numbers()); };
	std::set < int > & Get_reaction(void)		{ return(this->binList.Get_reaction().Get_numbers()); };
	std::set < int > & Get_temperature(void)	{ return(this->binList.Get_temperature().Get_numbers()); };
	std::set < int > & Get_pressure(void)		{ return(this->binList.Get_pressure().Get_numbers()); };
	bool Get_on(void)							{return this->on;};
	void Set_on(bool tf)						{this->on = tf;};
protected:
	std::string file_name;
	bool append;
	bool on;
	StorageBinList binList;
};

#endif // !defined(DUMPER_H_INCLUDED)
