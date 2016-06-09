ptf %
DATABASE @phreeqcdatabase@
SOLUTION_MASTER_SPECIES
    Tr            Tr               0     Tr              1
	Mn(+6)   MnO4-2		0	Mn

SOLUTION_SPECIES
Tr = Tr
	 log_k     0
	 # data from H4SiO4
	-dw	 1.10e-9
	-Vm  10.5  1.7  20  -2.7  0.1291 # supcrt + 2*H2O in a1
 2.0000 H2O + 1.0000 O2 + 1.0000 Mn++  =  MnO4-2 +4.0000 H+
        -gamma  5.0     0.0
        log_k           -32.4146
	-delta_H	151.703	kJ/mol	# Calculated enthalpy of reaction	MnO4-2
#	Enthalpy of formation:	-156 kcal/mol
        -analytic -1.0407e+001 -4.6464e-002 -1.0515e+004 1.0943e+001 -1.6408e+002
#       -Range:  0-300
 
 
2 NO3- + 12 H+ + 10 e- = N2 + 6 H2O
	#-log_k	207.08
	-log_k	0    ### To avoid getting a pe that is too low when PHREEQC calculates REACTION_T or REACTION_P
	-delta_h -312.130	kcal
	-dw	 1.96e-9
	-Vm 7 # Pray et al., 1952, IEC 44. 1146

Cl- = Cl-
	-gamma	3.5	  0.015
	-gamma	3.63  0.017 # cf. pitzer.dat
	# -dw	 2.03e-9 # original value
	-dw 1.18E-08 # modified to make diffusion of Cl slower than the other solutes due to negatively charged pore walls
	-Vm  4.465  4.801  4.325  -2.847  1.748  0  -0.331  20.16  0  1 # ref. 1	
	
END
SOLUTION_SPECIES
H+ = H+; -Dw 1e-9; -gamma       9.0     0
H2O = OH- + H+
        -analytic  293.29227  0.1360833  -10576.913  -123.73158  0  -6.996455e-5
        -gamma  3.5     0
        -dw      1e-9  470
        -Vm  -9.66  28.5  80.0 -22.9 1.89 0 1.09 0 0 1 # ref. 1
END

PHASES
##### Remineralization of l-alpha-Serine (as a solid phase): biological production of NH4+ ### MLever #####
Serine
C3H7NO3 + 6H2O = NH4+ + 5H2 + 2H+ + 3HCO3-
log_k -0.041852369	### data sent by MLever
 
### PHREEQC, added -Vm ###
 
Pyrochroite
	Mn(OH)2 + 2 H+ = Mn+2 + 2 H2O
	-log_k	15.2
	-Vm	27.29	# calculated from webmineral molecular weight & density data
 
### LLNL ###
 
Analcime ## Synonym of Analcite, Zeolite, Secondary Mineral
       # Na.96Al.96Si2.04O6:H2O +3.8400 H+  =  + 0.9600 Al+++ + 0.9600 Na+ + 2.0400 SiO2 + 2.9200 H2O
	Na.96Al.96Si2.04O6:H2O +3.8400 H+ + 4.08H2O  =  + 0.9600 Al+++ + 0.9600 Na+ + 2.0400 H4SiO4 + 2.9200 H2O
        log_k           6.1396
	-delta_H	-75.844	kJ/mol	# Calculated enthalpy of reaction	Analcime
#	Enthalpy of formation:	-3296.86 kJ/mol
        -analytic -6.8694e+000 6.6052e-003 9.8260e+003 -4.8540e+000 -8.8780e+005
#       -Range:  0-300
 
Analcime-dehy	## Synonym of Analcite, Zeolite, Secondary Mineral
	# Na.96Al.96Si2.04O6 +3.8400 H+  =  + 0.9600 Al+++ + 0.9600 Na+ + 1.9200 H2O + 2.0400 SiO2
        Na.96Al.96Si2.04O6 +3.8400 H+  + 4.0800H2O=  + 0.9600 Al+++ + 0.9600 Na+ + 1.9200 H2O + 2.0400 H4SiO4
        log_k           12.5023
	-delta_H	-116.641	kJ/mol	# Calculated enthalpy of reaction	Analcime-dehy
#	Enthalpy of formation:	-2970.23 kJ/mol
        -analytic -7.1134e+000 5.6181e-003 1.2185e+004 -5.0295e+000 -9.3890e+005
#       -Range:  0-300
 
 
Muscovite ## Primary Mineral
        #KAl3Si3O10(OH)2 +10.0000 H+  =  + 1.0000 K+ + 3.0000 Al+++ + 3.0000 SiO2 + 6.0000 H2O
        KAl3Si3O10(OH)2 +10.0000 H+  + 6H2O =  + 1.0000 K+ + 3.0000 Al+++ + 3.0000 H4SiO4 + 6.0000 H2O
        log_k           13.5858
	-delta_H	-243.224	kJ/mol	# Calculated enthalpy of reaction	Muscovite
#	Enthalpy of formation:	-1427.41 kcal/mol
        -analytic 3.3085e+001 -1.2425e-002 1.2477e+004 -2.0865e+001 -5.4692e+005
#       -Range:  0-300
	-Vm	140.89 # calculated from webmineral molecular weight & density data
 
 
Heulandite ## Zeolite, Secondary Mineral
	# Ba.065Sr.175Ca.585K.132Na.383Al2.165Si6.835O18:6H2O +8.6600 H+  =  + 0.0650 Ba++ + 0.1320 K+ + 0.1750 Sr++ + 0.3830 Na+ + 0.5850 Ca++ + 2.1650 Al+++ + 6.8350 SiO2 + 10.3300 H2O
         Ba.065Sr.175Ca.585K.132Na.383Al2.165Si6.835O18:6H2O +8.6600 H+ + 13.67 H2O =  + 0.0650 Ba++ + 0.1320 K+ + 0.1750 Sr++ + 0.3830 Na+ + 0.5850 Ca++ + 2.1650 Al+++ + 6.8350 H4SiO4 + 10.3300 H2O
        log_k           3.3506
	-delta_H	-97.2942	kJ/mol	# Calculated enthalpy of reaction	Heulandite
#	Enthalpy of formation:	-10594.5 kJ/mol
        -analytic -1.8364e+001 2.7879e-002 2.8426e+004 -1.7427e+001 -3.4723e+006
#       -Range:  0-300
 
 
 
Hedenbergite ## Clinopyroxene, Primary Mineral
        #CaFe(SiO3)2 +4.0000 H+  =  + 1.0000 Ca++ + 1.0000 Fe++ + 2.0000 H2O + 2.0000 SiO2
        CaFe(SiO3)2 +4.0000 H+  + 4H2O =  + 1.0000 Ca++ + 1.0000 Fe++ + 2.0000 H2O + 2.0000 H4SiO4
        log_k           19.6060
	-delta_H	-124.507	kJ/mol	# Calculated enthalpy of reaction	Hedenbergite
#	Enthalpy of formation:	-678.276 kcal/mol
        -analytic -1.9473e+001 1.5288e-003 1.2910e+004 2.1729e+000 -9.0058e+005
#       -Range:  0-300
	-Vm	69.88 # calculated from webmineral molecular weight & density data
 
 
Ca-Al_Pyroxene ## Primary Mineral
        #CaAl2SiO6 +8.0000 H+  =  + 1.0000 Ca++ + 1.0000 SiO2 + 2.0000 Al+++ + 4.0000 H2O
        CaAl2SiO6 +8.0000 H+  + 2H2O =  + 1.0000 Ca++ + 1.0000 H4SiO4 + 2.0000 Al+++ + 4.0000 H2O
        log_k           35.9759
	-delta_H	-361.548	kJ/mol	# Calculated enthalpy of reaction	Ca-Al_Pyroxene
#	Enthalpy of formation:	-783.793 kcal/mol
        -analytic -1.4664e+002 -5.0409e-002 2.1045e+004 5.1318e+001 3.2843e+002
#       -Range:  0-300
 
