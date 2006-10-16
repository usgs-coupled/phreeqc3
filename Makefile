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

all: release debug

RELEASE_DIR             = Release
DEBUG_DIR               = Debug
MAKEFILE                = Makefile

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

# Recursive make begins here
#
# =============================================================================
# Significant suffixes [assuming Fortran 90 (.f90)  source code]:
# Significant suffixes [assuming Fortran 90 (.F90)  source code, needs to be preprocessed ]:
# =============================================================================

#SRC     = ../phreeqc
.SUFFIXES : .o .c .cxx

# compilers

# -----------------------------------------------------------------------------
.c.o :
	${CXX} ${CXXFLAGS} -c -o $@ $<

.cxx.o :
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
DINVERSE_CL1MP=TRUE
CL1MP_LIB=/z/parkplace/usr/lib/libgmp.a 
CL1MP_OBJS=cl1mp.o

# -----------------------------------------------------------------------------
#efence for debugging
EFENCE_LIB=-L$(HOME)/packages/efence

# -----------------------------------------------------------------------------
# 2 Versions
# -----------------------------------------------------------------------------
ifeq ($(CFG), RELEASE)
  DEFINES      = -DPHREEQC_CPP -DINVERSE_CL1MP
  VPATH        = ..:../phreeqc
  INCLUDES     = -I../phreeqc
  CXX          = g++
  CXXFLAGS     = -Wall -pedantic -O3 $(DEFINES) $(INCLUDES)
  CXXFLAGS_MODEL = -Wall -pedantic -O2 $(DEFINES) $(INCLUDES)
  OBJECT_FILES = $(COMMON_COBJS) $(COMMON_CXXOBJS) $(CL1MP_OBJS)
  LD_FLAGS     = -lm ${CL1MP_LIB} 
endif

ifeq ($(CFG), DEBUG)
  DEFINES      = -DPHREEQC_CPP -DINVERSE_CL1MP
  VPATH        = ..:../phreeqc
  INCLUDES     = -I../phreeqc
  CXX          = g++
  CXXFLAGS     = -Wall -g $(DEFINES) $(INCLUDES)
  CXXFLAGS_MODEL = -Wall -g $(DEFINES) $(INCLUDES)
  OBJECT_FILES = $(COMMON_COBJS) $(COMMON_CXXOBJS) $(CL1MP_OBJS)
  LD_FLAGS     = -lm ${CL1MP_LIB} 
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
	       main.o \
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
	       smalldense.o \
	       spread.o \
	       step.o \
	       structures.o \
	       sundialsmath.o \
	       tally.o \
	       tidy.o \
	       transport.o \
	       utilities.o 	                

COMMON_CXXOBJS = \
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
	       Solution.o \
	       SolutionIsotope.o \
	       SolutionIsotopeList.o \
	       SSassemblage.o \
	       SSassemblageSS.o \
               StorageBin.o \
	       Surface.o \
	       SurfaceCharge.o \
	       SurfaceComp.o \
	       Temperature.o \
	       Utils.o 

# -----------------------------------------------------------------------------
#  Assign dependents to target on dependency line & link options on command 
#  line. Command line is initiated with a tab. ($@ is an internal macro for 
#  the current target.) 

${PROGRAM} : ${OBJECT_FILES}
	${CXX} -o $@ ${OBJECT_FILES}  ${LD_FLAGS} 
	@echo; echo Done making for phast
# -----------------------------------------------------------------------------
# Module dependency list
# -----------------------------------------------------------------------------
#
#  CXX files
#
cxxKinetics.o: ../cxxKinetics.cxx ../Utils.h ../char_star.h \
  ../cxxKinetics.h ../NumKeyword.h ../Parser.h ../char_star.h \
  ../phreeqc/global.h ../phreeqc/phrqtype.h ../char_star.h \
  ../KineticsComp.h ../NameDouble.h ../char_star.h ../Parser.h \
  ../char_star.h ../KineticsComp.h ../phreeqc/phqalloc.h \
  ../phreeqc/phrqproto.h
cxxMix.o: ../cxxMix.cxx ../Utils.h ../char_star.h ../cxxMix.h \
  ../NumKeyword.h ../Parser.h ../char_star.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../char_star.h ../phreeqc/phqalloc.h \
  ../phreeqc/phrqproto.h
