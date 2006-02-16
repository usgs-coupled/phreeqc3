#if !defined(TESTCISOTOPE_H_INCLUDED)
#define TESTCISOTOPE_H_INCLUDED

#include "Isotope.h"
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>


class TestCIsotope :
	public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestCIsotope );
	CPPUNIT_TEST( test_read_1 );
	CPPUNIT_TEST( test_read_2 );
	CPPUNIT_TEST( test_read_3 );
	CPPUNIT_TEST( test_read_4 );
	CPPUNIT_TEST( test_read_5 );
	CPPUNIT_TEST( test_read_6 );
	CPPUNIT_TEST( test_read_7 );
	CPPUNIT_TEST( test_read_8 );
	CPPUNIT_TEST_SUITE_END();

public:
	TestCIsotope(void);
	~TestCIsotope(void);

public:
	// read
	void test_read_1();
	void test_read_2();
	void test_read_3();
	void test_read_4();
	void test_read_5();
	void test_read_6();
	void test_read_7();
	void test_read_8();
};

#endif // TESTCISOTOPE_H_INCLUDED