Diopside ## Clinopyroxene, Primary Mineral
        #CaMgSi2O6 +4.0000 H+  =  + 1.0000 Ca++ + 1.0000 Mg++ + 2.0000 H2O + 2.0000 SiO2
        CaMgSi2O6 +4.0000 H+ + 4H2O =  + 1.0000 Ca++ + 1.0000 Mg++ + 2.0000 H2O + 2.0000 H4SiO4
        log_k           20.9643
	-delta_H	-133.775	kJ/mol	# Calculated enthalpy of reaction	Diopside
#	Enthalpy of formation:	-765.378 kcal/mol
        -analytic 7.1240e+001 1.5514e-002 8.1437e+003 -3.0672e+001 -5.6880e+005
#       -Range:  0-300
	-Vm	63.69	# calculated from webmineral molecular weight & density data
 
Enstatite ## Orthopyroxene, Primary Mineral
        #MgSiO3 +2.0000 H+  =  + 1.0000 H2O + 1.0000 Mg++ + 1.0000 SiO2
        MgSiO3 +2.0000 H+  + 2H2O =  + 1.0000 H2O + 1.0000 Mg++ + 1.0000 H4SiO4
        log_k           11.3269
	-delta_H	-82.7302	kJ/mol	# Calculated enthalpy of reaction	Enstatite
#	Enthalpy of formation:	-369.686 kcal/mol
        -analytic -4.9278e+001 -3.2832e-003 9.5205e+003 1.4437e+001 -5.4324e+005
#       -Range:  0-300
 
 
Clinoptilolite	## Zeolite, Secondary Mineral
#         Na.954K.543Ca.761Mg.124Sr.036Ba.062Mn.002Al3.45Fe.017Si14.5330O46.922H21.844 +13.8680 H+  =  + 0.0020 Mn++ + 0.0170 Fe+++ + 0.0360 Sr++ + 0.0620 Ba++ + 0.1240 Mg++ + 0.5430 K+ + 0.7610 Ca++ + 0.9540 Na+ + 3.4500 Al+++ + 14.5330 SiO2 + 17.8560 H2O
         Na.954K.543Ca.761Mg.124Sr.036Ba.062Mn.002Al3.45Fe.017Si14.5330O46.922H21.844 +13.8680 H+ + 29.0660 H2O =  + 0.0020 Mn++ + 0.0170 Fe+++ + 0.0360 Sr++ + 0.0620 Ba++ + 0.1240 Mg++ + 0.5430 K+ + 0.7610 Ca++ + 0.9540 Na+ + 3.4500 Al+++ + 14.5330 H4SiO4 + 17.8560 H2O
        log_k           -9.7861
	-delta_H	-20.8784	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite
#	Enthalpy of formation:	-20587.8 kJ/mol
        -analytic -1.3213e+000 6.4960e-002 5.0630e+004 -4.6120e+001 -7.4699e+006
#       -Range:  0-300
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite
 
Clinop-Ca	## Zeolite, Secondary Mineral
    #    Ca1.7335Al3.45Fe.017Si14.533O36:10.922H2O +13.8680 H+  =  + 0.0170 Fe+++ + 1.7335 Ca++ + 3.4500 Al+++ + 14.5330 SiO2 + 17.8560 H2O
        Ca1.7335Al3.45Fe.017Si14.533O36:10.922H2O +13.8680 H+ + 29.0660 H2O =  + 0.0170 Fe+++ + 1.7335 Ca++ + 3.4500 Al+++ + 14.5330 H4SiO4 + 17.8560 H2O
	  log_k           -7.0095
	-delta_H	-74.6745	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite-Ca
#	Enthalpy of formation:	-4919.84 kcal/mol
        -analytic -4.4820e+001 5.3696e-002 5.4878e+004 -3.1459e+001 -7.5491e+006
#       -Range:  0-300
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite
 
 
Clinop-K	## Zeolite, Secondary Mineral
   #     K3.467Al3.45Fe.017Si14.533O36:10.922H2O +13.8680 H+  =  + 0.0170 Fe+++ + 3.4500 Al+++ + 3.4670 K+ + 14.5330 SiO2 + 17.8560 H2O
        K3.467Al3.45Fe.017Si14.533O36:10.922H2O +13.8680 H+  + 29.0660 H2O =  + 0.0170 Fe+++ + 3.4500 Al+++ + 3.4670 K+ + 14.5330 H4SiO4 + 17.8560 H2O
        log_k           -10.9485
	-delta_H	67.4862	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite-K
#	Enthalpy of formation:	-4937.77 kcal/mol
        -analytic 1.1697e+001 6.9480e-002 4.7718e+004 -4.7442e+001 -7.6907e+006
#       -Range:  0-300
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite
 
Clinop-NH4	## Zeolite, Secondary Mineral
   #     (NH4)3.467Al3.45Fe.017Si14.533O36:10.922H2O +10.4010 H+  =  + 0.0170 Fe+++ + 3.4500 Al+++ + 3.4670 NH3 + 14.5330 SiO2 + 17.8560 H2O
        (NH4)3.467Al3.45Fe.017Si14.533O36:10.922H2O +10.4010 H+  + 29.0660 H2O =  + 0.0170 Fe+++ + 3.4500 Al+++ + 3.4670 NH3 + 14.5330 H4SiO4 + 17.8560 H2O
        log_k           -42.4791
	-delta_H	0	      	# Not possible to calculate enthalpy of reaction	Clinoptilolite-NH4
#	Enthalpy of formation:	0 kcal/mol
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite
 
 
Clinop-Na	## Zeolite, Secondary Mineral
   #     Na3.467Al3.45Fe.017Si14.533O36:10.922H2O +13.8680 H+  =  + 0.0170 Fe+++ + 3.4500 Al+++ + 3.4670 Na+ + 14.5330 SiO2 + 17.8560 H2O
        Na3.467Al3.45Fe.017Si14.533O36:10.922H2O +13.8680 H+  + 29.0660 H2O =  + 0.0170 Fe+++ + 3.4500 Al+++ + 3.4670 Na+ + 14.5330 H4SiO4 + 17.8560 H2O
        log_k           -7.1363
	-delta_H	2.32824	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite-Na
#	Enthalpy of formation:	-4912.36 kcal/mol
        -analytic -3.4572e+001 6.8377e-002 5.1962e+004 -3.3426e+001 -7.5586e+006
#       -Range:  0-300
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite
 
Clinop-Sr	## Zeolite, Secondary Mineral
 #       Sr1.7335Al3.45Fe.017Si14.533O36:10.922H2O +13.8680 H+  =  + 0.0170 Fe+++ + 1.7335 Sr++ + 3.4500 Al+++ + 14.5330 SiO2 + 17.8560 H2O
        Sr1.7335Al3.45Fe.017Si14.533O36:10.922H2O +13.8680 H+  + 29.0660 H2O =  + 0.0170 Fe+++ + 1.7335 Sr++ + 3.4500 Al+++ + 14.5330 H4SiO4 + 17.8560 H2O
       log_k           -7.1491
	-delta_H	-66.2129	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite-Sr
#	Enthalpy of formation:	-4925.1 kcal/mol
        -analytic 3.2274e+001 6.7050e-002 5.0880e+004 -5.9597e+001 -7.3876e+006
#       -Range:  0-300
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite
 
 
Clinop-dehy	## Zeolite, Secondary Mineral
   #    Sr.036Mg.124Ca.761Mn.002Ba.062K.543Na.954Al3.45Fe.017Si14.533O36 +13.8680 H+ =  + 0.0020 Mn++ + 0.0170 Fe+++ + 0.0360 Sr++ + 0.0620 Ba++ + 0.1240 Mg++ + 0.5430 K+ + 0.7610 Ca++ + 0.9540 Na+ + 3.4500 Al+++ + 6.9340 H2O + 14.5330 SiO2
        Sr.036Mg.124Ca.761Mn.002Ba.062K.543Na.954Al3.45Fe.017Si14.533O36 +13.8680 H+ + 29.0660 H2O =  + 0.0020 Mn++ + 0.0170 Fe+++ + 0.0360 Sr++ + 0.0620 Ba++ + 0.1240 Mg++ + 0.5430 K+ + 0.7610 Ca++ + 0.9540 Na+ + 3.4500 Al+++ + 6.9340 H2O + 14.5330 H4SiO4
        log_k           25.8490
	-delta_H	-276.592	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite-dehy
