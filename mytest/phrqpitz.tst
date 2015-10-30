DATABASE ../database/pitzer.dat
SELECTED_OUTPUT 101
    -file                 phrqpitz.tst_101.sel
USER_PUNCH 101
    -headings Mu SC
    -start
10 PUNCH STR_F$(MU, 20, 12)
20 PUNCH STR_F$(SC, 20, 10)
    -end
knobs
	-conv	1e-10
	-step	10
#	-iter	400
#Test Problem 1a:  Canadian Shield Brine T-93; Frape et al., 1984, Alk+Mac Scale
#0000020001 0 0       0.0
#ELEMENTS
#C         15   61.0171
#
#SOLUTION 1
#Canadian Shield Brine T-93.
# 9 15 2   5.         4.0      18.       1.204
#   464000.        645000.        55100.         7199.         121080.
#  14207000.      221760.        16284.         1519.
#END
PITZER
	 -macinnes  true
SOLUTION 1 Canadian Shield Brine T-93.
	 -units	    mg/L
	 pH	    5
	 pe	    4
	 density    1.204
	 temp	    18
	 Ca	    64000.
	 Na	    45000.
	 Mg	    5100.
	 K	    199.
	 Sr	    1080.
	 Cl	    207000.
	 Br	    1760.
	 S	    284.
	 Alkalinity 19. as HCO3
END
#Test Problem 1b:  Canadian Shield Brine T-93; Frape et al., 1984, Alk + No scale
#0000020000 0 0       0.0
#ELEMENTS
#C         15   61.0171
#
#SOLUTION 1
#Canadian Shield Brine T-93.
# 9 15 2   5.         4.0      18.       1.204
#   464000.        645000.        55100.         7199.         121080.
#  14207000.      221760.        16284.         1519.
#END
PITZER
	 -macinnes  false
SOLUTION 1 Canadian Shield Brine T-93.
	 -units	    mg/L
	 pH	    5
	 pe	    4
	 density    1.204
	 temp	    18
	 Ca	    64000.
	 Na	    45000.
	 Mg	    5100.
	 K	    199.
	 Sr	    1080.
	 Cl	    207000.
	 Br	    1760.
	 S	    284.
	 Alkalinity 19. as HCO3
END
#Test Problem 1c:  Canadian Shield Brine T-93; Frape et al., 1984, calcite + Mac
#0060020001 0 1       0.0
#ELEMENTS
#C         15   61.0171
#
#SOLUTION 1
#Canadian Shield Brine T-93.
# 9 15 2   5.         4.0      18.       1.204
#   464000.        645000.        55100.         7199.         121080.
#  14207000.      221760.        16284.         1519.
#MINERALS
#CALCITE    2         4.00   -8.406                1
#  15 1.0          4 1.0
#-171.8329   -.077993    2839.319    71.595
#
#REACTION
#  151.0     4.0
#END
PITZER
	 -macinnes  true
SOLUTION 1 Canadian Shield Brine T-93.
	 -units	    mg/L
	 pH	    5
	 pe	    4
	 density    1.204
	 temp	    18
	 Ca	    64000.
	 Na	    45000.
	 Mg	    5100.
	 K	    199.
	 Sr	    1080.
	 Cl	    207000.
	 Br	    1760.
	 S	    284.
	 Alkalinity 19. as HCO3
EQUILIBRIUM_PHASES
	Calcite	    0	CO2(g)	10.
END
#Test Problem 1d: Canadian Shield Brine T-93, calcite equil. No scale
#0060020000 0 1       0.0
#ELEMENTS
#C         15   61.0171
#
#SOLUTION 1
#Canadian Shield Brine T-93.
# 9 15 2   5.         4.0      18.       1.204
#   464000.        645000.        55100.         7199.         121080.
#  14207000.      221760.        16284.         1519.
#MINERALS
#CALCITE    2         4.00   -8.406                1
#  15 1.0          4 1.0
#-171.8329   -.077993    2839.319    71.595
#
#REACTION
#  151.0     4.0
#END
PITZER
	 -macinnes  false
SOLUTION 1 Canadian Shield Brine T-93.
	 -units	    mg/L
	 pH	    5
	 pe	    4
	 density    1.204
	 temp	    18
	 Ca	    64000.
	 Na	    45000.
	 Mg	    5100.
	 K	    199.
	 Sr	    1080.
	 Cl	    207000.
	 Br	    1760.
	 S	    284.
	 Alkalinity 19. as HCO3