Dictionary.o: ../Dictionary.cxx ../Dictionary.h ../Solution.h \
  ../NumKeyword.h ../Parser.h ../char_star.h ../SolutionIsotopeList.h \
  ../SolutionIsotope.h ../Parser.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../char_star.h ../Parser.h ../NameDouble.h \
  ../char_star.h ../Parser.h ../cxxMix.h ../NumKeyword.h ../char_star.h \
  ../char_star.h ../NameDouble.h ../phreeqc/phqalloc.h \
  ../phreeqc/phrqproto.h ../phreeqc/output.h
EntityMap.o: ../EntityMap.cxx ../Utils.h ../char_star.h ../EntityMap.h \
  ../phreeqc/global.h ../phreeqc/phrqtype.h ../char_star.h ../Parser.h \
  ../char_star.h ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h
Exchange.o: ../Exchange.cxx ../Utils.h ../char_star.h ../Exchange.h \
  ../NumKeyword.h ../Parser.h ../char_star.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../char_star.h ../ExchComp.h ../NameDouble.h \
  ../char_star.h ../Parser.h ../char_star.h ../ExchComp.h \
  ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h
ExchComp.o: ../ExchComp.cxx ../Utils.h ../char_star.h ../ExchComp.h \
  ../NameDouble.h ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../char_star.h ../Parser.h ../char_star.h ../char_star.h \
  ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h ../phreeqc/output.h
GasPhase.o: ../GasPhase.cxx ../Utils.h ../char_star.h ../GasPhase.h \
  ../NumKeyword.h ../Parser.h ../char_star.h ../NameDouble.h \
  ../phreeqc/global.h ../phreeqc/phrqtype.h ../char_star.h ../Parser.h \
  ../char_star.h ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h
ISolutionComp.o: ../ISolutionComp.cxx ../ISolutionComp.h ../Utils.h \
  ../char_star.h ../char_star.h ../ISolution.h ../ISolutionComp.h \
  ../NumKeyword.h ../Parser.h ../char_star.h ../Solution.h \
  ../NumKeyword.h ../SolutionIsotopeList.h ../SolutionIsotope.h \
  ../Parser.h ../phreeqc/global.h ../phreeqc/phrqtype.h ../char_star.h \
  ../Parser.h ../NameDouble.h ../char_star.h ../Parser.h ../cxxMix.h \
  ../NumKeyword.h ../char_star.h ../char_star.h ../Utils.h \
  ../phreeqc/phrqproto.h ../phreeqc/phqalloc.h
ISolution.o: ../ISolution.cxx ../ISolution.h ../ISolutionComp.h \
  ../Utils.h ../char_star.h ../char_star.h ../NumKeyword.h ../Parser.h \
  ../char_star.h ../Solution.h ../NumKeyword.h ../SolutionIsotopeList.h \
  ../SolutionIsotope.h ../Parser.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../char_star.h ../Parser.h ../NameDouble.h \
  ../char_star.h ../Parser.h ../cxxMix.h ../NumKeyword.h ../char_star.h \
  ../char_star.h ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h
KineticsComp.o: ../KineticsComp.cxx ../Utils.h ../char_star.h \
  ../KineticsComp.h ../NameDouble.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../char_star.h ../Parser.h ../char_star.h \
  ../char_star.h ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h
NameDouble.o: ../NameDouble.cxx ../Utils.h ../char_star.h ../NameDouble.h \
  ../phreeqc/global.h ../phreeqc/phrqtype.h ../char_star.h ../Parser.h \
  ../char_star.h ../phreeqc/output.h ../phreeqc/phqalloc.h \
  ../phreeqc/phrqproto.h
NumKeyword.o: ../NumKeyword.cxx ../NumKeyword.h ../Parser.h \
  ../char_star.h
Parser.o: ../Parser.cxx ../Parser.h ../char_star.h ../Utils.h \
  ../char_star.h ../char_star.h
PPassemblageComp.o: ../PPassemblageComp.cxx ../Utils.h ../char_star.h \
  ../PPassemblageComp.h ../NameDouble.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../char_star.h ../Parser.h ../char_star.h \
  ../char_star.h ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h
PPassemblage.o: ../PPassemblage.cxx ../Utils.h ../char_star.h \
  ../PPassemblage.h ../NumKeyword.h ../Parser.h ../char_star.h \
  ../phreeqc/global.h ../phreeqc/phrqtype.h ../char_star.h \
  ../PPassemblageComp.h ../NameDouble.h ../char_star.h ../Parser.h \
  ../char_star.h ../PPassemblageComp.h ../phreeqc/phqalloc.h \
  ../phreeqc/phrqproto.h
