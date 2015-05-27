README.TXT

                                PHREEQC

       A program for speciation, batch-reaction, one-dimensional 
            transport, and inverse geochemical calculations

This file describes batch versions of PHREEQC version 3 for Linux. The @M32@
(32-bit) and @M64@ (64-bit) versions, referred to as "Linux" versions, contain
an executable file compiled for Linux, whereas the plain .tar.gz version,
referred to as the "source" version, has no executable file, but has the files
necessary to configure and compile PHREEQC on any Unix operating system.

Instructions for installing, executing, and testing on Linux operating
systems are provided below. After installation, see the root or doc directory
of the PHREEQC installation for summary information on PHREEQC in phreeqc.txt
and the new features and bug fixes in RELEASE.TXT.

No graphical user interface exits for Linux. (For Windows, a graphical user
interface, PhreeqcI, is available at
http://wwwbrr.cr.usgs.gov/projects/GWC_coupled/phreeqc/index.html and a
Notepad++ interface is available at http://www.hydrochemistry.eu/downl.html.)


	  TABLE OF CONTENTS

	  A. Distribution files
	  B. Documentation
	  C. Linux versions (with executable)
  		C.1. Extracting files
  		C.2. Running
  		C.2. Testing
	  D. Source version (for Unix compilation)
	  	D.1. Extracting files
	  	D.2. Compiling, testing, installing
		D.3. Running
	  E. Description of test cases
	  F. Contacts


A. Distribution files

	The following distribution packages (containing the software, test data
	sets, and information files) are currently available for Linux systems:

phreeqc-@VERSION@-@REVISION@.@M32@.tar.gz--Compiled with @GCC_VER@ 
                                         kernel @KERNEL_VER@
phreeqc-@VERSION@-@REVISION@.@M64@.tar.gz--Compiled with @GCC_VER_64@ 
                                         kernel @KERNEL_VER_64@                                         
phreeqc-@VERSION@-@REVISION@.tar.gz      --Source code, but no executable


B. Documentation

   *The following two user's guides are available in electronic format. Portable
   Document Format (PDF) files are included in the doc subdirectory of the
   PHREEQC program distribution.
   
Parkhurst, D.L., and Appelo, C.A.J., 2013, Description of input and examples
   for PHREEQC version 3--A computer program for speciation, batch-reaction,
   one- dimensional transport, and inverse geochemical calculations: U.S.
   Geological Survey Techniques and Methods, book 6, chap. A43, 497 p.
   http://pubs.usgs.gov/tm/06/a43/.

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
    
   
C. Linux versions (with executable)

Compressed tar files, phreeqc-@VERSION@-@REVISION@.Linux.tar.gz and
phreeqc-@VERSION@-@REVISION@.Linux.x86_64.tar.gz, are used to distribute the
source code and the compiled executable for Linux operating systems. The 32-bit
Linux executable file is compiled with 
@GCC_VER@ kernel @KERNEL_VER_64@ 
and the 64-bit Linux executable file is compiled with 
@GCC_VER_64@ kernel @KERNEL_VER@.
All of the files needed to run and test PHREEQC are contained in each tar file.
(If you want to compile the program yourself, the file
phreeqc-@VERSION@-@REVISION@.tar.gz contains all of the files needed to
configure, compile, test, and install PHREEQC. See Section D for details.)


C.1. Extracting files

Follow the steps below to extract the files from a distribution tar
file. 

  Steps in extracting files                 	 Explanation
  ----------------------------------------  	 -----------------------------------
  For the 32-bit version:

  gunzip phreeqc-@VERSION@-@REVISION@.i686.tar.gz          Uncompress the tar.gz file.

  tar -xvpof phreeqc-@VERSION@-@REVISION@.i686.tar         Extract files from the tar file.
                                                   
  or, for the 64-bit version:
  
  gunzip phreeqc-@VERSION@-@REVISION@.x86_64.tar.gz        Uncompress the tar.gz file.

  tar -xvpof phreeqc-@VERSION@-@REVISION@.x86_64.tar       Extract files from the tar file. 

The directory phreeqc-@VERSION@-@REVISION@ is created when the files are
extracted; if this directory already exists, you may want to delete or rename
it before extracting the files.

The following directory structure is created (the contents of each
directory are shown to the right):

   phreeqc-@VERSION@-@REVISION@      Files NOTICE.TXT, RELEASE, and (this file) README 
     `--bin         Compiled executable
     `--database    Database files (one is required for execution)
     `--doc         Documentation files 
     `--examples    Examples from user's guide--used in verification tests
     `--test        Scripts to run verification tests

Notes:  It is recommended that no user files be kept in the PHREEQC directory
	structure.  If you plan to put files in the PHREEQC directory
	structure, do so only by creating subdirectories.


C.2. Running 

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

If the directory containing the executable file is not included in the PATH
environmental variable, you can (1) use the complete path name to the
executable in place of "phreeqc" above, or (2) copy the executable to the
current directory. The executable is phreeqc-@VERSION@-@REVISION@/bin/phreeqc.
In addition a database file may need to be in the current directory.
Phreeqc.dat is generally used by default and is found at
phreeqc-@VERSION@-@REVISION@/database/phreeqc.dat.

The shell variable PHREEQC_DATABASE can be used to specify the default database.
In the C shell, this variable can be set (assuming the tar file was extracted in
the directory /home/jdoe) with a command such as:

  setenv PHREEQC_DATABASE /home/jdoe/phreeqc-@VERSION@-@REVISION@/database/phreeqc.dat

In the Bourne or Korn shell, this variable can be set with the command:

  export PHREEQC_DATABASE=/home/jdoe/phreeqc-@VERSION@-@REVISION@/database/phreeqc.dat

The shell variable can be set permanently by including the appropriate command
in a file that is read when the shell is initiated, frequently $HOME/.login or
$HOME/.profile. If this shell variable is not set, the default database is
assumed to be phreeqc.dat in the current directory.  


C.3. Testing 

Input files are provided to verify that the program is correctly installed and
running on the system.  The tests execute the examples presented in the user's
guide, which demonstrate most of the program's capabilities. The directory
phreeqc-@VERSION@-@REVISION@/examples contains the input data for each test.

Provided that the directory phreeqc-@VERSION@-@REVISION@/test is a directory where
you have write access; the examples can be run from that directory.
Change to the directory and type the command:

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


After the tests are completed, the results can be found in the test directory.

To clean up after the tests, type the command:

     ./clean.sh


D. Source version (for compilation)

A compressed tar file, phreeqc-@VERSION@-@REVISION@.tar.gz, is used to
distribute the source code and other files necessary to compile, test, install,
and run PHREEQC on Unix operating systems. (A compiled executable for Linux is
distributed in the Linux versions. See section C.)

D.1. Extracting files

Follow the steps below to extract the files from a distribution tar
file. 

  Steps in extracting files                  Explanation
  ----------------------------------------   -----------------------------------
  gunzip phreeqc-@VERSION@-@REVISION@.source.tar.gz           Uncompress the tar.gz file.

  tar -xvpof phreeqc-@VERSION@-@REVISION@.source.tar          Extract files from the tar file.

The directory phreeqc-@VERSION@-@REVISION@ is created when the files are
extracted; if this directory already exists, you may want to delete or rename
it before extracting the files.

The following directory structure is created (the contents of each directory
are shown to the right):

   phreeqc-@VERSION@-@REVISION@      Files related to configure
     `--config      More configure files
     `--database    Database files required during execution
     `--doc         Documentation files 
     `--examples    Examples from user's guide--used in verification tests
     `--src         Source code
     `--test        Files used to test the compilation

Notes:  (a) A compiled executable is not included in the source
            distribution.
        (b) It is recommended that no user files be kept in the PHREEQC
            directory structure.  

D.2. Compiling, testing, installing

No support beyond this README file is provided for users compiling their own
versions of the software. In general, to compile the software, you will need:

       (a) a C++ compiler, and
       (b) a minimal level of knowledge of configure, Make, the compiler, and
           the Linux operating system.

A Makefile can be generated by configure, and the Makefile can be used to
compile, test, and install the software.

To compile, test, and install PHREEQC, do the following:

1.  Change directory to the directory that was extracted from the tar file.

        cd phreeqc-@VERSION@-@REVISION@
        
2.  Make a directory, for example, Release.
	
	mkdir Release
	
3.  Change directory to Release.
	
	cd Release

4.  Run configure
	
	../configure
	
    Many of the options for configure can be seen by typing:
    
    	../configure --help

5.  Compile the program
	
	make
	
6.  Check that compiled version runs the test cases. Check may take several
	minutes.
	
	make check
	
    Results of running the test cases are stored in the directory
    	
    	Release/test

7.  Install the program. By default the program is installed in /usr/local/bin
    and /usr/local/share/doc/phreeqc. You can specify an installation prefix
    other than "/usr/local" by using the prefix option, prefix=$HOME for
    example.
    
    	make install
    	
    The locations of various files are given (a) for default installation (make
    install), and (b) if prefix is defined to be $HOME (make install prefix=
    $HOME). $HOME is used to represent your home directory in the following
    pathnames.
    
    Executable:
      	(a) /usr/local/bin/phreeqc
    	(b) $HOME/bin/phreeqc
    	
    Documentation (PDFs, README, NOTICE, phreeqc.txt):
    	
    	(a) /usr/local/share/doc/phreeqc
    	(b) $HOME/share/doc/phreeqc
    
    Databases: 
    
    	(a) /usr/local/share/doc/phreeqc/database
    	(b) $HOME/share/doc/phreeqc/database
    
    Example input files:
    
    	(a) /usr/local/share/doc/phreeqc/examples
    	(b) $HOME/share/doc/phreeqc/examples


D.3. Running

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
the executable may be copied to the current directory. In addition the default
database file, phreeqc.dat, may need to be in the current directory.
Phreeqc.dat is in phreeqc-@VERSION@-@REVISION@/database/phreeqc.dat.

The shell variable PHREEQC_DATABASE can be used to specify the default
database. In the C shell, this variable can be set (assuming your database
is named /home/jdoe/local/project/mydata.dat) with the command:

  setenv PHREEQC_DATABASE /home/jdoe/local/project/mydata.dat

In the Bourne or Korn shell, this variable can be set with the command:

  export PHREEQC_DATABASE=/home/jdoe/local/project/mydata.dat

The shell variable can be set permanently by including the appropriate command
in a file that is read when the shell is initiated, frequently $HOME/.login or
$HOME/.profile. If this shell variable is not set, the default database is
assumed to be phreeqc.dat in the current directory.   

 
E. Description of test cases

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


F. CONTACTS

Inquiries about this software distribution should be directed to:

e-mail:  h2osoft@usgs.gov or dlpark@usgs.gov

