#include "TestCIsotope.h"
#include "Parser.h"
#include "utilities.h"

TestCIsotope::TestCIsotope()
{
}

TestCIsotope::~TestCIsotope()
{
}

void TestCIsotope::test_read_1()
{
	CIsotope iso;
	std::ostringstream oss_out;
	std::ostringstream oss_err;
	std::istringstream::pos_type next_pos;
	std::vector<std::string> opt_list;

	std::istringstream iss_in("-isotope");

	CParser parser(iss_in, oss_out, oss_err);
	opt_list.push_back("isotope");

	CPPUNIT_ASSERT(parser.get_option(opt_list, next_pos) == 0);
	CPPUNIT_ASSERT(iso.read(parser) == CIsotope::ERROR);
	CPPUNIT_ASSERT(oss_err.str().compare("ERROR: Expected isotope name to begin with an isotopic number.\n") == 0);
}

void TestCIsotope::test_read_2()
{
	CIsotope iso;
	std::ostringstream oss_out;
	std::ostringstream oss_err;
	std::istringstream::pos_type next_pos;
	std::vector<std::string> opt_list;

	std::istringstream iss_in("-isotope C");

	CParser parser(iss_in, oss_out, oss_err);
	opt_list.push_back("isotope");

	CPPUNIT_ASSERT(parser.get_option(opt_list, next_pos) == 0);
	CPPUNIT_ASSERT(iso.read(parser) == CIsotope::ERROR);
	CPPUNIT_ASSERT(oss_err.str().compare("ERROR: Expected isotope name to begin with an isotopic number.\n") == 0);
}

void TestCIsotope::test_read_3()
{
	CIsotope iso;
	std::ostringstream oss_out;
	std::ostringstream oss_err;
	std::istringstream::pos_type next_pos;
	std::vector<std::string> opt_list;

	std::istringstream iss_in("isotope 13");

	CParser parser(iss_in, oss_out, oss_err);
	opt_list.push_back("isotope");

	CPPUNIT_ASSERT(parser.get_option(opt_list, next_pos) == 0);
	CPPUNIT_ASSERT(iso.read(parser) == CIsotope::ERROR);
	CPPUNIT_ASSERT(oss_err.str().compare("ERROR: Expecting element name.\nERROR: isotope 13\n") == 0);
}

void TestCIsotope::test_read_4()
{
	CIsotope iso;
	std::ostringstream oss_out;
	std::ostringstream oss_err;
	std::istringstream::pos_type next_pos;
	std::vector<std::string> opt_list;

	std::istringstream iss_in("isotope 13C");

	CParser parser(iss_in, oss_out, oss_err);
	opt_list.push_back("isotope");

	CPPUNIT_ASSERT(parser.get_option(opt_list, next_pos) == 0);
	CPPUNIT_ASSERT(iso.read(parser) == CIsotope::ERROR);
	CPPUNIT_ASSERT(oss_err.str().compare("ERROR: Expected numeric value for isotope ratio.\n") == 0);
}

void TestCIsotope::test_read_5()
{
	CIsotope iso;
	std::ostringstream oss_out;
	std::ostringstream oss_err;
	std::istringstream::pos_type next_pos;
	std::vector<std::string> opt_list;

	std::istringstream iss_in("isotope 13C -");

	CParser parser(iss_in, oss_out, oss_err);
	opt_list.push_back("isotope");

	CPPUNIT_ASSERT(parser.get_option(opt_list, next_pos) == 0);
	CPPUNIT_ASSERT(iso.read(parser) == CIsotope::ERROR);
	CPPUNIT_ASSERT(oss_err.str().compare("ERROR: Expected numeric value for isotope ratio.\n") == 0);
}

void TestCIsotope::test_read_6()
{
	CIsotope iso;
	std::ostringstream oss_out;
	std::ostringstream oss_err;
	std::istringstream::pos_type next_pos;
	std::vector<std::string> opt_list;

	std::istringstream iss_in("isotope 13C -12.");

	CParser parser(iss_in, oss_out, oss_err);
	opt_list.push_back("isotope");

	CPPUNIT_ASSERT(parser.get_option(opt_list, next_pos) == 0);
	CPPUNIT_ASSERT(iso.read(parser) == CIsotope::OK);	
	CPPUNIT_ASSERT(oss_err.str().empty());

	CPPUNIT_ASSERT(iso.get_name().compare("13C") == 0);
	CPPUNIT_ASSERT(iso.get_ratio() == -12.0);
	CPPUNIT_ASSERT(!iso.get_ratio_uncertainty_defined());
}

void TestCIsotope::test_read_7()
{
	CIsotope iso;
	std::ostringstream oss_out;
	std::ostringstream oss_err;
	std::istringstream::pos_type next_pos;
	std::vector<std::string> opt_list;

	std::istringstream iss_in("isotope 13C -12. a");

	CParser parser(iss_in, oss_out, oss_err);
	opt_list.push_back("isotope");

	CPPUNIT_ASSERT(parser.get_option(opt_list, next_pos) == 0);
	CPPUNIT_ASSERT(iso.read(parser) == CIsotope::ERROR);
	CPPUNIT_ASSERT(oss_err.str().compare("ERROR: Expected numeric value for uncertainty in isotope ratio.\n") == 0);
}

void TestCIsotope::test_read_8()
{
	CIsotope iso;
	std::ostringstream oss_out;
	std::ostringstream oss_err;
	std::istringstream::pos_type next_pos;
	std::vector<std::string> opt_list;

	std::istringstream iss_in("isotope 13C -12. 1.");

	CParser parser(iss_in, oss_out, oss_err);
	opt_list.push_back("isotope");

	CPPUNIT_ASSERT(parser.get_option(opt_list, next_pos) == 0);
	CPPUNIT_ASSERT(iso.read(parser) == CIsotope::OK);
	CPPUNIT_ASSERT(oss_err.str().empty());

	CPPUNIT_ASSERT(iso.get_name().compare("13C") == 0);
	CPPUNIT_ASSERT(iso.get_ratio() == -12.0);
	CPPUNIT_ASSERT(iso.get_ratio_uncertainty() == 1.0);
	CPPUNIT_ASSERT(iso.get_ratio_uncertainty_defined());
}