EQUILIBRIUM_PHASES
	Calcite	    0	CO2(g)	10.
END
#Test Problem 2: Sea Water Invariant Point (equilibration with reaction).
#0130020000 3 7       0.0
#SOLUTION 1
#Pure water
# 0  0 0    7.0       4.0       25.0      1.0
#MINERALS
#ANHYDRIT   2         6.00    -4.362               1
#   4      1.000  16      1.000
#422.950     0.0         -18431.     -147.708
#BISCHOFI   3         0.00     4.455               1
#   5      1.000  14      2.000   3      6.000
#3.524       0.0         277.6
#CARNALLI   4         0.00    4.330                0
#   7      1.000   5      1.000  14      3.000   3      6.000
#HALITE     2         0.00    1.570                1
#   6      1.000  14      1.000
#-713.4616   -.1201241   37302.21    262.4583    -2106915.
#KIESERIT   3         6.00   -0.123                0
#   5      1.000  16      1.000   3      1.000
#MAGNESIT   2         4.00   -7.834     -6.169     0
#   5      1.000  15      1.000
#PCO2       1         4.0    -1.468     -4.776     1         -3.5
#  35  1.0
#108.3865    0.01985076  -6919.53    -40.45154    669365.0
#
#STEPS
#0.        .1        1.0
#REACTION
#   4.186    0.0        5.976    0.0        68.567   0.         7.181    0.
#  149.958   0.        15.043    4.        16.514    6.
#END
SOLUTION 1	Pure water
EQUILIBRIUM_PHASES
	Anhydrite
	Bischofite	0	100.
	Carnallite
	Halite
	Kieserite
	Magnesite
	CO2(g)		-3.5
END
#Test Problem 3: Find the anhydrite-gypsum phase boundary with NaCl addition.
#0060020000 0 2       0.0
#SOLUTION 1
#pure water
# 0  0 0   7.         4.0      25.        1.0
#MINERALS
#ANHYDRIT   2         6.00    -4.362               1
#   4      1.000  16      1.000
#422.950     0.0         -18431.     -147.708
#GYPSUM     3         6.00   -4.581                1
#   4 1.0         16 1.0          3 2.0
#90.318      0.0         -4213.      -32.641
#
#REACTION
#   61.      0.        141.      0.
#END
SOLUTION 1	Pure water
EQUILIBRIUM_PHASES
	Anhydrite	0	NaCl	10.
	Gypsum		0
END
#Test Problem 4: Solubility with incremental temperature variation: halite-water
#0033020000 5 2       0.0
#SOLUTION 1
#Pure water
# 0  0 0    7.0       4.0       25.0      1.0
#MINERALS
#HALITE     2         0.00    1.570                1
#   6      1.000  14      1.000
#-713.4616   -.1201241   37302.21    262.4583    -2106915.
#
#TEMP
#0.        25.       100.      200.      300.
#STEPS
#0.        0.        0.        0.        0.
#REACTION
#   61.      0.        141.      0.
#END
SOLUTION 1	Pure water
EQUILIBRIUM_PHASES
	Halite		0	100.
REACTION_TEMPERATURE
	0	25	100	200	300
END
#Test Problem 5 (part a): Calcite-CO2-H2O equilibrium at log PCO2 = -2.
#0050022000 0 0       0.0
#SOLUTION 1
#Pure water
# 0  0 0    7.0       4.0       25.0      1.0
#MINERALS
#CALCITE    2         4.00   -8.406                1
#  15 1.0          4 1.0
#-171.8329   -.077993    2839.319    71.595
#PCO2       1         4.0    -1.468     -4.776     1         -2.
#  35  1.0
#108.3865    0.01985076  -6919.53    -40.45154    669365.0
#
#END
SOLUTION 1	Pure water
EQUILIBRIUM_PHASES
	Calcite  0
	CO2(g)	-2.0	
SAVE solution 2
END
#Test Problem 5 (part b): Calcite-Halite-CO2-H2O system at log PCO2 = -2.
#0050021000 0 0       0.0
#SOLUTION 1
#Pure water
# 0  0 0    7.0       4.0       25.0      1.0
#MINERALS
#CALCITE    2         4.00   -8.406                1
#  15 1.0          4 1.0
#-171.8329   -.077993    2839.319    71.595
#PCO2       1         4.0    -1.468     -4.776     1         -2.
#  35  1.0
#108.3865    0.01985076  -6919.53    -40.45154    669365.0
#HALITE     2         0.00    1.570                1
#   6      1.000  14      1.000
#-713.4616   -.1201241   37302.21    262.4583    -2106915.
#
#END
SOLUTION 1	Pure water
EQUILIBRIUM_PHASES
	Calcite  0
	CO2(g)	-2.0	
	Halite   0
