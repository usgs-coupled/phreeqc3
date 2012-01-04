#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif
#include <stdlib.h>				// ::tolower
#include <ctype.h>				// ::tolower
#include <algorithm>			//std::transform
#include <iostream>				// std::cout std::cerr
#include <string.h>

#include "Utils.h"
#include "Parser.h"

////////////////////////////////////////////////////////////////////////////
int
Utilities::strcmp_nocase_arg1(const char *str1, const char *str2)
////////////////////////////////////////////////////////////////////////////
{
	//
	// Compare two strings disregarding case
	//
	int c1, c2;
	while ((c1 =::tolower(*str1++)) == (c2 = *str2++))
	{
		if (c1 == '\0')
			return (0);
	}
	if (c1 < c2)
		return (-1);
	return (1);
}

////////////////////////////////////////////////////////////////////////////
int
Utilities::strcmp_nocase(const char *str1, const char *str2)
////////////////////////////////////////////////////////////////////////////
{
	//
	// Compare two strings disregarding case
	//
	int c1, c2;
	while ((c1 =::tolower(*str1++)) == (c2 =::tolower(*str2++)))
	{
		if (c1 == '\0')
			return (0);
	}
	if (c1 < c2)
		return (-1);
	return (1);
}

////////////////////////////////////////////////////////////////////////////
void
Utilities::str_tolower(std::string & str)
////////////////////////////////////////////////////////////////////////////
{
	std::transform(str.begin(), str.end(), str.begin(), tolower);
}



////////////////////////////////////////////////////////////////////////////
bool
Utilities::replace(const char *str1, const char *str2, std::string & str)
////////////////////////////////////////////////////////////////////////////
{
	std::string::size_type n = str.find(str1, 0);
	if (n == std::string::npos)
		return false;

	str.replace(n, ::strlen(str1), str2);
	return true;
}

////////////////////////////////////////////////////////////////////////////
void
Utilities::squeeze_white(std::string & s_l)
////////////////////////////////////////////////////////////////////////////
{
	std::string str;
	std::string::iterator beg = s_l.begin();
	std::string::iterator end = s_l.end();
	//CParser::copy_token(str, beg, end);
	std::string::iterator pos;
	for (pos = beg; pos != end; pos++)
	{
		int c = *pos;
		if (!::isspace(c))
		{
			str += c;
		}
	}
	s_l = str;
}

//+NAN double: 7ff8000000000000
//-NAN double: fff8000000000000
/*
double Utilities::get_nan(void)
{
	unsigned long long raw = 0x7ff0000000000000;
	double d = *( double* )&raw;
	return(d);

}
*/
