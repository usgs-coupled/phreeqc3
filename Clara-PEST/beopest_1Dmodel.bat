@echo off
SETLOCAL

REM Sychronize latest WEBMOD executable, create the tsproc and PEST command files, and start beopest as master
REM Help printed if command entered with no arguments
IF %1.==. GOTO :USAGE

REM Set variables
set NODES=%1  rem nr of slaves
set PST=01_1D.pst
set HOME=%cd%
set HOME_SED=%HOME:\=/%
set "INPUT_DIR=%HOME%\input"
set INPUT_DIR_SED=%INPUT_DIR:\=/%
set "OUTPUT_DIR=%HOME%\output"
set OUTPUT_DIR_SED=%OUTPUT_DIR:\=/%
set "PEST_FILES_DIR=%HOME%\pest_files"
set PEST_FILES_DIR_SED=%PEST_FILES_DIR:\=/%
set "CONTROL_DIR=%HOME%\control"
set CONTROL_DIR_SED=%CONTROL:\=/%
set "PROJECT_DIR=%HOME%\pest_run_dir"
set PROJECT_DIR_SED=%PROJECT_DIR:\=/%
set "PEST_BIN_DIR=%HOME%\bin" 
set PEST_BIN_DIR_SED=%PEST_BIN_DIR:\=/%
set "PHREEQC_BIN=%HOME%\bin"


REM check chemistry files
for %%i in ( 
    %INPUT_DIR%\watercompaction.phr
    %INPUT_DIR%\phreeqc_Vm.dat
	%PEST_FILES_DIR%\compaction_definitions.phr.tpl.tpl
	%PEST_FILES_DIR%\01_1D.txt.ins
    ) do (
    if NOT exist %%i (
        echo Did not find chemistry file %%i
		echo Stopping.		
		goto :FINISH_UP		
    )
)

   
REM set port number
set PORT=4004 

REM setup output directory 
if NOT exist %OUTPUT_DIR% mkdir %OUTPUT_DIR%

REM setup working directory PROJECT_DIR=pest_run_dir
if exist %PROJECT_DIR% rmdir /s/q %PROJECT_DIR%
mkdir %PROJECT_DIR%

REM Sed to make compaction_definitions.phr.tpl in PROJECT_DIR
call :SED @phreeqcdatabase@ %INPUT_DIR%\phreeqc_Vm.dat       %PEST_FILES_DIR%\compaction_definitions.phr.tpl.tpl > %PROJECT_DIR%\compaction_definitions.phr.tpl
call :SED @PHREEQC_BIN@ %PHREEQC_BIN%     %PEST_FILES_DIR%\01_1D.bat.tpl >       %PROJECT_DIR%\01_1D.bat1
call :SED @INPUT_DIR@ %INPUT_DIR%     		%PROJECT_DIR%\01_1D.bat1 >       %PROJECT_DIR%\01_1D.bat
call :SED @PROJECT_DIR@ %PROJECT_DIR%     %PEST_FILES_DIR%\01_1D.pst.tpl >       %PROJECT_DIR%\01_1D.pst

DEL %PROJECT_DIR%\01_1D.bat1 

REM set up master directory
cd %PROJECT_DIR%

REM Copy files to run directory
copy %INPUT_DIR%\watercompaction.phr .
copy %PEST_FILES_DIR%\01_1D.txt.ins .
copy %PEST_FILES_DIR%\01_1D.rmf .

REM copy phreeqc.exe to run directory when using multiple machines in different locations
REM COPY %PHREEQC_BIN%\phreeqc.exe  .

REM make tmp dirs and run parallel pest workers
set MASTER=%COMPUTERNAME%
for /l %%X in (1, 1, %NODES%) do (
	if exist %PROJECT_DIR%\tmpest%%X rmdir /s/q %PROJECT_DIR%\tmpest%%X 
	mkdir %PROJECT_DIR%\tmpest%%X
	cd %PROJECT_DIR%\tmpest%%X
REM	copy %PROJECT_DIR%\01_1D.bat1 ./
	START /B %PEST_BIN_DIR%\beopest64.exe %PROJECT_DIR%\%PST% /H %MASTER%:%PORT% & cd %PROJECT_DIR%\..
	cd %PROJECT_DIR%\..
)

REM Run parallel pest Master /L means make optimum run in master
cd %PROJECT_DIR%
%PEST_BIN_DIR%\beopest64.exe %PROJECT_DIR%\%PST% /H /L /p1 :%PORT%

REM tidy up
IF "%ERRORLEVEL%" == "0" (
    if exist %PROJECT_DIR%\..\pest_results rmdir /s/q %PROJECT_DIR%\..\pest_results
    mkdir %PROJECT_DIR%\..\pest_results
    cd %PROJECT_DIR%
    copy *         ..\pest_results

    cd ..\pest_results
    mkdir pest_files
    copy ..\pest_files\*        .\pest_files

    cd   %PROJECT_DIR%\..
REM

) else (
    echo Beopest failed.
)

REM Done.
GOTO :FINISH_UP

:USAGE
echo.
echo usage: beopest_1Dmodel n 
echo where  n is number of slaves to start
GOTO :EOF

:SED
REM -- Prepare the Command Processor --
SETLOCAL ENABLEEXTENSIONS
SETLOCAL DISABLEDELAYEDEXPANSION

::BatchSubstitude - parses a File line by line and replaces a substring"
::syntax: BatchSubstitude.bat OldStr NewStr File
::          OldStr [in] - string to be replaced
::          NewStr [in] - string to replace with
::          File   [in] - file to be parsed
:$changed 20100115
:$source http://www.dostips.com
if "%~1"=="" findstr "^::" "%~f0"&GOTO:EOF
for /f "tokens=1,* delims=]" %%A in ('"type %3|find /n /v """') do (
    set "line=%%B"
    if defined line (
        call set "line=echo.%%line:%~1=%~2%%"
        for /f "delims=" %%X in ('"echo."%%line%%""') do %%~X
    ) ELSE echo.
)
EXIT /b

:FINISH_UP
cd %PROJECT_DIR%\..


