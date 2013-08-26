#!/bin/bash
#                                change gif to png:
for i in *.htm;  
do
	echo $i
	sed "s/gif/png/g" $i > t
	mv t $i
done
echo                     Done png.
#                                change BASELINE to MIDDLE:
for i in *.htm;  
do
	echo $i
	sed "s/BASELINE/MIDDLE/g" $i > t
	mv t $i
done
echo                     Done MIDDLE.
#                                change fig.. to fig. :
for i in *.htm;  
do
	echo $i
	sed "s/ig\.\./ig\. /g" $i > t
	mv t $i
done
echo                     Done Fig.
#                                change figure. to figure 
for i in *.htm;  
do
	echo $i
	sed "s/igure\./igure /g" $i > t
	mv t $i
done
echo                     Done Figure.
#                                USEMAP
for i in *.htm;  
do
	echo $i
	sed 's/USEMAP/style="width:65%" USEMAP/g' $i > t
	mv t $i
done
echo                     Done USEMAP	

./UpdateContents.bash

#cd HTML; egrep '<H1 CLASS="FM1stOrderHeadingTOC">' -A1 -v phreeqc3.htm > temp3; mv temp3 phreeqc3.htm


echo "(1) Need to mannually delete extra table of contents in phreeqc3.htm"
echo
echo "(2) Make sure examples 18 and 19 are correct. HTML from FrameMaker has the"
echo     same entry for both (Madison).
echo
echo "(3) Compile HTML, double click on phreeqc3.hhp and hit compile button
echo
