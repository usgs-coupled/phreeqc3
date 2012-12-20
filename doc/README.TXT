README.TXT

                                PHREEQC

       A program for speciation, batch-reaction, one-dimensional 
            transport, and inverse geochemical calculations

NOTE: This file describes the batch version of PHREEQC version 3. However, a
graphical user interface, PhreeqcI, is available that runs under Windows
operating systems. This interactive program can be downloaded from
http://wwwbrr.cr.usgs.gov/projects/GWC_coupled/phreeqc/index.html. PhreeqcI
has internal documentation for all PHREEQC data input and is a complete
interface to PHREEQC. 


                  PHREEQC - Version @VERSION@  @VER_DATE@

Instructions for installation, execution, and testing are provided
below.  After installation, see the phreeqc.txt file in the doc directory
of the PHREEQC installation for summary information on PHREEQC.

For assistance, enhancement requests, or to report bugs, contact 
dlpark@usgs.gov.


                          TABLE OF CONTENTS

                          A. Documentation
                          B. Windows: Installing and running PhreeqcI
                          C. Windows: Installing the batch version of PHREEQC
                          D. Linux: Extracting, compiling (optional), and installing 
                             the batch version of PHREEQC 
                          	D.1. Extracting files
                          	D.2. Compiling (optional)
                          	D.3. Installing
                          E. Windows and Linux: Running the batch program
                          F. Testing
                          	F.1. Windows
                          	F.2. Linux
                          	F.3. Example problems
                          G. Contacts

A. Documentation

   Brief description of the program PhreeqcI.

Charlton, S.R., and Parkhurst, D.L., 2002, PhreeqcI--A graphical user
   interface to the geochemical model PHREEQC: U.S. Geological Survey 
   Fact Sheet FS-031-02, 2 p.
   
   
   The following two user's guides are available in electronic format. Portable Document
   Format (PDF) files are included in the doc subdirectory of the PHREEQC program
   distribution.
   
Parkhurst, D.L., and Appelo, C.A.J., 2012, Description of input and examples for PHREEQC
   version 3—A computer program for speciation, batch-reaction, one- dimensional
   transport, and inverse geochemical calculations: U.S. Geological Survey Techniques
   and Methods, book 6, chap. A43, 497 p. http://pubs.usgs.gov/tm/06A43/.

Parkhurst, D.L., and Appelo, C.A.J., 1999, User's guide to PHREEQC (Version 2)--A
   computer program for speciation, batch-reaction, one-dimensional transport, and
   inverse geochemical calculations: U.S. Geological Survey Water-Resources
   Investigations Report 99-4259, 312 p. http://pubs.er.usgs.gov/publication/wri994259.


   The following two reports document the theory and implementation of isotopes in
   PHREEQC. Portable Document Format (PDF) of Thorstenson and Parkhurst (2002) is
   included in the doc subdirectory of the PHREEQC program distribution.

Thorstenson, D.C., and Parkhurst, D.L., 2002, Calculation of individual isotope
   equilibrium constants for implementation in geochemical models: U.S. Geological
   Survey Water-Resources Investigations Report 02-4172, 129 p.
   http://pubs.er.usgs.gov/publication/wri024172.

Thorstenson, D.C., and Parkhurst, D.L., 2004, Calculation of individual isotope
   equilibrium constants for geochemical reactions: Geochimica et Cosmochimica Acta, v.
   68, no. 11, p. 2449-2465.
   
B. Windows installation and running of the GUI PhreeqcI

Download and run the file phreeqci-@VERSION@.@REVISION@.msi. PhreeqcI will be installed
in the selected directory, C:\Program Files (x86)\USGS\Phreeqc Interactive
@VERSION@.@REVISION@ by default. The program can be executed from the Start menu.
   
C. Windows: installing and running PhreeqcI

Download and run the file phreeqc-@VERSION@.@REVISION@.msi. All of the files
needed to install and test PHREEQC will be installed in the selected
directory, C:\Program Files (x86)\USGS\phreeqc-@VERSION@.@REVISION@ by
default.

The following directory structure is created (the contents of each
directory are shown to the right):

   phreeqc-@VERSION@      files NOTICE.TXT, RELEASE.TXT, and this README.TXT
     `-----bin         compiled executable, template for execution script
     `-----database    database files required during execution
     `-----doc         documentation files 
     `-----examples    examples from user's guide--used in verification tests
     `-----src         Makefile and source code
     `-----test        scripts to run verification tests
     
Notes:  It is recommended that no user files be kept in the PHREEQC directory structure.
	If you plan to put files in the PHREEQC directory structure, do so only by
	creating subdirectories.
     
