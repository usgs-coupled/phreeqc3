# Microsoft Developer Studio Project File - Name="phreeqcpp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=phreeqcpp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "phreeqcpp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "phreeqcpp.mak" CFG="phreeqcpp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "phreeqcpp - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "phreeqcpp - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
RSC=rc.exe

!IF  "$(CFG)" == "phreeqcpp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE F90 /compile_only /nologo /warn:nofileopt
# ADD F90 /compile_only /nologo /warn:nofileopt
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "phreeqcpp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE F90 /check:bounds /compile_only /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD F90 /check:bounds /compile_only /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "C:\xerces-c-windows_2000-msvc_60\include" /I "$(DEV_GMP_INC)" /I "c:\phreeqcpp\src\phreeqc" /D "WIN32_MEMORY_DEBUG" /D "INVERSE_CL1MP" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_2.lib gmpDebug.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"$(DEV_GMP_LIB)" /libpath:"C:\xerces-c-windows_2000-msvc_60\lib" /libpath:"C:\xerces-c-windows_2000-msvc_60\bin"

!ENDIF 

# Begin Target

# Name "phreeqcpp - Win32 Release"
# Name "phreeqcpp - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;f90;for;f;fpp"
# Begin Source File

SOURCE=.\src\phreeqc\advection.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\basic.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\basicsubs.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\cl1.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\cl1mp.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\cvdense.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\cvode.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\dense.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\dw.c
# End Source File
# Begin Source File

SOURCE=.\src\Exchange.cxx
# End Source File
# Begin Source File

SOURCE=.\src\ExchComp.cxx
# End Source File
# Begin Source File

SOURCE=.\src\GasPhase.cxx
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\input.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\integrate.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\inverse.c
# End Source File
# Begin Source File

SOURCE=.\src\ISolution.cxx
# End Source File
# Begin Source File

SOURCE=.\src\ISolutionComp.cxx
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\isotopes.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\kinetics.c
# End Source File
# Begin Source File

SOURCE=.\src\KineticsComp.cxx
# End Source File
# Begin Source File

SOURCE=.\src\KineticsCxx.cxx
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\main.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\mainsubs.c
# End Source File
# Begin Source File

SOURCE=.\src\Mix.cxx
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\model.c
# End Source File
# Begin Source File

SOURCE=.\src\NameDouble.cxx
# End Source File
# Begin Source File

SOURCE=.\src\NumKeyword.cxx
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\nvector.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\nvector_serial.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\output.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\p2clib.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\parse.c
# End Source File
# Begin Source File

SOURCE=.\src\Parser.cxx
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\phqalloc.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\phreeqc_files.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\pitzer.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\pitzer_structures.c
# End Source File
# Begin Source File

SOURCE=.\src\PPassemblage.cxx
# End Source File
# Begin Source File

SOURCE=.\src\PPassemblageComp.cxx
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\prep.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\print.c
# End Source File
# Begin Source File

SOURCE=.\src\Reaction.cxx
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\read.c
# End Source File
# Begin Source File

SOURCE=.\src\ReadClass.cxx
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\readtr.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\smalldense.c
# End Source File
# Begin Source File

SOURCE=.\src\Solution.cxx
# End Source File
# Begin Source File

SOURCE=.\src\SolutionIsotope.cxx
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\spread.c
# End Source File
# Begin Source File

SOURCE=.\src\SSassemblage.cxx
# End Source File
# Begin Source File

SOURCE=.\src\SSassemblageSS.cxx
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\step.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\structures.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\sundialsmath.c
# End Source File
# Begin Source File

SOURCE=.\src\Surface.cxx
# End Source File
# Begin Source File

SOURCE=.\src\SurfCharge.cxx
# End Source File
# Begin Source File

SOURCE=.\src\SurfComp.cxx
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\tally.c
# End Source File
# Begin Source File

SOURCE=.\src\Temperature.cxx
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\tidy.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\transport.c
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\utilities.c
# End Source File
# Begin Source File

SOURCE=.\src\Utils.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\src\char_star.h
# End Source File
# Begin Source File

SOURCE=.\src\cvdense.h
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\cvdense.h
# End Source File
# Begin Source File

SOURCE=.\src\cvode.h
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\cvode.h
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\dense.h
# End Source File
# Begin Source File

SOURCE=.\src\Exchange.h
# End Source File
# Begin Source File

SOURCE=.\src\ExchComp.h
# End Source File
# Begin Source File

SOURCE=.\src\GasPhase.h
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\global.h
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\input.h
# End Source File
# Begin Source File

SOURCE=.\src\ISolution.h
# End Source File
# Begin Source File

SOURCE=.\src\ISolutionComp.h
# End Source File
# Begin Source File

SOURCE=.\src\kinetics.h
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\kinetics.h
# End Source File
# Begin Source File

SOURCE=.\src\KineticsComp.h
# End Source File
# Begin Source File

SOURCE=.\src\KineticsCxx.h
# End Source File
# Begin Source File

SOURCE=.\src\Mix.h
# End Source File
# Begin Source File

SOURCE=.\src\NameDouble.h
# End Source File
# Begin Source File

SOURCE=.\src\NumKeyword.h
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\nvector.h
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\nvector_serial.h
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\output.h
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\p2c.h
# End Source File
# Begin Source File

SOURCE=.\src\Parser.h
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\phqalloc.h
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\phrqproto.h
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\phrqtype.h
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\pitzer.h
# End Source File
# Begin Source File

SOURCE=.\src\PPassemblage.h
# End Source File
# Begin Source File

SOURCE=.\src\PPassemblageComp.h
# End Source File
# Begin Source File

SOURCE=.\src\Reaction.h
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\smalldense.h
# End Source File
# Begin Source File

SOURCE=.\src\Solution.h
# End Source File
# Begin Source File

SOURCE=.\src\SolutionIsotope.h
# End Source File
# Begin Source File

SOURCE=.\src\SSassemblage.h
# End Source File
# Begin Source File

SOURCE=.\src\SSassemblageSS.h
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\sundialsmath.h
# End Source File
# Begin Source File

SOURCE=.\src\phreeqc\sundialstypes.h
# End Source File
# Begin Source File

SOURCE=.\src\Surface.h
# End Source File
# Begin Source File

SOURCE=.\src\SurfCharge.h
# End Source File
# Begin Source File

SOURCE=.\src\SurfComp.h
# End Source File
# Begin Source File

SOURCE=.\src\Temperature.h
# End Source File
# Begin Source File

SOURCE=.\src\Utils.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
