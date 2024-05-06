set(phreeqc_DATABASE
  Amm.dat
  core10.dat
  ColdChem.dat
  frezchem.dat
  iso.dat
  llnl.dat
  kinec.v2.dat
  minteq.dat
  minteq.v4.dat
  phreeqc.dat
  phreeqc_rates.dat
  PHREEQC_ThermoddemV1.10_15Dec2020.dat
  pitzer.dat
  sit.dat
  Tipping_Hurley.dat
  wateq4f.dat
)

# for mytest tests
foreach(db ${phreeqc_DATABASE})
  configure_file(${db} ${db} COPYONLY)
endforeach()

if(WIN32)
  install(FILES ${phreeqc_DATABASE} DESTINATION database)
else()
  install(FILES ${phreeqc_DATABASE} DESTINATION ${CMAKE_INSTALL_DOCDIR}/database)
endif()
