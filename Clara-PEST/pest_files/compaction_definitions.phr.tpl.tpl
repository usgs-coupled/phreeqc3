ptf %
DATABASE @phreeqcdatabase@



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
  Celestite     0       0
END


RATES

Albite
-start
10 REM Palandri and Kharaka rate
11 REM p & q adjusted with PEST
13 z = 10*SI("Anorthite")
14 tanh = (exp(z) - exp(-z))/(exp(z) + exp(-z))
15 f = -0.5*tanh+0.5
#20 area = (10^PARM(1)) * M
20 area = (10^%    aalb       %) 
30 aH = act("H+")
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_acid = 10^-10.16
60 acid = ( k_acid ) * EXP((-65/R)*dif_temp) * aH^0.457
65 k_neut = 10^-12.56
70 neut = ( k_neut ) * EXP((-69.8/R)*dif_temp)
75 k_basic = 10^-15.60
80 basic = ( k_basic ) * EXP((-71/R)*dif_temp) * aH^-0.572
81 p = 10^( %    palb       % ) # -7.7754172E+00 # PEST
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
#20 area = (10^PARM(1)) * M
20 area = (10^%    aan        %) 
30 aH = act("H+")
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_acid = 10^-3.5
60 acid = ( k_acid ) * EXP((-16.6/R)*dif_temp) * aH^1.411
65 k_neut = 10^-9.12
70 neut = ( k_neut ) * EXP((-17.8/R)*dif_temp)
#71 p = 10^(  -9.3) # -8.7867060E+00 # PEST
71 p = 10^(  %    pan        %) # -8.7867060E+00 # PEST
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
#!14 if (SI("Clinop-Na") < 0 ) then goto 300 # only allowed to precipitate
#20 area = (10^PARM(1)) * M
20 area = (10^%    aclinop-na %) 
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_neut = 10^-13 # PEST
60 p = 10^(  %    pclinop-na %) # -4.2724745E+00 # PEST
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
#!14 if (SI("Clinop-K") < 0 ) then goto 300 # only allowed to precipitate
#20 area = (10^PARM(1)) * M
20 area = (10^%    aclinop-k      %) 
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_neut = 10^-13 # PEST
60 p = 10^ (%    pclinop-k  %) # -3.5 # PEST
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
#!14 if (SI("Clinop-Sr") < 0 ) then goto 300 # only allowed to precipitate
#20 area = (10^PARM(1)) * M
20 area = (10^%    aclinop-sr %) 
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_neut = 10^-13 # PEST
#60 p = 10^ ( -3.5 ) # -3.5  # PEST
60 p = 10^ (%    pclinop-sr %) # -3.5  # PEST
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
#!14 if (SI("Clinoptilolite") < 0 ) then goto 300 # only allowed to precipitate
 
#20 area = (10^PARM(1)) * M
20 area = (10^%    aclinop    %) 
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_neut = 10^-13 # PEST
#60 p = 10^( -6.776472 )	# -6.7764720E+00 # PEST
60 p = 10^( %    pclinop    % )	# -6.7764720E+00 # PEST
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
13 z = 10*SI("Chalcedony")
14 tanh = (exp(z) - exp(-z))/(exp(z) + exp(-z))
15 f = -0.5*tanh+0.5
#!13 If (SI("Chalcedony") > 0) then goto 300 # only allowed to dissolve
#20 area = (10^PARM(1)) * M
20 area = (10^%    aash       %) 
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_neut = 10^-13 # PEST
60 p = 10^(  %    pash       %)	# -7.4190486E+00 # PEST
65 q = 1 # PEST. since it can be the power of a neg number, it has to be integer
68 Ea = 65 # PEST. Minerals that dissolve very fast, 21 kJ/mol (Lasaga, 1984)
70 neut = ( k_neut ) * EXP((-Ea/R)*dif_temp)
 
90   rate = area * ( neut ) * (1- SR("Chalcedony")^p )^q
160  moles = rate * TIME * f
300 SAVE moles
-end
 
