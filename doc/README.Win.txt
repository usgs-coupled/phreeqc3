README.TXT

                                PHREEQC

       A program for speciation, batch-reaction, one-dimensional 
            transport, and inverse geochemical calculations

NOTE: This file describes installing the GUI PhreeqcI and installing, compiling,
and testing the batch version of PHREEQC version 3 on Windows operating systems.
The GUI and batch version can be downloaded from
http://wwwbrr.cr.usgs.gov/projects/GWC_coupled/phreeqc/index.html. PhreeqcI has
screens for most PHREEQC keywords and allows input files that use any of PHREEQC
version 3 capabilities. Alternatively, a Notepad++ interface for PHREEQC version
3 is available at http://www.hydrochemistry.eu/downl.html.


                  PHREEQC - Version @VERSION@  @VER_DATE@

Instructions for installing, executing, and testing on Windows operating
systems are provided below. After installation, see the phreeqc.txt file in the
doc directory of the PHREEQC installation for summary information on PHREEQC
and the RELEASE.TXT file in the root directory for new features and bug fixes.


	  TABLE OF CONTENTS

	  A. Distribution files
	  B. Documentation
	  C. Installing and running the GUI PhreeqcI
	  D. Installing the batch version of PHREEQC
	  E. Compiling the batch version (optional)
	  F. Running the batch version
	  G. Testing and examples
	  H. Contacts


A. Distribution files

The following distribution files (containing the software, test data sets, and
information files) are available at
http://wwwbrr.cr.usgs.gov/projects/GWC_coupled/phreeqc/index.html.

phreeqci-@VERSION@-@REVISION@.msi--GUI compiled by using Microsoft Visual C++
                                2005

 phreeqc-@VERSION@-@REVISION@.msi--Batch version compiled by using Microsoft
				Visual C++ 2005, includes source code
                                
B. Documentation

*Brief description of the program PhreeqcI.

Charlton, S.R., and Parkhurst, D.L., 2002, PhreeqcI--A graphical user
   interface to the geochemical model PHREEQC: U.S. Geological Survey 
   Fact Sheet FS-031-02, 2 p.
   
   
*The following two user's guides are available in electronic format. Portable
   Document Format (PDF) files are included in the doc subdirectory of the
   PHREEQC program distribution.
   
Parkhurst, D.L., and Appelo, C.A.J., 2013, Description of input and examples
   for PHREEQC version 3--A computer program for speciation, batch-reaction,
   one- dimensional transport, and inverse geochemical calculations: U.S.
   Geological Survey Techniques and Methods, book 6, chap. A43, 497 p.
   http://pubs.usgs.gov/tm/06/a43/.

Parkhurst, D.L., and Appelo, C.A.J., 1999, User's guide to PHREEQC (Version 2)--
   A computer program for speciation, batch-reaction, one-dimensional transport,
   and inverse geochemical calculations: U.S. Geological Survey Water-Resources
   Investigations Report 99-4259, 312 p.
   http://pubs.er.usgs.gov/publication/wri994259.


*The following two reports document the theory and implementation of isotopes
   in PHREEQC. Portable Document Format (PDF) of Thorstenson and Parkhurst
   (2002) is included in the doc subdirectory of the PHREEQC program
   distribution.

Thorstenson, D.C., and Parkhurst, D.L., 2002, Calculation of individual isotope
   equilibrium constants for implementation in geochemical models: U.S.
   Geological Survey Water-Resources Investigations Report 02-4172, 129 p.
   http://pubs.er.usgs.gov/publication/wri024172.

Thorstenson, D.C., and Parkhurst, D.L., 2004, Calculation of individual isotope
   equilibrium constants for geochemical reactions: Geochimica et Cosmochimica
   Acta, v. 68, no. 11, p. 2449-2465.
   
   
C. Installing and running the GUI PhreeqcI

Note that if the software is to be installed on a server system, 
the person installing the software generally must have administrator 
rights.

To install the GUI, double-click phreeqci-@VERSION@-@REVISION@.msi to execute
the installation program and follow the directions on the screen. The GUI is
completely independent from the batch version.

To execute the GUI, access the program through the start menus of Windows. 

By default, the software is installed in the directory C:\Program
Files\USGS\phreeqci-@VERSION@-@REVISION@ for 32-bit operating systems or
C:\Program Files (x86)\USGS\phreeqci-@VERSION@-@REVISION@ for 64-bit operating
systems.  This is the recommended installation directory, but you may use other
directories or disk drives.

