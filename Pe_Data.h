#if !defined(PE_DATA_H_INCLUDED)
#define PE_DATA_H_INCLUDED

#include <string>
#include <vector>

class cxxPe_Data
{
public:
        cxxPe_Data();
        cxxPe_Data(struct pe *pe_ptr);
        cxxPe_Data(const std::string& name);

        ~cxxPe_Data();

        void dump_xml(std::ostream& os, unsigned int indent = 0)const;

        std::string get_name()const {return this->name;};
        void set_name(std::string name) {this->name = name;};

        static int store(std::vector<cxxPe_Data>& vec, const std::string& token);
        static std::vector<cxxPe_Data> cxxPe_Data::alloc();

private:
        std::string name;
};

#endif // PE_DATA_H_INCLUDED
