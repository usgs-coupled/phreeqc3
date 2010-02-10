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

PROGRAM = phreeqcpp

CFG1 :=`uname`
CFG :=$(shell echo $(CFG1) | sed "s/CYGWIN.*/CYGWIN/")
ifeq ($(CFG), CYGWIN)
	SPOOL=>
	SPOOL2=2>&1
else
	SPOOL=>&
	SPOOL2=
endif

all: release debug class_release class_debug

Release: release

Debug: debug

Class_debug: class_debug

Class_release: class_release

RELEASE_DIR             = Release
DEBUG_DIR               = Debug
CLASS_DEBUG_DIR         = Class_debug
CLASS_DIR              = Class_release
MAKEFILE                = Makefile

# -----------------------------------------------------------------------------
# fixes shared object lookup error(SIGFPE floating point exception)
HASH_STYLE=$(call ld-option, -Wl$(comma)--hash-style=sysv)


#########################
#### Serial Versions ####
#########################
.PHONY : Release
release:
	mkdir -p $(RELEASE_DIR) 
	cd $(RELEASE_DIR); $(MAKE) -r -f ../$(MAKEFILE) CFG=RELEASE $(PROGRAM)

.PHONY : Debug
debug:
	mkdir -p $(DEBUG_DIR) 
	cd $(DEBUG_DIR); $(MAKE) -r -f ../$(MAKEFILE) CFG=DEBUG $(PROGRAM)

.PHONY : Class_debug
class_debug:
	mkdir -p $(CLASS_DEBUG_DIR) 
	cd $(CLASS_DEBUG_DIR); $(MAKE) -r -f ../$(MAKEFILE) CFG=CLASS_DEBUG $(PROGRAM)

.PHONY : Class_release
class_release:
	mkdir -p $(CLASS_DIR) 
	cd $(CLASS_DIR); $(MAKE) -r -f ../$(MAKEFILE) CFG=CLASS_RELEASE $(PROGRAM)

# Recursive make begins here
#
# =============================================================================
# Significant suffixes [assuming Fortran 90 (.f90)  source code]:
# Significant suffixes [assuming Fortran 90 (.F90)  source code, needs to be preprocessed ]:
# =============================================================================

#SRC     = ../phreeqc
.SUFFIXES : .o .c .cxx .cpp

# compilers

# -----------------------------------------------------------------------------
.c.o :
	${CXX} ${CXXFLAGS} -c -o $@ $<

.cxx.o :
	${CXX} ${CXXFLAGS} -c -o $@ $<

.cpp.o :
	${CXX} ${CXXFLAGS} -c -o $@ $<

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
INVERSE_CL1MP=TRUE
ifdef INVERSE_CL1MP
	DEFINE_INVERSE_CL1MP=-DINVERSE_CL1MP
	CL1MP_OBJS=cl1mp.o
	CL1MP_LIB=-lgmp
endif

# -----------------------------------------------------------------------------
#efence for debugging
EFENCE_LIB=-L$(HOME)/packages/efence

# -----------------------------------------------------------------------------
# 2 Versions
# -----------------------------------------------------------------------------
ifeq ($(CFG), RELEASE)
  DEFINES      = -DPHREEQC_CPP $(DEFINE_INVERSE_CL1MP)
  VPATH        = ..:../phreeqc
  INCLUDES     = -I../phreeqc
  CXX          = g++
  CXXFLAGS     = -Wall -pedantic -O3 $(DEFINES) $(INCLUDES)
  OBJECT_FILES = $(MAIN_FILE) $(COMMON_COBJS) $(COMMON_CXXOBJS) $(CL1MP_OBJS)
  LD_FLAGS     = -lm ${CL1MP_LIB} ${HASH_STYLE}
endif

ifeq ($(CFG), DEBUG)
  DEFINES      = -DPHREEQC_CPP -DUSE_PHRQ_ALLOC $(DEFINE_INVERSE_CL1MP)
  VPATH        = ..:../phreeqc
  INCLUDES     = -I../phreeqc
  CXX          = g++
  CXXFLAGS     = -Wall -g $(DEFINES) $(INCLUDES)
  OBJECT_FILES = $(MAIN_FILE) $(COMMON_COBJS) $(COMMON_CXXOBJS) $(CL1MP_OBJS)
  LD_FLAGS     = -lm ${CL1MP_LIB} ${HASH_STYLE}