Note that program components not installed initially can be installed later,
and any components damaged (for example, by inadvertently overwriting the
directory) can be quickly reinstalled by double-clicking
phreeqci-@VERSION@-@REVISION@.msi--a maintenance version of the installation
program will be executed.


D. Installing and running the batch version of PHREEQC

Note that if the software is to be installed on a server system, the person
installing the software generally must have administrator rights.

To install the software, double-click phreeqc-@VERSION@-@REVISION@.msi to execute
the installation program and follow the directions on the screen. The
installation program will modify the PATH environment variable so that
the PHREEQC program can be executed from a command prompt window from any
directory without needing to type the full pathname of the program's location.
The batch version is completely independent from the GUI version.

By default, the software is installed in the directory C:\Program
Files\USGS\phreeqc-@VERSION@-@REVISION@ (32-bit operating system) or C:\Program
Files (x86)\USGS\phreeqc-@VERSION@-@REVISION@ (64-bit operating system). This is
the recommended installation directory, but you may use other directories or
disk drives. The following documentation assumes default installation for a 64-
bit operating system.

Note that program components not installed initially can be installed later,
and any components damaged (for example, by inadvertently overwriting the
directory) can be quickly reinstalled by double-clicking
phreeqc-@VERSION@-@REVISION@.msi--a maintenance version of the installation
program will be executed.

The following directory structure will be created (the contents of each
directory are shown to the right):

C:\Program Files (x86)\USGS\phreeqc-@VERSION@-@REVISION@
                          Files NOTICE.TXT, RELEASE.TXT, this file
			  (README.TXT), and Visual Studio project files
             
     --bin                phreeqc.bat, batch file for executing PHREEQC
       --ClrRelease       phreeqc.exe, executable with charting
       --Release          phreeqc.exe, executable without charting (runs faster)    
     --database           Database files
     --doc                Documentation files 
     --examples           Examples from user's guide--used in verification tests
     --src                Source code
     --test               Batch files to run verification tests

Note:  It is recommended that no user files be kept in the PHREEQC
       directory structure.  If you plan to put files in the PHREEQC
       directory structure, do so only by creating subdirectories.

     
E. Compiling the batch version

The source code is provided so that users can generate an executable
themselves.  Little or no support is provided for users generating
their own versions of the software.  In general, to compile a new
version of PHREEQC, you will need:

       (a) a C++ compiler,
       (b) familiarity with the compiler and the Windows operating system.

A Visual Studio project (phreeqc.sln) is included in the top-level directory
	for compilation on Windows operating systems.


F. Running the batch version

The batch program can be executed from a command-prompt window with any of the
commands below.

  command to execute PHREEQC     explanation
  -----------------------------  -----------------------------------------
  phreeqc                        The program will query for each of the
                                 needed files.

  phreeqc input                  The input file is named input, the output
                                 file will be named input.out and the
                                 default database file will be used.
  
  phreeqc input output           The input file is named input, the output
                                 file is named output, and the default
                                 database file will be used.
  
  phreeqc input output database  All file names are specified explicitly.
  
  phreeqc input output database screen_output     

                                 All file names are specified explicitly,
                                 and screen output is directed to the
                                 file screen_output.
                                 