#	Enthalpy of formation:	-17210.2 kJ/mol
        -analytic -2.0505e+002 6.0155e-002 8.2682e+004 1.5333e+001 -9.1369e+006
#       -Range:  0-300
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite
 
Clinop-dehy-Ca	## Zeolite, Secondary Mineral
  #      Ca1.7335Al3.45Fe.017Si14.533O36 +13.8680 H+  =  + 0.0170 Fe+++ + 1.7335 Ca++ + 3.4500 Al+++ + 6.9340 H2O + 14.5330 SiO2
         Ca1.7335Al3.45Fe.017Si14.533O36 +13.8680 H+ + 29.0660 H2O =  + 0.0170 Fe+++ + 1.7335 Ca++ + 3.4500 Al+++ + 6.9340 H2O + 14.5330 H4SiO4
	    log_k           28.6255
	-delta_H	-329.278	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite-dehy-Ca
#	Enthalpy of formation:	-4112.83 kcal/mol
        -analytic -1.2948e+002 6.5698e-002 8.0229e+004 -1.2812e+001 -8.8320e+006
#       -Range:  0-300
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite
 
Clinop-dehy-K	## Zeolite, Secondary Mineral
#        K3.467Al3.45Fe.017Si14.533O36 +13.8680 H+  =  + 0.0170 Fe+++ + 3.4500 Al+++ + 3.4670 K+ + 6.9340 H2O + 14.5330 SiO2
         K3.467Al3.45Fe.017Si14.533O36 +13.8680 H+ + 29.0660 H2O =  + 0.0170 Fe+++ + 3.4500 Al+++ + 3.4670 K+ + 6.9340 H2O + 14.5330 H4SiO4
        log_k           24.6865
	-delta_H	-191.289	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite-dehy-K
#	Enthalpy of formation:	-4129.76 kcal/mol
        -analytic -1.2241e+002 7.4761e-002 7.6067e+004 -1.1315e+001 -9.1389e+006
#       -Range:  0-300
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite
 
Clinop-dehy-NH4	## Zeolite, Secondary Mineral
#        (NH4)3.467Al3.45Fe.017Si14.533O36 +10.4010 H+  =  + 0.0170 Fe+++ + 3.4500 Al+++ + 3.4670 NH3 + 6.9340 H2O + 14.5330 SiO2
         (NH4)3.467Al3.45Fe.017Si14.533O36 +10.4010 H+ + 29.0660 H2O =  + 0.0170 Fe+++ + 3.4500 Al+++ + 3.4670 NH3 + 6.9340 H2O + 14.5330 H4SiO4
        log_k           -6.8441
	-delta_H	0	      	# Not possible to calculate enthalpy of reaction	Clinoptilolite-dehy-NH4
#	Enthalpy of formation:	0 kcal/mol
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite
 
Clinop-dehy-Na	## Zeolite, Secondary Mineral
#        Na3.467Al3.45Fe.017Si14.533O36 +13.8680 H+  =  + 0.0170 Fe+++ + 3.4500 Al+++ + 3.4670 Na+ + 6.9340 H2O + 14.5330 SiO2
         Na3.467Al3.45Fe.017Si14.533O36 +13.8680 H+ + 29.0660 H2O =  + 0.0170 Fe+++ + 3.4500 Al+++ + 3.4670 Na+ + 6.9340 H2O + 14.5330 H4SiO4
        log_k           28.4987
	-delta_H	-253.798	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite-dehy-Na
#	Enthalpy of formation:	-4104.98 kcal/mol
        -analytic -1.4386e+002 7.6846e-002 7.8723e+004 -5.9741e+000 -8.9159e+006
#       -Range:  0-300
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite
 
Clinop-dehy-Sr ## Zeolite, Secondary Mineral
#        Sr1.7335Al3.45Fe.017Si14.533O36 +13.8680 H+  =  + 0.0170 Fe+++ + 1.7335 Sr++ + 3.4500 Al+++ + 6.9340 H2O + 14.5330 SiO2
         Sr1.7335Al3.45Fe.017Si14.533O36 +13.8680 H+ + 29.0660 H2O =  + 0.0170 Fe+++ + 1.7335 Sr++ + 3.4500 Al+++ + 6.9340 H2O + 14.5330 H4SiO4
        log_k           28.4859
	-delta_H	-321.553	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite-dehy-Sr
#	Enthalpy of formation:	-4117.92 kcal/mol
        -analytic -1.8410e+002 6.0457e-002 8.3626e+004 6.4304e+000 -9.0962e+006
#       -Range:  0-300
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite
 
Clinop-hy-Ca	## Zeolite, Secondary Mineral
#        Ca1.7335Al3.45Fe.017Si14.533O36:11.645H2O +13.8680 H+  =  + 0.0170 Fe+++ + 1.7335 Ca++ + 3.4500 Al+++ + 14.5330 SiO2 + 18.5790 H2O
        Ca1.7335Al3.45Fe.017Si14.533O36:11.645H2O +13.8680 H+ + 29.0660 H2O =  + 0.0170 Fe+++ + 1.7335 Ca++ + 3.4500 Al+++ + 14.5330 H4SiO4 + 18.5790 H2O
        log_k           -7.0108
	-delta_H	-65.4496	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite-hy-Ca
#	Enthalpy of formation:	-4971.44 kcal/mol
        -analytic 8.6833e+001 7.1520e-002 4.6854e+004 -7.8023e+001 -7.0900e+006
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite
 
Clinop-hy-K	## Zeolite, Secondary Mineral
#        K3.467Al3.45Fe.017Si14.533O36:7.499H2O +13.8680 H+  =  + 0.0170 Fe+++ + 3.4500 Al+++ + 3.4670 K+ + 14.4330 H2O + 14.5330 SiO2
         K3.467Al3.45Fe.017Si14.533O36:7.499H2O +13.8680 H+ + 29.0660 H2O =  + 0.0170 Fe+++ + 3.4500 Al+++ + 3.4670 K+ + 14.4330 H2O + 14.5330 H4SiO4
        log_k           -10.9523
	-delta_H	29.5879	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite-hy-K
#	Enthalpy of formation:	-4694.86 kcal/mol
        -analytic 1.6223e+001 7.3919e-002 5.0447e+004 -5.2790e+001 -7.8484e+006
#       -Range:  0-300
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite
 
Clinop-hy-Na	## Zeolite, Secondary Mineral
#        Na3.467Al3.45Fe.017Si14.533O36:10.877H2O +13.8680 H+  =  + 0.0170 Fe+++ + 3.4500 Al+++ + 3.4670 Na+ + 14.5330 SiO2 + 17.8110 H2O
         Na3.467Al3.45Fe.017Si14.533O36:10.877H2O +13.8680 H+ + 29.0660 H2O =  + 0.0170 Fe+++ + 3.4500 Al+++ + 3.4670 Na+ + 14.5330 H4SiO4 + 17.8110 H2O
        log_k           -7.1384
	-delta_H	1.88166	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite-hy-Na
#	Enthalpy of formation:	-4909.18 kcal/mol
        -analytic -8.4189e+000 7.2018e-002 5.0501e+004 -4.2851e+001 -7.4714e+006
#       -Range:  0-300
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite
 
Clinop-hy-Sr	## Zeolite, Secondary Mineral
#        Sr1.7335Al3.45Fe.017Si14.533O36:13.893H2O +13.8680 H+  =  + 0.0170 Fe+++ + 1.7335 Sr++ + 3.4500 Al+++ + 14.5330 SiO2 + 20.8270 H2O
         Sr1.7335Al3.45Fe.017Si14.533O36:13.893H2O +13.8680 H+ + 29.0660 H2O =  + 0.0170 Fe+++ + 1.7335 Sr++ + 3.4500 Al+++ + 14.5330 H4SiO4 + 20.8270 H2O
        log_k           -7.1498
	-delta_H	-31.6858	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite-hy-Sr
