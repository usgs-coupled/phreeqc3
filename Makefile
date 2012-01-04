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

all: class_release class_debug

Debug: class_debug
debug: class_debug
Class_debug: class_debug

Release: class_release
release: class_release
Class_release: class_release


CLASS_DEBUG_DIR         = Class_debug
CLASS_DIR              = Class_release
MAKEFILE                = Makefile

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
#	CL1MP_LIB=-lgmp
	CL1MP_LIB=/z/parkplace/usr/lib/libgmp.a 
endif

# -----------------------------------------------------------------------------
#efence for debugging
EFENCE_LIB=-L$(HOME)/packages/efence

# -----------------------------------------------------------------------------
# 2 Versions
# -----------------------------------------------------------------------------
ifeq ($(CFG), CLASS_DEBUG)
  DEFINES      = -DUSE_PHRQ_ALLOC $(DEFINE_INVERSE_CL1MP) 
  VPATH        = ..:../phreeqc
  INCLUDES     = -I../phreeqc -I..
  CXX          = g++
  CXXFLAGS     = -Wall -g $(DEFINES) $(INCLUDES)
  OBJECT_FILES = $(CLASS_FILES) $(COMMON_COBJS) $(COMMON_CXXOBJS) $(CL1MP_OBJS)
  LD_FLAGS     = -lm ${CL1MP_LIB} ${HASH_STYLE}
endif

ifeq ($(CFG), CLASS_RELEASE)
  DEFINES      = -DNDEBUG $(DEFINE_INVERSE_CL1MP) 
  VPATH        = ..:../phreeqc
  INCLUDES     = -I../phreeqc -I..
  CXX          = g++
  CXXFLAGS     = -Wall -pedantic -O3 $(DEFINES) $(INCLUDES)
#  CXXFLAGS     = -Wall -pedantic -p $(DEFINES) $(INCLUDES)
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
#	${CXX} -p -o $@ ${OBJECT_FILES}  ${LD_FLAGS} 
	${CXX} -o $@ ${OBJECT_FILES}  ${LD_FLAGS} 
	@echo; echo Done making for phreeqcpp
# -----------------------------------------------------------------------------
# Module dependency list
# -----------------------------------------------------------------------------
#
#  CXX files
#
cxxKinetics.o: ../cxxKinetics.cxx ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../cxxKinetics.h ../KineticsComp.h ../NameDouble.h ../phreeqc/phqalloc.h
cxxMix.o: ../cxxMix.cxx ../Utils.h ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../phreeqc/phqalloc.h
Exchange.o: ../Exchange.cxx ../Utils.h ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../Exchange.h ../ExchComp.h ../NameDouble.h ../phreeqc/phqalloc.h
ExchComp.o: ../ExchComp.cxx ../Utils.h ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../ExchComp.h ../NameDouble.h ../phreeqc/phqalloc.h
GasComp.o: ../GasComp.cxx ../Utils.h ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../GasComp.h ../NameDouble.h ../phreeqc/phqalloc.h
GasPhase.o: ../GasPhase.cxx ../Utils.h ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../GasPhase.h ../NameDouble.h ../Phreeqc_class.h ../GasComp.h \
 ../phreeqc/phqalloc.h
ISolutionComp.o: ../ISolutionComp.cxx ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../ISolutionComp.h ../phreeqc/phqalloc.h
ISolution.o: ../ISolution.cxx ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../ISolution.h ../ISolutionComp.h ../Solution.h ../SolutionIsotopeList.h \
 ../SolutionIsotope.h ../NameDouble.h ../Phreeqc_class.h \
 ../phreeqc/phqalloc.h
KineticsComp.o: ../KineticsComp.cxx ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../KineticsComp.h ../NameDouble.h ../phreeqc/phqalloc.h
NameDouble.o: ../NameDouble.cxx ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../NameDouble.h ../phreeqc/phqalloc.h
NumKeyword.o: ../NumKeyword.cxx ../NumKeyword.h ../PHRQ_base.h \
 ../Parser.h ../Keywords.h ../PHRQ_io.h
