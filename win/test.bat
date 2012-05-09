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
rem
set topdir=..
set data=%topdir%\examples
set prog=%topdir%\src\release\phreeqc.exe
set prognm=PHREEQC
set divd=---------------------------------------

rem  assign valid test range values as default values
set start=1
set stop=18
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
set name=ex%n%
set infile=%data%\%name%
set outfile=%name%.out
if exist %outfile% del %outfile%
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %topdir%\phreeqc.dat
if "%stop%"=="%n%" goto end

:test2
set n=2
set name=ex%n%
set infile=%data%\%name%
set outfile=%name%.out
if exist %outfile% del %outfile%
if exist %name%.sel del %name%.sel 
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %topdir%\phreeqc.dat
if "%stop%"=="%n%" goto end

:test3
set n=3
set name=ex%n%
set infile=%data%\%name%
set outfile=%name%.out
if exist %outfile% del %outfile%
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %topdir%\phreeqc.dat
if "%stop%"=="%n%" goto end

:test4
set n=4
set name=ex%n%
set infile=%data%\%name%
set outfile=%name%.out
if exist %outfile% del %outfile%
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %topdir%\phreeqc.dat
if "%stop%"=="%n%" goto end

:test5
set n=5
set name=ex%n%
set infile=%data%\%name%
set outfile=%name%.out
if exist %outfile% del %outfile%
if exist %name%.sel del %name%.sel 
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %topdir%\phreeqc.dat
if "%stop%"=="%n%" goto end

:test6
set n=6
set name=ex%n%
set infile=%data%\%name%
set outfile=%name%.out
if exist %outfile% del %outfile%
if exist %name%A-B.sel del %name%A-B.sel 
if exist %name%C.sel del %name%C.sel 
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %topdir%\phreeqc.dat
if "%stop%"=="%n%" goto end

:test7
set n=7
set name=ex%n%
set infile=%data%\%name%
set outfile=%name%.out
if exist %outfile% del %outfile%
if exist %name%.sel del %name%.sel 
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %topdir%\phreeqc.dat
if "%stop%"=="%n%" goto end

:test8
set n=8
set name=ex%n%
set infile=%data%\%name%
set outfile=%name%.out
if exist %outfile% del %outfile%
if exist %name%.sel del %name%.sel 
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %topdir%\phreeqc.dat
if "%stop%"=="%n%" goto end

:test9
set n=9
set name=ex%n%
set infile=%data%\%name%
set outfile=%name%.out
if exist %outfile% del %outfile%
if exist %name%.sel del %name%.sel 
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %topdir%\phreeqc.dat
if "%stop%"=="%n%" goto end

:test10
set n=10
set name=ex%n%
set infile=%data%\%name%
set outfile=%name%.out
if exist %outfile% del %outfile%
if exist %name%.sel del %name%.sel 
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %topdir%\phreeqc.dat
if "%stop%"=="%n%" goto end

:test11
set n=11
set name=ex%n%
set infile=%data%\%name%
set outfile=%name%.out
if exist %outfile% del %outfile%
if exist %name%adv.sel del %name%adv.sel 
if exist %name%trn.sel del %name%trn.sel 
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %topdir%\phreeqc.dat
if "%stop%"=="%n%" goto end

:test12
set n=12
set name=ex%n%
set infile=%data%\%name%
set outfile=%name%.out
if exist %outfile% del %outfile%
if exist %name%.sel del %name%.sel 
for %%s in (a) do if exist %name%%%s.out del %name%%%s.out
for %%s in (a) do if exist %name%%%s.sel del %name%%%s.sel
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %topdir%\phreeqc.dat
for %%s in (a) do %prog% %infile%%%s %name%%%s.out %topdir%\phreeqc.dat
if "%stop%"=="%n%" goto end

:test13
set n=13
set name=ex%n%
set infile=%data%\%name%
for %%s in (a b c) do if exist %name%%%s.out del %name%%%s.out
for %%s in (a b c) do if exist %name%%%s.sel del %name%%%s.sel
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
for %%s in (a b c) do %prog% %infile%%%s %name%%%s.out %topdir%\phreeqc.dat
if "%stop%"=="%n%" goto end

:test14
set n=14
set name=ex%n%
set infile=%data%\%name%
set outfile=%name%.out
if exist %outfile% del %outfile%
if exist %name%.sel del %name%.sel 
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %topdir%\wateq4f.dat
if "%stop%"=="%n%" goto end

:test15
set n=15
set name=ex%n%
set infile=%data%\%name%
set outfile=%name%.out
if exist %outfile% del %outfile%
if exist %name%.sel del %name%.sel 
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %data%\ex15.dat
if "%stop%"=="%n%" goto end

:test16
set n=16
set name=ex%n%
set infile=%data%\%name%
set outfile=%name%.out
if exist %outfile% del %outfile%
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %topdir%\phreeqc.dat
if "%stop%"=="%n%" goto end

:test17
set n=17
set name=ex%n%
set infile=%data%\%name%
set outfile=%name%.out
if exist %outfile% del %outfile%
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %topdir%\phreeqc.dat
if "%stop%"=="%n%" goto end

:test18
set n=18
set name=ex%n%
set infile=%data%\%name%
set outfile=%name%.out
if exist %outfile% del %outfile%
echo.
echo.
echo %divd%%divd%
echo Test run number %n%
%prog% %infile% %outfile% %topdir%\phreeqc.dat
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
