include(CTestConfig.cmake)

site_name(CTEST_SITE)
set(CTEST_BUILD_NAME ${CMAKE_HOST_SYSTEM_NAME})

set(CTEST_SOURCE_DIRECTORY "${CTEST_SCRIPT_DIRECTORY}")
set(CTEST_BINARY_DIRECTORY "${CTEST_SCRIPT_DIRECTORY}/_ctest")

set(CTEST_CMAKE_GENERATOR Ninja)
set(CTEST_CONFIGURATION_TYPE Release)
set(CTEST_BUILD_CONFIGURATION Release)
set(CTEST_USE_LAUNCHERS 1)

set(CTEST_UPDATE_TYPE git)
set(CTEST_UPDATE_COMMAND git)
set(CTEST_UPDATE_VERSION_ONLY TRUE)

ctest_empty_binary_directory(${CTEST_BINARY_DIRECTORY})
ctest_start("Continuous")
ctest_update()
ctest_configure(OPTIONS -DPHRQC_TESTING:BOOL=ON)
ctest_build()
if(${CMAKE_VERSION} VERSION_LESS "3.29")
  ctest_test(PARALLEL_LEVEL 2)
else()
  ctest_test(PARALLEL_LEVEL)
endif()
ctest_submit()
