#if !defined(RUNNER_H_INCLUDED)
#define RUNNER_H_INCLUDED
#include <set>					// std::set
#include <string>				// std::string

#include "StorageBinList.h"
#include "PHRQ_base.h"
class CParser;

class runner: public PHRQ_base
{
public:
	runner(PHRQ_io *io=NULL);
	runner(CParser & parser, PHRQ_io *io=NULL);
	virtual ~runner(void);
	bool Read(CParser & parser);
	StorageBinListItem & Get_cells(void) { return(this->cells); };
	double Get_time_step() { return(this->time_step); };
	double Get_start_time() { return(this->start_time); };
	bool Get_run_cells() { return(this->run_cells); };
	void Set_run_cells(bool tf) { this->run_cells = tf; };

protected:
	double time_step;
	double start_time;
	StorageBinListItem cells;
	bool run_cells;
};
#endif // !defined(RUNNER_H_INCLUDED)
