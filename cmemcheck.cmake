include(CTestConfig.cmake)

site_name(CTEST_SITE)
set(CTEST_BUILD_NAME ${CMAKE_HOST_SYSTEM_NAME})

set(CTEST_SOURCE_DIRECTORY "${CTEST_SCRIPT_DIRECTORY}")
set(CTEST_BINARY_DIRECTORY "${CTEST_SCRIPT_DIRECTORY}/_cmemcheck")

##set(ENV{CXXFLAGS} "--coverage")
set(CTEST_CMAKE_GENERATOR Ninja)
set(CTEST_CONFIGURATION_TYPE Debug)
set(CTEST_BUILD_CONFIGURATION Debug)
set(CTEST_USE_LAUNCHERS 1)

set(CTEST_UPDATE_TYPE git)
set(CTEST_UPDATE_COMMAND git)
set(CTEST_UPDATE_VERSION_ONLY TRUE)

##find_program(CTEST_COVERAGE_COMMAND NAMES gcov)
find_program(CTEST_MEMORYCHECK_COMMAND NAMES valgrind)

ctest_empty_binary_directory(${CTEST_BINARY_DIRECTORY})
ctest_start("Continuous")
ctest_update()
ctest_configure(OPTIONS -DPHRQC_TESTING:BOOL=ON)
ctest_build()
if(${CMAKE_VERSION} VERSION_LESS "3.29")
  ctest_memcheck(EXCLUDE "numdiff" EXCLUDE_LABEL "long_memcheck" PARALLEL_LEVEL 2)
else()
  ctest_memcheck(EXCLUDE "numdiff" EXCLUDE_LABEL "long_memcheck" PARALLEL_LEVEL)
endif()
##ctest_coverage()
ctest_submit()