Parser.o: ../Parser.cxx ../Utils.h ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h
PHRQ_base.o: ../PHRQ_base.cxx ../PHRQ_base.h ../PHRQ_io.h ../Keywords.h
PPassemblageComp.o: ../PPassemblageComp.cxx ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../PPassemblageComp.h ../NameDouble.h ../Phreeqc_class.h \
 ../phreeqc/phqalloc.h
PPassemblage.o: ../PPassemblage.cxx ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../PPassemblage.h ../PPassemblageComp.h ../NameDouble.h \
 ../Phreeqc_class.h ../phreeqc/phqalloc.h
Pressure.o: ../Pressure.cxx ../Utils.h ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../phreeqc/phqalloc.h
Reaction.o: ../Reaction.cxx ../Utils.h ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../Reaction.h ../NameDouble.h ../phreeqc/phqalloc.h
ReadClass.o: ../ReadClass.cxx ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../Phreeqc_class.h ../Solution.h ../SolutionIsotopeList.h \
 ../SolutionIsotope.h ../NameDouble.h ../Exchange.h ../ExchComp.h \
 ../Surface.h ../SurfaceComp.h ../SurfaceCharge.h ../PPassemblage.h \
 ../PPassemblageComp.h ../cxxKinetics.h ../KineticsComp.h \
 ../SSassemblage.h ../GasPhase.h ../GasComp.h ../Reaction.h \
 ../Temperature.h ../phreeqc/phqalloc.h
Solution.o: ../Solution.cxx ../Utils.h ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../Solution.h ../SolutionIsotopeList.h ../SolutionIsotope.h \
 ../NameDouble.h ../Phreeqc_class.h ../phreeqc/phqalloc.h
SolutionIsotope.o: ../SolutionIsotope.cxx ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../SolutionIsotope.h ../phreeqc/phqalloc.h
SolutionIsotopeList.o: ../SolutionIsotopeList.cxx ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../SolutionIsotopeList.h ../SolutionIsotope.h ../phreeqc/phqalloc.h
SSassemblage.o: ../SSassemblage.cxx ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../SSassemblage.h ../NameDouble.h ../Phreeqc_class.h ../SSassemblageSS.h \
 ../phreeqc/phqalloc.h
SSassemblageSS.o: ../SSassemblageSS.cxx ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../SSassemblageSS.h ../NameDouble.h ../Phreeqc_class.h \
 ../phreeqc/phqalloc.h
StorageBin.o: ../StorageBin.cxx ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../NameDouble.h ../StorageBin.h ../System.h ../Phreeqc_class.h \
 ../SSassemblage.h ../Solution.h ../SolutionIsotopeList.h \
 ../SolutionIsotope.h ../Exchange.h ../ExchComp.h ../GasPhase.h \
 ../GasComp.h ../cxxKinetics.h ../KineticsComp.h ../PPassemblage.h \
 ../PPassemblageComp.h ../SSassemblageSS.h ../Surface.h ../SurfaceComp.h \
 ../SurfaceCharge.h ../Reaction.h ../Temperature.h ../phreeqc/phqalloc.h
SurfaceCharge.o: ../SurfaceCharge.cxx ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../SurfaceCharge.h ../NameDouble.h ../Phreeqc_class.h \
 ../phreeqc/phqalloc.h
SurfaceComp.o: ../SurfaceComp.cxx ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../SurfaceComp.h ../Phreeqc_class.h ../NameDouble.h \
 ../phreeqc/phqalloc.h
Surface.o: ../Surface.cxx ../Phreeqc_class.h ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h ../Surface.h \
 ../SurfaceComp.h ../NameDouble.h ../SurfaceCharge.h \
 ../phreeqc/phqalloc.h
