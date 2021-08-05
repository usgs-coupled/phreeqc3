ptf %
DATABASE @phreeqcdatabase@

SOLUTION_SPECIES
H+ = H+; -Dw 1e-9; -gamma       9.0     0
H2O = OH- + H+
        -analytic  293.29227  0.1360833  -10576.913  -123.73158  0  -6.996455e-5
        -gamma  3.5     0
        -dw      1e-9  470
        -Vm  -9.66  28.5  80.0 -22.9 1.89 0 1.09 0 0 1 # ref. 1
        
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
END
 
 
PHASES
##### Remineralization of l-alpha-Serine (as a solid phase): biological production of NH4+ ### MLever #####
Serine
C3H7NO3 + 6H2O = NH4+ + 5H2 + 2H+ + 3HCO3-
log_k -0.041852369	### data sent by MLever
 
 
##### ASH composition from F Tepley ##### Thermodynamics as SiO2(a) in llnl.dat & -Vm Chalcedony in phreeqc.dat
#########################################
 
Ash_B13H1	### O balances all cations, Na is balanced by Cl and the remaining by O
(SiO2)0.259(AlO1.5)0.0533(Na0.0274O0.01300)(MgO)0.0006Cl0.0014(KO0.5)0.0149(CaO)0.0054(MnO)0.0003(FeO)0.0055 + 0.405800 H2O + 0.2244 H+ = 0.259H4SiO4 + 0.0533Al+3 + 0.0274Na+ + 0.0006Mg+2 + 0.0014Cl- + 0.0149K+ + 0.0054Ca+2 + 0.0003Mn+2 + 0.0055Fe+2
 
############
Ash_SrBa	### Same as Ash_B13H1, but w/ Sr & Ba. O balances all cations, Na is balanced by Cl and the remaining by O
##### 0.0055 Sr & Ba
#(SiO2)0.259(AlO1.5)0.0533(Na0.0274O0.01300)(MgO)0.0006Cl0.0014(KO0.5)0.0149(CaO)0.0054(MnO)0.0003(FeO)0.0055(BaO)0.0055(SrO)0.0055   + 0.405800 H2O + 0.2244 H+ + 0.011 H+ = \
#    0.259H4SiO4 + 0.0533Al+3 + 0.0274Na+ + 0.0006Mg+2 + 0.0014Cl- + 0.0149K+ + 0.0054Ca+2 + 0.0003Mn+2 + 0.0055Fe+2 + 0.0055Ba+2 + 0.0055Sr+2 + 0.011OH-
 
##### 0.0004 Sr & Ba
(SiO2)0.259(AlO1.5)0.0533(Na0.0274O0.01300)(MgO)0.0006Cl0.0014(KO0.5)0.0149(CaO)0.0054(MnO)0.0003(FeO)0.0055(BaO)0.0004(SrO)0.0004   + 0.405800 H2O + 0.2244 H+ + 0.0008 H+ = \
    0.259H4SiO4 + 0.0533Al+3 + 0.0274Na+ + 0.0006Mg+2 + 0.0014Cl- + 0.0149K+ + 0.0054Ca+2 + 0.0003Mn+2 + 0.0055Fe+2 + 0.0004Ba+2 + 0.0004Sr+2 + 0.0008OH-
 
        log_k           -2.7136
	-delta_H	20.0539	kJ/mol	# Calculated enthalpy of reaction	SiO2(am)
#	Enthalpy of formation:	-214.568 kcal/mol
        -analytic 1.2109e+000 7.0767e-003 2.3634e+003 -3.4449e+000 -4.8591e+005
#       -Range:  0-300
	-Vm 23.1
 
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
SELECTED_OUTPUT 1	
				
-file 01_1D.txt
-simulation	false
-state		false
-solution	false
-distance	true
-time		true
-step		false
-charge_balance
-ionic_strength
# -high_prec true
-temp
-percent_error
-alkalinity
-totals	Na	Mg	K	Ca	N(-3)	C C(4)	S S(6)	Cl	Si	Sr	Ba	Mn	Al	Fe # input SOLUTION data for 1D models
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
SELECTED_OUTPUT 1
-active false
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
  Barite		% kBarite       % 	0 # prec # SI at B1H4
#  Gypsum		0	0 # prec
  Anhydrite		0	0 # prec
  Goethite  		0     	0 # SI=4 <=> [Fe(aq)] = 0.5*LOD. Limit Of Detection. All samples are below LOD
  Pyrite    		0     	0 # prec
#  Pyrolusite    	0       0
#  Pyrochroite   	0       0
#  Rhodochrosite    	0	0
#  Manganite     	0       0
#  Strontianite          0       0
  Calcite		0	0
  Gibbsite		0	0 # prec
#  Quartz                0       0
  Clinop-Sr             % kclinop-Sr    %       0
  Clinop-K              % kclinop-K     %       0
  Clinop-Na             % kclinop-Na    %       0
