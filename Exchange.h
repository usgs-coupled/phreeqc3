#if !defined(EXCHANGE_H_INCLUDED)
#define EXCHANGE_H_INCLUDED

#include "NumKeyword.h"
#define EXTERNAL extern
#include "global.h"
#include <cassert> // assert
#include <map>     // std::map
#include <string>  // std::string
#include <list>    // std::list
#include <vector>  // std::vector

#include "char_star.h"
#include "ExchComp.h"

class cxxExchange : public cxxNumKeyword
{

public:
        cxxExchange();
        cxxExchange(struct exchange *);
        ~cxxExchange();

        struct exchange *cxxExchange2exchange();

        struct exch_comp *cxxExchComp2exch_comp();

        void dump_xml(std::ostream& os, unsigned int indent = 0)const;

        void dump_raw(std::ostream& s_oss, unsigned int indent)const;

        void read_raw(CParser& parser);

        bool get_related_phases(void);

        bool get_related_rate(void);

protected:
        std::list<cxxExchComp> exchComps;
        bool pitzer_exchange_gammas;

public:
        //static std::map<int, cxxExchange>& map;

};

#endif // !defined(EXCHANGE_H_INCLUDED)