Reaction.o: ../Reaction.cxx ../Utils.h ../char_star.h ../Reaction.h \
  ../NumKeyword.h ../Parser.h ../char_star.h ../NameDouble.h \
  ../phreeqc/global.h ../phreeqc/phrqtype.h ../char_star.h ../Parser.h \
  ../char_star.h ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h
ReadClass.o: ../ReadClass.cxx ../Parser.h ../char_star.h ../Solution.h \
  ../NumKeyword.h ../Parser.h ../SolutionIsotopeList.h \
  ../SolutionIsotope.h ../Parser.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../char_star.h ../Parser.h ../NameDouble.h \
  ../char_star.h ../Parser.h ../cxxMix.h ../NumKeyword.h ../char_star.h \
  ../char_star.h ../Exchange.h ../NumKeyword.h ../char_star.h \
  ../ExchComp.h ../NameDouble.h ../char_star.h ../Surface.h \
  ../NumKeyword.h ../char_star.h ../SurfaceComp.h ../NameDouble.h \
  ../char_star.h ../SurfaceCharge.h ../NameDouble.h ../char_star.h \
  ../PPassemblage.h ../NumKeyword.h ../char_star.h ../PPassemblageComp.h \
  ../NameDouble.h ../char_star.h ../cxxKinetics.h ../NumKeyword.h \
  ../char_star.h ../KineticsComp.h ../NameDouble.h ../char_star.h \
  ../SSassemblage.h ../NumKeyword.h ../char_star.h ../SSassemblageSS.h \
  ../NameDouble.h ../char_star.h ../GasPhase.h ../NumKeyword.h \
  ../NameDouble.h ../char_star.h ../Reaction.h ../NumKeyword.h \
  ../NameDouble.h ../char_star.h ../cxxMix.h ../Temperature.h \
  ../NumKeyword.h ../char_star.h ../phreeqc/phqalloc.h \
  ../phreeqc/output.h ../phreeqc/phrqproto.h
Solution.o: ../Solution.cxx ../Utils.h ../char_star.h ../Solution.h \
  ../NumKeyword.h ../Parser.h ../char_star.h ../SolutionIsotopeList.h \
  ../SolutionIsotope.h ../Parser.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../char_star.h ../Parser.h ../NameDouble.h \
  ../char_star.h ../Parser.h ../cxxMix.h ../NumKeyword.h ../char_star.h \
  ../char_star.h ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h \
  ../phreeqc/output.h ../ISolution.h ../ISolutionComp.h ../Utils.h \
  ../char_star.h ../NumKeyword.h ../Solution.h ../Exchange.h \
  ../NumKeyword.h ../char_star.h ../ExchComp.h ../NameDouble.h \
  ../char_star.h ../Surface.h ../NumKeyword.h ../char_star.h \
  ../SurfaceComp.h ../NameDouble.h ../char_star.h ../SurfaceCharge.h \
  ../NameDouble.h ../char_star.h ../PPassemblage.h ../NumKeyword.h \
  ../char_star.h ../PPassemblageComp.h ../NameDouble.h ../char_star.h \
  ../cxxKinetics.h ../NumKeyword.h ../char_star.h ../KineticsComp.h \
  ../NameDouble.h ../char_star.h ../SSassemblage.h ../NumKeyword.h \
  ../char_star.h ../SSassemblageSS.h ../NameDouble.h ../char_star.h \
  ../GasPhase.h ../NumKeyword.h ../NameDouble.h ../char_star.h \
  ../Reaction.h ../NumKeyword.h ../NameDouble.h ../char_star.h \
  ../Temperature.h ../NumKeyword.h ../char_star.h ../StorageBin.h \
  ../Utils.h ../Parser.h ../Solution.h ../Exchange.h ../GasPhase.h \
  ../cxxKinetics.h ../PPassemblage.h ../SSassemblage.h ../Surface.h \
  ../cxxMix.h ../Reaction.h ../Temperature.h ../NumKeyword.h
SolutionIsotope.o: ../SolutionIsotope.cxx ../SolutionIsotope.h \
  ../Parser.h ../char_star.h ../Utils.h ../char_star.h ../Parser.h \
  ../phreeqc/global.h ../phreeqc/phrqtype.h ../phreeqc/phqalloc.h \
  ../phreeqc/phrqproto.h
SolutionIsotopeList.o: ../SolutionIsotopeList.cxx ../Utils.h \
  ../char_star.h ../SolutionIsotopeList.h ../SolutionIsotope.h \
  ../Parser.h ../char_star.h ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../char_star.h ../Parser.h ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h
