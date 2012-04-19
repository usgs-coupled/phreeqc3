10 base_species$ = "Fe(OH)4-"
20 base_species_charge$ = "-"
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
220 for k = j to n1
230 for l = k to n1
240 if (i = j) and (j = k) and (k = l) then gosub 1000
250 if (i = j) and (j = k) and not (k = l) then gosub 2000
260 if (i = j) and not (j = k) and (k = l) then gosub 3000
270 if (i = j) and not (j = k) and not (k = l) then gosub 4000
280 if not (i = j) and (j = k) and (k = l) then gosub 5000
290 if not (i = j) and (j = k) and not (k = l) then gosub 6000
300 if not (i = j) and not (j = k) and (k = l) then gosub 7000
310 if not (i = j) and not (j = k) and not (k = l) then gosub 8000
320 next l
330 next k
340 next j
350 next i
360 end

1000 REM equation for different
1010 lhs$ = base_species$ 
1020 lhs$ = lhs$ + " + 4" + ligand$(i) + ligand_charge$ 
1060 rhs$ = prefix$ 
1070 rhs$ = rhs$ + "(" + ligand$(i) + ")4"
1110 rhs$ = rhs$ + base_species_charge$ 
1120 rhs$ = rhs$ + " + 4" + base_ligand$
1130 print lhs$ + " = " + rhs$
1140 return

2000 REM equation for different
2010 lhs$ = base_species$ 
2020 lhs$ = lhs$ + " + 3" + ligand$(i) + ligand_charge$ 
2040 lhs$ = lhs$ + " + " + ligand$(l) + ligand_charge$ 
2060 rhs$ = prefix$ 
2070 rhs$ = rhs$ + "(" + ligand$(i) + ")3"
2090 rhs$ = rhs$ + ligand$(l) 
2110 rhs$ = rhs$ + base_species_charge$ 
2120 rhs$ = rhs$ + " + 4" + base_ligand$
2130 print lhs$ + " = " + rhs$
2140 return

3000 REM equation for different
3010 lhs$ = base_species$ 
3020 lhs$ = lhs$ + " + 2" + ligand$(i) + ligand_charge$ 
3040 lhs$ = lhs$ + " + 2" + ligand$(k) + ligand_charge$ 
3060 rhs$ = prefix$ 
3070 rhs$ = rhs$ + "(" + ligand$(i) + ")2"
3090 rhs$ = rhs$ + "(" + ligand$(k) + ")2"
3110 rhs$ = rhs$ + base_species_charge$ 
3120 rhs$ = rhs$ + " + 4" + base_ligand$
3130 print lhs$ + " = " + rhs$
3140 return

4000 REM equation for different
4010 lhs$ = base_species$ 
4020 lhs$ = lhs$ + " + 2" + ligand$(i) + ligand_charge$ 
4040 lhs$ = lhs$ + " + " + ligand$(k) + ligand_charge$ 
4050 lhs$ = lhs$ + " + " + ligand$(l) + ligand_charge$ 
4060 rhs$ = prefix$ 
4070 rhs$ = rhs$ + "(" + ligand$(i) + ")2"
4090 rhs$ = rhs$ + ligand$(k) 
4100 rhs$ = rhs$ + ligand$(l) 
4110 rhs$ = rhs$ + base_species_charge$ 
4120 rhs$ = rhs$ + " + 4" + base_ligand$
4130 print lhs$ + " = " + rhs$
4140 return

5000 REM equation for different
5010 lhs$ = base_species$ 
5020 lhs$ = lhs$ + " + " + ligand$(i) + ligand_charge$ 
5030 lhs$ = lhs$ + " + 3" + ligand$(j) + ligand_charge$ 
5060 rhs$ = prefix$ 
5070 rhs$ = rhs$ + ligand$(i) 
5080 rhs$ = rhs$ + "(" + ligand$(j) + ")3"
5110 rhs$ = rhs$ + base_species_charge$ 
5120 rhs$ = rhs$ + " + 4" + base_ligand$
5130 print lhs$ + " = " + rhs$
5140 return

6000 REM equation for different
6010 lhs$ = base_species$ 
6020 lhs$ = lhs$ + " + " + ligand$(i) + ligand_charge$ 
6030 lhs$ = lhs$ + " + 2" + ligand$(j) + ligand_charge$ 
6050 lhs$ = lhs$ + " + " + ligand$(l) + ligand_charge$ 
6060 rhs$ = prefix$ 
6070 rhs$ = rhs$ + ligand$(i) 
6080 rhs$ = rhs$ + "(" + ligand$(j) + ")2"
6100 rhs$ = rhs$ + ligand$(l) 
6110 rhs$ = rhs$ + base_species_charge$ 
6120 rhs$ = rhs$ + " + 4" + base_ligand$
6130 print lhs$ + " = " + rhs$
6140 return

7000 REM equation for different
7010 lhs$ = base_species$ 
7020 lhs$ = lhs$ + " + " + ligand$(i) + ligand_charge$ 
7030 lhs$ = lhs$ + " + " + ligand$(j) + ligand_charge$ 
7040 lhs$ = lhs$ + " + 2" + ligand$(k) + ligand_charge$ 
7060 rhs$ = prefix$ 
7070 rhs$ = rhs$ + ligand$(i) 
7080 rhs$ = rhs$ + ligand$(j)
7100 rhs$ = rhs$ + "(" + ligand$(k) + ")2"
7110 rhs$ = rhs$ + base_species_charge$ 
7120 rhs$ = rhs$ + " + 4" + base_ligand$
7130 print lhs$ + " = " + rhs$
7140 return

8000 REM equation for different
8010 lhs$ = base_species$ 
8020 lhs$ = lhs$ + " + " + ligand$(i) + ligand_charge$ 
8030 lhs$ = lhs$ + " + " + ligand$(j) + ligand_charge$ 
8040 lhs$ = lhs$ + " + " + ligand$(k) + ligand_charge$ 
8050 lhs$ = lhs$ + " + " + ligand$(l) + ligand_charge$ 
8060 rhs$ = prefix$ 
8070 rhs$ = rhs$ + ligand$(i) 
8080 rhs$ = rhs$ + ligand$(j)
8090 rhs$ = rhs$ + ligand$(k) 
8100 rhs$ = rhs$ + ligand$(l) 
8110 rhs$ = rhs$ + base_species_charge$ 
8120 rhs$ = rhs$ + " + 4" + base_ligand$
8130 print lhs$ + " = " + rhs$
8140 return


