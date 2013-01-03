@echo off
setlocal
set TD=%~dp0

REM ---------------------------
REM Strip quotes
REM ---------------------------
if not defined PHREEQC_DATABASE goto strip
CALL :do_strip PHREEQC_DATABASE
goto strip
 
:do_strip
for /f "delims=" %%d in ('echo %%%1%%') do set %1=%%~d
Goto :eof

:strip

REM ---------------------------
REM Show usage every time.
REM ---------------------------
echo.
echo Usage:  phreeqc [input [output [database [screen_output]]]]
echo.
echo Default database file may be set with 
echo environmental variable PHREEQC_DATABASE.
echo.

REM ---------------------------
REM Determine how to run
REM ---------------------------
if not "%~4"=="" goto R_4
if not "%~3"=="" goto R_3
if not "%~2"=="" goto R_2
if not "%~1"=="" goto R_1
goto R_0

REM ************************  BEG R_4  ******************************
:R_4
REM ---------------------------
REM Check for input file
REM ---------------------------
if exist "%~1" goto R_4_1_OK
	echo.
	echo ERROR, input file not found: "%~1" .
	echo.
	goto END
:R_4_1_OK

REM ---------------------------
REM Check for database
REM ---------------------------
if exist "%~3" goto R_4_DEF
	echo.
	echo ERROR, database file not found: "%~3" .
	echo.
	goto END
:R_4_DEF

REM ---------------------------
REM Check if %~2 is writable
REM ---------------------------
copy "%~1" "%~2" > nul
if errorlevel 1 goto R_4_ERR1
if errorlevel 0 goto R_4_ERR0
:R_4_ERR1
	echo.
	echo ERROR, output file not writable:  "%~2" .
	echo.
	goto END
:R_4_ERR0

REM ---------------------------
REM Check if %~4 is writable
REM ---------------------------	
copy "%~1" "%~4" > nul
if errorlevel 1 goto R_4_ERR2
if errorlevel 0 goto R_4_ERR3
:R_4_ERR2
	echo.
	echo ERROR, screen output file not writable:  "%~4" .
	echo.
	goto END
:R_4_ERR3
	echo.
	echo Screen output sent to file:  "%~4" .
	echo.
	"%TD%ClrRelease\phreeqc" "%~1" "%~2" "%~3" "%~4"
	goto END
REM ************************  END R_4  ******************************


REM ************************  BEG R_3  ******************************
:R_3
REM ---------------------------
REM Check for input file
REM ---------------------------
if exist "%~1" goto R_3_1_OK
	echo.
	echo ERROR, input file not found: "%~1" .
	echo.
	goto END
:R_3_1_OK

REM ---------------------------
REM Check for database
REM ---------------------------
if exist "%~3" goto R_3_DEF
	echo.
	echo ERROR, database file not found: "%~3" .
	echo.
	goto END
:R_3_DEF

REM ---------------------------
REM Check if %~2 is writable
REM ---------------------------
copy "%~1" "%~2" > nul
if errorlevel 1 goto R_3_ERR1
if errorlevel 0 goto R_3_ERR0
:R_3_ERR1
	echo.
	echo ERROR, output file not writable:  "%~2" .
	echo.
	goto END
:R_3_ERR0
	"%TD%ClrRelease\phreeqc" "%~1" "%~2" "%~3"
	goto END
REM ************************  END R_3  ******************************


REM ************************  BEG R_2  ******************************
:R_2
REM ---------------------------
REM Check for input file
REM ---------------------------
if exist "%~1" goto R_2_1_OK
	echo.
	echo ERROR, input file not found: "%~1"
	echo.
	goto END
:R_2_1_OK

REM ---------------------------
REM Check if %~2 is writable
REM ---------------------------
copy "%~1" "%~2" > nul
if errorlevel 1 goto R_2_ERR1
if errorlevel 0 goto R_2_ERR0
:R_2_ERR1
	echo.
	echo ERROR, output file not writable:  "%~2" .
	echo.
	goto END
:R_2_ERR0
	del "%~2"

REM ---------------------------
REM Check for database
REM ---------------------------
if "%PHREEQC_DATABASE%"=="" goto T_2_DEF
if exist "%PHREEQC_DATABASE%" goto R_2_ENV
	echo.
	echo ERROR, database file not found: "%PHREEQC_DATABASE%" .
	echo.
	goto END
:T_2_DEF
if exist "%TD%database\phreeqc.dat" goto R_2_DEF
	echo.
	echo ERROR, database file not found: "%TD%database\phreeqc.dat" .
	echo.
	goto END
:R_2_DEF
	"%TD%ClrRelease\phreeqc" "%~1" "%~2" "%TD%database\phreeqc.dat"
	goto END
:R_2_ENV
	"%TD%ClrRelease\phreeqc" "%~1" "%~2" "%PHREEQC_DATABASE%"
goto END
REM ************************  END R_2  ******************************


REM ************************  BEG R_1  ******************************
:R_1
REM ---------------------------
REM Check for input file
REM ---------------------------
if exist "%~1" goto R_1_1_OK
	echo.
	echo ERROR, input file not found: "%~1" .
	echo.
	goto END
:R_1_1_OK

REM ---------------------------
REM Check if %~1.out is writable
REM ---------------------------
copy "%~1" "%~1.out" > nul
if errorlevel 1 goto R_1_ERR1
if errorlevel 0 goto R_1_ERR0
:R_1_ERR1
	echo.
	echo ERROR, output file not writable: "%~1.out" .
	echo.
	goto END
:R_1_ERR0
	del "%~1.out"

REM ---------------------------
REM Check for database
REM ---------------------------
if "%PHREEQC_DATABASE%"=="" goto T_1_DEF
if exist "%PHREEQC_DATABASE%" goto R_1_ENV
	echo.
	echo ERROR, database file not found: "%PHREEQC_DATABASE%" .
	echo.
	goto END
:T_1_DEF
if exist "%TD%database\phreeqc.dat" goto R_1_DEF
	echo.
	echo ERROR, database file not found: "%TD%database\phreeqc.dat" .
	echo.
	goto END
:R_1_DEF
	"%TD%ClrRelease\phreeqc" "%~1" "%~1.out" "%TD%database\phreeqc.dat"
	goto END
:R_1_ENV
	"%TD%ClrRelease\phreeqc" "%~1" "%~1.out" "%PHREEQC_DATABASE%"
	goto END
REM ************************  END R_1  ******************************


REM ************************  BEG R_0  ******************************
:R_0
REM ---------------------------
REM Check for database
REM ---------------------------
if "%PHREEQC_DATABASE%"=="" goto T_0_DEF
if exist "%PHREEQC_DATABASE%" goto R_0_ENV
	echo.
	echo ERROR, database file not found: "%PHREEQC_DATABASE%" .
	echo.
	goto END
:T_0_DEF
if exist "%TD%database\phreeqc.dat" goto R_0_DEF
	echo.
	echo ERROR, database file not found: "%TD%database\phreeqc.dat" .
	echo.
	goto END
:R_0_DEF
	set PHREEQC_DATABASE=%TD%database\phreeqc.dat
	"%TD%ClrRelease\phreeqc"
	goto END
:R_0_ENV
	"%TD%ClrRelease\phreeqc"
	goto END
REM ************************  END R_0  ******************************


REM ***********************  CLEAN UP  ******************************
:END
endlocal