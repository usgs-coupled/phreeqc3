.SUFFIXES : .o .cxx 

.cxx.o :
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

CXX      = g++
CPPFLAGS = -I/z/srv2rcolkr/home/charlton/cppunit-robocomp/include
CXXFLAGS = -Wall
LDFLAGS  = -lcppunit -L/z/srv2rcolkr/home/charlton/cppunit-robocomp/lib -static


COMMON_OBJS =	Conc.o\
		Isotope.o\
		NumKeyword.o\
		Parser.o\
		Pe_Data.o\
		Solution.o\
		Utilities.o

TEST_OBJS = 	TestCIsotope.o\
		TestCSolution.o\
		TestCParser.o\
		test.o

all: phreeqcxx test


phreeqcxx : ${COMMON_OBJS} main.o
	${CXX} -o $@ ${COMMON_OBJS} main.o

test : ${COMMON_OBJS} ${TEST_OBJS}
	${CXX} -o $@ ${COMMON_OBJS} ${TEST_OBJS} ${LDFLAGS}

clean :
	rm -f ${COMMON_OBJS} main.o ${TEST_OBJS}