SSassemblage.o: ../SSassemblage.cxx ../Utils.h ../char_star.h \
  ../SSassemblage.h ../NumKeyword.h ../Parser.h ../char_star.h \
  ../phreeqc/global.h ../phreeqc/phrqtype.h ../char_star.h \
  ../SSassemblageSS.h ../NameDouble.h ../char_star.h ../Parser.h \
  ../char_star.h ../SSassemblageSS.h ../phreeqc/phqalloc.h \
  ../phreeqc/phrqproto.h
SSassemblageSS.o: ../SSassemblageSS.cxx ../Utils.h ../char_star.h \
  ../SSassemblageSS.h ../NameDouble.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../char_star.h ../Parser.h ../char_star.h \
  ../char_star.h ../NameDouble.h ../phreeqc/phqalloc.h \
  ../phreeqc/phrqproto.h
StorageBin.o: ../StorageBin.cxx ../Utils.h ../char_star.h ../StorageBin.h \
  ../Utils.h ../Parser.h ../char_star.h ../Solution.h ../NumKeyword.h \
  ../Parser.h ../SolutionIsotopeList.h ../SolutionIsotope.h ../Parser.h \
  ../phreeqc/global.h ../phreeqc/phrqtype.h ../char_star.h ../Parser.h \
  ../NameDouble.h ../char_star.h ../Parser.h ../cxxMix.h ../NumKeyword.h \
  ../char_star.h ../char_star.h ../Exchange.h ../NumKeyword.h \
  ../char_star.h ../ExchComp.h ../NameDouble.h ../char_star.h \
  ../GasPhase.h ../NumKeyword.h ../NameDouble.h ../char_star.h \
  ../cxxKinetics.h ../NumKeyword.h ../char_star.h ../KineticsComp.h \
  ../NameDouble.h ../char_star.h ../PPassemblage.h ../NumKeyword.h \
  ../char_star.h ../PPassemblageComp.h ../NameDouble.h ../char_star.h \
  ../SSassemblage.h ../NumKeyword.h ../char_star.h ../SSassemblageSS.h \
  ../NameDouble.h ../char_star.h ../Surface.h ../NumKeyword.h \
  ../char_star.h ../SurfaceComp.h ../NameDouble.h ../char_star.h \
  ../SurfaceCharge.h ../NameDouble.h ../char_star.h ../cxxMix.h \
  ../Reaction.h ../NumKeyword.h ../NameDouble.h ../char_star.h \
  ../Temperature.h ../NumKeyword.h ../char_star.h ../Solution.h \
  ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h ../phreeqc/output.h
SurfaceCharge.o: ../SurfaceCharge.cxx ../Utils.h ../char_star.h \
  ../SurfaceCharge.h ../NameDouble.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../char_star.h ../Parser.h ../char_star.h \
  ../char_star.h ../phreeqc/output.h ../phreeqc/phqalloc.h \
  ../phreeqc/phrqproto.h
SurfaceComp.o: ../SurfaceComp.cxx ../Utils.h ../char_star.h \
  ../SurfaceComp.h ../NameDouble.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../char_star.h ../Parser.h ../char_star.h \
  ../char_star.h ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h \
  ../phreeqc/output.h
Surface.o: ../Surface.cxx ../Utils.h ../char_star.h ../Surface.h \
  ../NumKeyword.h ../Parser.h ../char_star.h ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../char_star.h ../SurfaceComp.h ../NameDouble.h \
  ../char_star.h ../Parser.h ../char_star.h ../SurfaceCharge.h \
  ../NameDouble.h ../char_star.h ../SurfaceComp.h ../phreeqc/phqalloc.h \
  ../phreeqc/phrqproto.h
Temperature.o: ../Temperature.cxx ../Utils.h ../char_star.h \
  ../Temperature.h ../NumKeyword.h ../Parser.h ../char_star.h \
  ../phreeqc/global.h ../phreeqc/phrqtype.h ../char_star.h \
  ../phreeqc/phqalloc.h ../phreeqc/phrqproto.h
Utils.o: ../Utils.cxx ../Utils.h ../char_star.h ../Parser.h \
  ../char_star.h ../phreeqc/output.h
#
#  PHREEQC files
#
advection.o: ../phreeqc/advection.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/phqalloc.h ../phreeqc/output.h \
  ../phreeqc/phrqproto.h
basic.o: ../phreeqc/basic.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h \
  ../phreeqc/p2c.h
basicsubs.o: ../phreeqc/basicsubs.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/phqalloc.h ../phreeqc/output.h \
  ../phreeqc/phrqproto.h
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
  ../phreeqc/phrqproto.h ../phreeqc/output.h ../phreeqc/pitzer.h
