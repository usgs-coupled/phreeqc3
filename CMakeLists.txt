cmake_minimum_required (VERSION 2.6)
project (phreeqc)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

##{{option(USE_GMP
##{{       "Use GNU Multiple Precision library" OFF)

# Disable deprecation warnings for standard C and STL functions in VS2005
# and later
IF(MSVC_VERSION EQUAL 1400 OR MSVC_VERSION GREATER 1400)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
  ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
ENDIF(MSVC_VERSION EQUAL 1400 OR MSVC_VERSION GREATER 1400) 

# define DOS
IF(WIN32)
  ADD_DEFINITIONS(-DDOS)
ENDIF(WIN32)

##{{# define INVERSE_CL1MP
##{{IF(USE_GMP)
##{{  ADD_DEFINITIONS(-DINVERSE_CL1MP)
##{{ENDIF(USE_GMP)


SET(phreeqc_SOURCES
src/class_main.cpp
src/cxxKinetics.cxx
src/cxxKinetics.h
src/cxxMix.cxx
src/cxxMix.h
src/dumper.cpp
src/dumper.h
src/Exchange.cxx
src/Exchange.h
src/ExchComp.cxx
src/ExchComp.h
src/GasComp.cxx
src/GasComp.h
src/GasPhase.cxx
src/GasPhase.h
src/ISolution.cxx
src/ISolution.h
src/ISolutionComp.cxx
src/ISolutionComp.h
src/Keywords.cpp
src/Keywords.h
src/KineticsComp.cxx
src/KineticsComp.h
src/NameDouble.cxx
src/NameDouble.h
src/NumKeyword.cxx
src/NumKeyword.h
src/Parser.cxx
src/Parser.h
src/PBasic.cpp
src/PBasic.h
src/Phreeqc.cpp
src/Phreeqc.h
src/PHRQ_base.cxx
src/PHRQ_base.h
src/PHRQ_io.cpp
src/PHRQ_io.h
src/PPassemblage.cxx
src/PPassemblage.h
src/PPassemblageComp.cxx
src/PPassemblageComp.h
src/Pressure.cxx
src/Pressure.h
src/Reaction.cxx
src/Reaction.h
src/ReadClass.cxx
src/runner.cpp
src/runner.h
src/SelectedOutput.cpp
src/SelectedOutput.h
src/Solution.cxx
src/Solution.h
src/SolutionIsotope.cxx
src/SolutionIsotope.h
src/SSassemblage.cxx
src/SSassemblage.h
src/SS.cxx
src/SS.h
src/SScomp.cxx
src/SScomp.h
src/StorageBin.cxx
src/StorageBin.h
src/StorageBinList.cpp
src/StorageBinList.h
src/Surface.cxx
src/Surface.h
src/SurfaceCharge.cxx
src/SurfaceCharge.h
src/SurfaceComp.cxx
src/SurfaceComp.h
src/System.cxx
src/System.h
src/Temperature.cxx
src/Temperature.h
src/Use.cpp
src/Use.h
src/UserPunch.cpp
src/UserPunch.h
src/Utils.cxx
src/Utils.h
src/advection.cpp
src/basicsubs.cpp
src/cl1.cpp
src/cvdense.cpp
src/cvdense.h
src/cvode.cpp
src/cvode.h
src/dense.cpp
src/dense.h
src/dw.cpp
src/gases.cpp
src/global_structures.h
src/input.cpp
src/integrate.cpp
src/inverse.cpp
src/isotopes.cpp
src/kinetics.cpp
src/mainsubs.cpp
src/model.cpp
src/NA.h
src/nvector.cpp
src/nvector.h
src/nvector_serial.cpp
src/nvector_serial.h
src/parse.cpp
src/phqalloc.cpp
src/phqalloc.h
src/PHRQ_io_output.cpp
src/phrqtype.h
src/pitzer.cpp
src/pitzer_structures.cpp
src/prep.cpp
src/print.cpp
src/read.cpp
src/readtr.cpp
src/sit.cpp
src/smalldense.cpp
src/smalldense.h
src/spread.cpp
src/step.cpp
src/structures.cpp
src/sundialsmath.cpp
src/sundialsmath.h
src/sundialstypes.h
src/tally.cpp
src/tidy.cpp
src/transport.cpp
src/utilities.cpp
)

##{{find_package(GMP)
##{{
##{{# define INVERSE_CL1MP
##{{IF(USE_GMP)
##{{  SET(GMP_INCLUDE_DIR $ENV{DEV_GMP_INC_MT})
##{{  SET(GMP_LIBRARIES $ENV{DEV_GMP_INC_MT})
##{{  INCLUDE_DIRECTORIES(${GMP_INCLUDE_DIR})
##{{  SET(phreeqc_SOURCES ${phreeqc_SOURCES} src/cl1mp.cpp)
##{{  set(LINK_LIBS ${LINK_LIBS} ${SZIP_LIBRARIES})
##{{ENDIF(USE_GMP)



option(PHRQC_ENABLE_HDF5 "Enable or disable HDF5 interface" "OFF")
if (PHRQC_ENABLE_HDF5)
  find_path(HDF5_INCLUDE_PATH hdf5.h)
  find_library(HDF5_LIBRARY hdf5)
else (PHRQC_ENABLE_HDF5)
  mark_as_advanced(FORCE HDF5_INCLUDE_PATH HDF5_LIBRARY)
endif (PHRQC_ENABLE_HDF5)


add_executable(phreeqc ${phreeqc_SOURCES})