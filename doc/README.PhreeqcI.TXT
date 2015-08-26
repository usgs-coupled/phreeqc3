README.PhreeqcI.txt

       PhreeqcI Version @VERSION@  @VER_DATE@

       A graphical user interface to:
       
       PHREEQC--A program for speciation, batch-reaction, one-dimensional 
       transport, and inverse geochemical calculations

This file describes installing the GUI PhreeqcI on Windows operating systems.
The GUI can be downloaded from 
http://wwwbrr.cr.usgs.gov/projects/GWC_coupled/phreeqc/index.html. 
PhreeqcI has screens for most PHREEQC keywords and allows input files that use any 
of PHREEQC version 3 capabilities. Alternatively, a Notepad++ interface for 
PHREEQC version 3 is available at http://www.hydrochemistry.eu/downl.html.


	  TABLE OF CONTENTS

	  A. Distribution file
	  B. Documentation
	  C. Installing and running the GUI PhreeqcI
	  D. Testing and examples
	  E. Contacts


-----------------------------------------------------------------------------------
A. Distribution files
-----------------------------------------------------------------------------------

The following distribution file (containing the software, databases, examples and
documentation) are available at
http://wwwbrr.cr.usgs.gov/projects/GWC_coupled/phreeqc/index.html.

phreeqci-@VERSION@-@REVISION@.msi--GUI compiled by using Microsoft Visual C++ 2005

-----------------------------------------------------------------------------------                   
B. Documentation
-----------------------------------------------------------------------------------

*Brief description of the program PhreeqcI.

Charlton, S.R., and Parkhurst, D.L., 2002, PhreeqcI--A graphical user
   interface to the geochemical model PHREEQC: U.S. Geological Survey 
   Fact Sheet FS-031-02, 2 p.
   
   
*The following two user's guides are available in electronic format. Portable
   Document Format (PDF) files are included in the doc subdirectory of the
   PHREEQC program distribution. Latest documentationis available from
   the Help menu of PhreeqcI.
   
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
   

-----------------------------------------------------------------------------------   
C. Installing and running the GUI PhreeqcI
-----------------------------------------------------------------------------------

Note: the person installing the software generally must have administrator 
rights.

To install the GUI, double-click phreeqci-@VERSION@-@REVISION@.msi to execute
the installation program and follow the directions on the screen. The GUI is
completely independent from the batch version.

To execute the GUI, access the program through the start menus of Windows. 

By default, PhreeqcI is installed in the directory 
C:\Program Files (x86)\USGS\phreeqci-@VERSION@-@REVISION@.  
This is the recommended installation directory, but you may use other
directories or disk drives.

Note that program components not installed initially can be installed later,
and any components damaged (for example, by inadvertently overwriting the
directory) can be reinstalled by double-clicking
phreeqci-@VERSION@-@REVISION@.msi--a maintenance version of the installation
program will be executed.


-----------------------------------------------------------------------------------
D. Testing and examples
-----------------------------------------------------------------------------------

Input files are provided to verify that the program is correctly installed and
running on your system.  The input files execute the examples presented in the 
2013 user's guide, which demonstrate most of the program's capabilities. The 
PHREEQC "examples" directory contains the input files. 

To run the examples, copy the "examples" directory (C:\Program Files (x86)\USGS\
Phreeqc Interactive 3.1.4-8929\examples) to a location where you have
read/write access. The examples can be run from the new directory either by
double clicking one of the .pqi files from Windows Explorer or by the 
File->Open menu of PhreeqcI.

The examples are described in the table below, where 'Num' corresponds
to the example number in the PHREEQC manual. Charts can be compared to charts
in the manual (Techniques and Methods 6-A43).

Num   Description of example                
----  --------------------------------------------   
  1   Add uranium species and speciate seawater

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
E. CONTACTS
-----------------------------------------------------------------------------------

Inquiries about this software distribution should be directed to:

e-mail:  h2osoft@usgs.gov or dlpark@usgs.gov

