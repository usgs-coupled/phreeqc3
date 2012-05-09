README.TXT

                                PHREEQC

       A program for speciation, batch-reaction, one-dimensional 
            transport, and inverse geochemical calculations

NOTE: This file describes the batch version of PHREEQC. However, a
graphical user interface, PhreeqcI, is available that runs under
Windows operating systems. This interactive program can be downloaded
from http://wwwbrr.cr.usgs.gov/projects/GWC_coupled/phreeqc/index.html. 
PhreeqcI has internal documentation for all PHREEQC data input and is
a complete interface to PHREEQC. A brief description of the program
has been published as a Fact Sheet:

Charlton, S.R., and Parkhurst, D.L., 2002, PhreeqcI--A graphical user
   interface to the geochemical model PHREEQC: U.S. Geological Survey 
   Fact Sheet FS-031-02, 2 p.

             PHREEQC - Version @VERSION@  @VER_DATE@

Instructions for installation, execution, and testing are provided
below.  After installation, see the file doc\phreeqc.txt in the PHREEQC
installation directory for summary information on PHREEQC.

For assistance, enhancement requests, or to report bugs, contact the
Hydrologic Analysis Software Support Program by sending e-mail to
h2osoft@usgs.gov or dlpark@usgs.gov.

For all versions of Microsoft Windows, this Windows console version of
PHREEQC must be run by using an MS-DOS Command Prompt window.


                            TABLE OF CONTENTS

                         A. SYSTEM REQUIREMENTS
                         B. DISTRIBUTION FILE
                         C. DOCUMENTATION
                         D. INSTALLING
                             o Installing PHREEQC
                             o PHREEQC directory structure
                         E. COMPILING
                         F. RUNNING THE SOFTWARE
                         G. TESTING
                         H. CONTACTS


A. SYSTEM REQUIREMENTS

For installation of PHREEQC, 10 megabytes of free disk space is needed.

To run PHREEQC, the following are necessary:

  - Windows 2000 or higher
  - processor running at 100 megahertz or faster
  - 8 megabytes RAM
  - 20 megabytes free disk space


B. DISTRIBUTION FILE

The following distribution file (containing the software, test data sets,
and information files) is currently available.

  phreeqc-@VERSION@-@REVISION@.msi - Compiled using Microsoft Visual C++ 2005, includes source code


C. DOCUMENTATION

Parkhurst, D.L., and Appelo, C.A.J., 1999, User's guide to PHREEQC
   (Version 2)--a computer program for speciation, batch-reaction,
   one-dimensional transport, and inverse geochemical calculations:
   U.S. Geological Survey Water-Resources Investigations Report 99-4259,
   312 p.

   This user's guide is available in electronic format. A Portable
   Document Format (PDF) file is included in the doc subdirectory of
   the PHREEQC program distribution. An online HTML version of the
   User's Guide can also be found at
   http://wwwbrr.cr.usgs.gov/projects/GWC_coupled/phreeqc/

Thorstenson, D.C., and Parkhurst, D.L., 2002, Calculation of
   individual isotope equilibrium constants for implementation in
   geochemical models: U.S. Geological Survey Water-Resources
   Investigations Report 02-4172, 129 p.

   This report documents the theory and implementation of isotopes 
   in PHREEQC. Portable Document Format (PDF) is included in the 
   doc subdirectory of the PHREEQC program distribution. 

D. INSTALLING

Installing PHREEQC
------------------

Note that if the software is to be installed on a server system, 
the person installing the software generally must have administrator 
rights.

To install the software, double-click phreeqc-@VERSION@-@REVISION@.msi to execute
the installation program and follow the directions on the screen.
The installation program optionally will modify the PATH environment
variable so that the PHREEQC program can be executed from any directory,
without needing to type the full pathname of the program's location.

By default, the software is installed in the directory
C:\Program Files\USGS\phreeqc-@VERSION@.  This is the recommended installation 
directory, but you may use other directories or disk drives.

Note that program components not installed initially can be installed
later, and that any components damaged by, for example, being
inadvertently overwritten, can be quickly reinstalled by double-clicking
phreeqc-@VERSION@-@REVISION@.msi--a maintenance version of the installation program
will be executed.


PHREEQC directory structure
---------------------------