System.o: ../System.cxx ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h ../System.h \
 ../NameDouble.h ../Parser.h ../Phreeqc_class.h ../SSassemblage.h \
 ../Solution.h ../SolutionIsotopeList.h ../SolutionIsotope.h \
 ../Exchange.h ../ExchComp.h ../GasPhase.h ../GasComp.h ../cxxKinetics.h \
 ../KineticsComp.h ../PPassemblage.h ../PPassemblageComp.h \
 ../SSassemblageSS.h ../Surface.h ../SurfaceComp.h ../SurfaceCharge.h \
 ../Reaction.h ../Temperature.h
Temperature.o: ../Temperature.cxx ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../Temperature.h ../phreeqc/phqalloc.h
Utils.o: ../Utils.cxx ../Utils.h ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h
Calculate_value.o: ../Calculate_value.cpp ../Calculate_value.h \
 ../PHRQ_base.h ../PHRQ_io.h ../Keywords.h ../phreeqc/phrqtype.h
Calculate_values\ .o: ../Calculate_values\ .cpp ../Calculate_values.h \
 ../PHRQ_base.h ../PHRQ_io.h ../Keywords.h ../Parser.h \
 ../phreeqc/phrqtype.h ../phreeqc/global_structures.h ../phreeqc/NA.h \
 ../PBasic.h
Calculate_values.o: ../Calculate_values.cpp ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Calculate_values.h \
 ../Calculate_value.h ../Parser.h ../PBasic.h
ChartHandler.o: ../ChartHandler.cpp
ChartObject.o: ../ChartObject.cpp
class_main.o: ../class_main.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h \
 ../NameDouble.h ../Parser.h ../Solution.h ../SolutionIsotopeList.h \
 ../SolutionIsotope.h ../Phreeqc_class.h ../Reaction.h ../PPassemblage.h \
 ../PPassemblageComp.h ../Exchange.h ../ExchComp.h ../Surface.h \
 ../SurfaceComp.h ../SurfaceCharge.h ../GasPhase.h ../GasComp.h \
 ../SSassemblage.h ../cxxKinetics.h ../KineticsComp.h
CurveObject.o: ../CurveObject.cpp ../CurveObject.h
dumper.o: ../dumper.cpp ../dumper.h ../StorageBinList.h ../PHRQ_base.h \
 ../Parser.h ../Keywords.h ../PHRQ_io.h
Keywords.o: ../Keywords.cpp ../Keywords.h
Logk.o: ../Logk.cpp ../Logk.h ../phreeqc/phrqtype.h
PBasic.o: ../PBasic.cpp ../PBasic.h ../phreeqc/phrqtype.h ../PHRQ_base.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Phreeqc.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../dumper.h \
 ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/phqalloc.h ../NameDouble.h ../Parser.h
Phreeqc.o: ../Phreeqc.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h \
 ../NameDouble.h ../Parser.h ../Solution.h ../SolutionIsotopeList.h \
 ../SolutionIsotope.h ../Phreeqc_class.h ../Reaction.h ../PPassemblage.h \
 ../PPassemblageComp.h ../Exchange.h ../ExchComp.h ../Surface.h \
 ../SurfaceComp.h ../SurfaceCharge.h ../GasPhase.h ../GasComp.h \
 ../SSassemblage.h ../cxxKinetics.h ../KineticsComp.h \
 ../phreeqc/phqalloc.h ../PBasic.h ../Temperature.h
PHRQ_io.o: ../PHRQ_io.cpp ../Utils.h ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h
runner.o: ../runner.cpp ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../Parser.h ../Keywords.h ../PHRQ_io.h ../phreeqc/NA.h
StorageBinList.o: ../StorageBinList.cpp ../StorageBinList.h \
 ../PHRQ_base.h ../Parser.h ../Keywords.h ../PHRQ_io.h
advection.o: ../phreeqc/advection.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h \
 ../phreeqc/phqalloc.h
