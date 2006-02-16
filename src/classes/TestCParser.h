#if !defined(TESTCPARSER_H_INCLUDED)
#define TESTCPARSER_H_INCLUDED

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

class TestCParser :
	public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestCParser );
	CPPUNIT_TEST( testEmptyInput );
	CPPUNIT_TEST( testCommentOnlyInput );
	CPPUNIT_TEST( testLineContinuation );
	CPPUNIT_TEST( testMultipleLineContinuation );
	CPPUNIT_TEST( testLogicalLineSeparator );
	CPPUNIT_TEST( test_check_line_1 );
	CPPUNIT_TEST( test_check_line_2 );
	CPPUNIT_TEST( test_get_elt_1 );
	CPPUNIT_TEST( test_get_elt_2 );
	CPPUNIT_TEST( test_get_elt_3 );
	CPPUNIT_TEST( test_get_elt_4 );
	CPPUNIT_TEST( test_get_elt_5 );
	CPPUNIT_TEST_SUITE_END();

public:
	TestCParser(void);
	~TestCParser(void);

public:
	void testEmptyInput();
	void testCommentOnlyInput();
	void testLineContinuation();
	void testMultipleLineContinuation();
	void testLogicalLineSeparator();

	// check_line
	void test_check_line_1();
	void test_check_line_2();

	// get_elt
	void test_get_elt_1();
	void test_get_elt_2();
	void test_get_elt_3();
	void test_get_elt_4();
	void test_get_elt_5();

};
#endif // TESTCPARSER_H_INCLUDED
