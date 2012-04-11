ptf @
SOLUTION 1
SELECTED_OUTPUT
  -file ex19_p.prn
  -reset false
  -high_precision true
USER_PUNCH
  -headings solute sorbed
  10 DATA 0, 3.1, 6.1, 5.9, 7.1, 8.1, 9.9, 12.3, 13,\
  13.6, 16, 19.1, 24.1, 25.8, 27.6, 33.2, 36.4
  20 DIM c(17), s(17)
  30 FOR i = 1 to 17
  40 READ c[i]
  50 NEXT i
  60 DATA 0, 0.86, 1.12, 1.71, 1.68, 2.03, 2.46, 2.85, 3.36,\
  3.22, 3.25, 3.56, 3.76, 4.17, 4.58, 4.82, 5.19
  70 FOR i = 1 to 17
  80 READ s[i]
  90 NEXT i
  100 FOR i = 1 to 17
  110 s1 = @K            @ * c[i]^@n            @
#  120 PUNCH c[i], ((s[i]) - (s1))^2, CHR$(13)
  120 PUNCH c[i], LOG(s1) - LOG(s[i]), CHR$(13)
  130 NEXT i
END  
