#!/bin/sh
#
#  check.sh:  differential file comparator (diff) run to compare original
#             output files against newly created output files
#
# output file names must follow format "nameX.sufx" where:  "name" is
# "test" by default or can be supplied as argument(s) to check.sh; all
# numbers X must appear in "for Test" loop list below; all suffixes must 
# appear in "for Sufx" loop list below
#
#  04/28/92, mygoze, initial coding
#  06/10/92, mygoze, added in NameList, Name, Sufx variables and "for Name",
#                    "for Sufx" loops to make check.sh more versatile 
#  02/10/00, mcrouse, last modified

DIVD=----------------------------------------
TOPDIR=..
DATA=$TOPDIR/examples

# set list of names of files to be tested; name will be "test" by default
NameList=${@:-test}

# delete old output file  
if [ -f check.out ]; then rm check.out; fi

for Namevar in $NameList
do
  for Test in 1 2 3 4 5 6 6A-B 6C 7 8 9 10 11 11adv 11trn 12 12a \
	      13a 13b 13c 14 15 16 17 18
  do
    for Sufx in out sel
    do
      if [ -f $DATA/$Namevar$Test.$Sufx -a -f $Namevar$Test.$Sufx ]
      then
      # do comparison only if both orig. and new output files exist
        echo $DIVD$DIVD | tee -a check.out
        echo "comparison of $DATA/$Namevar$Test.$Sufx with $Namevar$Test.$Sufx" \
             | tee -a check.out

        if diff -w $DATA/$Namevar$Test.$Sufx $Namevar$Test.$Sufx >> check.out
        then
          echo FILES ARE IDENTICAL | tee -a check.out
        else
          echo FILES DIFFER:  see file check.out for differences
        fi
      fi
    done
  done
done
