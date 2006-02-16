#include "Conc.h"
#include "Solution.h"
#include "Utilities.h"
#include <cassert>

CConc::CConc(void)
: description("")
, moles(0.0)
, input_conc(0.0)
, units("")
, equation_name("")
, phase_si(0.0)
, n_pe(-1)
, as("")
, gfw(0.0)
	//, skip(0);
	//, phase(NULL)
{
}

CConc::~CConc(void)
{
}

#ifdef SKIP
CConc::STATUS_TYPE CConc::read(CParser& parser, CSolution& solution)
{
	// std::string& str = parser.line(); 
	std::string str = parser.line();

	// defaults set in ctor

	// Remove space between "kg" and "solution" or "water" in units
	Utilities::replace("Kg", "kg", str);
	Utilities::replace("KG", "kg", str);
	while (Utilities::replace("kg ", "kg", str));

	std::istream::pos_type ptr = 0;

	//
	// Read master species list for mass balance equation
	//
	std::string token;
	std::string token1;
	int count_redox_states = 0;
	CParser::TOKEN_TYPE j;
	while ( ((j = parser.copy_token(token, ptr)) == CParser::TT_UPPER ) ||
		( token[0] == '[' ) ||
		( Utilities::strcmp_nocase_arg1(token.c_str(), "ph") == 0 ) ||
		( Utilities::strcmp_nocase_arg1(token.c_str(), "pe") == 0 ) )
	{
		++count_redox_states;
		Utilities::replace("(+", "(", token);
		if (count_redox_states > 1) token1 += " ";
		token1 += token;
	}
	if (count_redox_states == 0) {
		parser.incr_input_error();
		parser.error_msg("No element or master species given for concentration input.", CParser::OT_CONTINUE);
		return CConc::ERROR;
	}
	description = token1;

	// Determine if reading alkalinity, allow equivalents for units
	Utilities::str_tolower(token1);
	bool alk = false;
	if (token1.find("alk") == 0) {
		alk = true;
	}

	// Read concentration
	if (!(std::istringstream(token) >> this->input_conc)) {
		std::ostringstream err;
		err << "Concentration data error for " << token1 << " in solution input.";
		parser.error_msg(err, CParser::OT_CONTINUE);
		return CConc::ERROR;
	}
	if ( (j = parser.copy_token(token, ptr)) == CParser::TT_EMPTY) return CConc::OK;

	// Read optional data
	token1 = token;
	
	// Check for units info
	if (parser.check_units(token1, alk, false, solution.get_units(), false) == CParser::OK) {
		if (parser.check_units(token1, alk, false, solution.get_units(), true) == CParser::OK) {
			this->units = token1;
			if ( (j = parser.copy_token(token, ptr)) == CParser::TT_EMPTY) return CConc::OK;
		} else {
			return CConc::ERROR;
		}
	}

	// Check for "as" followed by formula to be used for gfw
	token1 = token;
	Utilities::str_tolower(token1);
	if (token1.compare("as") == 0)
	{
		parser.copy_token(token, ptr);
		this->as = token;
		if ( (j = parser.copy_token(token, ptr)) == CParser::TT_EMPTY) return CConc::OK;
	}
	// Check for "gfw" followed by gram formula weight
	else if (token1.compare("gfw") == 0)
	{
		if (parser.copy_token(token, ptr) != CParser::TT_DIGIT) {
			parser.error_msg("Expecting gram formula weight.", CParser::OT_CONTINUE);
			return CConc::ERROR;
		} else {
			parser.get_iss() >> this->gfw;
			if ( (j = parser.copy_token(token, ptr)) == CParser::TT_EMPTY) return CConc::OK;
		}
	}

	// Check for redox couple for pe
	if  ( Utilities::strcmp_nocase_arg1(token.c_str(), "pe") == 0 ) {
		this->n_pe = CPe_Data::store(solution.pe, token);
		if ( (j = parser.copy_token(token, ptr)) == CParser::TT_EMPTY) return CConc::OK;
	} else if (token.find("/") != std::string::npos) {
		if (parser.parse_couple(token) == CParser::OK) {
			this->n_pe = CPe_Data::store(solution.pe, token);
			if ( (j = parser.copy_token(token, ptr)) == CParser::TT_EMPTY) return CConc::OK;
		} else {
			return CConc::ERROR;
		}
	}

	// Must have phase
	this->equation_name = token;
	if ( (j = parser.copy_token(token, ptr)) == CParser::TT_EMPTY) return CConc::OK;

	// Check for saturation index
	if (!(std::istringstream(token) >> this->phase_si))
	{
		parser.error_msg("Expected saturation index.", CParser::OT_CONTINUE);
		return CConc::ERROR;
	}
	return CConc::OK;
}
#endif

void CConc::dump_xml(const CSolution& solution, std::ostream& os, unsigned int indent)const
{
	unsigned int i;
	for(i = 0; i < indent; ++i) os << Utilities::INDENT;
	os << "<conc>\n";

	for(i = 0; i < indent + 1; ++i) os << Utilities::INDENT;
	os << "<element_list>" << this->description << "</element_list>\n";

	for(i = 0; i < indent + 1; ++i) os << Utilities::INDENT;
	os << "<concentration>" << this->input_conc << "</concentration>\n";

	if (!this->units.empty()) {
		for(i = 0; i < indent + 1; ++i) os << Utilities::INDENT;
		os << "<units>" << this->units << "</units>\n";
	}

	if ( !this->as.empty() ) {
		for(i = 0; i < indent + 1; ++i) os << Utilities::INDENT;
		os << "<as>" << this->as << "</as>\n";
	}
	else if (this->gfw > 0.0) {
		for(i = 0; i < indent + 1; ++i) os << Utilities::INDENT;
		os << "<gfw>" << this->gfw << "</gfw>\n";
	}
	////if (this->n_pe > 0) {
		solution.pe[this->n_pe].dump_xml(os, indent + 1);
	////}

	if (!this->equation_name.empty()) {
		if (Utilities::strcmp_nocase_arg1(this->equation_name.c_str(), "charge") == 0)
		{
			for(i = 0; i < indent + 1; ++i) os << Utilities::INDENT;
			os << "<charge/>\n";
		}
		else
		{
			for(i = 0; i < indent + 1; ++i) os << Utilities::INDENT;
			os << "<phase_name>" << this->equation_name << "</phase_name>\n";

			for(i = 0; i < indent + 1; ++i) os << Utilities::INDENT;
			os << "<saturation_index>" << this->phase_si << "</saturation_index>\n";
		}
	}

	for(i = 0; i < indent; ++i) os << Utilities::INDENT;
	os << "</conc>\n";
}
