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
set topdir=%PHREEQC_ROOT%
set data=%topdir%\examples
set prog="%topdir%\bin\ClrRelease\phreeqc.exe"
set prog_nochart="%topdir%\bin\Release\phreeqc.exe"
set prognm=PHREEQC
set DB=%topdir%\database

set divd=---------------------------------------

rem  assign valid test range values as default values
set start=1
set stop=22
rem  if values supplied for start and stop, use those
if not "%1"=="" set start=%1
if not "%2"=="" set stop=%2
if not "%3"=="" set prog=%prog_nochart% 

echo %divd%%divd%
echo Begin processing %prognm% test runs %start% to %stop%
goto test%start%

:test1
set n=1
set name=ex1
if exist   %name%.log           del      %name%.log
if exist   ex1.out              del      ex1.out 
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n%
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
if "%stop%"=="%n%" goto end

:test2
set n=2
set name=ex2
if exist   %name%.log           del      %name%.log
if exist   ex2.out              del      ex2.out 
if exist   ex2.sel              del      ex2.sel 
if exist   ex2b.out             del      ex2b.out 
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n% 
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
REM Part b
set name=ex2b
if exist   %name%.log           del      %name%.log
if exist   %name%.tsv           del      %name%.tsv
copy "%data%\%name%.tsv" .
set infile=%data%\%name%
set outfile=%name%.out
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
if "%stop%"=="%n%" goto end

:test3
set n=3
set name=ex3
if exist   %name%.log           del      %name%.log
if exist   ex3.out              del      ex3.out
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n%
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
if "%stop%"=="%n%" goto end

:test4
set n=4
set name=ex4
if exist   %name%.log           del      %name%.log
if exist   ex4.out              del      ex4.out 
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n%
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
if "%stop%"=="%n%" goto end

:test5
set n=5
set name=ex5
if exist   %name%.log           del      %name%.log
if exist   ex5.out              del      ex5.out 
if exist   ex5.sel              del      ex5.sel 
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n%
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
if "%stop%"=="%n%" goto end

:test6
set n=6
set name=ex6
if exist   %name%.log           del      %name%.log
if exist   ex6A-B.sel           del      ex6A-B.sel 
if exist   ex6C.sel             del      ex6C.sel 
if exist   ex6.out              del      ex6.out
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n% 
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
if "%stop%"=="%n%" goto end

:test7
set n=7
set name=ex7
if exist   %name%.log           del      %name%.log
if exist   ex7.out              del      ex7.out 
if exist   ex7.sel              del      ex7.sel
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n%
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
if "%stop%"=="%n%" goto end

:test8
set n=8
set name=ex8
if exist   %name%.log           del      %name%.log
if exist   ex8.out              del      ex8.out 
if exist   ex8.sel              del      ex8.sel 
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n%
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
if "%stop%"=="%n%" goto end

:test9
set n=9
set name=ex9
if exist   %name%.log           del      %name%.log
if exist   ex9.out              del      ex9.out 
if exist   ex9.sel              del      ex9.sel
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n%
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
if "%stop%"=="%n%" goto end

:test10
set n=10
set name=ex10
if exist   %name%.log           del      %name%.log
if exist   ex10.out             del      ex10.out 
if exist   ex10.sel             del      ex10.sel 
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n%
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
if "%stop%"=="%n%" goto end

:test11
set n=11
set name=ex11
if exist   %name%.log           del      %name%.log
if exist   ex11adv.sel          del      ex11adv.sel 
if exist   ex11.out             del      ex11.out 
if exist   ex11trn.sel          del      ex11trn.sel 
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n%
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
if "%stop%"=="%n%" goto end

:test12
set n=12
set name=ex12
if exist   %name%.log           del      %name%.log
if exist   ex12a.out            del      ex12a.out 
if exist   ex12a.sel            del      ex12a.sel 
if exist   ex12.out             del      ex12.out 
if exist   ex12.sel             del      ex12.sel
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n% 
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
REM Part A
set name=ex12a
if exist   %name%.log           del      %name%.log
set infile=%data%\%name%
set outfile=%name%.out
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
if "%stop%"=="%n%" goto end

:test13
set n=13
set name=ex13a
if exist   %name%.log           del      %name%.log
if exist   ex13ac.out           del      ex13ac.out 
if exist   ex13a.out            del      ex13a.out 
if exist   ex13a.sel            del      ex13a.sel 
if exist   ex13b.out            del      ex13b.out 
if exist   ex13b.sel            del      ex13b.sel 
if exist   ex13c.out            del      ex13c.out 
if exist   ex13c.sel            del      ex13c.sel 
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n% 
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
REM Part B
set name=ex13b
if exist   %name%.log           del      %name%.log
set infile=%data%\%name%
set outfile=%name%.out
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
REM Part C
set name=ex13c
if exist   %name%.log           del      %name%.log
set infile=%data%\%name%
set outfile=%name%.out
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
REM Part A-C
set name=ex13ac
if exist   %name%.log           del      %name%.log
set infile=%data%\%name%
set outfile=%name%.out
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
if "%stop%"=="%n%" goto end

