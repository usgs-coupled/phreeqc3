TESTS = ex1.sh ex2.sh ex2b.sh ex3.sh ex4.sh ex5.sh  ex15.sh ex15a.sh ex15b.sh

EXTRA_DIST = $(TESTS)

TESTS_ENVIRONMENT = top_srcdir=$(top_srcdir) PATH=.$(PATH_SEPARATOR)../src$(PATH_SEPARATOR)$$PATH \
                    PHREEQC=`echo phreeqc|sed '$(transform)'` \
                    DATADIR=$(top_srcdir)/examples \
                    DBDIR=$(top_srcdir)/database \
                    $(SHELL)

EX1=ex1.out ex1.log
EX2=ex2.out ex2.log ex2.sel ex2b.out ex2b.log
EX3=ex3.out ex3.log
EX4=ex4.out ex4.log
EX5=ex5.out ex5.log ex5.sel
EX15=ex15.out ex15.log ex15.sel ex15a.out ex15a.log ex15b.out ex15b.log ex15b.sel  

DISTCLEANFILES=$(EX1) $(EX2) $(EX3) $(EX4) $(EX5) $(EX15) phreeqc.log
