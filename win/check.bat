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
rem

set start=%1
set stop=%2
if exist check.log del check.log

echo.
echo.
echo %divd%%divd%
echo Comparing test output with expected output...
echo.
echo.
goto test%start%

:test1
set n=1
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
if "%stop%"=="%n%" goto end

:test2
set n=2
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
if "%stop%"=="%n%" goto end

:test3
set n=3
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
if "%stop%"=="%n%" goto end

:test4
set n=4
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
if "%stop%"=="%n%" goto end

:test5
set n=5
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
if "%stop%"=="%n%" goto end

:test6
set n=6
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%A-B.sel %name%A-B.sel >> check.log
fc %data%\%name%C.sel %name%C.sel >> check.log
if "%stop%"=="%n%" goto end

:test7
set n=7
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
if "%stop%"=="%n%" goto end

:test8
set n=8
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
if "%stop%"=="%n%" goto end

:test9
set n=9
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
if "%stop%"=="%n%" goto end

:test10
set n=10
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
if "%stop%"=="%n%" goto end

:test11
set n=11
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%adv.sel %name%adv.sel >> check.log
fc %data%\%name%trn.sel %name%trn.sel >> check.log
if "%stop%"=="%n%" goto end

:test12
set n=12
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
for %%s in (a) do fc %data%\%name%%%s.out %name%%%s.out >> check.log
for %%s in (a) do fc %data%\%name%%%s.sel %name%%%s.sel >> check.log
if "%stop%"=="%n%" goto end

:test13
set n=13
set name=ex%n%
for %%s in (a b c) do fc %data%\%name%%%s.out %name%%%s.out >> check.log
for %%s in (a b c) do fc %data%\%name%%%s.sel %name%%%s.sel >> check.log
if "%stop%"=="%n%" goto end

:test14
set n=14
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
if "%stop%"=="%n%" goto end

:test15
set n=15
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
fc %data%\%name%.sel %name%.sel >> check.log
if "%stop%"=="%n%" goto end

:test16
set n=16
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
if "%stop%"=="%n%" goto end

:test17
set n=17
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
if "%stop%"=="%n%" goto end

:test18
set n=18
set name=ex%n%
fc %data%\%name%.out %name%.out >> check.log
if "%stop%"=="%n%" goto end

:end
echo Comparisons completed.  Examine file check.log for any differences.
echo.
echo Differences may indicate a problem with %prognm% installation.
echo.
