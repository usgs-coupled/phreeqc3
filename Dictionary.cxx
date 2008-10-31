// StorageBin.cxx: implementation of the cxxStorageBin class.
//
//////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning(disable : 4786)	// disable truncation warning (Only used by debugger)
#endif

#include "Dictionary.h"			// define first
#define EXTERNAL extern
#include "global.h"
#include "phqalloc.h"
#include "phrqproto.h"
#include "output.h"
#include <cassert>				// assert
#include <algorithm>			// std::sort

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
cxxDictionary::cxxDictionary()
{
	// default constructor for cxxStorageBin 
}

cxxDictionary::~cxxDictionary()
{
}

void
cxxDictionary::add_phreeqc()
{
	HashTable *Table = strings_hash_table;
	int i, j;
	Segment *seg;
	Element *p, *q;

	if (Table != NULL)
	{
		for (i = 0; i < Table->SegmentCount; i++)
		{
			/* test probably unnecessary    */
			if ((seg = Table->Directory[i]) != NULL)
			{
				for (j = 0; j < SegmentSize; j++)
				{
					p = seg[j];
					while (p != NULL)
					{
						q = p->Next;
						//p->Data = (char*) free_check_null((void*)p->Data);
						this->putString((char *) (p->Data));
						p = q;
					}
				}
			}
		}
	}
}

int
cxxDictionary::putString(std::string str)
{
	int n;
	std::map < std::string, int >::iterator it = stringkey.find(str);
	if (it != stringkey.end())
	{
		n = it->second;
	}
	else
	{
		n = (int) stringkey.size();
		stringkey[str] = n;
		intkey[n] = str;
	}
	return (n);
}

int
cxxDictionary::putString(char *str)
{
	std::string str1(str);
	return (putString(str1));
}

int
cxxDictionary::string2int(std::string str)
{
	int n;
	std::map < std::string, int >::iterator it = stringkey.find(str);
	if (it != stringkey.end())
	{
		n = it->second;
	}
	else
	{
		n = -1;
	}
	return (n);
}

int
cxxDictionary::string2int(char *str)
{
	if (str == NULL)
		return (-1);
	std::string str1(str);
	return (string2int(str1));
}


std::string * cxxDictionary::int2string(int i)
{
	std::map < int,
		std::string >::iterator
		it = intkey.find(i);
	if (it != intkey.end())
	{
		return (&it->second);
	}
	else
	{
		return (NULL);
	}
}

char *
cxxDictionary::int2char(int i)
{
	std::map < int, std::string >::iterator it = intkey.find(i);
	if (it != intkey.end())
	{
		return (string_hsave(it->second.c_str()));
	}
	else
	{
		return (NULL);
	}
}
