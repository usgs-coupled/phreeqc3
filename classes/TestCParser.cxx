#include "TestCParser.h"
#include "Parser.h"

TestCParser::TestCParser(void)
{
}

TestCParser::~TestCParser(void)
{
}

void TestCParser::testEmptyInput()
{
	std::ostringstream oss_out;
	std::ostringstream oss_err;
	std::istringstream iss_in("");

	CParser parser(iss_in, oss_out, oss_err);

	CPPUNIT_ASSERT(parser.check_line("", false, true, true, true) == CParser::LT_EOF);
	CPPUNIT_ASSERT(parser.check_line("", false, true, true, true) == CParser::LT_EOF);
}

void TestCParser::testCommentOnlyInput()
{
	std::ostringstream oss_out;
	std::ostringstream oss_err;
	std::istringstream iss_in("#this is a comment");

	CParser parser(iss_in, oss_out, oss_err);

	CPPUNIT_ASSERT(parser.check_line("", false, true, true, true) == CParser::LT_EOF);
	CPPUNIT_ASSERT(parser.check_line("", false, true, true, true) == CParser::LT_EOF);
}

void TestCParser::testLineContinuation()
{
	std::ostringstream oss_out;
	std::ostringstream oss_err;
	std::istringstream iss_in("Line \\\n1");

	CParser parser(iss_in, oss_out, oss_err);

	CPPUNIT_ASSERT(parser.get_line() == CParser::LT_OK);
	CPPUNIT_ASSERT(parser.line().compare("Line 1") == 0);

	CPPUNIT_ASSERT(parser.get_line() == CParser::LT_EOF);
	CPPUNIT_ASSERT(parser.line().compare("") == 0);
}

void TestCParser::testMultipleLineContinuation()
{
	std::ostringstream oss_out;
	std::ostringstream oss_err;
	std::istringstream iss_in("Line \\\n\\\n\\\n\\\n1");

	CParser parser(iss_in, oss_out, oss_err);

	CPPUNIT_ASSERT(parser.get_line() == CParser::LT_OK);
	CPPUNIT_ASSERT(parser.line().compare("Line 1") == 0);

	CPPUNIT_ASSERT(parser.get_line() == CParser::LT_EOF);
	CPPUNIT_ASSERT(parser.line().compare("") == 0);
}

void TestCParser::testLogicalLineSeparator()
{
	std::ostringstream oss_out;
	std::ostringstream oss_err;
	std::istringstream iss_in("Line 1;Line 2");

	CParser parser(iss_in, oss_out, oss_err);

	CPPUNIT_ASSERT(parser.get_line() == CParser::LT_OK);
	CPPUNIT_ASSERT(parser.line().compare("Line 1") == 0);

	CPPUNIT_ASSERT(parser.get_line() == CParser::LT_OK);
	CPPUNIT_ASSERT(parser.line().compare("Line 2") == 0);

	CPPUNIT_ASSERT(parser.get_line() == CParser::LT_EOF);
	CPPUNIT_ASSERT(parser.line().compare("") == 0);
}

void TestCParser::test_check_line_1()
{
	std::ostringstream oss_out;
	std::ostringstream oss_err;
	std::istringstream iss_in("SOLUTION");

	CParser parser(iss_in, oss_out, oss_err);

	CPPUNIT_ASSERT(parser.check_line("", false, true, true, true) == CParser::LT_KEYWORD);
	CPPUNIT_ASSERT(parser.check_line("", false, true, true, true) == CParser::LT_EOF);
	CPPUNIT_ASSERT(oss_out.str().compare("\tSOLUTION\n") == 0);
}

void TestCParser::test_check_line_2()
{
	std::ostringstream oss_out;
	std::ostringstream oss_err;
	std::istringstream iss_in("NOT_A_KEYWORD");

	CParser parser(iss_in, oss_out, oss_err);

	CPPUNIT_ASSERT(parser.check_line("", false, true, true, true) == CParser::LT_OK);
	CPPUNIT_ASSERT(parser.check_line("", false, true, true, true) == CParser::LT_EOF);
}

void TestCParser::test_get_elt_1()
{
	std::istringstream iss_in;
	std::ostringstream oss_out;
	std::ostringstream oss_err;

	std::string token("");
    CParser parser(iss_in, oss_out, oss_err);
	std::string::iterator begin = token.begin();
	std::string element;

	CPPUNIT_ASSERT(parser.get_elt(begin, token.end(), element) == CParser::ERROR);
	CPPUNIT_ASSERT(element.empty());
}

void TestCParser::test_get_elt_2()
{
	std::istringstream iss_in;
	std::ostringstream oss_out;
	std::ostringstream oss_err;

	std::string token("Ca");
    CParser parser(iss_in, oss_out, oss_err);
	std::string::iterator begin = token.begin();
	std::string element;

	CPPUNIT_ASSERT(parser.get_elt(begin, token.end(), element) == CParser::OK);
	CPPUNIT_ASSERT(element.compare("Ca") == 0);

	CPPUNIT_ASSERT(parser.get_elt(begin, token.end(), element) == CParser::ERROR);
}

void TestCParser::test_get_elt_3()
{
	std::istringstream iss_in;
	std::ostringstream oss_out;
	std::ostringstream oss_err;

	std::string token("CaCO3");
    CParser parser(iss_in, oss_out, oss_err);
	std::string::iterator begin = token.begin();
	std::string element;

	CPPUNIT_ASSERT(parser.get_elt(begin, token.end(), element) == CParser::OK);
	CPPUNIT_ASSERT(element.compare("Ca") == 0);

	CPPUNIT_ASSERT(parser.get_elt(begin, token.end(), element) == CParser::OK);
	CPPUNIT_ASSERT(element.compare("C") == 0);

	CPPUNIT_ASSERT(parser.get_elt(begin, token.end(), element) == CParser::OK);
	CPPUNIT_ASSERT(element.compare("O") == 0);
}

void TestCParser::test_get_elt_4()
{
	std::istringstream iss_in;
	std::ostringstream oss_out;
	std::ostringstream oss_err;

	std::string token("[18O]");
    CParser parser(iss_in, oss_out, oss_err);
	std::string::iterator begin = token.begin();
	std::string element;

	CPPUNIT_ASSERT(parser.get_elt(begin, token.end(), element) == CParser::OK);
	CPPUNIT_ASSERT(element.compare("[18O]") == 0);

	CPPUNIT_ASSERT(parser.get_elt(begin, token.end(), element) == CParser::ERROR);
}

void TestCParser::test_get_elt_5()
{
	std::istringstream iss_in;
	std::ostringstream oss_out;
	std::ostringstream oss_err;

	std::string token("[18O");
    CParser parser(iss_in, oss_out, oss_err);
	std::string::iterator begin = token.begin();
	std::string element;

	CPPUNIT_ASSERT(parser.get_elt(begin, token.end(), element) == CParser::ERROR);
	CPPUNIT_ASSERT(oss_err.str().compare("ERROR: No ending bracket (]) for element name\n") == 0);
}
