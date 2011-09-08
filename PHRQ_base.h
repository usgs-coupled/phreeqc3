#ifndef _PHRQBASE_H
#define _PHRQBASE_H

#include <sstream>

class PHRQ_io;
class PHRQ_base
{
public:
	// constructors
	PHRQ_base(void);
	PHRQ_base(PHRQ_io *);
	virtual ~ PHRQ_base();


	// methods
	void output_msg(const std::string &);
	void error_msg(const std::string &);
	void warning_msg(const std::string &);

	void Set_io(PHRQ_io * p_io)
	{
		this->io = p_io;
	}
	PHRQ_io * Get_io(void)
	{
		return this->io;
	}
	void Set_error_count(int i)
	{
		this->error_count = i;
	}
	int Get_error_count(void)
	{
		return this->error_count;
	}
	// data
private:
	PHRQ_io * io;
	int error_count;
};

#endif /* _PHRQBASE_H */
