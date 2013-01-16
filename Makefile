# Makefile for PHREEQCPP
#
# Generates object files and executables for 2 versions of PHREEQCPP
#	    Release
#           Debug
#
# Serial verisons:   Release Debug
#
# Makefile sets CFG variable, cd's to appropriate directory, runs Makefile recursively
# Recursive make sets appropriate compiler, objects, options, libraries, and compiles PHREEQC
#

PROGRAM = phreeqc

CFG1 :=`uname`
CFG :=$(shell echo $(CFG1) | sed "s/CYGWIN.*/CYGWIN/")
ifeq ($(CFG), CYGWIN)
	SPOOL=>
	SPOOL2=2>&1
else
	SPOOL=>&
	SPOOL2=
endif

all: class_release class_debug

Debug: class_debug
debug: class_debug
Class_debug: class_debug

Release: class_release
release: class_release
Class_release: class_release

Debug_64: class_debug_64
debug_64: class_debug_64
Class_debug_64: class_debug_64

Release_64: class_release_64
release_64: class_release_64
Class_release_64: class_release_64


CLASS_DEBUG_DIR        = Class_debug
CLASS_DIR              = Class_release
CLASS_DEBUG_64_DIR     = Class_debug_64
CLASS_64_DIR           = Class_release_64
MAKEFILE               = Makefile
DEPDIR                 = .deps

# -----------------------------------------------------------------------------
# fixes shared object lookup error(SIGFPE floating point exception)
HASH_STYLE=$(call ld-option, -Wl$(comma)--hash-style=sysv)


#########################
#### Serial Versions ####
#########################
.PHONY : Class_debug
class_debug:
	mkdir -p $(CLASS_DEBUG_DIR) 
	cd $(CLASS_DEBUG_DIR); $(MAKE) -r -f ../$(MAKEFILE) CFG=CLASS_DEBUG $(PROGRAM)

.PHONY : Class_release
class_release:
	mkdir -p $(CLASS_DIR) 
	cd $(CLASS_DIR); $(MAKE) -r -f ../$(MAKEFILE) CFG=CLASS_RELEASE $(PROGRAM)
.PHONY : Class_debug_64
class_debug_64:
	mkdir -p $(CLASS_DEBUG_64_DIR) 
	cd $(CLASS_DEBUG_64_DIR); $(MAKE) -r -f ../$(MAKEFILE) CFG=CLASS_DEBUG_64 $(PROGRAM)

.PHONY : Class_release_64
class_release_64:
	mkdir -p $(CLASS_64_DIR) 
	cd $(CLASS_64_DIR); $(MAKE) -r -f ../$(MAKEFILE) CFG=CLASS_RELEASE_64 $(PROGRAM)

# Recursive make begins here
#
# =============================================================================
# Significant suffixes [assuming Fortran 90 (.f90)  source code]:
# Significant suffixes [assuming Fortran 90 (.F90)  source code, needs to be preprocessed ]:
# =============================================================================

.SUFFIXES : .o .c .cxx .cpp

# compilers

# -----------------------------------------------------------------------------
.c.o :
	test -z "$(DEPDIR)" || /bin/mkdir -p "$(DEPDIR)"
	depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.o$$||'`;\
	${CXX} ${CXXFLAGS} -MT $@ -MD -MP -MF $$depbase.Tpo -c -o $@ $< &&\
	mv -f $$depbase.Tpo $$depbase.Po 

.cxx.o :
	test -z "$(DEPDIR)" || /bin/mkdir -p "$(DEPDIR)"
	depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.o$$||'`;\
	${CXX} ${CXXFLAGS} -MT $@ -MD -MP -MF $$depbase.Tpo -c -o $@ $< &&\
	mv -f $$depbase.Tpo $$depbase.Po 

.cpp.o :
	test -z "$(DEPDIR)" || /bin/mkdir -p "$(DEPDIR)"
	depbase=`echo $@ | sed 's|[^/]*$$|$(DEPDIR)/&|;s|\.o$$||'`;\
	${CXX} ${CXXFLAGS} -MT $@ -MD -MP -MF $$depbase.Tpo -c -o $@ $< &&\
	mv -f $$depbase.Tpo $$depbase.Po 