#	Enthalpy of formation:	-5136.33 kcal/mol
        -analytic 1.0742e-001 5.9065e-002 4.9985e+004 -4.4648e+001 -7.3382e+006
#       -Range:  0-300
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite
 
Laumontite	## Zeolite, Secondary Mineral
#        CaAl2Si4O12:4H2O +8.0000 H+  =  + 1.0000 Ca++ + 2.0000 Al+++ + 4.0000 SiO2 + 8.0000 H2O
         CaAl2Si4O12:4H2O +8.0000 H+ + 8.0000 H2O =  + 1.0000 Ca++ + 2.0000 Al+++ + 4.0000 H4SiO4 + 8.0000 H2O
        log_k           13.6667
	-delta_H	-184.657	kJ/mol	# Calculated enthalpy of reaction	Laumontite
#	Enthalpy of formation:	-1728.66 kcal/mol
        -analytic 1.1904e+000 8.1763e-003 1.9005e+004 -1.4561e+001 -1.5851e+006
#       -Range:  0-300
 
Stilbite	## Zeolite, Secondary Mineral
#        Ca1.019Na.136K.006Al2.18Si6.82O18:7.33H2O +8.7200 H+  =  + 0.0060 K+ + 0.1360 Na+ + 1.0190 Ca++ + 2.1800 Al+++ + 6.8200 SiO2 + 11.6900 H2O
         Ca1.019Na.136K.006Al2.18Si6.82O18:7.33H2O +8.7200 H+ + 13.6400 H2O =  + 0.0060 K+ + 0.1360 Na+ + 1.0190 Ca++ + 2.1800 Al+++ + 6.8200 H4SiO4 + 11.6900 H2O
        log_k           1.0545
	-delta_H	-83.0019	kJ/mol	# Calculated enthalpy of reaction	Stilbite
#	Enthalpy of formation:	-11005.7 kJ/mol
        -analytic -2.4483e+001 3.0987e-002 2.8013e+004 -1.5802e+001 -3.4491e+006
#       -Range:  0-300
 
Gismondine ## Zeolite, Secondary Mineral (Merlinoite identified in XRD is related to gismondine)
#        Ca2Al4Si4O16:9H2O +16.0000 H+  =  + 2.0000 Ca++ + 4.0000 Al+++ + 4.0000 SiO2 + 17.0000 H2O
        Ca2Al4Si4O16:9H2O +16.0000 H+  + 8.0000 H2O =  + 2.0000 Ca++ + 4.0000 Al+++ + 4.0000 H4SiO4 + 17.0000 H2O
        log_k           41.7170
	-delta_H	0	      	# Not possible to calculate enthalpy of reaction	Gismondine
#	Enthalpy of formation:	0 kcal/mol
 
Clinochlore-14A ## Chlorite, Secondary Mineral
#        Mg5Al2Si3O10(OH)8 +16.0000 H+  =  + 2.0000 Al+++ + 3.0000 SiO2 + 5.0000 Mg++ + 12.0000 H2O
        Mg5Al2Si3O10(OH)8 +16.0000 H+  + 6.0000 H2O =  + 2.0000 Al+++ + 3.0000 H4SiO4 + 5.0000 Mg++ + 12.0000 H2O
        log_k           67.2391
	-delta_H	-612.379	kJ/mol	# Calculated enthalpy of reaction	Clinochlore-14A
#	Enthalpy of formation:	-2116.96 kcal/mol
        -analytic -2.0441e+002 -6.2268e-002 3.5388e+004 6.9239e+001 5.5225e+002
#       -Range:  0-300
 
Clinochlore-7A ## Chlorite, Secondary Mineral
#        Mg5Al2Si3O10(OH)8 +16.0000 H+  =  + 2.0000 Al+++ + 3.0000 SiO2 + 5.0000 Mg++ + 12.0000 H2O
        Mg5Al2Si3O10(OH)8 +16.0000 H+ + 6.0000 H2O =  + 2.0000 Al+++ + 3.0000 H4SiO4 + 5.0000 Mg++ + 12.0000 H2O
        log_k           70.6124
	-delta_H	-628.14	kJ/mol	# Calculated enthalpy of reaction	Clinochlore-7A
#	Enthalpy of formation:	-2113.2 kcal/mol
        -analytic -2.1644e+002 -6.4187e-002 3.6548e+004 7.4123e+001 5.7037e+002
#       -Range:  0-300
 
 
Maximum_Microcline ## Feldspar, Primary
#        KAlSi3O8 +4.0000 H+  =  + 1.0000 Al+++ + 1.0000 K+ + 2.0000 H2O + 3.0000 SiO2
        KAlSi3O8 +4.0000 H+  + 6.0000 H2O =  + 1.0000 Al+++ + 1.0000 K+ + 2.0000 H2O + 3.0000 H4SiO4
        log_k           -0.2753
	-delta_H	-23.9408	kJ/mol	# Calculated enthalpy of reaction	Maximum_Microcline
#	Enthalpy of formation:	-949.188 kcal/mol
        -analytic -9.4387e+000 1.3561e-002 1.2656e+004 -7.4925e+000 -1.6795e+006
#       -Range:  0-300
	-Vm 108.15 # from phreeqc.dat (K-feldspar --> same chemical formula)
 
 
Sanidine_high	## Primary Mineral, feldspar
#        KAlSi3O8 +4.0000 H+  =  + 1.0000 Al+++ + 1.0000 K+ + 2.0000 H2O + 3.0000 SiO2
        KAlSi3O8 +4.0000 H+  + 6.0000 H2O =  + 1.0000 Al+++ + 1.0000 K+ + 2.0000 H2O + 3.0000 H4SiO4
        log_k           0.9239
	-delta_H	-35.0284	kJ/mol	# Calculated enthalpy of reaction	Sanidine_high
#	Enthalpy of formation:	-946.538 kcal/mol
        -analytic -3.4889e+000 1.4495e-002 1.2856e+004 -9.8978e+000 -1.6572e+006
#       -Range:  0-300
 
Albite
#        NaAlSi3O8 +4.0000 H+  =  + 1.0000 Al+++ + 1.0000 Na+ + 2.0000 H2O + 3.0000 SiO2
        NaAlSi3O8 +4.0000 H+  + 6.0000 H2O =  + 1.0000 Al+++ + 1.0000 Na+ + 2.0000 H2O + 3.0000 H4SiO4
        log_k           2.7645
	-delta_H	-51.8523	kJ/mol	# Calculated enthalpy of reaction	Albite
#	Enthalpy of formation:	-939.68 kcal/mol
        -analytic -1.1694e+001 1.4429e-002 1.3784e+004 -7.2866e+000 -1.6136e+006
#       -Range:  0-300
	-Vm 101.31	# from phreeqc.dat
 
 
Albite_low	## Primary Mineral
#        NaAlSi3O8 +4.0000 H+  =  + 1.0000 Al+++ + 1.0000 Na+ + 2.0000 H2O + 3.0000 SiO2
        NaAlSi3O8 +4.0000 H+  + 4.0000 H2O =  + 1.0000 Al+++ + 1.0000 Na+ + 3.0000 H4SiO4
        log_k           2.7645
	-delta_H	-51.8523	kJ/mol	# Calculated enthalpy of reaction	Albite_low
#	Enthalpy of formation:	-939.68 kcal/mol
        -analytic -1.2860e+001 1.4481e-002 1.3913e+004 -6.9417e+000 -1.6256e+006
#       -Range:  0-300
	-Vm 101.31	# from phreeqc.dat
 
Anorthite
#        CaAl2(SiO4)2 +8.0000 H+  =  + 1.0000 Ca++ + 2.0000 Al+++ + 2.0000 SiO2 + 4.0000 H2O
        CaAl2(SiO4)2 +8.0000 H+  =  + 1.0000 Ca++ + 2.0000 Al+++ + 2.0000 H4SiO4
        log_k           26.5780
	-delta_H	-303.039	kJ/mol	# Calculated enthalpy of reaction	Anorthite
#	Enthalpy of formation:	-1007.55 kcal/mol
        -analytic 3.9717e-001 -1.8751e-002 1.4897e+004 -6.3078e+000 -2.3885e+005
