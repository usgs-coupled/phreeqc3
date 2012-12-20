@echo off
rem  test.bat - batch program to run PHREEQC tests
rem
rem  Usage: test [start [stop]]
rem
rem         where: start = starting test number
rem                 stop = ending test number (may be same as start)
rem
rem         (Note that start and stop are assumed to be within the valid
rem          test range and are assumed to be in ascending order--the
rem          absence of numeric variables in batch files makes error checking
rem          awkward.)
rem
rem  History: 00/02/16 mcrouse
rem           December 2012, PHREEQC3
set topdir=..
set data=%topdir%\examples
REM set prog="%PATH_PHREEQC%src\Release\phreeqc.exe"
set prog="%topdir%\ClrClass_release\phreeqcpp.exe"
set prognm=PHREEQC

REM need to use these in distribution
REM set PHREEQCDAT="%PATH_PHREEQC%database\phreeqc.dat"
REM set WATEQ4FDAT="%PATH_PHREEQC%database\wateq4f.dat"
REM set PITZERDAT="%PATH_PHREEQC%database\pitzer.dat"
REM set ISODAT="%PATH_PHREEQC%database\iso.dat"

set PHREEQCDAT="%topdir%\database\phreeqc.dat"
set WATEQ4FDAT="%topdir%\database\wateq4f.dat"
set PITZERDAT="%topdir%\database\pitzer.dat"
set ISODAT="%topdir%\database\iso.dat"
set divd=---------------------------------------

rem  assign valid test range values as default values
set start=1
set stop=22
rem  if values supplied for start and stop, use those
if not "%1"=="" set start=%1
if not "%2"=="" set stop=%2

echo.
echo %divd%%divd%
echo Begin processing %prognm% test runs %start% to %stop%
echo.
goto test%start%

:test1
set n=1
set name=ex1
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %PHREEQCDAT%
mv phreeqc.log %name%.log
if "%stop%"=="%n%" goto end

:test2
set n=2
set name=ex2
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n% 
%prog% %infile% %outfile% %PHREEQCDAT%
REM Part b
set name=ex2b
del /Q %name%.*
cp %data%\%name%.tsv .
set infile=%data%\%name%
set outfile=%name%.out
%prog% %infile% %outfile% %PHREEQCDAT%
mv phreeqc.log %name%.log
if "%stop%"=="%n%" goto end

:test3
set n=3
set name=ex3
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %PHREEQCDAT%
mv phreeqc.log %name%.log
if "%stop%"=="%n%" goto end

:test4
set n=4
set name=ex4
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %PHREEQCDAT%
mv phreeqc.log %name%.log
if "%stop%"=="%n%" goto end

:test5
set n=5
set name=ex5
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %PHREEQCDAT%
mv phreeqc.log %name%.log
if "%stop%"=="%n%" goto end

:test6
set n=6
set name=ex6
del /Q %name%.* %name%*sel
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n% 
%prog% %infile% %outfile% %PHREEQCDAT%
mv phreeqc.log %name%.log
if "%stop%"=="%n%" goto end

:test7
set n=7
set name=ex7
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %PHREEQCDAT%
mv phreeqc.log %name%.log
if "%stop%"=="%n%" goto end

:test8
set n=8
set name=ex8
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %PHREEQCDAT%
mv phreeqc.log %name%.log
if "%stop%"=="%n%" goto end

:test9
set n=9
set name=ex9
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %PHREEQCDAT%
mv phreeqc.log %name%.log
if "%stop%"=="%n%" goto end

:test10
set n=10
set name=ex10
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %PHREEQCDAT%
mv phreeqc.log %name%.log
if "%stop%"=="%n%" goto end

:test11
set n=11
set name=ex11
del /Q %name%.* %name%*sel
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %PHREEQCDAT%
mv phreeqc.log %name%.log
if "%stop%"=="%n%" goto end

:test12
set n=12
set name=ex12
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n% 
%prog% %infile% %outfile% %PHREEQCDAT%
REM Part A
set name=ex12a
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
%prog% %infile% %outfile% %PHREEQCDAT%
mv phreeqc.log %name%.log
if "%stop%"=="%n%" goto end

:test13
set n=13
set name=ex13a
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n% 
%prog% %infile% %outfile% %PHREEQCDAT%
REM Part B
set name=ex13b
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
%prog% %infile% %outfile% %PHREEQCDAT%
mv phreeqc.log %name%.log
REM Part C
set name=ex13c
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
%prog% %infile% %outfile% %PHREEQCDAT%
mv phreeqc.log %name%.log
REM Part A-C
set name=ex13ac
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
%prog% %infile% %outfile% %PHREEQCDAT%
mv phreeqc.log %name%.log
if "%stop%"=="%n%" goto end

:test14
set n=14
set name=ex14
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n% 
%prog% %infile% %outfile% %PHREEQCDAT%
if "%stop%"=="%n%" goto end

:test15
set n=15
set name=ex15
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n% 
%prog% %infile% %outfile% %data%\ex15.dat
REM Part A
set name=ex15a
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
%prog% %infile% %outfile% %data%\ex15.dat
mv phreeqc.log %name%.log
REM Part B
set name=ex15b
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
%prog% %infile% %outfile% %data%\ex15.dat
mv phreeqc.log %name%.log
if "%stop%"=="%n%" goto end

:test16
set n=16
set name=ex16
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n% 
%prog% %infile% %outfile% %PHREEQCDAT%
if "%stop%"=="%n%" goto end

:test17
set n=17
set name=ex17
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n% 
%prog% %infile% %outfile% %PITZERDAT%
REM Part B
set name=ex17b
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
%prog% %infile% %outfile% %PITZERDAT%
mv phreeqc.log %name%.log
if "%stop%"=="%n%" goto end

:test18
set n=18
set name=ex18
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n% 
%prog% %infile% %outfile% %PHREEQCDAT%
if "%stop%"=="%n%" goto end

:test19
set n=19
set name=ex19
del /Q %name%.* %name%*tsv 
cp %data%\ex19_meas.tsv .
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n% 
%prog% %infile% %outfile% %PHREEQCDAT%
REM Part B
set name=ex19b
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
%prog% %infile% %outfile% %PHREEQCDAT%
mv phreeqc.log %name%.log
if "%stop%"=="%n%" goto end

:test20
set n=20
set name=ex20a
del /Q %name%.* %name%*tsv
cp 
cp %data%\ex20-c13.tsv .
cp %data%\ex20-c14.tsv .
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n% 
%prog% %infile% %outfile% %ISODAT%
REM Part B
set name=ex20b
del /Q %name%.*
set infile=%data%\%name%
set outfile=%name%.out
%prog% %infile% %outfile% %ISODAT%
mv phreeqc.log %name%.log
if "%stop%"=="%n%" goto end

:test21
set n=21
set name=ex21
del /Q %name%.* %name%*tsv
cp %data%\ex21*.tsv .
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n% 
%prog% %infile% %outfile% %PHREEQCDAT%
if "%stop%"=="%n%" goto end

:test22
set n=22
set name=ex22
del /Q %name%.* %name%*tsv
cp %data%\co2.tsv .
set infile=%data%\%name%
set outfile=%name%.out
echo.
echo.
echo %divd%%divd%
echo Test run number %n% 
%prog% %infile% %outfile% %PHREEQCDAT%
if "%stop%"=="%n%" goto end

:end

call %topdir%\test\check %start% %stop%

rem  clear the variables used from the environment
set topdir=
set data=
set prog=
set prognm=
set divd=
set start=
set stop=
set n=
set name=