endif

ifeq ($(CFG), CLASS_DEBUG)
  DEFINES      = -DPHREEQC_CPP -DUSE_PHRQ_ALLOC -DPHREEQC_CLASS $(DEFINE_INVERSE_CL1MP)
  VPATH        = ..:../phreeqc
  INCLUDES     = -I../phreeqc -I..
  CXX          = g++
  CXXFLAGS     = -Wall -g $(DEFINES) $(INCLUDES)
  OBJECT_FILES = $(CLASS_FILES) $(COMMON_COBJS) $(COMMON_CXXOBJS) $(CL1MP_OBJS)
  LD_FLAGS     = -lm ${CL1MP_LIB} ${HASH_STYLE}
endif

ifeq ($(CFG), CLASS_RELEASE)
  DEFINES      = -DPHREEQC_CPP -DUSE_PHRQ_ALLOC -DPHREEQC_CLASS $(DEFINE_INVERSE_CL1MP)
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
	       basic.o \
	       basicsubs.o \
	       cl1.o \
	       cvdense.o \
	       cvode.o \
	       dense.o \
	       dw.o \
	       input.o \
	       integrate.o \
	       inverse.o \
	       isotopes.o \
	       kinetics.o \
	       mainsubs.o \
	       model.o \
	       nvector.o \
	       nvector_serial.o \
	       output.o \
	       p2clib.o \
	       parse.o \
	       phqalloc.o \
	       phreeqc_files.o \
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
	       ISolution.o \
	       ISolutionComp.o \
	       KineticsComp.o \
	       cxxKinetics.o \
	       cxxMix.o \
	       NameDouble.o \
	       NumKeyword.o \
	       Parser.o \
	       PPassemblageComp.o \
	       PPassemblage.o \
	       Reaction.o \
	       ReadClass.o \
	       runner.o \
	       Solution.o \
	       SolutionIsotope.o \
	       SolutionIsotopeList.o \
	       SSassemblage.o \
	       SSassemblageSS.o \
               StorageBin.o \
               StorageBinList.o \
	       Surface.o \
	       SurfaceCharge.o \
	       SurfaceComp.o \
	       System.o \
	       Temperature.o \
	       Utils.o 

# -----------------------------------------------------------------------------
#  Assign dependents to target on dependency line & link options on command 
#  line. Command line is initiated with a tab. ($@ is an internal macro for 
#  the current target.) 

${PROGRAM} : ${OBJECT_FILES}
	${CXX} -o $@ ${OBJECT_FILES}  ${LD_FLAGS} 
	@echo; echo Done making for phreeqcpp
# -----------------------------------------------------------------------------
# Module dependency list
# -----------------------------------------------------------------------------
#
#  CXX files
#
cxxKinetics.o: ../cxxKinetics.cxx ../Utils.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h ../cxxKinetics.h \
  ../NumKeyword.h ../KineticsComp.h ../NameDouble.h ../Phreeqc_class.h \
  ../Parser.h ../cxxMix.h ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h
cxxMix.o: ../cxxMix.cxx ../Utils.h ../Parser.h ../Phreeqc_class.h \
  ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../cxxMix.h ../NumKeyword.h \
  ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h
Dictionary.o: ../Dictionary.cxx ../Dictionary.h ../Solution.h \
  ../Phreeqc_class.h ../NumKeyword.h ../SolutionIsotopeList.h \
  ../SolutionIsotope.h ../Parser.h ../NameDouble.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h \
  ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h ../phreeqc/output.h
Exchange.o: ../Exchange.cxx ../Utils.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h ../cxxMix.h \
  ../Phreeqc_class.h ../NumKeyword.h ../Exchange.h ../ExchComp.h \
  ../NameDouble.h ../Parser.h ../phreeqc/phqalloc.h \
  ../phreeqc/phrqproto.h ../phreeqc/output.h