#       -Range:  0-300
	-Vm 105.05	# from phreeqc.dat
 
Chalcopyrite	## Primary or Secondary Mineral
        CuFeS2 +2.0000 H+  =  + 1.0000 Cu++ + 1.0000 Fe++ + 2.0000 HS-
        log_k           -32.5638
	-delta_H	127.206	kJ/mol	# Calculated enthalpy of reaction	Chalcopyrite
#	Enthalpy of formation:	-44.453 kcal/mol
        -analytic -3.1575e+002 -9.8947e-002 8.3400e+002 1.2522e+002 1.3106e+001
#       -Range:  0-300
 
Brucite	## Secondary Mineral
        Mg(OH)2 +2.0000 H+  =  + 1.0000 Mg++ + 2.0000 H2O
        log_k           16.2980
	-delta_H	-111.34	kJ/mol	# Calculated enthalpy of reaction	Brucite
#	Enthalpy of formation:	-221.39 kcal/mol
        -analytic -1.0280e+002 -1.9759e-002 9.0180e+003 3.8282e+001 1.4075e+002
#       -Range:  0-300
 
Fayalite	## Primary Mineral, olivine
#        Fe2SiO4 +4.0000 H+  =  + 1.0000 SiO2 + 2.0000 Fe++ + 2.0000 H2O
        Fe2SiO4 +4.0000 H+ + 2.0000 H2O =  + 1.0000 H4SiO4 + 2.0000 Fe++ + 2.0000 H2O
        log_k           19.1113
	-delta_H	-152.256	kJ/mol	# Calculated enthalpy of reaction	Fayalite
#	Enthalpy of formation:	-354.119 kcal/mol
        -analytic 1.3853e+001 -3.5501e-003 7.1496e+003 -6.8710e+000 -6.3310e+004
#       -Range:  0-300
	-Vm	46.42 # calculated from webmineral molecular weight & density data
 
Forsterite	## Primary Mineral, olivine
  #      Mg2SiO4 +4.0000 H+  =  + 1.0000 SiO2 + 2.0000 H2O + 2.0000 Mg++
        Mg2SiO4 +4.0000 H+  + 2.0000 H2O =  + 1.0000 H4SiO4 + 2.0000 H2O + 2.0000 Mg++
        log_k           27.8626
	-delta_H	-205.614	kJ/mol	# Calculated enthalpy of reaction	Forsterite
#	Enthalpy of formation:	-520 kcal/mol
        -analytic -7.6195e+001 -1.4013e-002 1.4763e+004 2.5090e+001 -3.0379e+005
#       -Range:  0-300
 
 
Cristobalite(alpha)	## Primary or Secondary Mineral (webmineral: Post-volcanic, magmatic, and secondary)
#        SiO2  =  + 1.0000 SiO2
        SiO2  + 2.0000 H2O =  + 1.0000 H4SiO4
        log_k           -3.4488
	-delta_H	29.2043	kJ/mol	# Calculated enthalpy of reaction	Cristobalite(alpha)
#	Enthalpy of formation:	-216.755 kcal/mol
        -analytic -1.1936e+001 9.0520e-003 4.3701e+003 -1.1464e-001 -7.6568e+005
#       -Range:  0-300
 
Cristobalite(beta)	## Primary or Secondary Mineral (webmineral: Post-volcanic, magmatic, and secondary)
#        SiO2  =  + 1.0000 SiO2
        SiO2  + 2.0000 H2O =  + 1.0000 H4SiO4
        log_k           -3.0053
	-delta_H	24.6856	kJ/mol	# Calculated enthalpy of reaction	Cristobalite(beta)
#	Enthalpy of formation:	-215.675 kcal/mol
        -analytic -4.7414e+000 9.7567e-003 3.8831e+003 -2.5830e+000 -6.9636e+005
#       -Range:  0-300
 
Magnetite	## Primary or Secondary Mineral (webmineral: accessory mineral in igneous and metamorphic rocks)
        Fe3O4 +8.0000 H+  =  + 1.0000 Fe++ + 2.0000 Fe+++ + 4.0000 H2O
        log_k           10.4724
	-delta_H	-216.597	kJ/mol	# Calculated enthalpy of reaction	Magnetite
#	Enthalpy of formation:	-267.25 kcal/mol
        -analytic -3.0510e+002 -7.9919e-002 1.8709e+004 1.1178e+002 2.9203e+002
#       -Range:  0-300
 
Spinel	## possible Secondary mineral (webmineral: metamorphic mineral found in limestone and dolomite marbles and very-low silica and alkali igneous rocks, e.g. peridotites)
        Al2MgO4 +8.0000 H+  =  + 1.0000 Mg++ + 2.0000 Al+++ + 4.0000 H2O
        log_k           37.6295
	-delta_H	-398.108	kJ/mol	# Calculated enthalpy of reaction	Spinel
#	Enthalpy of formation:	-546.847 kcal/mol
        -analytic -3.3895e+002 -8.3595e-002 2.9251e+004 1.2260e+002 4.5654e+002
#       -Range:  0-300
 
Hausmannite	## Secondary or Primary Mineral (webmineral: primary mineral in hydrothermal veins. Also produced by metamorphism of manganiferous rocks)
        Mn3O4 +8.0000 H+  =  + 1.0000 Mn++ + 2.0000 Mn+++ + 4.0000 H2O
        log_k           10.1598
	-delta_H	-268.121	kJ/mol	# Calculated enthalpy of reaction	Hausmannite
#	Enthalpy of formation:	-1387.83 kJ/mol
        -analytic -2.0600e+002 -2.2214e-002 2.0160e+004 6.2700e+001 3.1464e+002
#       -Range:  0-300
 
Celadonite	## Secondary Mineral, Phyllosilicate Mica Group (webmineral: vesicle lining and coatings in altered volcanic of intermediate to basic compositions under low grade metamorphism)
#        KMgAlSi4O10(OH)2 +6.0000 H+  =  + 1.0000 Al+++ + 1.0000 K+ + 1.0000 Mg++ + 4.0000 H2O + 4.0000 SiO2
        KMgAlSi4O10(OH)2 +6.0000 H+  + 8.0000 H2O =  + 1.0000 Al+++ + 1.0000 K+ + 1.0000 Mg++ + 4.0000 H2O + 4.0000 H4SiO4
        log_k           7.4575
	-delta_H	-74.3957	kJ/mol	# Calculated enthalpy of reaction	Celadonite
#	Enthalpy of formation:	-1394.9 kcal/mol
        -analytic -3.3097e+001 1.7989e-002 1.8919e+004 -2.1219e+000 -2.0588e+006
#       -Range:  0-300
 
Jadeite	## Primary Mineral, Pyroxene
#        NaAl(SiO3)2 +4.0000 H+  =  + 1.0000 Al+++ + 1.0000 Na+ + 2.0000 H2O + 2.0000 SiO2
        NaAl(SiO3)2 +4.0000 H+ + 4H2O =  + 1.0000 Al+++ + 1.0000 Na+ + 2.0000 H2O + 2.0000 H4SiO4
        log_k           8.3888
	-delta_H	-84.4415	kJ/mol	# Calculated enthalpy of reaction	Jadeite
#	Enthalpy of formation:	-722.116 kcal/mol
        -analytic 1.5934e+000 5.0757e-003 9.5602e+003 -7.0164e+000 -8.4454e+005
#       -Range:  0-300
 
Goethite
        FeOOH +3.0000 H+  =  + 1.0000 Fe+++ + 2.0000 H2O
        log_k           0.5345
	-delta_H	-61.9291	kJ/mol	# Calculated enthalpy of reaction	Goethite
#	Enthalpy of formation:	-559.328 kJ/mol
        -analytic -6.0331e+001 -1.0847e-002 4.7759e+003 1.9429e+001 8.1122e+001
#       -Range:  0-200
	-Vm 20.84 # phreeqc.dat
 
Pyrolusite
	MnO2  =  + 0.5000 Mn++ + 0.5000 MnO4--
        log_k           -17.6439
	-delta_H	83.3804	kJ/mol	# Calculated enthalpy of reaction	Pyrolusite