Three alternative options exist for running the program: (1) the full pathname to
the batch file can be used in place of "phreeqc" in the above commands
("C:\Program Files (x86)USGS\phreeqc-@VERSION@-@REVISION@\bin\phreeqc.bat").
Note that because of the space present in "Program Files", quotation marks must
be used to enter the full pathname); (2) the full path to one of the executable
files can be used in place of "phreeqc" in the above commands 
("C:\Program Files (x86)USGS\phreeqc-@VERSION@-@REVISION@\bin\Release\phreeqc.exe",
for example); or (3) an executable ("C:\Program Files (x86)\USGS\
phreeqc-@VERSION@-@REVISION@\bin\ClrRelease\phreeqc.exe, for example) may be
copied to the current directory and executed with .\phreeqc. The database file
may need to be in the current directory. The default database file is installed
in C:\Program Files (x86)\USGS\phreeqc-@VERSION@-@REVISION@\database\phreeqc.dat.

Two executables are distributed in subdirectories of the bin directory. The
executable (phreeqc.exe) in \bin\ClrRelease has charting capabilities. The
executable in \bin\Release does not have charting capabilities. The version
without charting will run up to two times faster than the charting version and
may be preferred for long calculations that do not require charting.

The environmental variable PHREEQC_DATABASE can be used to specify the
default database for a DOS window session.  This environmental variable
can be set with the command:

  set PHREEQC_DATABASE=C:\mydirectory\myproject\mydata.dat

When using the batch file, phreeqc.bat, this environmental variable takes
precedence over any default definition of the database file. If PHREEQC is
invoked with at least three arguments, the third argument is the database file
and it takes precedence over the environmental variable or the default
database.


G. Testing and examples

Input files are provided to verify that the program is correctly installed and
running on the system.  The tests execute the examples presented in the user's
guide, which demonstrate most of the program's capabilities. The PHREEQC
"examples" directory contains the input data and the expected results for each
test. 

To run the examples, copy the "test" directory (C:\Program Files (x86)
\USGS\phreeqc-@VERSION@-@REVISION@\test) to a location where you have
read/write access. The examples can be run from the new directory either by
double clicking the test.bat file from Windows Explorer or by changing
directories to the new directory in a command-prompt window and typing the
following:

test.bat [start [stop [no_chart]]]

where:  start    is the number of the first test to perform, default = 1
        stop     is the number of the last test to perform, default = 22
        no_chart is any alphanumeric string, which causes no charts to be 
                 generated.

For example:

     command                            what happens
     ---------------------------------  --------------------------------
     test.bat                           runs all of the tests
     test.bat 1 1                       runs the first test
     test.bat 2 3                       runs tests 2 and 3
     test.bat 4                         runs test 4 through the last test
     test.bat 1 10 no                   runs tests 1 through 10 without charts

You will have to close chart windows after each chart has completed to proceed
to the remaining examples. After the tests are completed, the results can be
compared to the expected results (found in the examples directory) by executing
the check.bat batch file. See the file check.log for differences. Differences
in path names and run times are insignificant. The results in the directory
examples were calculated with the charting version of PHREEQC. Comparing non-
charting results to charting results may produce a large number of differences
in check.log; however, in almost all cases, the differences are related to the
calculated pe. The pe may differ significantly and many aqueous species
concentrations may differ, but the concentrations of aqueous species should
differ by less than 1e-8 mol/kgw.

To clean up after the tests, double click on clean.bat in Windows Explorer or
type the command clean.bat from a command prompt. 

The tests are described in the table below, where 'test' corresponds
to the example number in the PHREEQC manual. Charts can be compared to charts
in the manual (Techniques and Methods 6-A43).

test  description of test and files                  
----  --------------------------------------------   
  1   Add uranium and speciate seawater

  2   Temperature dependence of solubility                            
      of gypsum and anhydrite                                         
                                                                      
  3   Mixing seawater and groundwater
                                                                      
  4   Evaporation and solution volume

  5   Add oxygen, equilibrate with pyrite, calcite, and goethite.
                                                                      
  6   Reaction path calculations with K-feldspar to phase boundaries                                    

  7   Organic decomposition with fixed-pressure and
      fixed-volume gas phases  
                                                                      
  8   Sorption of zinc on hydrous iron oxides                         
                                                                      
  9   Kinetically controlled oxidation of ferrous iron. 
      Decoupled valence states of iron.                                      
                                                                       
 10   Solid solution of strontianite and aragonite.
                                                                      
 11   Transport and ion exchange.

 12   Advective and diffusive transport of heat and solutes. Constant boundary
      condition at one end, closed at other. The problem is designed so that
      temperature should equal Na-conc (in mmol/kgw) after diffusion.

 13   1D Transport in a Dual Porosity Column With Cation Exchange

 14   Transport with equilibrium_phases, exchange, and surface reactions

 15   1D Transport: Kinetic Biodegradation, Cell Growth, and Sorption 

 16   Inverse modeling of Sierra springs

 17   Inverse modeling of Black Sea water evaporation

 18   Inverse modeling of Madison aquifer
 
 19   Modeling Cd+2 sorption with Linear, Freundlich, and Langmuir isotherms,
      and with a deterministic distribution of sorption sites for organic
      matter, clay minerals, and iron oxyhydroxides
      
 20   Distribution of isotopes between water and calcite
 
 21   Modeling diffusion of HTO, 36Cl-, 22Na+, and Cs+ in a radial diffusion
      cell

 22   Modeling gas solubilities: CO2 at high pressures


H. CONTACTS

Inquiries about this software distribution should be directed to:

e-mail:  h2osoft@usgs.gov or dlpark@usgs.gov

