cmake_minimum_required (VERSION 3.3)

project (phreeqc)

# overide docdir on windows
if(WIN32 AND NOT CMAKE_INSTALL_DOCDIR)
  set(CMAKE_INSTALL_DOCDIR "" CACHE PATH "documentation root (doc)")
  set(CMAKE_INSTALL_DOCDIR "doc")
endif()

include(GNUInstallDirs)

# Disable deprecation warnings for standard C and STL functions in VS2005
# and later
IF(MSVC_VERSION EQUAL 1400 OR MSVC_VERSION GREATER 1400)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
  ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
ENDIF(MSVC_VERSION EQUAL 1400 OR MSVC_VERSION GREATER 1400) 

# define DOS
IF(WIN32)
  ADD_DEFINITIONS(-DDOS)
  ADD_DEFINITIONS(-D_CONSOLE)
ENDIF(WIN32)


SET(phreeqc_SOURCES
src/ChartHandler.cpp
src/ChartObject.cpp
src/CurveObject.cpp
src/Dictionary.cpp
src/Dictionary.h
src/ExchComp.cxx
src/ExchComp.h
src/Exchange.cxx
src/Exchange.h
src/GasComp.cxx
src/GasComp.h
src/GasPhase.cxx
src/GasPhase.h
src/ISolution.cxx
src/ISolution.h
src/ISolutionComp.cxx
src/ISolutionComp.h
src/KineticsComp.cxx
src/KineticsComp.h
src/NA.h
src/NameDouble.cxx
src/NameDouble.h
src/NumKeyword.cxx
src/NumKeyword.h
src/PBasic.cpp
src/PBasic.h
src/PHRQ_io_output.cpp
src/PPassemblage.cxx
src/PPassemblage.h
src/PPassemblageComp.cxx
src/PPassemblageComp.h
src/Phreeqc.cpp
src/Phreeqc.h
src/PhreeqcKeywords/Keywords.cpp
src/PhreeqcKeywords/Keywords.h
src/Pressure.cxx
src/Pressure.h
src/Reaction.cxx
src/Reaction.h
src/ReadClass.cxx
src/SS.cxx
src/SS.h
src/SSassemblage.cxx
src/SSassemblage.h
src/SScomp.cxx
src/SScomp.h
src/SelectedOutput.cpp
src/SelectedOutput.h
src/Serializer.cxx
src/Serializer.h
src/Solution.cxx
src/Solution.h
src/SolutionIsotope.cxx
src/SolutionIsotope.h
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
src/advection.cpp
src/basicsubs.cpp
src/cl1.cpp
src/class_main.cpp
src/common/PHRQ_base.cxx
src/common/PHRQ_base.h
src/common/PHRQ_io.cpp
src/common/PHRQ_io.h
src/common/Parser.cxx
src/common/Parser.h
src/common/Utils.cxx
src/common/Utils.h
src/common/phrqtype.h
src/cvdense.cpp
src/cvdense.h
src/cvode.cpp
src/cvode.h
src/cxxKinetics.cxx
src/cxxKinetics.h
src/cxxMix.cxx
src/cxxMix.h
src/dense.cpp
src/dense.h
src/dumper.cpp
src/dumper.h
src/gases.cpp
src/global_structures.h
src/input.cpp
src/integrate.cpp
src/inverse.cpp
src/isotopes.cpp
src/kinetics.cpp
src/mainsubs.cpp
src/model.cpp
src/nvector.cpp
src/nvector.h
src/nvector_serial.cpp
src/nvector_serial.h
src/parse.cpp
src/phqalloc.cpp
src/phqalloc.h
src/pitzer.cpp
src/pitzer_structures.cpp
src/prep.cpp
src/print.cpp
src/read.cpp
src/readtr.cpp
src/runner.cpp
src/runner.h
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

include_directories("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_SOURCE_DIR}/src/common")
include_directories("${PROJECT_SOURCE_DIR}/src/PhreeqcKeywords")


option(PHRQC_USE_GMP "Use GNU Multiple Precision library" "OFF")
if (PHRQC_USE_GMP)
  find_path(GMP_INCLUDE_PATH gmp.h)
  find_library(GMP_LIBRARY gmp)
else (PHRQC_USE_GMP)
  mark_as_advanced(FORCE GMP_INCLUDE_PATH GMP_LIBRARY)
endif (PHRQC_USE_GMP)


if (PHRQC_USE_GMP)
  if (GMP_INCLUDE_PATH)
    SET(phreeqc_SOURCES ${phreeqc_SOURCES} src/cl1mp.cpp)
  endif (GMP_INCLUDE_PATH)
endif (PHRQC_USE_GMP)

if (MSVC)

  # Copy Release to ClrRelease
  SET(CMAKE_CXX_FLAGS_CLRRELEASE ${CMAKE_CXX_FLAGS_RELEASE} CACHE STRING
      "Flags used by the C++ compiler during ClrRelease builds."
      FORCE)
  SET(CMAKE_C_FLAGS_CLRRELEASE ${CMAKE_C_FLAGS_RELEASE} CACHE STRING
      "Flags used by the C compiler during ClrRelease builds."
      FORCE)
  SET(CMAKE_EXE_LINKER_FLAGS_CLRRELEASE
      ${CMAKE_EXE_LINKER_FLAGS_RELEASE} CACHE STRING
      "Flags used for linking binaries during ClrRelease builds."
      FORCE)
  SET(CMAKE_SHARED_LINKER_FLAGS_CLRRELEASE
      ${CMAKE_SHARED_LINKER_FLAGS_RELEASE} CACHE STRING
      "Flags used by the shared libraries linker during ClrRelease builds."
      FORCE)
  MARK_AS_ADVANCED(
      CMAKE_CXX_FLAGS_CLRRELEASE
      CMAKE_C_FLAGS_CLRRELEASE
      CMAKE_EXE_LINKER_FLAGS_CLRRELEASE
      CMAKE_SHARED_LINKER_FLAGS_CLRRELEASE )


  # Copy Debug to ClrDebug
  SET(CMAKE_CXX_FLAGS_CLRDEBUG ${CMAKE_CXX_FLAGS_DEBUG} CACHE STRING
      "Flags used by the C++ compiler during ClrDebug builds."
      FORCE)
  SET(CMAKE_C_FLAGS_CLRDEBUG ${CMAKE_C_FLAGS_DEBUG} CACHE STRING
      "Flags used by the C compiler during ClrDebug builds."
      FORCE)
  SET(CMAKE_EXE_LINKER_FLAGS_CLRDEBUG
      ${CMAKE_EXE_LINKER_FLAGS_DEBUG} CACHE STRING
      "Flags used for linking binaries during ClrDebug builds."
      FORCE)
  SET(CMAKE_SHARED_LINKER_FLAGS_CLRDEBUG
      ${CMAKE_SHARED_LINKER_FLAGS_DEBUG} CACHE STRING
      "Flags used by the shared libraries linker during ClrDebug builds."
      FORCE)
  MARK_AS_ADVANCED(
      CMAKE_CXX_FLAGS_CLRDEBUG
      CMAKE_C_FLAGS_CLRDEBUG
      CMAKE_EXE_LINKER_FLAGS_CLRDEBUG
      CMAKE_SHARED_LINKER_FLAGS_CLRDEBUG )

      
  # Remove /EHsc from CMAKE_CXX_FLAGS
  STRING(REPLACE " /EHsc" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  STRING(REPLACE "/EHsc " "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  SET(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS}" CACHE STRING
      "Flags used by the compiler during all build types."
      FORCE)
  
  # Add /EHsc to CMAKE_CXX_FLAGS_RELEASE
  STRING(REPLACE "/EHsc " "" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
  STRING(REPLACE " /EHsc" "" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
  SET(CMAKE_CXX_FLAGS_RELEASE
      "/EHsc ${CMAKE_CXX_FLAGS_RELEASE}" CACHE STRING
      "Flags used by the compiler during Release build types."
      FORCE)      

  # Add /EHsc to CMAKE_CXX_FLAGS_DEBUG
  STRING(REPLACE "/EHsc " "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
  STRING(REPLACE " /EHsc" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
  SET(CMAKE_CXX_FLAGS_DEBUG
      "/EHsc ${CMAKE_CXX_FLAGS_DEBUG}" CACHE STRING
      "Flags used by the compiler during Debug build types."
      FORCE)

  # Add /EHa and /clr to CMAKE_CXX_FLAGS_CLRRELEASE
  STRING(REPLACE "/EHsc " "" CMAKE_CXX_FLAGS_CLRRELEASE ${CMAKE_CXX_FLAGS_CLRRELEASE})
  STRING(REPLACE " /EHsc" "" CMAKE_CXX_FLAGS_CLRRELEASE ${CMAKE_CXX_FLAGS_CLRRELEASE})
  SET(CMAKE_CXX_FLAGS_CLRRELEASE
      "-DMULTICHART /EHa ${CMAKE_CXX_FLAGS_CLRRELEASE} /clr" CACHE STRING
      "Flags used by the compiler during ClrRelease builds."
      FORCE)

  # Add /EHa and /clr to CMAKE_CXX_FLAGS_CLRDEBUG
  STRING(REPLACE "/EHsc " "" CMAKE_CXX_FLAGS_CLRDEBUG ${CMAKE_CXX_FLAGS_CLRDEBUG})
  STRING(REPLACE " /EHsc" "" CMAKE_CXX_FLAGS_CLRDEBUG ${CMAKE_CXX_FLAGS_CLRDEBUG})
  STRING(REPLACE " /RTC1" " " CMAKE_CXX_FLAGS_CLRDEBUG "${CMAKE_CXX_FLAGS_CLRDEBUG}")
  STRING(REPLACE "/RTC1 " " " CMAKE_CXX_FLAGS_CLRDEBUG "${CMAKE_CXX_FLAGS_CLRDEBUG}")
  SET(CMAKE_CXX_FLAGS_CLRDEBUG
      "-DMULTICHART /EHa ${CMAKE_CXX_FLAGS_CLRDEBUG} /clr" CACHE STRING
      "Flags used by the compiler during ClrDebug builds."
      FORCE)

  SET(CMAKE_CONFIGURATION_TYPES "Debug;Release;ClrDebug;ClrRelease" CACHE STRING 
  "Semicolon separated list of supported configuration types."
  FORCE)
endif (MSVC)

add_executable(phreeqc ${phreeqc_SOURCES})
set_target_properties(phreeqc PROPERTIES DEBUG_POSTFIX "d")


if (PHRQC_USE_GMP)
  if (GMP_INCLUDE_PATH)
    ADD_DEFINITIONS(-DINVERSE_CL1MP)
    include_directories(${GMP_INCLUDE_PATH})
  endif (GMP_INCLUDE_PATH)
  if (GMP_LIBRARY)
    target_link_libraries(phreeqc ${GMP_LIBRARY})
  endif(GMP_LIBRARY)
endif (PHRQC_USE_GMP)

if (WIN32)
  install (TARGETS phreeqc DESTINATION "${CMAKE_INSTALL_BINDIR}/Release" CONFIGURATIONS Debug Release)
  install (TARGETS phreeqc DESTINATION "${CMAKE_INSTALL_BINDIR}/ClrRelease" CONFIGURATIONS ClrDebug ClrRelease)
else()
  install (TARGETS phreeqc DESTINATION "${CMAKE_INSTALL_BINDIR}")
endif()

# tests
include(CTest)

# subdirectories
add_subdirectory(database)
add_subdirectory(doc)
add_subdirectory(examples)
if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/mytest/CMakeLists.txt)
  add_subdirectory(mytest)
endif()


# Setup references for /CLR
if (MSVC)
  set_target_properties(phreeqc PROPERTIES VS_DOTNET_REFERENCES "System;System.Drawing;System.Windows.Forms;${CMAKE_CURRENT_SOURCE_DIR}/src/ZedGraph.dll")  
endif (MSVC)
