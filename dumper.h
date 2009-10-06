#if !defined(DUMPER_H_INCLUDED)
#define DUMPER_H_INCLUDED
#include <set>					// std::set
#include <string>				// std::string
#include <list>					// std::list
#include <vector>					// std::vector
#include "Parser.h"
class dumper
{
public:
	dumper(void);
	dumper(CParser & parser);
	~dumper(void);
	bool Read(CParser & parser);
	void DumpAll(bool tf);
	std::string get_file_name(void) { return(this->file_name); };
	bool get_append(void) { return(this->append); };
	bool get_dump_solution(void) { return(this->dump_solution); };
	bool get_dump_pp_assemblage(void) { return(this->dump_pp_assemblage); };
	bool get_dump_exchange(void) { return(this->dump_exchange); };
	bool get_dump_surface(void) { return(this->dump_surface); };
	bool get_dump_s_s_assemblage(void) { return(this->dump_s_s_assemblage); };
	bool get_dump_gas_phase(void) { return(this->dump_gas_phase); };
	bool get_dump_kinetics(void) { return(this->dump_kinetics); };

	std::set < int > & get_solution(void) { return(this->solution); };
	std::set < int > & get_pp_assemblage(void) { return(this->pp_assemblage); };
	std::set < int > & get_exchange(void) { return(this->exchange); };
	std::set < int > & get_surface(void) { return(this->surface); };
	std::set < int > & get_s_s_assemblage(void) { return(this->s_s_assemblage); };
	std::set < int > & get_gas_phase(void) { return(this->gas_phase); };
	std::set < int > & get_kinetics(void) { return(this->kinetics); };
protected:
	std::string file_name;
	bool append;
	bool dump_solution;
	bool dump_pp_assemblage;
	bool dump_exchange;
	bool dump_surface;
	bool dump_s_s_assemblage;
	bool dump_gas_phase;
	bool dump_kinetics;

	std::set < int > solution;
	std::set < int > pp_assemblage;
	std::set < int > exchange;
	std::set < int > surface;
	std::set < int > s_s_assemblage;
	std::set < int > gas_phase;
	std::set < int > kinetics;
};

#endif // !defined(DUMPER_H_INCLUDED)