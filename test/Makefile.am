TESTS = ex1 ex2 ex2b ex3 ex4 ex5 ex6 ex7 ex8 ex9 ex10 ex11 ex12 ex12a ex13a ex13b ex13c ex13ac \
        ex14 ex15 ex15a ex15b ex16 ex17 ex17b ex18 ex19 ex19b ex20a ex20b ex21 ex22

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
EX6=ex6.out ex6.log ex6.sel
EX7=ex7.out ex7.log
EX8=ex8.out ex8.log
EX9=ex9.out ex9.log
EX10=ex10.out ex10.log
EX11=ex11.out ex11.log ex11.sel
EX12=ex12.out ex12.log ex12.sel ex12a.out ex12a.log ex12a.sel
EX13=ex13a.out ex13a.log ex13b.out ex13b.log ex13c.out ex13c.log ex13ac.out ex13ac.log
EX14=ex14.out ex14.log
EX15=ex15.out ex15.log ex15.sel ex15a.out ex15a.log ex15b.out ex15b.log ex15b.sel  
EX16=ex16.out ex16.log ex16.sel
EX17=ex17.out ex17.log ex17.sel ex17b.out ex17b.log ex17b.sel
EX18=ex18.out ex18.log
EX19=ex19.out ex19.log ex19b.out ex19b.log
EX20=ex20a.out ex20a.log ex20b.out ex20b.log
EX21=ex21.out ex21.log
EX22=ex22.out ex22.log


DISTCLEANFILES=$(EX1) $(EX2) $(EX3) $(EX4) $(EX5) $(EX15) phreeqc.log
