# Locations to save compressed tar file for distribution
EXPORT=$(TOPDIR)/src/phreeqc_export
EXPORT_DIR=$(EXPORT)
WIN_DIR=$(TOPDIR)/win
DIST_DIR=$(EXPORT_DIR)
DEBUG_DIR=phreeqc_debug
CURSRC=$(CURDIR)
DEBUG_EXE=$(CURSRC)/phreeqc
VERSION=2.12
VER_DATE:=November 10, 2005
VER_LONG_DATE:=$(shell date -d "$(VER_DATE)" "+%B %e, %G")
V_FIXDATE:=$(shell date -d "$(VER_DATE)" "+%d-%b-%G")
GCC_VER:=$(shell gcc -v 2>&1 | egrep ^gcc | sed "s/version //")
KERNEL_VER:=$(shell uname -r)
ROOTNAME=$(PROGRAM)-$(VERSION)-$(REVISION)
TEXTCP=textcp DOS
SUN_DIR=$(TOPDIR)/src/Sun
UNIX2DOS=unix2dos
CCFLAGS_DBG=-Wall -ansi -g -DUSE_PHRQ_ALLOC # -std=c99 

CFG1 :=`uname`
CFG :=$(shell echo $(CFG1) | sed "s/CYGWIN.*/CYGWIN/")
ifeq ($(CFG), CYGWIN)
	SPOOL=>
	SPOOL2=2>&1
else
	SPOOL=>&
	SPOOL2=
endif

# list of files for distribution
##FILES=  \
##	$(CURSRC)/Makefile \
##	$(CURSRC)/advection.c \
##	$(CURSRC)/basic.c \
##	$(CURSRC)/basicsubs.c \
##	$(CURSRC)/cl1.c \
##	$(CURSRC)/cl1mp.c \
##	$(CURSRC)/cvdense.c \
##	$(CURSRC)/cvode.c \
##	$(CURSRC)/dense.c \
##	$(CURSRC)/dw.c \
##	$(CURSRC)/input.c \
##	$(CURSRC)/integrate.c \
##	$(CURSRC)/inverse.c \
##	$(CURSRC)/isotopes.c \
##	$(CURSRC)/kinetics.c \
##	$(CURSRC)/main.c \
##	$(CURSRC)/mainsubs.c \
##	$(CURSRC)/model.c \
##	$(CURSRC)/nvector.c \
##	$(CURSRC)/nvector_serial.c \
##	$(CURSRC)/output.c \
##	$(CURSRC)/p2clib.c \
##	$(CURSRC)/parse.c \
##	$(CURSRC)/phqalloc.c \
##	$(CURSRC)/phreeqc_files.c \
##	$(CURSRC)/pitzer.c \
##	$(CURSRC)/pitzer_structures.c \
##	$(CURSRC)/prep.c \
##	$(CURSRC)/print.c \
##	$(CURSRC)/read.c \
##	$(CURSRC)/readtr.c \
##	$(CURSRC)/sit.c \
##	$(CURSRC)/smalldense.c \
##	$(CURSRC)/spread.c \
##	$(CURSRC)/step.c \
##	$(CURSRC)/structures.c \
##	$(CURSRC)/sundialsmath.c \
##	$(CURSRC)/tally.c \
##	$(CURSRC)/tidy.c \
##	$(CURSRC)/transport.c \
##	$(CURSRC)/utilities.c \
##	$(CURSRC)/basic.h \
##	$(CURSRC)/cvdense.h \
##	$(CURSRC)/cvode.h \
##	$(CURSRC)/dense.h \
##	$(CURSRC)/global.h \
##	$(CURSRC)/global_structures.h \
##	$(CURSRC)/input.h \
##	$(CURSRC)/kinetics.h \
##	$(CURSRC)/nvector.h \
##	$(CURSRC)/nvector_serial.h \
##	$(CURSRC)/output.h \
##	$(CURSRC)/p2c.h \
##	$(CURSRC)/phqalloc.h \
##	$(CURSRC)/phrqproto.h \
##	$(CURSRC)/phrqtype.h \
##	$(CURSRC)/pitzer.h \
##	$(CURSRC)/pitzer_structures.h \
##	$(CURSRC)/smalldense.h \
##	$(CURSRC)/sundialsmath.h \
##	$(CURSRC)/sundialstypes.h \
##	database/*.dat \
##	examples/ex1 examples/ex1.out \
##	examples/ex2 examples/ex2.out examples/ex2.sel \
##	examples/ex3 examples/ex3.out \
##	examples/ex4 examples/ex4.out \
##	examples/ex5 examples/ex5.out examples/ex5.sel \
##	examples/ex6 examples/ex6.out examples/ex6A-B.sel examples/ex6C.sel \
##	examples/ex7 examples/ex7.out examples/ex7.sel \
##	examples/ex8 examples/ex8.out examples/ex8.sel \
##	examples/ex9 examples/ex9.out examples/ex9.sel \
##	examples/ex10 examples/ex10.out examples/ex10.sel \
##	examples/ex11 examples/ex11.out examples/ex11adv.sel examples/ex11trn.sel \
##	examples/ex12 examples/ex12.out examples/ex12.sel \
##	examples/ex12a examples/ex12a.out examples/ex12a.sel \
##	examples/ex13a examples/ex13a.out examples/ex13a.sel \
##	examples/ex13b examples/ex13b.out examples/ex13b.sel \
##	examples/ex13c examples/ex13c.out examples/ex13c.sel \
##	examples/ex14 examples/ex14.out examples/ex14.sel \
##	examples/ex15 examples/ex15.dat examples/ex15.out examples/ex15.sel \
##	examples/ex16 examples/ex16.out \
##	examples/ex17 examples/ex17.out \
##	examples/ex18 examples/ex18.out \
##	NOTICE.TXT \
##	README.TXT \
##	RELEASE.TXT \
##	doc/manual.pdf \
##	doc/wrir02-4172.pdf \
##	doc/phreeqc.txt \
##	bin/phreeqc.orig \
##	test/test.sh \
##	test/clean.sh \
##	test/check.sh 

