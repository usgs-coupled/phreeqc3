IF(WIN32)
  SET(phreeqc_ROOT
  NOTICE.TXT
  ## TODO README.TXT
  RELEASE.TXT
  )
ELSE()
  SET(phreeqc_ROOT
  NOTICE
  README
  RELEASE
  )
ENDIF()

SET(phreeqc_DOC
Phreeqc_2_1999_manual.pdf
Phreeqc_3_2013_manual.pdf
phreeqc3.chm
phreeqc.txt
wrir02-4172.pdf
)

IF(WIN32)
  install (FILES ${phreeqc_ROOT} DESTINATION ${CMAKE_INSTALL_PREFIX})
ELSE()
  install (FILES ${phreeqc_ROOT} DESTINATION ${CMAKE_INSTALL_DOCDIR})
ENDIF()

install (FILES ${phreeqc_DOC} DESTINATION ${CMAKE_INSTALL_DOCDIR})
