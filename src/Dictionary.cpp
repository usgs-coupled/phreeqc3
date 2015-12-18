#include "Dictionary.h"
Dictionary::Dictionary(void)
{
}

Dictionary::Dictionary(std::string & words_string)
{
	std::istringstream words_stream(words_string);
	std::string str;
	while (words_stream >> str)
	{
		this->Find(str);
	}
}
Dictionary::~Dictionary(void)
{
}

int 
Dictionary::Find(std::string str)
{
	std::map<std::string, int>::iterator it = this->dictionary_map.find(str);
	if (it != this->dictionary_map.end()) 
	{
		return it->second;
	}
	int i = this->MapSize();
	this->dictionary_map[str] = i;
	this->words.push_back(str);
	this->dictionary_oss << str << "\n";
	return i;
}