##SED_ARGS= \
##	-e "s/@VERSION@/$(VERSION)/" \
##	-e "s/@VER_DATE@/$(VER_LONG_DATE)/" \
##	-e "s/VERSION_DATE/$(VERSION)/" \
##	-e "s^@GCC_VER@^$(GCC_VER)^" \
##	-e "s^@KERNEL_VER@^$(KERNEL_VER)^" \
##	-e "s/@REVISION@/$(REVISION)/" \
##	-e "s/@PHREEQC_VER@/$(VERSION)/" \
##	-e "s/@PHREEQC_DATE@/$(VER_LONG_DATE)/"
##

##SED_ARGS2=\
	-e "s/^\# Remove the following definition if you do not have /\# Add the following definition if you have/" \
	-e "s/^INVERSE_CL1MP=TRUE/\#INVERSE_CL1MP=TRUE/"

##remake_output_files: clean_linux_output_files linux_output_files # clean_sun_output_files sun_output_files

##output_files: linux_output_files # sun_output_files

# make sure program is compiles, run examples and mytest
##linux_output_files: all 
##	cd ../examples; make >& make.out 
##	cd ../mytest; make >& make.out 

##clean_linux_output_files:
##	cd ../examples; make clean
##	cd ../mytest; make clean

##sun_output_files: phreeqc.sun 
##	ssh u450rcolkr "cd $(SUN_DIR)/examples; make -f $(SUN_DIR)/../../examples/Makefile INPUT=$(SUN_DIR)/../../examples PHREEQCDAT=$(SUN_DIR)/../../database/phreeqc.dat WATEQ4FDAT=$(SUN_DIR)/../../database/wateq4f.dat"

##phreeqc.sun: 
##	ssh u450rcolkr "cd $(SUN_DIR)/src; make -f $(SUN_DIR)/../Makefile SRC=$(SUN_DIR)/.. EXE=$(SUN_DIR)/bin/phreeqc"	

##clean_sun_output_files:
##	cd $(SUN_DIR)/examples; make -f ../../../examples/Makefile clean

##all_dist:  clean_dist linux source # win # sun 

##test_dist: linux_test source_test # sun_test

#
#Linux
#
##linux: linux_export linux_clean linux_sed_files linux_compile linux_output linux_dist 

##source: source_export source_clean source_sed_files source_dist

##linux_export:
##	mkdir -p $(EXPORT_DIR)
##	rm -rf $(EXPORT_DIR)/Linux
##	svn export -r $(REVISION) http://internalbrr/svn_GW/phreeqc/trunk $(EXPORT_DIR)/Linux

