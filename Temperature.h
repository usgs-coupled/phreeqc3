#if !defined(TEMPERATURE_H_INCLUDED)
#define TEMPERATURE_H_INCLUDED

#include "NumKeyword.h"
#define EXTERNAL extern
#include "global.h"
#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

class cxxTemperature:public cxxNumKeyword
{

  public:
	cxxTemperature();
	cxxTemperature(struct temperature *);
	 ~cxxTemperature();

	struct temperature *cxxTemperature2temperature();

	//void dump_xml(std::ostream& os, unsigned int indent = 0)const;

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void read_raw(CParser & parser);

protected:
	std::vector < double >temps;
	int countTemps;
	bool equalIncrements;

public:
	//static std::map<int, cxxTemperature>& map;

};

#endif // !defined(TEMPERATURE_H_INCLUDED)
