cmake_minimum_required (VERSION 3.10)

SET(phreeqc_EXAMPLES
co2.dat
co2.tsv
co2_VP.dat
ex1
ex2
ex2b
ex2b.tsv
ex3
ex4
ex5
ex6
ex7
ex8
ex9
ex10
ex11
ex12
ex12a
ex12b
ex13a
ex13ac
ex13b
ex13c
ex14
ex15
ex15a
ex15b
ex15.dat
ex16
ex17
ex17b
ex18
ex19
ex19_meas.tsv
ex19b
ex20a
ex20b
ex20-c13.tsv
ex20-c14.tsv
ex21
ex21_Cl_tr_rad.tsv
ex21_Cs_rad.tsv
ex21_HTO_rad.tsv
ex21_Na_tr_rad.tsv
ex22
Zn1e_4
Zn1e_7
)

if(WIN32)
  install (FILES ${phreeqc_EXAMPLES} DESTINATION examples)
else()
  install (FILES ${phreeqc_EXAMPLES} DESTINATION ${CMAKE_INSTALL_DOCDIR}/examples)
endif()

#
# copy tsv files to build directories for testing
#

# ex2b.tsv
configure_file (
  "ex2b.tsv"
  "ex2b.tsv"
  )

# ex19_meas.tsv
configure_file (
  "ex19_meas.tsv"
  "ex19_meas.tsv"
  )

# ex20-c13.tsv
configure_file (
  "ex20-c13.tsv"
  "ex20-c13.tsv"
  )

# ex20-c14.tsv
configure_file (
  "ex20-c14.tsv"
  "ex20-c14.tsv"
  )

# ex21_Cl_tr_rad.tsv
configure_file (
  "ex21_Cl_tr_rad.tsv"
  "ex21_Cl_tr_rad.tsv"
  )

# ex21_Cs_rad.tsv
configure_file (
  "ex21_Cs_rad.tsv"
  "ex21_Cs_rad.tsv"
  )

# ex21_HTO_rad.tsv
configure_file (
  "ex21_HTO_rad.tsv"
  "ex21_HTO_rad.tsv"
  )

# ex21_Na_tr_rad.tsv
configure_file (
  "ex21_Na_tr_rad.tsv"
  "ex21_Na_tr_rad.tsv"
  )

# co2.tsv
configure_file (
  "co2.tsv"
  "co2.tsv"
  )

# co2.dat
configure_file (
  "co2.dat"
  "co2.dat"
  )

# co2_VP.dat
configure_file (
  "co2_VP.dat"
  "co2_VP.dat"
  )
  
#
# run examples for testing
#

# ex1
configure_file(ex1 ex1 COPYONLY)
add_test(NAME examples.ex1
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex1 ex1.out ../database/phreeqc.dat ex1.log
)

# ex2
configure_file(ex2 ex2 COPYONLY)
add_test(NAME examples.ex2
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex2 ex2.out ../database/phreeqc.dat ex2.log
)

# ex2b
configure_file(ex2b ex2b COPYONLY)
add_test(NAME examples.ex2b
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex2b ex2b.out ../database/phreeqc.dat ex2b.log
)

# ex3
configure_file(ex3 ex3 COPYONLY)
add_test(NAME examples.ex3
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex3 ex3.out ../database/phreeqc.dat ex3.log
)

# ex4
configure_file(ex4 ex4 COPYONLY)
add_test(NAME examples.ex4
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex4 ex4.out ../database/phreeqc.dat ex4.log
)

# ex5
configure_file(ex5 ex5 COPYONLY)
add_test(NAME examples.ex5
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex5 ex5.out ../database/phreeqc.dat ex5.log
)

# ex6
configure_file(ex6 ex6 COPYONLY)
add_test(NAME examples.ex6
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex6 ex6.out ../database/phreeqc.dat ex6.log
)

# ex7
configure_file(ex7 ex7 COPYONLY)
add_test(NAME examples.ex7
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex7 ex7.out ../database/phreeqc.dat ex7.log
)

# ex8
configure_file(ex8 ex8 COPYONLY)
add_test(NAME examples.ex8
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex8 ex8.out ../database/phreeqc.dat ex8.log
)

# ex9
configure_file(ex9 ex9 COPYONLY)
add_test(NAME examples.ex9
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex9 ex9.out ../database/phreeqc.dat ex9.log
)

# ex10
configure_file(ex10 ex10 COPYONLY)
add_test(NAME examples.ex10
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex10 ex10.out ../database/phreeqc.dat ex10.log
)

