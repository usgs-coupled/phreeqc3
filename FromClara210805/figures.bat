REM tidy up
set PEST_BIN_DIR=C:\CapeCod\PEST\bin
set ROOT=01_1D
REM Plot residuals and correlation
    %PEST_BIN_DIR%\pest_plot %ROOT%.rei fit.pdf none
REM Plot parameters changes
    %PEST_BIN_DIR%\parm_plot %ROOT%.pst %ROOT%.sen par_calib.pdf none
REM Plot sensitivities
    %PEST_BIN_DIR%\sen_plot %ROOT%.sen sensitivity.pdf
REM Plot contributions to phi by observation group
    %PEST_BIN_DIR%\pcon_plot %ROOT%.rec phi.pdf none
REM Compute influence statistics
    %PEST_BIN_DIR%\infstat %ROOT% %ROOT%.infstat