#  Clinop-Ca             % kclinop-Ca    %       0
  Chlorite(14A)	        % kChlor        %       0
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
81 p = 1 #  10^ -8.3
82 q = 1 # since it can be the power of a neg number, it has to be integer
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
71 p = 1 # 10^ -9.3
72 q = 1 # since it can be the power of a neg number, it has to be integer
90   rate = area * (acid + neut) * (1- (SR("Anorthite")^p) )^q
160  moles = rate * TIME
300 SAVE moles
-end
 
 
Clinop-Na
-start
10 REM only neutral mechanism
11 REM k_neut, p & q adjusted with PEST
12 REM Ea adjusted with PEST
14 if (SI("Clinop-Na") < 0 ) then goto 300 # only allowed to precipitate
20 area = (10^PARM(1)) # * M
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_neut = 10^-10 # PEST
60 p = 1 # 10^ -4.8
65 q = 1 # since it can be the power of a neg number, it has to be integer
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
14 if (SI("Clinop-K") < 0 ) then goto 300 # only allowed to precipitate
20 area = (10^PARM(1)) # * M
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_neut = 10^-10 # PEST
60 p = 1 # 10^ -4.0
65 q = 1 # since it can be the power of a neg number, it has to be integer
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
14 if (SI("Clinop-Sr") < 0 ) then goto 300 # only allowed to precipitate
20 area = (10^PARM(1)) # * M
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_neut = 10^-10 # PEST
60 p = 1 # 10^ -4.0
65 q = 1 # since it can be the power of a neg number, it has to be integer
68 Ea = 60 # PEST. 1st guess 60 kJ/mol (Lasaga, 1984)
70 neut = ( k_neut ) * EXP((-Ea/R)*dif_temp)
90   rate = area * ( neut ) * (1- SR("Clinop-Sr")^p )^q * TOT("Sr") / (1e-8 + TOT("Sr"))
160  moles = rate * TIME
300 SAVE moles
-end
 
Clinoptilolite
-start
10 REM only neutral mechanism
11 REM k_neut, p & q adjusted with PEST
12 REM Ea adjusted with PEST
14 if (SI("Clinoptilolite") < 0 ) then goto 300 # only allowed to precipitate
20 area = (10^PARM(1)) * M
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_neut = 10^-10 # PEST
60 p = 1 # 1 # 10^ -7.3
65 q = 1 # 1 # since it can be the power of a neg number, it has to be integer
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
13 If (SI("SiO2(a)") > 0) then goto 300 # only allowed to dissolve
20 area = (10^PARM(1)) * M
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_neut = 10^-10 # PEST
60 p = 1 # 0.08 # 10^(  -7.4190486E+00)
65 q = 1 # PEST. since it can be the power of a neg number, it has to be integer
68 Ea = 65 # PEST. Minerals that dissolve very fast, 21 kJ/mol (Lasaga, 1984)
70 neut = ( k_neut ) * EXP((-Ea/R)*dif_temp)
90   rate = area * ( neut ) * (1- SR("SiO2(a)")^p )^q
160  moles = rate * TIME
300 SAVE moles
-end

 
Fayalite
-start
10 REM Palandri and Kharaka rate for Fayalite
11 REM p & q adjusted with PEST
15 if (SI("Fayalite") > 0 ) then goto 300 # only allowed to dissolve
20 area = (10^PARM(1)) * M
30 aH = act("H+")
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_acid = 10^-4.8
60 acid = ( k_acid ) * EXP((-94.4/R)*dif_temp) * aH^0.47
65 k_neut = 10^-12.8
70 neut = ( k_neut ) * EXP((-94.4/R)*dif_temp)
81 p = 1 # 10^(  -7.8806472E+00)
82 q = 1 # since it can be the power of a neg number, it has to be integer
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
71 p = 1 # 10^(  -6.0314393E+00)
72 q = 1 # since it can be the power of a neg number, it has to be integer
90   rate = area * (acid + neut) * (1- (SR("Hedenbergite")^p) )^q
160  moles = rate * TIME
300 SAVE moles
-end

 
Quartz
-start
10 REM Palandri and Kharaka rate for Quartz
11 REM p & q adjusted with PEST
20 area = (10^PARM(1)) * M
30 aH = act("H+")
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_basic = 10^-16.29
60 basic = ( k_basic ) * EXP((-87.7/R)*dif_temp) * aH^-0.5
65 k_neut = 10^-13.99
70 neut = ( k_neut ) * EXP((-87.7/R)*dif_temp)
71 p = 1 # 10^(-5.4)
72 q = 1 # since it can be the power of a neg number, it has to be integer
90   rate = area * (basic + neut) * (1- (SR("Quartz")^p) )^q
160  moles = rate * TIME
300 SAVE moles
-end
 