# -----------------------------------------------------------------------------
#hdf options
#HDF5_ROOT=$(HOME)/../../usr
#HDF5_INCLUDES=-I$(HDF5_ROOT)/src
#HDF5_LIBS=${HDF5_ROOT}/lib/libhdf5.a -lz -lpthread  

# -----------------------------------------------------------------------------
# #define compile options

# -----------------------------------------------------------------------------
# #define gmp for inverse modeling
# comment the following lines to remove multiprecision option

# -----------------------------------------------------------------------------
#efence for debugging
EFENCE_LIB=-L$(HOME)/packages/efence

# -----------------------------------------------------------------------------
# 4 Versions
# -----------------------------------------------------------------------------
ifeq ($(CFG), CLASS_DEBUG)
  INVERSE_CL1MP=FALSE
  ifdef INVERSE_CL1MP
	DEFINE_INVERSE_CL1MP=-DINVERSE_CL1MP
	CL1MP_OBJS=cl1mp.o
#	CL1MP_LIB=-lgmp
	CL1MP_LIB=/z/parkplace/usr/lib/libgmp.a 
  endif
  DEFINES      = -DUSE_PHRQ_ALLOC $(DEFINE_INVERSE_CL1MP) # -DPHREEQC2
  VPATH        = ..
  INCLUDES     = -I..
  CXX          = g++
  CXXFLAGS     = -Wall -g $(DEFINES) $(INCLUDES) 
  OBJECT_FILES = $(CLASS_FILES) $(COMMON_COBJS) $(COMMON_CXXOBJS) $(CL1MP_OBJS)
  LD_FLAGS     = -lm ${CL1MP_LIB} ${HASH_STYLE}
endif

ifeq ($(CFG), CLASS_RELEASE)
  INVERSE_CL1MP=TRUE
  ifdef INVERSE_CL1MP
	DEFINE_INVERSE_CL1MP=-DINVERSE_CL1MP
	CL1MP_OBJS=cl1mp.o
#	CL1MP_LIB=-lgmp
	CL1MP_LIB=/z/parkplace/usr/lib/libgmp.a 
  endif
  DEFINES      = -DNDEBUG $(DEFINE_INVERSE_CL1MP) # -DPHREEQC2
  VPATH        = ..:../phreeqc
  INCLUDES     = -I../phreeqc -I..
  CXX          = g++
  CXXFLAGS     = -Wall -pedantic -O3 $(DEFINES) $(INCLUDES)
  OBJECT_FILES = $(CLASS_FILES) $(COMMON_COBJS) $(COMMON_CXXOBJS) $(CL1MP_OBJS)
  LD_FLAGS     = -lm ${CL1MP_LIB} ${HASH_STYLE}
endif

ifeq ($(CFG), CLASS_DEBUG_64)
  DEFINES      = -DUSE_PHRQ_ALLOC # $(DEFINE_INVERSE_CL1MP) # -DPHREEQC2
  VPATH        = ..:../phreeqc
  INCLUDES     = -I../phreeqc -I..
  CXX          = g++
  CXXFLAGS     = -Wall -g $(DEFINES) $(INCLUDES) 
  OBJECT_FILES = $(CLASS_FILES) $(COMMON_COBJS) $(COMMON_CXXOBJS) $(CL1MP_OBJS)
  LD_FLAGS     = -lm ${CL1MP_LIB} ${HASH_STYLE}
endif

ifeq ($(CFG), CLASS_RELEASE_64)
  DEFINES      = -DNDEBUG # $(DEFINE_INVERSE_CL1MP) # -DPHREEQC2
  VPATH        = ..:../phreeqc
  INCLUDES     = -I../phreeqc -I..
  CXX          = g++
  CXXFLAGS     = -Wall -pedantic -O3 $(DEFINES) $(INCLUDES)
  OBJECT_FILES = $(CLASS_FILES) $(COMMON_COBJS) $(COMMON_CXXOBJS) $(CL1MP_OBJS)
  LD_FLAGS     = -lm ${CL1MP_LIB} ${HASH_STYLE}
