#if !defined(NAME_COEF_H_INCLUDED)
#define NAME_COEF_H_INCLUDED
#include <vector>
class name_coef
{
public:
	~name_coef() {};
	name_coef()
	{
		//name.clear();
		coef = 0;
	}
	std::string name;
	double coef;
};
#endif // !defined(NAME_COEF_H_INCLUDED)