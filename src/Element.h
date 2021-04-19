#if !defined(ELEMENT_H_INCLUDED)
#define ELEMENT_H_INCLUDED
#include <vector>
#include <string>
/*----------------------------------------------------------------------
 *   Elements
 *---------------------------------------------------------------------- */
class element
{
public:
	~element() {};
	element()
	{
		// element name
		//name.clear();
		/*    int in; */
		master = NULL;
		primary = NULL;
		gfw = 0;
	}
	std::string name;
	class master* master;
	class master* primary;
	double gfw;
};
/*----------------------------------------------------------------------
 *   Element List
 *---------------------------------------------------------------------- */
class elt_list
{
public:
	~elt_list() {};
	elt_list()
	{	/* list of name and number of elements in an equation */
		elt = NULL;	/* pointer to element structure */
		coef = 0.0;			/* number of element e's in eqn */
	}
	class element* elt;
	double coef;
};
#endif // !defined(ELEMENT_H_INCLUDED)