SET(phreeqc_ROOT
NOTICE
README
RELEASE
)

SET(phreeqc_DOC
Phreeqc_2_1999_manual.pdf
Phreeqc_3_2013_manual.pdf
phreeqc3.chm
phreeqc.txt
wrir02-4172.pdf
)

IF(WIN32)
  foreach(file ${phreeqc_ROOT})
    install (FILES ${file} DESTINATION ${CMAKE_INSTALL_PREFIX} RENAME ${file}.txt)
  endforeach()
ELSE()
  install (FILES ${phreeqc_ROOT} DESTINATION ${CMAKE_INSTALL_DOCDIR})
ENDIF()

install (FILES ${phreeqc_DOC} DESTINATION ${CMAKE_INSTALL_DOCDIR})
