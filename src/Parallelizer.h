#if !defined(PARALLELIZER_H_INCLUDED)
#define PARALLELIZER_H_INCLUDED
#include "PhreeqcRM.h"
#include <vector>
#include <list>

class Parallelizer: public PhreeqcRM
{
public:
	Parallelizer(int nxyz_arg, MP_TYPE data_for_parallel_processing, PHRQ_io *io=NULL);
	~Parallelizer(void);
	Phreeqc * GetPhreeqcPtr(void) {return this->phreeqc_ptr;}
	void SetPhreeqcPtr(Phreeqc *ptr) {this->phreeqc_ptr = ptr;}

public:
	IRM_RESULT Initialize();
	IRM_RESULT Phreeqc2RM(Phreeqc *phreeqc_ptr);
	IRM_RESULT RM2Phreeqc(Phreeqc *phreeqc_ptr);
	IRM_RESULT RunCellsParallel();

	Phreeqc *phreeqc_ptr;
};
#endif // !defined(PARALLELIZER_H_INCLUDED)