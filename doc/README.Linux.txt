README.TXT

                                PHREEQC

       A program for speciation, batch-reaction, one-dimensional 
            transport, and inverse geochemical calculations

This file describes the batch version of PHREEQC version 3 for Linux. No
graphical user interface exits for Linux.

(For Windows, a graphical user interface, PhreeqcI, is available at
http://wwwbrr.cr.usgs.gov/projects/GWC_coupled/phreeqc/index.html and a
Notepad++ interface is available at http://www.hydrochemistry.eu/downl.html.)


                  PHREEQC - Version @VERSION@  @VER_DATE@

Instructions for installation, execution, and testing are provided below. After
installation, see the phreeqc.txt file in the doc directory of the PHREEQC
installation for summary information on PHREEQC and the RELEASE.TXT file for
new features and bug fixes.

	  TABLE OF CONTENTS

	  A. Distribution files
	  B. Documentation
	  C. Extracting files
	  D. Compiling (optional)
	  E. Installing PHREEQC
	  F. Running the batch program
	  G. Testing and example problems
	  H. Contacts


A. Distribution files

	The following distribution packages (containing the software, test data
	sets, and information files) are currently available for Linux systems:

phreeqc-@VERSION@.@REVISION@.Linux.tar.gz--Compiled with @GCC_VER@ 
                                         kernel @KERNEL_VER@
phreeqc-@VERSION@.@REVISION@.source.tar.gz--Source code but no executable


B. Documentation

   *The following two user's guides are available in electronic format. Portable
   Document Format (PDF) files are included in the doc subdirectory of the
   PHREEQC program distribution.
   
Parkhurst, D.L., and Appelo, C.A.J., 2012, Description of input and examples
   for PHREEQC version 3--A computer program for speciation, batch-reaction,
   one- dimensional transport, and inverse geochemical calculations: U.S.
   Geological Survey Techniques and Methods, book 6, chap. A43, 497 p.
   http://pubs.usgs.gov/tm/06A43/.

Parkhurst, D.L., and Appelo, C.A.J., 1999, User's guide to PHREEQC (Version
   2)-- A computer program for speciation, batch-reaction, one-dimensional
   transport, and inverse geochemical calculations: U.S. Geological Survey
   Water-Resources Investigations Report 99-4259, 312 p.
   http://pubs.er.usgs.gov/publication/wri994259.


   *The following two reports document the theory and implementation of
   isotopes in PHREEQC. Portable Document Format (PDF) of Thorstenson and
   Parkhurst (2002) is included in the doc subdirectory of the PHREEQC program
   distribution.

Thorstenson, D.C., and Parkhurst, D.L., 2002, Calculation of individual isotope
   equilibrium constants for implementation in geochemical models: U.S.
   Geological Survey Water-Resources Investigations Report 02-4172, 129 p.
   http://pubs.er.usgs.gov/publication/wri024172.

Thorstenson, D.C., and Parkhurst, D.L., 2004, Calculation of individual isotope
   equilibrium constants for geochemical reactions: Geochimica et Cosmochimica
   Acta, v. 68, no. 11, p. 2449-2465.
   
   *Brief description of the program PhreeqcI.

Charlton, S.R., and Parkhurst, D.L., 2002, PhreeqcI--A graphical user interface
   to the geochemical model PHREEQC: U.S. Geological Survey Fact Sheet
   FS-031-02, 2 p.
    
   
C. Extracting files

Compressed tar files are used to distribute the source code and versions of the
software compiled for Linux operating systems. All of the files needed to
install and test PHREEQC are contained in the file
phreeqc-@VERSION@.@REVISION@.Linux.tar.gz. A Linux executable file is included,
which is compiled with @GCC_VER@ kernel @KERNEL_VER@. If you want to compile
the program for another operating system, the file
phreeqc-@VERSION@.@REVISION@.source.tar.gz contains all of the files needed to
compile and install PHREEQC. For all tar files, the directory phreeqc-@VERSION@
is created when the files are extracted; if this directory already exists, you
may want to delete or rename it before extracting the files.

Follow the steps below to extract the files from a distribution tar
file. 

  Steps in extracting files                 Explanation
  ----------------------------------------  -----------------------------------
  gunzip phreeqc-@VERSION@-@REVISION@.Linux.tar.gz   Uncompress the 
  						   distribution file.

  tar -xvpof phreeqc-@VERSION@-@REVISION@.Linux.tar  Extract files from the tar
                                                   file.