endif

# -----------------------------------------------------------------------------
#

COMMON_COBJS =  \
               advection.o \
	       basicsubs.o \
	       cl1.o \
	       cvdense.o \
	       cvode.o \
	       dense.o \
	       dw.o \
	       gases.o \
	       input.o \
	       integrate.o \
	       inverse.o \
	       isotopes.o \
	       kinetics.o \
	       mainsubs.o \
	       model.o \
	       nvector.o \
	       nvector_serial.o \
	       parse.o \
	       PHRQ_io_output.o \
	       phqalloc.o \
	       pitzer.o \
	       pitzer_structures.o \
	       prep.o \
	       print.o \
	       read.o \
	       readtr.o \
	       sit.o \
	       smalldense.o \
	       spread.o \
	       step.o \
	       structures.o \
	       sundialsmath.o \
	       tally.o \
	       tidy.o \
	       transport.o \
	       utilities.o

MAIN_FILE      = main.o 

CLASS_FILES = \
		class_main.o \
		Phreeqc.o


COMMON_CXXOBJS = \
	       dumper.o \
	       Exchange.o \
	       ExchComp.o \
	       GasPhase.o \
	       GasComp.o \
	       ISolution.o \
	       ISolutionComp.o \
	       Keywords.o \
	       KineticsComp.o \
	       cxxKinetics.o \
	       cxxMix.o \
	       NameDouble.o \
	       NumKeyword.o \
	       Parser.o \
	       PBasic.o \
	       PHRQ_base.o \
	       PHRQ_io.o \
	       PPassemblageComp.o \
	       PPassemblage.o \
	       Pressure.o \
	       Reaction.o \
	       ReadClass.o \
	       runner.o \
	       Solution.o \
	       SolutionIsotope.o \
	       SSassemblage.o \
	       SScomp.o \
	       SS.o \
               StorageBin.o \
               StorageBinList.o \
	       Surface.o \
	       SurfaceCharge.o \
	       SurfaceComp.o \
	       System.o \
	       Temperature.o \
	       Use.o \
	       Utils.o 

# -----------------------------------------------------------------------------
#  Assign dependents to target on dependency line & link options on command 
#  line. Command line is initiated with a tab. ($@ is an internal macro for 
#  the current target.) 

${PROGRAM} : ${OBJECT_FILES}
#	${CXX} -p -o $@ ${OBJECT_FILES}  ${LD_FLAGS} 
	${CXX} -o $@ ${OBJECT_FILES}  ${LD_FLAGS} 
	@echo; echo Done making for phreeqc
