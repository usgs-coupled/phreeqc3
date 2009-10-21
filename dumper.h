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
	bool get_bool_solution(void) { return(this->bool_solution); };
	bool get_bool_pp_assemblage(void) { return(this->bool_pp_assemblage); };
	bool get_bool_exchange(void) { return(this->bool_exchange); };
	bool get_bool_surface(void) { return(this->bool_surface); };
	bool get_bool_s_s_assemblage(void) { return(this->bool_s_s_assemblage); };
	bool get_bool_gas_phase(void) { return(this->bool_gas_phase); };
	bool get_bool_kinetics(void) { return(this->bool_kinetics); };

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
	bool bool_solution;
	bool bool_pp_assemblage;
	bool bool_exchange;
	bool bool_surface;
	bool bool_s_s_assemblage;
	bool bool_gas_phase;
	bool bool_kinetics;

	std::set < int > solution;
	std::set < int > pp_assemblage;
	std::set < int > exchange;
	std::set < int > surface;
	std::set < int > s_s_assemblage;
	std::set < int > gas_phase;
	std::set < int > kinetics;
};

#endif // !defined(DUMPER_H_INCLUDED)