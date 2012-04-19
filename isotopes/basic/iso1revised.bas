10 base_species$ = "FeHCO3+"
20 base_species_charge$ = "+"
30 base_ligand$ = "HCO3-"
40 prefix$ = "Fe"
50 ligand_charge$ = "-"
60 n1 = 72
70 dim ligand$(n1)

80 data "HCO3","HCO2[18O]","HCO[18O]O","HCO[18O]2",
81 data "HC[18O]O2","HC[18O]O[18O]","HC[18O]2O","HC[18O]3"
82 data "H[13C]O3","H[13C]O2[18O]","H[13C]O[18O]O","H[13C]O[18O]2",
83 data "H[13C][18O]O2","H[13C][18O]O[18O]","H[13C][18O]2O","H[13C][18O]3"
84 data "H[14C]O3","H[14C]O2[18O]","H[14C]O[18O]O","H[14C]O[18O]2",
85 data "H[14C][18O]O2","H[14C][18O]O[18O]","H[14C][18O]2O","H[14C][18O]3"
86 data "DCO3","DCO2[18O]","DCO[18O]O","DCO[18O]2",
87 data "DC[18O]O2","DC[18O]O[18O]","DC[18O]2O","DC[18O]3"
88 data "D[13C]O3","D[13C]O2[18O]","D[13C]O[18O]O","D[13C]O[18O]2",
89 data "D[13C][18O]O2","D[13C][18O]O[18O]","D[13C][18O]2O","D[13C][18O]3"
90 data "D[14C]O3","D[14C]O2[18O]","D[14C]O[18O]O","D[14C]O[18O]2",
91 data "D[14C][18O]O2","D[14C][18O]O[18O]","D[14C][18O]2O","D[14C][18O]3"
92 data "TCO3","TCO2[18O]","TCO[18O]O","TCO[18O]2",
93 data "TC[18O]O2","TC[18O]O[18O]","TC[18O]2O","TC[18O]3"
94 data "T[13C]O3","T[13C]O2[18O]","T[13C]O[18O]O","T[13C]O[18O]2",
95 data "T[13C][18O]O2","T[13C][18O]O[18O]","T[13C][18O]2O","T[13C][18O]3"
96 data "T[14C]O3","T[14C]O2[18O]","T[14C]O[18O]O","T[14C]O[18O]2",
97 data "T[14C][18O]O2","T[14C][18O]O[18O]","T[14C][18O]2O","T[14C][18O]3"

120 data_line = 79
130 for i = 1 to n1
140   if not ((i mod 4) = 1) then goto 170
150   data_line = data_line + 1
155   print data_line, i
160   restore data_line
170   read ligand$(i)
180 next i
200 for i = 1 to n1
210 lhs$ = base_species$ + " + " + ligand$(i) + ligand_charge$
220 rhs$ = prefix$ + ligand$(i) + base_species_charge$ + " + " + base_ligand$
230 print lhs$ + " = " + rhs$
240 next i
250 end

10000 rem data "OH","OD","OT","[18O]H","[18O]D","[18O]T"

10180 rem data "CO3","CO2[18O]","CO[18O]2","C[18O]3"
10181 rem data "[13C]O3","[13C]O2[18O]","[13C]O[18O]2","[13C][18O]3"
10182 rem data "[14C]O3","[14C]O2[18O]","[14C]O[18O]2","[14C][18O]3"

10279 REM  HCO3 isotopomers
10280 rem data "HCO3","HCO2[18O]","HCO[18O]O","HCO[18O]2",
10281 rem data "HC[18O]O2","HC[18O]O[18O]","HC[18O]2O","HC[18O]3"
10282 rem data "H[13C]O3","H[13C]O2[18O]","H[13C]O[18O]O","H[13C]O[18O]2",
10283 rem data "H[13C][18O]O2","H[13C][18O]O[18O]","H[13C][18O]2O","H[13C][18O]3"
10284 rem data "H[14C]O3","H[14C]O2[18O]","H[14C]O[18O]O","H[14C]O[18O]2",
10285 rem data "H[14C][18O]O2","H[14C][18O]O[18O]","H[14C][18O]2O","H[14C][18O]3"

10290 rem data "DCO3","DCO2[18O]","DCO[18O]O","DCO[18O]2",
10291 rem data "DC[18O]O2","DC[18O]O[18O]","DC[18O]2O","DC[18O]3"
10292 rem data "D[13C]O3","D[13C]O2[18O]","D[13C]O[18O]O","D[13C]O[18O]2",
10293 rem data "D[13C][18O]O2","D[13C][18O]O[18O]","D[13C][18O]2O","D[13C][18O]3"
10294 rem data "D[14C]O3","D[14C]O2[18O]","D[14C]O[18O]O","D[14C]O[18O]2",
10295 rem data "D[14C][18O]O2","D[14C][18O]O[18O]","D[14C][18O]2O","D[14C][18O]3"

10300 rem data "TCO3","TCO2[18O]","TCO[18O]O","TCO[18O]2",
10301 rem data "TC[18O]O2","TC[18O]O[18O]","TC[18O]2O","TC[18O]3"
10302 rem data "T[13C]O3","T[13C]O2[18O]","T[13C]O[18O]O","T[13C]O[18O]2",
10303 rem data "T[13C][18O]O2","T[13C][18O]O[18O]","T[13C][18O]2O","T[13C][18O]3"
10304 rem data "T[14C]O3","T[14C]O2[18O]","T[14C]O[18O]O","T[14C]O[18O]2",
10305 rem data "T[14C][18O]O2","T[14C][18O]O[18O]","T[14C][18O]2O","T[14C][18O]3"


10480 rem data "HCO3","HCO2[18O]","HCO[18O]2","HC[18O]3","H[13C]O3","H[13C]O2[18O]","H[13C]O[18O]2","H[13C][18O]3","H[14C]O3","H[14C]O2[18O]","H[14C]O[18O]2","H[14C][18O]3"
10481 rem data "DCO3","DCO2[18O]","DCO[18O]2","DC[18O]3","D[13C]O3","D[13C]O2[18O]","D[13C]O[18O]2","D[13C][18O]3","D[14C]O3","D[14C]O2[18O]","D[14C]O[18O]2","D[14C][18O]3"
10482 rem data "TCO3","TCO2[18O]","TCO[18O]2","TC[18O]3","T[13C]O3","T[13C]O2[18O]","T[13C]O[18O]2","T[13C][18O]3","T[14C]O3","T[14C]O2[18O]","T[14C]O[18O]2","T[14C][18O]3"

20080 rem data "HSO4","DSO4","TSO4","H[34S]O4","D[34S]O4","T[34S]O4"
30080 rem data "PO4","PO3[18O]","PO2[18O]2","PO[18O]3","P[18O]4"
