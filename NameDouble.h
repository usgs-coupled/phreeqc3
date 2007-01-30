#if !defined(NAMEDOUBLE_H_INCLUDED)
#define NAMEDOUBLE_H_INCLUDED

//#define EXTERNAL extern
#include <cassert> // assert
#include <map>     // std::map
#include <string>  // std::string
#include <list>    // std::list
#include <vector>  // std::vector

//#include "global.h"
#include "char_star.h"
#include "Parser.h"
class cxxNameDouble : public std::map <char *, double, CHARSTAR_LESS> 
{

public:
        enum ND_TYPE {
                ND_ELT_MOLES     = 1,
                ND_SPECIES_LA    = 2,
                ND_SPECIES_GAMMA = 3,
                ND_NAME_COEF     = 4
        };

        cxxNameDouble();
        cxxNameDouble(struct elt_list *);
        cxxNameDouble(struct conc *);
        cxxNameDouble(struct master_activity *ma, int count, ND_TYPE);
        cxxNameDouble(struct name_coef *nc, int count);
	cxxNameDouble(const cxxNameDouble &old, double factor);
        ~cxxNameDouble();

        struct elt_list *elt_list();

        struct master_activity *master_activity()const;

        struct conc *conc()const;

        struct name_coef *name_coef()const;

        void dump_xml(std::ostream& s_oss, unsigned int indent)const;

        void dump_raw(std::ostream& s_oss, unsigned int indent)const;

        CParser::STATUS_TYPE read_raw(CParser& parser, std::istream::pos_type& pos);

        void add_extensive(const cxxNameDouble &old, double factor);
	void add_intensive(const cxxNameDouble &addee, double fthis, double f2);
	void add_log_activities(const cxxNameDouble &addee, double fthis, double f2);
	void add(char * key, double total);

        void insert(char *str, double d) {
		(*this)[str] = d;
	}

	void mpi_pack(std::vector<int>& ints, std::vector<double>& doubles);
	void mpi_pack(int *ints, int *ii, double *doubles, int *dd);

	void mpi_unpack(int *ints, int *ii, double *doubles, int *dd);

        enum ND_TYPE type;
	
protected:

        //std::map <char *, double, CHARSTAR_LESS> totals; 


public:
        //static std::map<int, cxxNameDouble>& map;

};

#endif // !defined(NAMEDOUBLE_H_INCLUDED)
