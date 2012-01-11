#if !defined(UTILITIES_H_INCLUDED)
#define UTILITIES_H_INCLUDED
#include "Phreeqc.h"
#include <string>
#include <sstream>				// std::istringstream std::ostringstream
#include <ostream>				// std::ostream
#include <istream>				// std::istream
#include <map>					// std::map
#include "Parser.h"

namespace Utilities
{

	const char INDENT[] = "  ";
#ifdef SKIP
	enum STATUS_TYPE
	{
		ST_OK = 0,
		ST_ERROR = 1
	};
#endif
	//STATUS_TYPE parse_couple(std::string & token);

	int strcmp_nocase(const char *str1, const char *str2);

	int strcmp_nocase_arg1(const char *str1, const char *str2);

	void str_tolower(std::string & str);

	bool replace(const char *str1, const char *str2, std::string & str);

	void squeeze_white(std::string & s_l);

	LDBLE get_nan(void);

	//void error_msg(const std::string&, const int stopflag);

	// operations on maps of entities (Solution, Exchange, ...)
	template < typename T >
	void Rxn_dump_raw(const T & b, std::ostream & s_oss, unsigned int indent)
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

	template < typename T >
	T * Rxn_find(std::map < int, T > &b, int i)
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

	template < typename T >
	T * Rxn_copy(std::map < int, T > &b, int i, int j)
	{
		typename std::map < int, T >::iterator it;
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

	template < typename T >
	void Rxn_copies(std::map < int, T > &b, int n_user, int n_user_end)
	{
		if (n_user_end <= n_user) return;
		typename std::map < int, T >::iterator it;
		it = b.find(n_user);
		if (it != b.end())
		{
			for (int j = n_user + 1; j <= n_user_end; j++)
			{
				b[j] = it->second;
				it = b.find(j);
				it->second.Set_n_user(j);
				it->second.Set_n_user_end(j);
			}
		}
	}
	template < typename T >
	int Rxn_read_raw(std::map < int, T > &m, Phreeqc * phreeqc_cookie)
	{
		typename std::map < int, T >::iterator it;
		assert(!phreeqc_cookie->reading_database());

		T entity(phreeqc_cookie->Get_phrq_io());

		CParser parser(phreeqc_cookie->Get_phrq_io());
		//if (phreeqc_cookie->pr.echo_input == FALSE) parser.set_echo_file(CParser::EO_NONE);
		entity.read_raw(parser);

		// Store
		if (entity.Get_base_error_count() == 0)
		{
			m[entity.Get_n_user()] = entity;
		}

		// Make copies if necessary
		Utilities::Rxn_copies(m, entity.Get_n_user(), entity.Get_n_user_end());
		return phreeqc_cookie->cleanup_after_parser(parser);
	}

	template < typename T >
	int Rxn_read_modify(std::map < int, T > &m, Phreeqc * phreeqc_cookie)
	{
		typename std::map < int, T >::iterator it;
		
		CParser parser(phreeqc_cookie->Get_phrq_io());

		std::string key_name;
		std::string::iterator b = parser.line().begin();
		std::string::iterator e = parser.line().end();
		CParser::copy_token(key_name, b, e);

		cxxNumKeyword nk;
		nk.read_number_description(parser);
		T * entity_ptr = Utilities::Rxn_find(m, nk.Get_n_user());
		if (!entity_ptr)
		{
			std::ostringstream errstr;
			errstr <<  "Could not find " << key_name << " " << nk.Get_n_user() << " to modify.\n";
			phreeqc_cookie->error_msg(errstr.str().c_str(), PHRQ_io::OT_STOP);
		}

		entity_ptr->read_raw(parser, false);

		return phreeqc_cookie->cleanup_after_parser(parser);
	}

	/*
	   template<class T>
	   bool exists (std::map<int, T> b, int i){
	   return (b.find(i) != b.end());}
	 */

}
#endif // UTILITIES_H_INCLUDED