basicsubs.o: ../phreeqc/basicsubs.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h \
 ../phreeqc/phqalloc.h ../phreeqc/../NameDouble.h ../phreeqc/../Parser.h \
 ../phreeqc/../PHRQ_base.h ../phreeqc/../Keywords.h \
 ../phreeqc/../PHRQ_io.h ../phreeqc/../PBasic.h ../phreeqc/../Exchange.h \
 ../phreeqc/../NumKeyword.h ../phreeqc/../ExchComp.h \
 ../phreeqc/../NameDouble.h ../phreeqc/../GasPhase.h \
 ../phreeqc/../Phreeqc_class.h ../phreeqc/../GasComp.h
ChartObject.o: ../phreeqc/ChartObject.cpp
cl1.o: ../phreeqc/cl1.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h \
 ../phreeqc/phqalloc.h
cl1mp.o: ../phreeqc/cl1mp.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h \
 ../phreeqc/phqalloc.h
cvdense.o: ../phreeqc/cvdense.cpp ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../phreeqc/sundialsmath.h \
 ../Phreeqc.h ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../phreeqc/phqalloc.h
cvode.o: ../phreeqc/cvode.cpp ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/sundialsmath.h ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/dense.h ../phreeqc/smalldense.h \
 ../runner.h ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../phreeqc/phqalloc.h
dense.o: ../phreeqc/dense.cpp ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/sundialsmath.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h
dw.o: ../phreeqc/dw.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h
gases.o: ../phreeqc/gases.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h ../GasPhase.h \
 ../NameDouble.h ../Parser.h ../Phreeqc_class.h ../GasComp.h
input.o: ../phreeqc/input.cpp ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h ../Phreeqc.h \
 ../phreeqc/phqalloc.h
integrate.o: ../phreeqc/integrate.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h \
 ../phreeqc/phqalloc.h
inverse.o: ../phreeqc/inverse.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h \
 ../phreeqc/phqalloc.h
isotopes.o: ../phreeqc/isotopes.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h \
 ../phreeqc/phqalloc.h
kinetics.o: ../phreeqc/kinetics.cpp ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h ../Phreeqc.h \
 ../phreeqc/phqalloc.h ../StorageBin.h ../System.h ../NameDouble.h \
 ../Phreeqc_class.h ../Reaction.h ../cxxKinetics.h ../KineticsComp.h \
 ../Solution.h ../SolutionIsotopeList.h ../SolutionIsotope.h ../cxxMix.h \
 ../PPassemblage.h ../PPassemblageComp.h ../Surface.h ../SurfaceComp.h \
 ../SurfaceCharge.h ../Exchange.h ../ExchComp.h ../GasPhase.h \
 ../GasComp.h ../SSassemblage.h ../Temperature.h \
 ../phreeqc/nvector_serial.h
mainsubs.o: ../phreeqc/mainsubs.cpp ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h ../Phreeqc.h \
 ../phreeqc/phqalloc.h ../PBasic.h ../Temperature.h ../Exchange.h \
 ../ExchComp.h ../NameDouble.h ../ExchComp.h ../GasPhase.h \
 ../Phreeqc_class.h ../GasComp.h ../Reaction.h
model_aw_fraction.o: ../phreeqc/model_aw_fraction.cpp ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../phreeqc/phqalloc.h
model.o: ../phreeqc/model.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h \
 ../phreeqc/phqalloc.h ../cxxMix.h ../Exchange.h ../ExchComp.h \
 ../NameDouble.h ../Parser.h ../GasPhase.h ../Phreeqc_class.h \
 ../GasComp.h
model\ with\ ah2o.o: ../phreeqc/model\ with\ ah2o.cpp ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../phreeqc/phqalloc.h
nvector.o: ../phreeqc/nvector.cpp ../phreeqc/nvector.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h
nvector_serial.o: ../phreeqc/nvector_serial.cpp \
 ../phreeqc/nvector_serial.h ../phreeqc/nvector.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h \
 ../phreeqc/sundialsmath.h