# -----------------------------------------------------------------------------
# Module dependency list
# -----------------------------------------------------------------------------
#
#  CXX files
#
-include $(DEPDIR)/cxxKinetics.Po
-include $(DEPDIR)/cxxMix.Po
-include $(DEPDIR)/Exchange.Po
-include $(DEPDIR)/ExchComp.Po
-include $(DEPDIR)/GasComp.Po
-include $(DEPDIR)/GasPhase.Po
-include $(DEPDIR)/ISolutionComp.Po
-include $(DEPDIR)/ISolution.Po
-include $(DEPDIR)/KineticsComp.Po
-include $(DEPDIR)/NameDouble.Po
-include $(DEPDIR)/NumKeyword.Po
-include $(DEPDIR)/Parser.Po
-include $(DEPDIR)/PHRQ_base.Po
-include $(DEPDIR)/PPassemblageComp.Po
-include $(DEPDIR)/PPassemblage.Po
-include $(DEPDIR)/Pressure.Po
-include $(DEPDIR)/Reaction.Po
-include $(DEPDIR)/ReadClass.Po
-include $(DEPDIR)/Solution.Po
-include $(DEPDIR)/SolutionIsotope.Po
-include $(DEPDIR)/SSassemblage.Po
-include $(DEPDIR)/SScomp.Po
-include $(DEPDIR)/SS.Po
-include $(DEPDIR)/StorageBin.Po
-include $(DEPDIR)/SurfaceCharge.Po
-include $(DEPDIR)/SurfaceComp.Po
-include $(DEPDIR)/Surface.Po
-include $(DEPDIR)/System.Po
-include $(DEPDIR)/Temperature.Po
-include $(DEPDIR)/Utils.Po
-include $(DEPDIR)/ChartHandler.Po
-include $(DEPDIR)/ChartObject.Po
-include $(DEPDIR)/class_main.Po
-include $(DEPDIR)/CurveObject.Po
-include $(DEPDIR)/dumper.Po
-include $(DEPDIR)/Keywords.Po
-include $(DEPDIR)/PBasic.Po
-include $(DEPDIR)/Phreeqc.Po
-include $(DEPDIR)/PHRQ_io.Po
-include $(DEPDIR)/runner.Po
-include $(DEPDIR)/StorageBinList.Po
-include $(DEPDIR)/Use.Po
-include $(DEPDIR)/advection.Po
-include $(DEPDIR)/basicsubs.o
-include $(DEPDIR)/cl1.Po
-include $(DEPDIR)/cl1mp.Po
-include $(DEPDIR)/cvdense.Po
-include $(DEPDIR)/cvode.Po
-include $(DEPDIR)/dense.Po
-include $(DEPDIR)/dw.Po
-include $(DEPDIR)/gases.Po
-include $(DEPDIR)/input.Po
-include $(DEPDIR)/integrate.Po
-include $(DEPDIR)/inverse.Po
-include $(DEPDIR)/isotopes.Po
-include $(DEPDIR)/kinetics.Po
-include $(DEPDIR)/mainsubs.Po
-include $(DEPDIR)/model.Po
-include $(DEPDIR)/nvector.Po
-include $(DEPDIR)/nvector_serial.Po
-include $(DEPDIR)/parse.Po
-include $(DEPDIR)/phqalloc.Po
-include $(DEPDIR)/PHRQ_io_output.Po
-include $(DEPDIR)/pitzer.Po
-include $(DEPDIR)/pitzer_structures.Po
-include $(DEPDIR)/prep.Po
-include $(DEPDIR)/print.Po
-include $(DEPDIR)/read.Po
-include $(DEPDIR)/readtr.Po
-include $(DEPDIR)/sit.Po
-include $(DEPDIR)/smalldense.Po
-include $(DEPDIR)/spread.Po
-include $(DEPDIR)/step.Po
-include $(DEPDIR)/structures.Po
-include $(DEPDIR)/sundialsmath.Po
-include $(DEPDIR)/tally.Po
-include $(DEPDIR)/tidy.Po
-include $(DEPDIR)/transport.Po
-include $(DEPDIR)/utilities.Po

# -----------------------------------------------------------------------------
clean:
	rm -rf Class_release Class_debug Class_release_64 Class_debug_64

dependencies:
	mkdir -p $(CLASS_DEBUG_DIR) 
	cd $(CLASS_DEBUG_DIR); gcc -MM -I.. ../*.cxx ../*.cpp 

tester:
	cd ../mytest; make clean; make -k -j 4 $(SPOOL) make.out $(SPOOL2); make diff $(SPOOL) diff.out $(SPOOL2)
	cd ../examples; make clean; make -k -j 4 $(SPOOL) make.out $(SPOOL2); make diff $(SPOOL) diff.out $(SPOOL2)
	svn status -q ../mytest 
	svn status -q ../examples

#ld-option
# Usage: ldflags += $(call ld-option, -Wl$(comma)--hash-style=sysv)
comma=,
ld-option = $(shell if $(CC) $(1) \
              -nostdlib -o /dev/null -xc /dev/null \
              > /dev/null 2>&1 ; then echo "$(1)" ; else echo "$(2)"; fi)

# =============================================================================
# End of makefile.
# =============================================================================
