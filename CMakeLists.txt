SET(phreeqc_EXAMPLES
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

# ex1
add_test(NAME ex1
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex1 ex1.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex1.log
)

# ex2
add_test(NAME ex2
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex2 ex2.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex2.log
)

# ex2b
add_test(NAME ex2b
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex2b ex2b.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex2b.log
)

# ex3
add_test(NAME ex3
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex3 ex3.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex3.log
)

# ex4
add_test(NAME ex4
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex4 ex4.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex4.log
)

# ex5
add_test(NAME ex5
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex5 ex5.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex5.log
)

# ex6
add_test(NAME ex6
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex6 ex6.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex6.log
)

# ex7
add_test(NAME ex7
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex7 ex7.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex7.log
)

# ex8
add_test(NAME ex8
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex8 ex8.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex8.log
)

# ex9
add_test(NAME ex9
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex9 ex9.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex9.log
)

# ex10
add_test(NAME ex10
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex10 ex10.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex10.log
)

# ex11
add_test(NAME ex11
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex11 ex11.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex11.log
)

# ex12
add_test(NAME ex12
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex12 ex12.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex12.log
)

# ex12a
add_test(NAME ex12a
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex12a ex12a.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex12a.log
)

# ex13a
add_test(NAME ex13a
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex13a ex13a.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex13a.log
)

# ex13b
add_test(NAME ex13b
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex13b ex13b.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex13b.log
)

# ex13c
add_test(NAME ex13c
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex13c ex13c.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex13c.log
)

# ex13ac
add_test(NAME ex13ac
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex13ac ex13ac.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex13ac.log
)

# ex14
add_test(NAME ex14
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex14 ex14.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex14.log
)

# ex15
add_test(NAME ex15
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex15 ex15.out ${PROJECT_SOURCE_DIR}/examples/ex15.dat ex15.log
)

# ex15a
add_test(NAME ex15a
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex15a ex15a.out ${PROJECT_SOURCE_DIR}/examples/ex15.dat ex15a.log
)

# ex15b
add_test(NAME ex15b
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex15b ex15b.out ${PROJECT_SOURCE_DIR}/examples/ex15.dat ex15b.log
)

# ex16
add_test(NAME ex16
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex16 ex16.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex16.log
)
	
# ex17
add_test(NAME ex17
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex17 ex17.out ${PROJECT_SOURCE_DIR}/database/pitzer.dat ex17.log
)

# ex17b
add_test(NAME ex17b
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex17b ex17b.out ${PROJECT_SOURCE_DIR}/database/pitzer.dat ex17b.log
)

# ex18
add_test(NAME ex18
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex18 ex18.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex18.log
)

# ex19
add_test(NAME ex19
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex19 ex19.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex19.log
)

# ex19b
add_test(NAME ex19b
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex19b ex19b.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex19b.log
)

# ex20a
add_test(NAME ex20a
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex20a ex20a.out ${PROJECT_SOURCE_DIR}/database/iso.dat ex20a.log
)

# ex20b
add_test(NAME ex20b
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex20b ex20b.out ${PROJECT_SOURCE_DIR}/database/iso.dat ex20b.log
)

# ex21
add_test(NAME ex21
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex21 ex21.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex21.log
)

# ex22
add_test(NAME ex22
COMMAND $<TARGET_FILE:phreeqc> ${PROJECT_SOURCE_DIR}/examples/ex22 ex22.out ${PROJECT_SOURCE_DIR}/database/phreeqc.dat ex22.log
)