##linux_clean:
##	rm -f $(EXPORT_DIR)/Linux/bin/$(PROGRAM) $(EXPORT_DIR)/Linux/src/*.o 

##linux_sed_list= \
##	"$(EXPORT_DIR)/Linux/doc/README.TXT \
##	$(EXPORT_DIR)/Linux/src/main.c"

##linux_sed_files:
##	sed $(SED_ARGS) < $(EXPORT_DIR)/Linux/src/revisions > $(EXPORT_DIR)/Linux/RELEASE.TXT
##	for FILE in "$(linux_sed_list)"; do \
##		sed $(SED_ARGS) < $$FILE > t; mv t $$FILE; done
##	mv $(EXPORT_DIR)/Linux/doc/README.TXT $(EXPORT_DIR)/Linux/README.TXT
##	mv $(EXPORT_DIR)/Linux/doc/NOTICE.TXT $(EXPORT_DIR)/Linux/NOTICE.TXT

##linux_compile:
##	make -C $(EXPORT_DIR)/Linux/src

##linux_output:
##	cd $(EXPORT_DIR)/Linux/examples; make clean; make >& make.out

##linux_dist: 
##	cd $(EXPORT_DIR)/Linux; rm -f $(PROGRAM).tar
##
##	cd $(EXPORT_DIR)/Linux; for FILE in $(FILES); do tar -rf $(PROGRAM).tar $$FILE; done
##	cd $(EXPORT_DIR)/Linux; tar -rf $(PROGRAM).tar bin/$(PROGRAM)
##	cd $(EXPORT_DIR)/Linux; rm -rf $(PROGRAM)-$(VERSION)
##	cd $(EXPORT_DIR)/Linux; mkdir $(PROGRAM)-$(VERSION)
##	cd $(EXPORT_DIR)/Linux; mv $(PROGRAM).tar $(PROGRAM)-$(VERSION)
##	cd $(EXPORT_DIR)/Linux; cd $(PROGRAM)-$(VERSION); tar -xf $(PROGRAM).tar; rm -f $(PROGRAM).tar	
##	cd $(EXPORT_DIR)/Linux; sed $(SED_ARGS2) < $(PROGRAM)-$(VERSION)/src/Makefile > t; mv t $(PROGRAM)-$(VERSION)/src/Makefile	
##	cd $(EXPORT_DIR)/Linux; tar -czf $(PROGRAM).Linux.tar.gz $(PROGRAM)-$(VERSION)
##	cd $(EXPORT_DIR)/Linux; mv $(PROGRAM).Linux.tar.gz $(DIST_DIR)/$(ROOTNAME).Linux.tar.gz
##	cd $(EXPORT_DIR)/Linux; echo $(ROOTNAME).Linux.tar.gz saved in $(DIST_DIR).
##	cd $(EXPORT_DIR)/Linux; rm -rf $(PROGRAM)-$(VERSION)

##source_export:
##	mkdir -p $(EXPORT_DIR)
##	rm -rf $(EXPORT_DIR)/Source
##	svn export -r $(REVISION) http://internalbrr/svn_GW/phreeqc/trunk $(EXPORT_DIR)/Source

##source_clean:
##	rm -f $(EXPORT_DIR)/Source/bin/$(PROGRAM) $(EXPORT_DIR)/Source/src/*.o 

##source_sed_list= \
##	"$(EXPORT_DIR)/Source/doc/README.TXT \
##	$(EXPORT_DIR)/Source/src/main.c"

##source_sed_files:
##	sed $(SED_ARGS) < $(EXPORT_DIR)/Source/src/revisions > $(EXPORT_DIR)/Source/RELEASE.TXT
##	sed $(SED_ARGS2) < $(EXPORT_DIR)/Source/src/Makefile > t; mv t $(EXPORT_DIR)/Source/src/Makefile
##	for FILE in "$(source_sed_list)"; do \
##		sed $(SED_ARGS) < $$FILE > t; mv t $$FILE; done
##	mv $(EXPORT_DIR)/Source/doc/README.TXT $(EXPORT_DIR)/Source/README.TXT
##	mv $(EXPORT_DIR)/Source/doc/NOTICE.TXT $(EXPORT_DIR)/Source/NOTICE.TXT