ExchComp.o: ../ExchComp.cxx ../Utils.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h ../ExchComp.h \
  ../NameDouble.h ../Phreeqc_class.h ../Parser.h ../Dictionary.h \
  ../Solution.h ../NumKeyword.h ../SolutionIsotopeList.h \
  ../SolutionIsotope.h ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h \
  ../phreeqc/output.h
GasPhase.o: ../GasPhase.cxx ../Utils.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h ../GasPhase.h \
  ../NumKeyword.h ../NameDouble.h ../Phreeqc_class.h ../Parser.h \
  ../cxxMix.h ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h \
  ../phreeqc/output.h
ISolutionComp.o: ../ISolutionComp.cxx ../Utils.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h ../ISolutionComp.h \
  ../Phreeqc_class.h ../phreeqc/phrqproto.h ../phreeqc/phqalloc.h \
  ../phreeqc/output.h
ISolution.o: ../ISolution.cxx ../Utils.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h ../ISolution.h \
  ../ISolutionComp.h ../Phreeqc_class.h ../NumKeyword.h ../Solution.h \
  ../SolutionIsotopeList.h ../SolutionIsotope.h ../Parser.h \
  ../NameDouble.h ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h \
  ../phreeqc/output.h
KineticsComp.o: ../KineticsComp.cxx ../Utils.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h ../KineticsComp.h \
  ../NameDouble.h ../Phreeqc_class.h ../Parser.h ../Dictionary.h \
  ../Solution.h ../NumKeyword.h ../SolutionIsotopeList.h \
  ../SolutionIsotope.h ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h
NameDouble.o: ../NameDouble.cxx ../Utils.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h ../NameDouble.h \
  ../Phreeqc_class.h ../Parser.h ../Dictionary.h ../Solution.h \
  ../NumKeyword.h ../SolutionIsotopeList.h ../SolutionIsotope.h \
  ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h
NumKeyword.o: ../NumKeyword.cxx ../NumKeyword.h ../Parser.h \
  ../Phreeqc_class.h
Parser.o: ../Parser.cxx ../Utils.h ../Parser.h ../Phreeqc_class.h \
  ../phreeqc/output.h
PPassemblageComp.o: ../PPassemblageComp.cxx ../Utils.h \
  ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../PPassemblageComp.h ../NameDouble.h \
  ../Phreeqc_class.h ../Parser.h ../Dictionary.h ../Solution.h \
  ../NumKeyword.h ../SolutionIsotopeList.h ../SolutionIsotope.h \
  ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h ../phreeqc/output.h
PPassemblage.o: ../PPassemblage.cxx ../Utils.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h ../PPassemblage.h \
  ../NumKeyword.h ../PPassemblageComp.h ../NameDouble.h \
  ../Phreeqc_class.h ../Parser.h ../cxxMix.h ../phreeqc/phqalloc.h \
  ../phreeqc/phrqproto.h
Reaction.o: ../Reaction.cxx ../Utils.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h ../Reaction.h \
  ../NumKeyword.h ../NameDouble.h ../Phreeqc_class.h ../Parser.h \
  ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h
ReadClass.o: ../ReadClass.cxx ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../Phreeqc_class.h ../Parser.h \
  ../Solution.h ../NumKeyword.h ../SolutionIsotopeList.h \
  ../SolutionIsotope.h ../NameDouble.h ../Exchange.h ../ExchComp.h \
  ../Surface.h ../SurfaceComp.h ../SurfaceCharge.h ../PPassemblage.h \
  ../PPassemblageComp.h ../cxxKinetics.h ../KineticsComp.h \
  ../SSassemblage.h ../GasPhase.h ../Reaction.h ../cxxMix.h \
  ../Temperature.h ../dumper.h ../StorageBinList.h ../runner.h \
  ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h ../phreeqc/output.h
SAXPhreeqc.o: ../SAXPhreeqc.cxx ../SAXPhreeqc.h ../SaxPhreeqcHandlers.h \
  ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../phreeqc/phrqproto.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h
Solution.o: ../Solution.cxx ../Utils.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h ../Solution.h \
  ../Phreeqc_class.h ../NumKeyword.h ../SolutionIsotopeList.h \
  ../SolutionIsotope.h ../Parser.h ../NameDouble.h ../cxxMix.h \
  ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h ../phreeqc/output.h
