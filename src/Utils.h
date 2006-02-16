#if !defined(UTILITIES_H_INCLUDED)
#define UTILITIES_H_INCLUDED

#include <string>

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

}

#endif // UTILITIES_H_INCLUDED
