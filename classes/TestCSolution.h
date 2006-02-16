#if !defined(TESTCSOLUTION_H_INCLUDED)
#define TESTCSOLUTION_H_INCLUDED

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>

class TestCSolution :
	public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( TestCSolution );
	CPPUNIT_TEST( test_read_ex1 );
	CPPUNIT_TEST( test_read_ex7 );
	CPPUNIT_TEST_SUITE_END();

public:
	TestCSolution(void);
	~TestCSolution(void);

public:
	void test_read_ex1();
	void test_read_ex7();

};
#endif // TESTCSOLUTION_H_INCLUDED