SolutionIsotope.o: ../SolutionIsotope.cxx ../Utils.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h \
  ../SolutionIsotope.h ../Parser.h ../Phreeqc_class.h \
  ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h
SolutionIsotopeList.o: ../SolutionIsotopeList.cxx ../Utils.h \
  ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../SolutionIsotopeList.h \
  ../SolutionIsotope.h ../Parser.h ../Phreeqc_class.h \
  ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h
SSassemblage.o: ../SSassemblage.cxx ../Utils.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h ../SSassemblage.h \
  ../Phreeqc_class.h ../NumKeyword.h ../NameDouble.h ../Parser.h \
  ../SSassemblageSS.h ../cxxMix.h ../phreeqc/phqalloc.h \
  ../phreeqc/phrqproto.h
SSassemblageSS.o: ../SSassemblageSS.cxx ../Utils.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h \
  ../SSassemblageSS.h ../NameDouble.h ../Phreeqc_class.h ../Parser.h \
  ../Dictionary.h ../Solution.h ../NumKeyword.h ../SolutionIsotopeList.h \
  ../SolutionIsotope.h ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h
StorageBin.o: ../StorageBin.cxx ../Utils.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h ../NameDouble.h \
  ../Phreeqc_class.h ../Parser.h ../StorageBin.h ../System.h \
  ../SSassemblage.h ../NumKeyword.h ../Solution.h \
  ../SolutionIsotopeList.h ../SolutionIsotope.h ../Exchange.h \
  ../ExchComp.h ../GasPhase.h ../cxxKinetics.h ../KineticsComp.h \
  ../PPassemblage.h ../PPassemblageComp.h ../SSassemblageSS.h \
  ../Surface.h ../SurfaceComp.h ../SurfaceCharge.h ../cxxMix.h \
  ../Reaction.h ../Temperature.h ../phreeqc/phqalloc.h \
  ../phreeqc/phrqproto.h ../phreeqc/output.h
SurfaceCharge.o: ../SurfaceCharge.cxx ../Utils.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h ../SurfaceCharge.h \
  ../NameDouble.h ../Phreeqc_class.h ../Parser.h ../Dictionary.h \
  ../Solution.h ../NumKeyword.h ../SolutionIsotopeList.h \
  ../SolutionIsotope.h ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h \
  ../phreeqc/output.h
SurfaceComp.o: ../SurfaceComp.cxx ../Utils.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h ../SurfaceComp.h \
  ../Phreeqc_class.h ../NameDouble.h ../Parser.h ../Dictionary.h \
  ../Solution.h ../NumKeyword.h ../SolutionIsotopeList.h \
  ../SolutionIsotope.h ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h \
  ../phreeqc/output.h
Surface.o: ../Surface.cxx ../Phreeqc_class.h ../Utils.h \
  ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../Surface.h ../NumKeyword.h \
  ../SurfaceComp.h ../NameDouble.h ../Parser.h ../SurfaceCharge.h \
  ../cxxMix.h ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h
System.o: ../System.cxx ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../System.h ../NameDouble.h \
  ../Phreeqc_class.h ../Parser.h ../SSassemblage.h ../NumKeyword.h \
  ../Solution.h ../SolutionIsotopeList.h ../SolutionIsotope.h \
  ../Exchange.h ../ExchComp.h ../GasPhase.h ../cxxKinetics.h \
  ../KineticsComp.h ../PPassemblage.h ../PPassemblageComp.h \
  ../SSassemblageSS.h ../Surface.h ../SurfaceComp.h ../SurfaceCharge.h \
  ../cxxMix.h ../Reaction.h ../Temperature.h
Temperature.o: ../Temperature.cxx ../Utils.h ../Parser.h \
  ../Phreeqc_class.h ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../Temperature.h ../NumKeyword.h \
  ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h
Utils.o: ../Utils.cxx ../Utils.h ../Parser.h ../Phreeqc_class.h \
  ../phreeqc/output.h
