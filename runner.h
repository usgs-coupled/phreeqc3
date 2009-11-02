#if !defined(RUNNER_H_INCLUDED)
#define RUNNER_H_INCLUDED
#include <set>					// std::set
#include <string>				// std::string
#include "Parser.h"
#include "StorageBinList.h"
class runner
{
public:
	runner(void);
	runner(CParser & parser);
	virtual ~runner(void);
	bool Read(CParser & parser);
	StorageBinListItem & Get_cells(void) { return(this->cells); };
	double Get_time_step() { return(this->time_step); };
	double Get_start_time() { return(this->start_time); };

protected:
	double time_step;
	double start_time;
	StorageBinListItem cells;
};
#endif // !defined(RUNNER_H_INCLUDED)