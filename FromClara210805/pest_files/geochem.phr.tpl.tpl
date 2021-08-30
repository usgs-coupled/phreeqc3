ptf %
DATABASE @phreeqcdatabase@
PHASES

Clinop-Ca	## Zeolite, Secondary Mineral
    #    Ca1.7335Al3.45Fe.017Si14.533O36:10.922H2O +13.8680 H+  =  + 0.0170 Fe+++ + 1.7335 Ca++ + 3.4500 Al+++ + 14.5330 SiO2 + 17.8560 H2O
        Ca1.7335Al3.45Fe.017Si14.533O36:10.922H2O +13.8680 H+ + 29.0660 H2O =  + 0.0170 Fe+++ + 1.7335 Ca++ + 3.4500 Al+++ + 14.5330 H4SiO4 + 17.8560 H2O
	  log_k           -7.0095
	-delta_H	-74.6745	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite-Ca
#	Enthalpy of formation:	-4919.84 kcal/mol
        -analytic -4.4820e+001 5.3696e-002 5.4878e+004 -3.1459e+001 -7.5491e+006
		-add_constant % kclinop-Ca %
#       -Range:  0-300
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite
 
 
Clinop-K	## Zeolite, Secondary Mineral
   #     K3.467Al3.45Fe.017Si14.533O36:10.922H2O +13.8680 H+  =  + 0.0170 Fe+++ + 3.4500 Al+++ + 3.4670 K+ + 14.5330 SiO2 + 17.8560 H2O
        K3.467Al3.45Fe.017Si14.533O36:10.922H2O +13.8680 H+  + 29.0660 H2O =  + 0.0170 Fe+++ + 3.4500 Al+++ + 3.4670 K+ + 14.5330 H4SiO4 + 17.8560 H2O
       log_k           -10.9485
	-delta_H	67.4862	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite-K
#	Enthalpy of formation:	-4937.77 kcal/mol
        -analytic 1.1697e+001 6.9480e-002 4.7718e+004 -4.7442e+001 -7.6907e+006
		-add_constant % kclinop-K %
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
		-add_constant % kclinop-Na %
#       -Range:  0-300
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite
 
Clinop-Sr	## Zeolite, Secondary Mineral
 #       Sr1.7335Al3.45Fe.017Si14.533O36:10.922H2O +13.8680 H+  =  + 0.0170 Fe+++ + 1.7335 Sr++ + 3.4500 Al+++ + 14.5330 SiO2 + 17.8560 H2O
        Sr1.7335Al3.45Fe.017Si14.533O36:10.922H2O +13.8680 H+  + 29.0660 H2O =  + 0.0170 Fe+++ + 1.7335 Sr++ + 3.4500 Al+++ + 14.5330 H4SiO4 + 17.8560 H2O
       log_k           -7.1491
	-delta_H	-66.2129	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite-Sr
#	Enthalpy of formation:	-4925.1 kcal/mol
        -analytic 3.2274e+001 6.7050e-002 5.0880e+004 -5.9597e+001 -7.3876e+006
		-add_constant % kclinop-Sr %
#       -Range:  0-300
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite

Clinop-Mg	## Zeolite, Secondary Mineral
 #       Sr1.7335Al3.45Fe.017Si14.533O36:10.922H2O +13.8680 H+  =  + 0.0170 Fe+++ + 1.7335 Sr++ + 3.4500 Al+++ + 14.5330 SiO2 + 17.8560 H2O
        Mg1.7335Al3.45Fe.017Si14.533O36:10.922H2O +13.8680 H+  + 29.0660 H2O =  + 0.0170 Fe+++ + 1.7335 Mg++ + 3.4500 Al+++ + 14.5330 H4SiO4 + 17.8560 H2O
       log_k           -7.1491
	-delta_H	-66.2129	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite-Sr
#	Enthalpy of formation:	-4925.1 kcal/mol
        -analytic 3.2274e+001 6.7050e-002 5.0880e+004 -5.9597e+001 -7.3876e+006
		-add_constant % kclinop-Mg %
#       -Range:  0-300
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite		
 
Clinop-Fe2	## Zeolite, Secondary Mineral
#       Sr1.7335Al3.45Fe.017Si14.533O36:10.922H2O +13.8680 H+  =  + 0.0170 Fe+++ + 1.7335 Sr++ + 3.4500 Al+++ + 14.5330 SiO2 + 17.8560 H2O
        Fe1.7335Al3.45Fe.017Si14.533O36:10.922H2O +13.8680 H+  + 29.0660 H2O =  + 0.0170 Fe+++ + 1.7335 Fe++ + 3.4500 Al+++ + 14.5330 H4SiO4 + 17.8560 H2O
        log_k           -7.1491
	-delta_H	-66.2129	kJ/mol	# Calculated enthalpy of reaction	Clinoptilolite-Sr
#	Enthalpy of formation:	-4925.1 kcal/mol
        -analytic 3.2274e+001 6.7050e-002 5.0880e+004 -5.9597e+001 -7.3876e+006
		-add_constant % kclinop-Fe2 %
#       -Range:  0-300
	-Vm	633.1 # Gaucher et al. 2004 (App Geoch), Na, K & Ca Clinoptilolite	
	
END
SOLUTION_SPECIES
H+ = H+; -Dw 1e-9; -gamma       9.0     0
H2O = OH- + H+
        -analytic  293.29227  0.1360833  -10576.913  -123.73158  0  -6.996455e-5
        -gamma  3.5     0
        -dw      1e-9  470
        -Vm  -9.66  28.5  80.0 -22.9 1.89 0 1.09 0 0 1 # ref. 1
		
H2O + 0.01e- = H2O-0.01
log_k -8
        
