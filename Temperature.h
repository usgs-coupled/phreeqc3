#if !defined(TEMPERATURE_H_INCLUDED)
#define TEMPERATURE_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

#include "NumKeyword.h"

class cxxTemperature:public cxxNumKeyword
{

  public:
	cxxTemperature(PHRQ_io *io=NULL);
	cxxTemperature(struct temperature *, PHRQ_io *io=NULL);
	~cxxTemperature();

	//void dump_xml(std::ostream& os, unsigned int indent = 0)const;

	void dump_raw(std::ostream & s_oss, unsigned int indent, int *n_out=NULL) const;

	void read_raw(CParser & parser);

	const std::vector<double> & Get_temps(void) const {return temps;};
	int Get_countTemps(void) const {return countTemps;};
	bool Get_equalIncrements(void) const {return equalIncrements;};

protected:
	std::vector < double >temps;
	int countTemps;
	bool equalIncrements;

public:
	//static std::map<int, cxxTemperature>& map;

};

#endif // !defined(TEMPERATURE_H_INCLUDED)
