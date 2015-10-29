@echo off
setlocal ENABLEDELAYEDEXPANSION
set failed=0

set PATH_DIFF=C:\Program Files (x86)\GnuWin32\bin
set PATH=%PATH%;%PATH_DIFF%

cd mytest

REM Uncomment next line to force reldiff error
REM sed -i -e "s/8.0/7.0/g" alkalinity

for %%f in (*_101.sel) do (
  set ff=%%f
  call :test !ff:_101.sel=!
)
exit /b !failed!
goto :EOF

:test
echo Testing %1
move %1_101.sel %1_101.sel.expected > NUL
..\x64\Release\phreeqc %1 %1.out xxx %1.log
diff %1_101.sel %1_101.sel.expected > %1.diff
SET FILESIZE=%~z1
echo %FILESIZE%
reldiff %1
if %ERRORLEVEL% NEQ 0 (
  echo "  FAILED"
  set failed=1
)
goto :EOF

endlocal