parse.o: ../phreeqc/parse.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h \
 ../phreeqc/phqalloc.h
phqalloc.o: ../phreeqc/phqalloc.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h
PHRQ_io_output.o: ../phreeqc/PHRQ_io_output.cpp ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../phreeqc/phqalloc.h
pitzer.o: ../phreeqc/pitzer.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h \
 ../phreeqc/phqalloc.h ../Exchange.h ../ExchComp.h ../NameDouble.h \
 ../Parser.h
pitzer_structures.o: ../phreeqc/pitzer_structures.cpp ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../phreeqc/phqalloc.h
prep.o: ../phreeqc/prep.cpp ../Utils.h ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../Phreeqc.h ../phreeqc/phqalloc.h ../Exchange.h ../ExchComp.h \
 ../NameDouble.h ../GasPhase.h ../Phreeqc_class.h ../GasComp.h
print.o: ../phreeqc/print.cpp ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h ../Phreeqc.h \
 ../phreeqc/phqalloc.h ../Temperature.h ../cxxMix.h ../Exchange.h \
 ../ExchComp.h ../NameDouble.h ../GasPhase.h ../Phreeqc_class.h \
 ../GasComp.h ../Reaction.h
read.o: ../phreeqc/read.cpp ../Utils.h ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../Phreeqc.h ../phreeqc/phqalloc.h ../Pressure.h ../Temperature.h \
 ../Parser.h ../cxxMix.h ../Exchange.h ../ExchComp.h ../NameDouble.h \
 ../GasPhase.h ../Phreeqc_class.h ../GasComp.h ../Reaction.h
readtr.o: ../phreeqc/readtr.cpp ../phreeqc/../StorageBin.h \
 ../phreeqc/../System.h ../phreeqc/../NameDouble.h ../phreeqc/../Parser.h \
 ../phreeqc/../PHRQ_base.h ../phreeqc/../Keywords.h \
 ../phreeqc/../PHRQ_io.h ../phreeqc/phrqtype.h \
 ../phreeqc/../Phreeqc_class.h ../phreeqc/../SSassemblageSS.h \
 ../Phreeqc.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../phreeqc/phqalloc.h
sit.o: ../phreeqc/sit.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h \
 ../phreeqc/phqalloc.h ../Exchange.h ../ExchComp.h ../NameDouble.h \
 ../Parser.h
smalldense.o: ../phreeqc/smalldense.cpp ../phreeqc/smalldense.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h \
 ../phreeqc/sundialsmath.h
spread.o: ../phreeqc/spread.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h \
 ../phreeqc/phqalloc.h
step.o: ../phreeqc/step.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h \
 ../phreeqc/phqalloc.h ../StorageBin.h ../System.h ../NameDouble.h \
 ../Parser.h ../Phreeqc_class.h ../Solution.h ../SolutionIsotopeList.h \
 ../SolutionIsotope.h ../PPassemblage.h ../PPassemblageComp.h \
 ../SSassemblage.h ../SSassemblageSS.h ../NameDouble.h ../Temperature.h \
 ../cxxMix.h ../Exchange.h ../ExchComp.h ../GasPhase.h ../GasComp.h \
 ../Reaction.h