:test14
set n=14
set name=ex14
if exist   %name%.log           del      %name%.log
if exist   ex14.out             del      ex14.out 
if exist   ex14.sel             del      ex14.sel 
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n% 
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
if "%stop%"=="%n%" goto end

:test15
set n=15
set name=ex15
if exist   %name%.log           del      %name%.log
if exist   ex15.out             del      ex15.out 
if exist   ex15.sel             del      ex15.sel 
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n% 
echo    %name%
%prog% "%infile%" %outfile% "%data%\ex15.dat" %name%.log
REM Part A
set name=ex15a
if exist   %name%.log           del      %name%.log
set infile=%data%\%name%
set outfile=%name%.out
echo    %name%
%prog% "%infile%" %outfile% "%data%\ex15.dat" %name%.log
REM Part B
set name=ex15b
if exist   %name%.log           del      %name%.log
set infile=%data%\%name%
set outfile=%name%.out
echo    %name%
%prog% "%infile%" %outfile% "%data%\ex15.dat" %name%.log
if "%stop%"=="%n%" goto end

:test16
set n=16
set name=ex16
if exist   %name%.log           del      %name%.log
if exist   ex16.out             del      ex16.out 
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n% 
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
if "%stop%"=="%n%" goto end

:test17
set n=17
set name=ex17
if exist   %name%.log           del      %name%.log
if exist   ex17b.out            del      ex17b.out 
if exist   ex17.out             del      ex17.out 
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n% 
echo    %name%
%prog% "%infile%" %outfile% "%DB%\pitzer.dat" %name%.log
REM Part B
set name=ex17b
if exist   %name%.log           del      %name%.log
set infile=%data%\%name%
set outfile=%name%.out
echo    %name%
%prog% "%infile%" %outfile% "%DB%\pitzer.dat" %name%.log
if "%stop%"=="%n%" goto end

:test18
set n=18
set name=ex18
if exist   %name%.log           del      %name%.log
if exist   ex18.out             del      ex18.out 
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n% 
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
if "%stop%"=="%n%" goto end

:test19
set n=19
set name=ex19
if exist   %name%.log           del      %name%.log
if exist   ex19b.out            del      ex19b.out 
if exist   ex19.out             del      ex19.out 
if exist   ex19_meas.tsv        del      ex19_meas.tsv
copy "%data%\ex19_meas.tsv" .
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n% 
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
REM Part B
set name=ex19b
if exist   %name%.log           del      %name%.log
set infile=%data%\%name%
set outfile=%name%.out
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
if "%stop%"=="%n%" goto end

:test20
set n=20
set name=ex20a
if exist   %name%.log           del      %name%.log
if exist   ex20a.out            del      ex20a.out 
if exist   ex20b.out            del      ex20b.out
if exist   ex20-c13.tsv         del      ex20-c13.tsv
if exist   ex20-c14.tsv         del      ex20-c14.tsv
copy "%data%\ex20-c13.tsv" .
copy "%data%\ex20-c14.tsv" .
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n% 
echo    %name%
%prog% "%infile%" %outfile% "%DB%\iso.dat" %name%.log
REM Part B
set name=ex20b
if exist   %name%.log           del      %name%.log
set infile=%data%\%name%
set outfile=%name%.out
echo    %name%
%prog% "%infile%" %outfile% "%DB%\iso.dat" %name%.log
if "%stop%"=="%n%" goto end

:test21
set n=21
set name=ex21
if exist   %name%.log           del      %name%.log
if exist   ex21.out             del      ex21.out 
if exist   ex21_Cl_tr_rad.tsv   del      ex21_Cl_tr_rad.tsv
if exist   ex21_Cs_rad.tsv      del      ex21_Cs_rad.tsv
if exist   ex21_HTO_rad.tsv     del      ex21_HTO_rad.tsv
if exist   ex21_Na_tr_rad.tsv   del      ex21_Na_tr_rad.tsv
copy "%data%\ex21*.tsv" .
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n% 
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
if "%stop%"=="%n%" goto end

:test22
set n=22
set name=ex22
if exist   %name%.log           del      %name%.log
if exist   ex22.out             del      ex22.out
if exist   co2.tsv              del      co2.tsv
copy "%data%\co2.tsv" .
set infile=%data%\%name%
set outfile=%name%.out
echo %divd%%divd%
echo Test run number %n% 
echo    %name%
%prog% "%infile%" %outfile% "%DB%\phreeqc.dat" %name%.log
if "%stop%"=="%n%" goto end

:end

echo %divd%%divd%
echo Done running.

call .\check %start% %stop%
echo Done checking.

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