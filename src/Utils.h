#if !defined(UTILITIES_H_INCLUDED)
#define UTILITIES_H_INCLUDED

#include <string>
#include <sstream>				// std::istringstream std::ostringstream
#include <ostream>				// std::ostream
#include <istream>				// std::istream
#include <map>					// std::map

namespace Utilities
{

	const char INDENT[] = "  ";

	enum STATUS_TYPE
	{
		OK = 0,
		ERROR = 1
	};

	STATUS_TYPE parse_couple(std::string & token);

	int strcmp_nocase(const char *str1, const char *str2);

	int strcmp_nocase_arg1(const char *str1, const char *str2);

	void str_tolower(std::string & str);

	bool replace(const char *str1, const char *str2, std::string & str);

	void squeeze_white(std::string & s_l);

	double get_nan(void);

	//void error_msg(const std::string&, const int stopflag);

	// operations on maps of entities (Solution, Exchange, ...)
	template < typename T >
	void Reactant_dump_map_raw(const T & b, std::ostream & s_oss, unsigned int indent)
	{
		//std :: map < int, T > :: const_iterator it;
		typename T::const_iterator it;
		for (it = b.begin(); it != b.end(); ++it)
		{
			it->second.dump_raw(s_oss, indent);
		}
		return;
	}
	//template < typename T > bool exists(const T & b, int i)
	//{
	//	return (b.find(i) != b.end());
	//}

	template < typename T > T * Reactant_find(std::map < int, T > &b, int i)
	{
		if (b.find(i) != b.end())
		{
			return (&(b.find(i)->second));
		}
		else
		{
			return (NULL);
		}
	}

	template < typename T > T * Reactant_copy(std::map < int, T > &b, int i, int j)
	{
		std::map < int, T >::iterator it;
		it = b.find(i);
		if (it != b.end())
		{
			b[j] = it->second;
			it = b.find(j);
			it->second.Set_n_user(j);
			it->second.Set_n_user_end(j);
			return &(it->second);
		}
		else
		{
			return (NULL);
		}
	}

	/*
	   template<class T> 
	   bool exists (std::map<int, T> b, int i){
	   return (b.find(i) != b.end());}
	 */

}

#endif // UTILITIES_H_INCLUDED
