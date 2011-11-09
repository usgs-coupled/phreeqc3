CFG1 :=`uname`
CFG :=$(shell echo $(CFG1) | sed "s/CYGWIN.*/CYGWIN/")

INPUT=../examples
PHREEQCDAT=../database/phreeqc.dat
WATEQ4FDAT=../database/wateq4f.dat

PHREEQC=../src/Class_release/phreeqcpp

ifeq ($(CFG), CYGWIN)
   PHREEQC=/cygdrive/c/Programs/phreeqcpp-trunk/ErrorHandling/Class_release/phreeqcpp.exe
endif

all: ex1.out ex2.out ex3.out ex4.out ex5.out ex6.out ex7.out ex8.out ex9.out \
	ex10.out ex11.out ex12.out ex12a.out ex13a.out ex13b.out ex13c.out ex14.out \
	ex15.out ex16.out ex17.out ex18.out 

ex1.out: $(INPUT)/ex1 $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex1 ex1.out $(PHREEQCDAT)
	mv phreeqc.log ex1.log

ex2.out: $(INPUT)/ex2 $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex2 ex2.out $(PHREEQCDAT)
	mv phreeqc.log ex2.log

ex3.out: $(INPUT)/ex3 $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex3 ex3.out $(PHREEQCDAT)
	mv phreeqc.log ex3.log

ex4.out: $(INPUT)/ex4 $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex4 ex4.out $(PHREEQCDAT)
	mv phreeqc.log ex4.log

ex5.out: $(INPUT)/ex5 $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex5 ex5.out $(PHREEQCDAT)
	mv phreeqc.log ex5.log

ex6.out: $(INPUT)/ex6 $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex6 ex6.out $(PHREEQCDAT)
	mv phreeqc.log ex6.log

ex7.out: $(INPUT)/ex7 $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex7 ex7.out $(PHREEQCDAT)
	mv phreeqc.log ex7.log

ex8.out: $(INPUT)/ex8 $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex8 ex8.out $(PHREEQCDAT)
	mv phreeqc.log ex8.log

ex9.out: $(INPUT)/ex9 $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex9 ex9.out $(PHREEQCDAT)
	mv phreeqc.log ex9.log

ex10.out: $(INPUT)/ex10 $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex10 ex10.out $(PHREEQCDAT)
	mv phreeqc.log ex10.log

ex11.out: $(INPUT)/ex11 $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex11 ex11.out $(PHREEQCDAT)
	mv phreeqc.log ex11.log

ex12.out: $(INPUT)/ex12 $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex12 ex12.out $(PHREEQCDAT)
	mv phreeqc.log ex12.log

ex12a.out: $(INPUT)/ex12a $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex12a ex12a.out $(PHREEQCDAT)
	mv phreeqc.log ex12a.log

ex13a.out: $(INPUT)/ex13a $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex13a ex13a.out $(PHREEQCDAT)
	mv phreeqc.log ex13a.log

ex13b.out: $(INPUT)/ex13b $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex13b ex13b.out $(PHREEQCDAT)
	mv phreeqc.log ex13b.log

ex13c.out: $(INPUT)/ex13c $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex13c ex13c.out $(PHREEQCDAT)
	mv phreeqc.log ex13c.log

ex14.out: $(INPUT)/ex14 $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex14 ex14.out $(WATEQ4FDAT)
	mv phreeqc.log ex14.log

ex15.out: $(INPUT)/ex15 $(PHREEQC) $(INPUT)/ex15.dat
	$(PHREEQC) $(INPUT)/ex15 ex15.out $(INPUT)/ex15.dat
	mv phreeqc.log ex15.log

ex16.out: $(INPUT)/ex16 $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex16 ex16.out $(PHREEQCDAT)
	mv phreeqc.log ex16.log

ex17.out: $(INPUT)/ex17 $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex17 ex17.out $(PHREEQCDAT)
	mv phreeqc.log ex17.log

ex18.out: $(INPUT)/ex18 $(PHREEQC) $(PHREEQCDAT)
	$(PHREEQC) $(INPUT)/ex18 ex18.out $(PHREEQCDAT)
	mv phreeqc.log ex18.log

diff:
	svn diff --diff-cmd diff -x -bw	

ndiff:
	svn diff --diff-cmd /home/dlpark/bin/ndiff -x "--relative-error 1e-7"

clean:
	rm -f *.out *.log *.sel

revert:
	svn st | egrep ^! | cut -b 2- | xargs svn revert

zero:
	for FILE in *.out *.sel; \
		do \
			if [ $$FILE != make.out ]; then \
				echo $$FILE; \
				./zero.sed $$FILE; \
			fi \
		done; 

diff_phreeqc:
	for FILE in ex*.out ex*.sel; \
		do \
			echo $$FILE; \
			diff -bw $$FILE ../../phreeqc/examples; \
		done; 
