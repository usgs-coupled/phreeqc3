@echo off
rem  clean.bat - clean up files after test runs
rem  Usage:  clean
rem  History: 00/02/16 mcrouse

rem  remove output from each test
for %%n in (1 2 3 4 5 6 7 8 9 10 11 12 14 15 16 17 18) do  if exist ex%%n.out del ex%%n.out
for %%n in (12a 13a 13b 13c) do  if exist ex%%n.out del ex%%n.out
for %%n in (2 5 7 8 9 10 12 12a 14 15) do  if exist ex%%n.sel del ex%%n.sel
for %%n in (6A-B 6C) do  if exist ex%%n.sel del ex%%n.sel
for %%n in (11adv 11trn) do  if exist ex%%n.sel del ex%%n.sel
for %%n in (13a 13b 13c) do  if exist ex%%n.sel del ex%%n.sel

if exist check.log del check.log
if exist phreeqc.log del phreeqc.log