# ex11
configure_file(ex11 ex11 COPYONLY)
add_test(NAME examples.ex11
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex11 ex11.out ../database/phreeqc.dat ex11.log
)

# ex12
configure_file(ex12 ex12 COPYONLY)
add_test(NAME examples.ex12
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex12 ex12.out ../database/phreeqc.dat ex12.log
)

# ex12a
configure_file(ex12a ex12a COPYONLY)
add_test(NAME examples.ex12a
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex12a ex12a.out ../database/phreeqc.dat ex12a.log
)

# ex13a
configure_file(ex13a ex13a COPYONLY)
add_test(NAME examples.ex13a
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex13a ex13a.out ../database/phreeqc.dat ex13a.log
)

# ex13b
configure_file(ex13b ex13b COPYONLY)
add_test(NAME examples.ex13b
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex13b ex13b.out ../database/phreeqc.dat ex13b.log
)

# ex13c
configure_file(ex13c ex13c COPYONLY)
add_test(NAME examples.ex13c
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex13c ex13c.out ../database/phreeqc.dat ex13c.log
)

# ex13ac
configure_file(ex13ac ex13ac COPYONLY)
add_test(NAME examples.ex13ac
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex13ac ex13ac.out ../database/phreeqc.dat ex13ac.log
)

# ex14
configure_file(ex14 ex14 COPYONLY)
add_test(NAME examples.ex14
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex14 ex14.out ../database/phreeqc.dat ex14.log
)

# ex15
configure_file(ex15 ex15 COPYONLY)
configure_file(ex15.dat ex15.dat COPYONLY)
add_test(NAME examples.ex15
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex15 ex15.out ../examples/ex15.dat ex15.log
)

# ex15a
configure_file(ex15a ex15a COPYONLY)
add_test(NAME examples.ex15a
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex15a ex15a.out ../examples/ex15.dat ex15a.log
)

# ex15b
configure_file(ex15b ex15b COPYONLY)
add_test(NAME examples.ex15b
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex15b ex15b.out ../examples/ex15.dat ex15b.log
)

# ex16
configure_file(ex16 ex16 COPYONLY)
add_test(NAME examples.ex16
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex16 ex16.out ../database/phreeqc.dat ex16.log
)

# ex17
configure_file(ex17 ex17 COPYONLY)
add_test(NAME examples.ex17
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex17 ex17.out ../database/pitzer.dat ex17.log
)

# ex17b
configure_file(ex17b ex17b COPYONLY)
add_test(NAME examples.ex17b
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex17b ex17b.out ../database/pitzer.dat ex17b.log
)

# ex18
configure_file(ex18 ex18 COPYONLY)
add_test(NAME examples.ex18
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex18 ex18.out ../database/phreeqc.dat ex18.log
)

# ex19
configure_file(ex19 ex19 COPYONLY)
add_test(NAME examples.ex19
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex19 ex19.out ../database/phreeqc.dat ex19.log
)

# ex19b
configure_file(ex19b ex19b COPYONLY)
add_test(NAME examples.ex19b
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex19b ex19b.out ../database/phreeqc.dat ex19b.log
)

# ex20a
configure_file(ex20a ex20a COPYONLY)
add_test(NAME examples.ex20a
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex20a ex20a.out ../database/iso.dat ex20a.log
)

# ex20b
configure_file(ex20b ex20b COPYONLY)
add_test(NAME examples.ex20b
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex20b ex20b.out ../database/iso.dat ex20b.log
)

# ex21
configure_file(ex21 ex21 COPYONLY)
add_test(NAME examples.ex21
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex21 ex21.out ../database/phreeqc.dat ex21.log
)

# ex22
configure_file(ex22 ex22 COPYONLY)
add_test(NAME examples.ex22
  COMMAND $<TARGET_FILE:phreeqc> ../examples/ex22 ex22.out ../database/phreeqc.dat ex22.log
)

# Note when setting labels we can't use the following:
#   set_tests_properties(${test} PROPERTIES LABELS "pitzer")
# since it will overwrite any existing labels already set

# long_debug
# > 600 seconds Debug
set(LONG_DEBUG
  examples.ex21
)

# label tests that take too long
foreach(test ${LONG_DEBUG})
  set_property(TEST ${test} APPEND PROPERTY LABELS "long_debug")
  set_property(TEST ${test} APPEND PROPERTY LABELS "long_memcheck")
endforeach()