#	Enthalpy of formation:	-520.031 kJ/mol
        -analytic -1.1541e+002 -4.1665e-002 -1.8960e+003 4.7094e+001 -2.9551e+001
#       -Range:  0-300
 
Illite
#        K0.6Mg0.25Al1.8Al0.5Si3.5O10(OH)2 +8.0000 H+  =  + 0.2500 Mg++ + 0.6000 K+ + 2.3000 Al+++ + 3.5000 SiO2 + 5.0000 H2O
        K0.6Mg0.25Al1.8Al0.5Si3.5O10(OH)2 +8.0000 H+  + 2 H2O =  + 0.2500 Mg++ + 0.6000 K+ + 2.3000 Al+++ + 3.5000 H4SiO4
       log_k           9.0260
	-delta_H	-171.764	kJ/mol	# Calculated enthalpy of reaction	Illite
#	Enthalpy of formation:	-1394.71 kcal/mol
        -analytic 2.6069e+001 -1.2553e-003 1.3670e+004 -2.0232e+001 -1.1204e+006
#       -Range:  0-300
	-Vm 141.48 # phreeqc.dat
 
Kaolinite
#        Al2Si2O5(OH)4 +6.0000 H+  =  + 2.0000 Al+++ + 2.0000 SiO2 + 5.0000 H2O
        Al2Si2O5(OH)4 +6.0000 H+  =  + 2.0000 Al+++ + 2.0000 H4SiO4 + 1.0000 H2O
        log_k           6.8101
	-delta_H	-151.779	kJ/mol	# Calculated enthalpy of reaction	Kaolinite
#	Enthalpy of formation:	-982.221 kcal/mol
        -analytic 1.6835e+001 -7.8939e-003 7.7636e+003 -1.2190e+001 -3.2354e+005
#       -Range:  0-300
	-Vm 99.35 # phreeqc.dat
 
 
 
Clinop-Mg	## Zeolite, Secondary Mineral, INVENTED Mg-Clinoptilolite # Thermodynamics as Clinop-Ca
        Mg1.7335Al3.45Fe.017Si14.533O36:10.922H2O +13.8680 H+ + 29.0660 H2O =  + 0.0170 Fe+++ + 1.7335 Mg++ + 3.4500 Al+++ + 14.5330 H4SiO4 + 17.8560 H2O
	    log_k           28.6255
	-delta_H	-329.278	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite-dehy-Ca
#	Enthalpy of formation:	-4112.83 kcal/mol
        -analytic -1.2948e+002 6.5698e-002 8.0229e+004 -1.2812e+001 -8.8320e+006
#       -Range:  0-300
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite
 
 
##### ASH composition from F Tepley ##### Thermodynamics as SiO2(am) in llnl.dat & -Vm Chalcedony in phreeqc.dat
#########################################
 
Ash_B13H1	### O balances all cations, Na is balanced by Cl and the remaining by O
(SiO2)0.259(AlO1.5)0.0533(Na0.0274O0.01300)(MgO)0.0006Cl0.0014(KO0.5)0.0149(CaO)0.0054(MnO)0.0003(FeO)0.0055 + 0.405800 H2O + 0.2244 H+ = 0.259H4SiO4 + 0.0533Al+3 + 0.0274Na+ + 0.0006Mg+2 + 0.0014Cl- + 0.0149K+ + 0.0054Ca+2 + 0.0003Mn+2 + 0.0055Fe+2
 
############
Ash_SrBa	### Same as Ash_B13H1, but w/ Sr & Ba. O balances all cations, Na is balanced by Cl and the remaining by O
##### 0.0055 Sr & Ba
#(SiO2)0.259(AlO1.5)0.0533(Na0.0274O0.01300)(MgO)0.0006Cl0.0014(KO0.5)0.0149(CaO)0.0054(MnO)0.0003(FeO)0.0055(BaO)0.0055(SrO)0.0055   + 0.405800 H2O + 0.2244 H+ + 0.011 H+ = \
#    0.259H4SiO4 + 0.0533Al+3 + 0.0274Na+ + 0.0006Mg+2 + 0.0014Cl- + 0.0149K+ + 0.0054Ca+2 + 0.0003Mn+2 + 0.0055Fe+2 + 0.0055Ba+2 + 0.0055Sr+2 + 0.011OH-
 
##### 0.0001 Sr & Ba
(SiO2)0.259(AlO1.5)0.0533(Na0.0274O0.01300)(MgO)0.0006Cl0.0014(KO0.5)0.0149(CaO)0.0054(MnO)0.0003(FeO)0.0055(BaO)0.0001(SrO)0.0001   + 0.405800 H2O + 0.2244 H+ + 0.0002 H+ = \
    0.259H4SiO4 + 0.0533Al+3 + 0.0274Na+ + 0.0006Mg+2 + 0.0014Cl- + 0.0149K+ + 0.0054Ca+2 + 0.0003Mn+2 + 0.0055Fe+2 + 0.0001Ba+2 + 0.0001Sr+2 + 0.0002OH-
 
        log_k           -2.7136
	-delta_H	20.0539	kJ/mol	# Calculated enthalpy of reaction	SiO2(am)
#	Enthalpy of formation:	-214.568 kcal/mol
        -analytic 1.2109e+000 7.0767e-003 2.3634e+003 -3.4449e+000 -4.8591e+005
#       -Range:  0-300
	-Vm 23.1
 
 
 
Ash_B23X3	### O balances all cations, Na is balanced by Cl and the remaining by O
(SiO2)0.24874(AlO1.5)0.05649(Na0.03187O0.015075)(MgO)0.003Cl0.00172(KO0.5)0.01113(CaO)0.00898(MnO)0.00019(FeO)0.00869 + 0.371245 H2O + 0.25247 H+ = 0.24874H4SiO4 + 0.05649Al+3 + 0.03187Na+ + 0.003Mg+2 + 0.00172Cl- + 0.01113K+ + 0.00898Ca+2 + 0.00019Mn+2 + 0.00869Fe+2																								
								
 
 
 
######### Primary minerals with trace elements ##############
 
#############################################################
 
Albite-SrBa
(NaAlSi3O8).964(Na2SrAl4Si4O16).0102(Na2BaAl4Si4O16).0018 + 3.8560 H2O + 4.0480 H+ = 0.9880 Na+ + 1.0120 Al+3 + 2.9400 H4SiO4 + 0.0102 Sr+2 + 0.0018 Ba+2
        log_k           2.7645 # Albite data
	-delta_H	-51.8523	kJ/mol	# Calculated enthalpy of reaction	Albite
#	Enthalpy of formation:	-939.68 kcal/mol
        -analytic -1.1694e+001 1.4429e-002 1.3784e+004 -7.2866e+000 -1.6136e+006
#       -Range:  0-300
	-Vm 101.31	# from phreeqc.dat
 
 
###### Olivine: Fayalite-Forsterite-Tephroite (Fe, Mg, Mn) #####
 
Olivine_B13H1 #### MOLAR RATIOS according to Ash_B13H1
(Fe0.73Mg0.25Mn0.02)2SiO4 +4.0000 H+ + 2.0000 H2O =  + 1.0000 H4SiO4 + 1.4600 Fe++ + 0.50000Mg++ + 0.04000Mn++ + 2.0000 H2O
        log_k           19.1113 # Fayalite data
	-delta_H	-152.256	kJ/mol	# Calculated enthalpy of reaction	Fayalite
#	Enthalpy of formation:	-354.119 kcal/mol
        -analytic 1.3853e+001 -3.5501e-003 7.1496e+003 -6.8710e+000 -6.3310e+004
#       -Range:  0-300
	-Vm	46.42 # calculated from webmineral molecular weight & density data
 
END


EQUILIBRIUM_PHASES 1000
END
EXCHANGE 1000
END
GAS_PHASE 1000
END
KINETICS 1000
END
SOLID_SOLUTIONS 1000
END
SURFACE 1000
END
SOLUTION 1001 B1H4
 -units	mol/kgw
