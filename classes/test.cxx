#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>

#include "TestCParser.h"
#include "TestCIsotope.h"
#include "TestCSolution.h"

int main(int argc, char **argv)
{
	CppUnit::TextUi::TestRunner runner;

	runner.addTest(TestCParser::suite());
	runner.addTest(TestCIsotope::suite());
	runner.addTest(TestCSolution::suite());

	runner.setOutputter(
		CppUnit::CompilerOutputter::defaultOutputter(&runner.result(), std::cout)
		);

	bool wasSucessful = runner.run("", false);
	return wasSucessful;
}

