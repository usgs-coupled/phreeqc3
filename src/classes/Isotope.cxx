#include "Isotope.h"
#include "Utilities.h"
#include <cassert>

CIsotope::CIsotope(void)
: isotope_number(0.0)
, ratio_uncertainty_defined(false)
{
}

CIsotope::~CIsotope(void)
{
}

std::string CIsotope::get_name()const
{
	std::ostringstream oss;
	oss << this->isotope_number << this->elt_name;
	return oss.str();
}
#ifdef SKIP
CIsotope::STATUS CIsotope::read(CParser& parser)
{
	if ( !(parser.get_iss() >> this->isotope_number) ) {
		assert(parser.get_iss().fail());
		parser.incr_input_error();
		parser.error_msg("Expected isotope name to"
			" begin with an isotopic number.", CParser::OT_CONTINUE);
		return ERROR;
	}
	assert(parser.get_iss().good() || parser.get_iss().eof());

	// read and save element name
	std::istringstream::int_type c = parser.get_iss().peek();
	if ( c == std::char_traits<char>::eof() || !(::isupper(c)) ) {
		parser.error_msg("Expecting element name.", CParser::OT_CONTINUE);
		parser.error_msg(parser.line().c_str(), CParser::OT_CONTINUE);
		parser.incr_input_error();
		return ERROR;
	}
	assert(parser.get_iss().good() || parser.get_iss().eof());
	if ( !(parser.get_iss() >> this->elt_name) ) {
		// should never get here
		return ERROR;
	}
	assert(parser.get_iss().good() || parser.get_iss().eof());
	assert(!this->elt_name.empty() && ::isupper(this->elt_name[0]));

	// read and store isotope ratio
	if ( !(parser.get_iss() >> this->ratio) ) {
		assert(parser.get_iss().fail());
		parser.incr_input_error();
		parser.error_msg("Expected numeric value for isotope ratio.", CParser::OT_CONTINUE);
		return ERROR;
	}
	assert(parser.get_iss().good() || parser.get_iss().eof());

	// read and store isotope ratio
	this->ratio_uncertainty_defined = false;
	if ( !(parser.get_iss() >> this->ratio_uncertainty)) {
		if ( !parser.get_iss().eof() ) {
			parser.incr_input_error();
			parser.error_msg("Expected numeric value for uncertainty in isotope ratio.", CParser::OT_CONTINUE);
			return ERROR;
		}
	} else {
		this->ratio_uncertainty_defined = true;
	}
	assert(parser.get_iss().good() || parser.get_iss().eof());
	return OK;
}
#endif
#ifdef SKIP
void CIsotope::dump_xml(std::ostream& os, unsigned int indent)const
{
	unsigned int i;

	for(i = 0; i < indent; ++i) os << Utilities::INDENT;
	os << "<isotope name=\"" << get_name() << "\" value=\"" << this->ratio << "\"";
	if ( this->ratio_uncertainty_defined /* Utilities::isnan(this->ratio_uncertainty) */ ) {
        os << "/>\n";
	}
	else {
        os << ">\n";

		for(i = 0; i < indent + 1; ++i) os << Utilities::INDENT;
		os << "<uncertainity_limit>" << this->ratio_uncertainty << "</uncertainity_limit>\n";

		for(i = 0; i < indent; ++i) os << Utilities::INDENT;
        os << "</isotope>\n";
	}
}
#endif
bool CIsotope::operator<(const CIsotope& isotope)const
{
	int i = Utilities::strcmp_nocase(this->elt_name.c_str(), isotope.elt_name.c_str());
	if (i != 0) return (i < 0);
	return ( this->isotope_number < isotope.isotope_number );
}

