#if !defined(NUMKEYWORD_H_INCLUDED)
#define NUMKEYWORD_H_INCLUDED

#include "Parser.h"
#include <ostream>  // std::ostream
#include <string>   // std::string

class CNumKeyword
{
public:
	CNumKeyword();
	virtual ~CNumKeyword();

	int get_n_user()const         { return this->n_user; }
	void set_n_user(int user) { this->n_user = user; }

	int get_n_user_end()const                { return this->n_user_end; }
	void set_n_user_end(int user_end)    { this->n_user_end = user_end; }

	bool operator<(const CNumKeyword& key)const    { return (this->n_user < key.n_user); }

	virtual void dump_xml(std::ostream& os, unsigned int indent = 0)const;

	void read_number_description(CParser& parser);

protected:
	int n_user;
	int n_user_end;
	std::string description;

private:
	void read_number_description(std::istream& is);
};
#endif // !defined(NUMKEYWORD_H_INCLUDED)
