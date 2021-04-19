#if !defined(RATE_H_INCLUDED)
#define RATE_H_INCLUDED
#include <string>
/* ----------------------------------------------------------------------
 *   RATES
 * ---------------------------------------------------------------------- */
class rate
{
public:
	~rate() {};
	rate()
	{
		//name.clear();
		//std::string commands;
		new_def = 0;
		linebase = NULL;
		varbase = NULL;
		loopbase = NULL;
	}
	std::string name;
	std::string commands;
	int new_def;
	void* linebase;
	void* varbase;
	void* loopbase;
};
#endif // !defined(RATE_H_INCLUDED)