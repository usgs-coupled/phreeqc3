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
echo NEED TO MANUALLY DELETE EXTRA TABLE OF CONTENTS IN phreeqc3.htm
#
#  !!!!! NEED TO MANUALLY DELETE EXTRA TABLE OF CONTENTS IN phreeqc3.htm
#
