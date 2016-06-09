if exist 01_1D.txt (
    DEL 01_1D.txt
    )
@PHREEQC_BIN@\phreeqc @INPUT_DIR@\watercompaction.phr
if errorlevel 1 goto failure
if errorlevel 0 goto end
:failure
del 01_1D.txt
:end