D. Linux: Extracting, compiling (optional), and installing the batch version of PHREEQC 

D.1. Linux: Extracting files

Compressed tar files are used to distribute the source code and versions of the software
compiled for Linux operating systems. All of the files needed to install and test
PHREEQC are contained in the file phreeqc-@VERSION@.@REVISION@.Linux.tar.gz. A Linux
executable file is included, which is compiled with @GCC_VER@ kernel @KERNEL_VER@. If
you want to compile the program for another operating system, the file
phreeqc-@VERSION@.@REVISION@.source.tar.gz contains all of the files needed to compile
and install PHREEQC. For all tar files, the directory phreeqc-@VERSION@ is created when
the files are extracted; if this directory already exists, you may want to delete or
rename it before extracting the files.

Follow the steps below to extract the files from a distribution tar
file. 

  Steps in extracting files                 Explanation
  ----------------------------------------  -----------------------------------
  gunzip phreeqc-@VERSION@-@REVISION@.Linux.tar.gz    Uncompress the distribution file.

  tar -xvpof phreeqc-@VERSION@-@REVISION@.Linux.tar   Extract files from the tar file.

The following directory structure is created (the contents of each
directory are shown to the right):

   phreeqc-@VERSION@      files NOTICE.TXT, RELEASE.TXT, and this README.TXT
     `-----bin         compiled executable, template for execution script
     `-----database    database files required during execution
     `-----doc         documentation files 
     `-----examples    examples from user's guide--used in verification tests
     `-----src         Makefile and source code
     `-----test        scripts to run verification tests

Notes:  (a) A compiled executable is not included in the source
            distribution.
        (b) It is recommended that no user files be kept in the PHREEQC
            directory structure.  If you plan to put files in the PHREEQC
            directory structure, do so only by creating subdirectories.

D.2. Linux: Compiling (optional)

XXXXXXXXXXXXXXX need to describe confinure here XXXXXXXXXXXXXXXXXX

If you have retrieved a pre-compiled distribution of the software, skip to
the Installing section below.

If a compiled version of the software is not available for your computer
or you want to build the executable yourself, follow the instructions in
this section.  The source distribution is provided for those users who
want the source code.  Little or no support is provided for users
generating their own versions of the software.  In general, to compile a
new version of the software, you will need:

       (a) a C compiler, and
       (b) a minimal level of knowledge of Make, the compiler, and
           the Linux operating system.

As provided in the source distribution, the software is set up to be
compiled under Linux with gcc.

To generate a new executable, do the following:

1.  Change directory to the source directory:
        cd phreeqc-@VERSION@/src

2.  Modify the beginning of the file named Makefile to correctly specify 
    system-dependent variables:
     
        CC             C compiler name
        CCFLAGS        C compiler flags
        LOADFLAGS      linker flags
        MULTICHART     Requires WINE and zedgraph .NET control
        INVERSE_CL1MP  Do not define if the gmp package 
                       is not available on your system

3.  Run make in the src directory to compile the source:

        make

    make will:

        a.  compile the source code, and
        b.  place the program executable in the PHREEQC bin directory.


D.3. Linux: Installing

To make PHREEQC easy to use, a link to the script that runs PHREEQC
should be placed in a directory that is included in each user's search
path. Run make in the PHREEQC src subdirectory to create the link:

    make install BINDIR=directory_for_links

A script for running the executable is placed in the PHREEQC installation
directory and a link to that script is placed in the directory defined
by BINDIR.  If each user's search path consists of

    /usr/bin:/usr/opt/bin:/usr/local/bin

using the command

    make install BINDIR=/usr/local/bin

will make PHREEQC accessible from any directory without requiring the full
pathname of the executable.  Note that to create and delete links to the
PHREEQC script, the installer must have sufficient rights to the BINDIR
directory. By default, BINDIR is $HOME/bin.


E. Linux and Windows: Running the batch program

If PHREEQC has been installed in a directory included in the users' PATH, the program can
be executed with any of the commands below.

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

For Linux, if the "make install" command described above has not been executed,
the executable may be copied to the current directory. The executable
is normally phreeqc-@VERSION@/bin/phreeqc. In addition the default database
file will be phreeqc.dat in the current directory. Phreeqc.dat is
installed in phreeqc-@VERSION@/database/phreeqc.dat.

The shell variable PHREEQC_DATABASE can be used to specify the default
database. In the C shell, this variable can be set with the command:

  setenv PHREEQC_DATABASE /home/jdoe/local/project/mydata.dat

In the Bourne or Korn shell, this variable can be set with the command:

  export PHREEQC_DATABASE=/home/jdoe/local/project/mydata.dat

