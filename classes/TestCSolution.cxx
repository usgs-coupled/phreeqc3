#include "TestCSolution.h"
#include "Solution.h"
#include "Parser.h"
#include "utilities.h"
#include <iostream> // std::cout std::cerr

TestCSolution::TestCSolution()
{
}

TestCSolution::~TestCSolution()
{
}

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

void TestCSolution::test_read_ex1()
{
	CPPUNIT_ASSERT(CSolution::s_map.find(1) == CSolution::s_map.end());
	std::string input =
		"SOLUTION 1  SEAWATER FROM NORDSTROM ET AL. (1979)\n"
		"        units   ppm\n"
		"        pH      8.22\n"
		"        pe      8.451\n"
		"        density 1.023\n"
		"        temp    25.0\n"
		"        redox   O(0)/O(-2)\n"
		"        Ca              412.3\n"
		"        Mg              1291.8\n"
		"        Na              10768.0\n"
		"        K               399.1\n"
		"        Fe              0.002\n"
		"        Mn              0.0002  pe\n"
		"        Si              4.28\n"
		"        Cl              19353.0\n"
		"        Alkalinity      141.682 as HCO3\n"
		"        S(6)            2712.0\n"
		"        N(5)            0.29    gfw   62.0\n"
		"        N(-3)           0.03    as    NH4\n"
		"        U               3.3     ppb   N(5)/N(-3)\n"
		"        O(0)            1.0     O2(g) -0.7\n"
		;

	// setup parser
	std::istringstream iss_in(input);
	std::ostringstream oss_out;
	std::ostringstream oss_err;
	CParser parser(iss_in, oss_out, oss_err);
	for (int simulation = 1; ; ++simulation) {
		if (read_input(parser) == CParser::LT_EOF) break;
	}
	CPPUNIT_ASSERT(parser.get_input_error() == 0);
	CPPUNIT_ASSERT(CSolution::s_map.size() == 1);
	CPPUNIT_ASSERT(CSolution::s_map.find(1) != CSolution::s_map.end());

	CPPUNIT_ASSERT(CSolution::s_map[1].get_units().compare("mg/kgs") == 0);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(8.22, CSolution::s_map[1].get_ph(), 0.005);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(8.451, CSolution::s_map[1].get_solution_pe(), 0.0005);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.023, CSolution::s_map[1].get_density(), 0.0005);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(25.0, CSolution::s_map[1].get_tc(), 0.5);
	CPPUNIT_ASSERT(CSolution::s_map[1].get_redox().compare("O(-2)/O(0)") == 0);

	// empty map for subsequent tests
	CSolution::s_map.erase(CSolution::s_map.begin(), CSolution::s_map.end());
}

void TestCSolution::test_read_ex7()
{
	CPPUNIT_ASSERT(CSolution::s_map.find(1) == CSolution::s_map.end());
	std::string input =
		"SOLUTION 1\n"
		;

	// setup parser
	std::istringstream iss_in(input);
	std::ostringstream oss_out;
	std::ostringstream oss_err;
	CParser parser(iss_in, oss_out, oss_err);
	for (int simulation = 1; ; ++simulation) {
		if (read_input(parser) == CParser::LT_EOF) break;
	}
	CPPUNIT_ASSERT(parser.get_input_error() == 0);
	CPPUNIT_ASSERT(CSolution::s_map.size() == 1);
	CPPUNIT_ASSERT(CSolution::s_map.find(1) != CSolution::s_map.end());

	CPPUNIT_ASSERT(CSolution::s_map[1].get_units().compare("mMol/kgw") == 0);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(7.0, CSolution::s_map[1].get_ph(), 0.05);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, CSolution::s_map[1].get_solution_pe(), 0.05);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, CSolution::s_map[1].get_density(), 0.05);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(25.0, CSolution::s_map[1].get_tc(), 0.5);
	CPPUNIT_ASSERT(CSolution::s_map[1].get_redox().compare("pe") == 0);

	// empty map for subsequent tests
	CSolution::s_map.erase(CSolution::s_map.begin(), CSolution::s_map.end());
}