Fayalite
-start
10 REM Palandri and Kharaka rate for Fayalite
11 REM p & q adjusted with PEST
13 z = 10*SI("Fayalite")
14 tanh = (exp(z) - exp(-z))/(exp(z) + exp(-z))
15 f = -0.5*tanh+0.5
#!15 if (SI("Fayalite") > 0 ) then goto 300 # only allowed to dissolve
# 16 if (SI("Fayalite") < -50 ) then goto 300 # stop when some constituent is absent
#20 area = (10^PARM(1)) * M
20 area = (10^%    aoliv      %) 
30 aH = act("H+")
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_acid = 10^-4.8
60 acid = ( k_acid ) * EXP((-94.4/R)*dif_temp) * aH^0.47
65 k_neut = 10^-12.8
70 neut = ( k_neut ) * EXP((-94.4/R)*dif_temp)
81 p = 10^(  %    poliv      %)	# -7.8806472E+00 # PEST
82 q = 1 # PEST. since it can be the power of a neg number, it has to be integer
90   rate = area * (acid + neut) * (1- (SR("Fayalite")^p) )^q
160  moles = rate * TIME
300 SAVE moles
-end
 
 
Hedenbergite
-start
10 REM Palandri and Kharaka rate for Diopside
11 REM p & q adjusted with PEST
13 z = 10*SI("Hedenbergite")
14 tanh = (exp(z) - exp(-z))/(exp(z) + exp(-z))
15 f = -0.5*tanh+0.5
#!12 if (SI("Hedenbergite") > 0 ) then goto 300 # only allowed to dissolve
#20 area = (10^PARM(1)) * M
20 area = (10^%    apyrox     %) 
30 aH = act("H+")
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_acid = 10^-6.36
60 acid = ( k_acid ) * EXP((-96.1/R)*dif_temp) * aH^0.71
65 k_neut = 10^-11.11
70 neut = ( k_neut ) * EXP((-40.6/R)*dif_temp)
71 p = 10^(  %    ppyrox     %)	# -6.0314393E+00 # PEST
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
#20 area = (10^PARM(1)) * M
20 area = (10^%    aqz        %) 
30 aH = act("H+")
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_basic = 10^-16.29
60 basic = ( k_basic ) * EXP((-87.7/R)*dif_temp) * aH^-0.5
65 k_neut = 10^-13.99
70 neut = ( k_neut ) * EXP((-87.7/R)*dif_temp)
71 p = 10^(%    pqz        %) # -5.4 # PEST
72 q = 1 # PEST. since it can be the power of a neg number, it has to be integer
90   rate = area * (basic + neut) * (1- (SR("Quartz")^p) )^q
160  moles = rate * TIME
300 SAVE moles
-end
 
Chlorite(14A)
-start
10 REM Palandri and Kharaka rate
11 REM p & q adjusted with PEST
#20 area = (10^PARM(1)) * M
20 area = (10^%    achl       %) 
30 aH = act("H+")
40 dif_temp = 1/TK - 1/298.15
50 R = 8.3144598 * 1E-3 # kJ*K-1*mol-1
55 k_acid = 10^-11.11
60 acid = ( k_acid ) * EXP((-88.0/R)*dif_temp) * aH^0.5
65 k_neut = 10^-12.52
70 neut = ( k_neut ) * EXP((-88.0/R)*dif_temp)
71 p = 10^ ( %    pchl       %)	# -4.7600550E+00 # PEST
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
 
Anorthite
-m0 9.88E-01 # mol/L-sedim. Generic andesitic comp. 10wtperc An. 0 to 45mbsf: Poros= .74; Grain Dens=2.74kg/L;Porewater Dens=1.050kg/L
-formula Anorthite
-parm    1.9107242E+00 # 100 # silt size particles. specific-surf-area (m2/mol-of-min)
 
Clinop-Na
-m0 1e-6 # 1e-20 # assumed almost zero
-formula Clinop-Na
-parm    2.7779020E+00 # 600 # assumed very small initial spheres. specific-surf-area (m2/mol-of-min)
 
#Clinoptilolite
#-m0 1e-20 # assumed almost zero
#-formula Clinoptilolite
#-parm    2.7779000E+00 # 600 # assumed very small initial spheres. specific-surf-area (m2/mol-of-min)
 
Ash_SrBa
-m0 4.23E+01 # mol/L-sedim. Generic andesitic comp. 25wtperc Ash. 0 to 45mbsf: Poros= .74; Grain Dens=2.74kg/L;Porewater Dens=1.050kg/L
-formula Ash_SrBa
-parm    2.2665400E+00 # 100 # silt size particles. specific-surf-area (m2/mol-of-min)

Clinop-K
-m0 1e-6 # # assumed almost zero
-formula Clinop-K
-parm    2.7779020E+00 # 600 # assumed very small initial spheres. specific-surf-area (m2/mol-of-min)

#Clinop-Sr
#-m0 1e-6 # 1e-20 # assumed almost zero
#-formula Clinop-Sr
#-parm    2.7779020E+00 # 600 # assumed very small initial spheres. specific-surf-area (m2/mol-of-min)

 
Fayalite
-m0 1.43E-01 # mol/L-sedim. Generic andesitic comp. 1.1wtperc Fayalite. 0 to 45mbsf: Poros= .74; Grain Dens=2.74kg/L;Porewater Dens=1.050kg/L
-formula Fayalite
-parm    1.9251094E+00 # 100 # silt size particles. specific-surf-area (m2/mol-of-min)
 
Hedenbergite
-m0 1.10E+00 # mol/L-sedim. Generic andesitic comp. 10wtperc Hedenb. 0 to 45mbsf: Poros= .74; Grain Dens=2.74kg/L;Porewater Dens=1.050kg/L
-formula Hedenbergite
-parm    1.8358232E+00 # 100 # silt size particles. specific-surf-area (m2/mol-of-min)
 
#!Quartz
#!-m0 2.43E+00 # mol/L-sedim. Generic andesitic comp. 5.3wtperc Qz. 0 to 45mbsf: Poros= .74; Grain Dens=2.74kg/L;Porewater Dens=1.050kg/L
#!-formula Quartz
#!-parm    1.9634959E+00 # 100 # silt size particles. specific-surf-area (m2/mol-of-min)
 
Chlorite(14A)
-m0 1e-6 # 1e-20 # assumed almost zero
-formula Chlorite(14A)
-parm    2.7805280E+00 # 600 # assumed very small initial spheres. specific-surf-area (m2/mol-of-min)


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
				
				