Chlorite(14A)
-start
10 REM Palandri and Kharaka rate
11 REM p & q adjusted with PEST
20 area = (10^PARM(1)) # * M
30 aH = act("H+")
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_acid = 10^-11.11
60 acid = ( k_acid ) * EXP((-88.0/R)*dif_temp) * aH^0.5
65 k_neut = 10^-12.52
70 neut = ( k_neut ) * EXP((-88.0/R)*dif_temp)
71 p = 1 # 0.07 # 10^ -5.3
72 q = 1 # since it can be the power of a neg number, it has to be integer
90   rate = area * (acid + neut) * (1- (SR("Chlorite(14A)")^p) )^q
160  moles = rate * TIME
300 SAVE moles
-end


END

KINETICS 1000
-cvode

#Albite
#-m0 1.04E+00 # mol/L-sedim. Generic andesitic comp. 10wtperc Ab. 0 to 45mbsf: Poros= .74; Grain Dens=2.74kg/L;Porewater Dens=1.050kg/L
#-formula Albite
#-parm	  %    palb       % # -8 # PEST. specific-surf-area (m2/mol-of-min)
 
Anorthite
-m0 9.88E-01 # mol/L-sedim. Generic andesitic comp. 10wtperc An. 0 to 45mbsf: Poros= .74; Grain Dens=2.74kg/L;Porewater Dens=1.050kg/L
-formula Anorthite
-parm	%    panor      % # -7 # PEST. specific-surf-area (m2/mol-of-min)
 
#Clinop-Na
#-m0 % min_moles     % # assumed almost zero
#-formula Clinop-Na
#-parm	 %    pclinop-na % # -8 # -7 # PEST. specific-surf-area (m2/mol-of-min)
# 
#Clinoptilolite
#-m0 % min_moles     % # 1e-20 # assumed almost zero
#-formula 	Clinoptilolite
#-parm	  -9 # PEST. specific-surf-area (m2/mol-of-min)
 
Ash_SrBa
-m0 4.23E+01 # mol/L-sedim. Generic andesitic comp. 25wtperc Ash. 0 to 45mbsf: Poros= .74; Grain Dens=2.74kg/L;Porewater Dens=1.050kg/L
-formula Ash_SrBa
-parm	%    pAsh_SrBa  % #  -7.5 # -7 # PEST. specific-surf-area (m2/mol-of-min)
 
#Fayalite
#-m0 1.43E-01 # mol/L-sedim. Generic andesitic comp. 1.1wtperc Fayalite. 0 to 45mbsf: Poros= .74; Grain Dens=2.74kg/L;Porewater Dens=1.050kg/L
#-formula Fayalite
#-parm	%    pFay       % #  -9 # PEST. specific-surf-area (m2/mol-of-min)
# 
#Hedenbergite
#-m0 1.10E+00 # mol/L-sedim. Generic andesitic comp. 10wtperc Hedenb. 0 to 45mbsf: Poros= .74; Grain Dens=2.74kg/L;Porewater Dens=1.050kg/L
#-formula Hedenbergite
#-parm	%    pHed       % #  -7 # PEST. specific-surf-area (m2/mol-of-min)
# 
#Quartz
#-m0 2.43E+00 # mol/L-sedim. Generic andesitic comp. 5.3wtperc Qz. 0 to 45mbsf: Poros= .74; Grain Dens=2.74kg/L;Porewater Dens=1.050kg/L
#-formula Quartz
#-parm	%    pQtz       % #  -8 # PEST. specific-surf-area (m2/mol-of-min)
 
#Chlorite(14A)
#-m0 % min_moles     % # assumed almost zero
#-formula Chlorite(14A)
#-parm	%    pChlor     % #  -20 # -4.8# PEST. specific-surf-area (m2/mol-of-min)
 
#Clinop-K
#-m0 % min_moles     % # assumed almost zero
#-formula Clinop-K
#-parm	 %    pclinop-k % # -6 # PEST. specific-surf-area (m2/mol-of-min)
 
#Clinop-Sr
#-m0 % min_moles     % # assumed almost zero
#-formula Clinop-Sr
#-parm	%    pclinop-Sr % # -8 # PEST. specific-surf-area (m2/mol-of-min)

END

# KINETICS 1000
# END

PRINT
-user_graph false
END

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
# -high_prec true
-saturation_indices
Albite	Anorthite	Olivine_B13H1	Ca-Al_Pyroxene Hedenbergite
Quartz	Calcite	Barite	Gypsum	SiO2(a)	
Clinoptilolite	Clinop-Ca	Clinop-Na	Clinop-K	Clinop-Sr	Chlorite(14A)
Pyrolusite	Pyrochroite	Goethite Gibbsite
 
-equilibrium_phases Barite Gypsum	Anhydrite	Goethite Pyrite Calcite Gibbsite
 
-kinetic_reactants	Albite Anorthite Ash_SrBa	Fayalite	Hedenbergite Quartz
				Clinop-Na	Clinoptilolite	Chlorite(14A) Clinop-K	Clinop-Sr
END