pH	7.220
pe  6.502
temp	1.70
pressure	467.00
density	1.05001
	Al                3.690e-05
	Ba                5.810e-07
	C(4)              3.060e-03
	Ca                1.170e-02
	Cl                5.640e-01
	Fe                1.690e-05
	K                 2.430e-02
	Mg                5.320e-02
	Mn                1.890e-04
	N(-3)             2.940e-04
	Na                4.850e-01
	S(6)              2.900e-02
	Si                4.890e-04
	Sr                9.170e-05
END
SOLUTION 1002 B1H4 w/ tracer
 -units	mol/kgw
pH	7.220
pe  6.502
temp	1.70
pressure	467.00
density	1.05001
	Al                3.690e-05
	Ba                5.810e-07
	C(4)              3.060e-03
	Ca                1.170e-02
	Cl                5.640e-01
	Fe                1.690e-05
	K                 2.430e-02
	Mg                5.320e-02
	Mn                1.890e-04
	N(-3)             2.940e-04
	Na                4.850e-01
	S(6)              2.900e-02
	Si                4.890e-04
	Sr                9.170e-05
Tr      0.001

END

EQUILIBRIUM_PHASES 1001
  Barite		0.13 	0 prec # SI at B1H4
  Gypsum		0		0 prec
  Anhydrite		0		0 prec
  Goethite  	4     	0 # SI=4 <=> [Fe(aq)] = 0.5*LOD. Limit Of Detection. All samples are below LOD
  Pyrite    	0     	0
END


RATES

Albite
-start
10 REM Palandri and Kharaka rate
11 REM p & q adjusted with PEST
20 area = (10^PARM(1)) * M
30 aH = act("H+")
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_acid = 10^-10.16
60 acid = ( k_acid ) * EXP((-65/R)*dif_temp) * aH^0.457
65 k_neut = 10^-12.56
70 neut = ( k_neut ) * EXP((-69.8/R)*dif_temp)
75 k_basic = 10^-15.60
80 basic = ( k_basic ) * EXP((-71/R)*dif_temp) * aH^-0.572
81 p = 10^( %    palb      % ) # -7.7754172E+00 # PEST
82 q = 1 # PEST. since it can be the power of a neg number, it has to be integer
90   rate = area * (acid + neut + basic) * (1- (SR("Albite")^p) )^q
160  moles = rate * TIME
300 SAVE moles
-end
 
Anorthite
-start
10 REM Palandri and Kharaka rate
11 REM p & q adjusted with PEST
12 if (SI("Anorthite") > 0 ) then goto 300 # only allowed to dissolve
20 area = (10^PARM(1)) * M
30 aH = act("H+")
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_acid = 10^-3.5
60 acid = ( k_acid ) * EXP((-16.6/R)*dif_temp) * aH^1.411
65 k_neut = 10^-9.12
70 neut = ( k_neut ) * EXP((-17.8/R)*dif_temp)
71 p = 10^(  -9.3) # -8.7867060E+00 # PEST
72 q = 1 # PEST. since it can be the power of a neg number, it has to be integer
90   rate = area * (acid + neut) * (1- (SR("Anorthite")^p) )^q
160  moles = rate * TIME
300 SAVE moles
-end
 
 
Clinop-Na
-start
10 REM only neutral mechanism
11 REM k_neut, p & q adjusted with PEST
12 REM Ea adjusted with PEST
	# 13 if (TOT("Al") < 1e-10) then goto 300
14 if (SI("Clinop-Na") < 0 ) then goto 300 # only allowed to precipitate
20 area = (10^PARM(1)) * M
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_neut = 10^-13 # PEST
60 p = 10^(  %    pclinop-na%) # -4.2724745E+00 # PEST
65 q = 1 # PEST. since it can be the power of a neg number, it has to be integer
68 Ea = 60 # PEST. 1st guess 60 kJ/mol (Lasaga, 1984)
70 neut = ( k_neut ) * EXP((-Ea/R)*dif_temp)
 
90   rate = area * ( neut ) * (1- SR("Clinop-Na")^p )^q
160  moles = rate * TIME
300 SAVE moles
-end

Clinop-K
-start
10 REM only neutral mechanism
11 REM k_neut, p & q adjusted with PEST
12 REM Ea adjusted with PEST
	# 13 if (TOT("Al") < 1e-10) then goto 300
14 if (SI("Clinop-K") < 0 ) then goto 300 # only allowed to precipitate
20 area = (10^PARM(1)) * M
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_neut = 10^-13 # PEST
60 p = 10^ (%    pclinop-k %) # -3.5 # PEST
65 q = 1 # PEST. since it can be the power of a neg number, it has to be integer
68 Ea = 60 # PEST. 1st guess 60 kJ/mol (Lasaga, 1984)
70 neut = ( k_neut ) * EXP((-Ea/R)*dif_temp)
 
90   rate = area * ( neut ) * (1- SR("Clinop-K")^p )^q
160  moles = rate * TIME
300 SAVE moles
-end

Clinop-Sr
-start
10 REM only neutral mechanism
11 REM k_neut, p & q adjusted with PEST
12 REM Ea adjusted with PEST
	# 13 if (TOT("Al") < 1e-10) then goto 300
14 if (SI("Clinop-Sr") < 0 ) then goto 300 # only allowed to precipitate
20 area = (10^PARM(1)) * M
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_neut = 10^-13 # PEST
60 p = 10^ ( -3.5 ) # -3.5  # PEST
65 q = 1 # PEST. since it can be the power of a neg number, it has to be integer
68 Ea = 60 # PEST. 1st guess 60 kJ/mol (Lasaga, 1984)
70 neut = ( k_neut ) * EXP((-Ea/R)*dif_temp)
 
90   rate = area * ( neut ) * (1- SR("Clinop-Sr")^p )^q
160  moles = rate * TIME
300 SAVE moles
-end
 
 
Clinoptilolite
-start
10 REM only neutral mechanism
11 REM k_neut, p & q adjusted with PEST
12 REM Ea adjusted with PEST
	# 13 If (TOT("Ba") < 1e-10 OR TOT("Al") < 1e-10) then goto 300
14 if (SI("Clinoptilolite") < 0 ) then goto 300 # only allowed to precipitate
 
20 area = (10^PARM(1)) * M
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_neut = 10^-13 # PEST
60 p = 10^( -6.776472 )	# -6.7764720E+00 # PEST
65 q = 1 # PEST. since it can be the power of a neg number, it has to be integer
68 Ea = 60 # PEST. 1st guess 60 kJ/mol (Lasaga, 1984)
70 neut = ( k_neut ) * EXP((-Ea/R)*dif_temp)
 
90   rate = area * ( neut ) * (1- SR("Clinoptilolite")^p )^q
160  moles = rate * TIME
300 SAVE moles
-end
 
Ash_SrBa
-start
10 REM only neutral mechanism
11 REM k_neut, p & q adjusted with PEST
12 REM Ea adjusted with PEST
13 If (SI("Chalcedony") > 0) then goto 300 # only allowed to dissolve
20 area = (10^PARM(1)) * M
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_neut = 10^-13 # PEST
60 p = 10^(  %    pash      %)	# -7.4190486E+00 # PEST
65 q = 1 # PEST. since it can be the power of a neg number, it has to be integer
68 Ea = 65 # PEST. Minerals that dissolve very fast, 21 kJ/mol (Lasaga, 1984)
70 neut = ( k_neut ) * EXP((-Ea/R)*dif_temp)
 
90   rate = area * ( neut ) * (1- SR("Chalcedony")^p )^q
160  moles = rate * TIME
300 SAVE moles
-end
 
