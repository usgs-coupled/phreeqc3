#include "Pe_Data.h"
#include "Utilities.h"
#include <ostream>     // std::ostream

CPe_Data::CPe_Data()
: name("")
{
}

CPe_Data::CPe_Data(const std::string& name)
: name(name)
{
}

CPe_Data::~CPe_Data()
{
}

int CPe_Data::store(std::vector<CPe_Data>& vec_pe_data, const std::string& token)
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

std::vector<CPe_Data> CPe_Data::alloc()
{
	std::vector<CPe_Data> vec;
	vec.push_back(CPe_Data("pe"));
	// TODO: see pe_data_alloc
	return vec;
}

void CPe_Data::dump_xml(std::ostream& os, unsigned int indent)const
{
	unsigned int i;
	for(i = 0; i < indent; ++i) os << Utilities::INDENT;
	os << "<redox>" << this->name << "</redox>\n";
}