##source_dist:
##	cd $(EXPORT_DIR)/Source; rm -f $(PROGRAM).tar
##	cd $(EXPORT_DIR)/Source; for FILE in $(FILES); do tar -rf $(PROGRAM).tar $$FILE; done
##	cd $(EXPORT_DIR)/Source; rm -rf $(PROGRAM)-$(VERSION)
##	cd $(EXPORT_DIR)/Source; mkdir $(PROGRAM)-$(VERSION)
##	cd $(EXPORT_DIR)/Source; mv $(PROGRAM).tar $(PROGRAM)-$(VERSION)
##	cd $(EXPORT_DIR)/Source; cd $(PROGRAM)-$(VERSION); tar -xf $(PROGRAM).tar; rm $(PROGRAM).tar
##	cd $(EXPORT_DIR)/Source; tar -czf $(PROGRAM).source.tar.gz $(PROGRAM)-$(VERSION)
##	cd $(EXPORT_DIR)/Source; mv $(PROGRAM).source.tar.gz $(DIST_DIR)/$(ROOTNAME).source.tar.gz
##	cd $(EXPORT_DIR)/Source; echo $(ROOTNAME).source.tar.gz saved in $(DIST_DIR).
##	cd $(EXPORT_DIR)/Source; rm -rf $(PROGRAM)-$(VERSION)
##
##linux_test:
##	rm -rf $(DIST_DIR)/phreeqc-$(VERSION).Linux
##	cd $(DIST_DIR); tar -xzf phreeqc-$(VERSION)-*.Linux.tar.gz; mv phreeqc-$(VERSION) phreeqc-$(VERSION).Linux
##	cd $(DIST_DIR)/phreeqc-$(VERSION).Linux/test; ./test.sh
##	rm -f $(DIST_DIR)/phreeqc-$(VERSION).Linux/bin/phreeqc
##	cd $(DIST_DIR)/phreeqc-$(VERSION).Linux/src; make -k
##	cd $(DIST_DIR)/phreeqc-$(VERSION).Linux/test; ./clean.sh; ./test.sh

##source_test:
##	rm -rf $(DIST_DIR)/phreeqc-$(VERSION).source
##	cd $(DIST_DIR); tar -xzf phreeqc-$(VERSION)-*.source.tar.gz; mv phreeqc-$(VERSION) phreeqc-$(VERSION).source
##	cd $(DIST_DIR)/phreeqc-$(VERSION).source/src; make -k
##	cd $(DIST_DIR)/phreeqc-$(VERSION).source/test; ./test.sh

##clean_dist:
##	rm -rf $(EXPORT_DIR)

##clean_all: clean_dist clean_linux_output_files clean # clean_sun_output_files

#
#Win
#
##win: win_export win_sed_files win_dist 

##win_export:
##	mkdir -p $(EXPORT_DIR)
##	rm -rf $(EXPORT_DIR)/Win
##	svn export -r $(REVISION) http://internalbrr/svn_GW/phreeqc/trunk $(EXPORT_DIR)/Win

##win_sed_list= \
	"$(EXPORT_DIR)/Win/src/main.c"

##win_sed_files:
##	sed $(SED_ARGS) < $(EXPORT_DIR)/Win/src/revisions > $(EXPORT_DIR)/Win/RELEASE.TXT
##	$(UNIX2DOS) $(EXPORT_DIR)/Win/RELEASE.TXT
##	sed $(SED_ARGS) < $(WIN_DIR)/README.TXT > $(EXPORT_DIR)/Win/README.TXT
##	$(UNIX2DOS) $(EXPORT_DIR)/Win/README.TXT
##	for FILE in "$(win_sed_list)"; do \
##		sed $(SED_ARGS) < $$FILE > t; \
		mv t $$FILE; \
		$(UNIX2DOS) $$FILE; \
	done
##	mv $(EXPORT_DIR)/Win/doc/NOTICE.TXT $(EXPORT_DIR)/Win/NOTICE.TXT

##win_dist: 
##	cd $(EXPORT_DIR)/Win; rm -f $(PROGRAM).tar
# Translate cr/lf
##	cd $(EXPORT_DIR)/Win; for FILE in $(FILES); do \
		if [ $$FILE = doc/manual.pdf -o $$FILE = doc/wrir02-4172.pdf ]; then cp $$FILE t; mv t $$FILE; \
		else $(TEXTCP) $$FILE t; mv t $$FILE; fi; done
