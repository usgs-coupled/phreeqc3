#!/bin/bash
# change gif to png:
for i in *.htm;  
do
	echo $i
	sed "s/gif/png/g" $i > t
	mv t $i
done
echo                     Done png.

# change BASELINE to MIDDLE:
for i in *.htm;  
do
	echo $i
	sed "s/BASELINE/MIDDLE/g" $i > t
	mv t $i
done
echo                     Done MIDDLE.

# change fig.. to fig. :
for i in *.htm;  
do
	echo $i
	sed "s/ig\.\./ig\. /g" $i > t
	mv t $i
done
echo                     Done Fig.

# change figure. to figure 
for i in *.htm;  
do
	echo $i
	sed "s/igure\./igure /g" $i > t
	mv t $i
done
echo                     Done Figure.

# USEMAP
for i in *.htm;  
do
	echo $i
	sed 's/png" USEMAP/png" style="width:65%" USEMAP/g' $i > t
	mv t $i
done
echo                     Done USEMAP	

bash ./UpdateContents.bash
echo                     Done UpdateContents

# Fix example 19 in phreeqc3.htm
export HEAD19='CLASS="XRef">Modeling Cd+2 Sorption With Linear, Freundlich, and Langmuir Isotherms, and With a Deterministic Distribution of Sorption Sites for Organic Matter, Clay Minerals, and Iron Oxyhydroxides</A></A></A></P>'
export NAME19=`grep 'Example 19--<A NAME=' phreeqc3-74.htm | sed "s/^.*\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\_[0-9][0-9][0-9][0-9][0-9]\)\(.*\)/\1/" `
export REST='"phreeqc3-74.htm#'$NAME19'" '"$HEAD19"
sed "s?\(^.*phreeqc3-74.*19. <A HREF=\)\(.*\)?\1$REST?" phreeqc3.htm > out; mv out phreeqc3.htm
echo                     Done Example 19

#cd HTML; egrep '<H1 CLASS="FM1stOrderHeadingTOC">' -A1 -v phreeqc3.htm > temp3; mv temp3 phreeqc3.htm
egrep '<H1' -v phreeqc3.htm | egrep '/H1>' -v > temp3; mv temp3 phreeqc3.htm

#echo
#echo "(1) Need to manually delete extra table of contents in phreeqc3.htm"
echo
echo "(1) Compile HTML, double click on phreeqc3.hhp and hit compile button"

