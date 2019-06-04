# set standard directory locations
include(GNUInstallDirs)

set(phreeqc_DATABASE
  Amm.dat
  core10.dat
  ColdChem.dat
  frezchem.dat
  iso.dat
  llnl.dat
  minteq.dat
  minteq.v4.dat
  phreeqc.dat
  pitzer.dat
  sit.dat
  Tipping_Hurley.dat
  wateq4f.dat
)

if (WIN32)
  install (FILES ${phreeqc_DATABASE} DESTINATION database)
else()
  install (FILES ${phreeqc_DATABASE} DESTINATION ${CMAKE_INSTALL_DOCDIR}/database)
endif()
