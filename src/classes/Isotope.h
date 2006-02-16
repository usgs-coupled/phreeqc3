#if !defined(ISOTOPE_H_INCLUDED)
#define ISOTOPE_H_INCLUDED

#include "Parser.h"
#include <ostream>   // std::ostream
#include <string>    // std::string

class CIsotope
{
public:
	CIsotope(void);
	~CIsotope(void);

	enum STATUS {
		ERROR = 0,
		OK    = 1
	};

	CIsotope::STATUS read(CParser& parser);

	void dump_xml(std::ostream& os, unsigned int indent)const;

	std::string get_name()const ;

	double get_ratio()const              { return this->ratio; }

	double get_ratio_uncertainty()const  { return this->ratio_uncertainty; }

	bool get_ratio_uncertainty_defined()const  { return this->ratio_uncertainty_defined; }

	bool operator<(const CIsotope& conc)const;

private:
	double       isotope_number;
	std::string  elt_name;
	double       ratio;
	double       ratio_uncertainty;
	bool         ratio_uncertainty_defined;
};
#endif // ISOTOPE_H_INCLUDED