class_main.o: ../class_main.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
  ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
  ../phreeqc/sundialstypes.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
  ../phreeqc/nvector.h ../phreeqc/dense.h ../phreeqc/smalldense.h \
  ../phreeqc/nvector_serial.h ../phreeqc/dense.h ../phreeqc/p2c.h \
  ../phreeqc/global_structures.h ../phreeqc/pitzer_structures.h \
  ../phreeqc/pitzer.h ../phreeqc/input.h ../phreeqc/output.h \
  ../phreeqc/global.h ../phreeqc/global_structures.h \
  ../phreeqc/phrqproto.h
dumper.o: ../dumper.cpp ../dumper.h ../StorageBinList.h ../Parser.h \
  ../Phreeqc_class.h
Phreeqc.o: ../Phreeqc.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
  ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
  ../phreeqc/sundialstypes.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
  ../phreeqc/nvector.h ../phreeqc/dense.h ../phreeqc/smalldense.h \
  ../phreeqc/nvector_serial.h ../phreeqc/dense.h ../phreeqc/p2c.h \
  ../phreeqc/global_structures.h ../phreeqc/pitzer_structures.h \
  ../phreeqc/pitzer.h ../phreeqc/input.h ../phreeqc/output.h \
  ../phreeqc/global.h ../phreeqc/global_structures.h
runner.o: ../runner.cpp ../runner.h ../StorageBinList.h ../Parser.h \
  ../Phreeqc_class.h
StorageBinList.o: ../StorageBinList.cpp ../StorageBinList.h ../Parser.h \
  ../Phreeqc_class.h
#
#  PHREEQC files
#
advection.o: ../phreeqc/advection.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h
basic.o: ../phreeqc/basic.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../phreeqc/phqalloc.h \
  ../phreeqc/output.h ../phreeqc/phrqproto.h ../phreeqc/p2c.h \
  ../phreeqc/basic.h
basicsubs.o: ../phreeqc/basicsubs.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h
cl1.o: ../phreeqc/cl1.c ../phreeqc/phqalloc.h ../phreeqc/output.h \
  ../phreeqc/phrqtype.h
cl1mp.o: ../phreeqc/cl1mp.c ../phreeqc/phqalloc.h ../phreeqc/output.h \
  ../phreeqc/phrqtype.h
cvdense.o: ../phreeqc/cvdense.c ../phreeqc/cvdense.h ../phreeqc/cvode.h \
  ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
  ../phreeqc/dense.h ../phreeqc/smalldense.h ../phreeqc/sundialsmath.h \
  ../phreeqc/output.h ../phreeqc/phqalloc.h
cvode.o: ../phreeqc/cvode.c ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
  ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/sundialsmath.h \
  ../phreeqc/output.h ../phreeqc/kinetics.h ../phreeqc/phqalloc.h
dense.o: ../phreeqc/dense.c ../phreeqc/sundialstypes.h \
  ../phreeqc/phrqtype.h ../phreeqc/sundialsmath.h ../phreeqc/dense.h \
  ../phreeqc/smalldense.h ../phreeqc/output.h ../phreeqc/phqalloc.h
dw.o: ../phreeqc/dw.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../phreeqc/phrqproto.h \
  ../phreeqc/output.h ../phreeqc/pitzer.h
input.o: ../phreeqc/input.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../phreeqc/input.h ../phreeqc/output.h \
  ../phreeqc/phrqproto.h ../phreeqc/phqalloc.h
integrate.o: ../phreeqc/integrate.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h
inverse.o: ../phreeqc/inverse.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../phreeqc/phqalloc.h \
  ../phreeqc/output.h ../phreeqc/phrqproto.h
isotopes.o: ../phreeqc/isotopes.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h
kinetics.o: ../phreeqc/kinetics.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h \
  ../phreeqc/sundialstypes.h ../phreeqc/cvode.h ../phreeqc/nvector.h \
  ../phreeqc/cvdense.h ../phreeqc/dense.h ../phreeqc/smalldense.h \
  ../phreeqc/nvector_serial.h ../phreeqc/kinetics.h
main.o: ../phreeqc/main.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../phreeqc/output.h \
  ../phreeqc/phrqproto.h ../phreeqc/input.h
mainsubs.o: ../phreeqc/mainsubs.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h \
  ../phreeqc/input.h
