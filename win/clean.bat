@echo off
rem  clean.bat - clean up files after test runs
rem  Usage:  clean

rem  remove output from each test
rm -f *.out *.log *.tsv *.sel check.log phreeqc.log radial Zn* ex* co2.tsv

