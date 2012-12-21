@echo off
rem  check.bat - batch program to compare original output files
rem              against newly created output files
rem
rem  called from test.bat; not run independently
rem
rem  Usage: check [start [stop]]
rem
rem         where: start = starting test number
rem                 stop = ending test number (may be same as start)
rem
rem  History: 00/02/16 mcrouse
rem           December 2012, PHREEQC3

set start=1
set stop=22
rem  if values supplied for start and stop, use those
if not "%1"=="" set start=%1
if not "%2"=="" set stop=%2

if exist check.log del check.log

set data=..\examples\examples_pc
set divd=---------------------------------------

echo Comparing test output with expected output...

goto test%start%

:test1
set n=1
set name=ex1
echo %divd%%divd%
echo Test case %n%
fc %data%\%name%.out %name%.out >> check.log
if "%stop%"=="%n%" goto end

:test2
set n=2
echo %divd%%divd%
echo Test case %n%
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
set name=ex2b
fc %data%\%name%.out %name%.out >> check.log
if "%stop%"=="%n%" goto end

:test3
set n=3
echo %divd%%divd%
echo Test case %n%
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
if "%stop%"=="%n%" goto end

:test4
set n=4
echo %divd%%divd%
echo Test case %n%
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
if "%stop%"=="%n%" goto end

:test5
set n=5
echo %divd%%divd%
echo Test case %n%
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
if "%stop%"=="%n%" goto end

:test6
set n=6
echo %divd%%divd%
echo Test case %n%
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%A-B.sel %name%A-B.sel >> check.log
fc %data%\%name%C.sel %name%C.sel >> check.log
if "%stop%"=="%n%" goto end

:test7
set n=7
echo %divd%%divd%
echo Test case %n%
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
if "%stop%"=="%n%" goto end

:test8
set n=8
echo %divd%%divd%
echo Test case %n%
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
if "%stop%"=="%n%" goto end

:test9
set n=9
echo %divd%%divd%
echo Test case %n%
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
if "%stop%"=="%n%" goto end

:test10
set n=10
echo %divd%%divd%
echo Test case %n%
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
if "%stop%"=="%n%" goto end

:test11
set n=11
echo %divd%%divd%
echo Test case %n%
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%adv.sel %name%adv.sel >> check.log
fc %data%\%name%trn.sel %name%trn.sel >> check.log
if "%stop%"=="%n%" goto end

:test12
set n=12
echo %divd%%divd%
echo Test case %n%
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
set name=ex%n%a
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
if "%stop%"=="%n%" goto end

:test13
set n=13
echo %divd%%divd%
echo Test case %n%
set name=ex%n%a
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
set name=ex%n%ac
fc %data%\%name%.out %name%.out >> check.log
set name=ex%n%b
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
set name=ex%n%c
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
if "%stop%"=="%n%" goto end

:test14
set n=14
echo %divd%%divd%
echo Test case %n%
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
if "%stop%"=="%n%" goto end

:test15
set n=15
echo %divd%%divd%
echo Test case %n%
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
set name=ex%n%a
fc %data%\%name%.out %name%.out >> check.log
set name=ex%n%b
fc %data%\%name%.out %name%.out >> check.log
if "%stop%"=="%n%" goto end

:test16
set n=16
echo %divd%%divd%
echo Test case %n%
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
if "%stop%"=="%n%" goto end

:test17
set n=17
echo %divd%%divd%
echo Test case %n%
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
set name=ex%n%b
fc %data%\%name%.out %name%.out >> check.log
if "%stop%"=="%n%" goto end

:test18
set n=18
echo %divd%%divd%
echo Test case %n%
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
if "%stop%"=="%n%" goto end

:test19
set n=19
echo %divd%%divd%
echo Test case %n%
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
set name=ex%n%b
fc %data%\%name%.out %name%.out >> check.log
if "%stop%"=="%n%" goto end

:test20
set n=20
echo %divd%%divd%
echo Test case %n%
set name=ex%n%a
fc %data%\%name%.out %name%.out >> check.log
set name=ex%n%b
fc %data%\%name%.out %name%.out >> check.log
if "%stop%"=="%n%" goto end

:test21
set n=21
echo %divd%%divd%
echo Test case %n%
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
if "%stop%"=="%n%" goto end

:test22
set n=22
echo %divd%%divd%
echo Test case %n%
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
if "%stop%"=="%n%" goto end

:end
echo Comparisons completed.  Examine file check.log for any differences.
echo.
echo Differences may indicate a problem with %prognm% installation.
echo.
