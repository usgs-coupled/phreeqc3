#if !defined(PE_DATA_H_INCLUDED)
#define PE_DATA_H_INCLUDED

#include <string>
#include <vector>

class CPe_Data
{
public:
	CPe_Data();
	CPe_Data(const std::string& name);

	~CPe_Data();

	void dump_xml(std::ostream& os, unsigned int indent = 0)const;

	std::string get_name()const {return this->name;};
	void set_name(std::string name) {this->name = name;};

	static int store(std::vector<CPe_Data>& vec, const std::string& token);
	static std::vector<CPe_Data> CPe_Data::alloc();

private:
	std::string name;
};

#endif // PE_DATA_H_INCLUDED
