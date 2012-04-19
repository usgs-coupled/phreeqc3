10 base_species$ = "Fe(OH)2+"
20 base_species_charge$ = "+"
30 base_ligand$ = "OH-"
40 prefix$ = "Fe"
50 ligand_charge$ = "-"
60 n1 = 6
70 dim ligand$(n1)
80 data "OH","OD","OT","[18O]H","[18O]D","[18O]T"
90 restore 80
100 for i = 1 to n1
110  read ligand$(i)
120 next i
200 for i = 1 to n1
210 for j = i to n1
220 if (i = j) then gosub 1000
230 if not (i = j) then gosub 2000
240 next j
250 next i
260 end


1000 REM equation for same
1010 lhs$ = base_species$ + " + 2" + ligand$(i) + ligand_charge$
1020 rhs$ = prefix$ + "(" + ligand$(i) + ")2" + base_species_charge$ + " + 2" + base_ligand$
1030 print lhs$ + " = " + rhs$
1040 return
2000 REM equation for different
2010 lhs$ = base_species$ + " + " + ligand$(i) + ligand_charge$ + " + " + ligand$(j) + ligand_charge$
2020 rhs$ = prefix$ + ligand$(i) + ligand$(j) + base_species_charge$ + " + 2" + base_ligand$
2030 print lhs$ + " = " + rhs$
2040 return
