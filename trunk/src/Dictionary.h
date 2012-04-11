#if !defined(DICTIONARY_H_INCLUDED)
#define DICTIONARY_H_INCLUDED

#include <cassert>				// assert
#include <map>					// std::map
#include <string>				// std::string
#include <list>					// std::list
#include <vector>				// std::vector

class cxxDictionary
{

  public:
	cxxDictionary();

	~cxxDictionary();

	void add_phreeqc();

	int size()
	{
		return (int) stringkey.size();
	}

	int putString(std::string str);
	int putString(char *str);

	int string2int(std::string str);

	int string2int(char *str);

	std::string * int2string(int i);
	std::string & int2stdstring(int i);

	char *int2char(int i);

  protected:
	std::map < std::string, int >stringkey;
	std::map < int, std::string > intkey;

  public:


};

#endif // !defined(DICTIONARY_H_INCLUDED)
