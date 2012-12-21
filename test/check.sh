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
#  December, 2012, PHREEQC3

DIVD=----------------------------------------
TOPDIR=..
DATA=$TOPDIR/examples

# set list of names of files to be tested; name will be "test" by default
NameList=${@:-test}

# delete old output file  
if [ -f check.out ]; then rm check.out; fi

for Namevar in       ex1.out \
      ex2.out \
      ex2.sel \
      ex2b.out \
      ex3.out \
      ex4.out \
      ex5.out \
      ex5.sel \
      ex6A-B.sel \
      ex6C.sel \
      ex6.out \
      ex7.out \
      ex7.sel \
      ex8.out \
      ex8.sel \
      ex9.out \
      ex9.sel \
      ex10.out \
      ex10.sel \
      ex11adv.sel \
      ex11.out \
      ex11trn.sel \
      ex12a.out \
      ex12a.sel \
      ex12.out \
      ex12.sel \
      ex13ac.out \
      ex13a.out \
      ex13a.sel \
      ex13b.out \
      ex13b.sel \
      ex13c.out \
      ex13c.sel \
      ex14.out \
      ex14.sel \
      ex15.out \
      ex15.sel \
      ex16.out \
      ex17b.out \
      ex17.out \
      ex18.out \
      ex19b.out \
      ex19.out \
      ex20a.out \
      ex20b.out \
      ex21.out \
      ex22.out 
do
    if [ -f $DATA/$Namevar -a -f $Namevar ]
    then
      # do comparison only if both orig. and new output files exist
        echo -e $DIVD$DIVD | tee -a check.out
        echo -e "comparison of $DATA/$Namevar with $Namevar" \
            | tee -a check.out

        if diff -w $DATA/$Namevar $Namevar >> check.out
        then
            echo FILES ARE IDENTICAL | tee -a check.out
        else
            echo FILES DIFFER:  see file check.out for differences
        fi
    fi
done