structures.o: ../phreeqc/structures.cpp ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h ../Phreeqc.h \
 ../phreeqc/phqalloc.h ../Temperature.h ../cxxMix.h ../Exchange.h \
 ../ExchComp.h ../NameDouble.h ../GasPhase.h ../Phreeqc_class.h \
 ../GasComp.h ../Reaction.h ../phreeqc/../cxxKinetics.h \
 ../phreeqc/../NumKeyword.h ../phreeqc/../KineticsComp.h \
 ../phreeqc/../NameDouble.h ../phreeqc/../PHRQ_base.h \
 ../phreeqc/../PPassemblage.h ../phreeqc/../PPassemblageComp.h \
 ../phreeqc/../Phreeqc_class.h ../phreeqc/../PPassemblageComp.h \
 ../phreeqc/../Solution.h ../phreeqc/../SolutionIsotopeList.h \
 ../phreeqc/../SolutionIsotope.h ../phreeqc/../Parser.h \
 ../phreeqc/../PHRQ_io.h ../phreeqc/../SolutionIsotopeList.h \
 ../phreeqc/../SSassemblage.h ../phreeqc/../SSassemblageSS.h \
 ../phreeqc/../Surface.h ../phreeqc/../SurfaceComp.h \
 ../phreeqc/../SurfaceCharge.h ../phreeqc/../SurfaceComp.h \
 ../phreeqc/../SurfaceCharge.h ../phreeqc/../StorageBin.h \
 ../phreeqc/../System.h
sundialsmath.o: ../phreeqc/sundialsmath.cpp ../phreeqc/sundialsmath.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h
tally.o: ../phreeqc/tally.cpp ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h ../Phreeqc.h \
 ../phreeqc/phqalloc.h ../Temperature.h ../Exchange.h ../ExchComp.h \
 ../NameDouble.h ../GasPhase.h ../Phreeqc_class.h ../GasComp.h \
 ../Reaction.h
tidy.o: ../phreeqc/tidy.cpp ../Utils.h ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h \
 ../Phreeqc.h ../phreeqc/phqalloc.h ../Exchange.h ../ExchComp.h \
 ../NameDouble.h ../GasPhase.h ../Phreeqc_class.h ../GasComp.h
transport.o: ../phreeqc/transport.cpp ../Utils.h ../Phreeqc.h \
 ../phreeqc/phrqtype.h ../phreeqc/cvdense.h ../phreeqc/cvode.h \
 ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/nvector.h \
 ../phreeqc/dense.h ../phreeqc/smalldense.h ../runner.h \
 ../StorageBinList.h ../PHRQ_base.h ../dumper.h ../PHRQ_io.h \
 ../Keywords.h ../Pressure.h ../NumKeyword.h ../cxxMix.h \
 ../phreeqc/global_structures.h ../phreeqc/NA.h ../Parser.h ../Phreeqc.h \
 ../phreeqc/phqalloc.h ../Exchange.h ../ExchComp.h ../NameDouble.h \
 ../GasPhase.h ../Phreeqc_class.h ../GasComp.h
utilities.o: ../phreeqc/utilities.cpp ../Phreeqc.h ../phreeqc/phrqtype.h \
 ../phreeqc/cvdense.h ../phreeqc/cvode.h ../phreeqc/sundialstypes.h \
 ../phreeqc/phrqtype.h ../phreeqc/nvector.h ../phreeqc/dense.h \
 ../phreeqc/smalldense.h ../runner.h ../StorageBinList.h ../PHRQ_base.h \
 ../dumper.h ../PHRQ_io.h ../Keywords.h ../Pressure.h ../NumKeyword.h \
 ../cxxMix.h ../phreeqc/global_structures.h ../phreeqc/NA.h \
 ../phreeqc/phqalloc.h ../NameDouble.h ../Parser.h ../Exchange.h \
 ../ExchComp.h ../NameDouble.h
# -----------------------------------------------------------------------------
clean:
	rm -rf Class_release Class_debug

dependencies:
	mkdir -p $(CLASS_DEBUG_DIR) 
	cd $(CLASS_DEBUG_DIR); gcc -MM -I.. -I../phreeqc ../*.cxx ../*.cpp ../phreeqc/*.cpp

tester:
	cd ../mytest; make clean; make -k $(SPOOL) make.out $(SPOOL2); make diff $(SPOOL) diff.out $(SPOOL2)
	cd ../examples; make clean; make $(SPOOL) make.out $(SPOOL2); make diff $(SPOOL) diff.out $(SPOOL2)
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
