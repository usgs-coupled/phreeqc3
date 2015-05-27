README.TXT

                                PHREEQC

       A program for speciation, batch-reaction, one-dimensional 
            transport, and inverse geochemical calculations

This file describes compilation of the batch version of PHREEQC version 3 for
Linux. No executable file is included. The distribution contains all files
necessary to configure and compile PHREEQC on any Unix operating system.

Instructions for configuring, compiling, installing, executing, and testing
on Linux operating systems are provided below. After installation, see the
root or doc directory of the PHREEQC installation for summary information on
PHREEQC in phreeqc.txt and the new features and bug fixes in RELEASE.TXT.

No graphical user interface exists for Linux. (For Windows, a graphical user
interface, PhreeqcI, is available at
http://wwwbrr.cr.usgs.gov/projects/GWC_coupled/phreeqc/index.html and a
Notepad++ interface is available at http://www.hydrochemistry.eu/downl.html.)


    TABLE OF CONTENTS

    A. Distribution files
    B. Documentation
    C. Extracting files (for Unix compilation)
    D. Compiling, testing, installing
    E. Running PHREEQC
    F. Description of test cases
    G. Contacts

-----------------------------------------------------------------------------------
A. Distribution file
-----------------------------------------------------------------------------------

    The following distribution package (containing the source code, configure
    files, examples, and documentation) is available for Linux systems:
                        
phreeqc-@VERSION@-@REVISION@.tar.gz      For compilation on Linux

-----------------------------------------------------------------------------------
B. Documentation
-----------------------------------------------------------------------------------

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
    
-----------------------------------------------------------------------------------   
C. Extracting files (for Unix compilation)
-----------------------------------------------------------------------------------

A compressed tar file, phreeqc-@VERSION@-@REVISION@.tar.gz, is used to
distribute the source code and other files necessary to compile, test, install,
and run PHREEQC on Unix operating systems. 

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

-----------------------------------------------------------------------------------
D. Compiling, testing, installing
-----------------------------------------------------------------------------------

No support beyond this README file is provided for users compiling their own
versions of the software. In general, to compile the software, you will need:

       (a) a C++ compiler, and
       (b) a minimal level of knowledge of configure, Make, the compiler, and
           the Linux operating system.

A Makefile can be generated by configure, and the Makefile can be used to
compile, test, and install the software.

To compile, test, and install PHREEQC, do the following:

D.1.  Change directory to the directory that was extracted from the tar file.

        cd phreeqc-@VERSION@-@REVISION@
        
D.2.  Make a directory, for example, Release.
    
    mkdir Release
    
D.3.  Change directory to Release.
    
    cd Release

D.4.  Run configure
    
    Many of the options for configure can be seen by typing:
    
        ../configure --help
        
    The most common option:
        --prefix=dir specifies the directory for installation of the
            library. Default is /usr/local, $HOME would install
            to your home directory.
            
    Run configure as follows:    
    
       ../configure [options] 

D.5.  Compile the program
    
    make [-j 4]

    Optionally, use -j n--where n is the number of compilations make runs in parallel.
    
D.6.  Check that compiled version runs the test cases. 
    Check may take several minutes.
    
    make check
    
    Results of running the test cases are stored in the directory
        
        Release/test

D.7. Install the program.  
    By default the program is installed in /usr/local/bin
    and /usr/local/share/doc/phreeqc. You can specify an installation prefix
    other than "/usr/local" by using the prefix option, prefix=$HOME for
    example.
    
        make install
        
    The locations of various files are given below, where "install" is the
    installation directory, /usr/local by default, or the value given 
    by the --prefix option to configure.
    
    Executable:
        install/bin/phreeqc
        
    Documentation:
        
        install/share/doc/phreeqc
            Phreeqc_3_2013_manual.pdf         Manual for Phreeqc version 3
            Phreeqc_2_1999_manual.pdf         Manual for Phreeqc version 2
            phreeqc.txt                       Short explanation for Phreeqc
            RELEASE                           Release notes
            README                            this readme file
            NOTICE                            User rights notice
            ???phreeqc3.chm
    
    Databases: 
    
        install/share/doc/phreeqc/database
            Amm.dat
            frezchem.dat
            iso.dat
            llnl.dat
            minteq.dat
            minteq.v4.dat
            phreeqc.dat
            pitzer.dat
            sit.dat
            wateq4f.dat
    
    Example input files:
    
        install/share/doc/phreeqc/examples
            Input files for 22 examples documented in the Phreeqc 3 manual

-----------------------------------------------------------------------------------
E. Running PHREEQC
-----------------------------------------------------------------------------------

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

For Linux, if the "make install" command described above has not been
executed, the executable may be copied to the current directory. In addition
the default database file, phreeqc.dat, or another desired database may need
to be in the current directory.

Databases are phreeqc-@VERSION@-@REVISION@/database and, if installed, 
install/share/doc/phreeqc/database.

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

----------------------------------------------------------------------------------- 
F. Description of test cases
-----------------------------------------------------------------------------------

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

-----------------------------------------------------------------------------------
G. Contacts
-----------------------------------------------------------------------------------

Inquiries about this software distribution should be directed to:

e-mail:  h2osoft@usgs.gov or dlpark@usgs.gov

