SET(phreeqc_ROOT
NOTICE.TXT
## TODO README.TXT
RELEASE.TXT
)

SET(phreeqc_DOC
Phreeqc_2_1999_manual.pdf
Phreeqc_3_2013_manual.pdf
phreeqc.txt
wrir02-4172.pdf
)

install (FILES ${phreeqc_ROOT} DESTINATION ${CMAKE_INSTALL_PREFIX})
install (FILES ${phreeqc_DOC} DESTINATION ${CMAKE_INSTALL_DOCDIR})
