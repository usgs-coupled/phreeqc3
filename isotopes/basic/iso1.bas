10 base_species$ = "FeHSO4+"
20 base_species_charge$ = "+"
30 base_ligand$ = "HSO4-"
40 prefix$ = "Fe"
50 ligand_charge$ = "-"
60 n1 = 6
70 dim ligand$(n1)
80 data "HSO4","DSO4","TSO4","H[34S]O4","D[34S]O4","T[34S]O4"
90 restore 80
100 for i = 1 to n1
110  read ligand$(i)
120 next i
200 for i = 1 to n1
210 lhs$ = base_species$ + " + " + ligand$(i) + ligand_charge$
220 rhs$ = prefix$ + ligand$(i) + base_species_charge$ + " + " + base_ligand$
230 print lhs$ + " = " + rhs$
240 next i
250 end

10000 data "OH","OD","OT","[18O]H","[18O]D","[18O]T"
10010 data "CO3","CO2[18O]","CO[18O]2","C[18O]3","[13C]O3","[13C]O2[18O]","[13C]O[18O]2","[13C][18O]3","[14C]O3","[14C]O2[18O]","[14C]O[18O]2","[14C][18O]3"

10080 data "HCO3","HCO2[18O]","HCO[18O]2","HC[18O]3","H[13C]O3","H[13C]O2[18O]","H[13C]O[18O]2","H[13C][18O]3","H[14C]O3","H[14C]O2[18O]","H[14C]O[18O]2","H[14C][18O]3"
10081 data "DCO3","DCO2[18O]","DCO[18O]2","DC[18O]3","D[13C]O3","D[13C]O2[18O]","D[13C]O[18O]2","D[13C][18O]3","D[14C]O3","D[14C]O2[18O]","D[14C]O[18O]2","D[14C][18O]3"
10082 data "TCO3","TCO2[18O]","TCO[18O]2","TC[18O]3","T[13C]O3","T[13C]O2[18O]","T[13C]O[18O]2","T[13C][18O]3","T[14C]O3","T[14C]O2[18O]","T[14C]O[18O]2","T[14C][18O]3"
