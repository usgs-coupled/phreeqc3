#if !defined(NAMEDOUBLE_H_INCLUDED)
#define NAMEDOUBLE_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector
class Phreeqc;
#include "Parser.h"
#include "phrqtype.h"

class cxxNameDouble:public
	std::map < std::string, LDBLE >
{

  public:
	enum ND_TYPE
	{
		ND_ELT_MOLES = 1,
		ND_SPECIES_LA = 2,
		ND_SPECIES_GAMMA = 3,
		ND_NAME_COEF = 4
	};

	cxxNameDouble();
	cxxNameDouble(struct elt_list *);
	cxxNameDouble(struct elt_list *, int count);
	cxxNameDouble(struct conc *);
	cxxNameDouble(struct master_activity *ma, int count, ND_TYPE);
	cxxNameDouble(struct name_coef *nc, int count);
	cxxNameDouble(const cxxNameDouble & old, LDBLE factor);
	 ~
	cxxNameDouble();

	void
	dump_xml(std::ostream & s_oss, unsigned int indent) const;
	void
	dump_raw(std::ostream & s_oss, unsigned int indent) const;
	CParser::STATUS_TYPE
	read_raw(CParser & parser, std::istream::pos_type & pos);
	void
	add_extensive(const cxxNameDouble & old, LDBLE factor);
	void
	add_intensive(const cxxNameDouble & addee, LDBLE fthis, LDBLE f2);
	void
	add_log_activities(const cxxNameDouble & addee, LDBLE fthis, LDBLE f2);
	void
	add(const char *key, LDBLE total);
	void
	multiply(LDBLE factor);
	void
	merge_redox(const cxxNameDouble & source);

	std::vector< std::pair<std::string, LDBLE> > 
	sort_second(void);

	void
	insert(const char *str, LDBLE d)
	{
		(*this)[str] = d;
	}
	void
	mpi_pack(std::vector < int >&ints, std::vector < LDBLE >&doubles);
	void
	mpi_pack(int *ints, int *ii, LDBLE *doubles, int *dd);

	void
	mpi_unpack(int *ints, int *ii, LDBLE *doubles, int *dd);

	enum ND_TYPE
		type;

};

#endif // !defined(NAMEDOUBLE_H_INCLUDED)
