10 n1 = 6
20 n2 = 2
30 dim a$(n1)
40 data "HS","DS","TS","H[34S]", "D[34S]", "T[34S]"
50 restore 40
60 for i = 1 to n1
70 read a$(i)
80 next i
200 for i = 1 to 6
220 for j = i to 6
230 if (i = j) then gosub 1000 
240 if not (i = j) then gosub 2000 
250 next j
260 next i
270 end
1000 REM equation for same
1010 print "Fe(HS)2 + 2" + A$(i) + "- = Fe(" + A$(i) + ")2" + " + 2HS-" 
1020 return
2000 REM equation for different
2010 print "Fe(HS)2 + " + A$(i) + "- + " + A$(j) "- = Fe" + A$(i) + A$(j) + " + 2HS-" 
2020 return
