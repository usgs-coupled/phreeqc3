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
echo (1) Need to mannually delete extra table of contents in phreeqc3.htm
echo 
echo (2) Need to fix ../contents.hhc.
echo     50651149_78104 replaced with 50581517_78104 for REACTION_TEMPERATURE_RAW
echo
echo     Easiest way is to global change from 50651149 to 50581517, for example
echo     There are probably only 8-10 global changes that need to be made.
echo
echo (3) Make sure examples 18 and 19 are correct. HTML from FrameMaker has the
echo     same entry for both (Madison).