model.o: ../phreeqc/model.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../phreeqc/phqalloc.h \
  ../phreeqc/output.h ../phreeqc/phrqproto.h
nvector.o: ../phreeqc/nvector.c ../phreeqc/nvector.h \
  ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/output.h
nvector_serial.o: ../phreeqc/nvector_serial.c ../phreeqc/nvector_serial.h \
  ../phreeqc/nvector.h ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h \
  ../phreeqc/sundialsmath.h ../phreeqc/output.h ../phreeqc/phqalloc.h
output.o: ../phreeqc/output.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../phreeqc/output.h \
  ../phreeqc/phrqproto.h ../phreeqc/phqalloc.h
p2clib.o: ../phreeqc/p2clib.c ../phreeqc/p2c.h ../phreeqc/output.h
parse.o: ../phreeqc/parse.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../phreeqc/phqalloc.h \
  ../phreeqc/output.h ../phreeqc/phrqproto.h
phqalloc.o: ../phreeqc/phqalloc.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h \
  ../phreeqc/output.h
phreeqc_files.o: ../phreeqc/phreeqc_files.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h \
  ../phreeqc/input.h
pitzer.o: ../phreeqc/pitzer.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../phreeqc/phqalloc.h \
  ../phreeqc/output.h ../phreeqc/phrqproto.h \
  ../phreeqc/pitzer_structures.h ../phreeqc/pitzer.h
pitzer_structures.o: ../phreeqc/pitzer_structures.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h \
  ../phreeqc/pitzer_structures.h ../phreeqc/pitzer.h
prep.o: ../phreeqc/prep.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../phreeqc/phqalloc.h \
  ../phreeqc/output.h ../phreeqc/phrqproto.h
print.o: ../phreeqc/print.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../phreeqc/phqalloc.h \
  ../phreeqc/output.h ../phreeqc/phrqproto.h ../phreeqc/pitzer.h
read.o: ../phreeqc/read.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../phreeqc/phqalloc.h \
  ../phreeqc/output.h ../phreeqc/phrqproto.h
readtr.o: ../phreeqc/readtr.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../phreeqc/phqalloc.h \
  ../phreeqc/output.h ../phreeqc/phrqproto.h
sit.o: ../phreeqc/sit.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../phreeqc/phqalloc.h \
  ../phreeqc/output.h ../phreeqc/phrqproto.h \
  ../phreeqc/pitzer_structures.h ../phreeqc/pitzer.h
smalldense.o: ../phreeqc/smalldense.c ../phreeqc/smalldense.h \
  ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h \
  ../phreeqc/sundialsmath.h ../phreeqc/output.h ../phreeqc/phqalloc.h
spread.o: ../phreeqc/spread.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../phreeqc/phqalloc.h \
  ../phreeqc/output.h ../phreeqc/phrqproto.h
step.o: ../phreeqc/step.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../phreeqc/phqalloc.h \
  ../phreeqc/output.h ../phreeqc/phrqproto.h
structures.o: ../phreeqc/structures.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h
sundialsmath.o: ../phreeqc/sundialsmath.c ../phreeqc/sundialsmath.h \
  ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/output.h
tally.o: ../phreeqc/tally.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../phreeqc/phqalloc.h \
  ../phreeqc/output.h ../phreeqc/phrqproto.h
tidy.o: ../phreeqc/tidy.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/global_structures.h ../phreeqc/phqalloc.h \
  ../phreeqc/output.h ../phreeqc/phrqproto.h
transport.o: ../phreeqc/transport.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h
utilities.o: ../phreeqc/utilities.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/global_structures.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h
# -----------------------------------------------------------------------------

clean:
	rm -rf Release Debug Class_release Class_debug

dependencies:
	mkdir -p $(DEBUG_DIR) 
	cd $(DEBUG_DIR); gcc -MM -I../phreeqc ../*.cxx ../*.cpp ../phreeqc/*.c

tester:
	cd ../mytest; make clean; make -k $(SPOOL) make.out $(SPOOL2); make zero; make diff $(SPOOL) diff.out $(SPOOL2)
	cd ../examples; make clean; make $(SPOOL) make.out $(SPOOL2); make zero; make diff $(SPOOL) diff.out $(SPOOL2)
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