##	cd $(EXPORT_DIR)/Win; for FILE in $(FILES); do tar -rf $(PROGRAM).tar $$FILE; done
##	cd $(EXPORT_DIR)/Win; rm -rf $(PROGRAM)-$(VERSION)
##	cd $(EXPORT_DIR)/Win; mkdir $(PROGRAM)-$(VERSION)
##	cd $(EXPORT_DIR)/Win; mv $(PROGRAM).tar $(PROGRAM)-$(VERSION)
##	cd $(EXPORT_DIR)/Win; cd $(PROGRAM)-$(VERSION); tar -xf $(PROGRAM).tar; rm -f $(PROGRAM).tar
# remove example output
##	cd $(EXPORT_DIR)/Win; rm -f $(PROGRAM)-$(VERSION)/examples/*.out $(PROGRAM)-$(VERSION)/examples/*.sel
# remove bin directory
##	cd $(EXPORT_DIR)/Win; rm -rf $(PROGRAM)-$(VERSION)/bin
# remove test directory files
##	cd $(EXPORT_DIR)/Win; rm -f $(PROGRAM)-$(VERSION)/test/*
##	cd $(EXPORT_DIR)/Win; $(TEXTCP) $(WIN_DIR)/clean.bat $(PROGRAM)-$(VERSION)/test/clean.bat
##	cd $(EXPORT_DIR)/Win; $(TEXTCP) $(WIN_DIR)/check.bat $(PROGRAM)-$(VERSION)/test/check.bat
##	cd $(EXPORT_DIR)/Win; $(TEXTCP) $(WIN_DIR)/test.bat $(PROGRAM)-$(VERSION)/test/test.bat
# copy bat file
##	cd $(EXPORT_DIR)/Win; $(TEXTCP) $(WIN_DIR)/phreeqc.bat $(PROGRAM)-$(VERSION)/phreeqc.bat
##	cd $(EXPORT_DIR); rm -f $(PROGRAM).Windows.tar.gz
##	cd $(EXPORT_DIR)/Win/$(PROGRAM)-$(VERSION); tar -czf $(DIST_DIR)/$(ROOTNAME).Windows.tar.gz .
##	@echo $(ROOTNAME).Windows.tar.gz saved in $(DIST_DIR).
##	cd $(EXPORT_DIR)/Win; rm -rf $(PROGRAM)-$(VERSION)

##win_echo_files:
##	@echo $(FILES)

debug: 
	mkdir -p $(DEBUG_DIR)
	cd $(DEBUG_DIR); make -f $(CURSRC)/Makefile SRC=$(CURSRC) CCFLAGS="$(CCFLAGS_DBG) -DINVERSE_CL1MP"  EXE=$(DEBUG_EXE)

debug_nomp: 
	mkdir -p $(DEBUG_DIR)
	cd $(DEBUG_DIR); make -f $(CURSRC)/Makefile SRC=$(CURSRC) CCFLAGS="$(CCFLAGS_DBG)" INVERSE_CL1MP= EXE=$(DEBUG_EXE)

##web:
##	cp $(DIST_DIR)/phreeqc-$(VERSION)*.tar.gz /var/anonymous/ftp/dlpark/geochem/unix/phreeqc
##	cp $(EXPORT_DIR)/Linux/README.TXT /var/anonymous/ftp/dlpark/geochem/unix/phreeqc/README.TXT 
##	cp $(EXPORT_DIR)/Linux/README.TXT /z/linarcolkr/home/www/projects/GWC_coupled/phreeqc/README.Unix.TXT
##	cp ~/programs/phreeqc/win/README.TXT /var/anonymous/ftp/dlpark/geochem/pc/phreeqc/README.TXT 
##	cp ~/programs/phreeqc/win/README.TXT /z/linarcolkr/home/www/projects/GWC_coupled/phreeqc/README.Win.TXT
##	cp $(EXPORT_DIR)/Linux/doc/phreeqc.txt /z/linarcolkr/home/www/projects/GWC_coupled/phreeqc/phreeqc.txt
##	cp $(EXPORT_DIR)/Linux/RELEASE.TXT /z/linarcolkr/home/www/projects/GWC_coupled/phreeqc/RELEASE.TXT

tester:
	cd ../mytest; make clean; make -k $(SPOOL) make.out $(SPOOL2); make diff $(SPOOL) diff.out $(SPOOL2)
	cd ../examples; make clean; make $(SPOOL) make.out $(SPOOL2); make diff $(SPOOL) diff.out $(SPOOL2)
	svn status -q ../mytest 
	svn status -q ../examples
