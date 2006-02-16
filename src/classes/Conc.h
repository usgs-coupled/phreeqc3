#if !defined(CONC_H_INCLUDED)
#define CONC_H_INCLUDED

#include "Parser.h"
#include "Utilities.h"

#include <string>

// forward declarations
class CSolution;  // reqd for read and dump_xml

class CConc
{
public:
	CConc(void);
	~CConc(void);

	enum STATUS_TYPE {
		ERROR  = 0,
		OK     = 1
	};

public:

	STATUS_TYPE read(CParser& parser, CSolution& sol);

	void dump_xml(const CSolution& solution, std::ostream& os, unsigned int indent = 0)const;

	double get_input_conc()const {return this->input_conc;}
	void set_input_conc(double input_conc) {this->input_conc = input_conc;}

	std::string get_equation_name()const {return this->equation_name;}
	void set_equation_name(std::string equation_name) {this->equation_name = equation_name;}

	std::string get_description()const {return this->description;}
	void set_description(std::string description) {this->description = description;}

	std::string get_units()const {return this->units;}
	void set_units(std::string units) {this->units = units;}

	int get_n_pe()const {return this->n_pe;}
	void set_n_pe(int n_pe) {this->n_pe = n_pe;}

	bool operator<(const CConc& conc)const    { return (this->description < conc.description); }

private:
	std::string description;
	// int m_skip;
	double moles;
	double input_conc;
	std::string units;
	std::string equation_name;
	// struct phase *m_phase;
	double phase_si;
	int n_pe;
	std::string as;
	double gfw;
};

#endif // CONC_H_INCLUDED