The shell variable can be set permanently by including the appropriate
command in a file that is read when the shell is initiated, frequently
$HOME/.login or $HOME/.profile.  If this shell variable is not set,
the default database is set in the script in the installation directory
to phreeqc-@VERSION@/database/phreeqc.dat.  It is possible to specify a
different default database by editing the script.


F. Testing

Input files are provided to verify that the program is correctly installed and
running on the system.  The tests execute the examples presented in the user's
guide, which demonstrate most of the program's capabilities. The PHREEQC
"examples" directory contains the input data and the expected results for each
test. On Windows operating systems, the "examples" and "Win" directories have
been copied to My Documents\xxx, and the examples can be run from Win. 

F.1. Windows

On Windows operating systems, the "examples" and "Win" directories from
phreeqc-@VERSION@ have been copied to My Documents\xxx, and the examples can be
run from Win. Either double click the test.bat file from Windows Explorer or cd
to the directory in a command-prompt window and type ./test.sh [start [stop]]

where:  start = the number of the first test to perform, default = 1
        stop  = the number of the last test to perform, default = 22

For example:

     command                             what happens
     ----------------------------------  --------------------------------
     ./test.sh                           runs all of the tests
     ./test.sh 1 1                       runs the first test
     ./test.sh 2 3                       runs tests 2 and 3
     ./test.sh 4                         runs test 4 through the last test


You will have to close chart windows after each chart has completed to proceed to
the remaining examples. After the tests are completed, the results are compared
to the expected results (found in the examples directory).  See the file
check.log; if all goes well, there should be few or no differences. Differences
in path names and run times are insignificant.

To clean up after the tests, double click on clean.bat in Windows Explorer or
type the command ./clean.bat from a command prompt. 

F.2. Linux

On Linux, you should copy the directory phreeqc-@VERSION@/test to an area where
you have write access; the examples can be run from the copied directory. Change
to the directory that you copied and type the command:

     ./test.sh [start [stop]]

where:  start = the number of the first test to perform, default = 1
        stop  = the number of the last test to perform, default = 22

For example:

     command                             what happens
     ----------------------------------  --------------------------------
     ./test.sh                           runs all of the tests
     ./test.sh 1 1                       runs the first test
     ./test.sh 2 3                       runs tests 2 and 3
     ./test.sh 4                         runs test 4 through the last test


After the tests are completed, the results are compared to the expected results
(found in the examples directory).  See the file check.log; if all goes well,
there should be few or no differences. Differences in path names and run times
are insignificant.

To clean up after the tests, type the command:

     ./clean.sh

Notes: The output data files for phreeqc-@VERSION@.@REVISION@.source.tar.gz
       were created on a Linux system. You may notice slight numeric differences 
       in the results. These are generally due to different round-off algorithms
       and the different architecture of the central processing unit chip.
       Slight differences in output formats may occur on other computers,
       particularly for the value 0.0.

       Problem 9 generates a warning message that indicates negative 
       concentrations were generated in a kinetic run. The numerical
       method automatically reduces the step size until negative 
       concentrations are eliminated and an accurate kinetic integration
       is obtained.

       Problems 11, 12, 13, and 15 generate warning messages. The
       messages simply indicate a shorthand method for defining cell
       lengths and cell dispersivities was used.

F.3. Example problems

The tests are described in the table below, where 'test' corresponds
to the example number in the PHREEQC manual. 

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

 12   Advective and diffusive transport of heat and solutes. Constant
      boundary condition at one end, closed at other. The problem is 
      designed so that temperature should equal Na-conc (in mmol/kgw) 
      after diffusion.

 13   1D Transport in a Dual Porosity Column With Cation Exchange

 14   Transport with equilibrium_phases, exchange, and surface reactions

 15   1D Transport: Kinetic Biodegradation, Cell Growth, and Sorption 

 16   Inverse modeling of Sierra springs

 17   Inverse modeling of Black Sea water evaporation

 18   Inverse modeling of Madison aquifer
 
 19   Modeling Cd+2 sorption with Linear, Freundlich, and Langmuir isotherms, and
      with a deterministic distribution of sorption sites for organic matter,
      clay minerals, and iron oxyhydroxides
      
 20   Distribution of isotopes between water and calcite
 
 21   Modeling diffusion of HTO, 36Cl-, 22Na+, and Cs+ in a radial diffusion cell

 22   Modeling gas solubilities: CO2 at high pressures

G. CONTACTS

Inquiries about this software distribution should be directed to:

e-mail:  h2osoft@usgs.gov or dlpark@usgs.gov

