#include <iostream> // std::cout std::cerr
#include <string>   // std::string
#include <vector>   // std::vector
#include <map>      // std::map

#include "Parser.h"
#include "Solution.h"

CParser::LINE_TYPE read_input(CParser& parser)
{
	CParser::LINE_TYPE i;
	while ((i = parser.check_line("Subroutine Read", false, true, true, true)) != CParser::LT_KEYWORD)
	{
		if (i == CParser::LT_EOF) return CParser::LT_EOF;
	}

	for (;;) {
		switch(parser.next_keyword())
		{
		case CParser::KT_END: 
		case CParser::KT_EOF:
		case CParser::KT_NONE:
			goto END_OF_SIMULATION_INPUT;
			break;

		case CParser::KT_SOLUTION:
			CSolution::read(parser);
			break;

		default:
			break;
		}
	}

END_OF_SIMULATION_INPUT:
	return CParser::LT_OK;
}

int main(void)
{
	std::string input =
        "SOLUTION 25 Test solution number 25\n"
        "     temp      25.0                                   \n"
        "     pH        7.0     charge                         \n"
        "     pe        4.5                                    \n"
        "     redox     O(-2)/O(0)                             \n"
        "     units     ppm                                    \n"
        "     density   1.02                                   \n"
        "     Ca        80.                                    \n"
        "     S(6)      96.     as SO4                         \n"
        "     S(-2)     1.      as S                           \n"
        "     N(5) N(3) 14.     as N                           \n"
        "     O(0)      8.0                                    \n"
        "     C         61.0    as HCO3      CO2(g)     -3.5   \n"
        "     Fe        55.     ug/kgs as Fe S(6)/S(-2) Pyrite \n"
        "     -isotope  13C     -12.   1.  # permil PDB        \n"
        "     -isotope  34S     15.    1.5 # permil CDT        \n"
        "     -water    0.5     # kg                           \n"
        ;

	// setup parser
	std::istringstream iss_in(input);
	std::ostringstream oss_out;
	std::ostringstream oss_err;
	CParser parser(iss_in, oss_out, oss_err);
	for (int simulation = 1; ; ++simulation) {
		if (read_input(parser) == CParser::LT_EOF) break;
	}

	// dump solutions as xml string
	std::ostringstream oss_tmp;
	std::map<int, CSolution>::const_iterator it = CSolution::s_map.begin();
	for (; it != CSolution::s_map.end(); ++it)
	{
		(*it).second.dump_xml(oss_tmp);
	}
	oss_tmp << "\n";

	// output xml string
	std::cout << oss_tmp.str();

	return 0;
}
