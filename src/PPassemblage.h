#if !defined(PPASSEMBLAGE_H_INCLUDED)
#define PPASSEMBLAGE_H_INCLUDED

#include "NumKeyword.h"
#define EXTERNAL extern
#include "global.h"
#include <cassert> // assert
#include <map>     // std::map
#include <string>  // std::string
#include <list>    // std::list
#include <vector>  // std::vector

#include "char_star.h"
#include "PPassemblageComp.h"

class cxxPPassemblage : public cxxNumKeyword
{

public:
        cxxPPassemblage();
        cxxPPassemblage(struct pp_assemblage *);
        ~cxxPPassemblage();

        struct pp_assemblage *cxxPPassemblage2pp_assemblage();

        struct pure_phase *cxxPPassemblageComp2pure_phase();

        void dump_xml(std::ostream& os, unsigned int indent = 0)const;

        void dump_raw(std::ostream& s_oss, unsigned int indent)const;

        void read_raw(CParser& parser);

protected:
        std::list<cxxPPassemblageComp> ppAssemblageComps;
        cxxNameDouble eltList;

public:
        //static std::map<int, cxxPPassemblage>& map;

};

#endif // !defined(PPASSEMBLAGE_H_INCLUDED)