Fayalite
-start
10 REM Palandri and Kharaka rate for Fayalite
11 REM p & q adjusted with PEST
15 if (SI("Fayalite") > 0 ) then goto 300 # only allowed to dissolve
# 16 if (SI("Fayalite") < -50 ) then goto 300 # stop when some constituent is absent
20 area = (10^PARM(1)) * M
30 aH = act("H+")
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_acid = 10^-4.8
60 acid = ( k_acid ) * EXP((-94.4/R)*dif_temp) * aH^0.47
65 k_neut = 10^-12.8
70 neut = ( k_neut ) * EXP((-94.4/R)*dif_temp)
81 p = 10^(  %    poliv     %)	# -7.8806472E+00 # PEST
82 q = 1 # PEST. since it can be the power of a neg number, it has to be integer
90   rate = area * (acid + neut) * (1- (SR("Fayalite")^p) )^q
160  moles = rate * TIME
300 SAVE moles
-end
 
 
Hedenbergite
-start
10 REM Palandri and Kharaka rate for Diopside
11 REM p & q adjusted with PEST
12 if (SI("Hedenbergite") > 0 ) then goto 300 # only allowed to dissolve
20 area = (10^PARM(1)) * M
30 aH = act("H+")
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_acid = 10^-6.36
60 acid = ( k_acid ) * EXP((-96.1/R)*dif_temp) * aH^0.71
65 k_neut = 10^-11.11
70 neut = ( k_neut ) * EXP((-40.6/R)*dif_temp)
71 p = 10^(  %    ppyrox    %)	# -6.0314393E+00 # PEST
72 q = 1 # PEST. since it can be the power of a neg number, it has to be integer
90   rate = area * (acid + neut) * (1- (SR("Hedenbergite")^p) )^q
160  moles = rate * TIME
300 SAVE moles
-end
 
Quartz
-start
10 REM Palandri and Kharaka rate for Quartz
11 REM p & q adjusted with PEST
12 if (SI("Quartz") > 0 ) then goto 300 # only allowed to dissolve
20 area = (10^PARM(1)) * M
30 aH = act("H+")
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_basic = 10^-16.29
60 basic = ( k_basic ) * EXP((-87.7/R)*dif_temp) * aH^-0.5
65 k_neut = 10^-13.99
70 neut = ( k_neut ) * EXP((-87.7/R)*dif_temp)
71 p = 10^(%    pqz       %) # -5.4 # PEST
72 q = 1 # PEST. since it can be the power of a neg number, it has to be integer
90   rate = area * (basic + neut) * (1- (SR("Quartz")^p) )^q
160  moles = rate * TIME
300 SAVE moles
-end
 
Chlorite(14A)
-start
10 REM Palandri and Kharaka rate
11 REM p & q adjusted with PEST
20 area = (10^PARM(1)) * M
30 aH = act("H+")
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_acid = 10^-11.11
60 acid = ( k_acid ) * EXP((-88.0/R)*dif_temp) * aH^0.5
65 k_neut = 10^-12.52
70 neut = ( k_neut ) * EXP((-88.0/R)*dif_temp)
71 p = 10^ ( %    pchl      %)	# -4.7600550E+00 # PEST
72 q = 1 # PEST. since it can be the power of a neg number, it has to be integer
90   rate = area * (acid + neut) * (1- (SR("Chlorite(14A)")^p) )^q
160  moles = rate * TIME
300 SAVE moles
-end

END

KINETICS 1000
-cvode

Albite
-m0 1.04E+00 # mol/L-sedim. Generic andesitic comp. 10wtperc Ab. 0 to 45mbsf: Poros= .74; Grain Dens=2.74kg/L;Porewater Dens=1.050kg/L
-formula Albite
-parm    1.9463848E+00 # 100 # silt size particles. specific-surf-area (m2/mol-of-min)
 
#!Anorthite
#!-m0 9.88E-01 # mol/L-sedim. Generic andesitic comp. 10wtperc An. 0 to 45mbsf: Poros= .74; Grain Dens=2.74kg/L;Porewater Dens=1.050kg/L
#!-formula Anorthite
#!-parm    1.9107242E+00 # 100 # silt size particles. specific-surf-area (m2/mol-of-min)
#! 
#!Clinop-Na
#!-m0 1e-20 # assumed almost zero
#!-formula Clinop-Na
#!-parm    2.7779020E+00 # 600 # assumed very small initial spheres. specific-surf-area (m2/mol-of-min)
#! 
#!#Clinoptilolite
#!#-m0 1e-20 # assumed almost zero
#!#-formula Clinoptilolite
#!#-parm    2.7779000E+00 # 600 # assumed very small initial spheres. specific-surf-area (m2/mol-of-min)
#! 
#!Ash_SrBa
#!-m0 4.23E+01 # mol/L-sedim. Generic andesitic comp. 25wtperc Ash. 0 to 45mbsf: Poros= .74; Grain Dens=2.74kg/L;Porewater Dens=1.050kg/L
#!-formula Ash_SrBa
#!-parm    2.2665400E+00 # 100 # silt size particles. specific-surf-area (m2/mol-of-min)
#! 
#!Fayalite
#!-m0 1.43E-01 # mol/L-sedim. Generic andesitic comp. 1.1wtperc Fayalite. 0 to 45mbsf: Poros= .74; Grain Dens=2.74kg/L;Porewater Dens=1.050kg/L
#!-formula Fayalite
#!-parm    1.9251094E+00 # 100 # silt size particles. specific-surf-area (m2/mol-of-min)
#! 
#!Hedenbergite
#!-m0 1.10E+00 # mol/L-sedim. Generic andesitic comp. 10wtperc Hedenb. 0 to 45mbsf: Poros= .74; Grain Dens=2.74kg/L;Porewater Dens=1.050kg/L
#!-formula Hedenbergite
#!-parm    1.8358232E+00 # 100 # silt size particles. specific-surf-area (m2/mol-of-min)
#! 
#!Quartz
#!-m0 2.43E+00 # mol/L-sedim. Generic andesitic comp. 5.3wtperc Qz. 0 to 45mbsf: Poros= .74; Grain Dens=2.74kg/L;Porewater Dens=1.050kg/L
#!-formula Quartz
#!-parm    1.9634959E+00 # 100 # silt size particles. specific-surf-area (m2/mol-of-min)
#! 
#!Chlorite(14A)
#!-m0 1e-20 # assumed almost zero
#!-formula Chlorite(14A)
#!-parm    2.7805280E+00 # 600 # assumed very small initial spheres. specific-surf-area (m2/mol-of-min)
#!
#!Clinop-K
#!-m0 1e-20 # assumed almost zero
#!-formula Clinop-K
#!-parm    2.7779020E+00 # 600 # assumed very small initial spheres. specific-surf-area (m2/mol-of-min)
#!
#!#Clinop-Sr
#!#-m0 1e-20 # assumed almost zero
#!#-formula Clinop-Sr
#!#-parm    2.7779020E+00 # 600 # assumed very small initial spheres. specific-surf-area (m2/mol-of-min)
#!

END

PRINT
-user_graph false
END


SELECTED_OUTPUT 1	
				
-file 01_1D.txt
-simulation	true
-state	true
-solution	true
-distance	true
-time	true
-step	true
-charge_balance
-ionic_strength
-high_prec true
-temp
-percent_error
-alkalinity
-totals	Na	Mg	K	Ca	N(-3)	C(4)	S(6)	Cl	Si	Sr	Ba	Mn	Al	Fe # input SOLUTION data for 1D models



USER_PUNCH 2
-headings	Ma P_atm Sol_dens_kg/L Sol_vol_L TDS_g/kg EC_uS/cm
10 PUNCH TOTAL_TIME / 3.155692608E+13 # to plot time in Ma
20 PUNCH pressure
30 PUNCH RHO
40 PUNCH SOLN_VOL
50 PUNCH (RHO * SOLN_VOL - TOT("Water")) * 1000 / (RHO * SOLN_VOL)
60 PUNCH SC 
 
SELECTED_OUTPUT 2
-file other_outputs.txt
-simulation	true
-state	true
-solution	true
-distance	true
-time	true
-step	true
-high_prec true
-saturation_indices
Albite	Anorthite	Olivine_B13H1	Ca-Al_Pyroxene Hedenbergite
Quartz	Calcite	Barite	Gypsum	Chalcedony	
Clinoptilolite	Clinop-Ca	Clinop-Na	Clinop-K	Clinop-Sr	Chlorite(14A)
Pyrolusite	Pyrochroite	Goethite Gibbsite
 
-equilibrium_phases Barite Gypsum	Anhydrite	Goethite Pyrite
 
-kinetic_reactants	Albite Anorthite Ash_SrBa	Fayalite	Hedenbergite Quartz
				Clinop-Na	Clinoptilolite	Chlorite(14A) Clinop-K	Clinop-Sr

END
				
				
