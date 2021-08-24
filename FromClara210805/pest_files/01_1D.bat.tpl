if exist 01_1D.txt (
    DEL 01_1D.txt
    )
@PHREEQC_BIN@\phreeqc sedimentation.phr
@PHREEQC_BIN@\TsvData
del all.pst
del *.ins
copy 01_1D.txt 01_1D.txt.save
copy sedimentation.phr.out sedimentation.phr.out.save
REM if errorlevel 1 goto failure
REM if errorlevel 0 goto end
REM :failure
REM del 01_1D.txt
REM :end
ECHO "Done with 01_1d.bat"
EXIT