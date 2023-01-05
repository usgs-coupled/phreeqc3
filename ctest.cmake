include(CTestConfig.cmake)

site_name(CTEST_SITE)
set(CTEST_BUILD_NAME ${CMAKE_HOST_SYSTEM_NAME})

set(CTEST_SOURCE_DIRECTORY "${CTEST_SCRIPT_DIRECTORY}")
set(CTEST_BINARY_DIRECTORY "${CTEST_SCRIPT_DIRECTORY}/_ctest")

##set(ENV{CXXFLAGS} "--coverage")
set(CTEST_CMAKE_GENERATOR Ninja)
set(CTEST_USE_LAUNCHERS 1)

set(CTEST_UPDATE_TYPE git)
set(CTEST_UPDATE_COMMAND git)

##set(CTEST_COVERAGE_COMMAND "gcov")
find_program(CTEST_MEMORYCHECK_COMMAND NAMES valgrind)

ctest_empty_binary_directory(${CTEST_BINARY_DIRECTORY})
ctest_start("Continuous")
ctest_update()
ctest_configure(OPTIONS "-DCMAKE_BUILD_TYPE=Debug")
ctest_build()
ctest_test(INCLUDE "mytest.*.ex10$")
#ctest_coverage()
if (CTEST_MEMORYCHECK_COMMAND)
  ctest_memcheck(INCLUDE "mytest.*.ex10$" EXCLUDE "numdiff")
endif()
ctest_submit()