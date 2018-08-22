set f=%1
REM run original
C:/GitPrograms/phreeqc3/_build_vs2015_x64/Release/phreeqc.exe %f% 
move %f%.out %f%.out.expected
move %f%_101.sel %f%_101.sel.expected
move dump.out dump.out.expected
REM run copy version
C:/GitPrograms/phreeqc3/_build_vs2015_x64/debug/phreeqcd.exe %f% 
reldiff.exe %f%