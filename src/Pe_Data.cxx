#include "Pe_Data.h"
#include "Utilities.h"
#include <ostream>     // std::ostream

cxxPe_Data::cxxPe_Data()
: name("")
{
}

cxxPe_Data::cxxPe_Data(const std::string& name)
: name(name)
{
}

cxxPe_Data::~cxxPe_Data()
{
}

int cxxPe_Data::store(std::vector<cxxPe_Data>& vec_pe_data, const std::string& token)
{
        unsigned int i = 0;
        unsigned int size = vec_pe_data.size();
        for (; i < size; ++i) {
                if (vec_pe_data[i].name.compare(token) == 0)
                        return i;
        }
        vec_pe_data.push_back(token);
        return i;
}

std::vector<cxxPe_Data> cxxPe_Data::alloc()
{
        std::vector<cxxPe_Data> vec;
        vec.push_back(cxxPe_Data("pe"));
        // TODO: see pe_data_alloc
        return vec;
}

void cxxPe_Data::dump_xml(std::ostream& os, unsigned int indent)const
{
        unsigned int i;
        for(i = 0; i < indent; ++i) os << Utilities::INDENT;
        os << "<redox>" << this->name << "</redox>\n";
}

