#if !defined(REACTION_H_INCLUDED)
#define REACTION_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

#include "NumKeyword.h"
#include "NameDouble.h"

class cxxReaction:public cxxNumKeyword
{

  public:
	cxxReaction(PHRQ_io *io = NULL);
	cxxReaction(struct irrev *, PHRQ_io *io = NULL);
	 ~cxxReaction();

	//void dump_xml(std::ostream& os, unsigned int indent = 0)const;

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void read_raw(CParser & parser, bool check);
	const cxxNameDouble &Get_elementList(void) const {return this->elementList;};
	const cxxNameDouble &Get_reactantList(void) const {return this->reactantList;};
	const std::vector < double > &Get_steps(void) const {return this->steps;};
	int Get_countSteps(void) const {return this->countSteps;};
	bool Get_equalIncrements(void) const {return this->equalIncrements;};
	const std::string &Get_units(void) const {return this->units;};

	void Set_units(const char * s)
	{
		if (s != NULL)
			this->units = std::string(s);
		else
			this->units.clear();
	}

protected:
	cxxNameDouble reactantList;
	cxxNameDouble elementList;
	std::vector < double >steps;
	int countSteps;
	bool equalIncrements;
	std::string units;

public:
	//static std::map<int, cxxReaction>& map;

};

#endif // !defined(REACTION_H_INCLUDED)
