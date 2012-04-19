10 base_species$ = "Al(OH)4-"
20 base_species_charge$ = "-"
30 base_ligand$ = "OH-"
40 ligand_charge$ = "-"
50 prefix$ = "Al"
60 n1 = 6
70 dim ligand$(n1), lig$(4)
80 data "OH","OD","OT","[18O]H","[18O]D","[18O]T"
90 restore 80
100 for i = 1 to n1
110  read ligand$(i)
120 next i
180 total_eqns = 0
190 eqns = 0
200 for i = 1 to n1
210 for j = 1 to n1
220 for k = 1 to n1
230 for l = 1 to n1
250 gosub 8000
320 next l
330 next k
340 next j
350 next i
360 print eqns, total_eqns
370 end

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
8130 lig$(1) = ligand$(i)
8140 lig$(2) = ligand$(j)
8150 lig$(3) = ligand$(k)
8160 lig$(4) = ligand$(l)
8170 gosub 9000
8180 if (printit = 1) then print lhs$ + " = " + rhs$
8190 REM if (printit = 0) then print "# " + lhs$ + " = " + rhs$
8200 return

9000 REM Sum minor isotopes
9020 REM sum D
9030 nd = 0
9040 for ii = 1 to 4
9050     if instr(lig$(ii),"D") > 0 then nd = nd + 1
9060 next ii
9070 REM sum T
9080 nt = 0
9090 for ii = 1 to 4
9100     if instr(lig$(ii),"T") > 0 then nt = nt + 1
9110 next ii
9120 REM sum [18O]
9130 n18o = 0
9140 for ii = 1 to 4
9150     if instr(lig$(ii),"[18O]") > 0 then n18o = n18o + 1
9160 next ii
9170 REM sum [13C]
9180 n13C = 0
9190 for ii = 1 to 4
9200     if instr(lig$(ii),"[13C]") > 0 then n13C = n13C + 1
9210 next ii
9220 REM sum [14C]
9230 n14C = 0
9240 for ii = 1 to 4
9250     if instr(lig$(ii),"[14C]") > 0 then n14C = n14C + 1
9260 next ii
9270 REM sum [34S]
9280 n34s = 0
9290 for ii = 1 to 4
9300     if instr(lig$(ii),"[34S]") > 0 then n34s = n34s + 1
9310 next ii
9320 printit = 1
9330 if (nd > 2) then printit = 0
9340 if (nt > 1) then printit = 0
9350 if (n18o > 2) then printit = 0
9360 if (n13c > 2) then printit = 0
9370 if (n14c > 1) then printit = 0
9380 if (n34S > 2) then printit = 0
9390 if (printit = 1) then eqns = eqns + 1
9400 total_eqns = total_eqns + 1
9410 return


