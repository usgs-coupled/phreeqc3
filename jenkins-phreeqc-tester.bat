@echo off
setlocal ENABLEDELAYEDEXPANSION
set failed=0

set PATH_DIFF=C:\Program Files (x86)\GnuWin32\bin
set PATH=%PATH%;%PATH_DIFF%

cd mytest

REM sed -i -e "s/8.0/7.0/g" alkalinity

REM move alkalinity_101.sel alkalinity_101.sel.expected
REM ..\x64\Release\phreeqc alkalinity alkalinity.out xxx alkalinity.log
REM diff alkalinity_101.sel alkalinity_101.sel.expected > alkalinity.diff

REM @echo off

for %%f in (*_101.sel) do (
  set ff=%%f
  call :test !ff:_101.sel=!
)
echo failed=!failed!
exit /b !failed!
goto :EOF

:test
echo Testing %1
move %1_101.sel %1_101.sel.expected
..\x64\Release\phreeqc %1 %1.out xxx %1.log
diff %1_101.sel %1_101.sel.expected > %1.diff
if %ERRORLEVEL% NEQ 0 (
  echo "  FAILED"
  set failed=1
)
goto :EOF

endlocal