@echo off
rem  clean.bat - clean up files after test runs
rem  Usage:  clean
rem
rem  History: 00/02/16 mcrouse
rem           December 2012, PHREEQC3

rem  remove output from each test
del /Q *.out *.log *.tsv *.sel check.log phreeqc.log radial Zn* ex* co2.tsv