SOLUTION_MASTER_SPECIES
Mn(+6)   MnO4-2		0	Mn
SOLUTION_SPECIES

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
#(SiO2)0.259(AlO1.5)0.0533(Na0.0274O0.01300)(MgO)0.0006Cl0.0014(KO0.5)0.0149(CaO)0.0054(MnO)0.0003(FeO)0.0055(BaO)0.0004(SrO)0.0004   + 0.405800 H2O + 0.2244 H+ + 0.0008 H+ = \
#    0.259H4SiO4 + 0.0533Al+3 + 0.0274Na+ + 0.0006Mg+2 + 0.0014Cl- + 0.0149K+ + 0.0054Ca+2 + 0.0003Mn+2 + 0.0055Fe+2 + 0.0004Ba+2 + 0.0004Sr+2 + 0.0008OH-

(SiO2)0.259(AlO1.5)0.0533(Na0.0274O0.01300)(MgO)0.0006Cl0.0014(KO0.5)0.0149(CaO)0.0054(MnO)0.0003(Fe2O3)0.00275(BaO)0.0004(SrO)0.0004   + 0.405800 H2O + 0.2244 H+ + 0.0008 H+ + 0.00275H2O = \
    0.259H4SiO4 + 0.0533Al+3 + 0.0274Na+ + 0.0006Mg+2 + 0.0014Cl- + 0.0149K+ + 0.0054Ca+2 + 0.0003Mn+2 + 0.0055Fe+3 + 0.0004Ba+2 + 0.0004Sr+2 + 0.0008OH- + 0.0055OH- 

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

SiO2(a)
	SiO2 + 2 H2O = H4SiO4
	-log_k	-2.71
	-delta_h  3.340 kcal
	-analytic	-0.26	0.0	-731.0	
	-add_constant % kSiO2 %
	
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
USER_PUNCH 1
-heading S6 J_tot_Cl- J_conc_Cl- J_psi_Cl- Uphill H2O_kg Cl_mol
10 j_tot = MCD_Jtot("Cl-")
20 j_conc = MCD_Jconc("Cl-")
30 j_psi = j_tot - j_conc
40 uphill$ = "No"
50 if j_tot * j_conc < 0 then uphill$ = "UPHILL"
60 PUNCH TOT("S(6)"), MCD_Jtot("Cl-"), MCD_Jconc("Cl-"), j_psi, uphill$, TOT("water"), TOTMOL("Cl")
END
SOLUTION 1000 B1H4
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
	#N(-3)             2.940e-04
	Na                4.850e-01 charge
	S(6)              2.900e-02
	Si                4.890e-04
	Sr                9.170e-05
END
SELECTED_OUTPUT 1
-active false
END
KNOBS
-step 2
-pe   1.5
-iter 200
-diagonal
#-tol 1e-17
SOLID_SOLUTION 1000
Clinop_ss
  -comp Clinop-Sr      1e-4
  -comp Clinop-K       1e-4
  -comp Clinop-Na      1e-4
# -comp Clinop-Ca      1e-4
# -comp Clinop-Mg      1e-4
# -comp Clinop-Fe2     1e-4
END


EQUILIBRIUM_PHASES 1000
  Barite         % kBarite %             0 # prec # SI at B1H4
  Anhydrite      0                       0 # prec
# Goethite       0                       0 # SI=4 <=> [Fe(aq)] = 0.5*LOD. Limit Of Detection. All samples are below LOD
  Hematite       0                       0
  Pyrite         0                       0 # prec
  Siderite       0                       0
  Pyrolusite     0                       0
  Pyrochroite    0                       0
  Rhodochrosite  0                       0
  Manganite      0                       0
  Hausmannite    0                       0
# Strontianite   0                       0
  Calcite        0                       0
#  Gibbsite      0                       0 # prec
#  Kaolinite     0                       0
#  Quartz        0                       0
#  SiO2(a)       0                       0
#  Clinop-Sr     0                       0
#  Clinop-K      0                       0
#  Clinop-Na     0                       0
#  Clinop-Ca     0                       0
#  Clinop-Mg     0                       0
#  Clinop-Fe2    0                       0
  Chlorite(14A)  % kChlor %              0
  Illite         % kIllite %             0
END

RATES

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
60 p = 1
60 p = % pAsh % # 1 # 0.08 # 10^(  -7.4190486E+00)
65 q = 1 # PEST. since it can be the power of a neg number, it has to be integer
65 q = % qAsh %
68 Ea = 65 # PEST. Minerals that dissolve very fast, 21 kJ/mol (Lasaga, 1984)
70 neut = ( k_neut ) * EXP((-Ea/R)*dif_temp)
90  rate = area * ( neut ) * (1- SR("SiO2(a)")^p )^q
160  moles = rate * TIME
300 SAVE moles
-end

END

KINETICS 1000
#-cvode

Anorthite
-m0 9.88E-01 # mol/L-sedim. Generic andesitic comp. 10wtperc An. 0 to 45mbsf: Poros= .74; Grain Dens=2.74kg/L;Porewater Dens=1.050kg/L
-formula Anorthite
-parm	%    r_anor      % # -7 # PEST. specific-surf-area (m2/mol-of-min)

Ash_SrBa
-m0 4.23E+01 # mol/L-sedim. Generic andesitic comp. 25wtperc Ash. 0 to 45mbsf: Poros= .74; Grain Dens=2.74kg/L;Porewater Dens=1.050kg/L
-formula Ash_SrBa
-parm	%    r_Ash_SrBa  % #  -7.5 # -7 # PEST. specific-surf-area (m2/mol-of-min)
 
END

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
				
-solid_solutions 	Clinop-Na	Clinop-K	Clinop-Sr	Clinop-Ca Clinop-Mg	
END