input.o: ../phreeqc/input.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/input.h ../phreeqc/output.h ../phreeqc/phrqproto.h \
  ../phreeqc/phqalloc.h
integrate.o: ../phreeqc/integrate.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/phqalloc.h ../phreeqc/output.h \
  ../phreeqc/phrqproto.h
inverse.o: ../phreeqc/inverse.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h
isotopes.o: ../phreeqc/isotopes.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/phqalloc.h ../phreeqc/output.h \
  ../phreeqc/phrqproto.h
kinetics.o: ../phreeqc/kinetics.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/phqalloc.h ../phreeqc/output.h \
  ../phreeqc/phrqproto.h ../phreeqc/sundialstypes.h ../phreeqc/cvode.h \
  ../phreeqc/nvector.h ../phreeqc/cvdense.h ../phreeqc/dense.h \
  ../phreeqc/smalldense.h ../phreeqc/nvector_serial.h \
  ../phreeqc/kinetics.h
main.o: ../phreeqc/main.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/output.h ../phreeqc/phrqproto.h ../phreeqc/input.h
mainsubs.o: ../phreeqc/mainsubs.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/phqalloc.h ../phreeqc/output.h \
  ../phreeqc/phrqproto.h ../phreeqc/input.h
model.o: ../phreeqc/model.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h
	${CXX} ${CXXFLAGS_MODEL} -c -o $@ $<
nvector.o: ../phreeqc/nvector.c ../phreeqc/nvector.h \
  ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/output.h
nvector_serial.o: ../phreeqc/nvector_serial.c ../phreeqc/nvector_serial.h \
  ../phreeqc/nvector.h ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h \
  ../phreeqc/sundialsmath.h ../phreeqc/output.h ../phreeqc/phqalloc.h
output.o: ../phreeqc/output.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/output.h ../phreeqc/phrqproto.h ../phreeqc/phqalloc.h
p2clib.o: ../phreeqc/p2clib.c ../phreeqc/p2c.h ../phreeqc/output.h
parse.o: ../phreeqc/parse.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h
phqalloc.o: ../phreeqc/phqalloc.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/output.h
phreeqc_files.o: ../phreeqc/phreeqc_files.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/phqalloc.h ../phreeqc/output.h \
  ../phreeqc/phrqproto.h ../phreeqc/input.h
pitzer.o: ../phreeqc/pitzer.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h \
  ../phreeqc/pitzer.h
pitzer_structures.o: ../phreeqc/pitzer_structures.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/phqalloc.h ../phreeqc/output.h \
  ../phreeqc/phrqproto.h ../phreeqc/pitzer.h
prep.o: ../phreeqc/prep.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h
print.o: ../phreeqc/print.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h \
  ../phreeqc/pitzer.h
read.o: ../phreeqc/read.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h
readtr.o: ../phreeqc/readtr.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h
smalldense.o: ../phreeqc/smalldense.c ../phreeqc/smalldense.h \
  ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h \
  ../phreeqc/sundialsmath.h ../phreeqc/output.h ../phreeqc/phqalloc.h
spread.o: ../phreeqc/spread.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h
step.o: ../phreeqc/step.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h
structures.o: ../phreeqc/structures.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/phqalloc.h ../phreeqc/output.h \
  ../phreeqc/phrqproto.h
sundialsmath.o: ../phreeqc/sundialsmath.c ../phreeqc/sundialsmath.h \
  ../phreeqc/sundialstypes.h ../phreeqc/phrqtype.h ../phreeqc/output.h
tally.o: ../phreeqc/tally.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h
tidy.o: ../phreeqc/tidy.c ../phreeqc/global.h ../phreeqc/phrqtype.h \
  ../phreeqc/phqalloc.h ../phreeqc/output.h ../phreeqc/phrqproto.h
transport.o: ../phreeqc/transport.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/phqalloc.h ../phreeqc/output.h \
  ../phreeqc/phrqproto.h
utilities.o: ../phreeqc/utilities.c ../phreeqc/global.h \
  ../phreeqc/phrqtype.h ../phreeqc/phqalloc.h ../phreeqc/output.h \
  ../phreeqc/phrqproto.h
# -----------------------------------------------------------------------------

clean:
	rm -rf Release Debug

dependencies:
	mkdir -p $(DEBUG_DIR) 
	cd $(DEBUG_DIR); gcc -MM -I../phreeqc ../*.cxx


# =============================================================================
# End of makefile.
# =============================================================================