The following directory structure is created (the contents of each
directory are shown to the right):

   phreeqc-@VERSION@      files NOTICE.TXT, RELEASE.TXT, and this README.Linux.txt
     `--bin         compiled executable, template for execution script
     `--database    database files required during execution
     `--doc         documentation files 
     `--examples    examples from user's guide--used in verification tests
     `--src         Makefile and source code
     `--test        scripts to run verification tests

Notes:  (a) A compiled executable is not included in the source
            distribution.
        (b) It is recommended that no user files be kept in the PHREEQC
            directory structure.  If you plan to put files in the PHREEQC
            directory structure, do so only by creating subdirectories.

D. Compiling (optional)

XXXXXXXXXXXXXXX need to describe confinure here XXXXXXXXXXXXXXXXXX

If you can use the executable in the Linux.tar.gz version of the distribution
of the software, skip to the Installing section below.

If a compiled version of the software is not available for your computer or you
want to build the executable yourself, follow the instructions in this section.
The source distribution is provided for those users who want the source code.
Little or no support is provided for users generating their own versions of the
software.  In general, to compile a new version of the software, you will need:

       (a) a C compiler, and
       (b) a minimal level of knowledge of configure, Make, the compiler, and
           the Linux operating system.

As provided in the source distribution, a Makefile can be generated by
configure, and the Makefile can be used to compile the software.

To compile PHREEQC, do the following:

1.  Change directory to the source directory:
        cd phreeqc-@VERSION@/src

2.  Modify the beginning of the file named Makefile to correctly specify 
    system-dependent variables:
     
        CC             C compiler name
        CCFLAGS        C compiler flags
        LOADFLAGS      linker flags
        MULTICHART     Requires WINE and zedgraph .NET control
        INVERSE_CL1MP  Requires gmp (gnu multiprecision package).Do not define
		       if the gmp package is not available on your system

3.  Run "configure".

4.  Run "make" to compile the source:

        make

    make will:

        a.  compile the source code, and
        b.  place the program executable in the PHREEQC bin directory.


E. Installing PHREEQC

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


F. Running the program

If PHREEQC has been installed in a directory included in the users' PATH, the
program can be executed with any of the commands below.

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
the executable may be copied to the current directory. The executable is
normally phreeqc-@VERSION@/bin/phreeqcpp. In addition the default database
file, phreeqc.dat, may need to be in the current directory. Phreeqc.dat is
installed in phreeqc-@VERSION@/database/phreeqc.dat.

The shell variable PHREEQC_DATABASE can be used to specify the default
database. In the C shell, this variable can be set with the command:

  setenv PHREEQC_DATABASE /home/jdoe/local/project/mydata.dat

In the Bourne or Korn shell, this variable can be set with the command:

  export PHREEQC_DATABASE=/home/jdoe/local/project/mydata.dat

The shell variable can be set permanently by including the appropriate command
in a file that is read when the shell is initiated, frequently $HOME/.login or
$HOME/.profile.  If this shell variable is not set, the default database is set
in the script in the installation directory to
phreeqc-@VERSION@/database/phreeqc.dat.  It is possible to specify a different
default database by editing the script.

G. Testing and example problems

Input files are provided to verify that the program is correctly installed and
running on the system.  The tests execute the examples presented in the user's
guide, which demonstrate most of the program's capabilities. The PHREEQC
"examples" directory contains the input data and the expected results for each
test. 

On Linux, you should copy the directory phreeqc-@VERSION@/test to an area where
you have write access; the examples can be run from the copied directory.
Change to the directory that you copied and type the command:

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
       were created on a Linux system. You may notice slight numeric
       differences in the results. These are generally due to different round-
       off algorithms and the different architecture of the central processing
       unit chip. Slight differences in output formats may occur on other
       computers, particularly for the value 0.0.

       Problem 9 generates a warning message that indicates negative
       concentrations were generated in a kinetic run. The numerical method
       automatically reduces the step size until negative concentrations are
       eliminated and an accurate kinetic integration is obtained.

       Problems 11, 12, 13, and 15 generate warning messages. The messages
       simply indicate a shorthand method for defining cell lengths and cell
       dispersivities was used.

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

