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

	struct irrev *cxxReaction2irrev(PHREEQC_PTR_ARG);

	//void dump_xml(std::ostream& os, unsigned int indent = 0)const;

	void dump_raw(std::ostream & s_oss, unsigned int indent) const;

	void read_raw(PHREEQC_PTR_ARG_COMMA CParser & parser, bool check);
	cxxNameDouble &get_elementList(void)
	{
		return this->elementList;
	}

	void set_units(const char * s)
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
