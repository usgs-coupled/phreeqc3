#if !defined(SERIALIZER_H_INCLUDED)
#define SERIALIZER_H_INCLUDED
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include "Dictionary.h"
class Phreeqc;
class Serializer
{
	Serializer(void);
	~Serializer(void);

public:
	bool Serialize(Phreeqc &phreeqc_ptr, int start, int end, bool include_t, bool include_p);
	bool Deserialize(Phreeqc &phreeqc_ptr, Dictionary &dictionary, std::vector<int> &ints, std::vector<double> &doubles);

protected:
	std::vector<int> ints;
	std::vector<double> doubles;
	std::string words_string;
	Dictionary dictionary;
};
#endif // !defined(SERIALIZER_H_INCLUDED)