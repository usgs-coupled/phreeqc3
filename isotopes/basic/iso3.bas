10 base_species$ = "Fe(OH)3"
20 base_species_charge$ = ""
30 base_ligand$ = "OH-"
40 prefix$ = "Fe"
50 ligand_charge$ = "-"
60 n1 = 6
70 dim ligand$(n1)
80 data "OH","OD","OT","[18O]H","[18O]D","[18O]D"
90 restore 80
100 for i = 1 to n1
110  read ligand$(i)
120 next i
200 for i = 1 to n1
210 for j = i to n1
220 for k = j to n1
230 if (i = j) and (j = k) then gosub 1000
240 if (i = j) and not (j = k) then gosub 2000
250 if not (i = j) and (j = k) then gosub 3000
260 if not (i = j) and not (j = k) then gosub 3000
270 next k
280 next j
290 next i
300 end

1000 REM equation for same
1010 lhs$ = base_species$ + " + 3" + ligand$(i) + ligand_charge$
1020 rhs$ = prefix$ + "(" + ligand$(i) + ")3" + base_species_charge$ + " + 3" + base_ligand$
1030 print lhs$ + " = " + rhs$
1040 return
2000 REM equation for different
2010 lhs$ = base_species$ + " + 2" + ligand$(i) + ligand_charge$ + " + " + ligand$(k) + ligand_charge$
2020 rhs$ = prefix$ + "(" + ligand$(i) + ")2" + ligand$(k) + base_species_charge$ + " + 3" + base_ligand$
2030 print lhs$ + " = " + rhs$
2040 return
3000 REM equation for different
3010 lhs$ = base_species$ + " + " + ligand$(i) + ligand_charge$ + " + 2" + ligand$(j) + ligand_charge$
3020 rhs$ = prefix$ + ligand$(i) + "(" + ligand$(j) + ")2" + base_species_charge$ + " + 3" + base_ligand$
3030 print lhs$ + " = " + rhs$
3040 return
4000 REM equation for different
4010 lhs$ = base_species$ + " + " + ligand$(i) + ligand_charge$ + " + " + ligand$(j) + ligand_charge$ + ligand$(k) + ligand_charge$ 
4020 rhs$ = prefix$ + ligand$(i) + ligand$(j) + ligand$(k) + base_species_charge$ + " + 3" + base_ligand$
4030 print lhs$ + " = " + rhs$
4040 return