SAVE solution 1
END
#Test Problem 5 (part c): Mix solns. 1 and 2 maintaining calcite saturation.
#0010020000 5 0       0.0
#MINERALS
#CALCITE    2         4.00   -8.406                1
#  15 1.0          4 1.0
#-171.8329   -.077993    2839.319    71.595
#
#STEPS
# .1       .3        .5        .7         .9
#END
MIX 1
    1	.1
    2	.9
EQUILIBRIUM_PHASES 1
	Calcite
END
MIX 1
    1	.3
    2	.7
EQUILIBRIUM_PHASES 1
	Calcite
END
MIX 1
    1	.5
    2	.5
EQUILIBRIUM_PHASES 1
	Calcite
END
MIX 1
    1	.7
    2	.3
EQUILIBRIUM_PHASES 1
	Calcite
END
MIX 1
    1	.9
    2	.1
EQUILIBRIUM_PHASES 1
	Calcite
END
#Test Problem 6 (part a): Equilibrate sea water with dolomite and air.
#0050021000 0 0       0.0
#ELEMENTS
#C         15   61.0171
#
#SOLUTION 1
#SEAWATER FROM NORDSTROM ET AL. (1979) TEST CASE IN PPM.
# 8 15 3   8.22      8.451     25.0      1.023
#   4 412.3        5 1291.8       6 10768.0      7 399.1       22 67.3
#  14 19353.0     15 141.682     16 2712.0
#MINERALS
#PCO2       1         4.0    -1.468     -4.776     1         -3.5
#  35  1.0
#108.3865    0.01985076  -6919.53    -40.45154    669365.0
#DOLOMITE   3         8.00  -17.083 -9.436         0
#   4 1.0          5 1.0         15 2.0
#
#END
SOLUTION 1
	 -units	ppm
	 pH		8.22
	 pe		8.451
	 temp		25
	 density	1.023
	 Ca		412.3
	 Mg		1291.8
	 Na		10768.0
	 K		399.1
	 Br		67.3
	 Cl		19353.0     
#	 Alkalinity	141.682     
	 C		2.2103 mmol/kgs
	 S		2712.0
EQUILIBRIUM_PHASES
	CO2(g)		-3.5
	Dolomite	0
SAVE solution 1
END
##6 (part b): Evaporate final solution of part a to gypsum saturation.
#0060021000 1 1       0.0
#MINERALS
#GYPSUM     3         6.00   -4.581                1
#   4 1.0         16 1.0          3 2.0
#90.318      0.0         -4213.      -32.641
#PCO2       1         4.0    -1.468     -4.776     1         -3.5
#  35  1.0
#108.3865    0.01985076  -6919.53    -40.45154    669365.0
#DOLOMITE   3         8.00  -17.083 -9.436         0
#   4 1.0          5 1.0         15 2.0
#
#REACTION
#   3 1.0
#END
USE solution 1
EQUILIBRIUM_PHASES
	Gypsum		0	H2O	100.
	CO2(g)		-3.5
	Dolomite	0
SAVE solution 1
END
##6 (part c): Evaporate final solution of part b to halite saturation.
#0060021000 1 1       0.0
#MINERALS
#HALITE     2         0.00    1.570                1
#   6      1.000  14      1.000
#-713.4616   -.1201241   37302.21    262.4583    -2106915.
#GYPSUM     3         6.00   -4.581                1
#   4 1.0         16 1.0          3 2.0
#90.318      0.0         -4213.      -32.641
#PCO2       1         4.0    -1.468     -4.776     1         -3.5
#  35  1.0
#108.3865    0.01985076  -6919.53    -40.45154    669365.0
#DOLOMITE   3         8.00  -17.083 -9.436         0
#   4 1.0          5 1.0         15 2.0
#
#REACTION
#   3 1.0
#END
USE solution 1
EQUILIBRIUM_PHASES
	Halite		0	H2O	100.
	Gypsum		0
	CO2(g)		-3.5
	Dolomite	0
SAVE solution 1
END