The following directory structure will be created (the contents of each
directory are shown to the right):

   phreeqc-@VERSION@         files NOTICE.TXT, RELEASE.TXT, and this README.TXT;
                          batch file to run PHREEQC, database files
     `-----doc            documentation files 
     `-----examples       examples from user's guide--used in verification tests
     `-----src            Makefile and source code
      |     `-----release compiled executable
     `-----test           batch files to run verification tests

Note:  It is recommended that no user files be kept in the PHREEQC
       directory structure.  If you plan to put files in the PHREEQC
       directory structure, do so only by creating subdirectories.


E. COMPILING

The source code is provided so that users can generate the executable
themselves.  Little or no support is provided for users generating
their own versions of the software.  In general, to compile a new
version of the software, you will need:

       (a) a C compiler,
       (b) familiarity with the compiler and the Windows operating system.

A Makefile used to compile the program under Linux is provided as an example
in the phreeqc-@VERSION@-@REVISION@.msi distribution, 


F. RUNNING THE SOFTWARE


If the option to modify the PATH environment variable was chosen
during installation, then the following commands may be used to run 
PHREEQC from an MS-DOS Command Prompt window.

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

If the option to modify the PATH environment variable was not chosen
during installation, then two options exist: (1) the full pathname 
to the script that runs phreeqc can be used in place of "phreeqc" in
the above commands (for example "c:\program files\usgs\phreeqc-@VERSION@\phreeqc". 
Note that because of the space present in "program files", quotation
marks must be used to enter the full pathname.), or, (2) the
executable (installed in phreeqc-@VERSION@\src\release\phreeqc.exe) may be
copied to the current directory; further, the default database file
must be in the current directory and named "phreeqc.dat". The default
database file is installed in phreeqc-@VERSION@\phreeqc.dat.

The environment variable PHREEQC_DATABASE can be used to specify the
default database for a DOS window session.  This environment variable
can be set with the command:

  set PHREEQC_DATABASE=c:\mydirectory\myproject\mydata.dat

This environment variable takes precedence over any default definition
of the database file. If PHREEQC is invoked with at least three
arguments, the third argument is the database file and it takes
precedence over the environment variable or the default database.


G. TESTING


Test data sets are provided to verify that the program is correctly installed
and running on the system.  The tests execute the examples presented in
the user's guide, which demonstrate most of the program's capabilities.
The tests are run in the phreeqc-@VERSION@\test directory.  The directory
phreeqc-@VERSION@\examples contains the PHREEQC input data and expected results
for each test.

To test the installation, in an MS-DOS Command Prompt window change to
the phreeqc-@VERSION@\test directory:

     c:
     cd "program files"
     cd usgs\phreeqc-@VERSION@\test

Execute the tests by typing the command:

     test [start [stop]]

where:  start = the number of the first test to perform, default = 1
        stop  = the number of the last test to perform, default = 18

For example:

     command                             what happens
     ----------------------------------  --------------------------------
     test                                runs all of the tests
     test 1 1                            runs the first test
     test 2 3                            runs tests 2 and 3
     test 4                              runs test 4 through the last test


After the tests are completed, the results are compared to the
expected results (found in the PHREEQC examples directory).  See the
file check.log; if all goes well, there should be no differences.


Note:  Sometimes different processors, or different compilers cause
       minor numerical differences in the results or differences in
       representation of exponents. In general, concentrations should
       differ by less than about 1e-14 (mol/kg water) between two sets
       of results. The pe may vary several units in some systems
       without redox buffering; however, all concentrations should
       still differ by only small amounts.

       Problem 9 generates a warning message that indicates negative 
       concentrations were generated in a kinetic run. The numerical
       method automatically reduces the step size until negative 
       concentrations are eliminated and an accurate kinetic integration
       is obtained.

       Problems 11, 12, 13, and 15 generate warning messages. The
       messages simply indicate a shorthand method for defining cell
       lengths and cell dispersivities was used in a transport 
       calculation.


To clean up after the tests, type the command:

     clean

The tests are described in the table below.  Test is the test number and
the usage column indicates how a file is used, with i for input and o for
output.

test  description of test and files                  file name & usage
----  --------------------------------------------   -----------------
  1   Add uranium and speciate seawater

      PHREEQC input file                             ex1             i
      Printed results of calculation                 ex1.out         o
                                                                      
  2   Temperature dependence of solubility                            
      of gypsum and anhydrite                                         
                                                                      
      PHREEQC input file                             ex2             i
      Printed results of calculation                 ex2.out         o
      Results written to the selected-output file    ex2.sel         o

  3   A. Calcite equilibrium at log Pco2 = -2.0 and 25C
      B. Definition of seawater
      C. Mix 70% ground water, 30% seawater
      D. Equilibrate mixture with calcite and dolomite
      E. Equilibrate mixture with calcite only

      PHREEQC input file                             ex3             i
      Printed results of calculation                 ex3.out         o
                                                                      
  4   A. Rain water evaporation                                       
      B. Factor of 20 more solution                                   
                                                                      
      PHREEQC input file                             ex4             i
      Printed results of calculation                 ex4.out         o

  5   Add oxygen, equilibrate with pyrite, calcite, and goethite.

      PHREEQC input file                             ex5             i
      Printed results of calculation                 ex5.out         o
      Results written to the selected-output file    ex5.sel         o
                                                                      
  6   6A. React to phase boundaries                                    
      6A1.--Find amount of k-feldspar dissolved to                         
            reach gibbsite saturation                                  
      6A2.--Find amount of k-feldspar dissolved to                         
            reach kaolinite saturation                                 
      6A3.--Find amount of k-feldspar dissolved to                         
            reach K-mica saturation                                 
      6A4.--Find amount of k-feldspar dissolved to                         
            reach k-feldspar saturation                                    
      6A5.--Find point with kaolinite present,                         
            but no gibbsite                                            
      6A6.--Find point with K-mica present,                         
            but no kaolinite                                           
      6B. Path between phase boundaries                                
      6C. Kinetic calculation
                                                                      
      PHREEQC input file                             ex6             i
      Printed results of calculation                 ex6.out         o
      Results written to the selected-output file    ex6A-B.sel      o
      Results written to the selected-output file    ex6C.sel        o

  7   Organic decomposition with fixed-pressure and
      fixed-volume gas phases  

      PHREEQC input file                             ex7             i
      Printed results of calculation                 ex7.out         o
      Results written to the selected-output file    ex7.sel         o
                                                                      
  8   Sorption of zinc on hydrous iron oxides                         
                                                                      
      PHREEQC input file                             ex8             i
      Printed results of calculation                 ex8.out         o
      Results written to the selected-output file    ex8.sel         o
                                                                      
  9   Kinetically controlled oxidation of ferrous iron. 
      Decoupled valence states of iron.                                      
                                                                      
      PHREEQC input file                             ex9             i
      Printed results of calculation                 ex9.out         o
      Results written to the selected-output file    ex9.sel         o
                                                                      
 10   Solid solution of strontianite and aragonite.
                                                                      
      PHREEQC input file                             ex10            i
      Printed results of calculation                 ex10.out        o
      Results written to the selected-output file    ex10.sel        o
                                                                      
 11   Transport and ion exchange.
                                                                      
      PHREEQC input file                             ex11            i
      Printed results of calculation                 ex11.out        o
      Results written to the selected-output file    ex11adv.sel     o
      Results written to the selected-output file    ex11trn.sel     o

 12   Advective and diffusive transport of heat and solutes. Constant
      boundary condition at one end, closed at other. The problem is 
      designed so that temperature should equal Na-conc (in mmol/kgw) 
      after diffusion.

      PHREEQC input file                             ex12            i
      Printed results of calculation                 ex12.out        o
      Results written to the selected-output file    ex12.sel        o

 13   A. 1 mmol/l NaCl/NO3 enters column with stagnant zones. Implicit 
         definition of first-order exchange model.
      B. 1 mmol/l NaCl/NO3 enters column with stagnant zones. Explicit 
         definition of first-order exchange factors.
      C. 1 mmol/l NaCl/NO3 enters column with stagnant zones. 5 layer 
         stagnant zone with finite differences.

      PHREEQC input file                             ex13a           i
      Printed results of calculation                 ex13a.out       o
      Results written to the selected-output file    ex13a.sel       o
      PHREEQC input file                             ex13b           i
      Printed results of calculation                 ex13b.out       o
      Results written to the selected-output file    ex13b.sel       o
      PHREEQC input file                             ex13c           i
      Printed results of calculation                 ex13c.out       o
      Results written to the selected-output file    ex13c.sel       o

 14   Transport with equilibrium_phases, exchange, and surface reactions

      PHREEQC input file                             ex14            i
      Printed results of calculation                 ex14.out        o
      Results written to the selected-output file    ex14.sel        o

 15   1D Transport: Kinetic Biodegradation, Cell Growth, and Sorption 

      PHREEQC input file                             ex15            i
      database file                                  ex15.dat        i
      Printed results of calculation                 ex15.out        o
      Results written to the selected-output file    ex15.sel        o

 16   Inverse modeling of Sierra springs

      PHREEQC input file                             ex16            i
      Printed results of calculation                 ex16.out        o

 17   Inverse modeling of Black Sea water evaporation

      PHREEQC input file                             ex17            i
      Printed results of calculation                 ex17.out        o

 18   Inverse modeling of Madison aquifer

      PHREEQC input file                             ex18            i
      Printed results of calculation                 ex18.out        o


H. CONTACTS

Inquiries about this software distribution should be directed to:

e-mail:  h2osoft@usgs.gov or dlpark@usgs.gov
