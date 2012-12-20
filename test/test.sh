#!/bin/sh
# usr/opt/wrdapp/phreeqc1.1/test/test.sh -- run phreeqe test data sets
#
# Usage: test.sh [start [stop]]
#        test.sh [start [stop] | tee test.out
#
#        where: start = starting test number
#                stop = ending test number (may be same as start)
#
# To use this script for another program in a different directory, globally
# change the program name (be sure to change both upper and lower case
# occurrences) and set the default value for the Stop variable as appropriate.
#
# History: 12/24/91, mblalock, initial coding
#          04/27/92, mygoze, restructuring
#          11/07/95, rsregan, modified for use with phreeqc
#          02/10/00, mcrouse, last modified
#          December 2012, PHREEQC3
#
# Variable definitions
# --------------------
#
  PROGNM=phreeqc
  TOPDIR=..
  PROG=$TOPDIR/bin/$PROGNM
  DATA=$TOPDIR/examples
  DB=$TOPDIR/database
  CHECK=$TOPDIR/test/check.sh
  CLEAN=$TOPDIR/test/clean.sh
  END=22
  DIVD========================================

#
  exec 2>&1                                # stderr shows up in .out file

  Start=${1:-1}                            # by default, start at 1
  Stop=${2:-$END}                          # by default, stop at $END
  if [ $Start -lt 1 ] ; then Start=1 ; fi
  if [ $Stop -lt 1 ] ; then Stop=$END ; fi
  if [ $Start -gt $END -o $Stop -gt $END ]; then
    echo "\nWarning, invalid arguments--test range is 1 - $END for $PROGNM"
    echo "input arguments were $Start - $Stop\n"
    if [ $Stop -gt $END ] ; then Stop=$END ; fi
    if [ $Start -gt $END ]; then
      echo "no $PROGNM tests will be performed"
    else
      echo "Tests $Start - $Stop will be performed"
    fi
    echo
  fi

#
# remove old output file
#
  $CLEAN

#
# begin test runs
#
  Test=$Start

  if [ $Test -ge $Start -a $Test -le $Stop ]; then
    echo "\n\n"$DIVD$DIVD
    echo "Begin processing $PROGNM test runs $Start to $Stop\n"
    date
  fi

  while [ $Test -ge $Start -a $Test -le $Stop ]; do
 
     echo "\n\n"$DIVD$DIVD
     echo "Test run number $Test"
    
     if [ $Test -eq 1 ]; then
	Namevar=ex$Test
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat
	mv phreeqc.log $Namevar$.log
	
     elif [ $Test -eq 2 ]; then
	Namevar=ex$Test
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat
	mv phreeqc.log $Namevar$.log    
	
	Namevar=ex2b
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat
	mv phreeqc.log $Namevar$.log  
     fi
     
     

#    Namevar=ex$Test
#    infile=$DATA/$Namevar
#    outfile=$Namevar.out
#    if [ $Test -eq 13 ]; then
#       for subtest in a b c
#       do
#          infile=$DATA/$Namevar$subtest
#          outfile=$Namevar$subtest.out
#          $PROG $infile $outfile $DB/phreeqc.dat
#	  mv phreeqc.log $Namevar$subtest.log
#       done
#    elif [ $Test -eq 12 ]; then
#       $PROG $infile $outfile $DB/phreeqc.dat
#	  mv phreeqc.log $Namevar.log
#       subtest=a
#       infile=$infile$subtest
#       outfile=$Namevar$subtest.out
#       $PROG $infile $outfile $DB/phreeqc.dat
#       mv phreeqc.log $Namevar$subtest.log
#    elif [ $Test -eq 14 ]; then
#       $PROG $infile $outfile $DB/wateq4f.dat
#       mv phreeqc.log $Namevar.log
#    elif [ $Test -eq 15 ]; then
#       $PROG $infile $outfile $DATA/ex15.dat
#       mv phreeqc.log $Namevar.log
#    else
#       $PROG $infile $outfile $DB/phreeqc.dat
#       mv phreeqc.log $Namevar.log
#    fi
    Test=`expr $Test + 1`
  done
  Test=`expr $Test - 1`
#
  if [ $Test -ge $Start -a $Test -le $Stop ]; then
    echo "\n\n"$DIVD$DIVD
    echo "Completed $PROGNM test runs $Start to $Test\n"
  fi

# check output against original output in $TOPDIR/data directory
#  $CHECK ex
