#if !defined(UTILITIES_H_INCLUDED)
#define UTILITIES_H_INCLUDED

#include <string>
#include <sstream>  // std::istringstream std::ostringstream
#include <ostream>  // std::ostream
#include <istream>  // std::istream
#include <map>      // std::map
#include "char_star.h"

namespace Utilities {

        const char INDENT[] = "  ";

        enum STATUS_TYPE {
                OK           = 0,
                ERROR        = 1
        };

        STATUS_TYPE parse_couple(std::string& token);

        int strcmp_nocase(const char *str1, const char *str2);

        int strcmp_nocase_arg1(const char *str1, const char *str2);

        void str_tolower(std::string& str);

        bool replace(const char* str1, const char* str2, std::string& str);

        void squeeze_white(std::string& s_l);

        void error_msg(const std::string&, const int stopflag);

	// operations on maps of entities (Solution, Exchange, ...)
        template<class T> 
                bool exists (std::map<int, T> b, int i){
                return (b.find(i) != b.end());}

	/*
        template<class T> 
                void dump_raw ( std::map<int, T> b, std::ostream& s_oss, unsigned int indent ) 
		{
			std :: map < int, T > :: iterator it;
			for (it = b.begin(); it != b.end(); ++it) {
				it->second.dump_raw(s_oss, indent);
			}
			return;
		}
	*/
}

#endif // UTILITIES_H_INCLUDED
