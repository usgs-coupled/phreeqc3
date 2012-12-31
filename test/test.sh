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
  PROGNM=PHREEQC
  TOPDIR=..
#  PROG=$TOPDIR/bin/phreeqcpp
  PROG=../src/Class_release_64/phreeqcpp
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
    echo 
    echo "Warning, invalid arguments--test range is 1 - $END for $PROGNM"
    echo "input arguments were $Start - $Stop"
    echo
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
    echo
    echo $DIVD$DIVD
    echo "Begin processing $PROGNM test runs $Start to $Stop"
    echo
    date
  fi

  while [ $Test -ge $Start -a $Test -le $Stop ]; do
 
     echo $DIVD$DIVD
     echo "Test run number $Test"
 
 # Example 1
     if [ $Test -eq 1 ]; then
	Namevar=ex1
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log

 # Example 2	
     elif [ $Test -eq 2 ]; then
	Namevar=ex2
	rm -f $Namevar.* $Namevar.tsv
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
	
	Namevar=ex2b
	rm -f %Namevar%.*
	cp $DATA/$Namevar.tsv .
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
	
 # Example 3
      elif [ $Test -eq 3 ]; then
	Namevar=ex3
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
	
 # Example 4
      elif [ $Test -eq 4 ]; then
	Namevar=ex4
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
	
# Example 5
      elif [ $Test -eq 5 ]; then
	Namevar=ex5
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
	
# Example 6
      elif [ $Test -eq 6 ]; then
	Namevar=ex6
	rm -f $Namevar.* $Namevar*sel
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
	
# Example 7
      elif [ $Test -eq 7 ]; then
	Namevar=ex7
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
	
# Example 8
      elif [ $Test -eq 8 ]; then
	Namevar=ex8
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
	
# Example 9
      elif [ $Test -eq 9 ]; then
	Namevar=ex9
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
	
# Example 10
      elif [ $Test -eq 10 ]; then
	Namevar=ex10
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
		
# Example 11
      elif [ $Test -eq 11 ]; then
	Namevar=ex11
	rm -f $Namevar.* $Namevar*sel
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
		
# Example 12
      elif [ $Test -eq 12 ]; then
	Namevar=ex12
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
	
	Namevar=ex12a
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
		
# Example 13
      elif [ $Test -eq 13 ]; then
	Namevar=ex13a
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
	
	Namevar=ex13b
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
	
	Namevar=ex13c
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
	
	Namevar=ex13ac
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
 	
# Example 14
      elif [ $Test -eq 14 ]; then
	Namevar=ex14
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
	
	
# Example 15
      elif [ $Test -eq 15 ]; then
	Namevar=ex15
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DATA/ex15.dat $Namevar.log
	
	Namevar=ex15a
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DATA/ex15.dat $Namevar.log
	
	Namevar=ex15b
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DATA/ex15.dat $Namevar.log
	
# Example 16
      elif [ $Test -eq 16 ]; then
	Namevar=ex16
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
		

# Example 17
      elif [ $Test -eq 17 ]; then
	Namevar=ex17
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/pitzer.dat $Namevar.log
	
	Namevar=ex17b
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/pitzer.dat $Namevar.log

# Example 18
      elif [ $Test -eq 18 ]; then
	Namevar=ex18
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
	
# Example 19
      elif [ $Test -eq 19 ]; then
	Namevar=ex19
	rm -f $Namevar.* ex19_meas.tsv
	cp $DATA/ex19_meas.tsv .
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log
	
	Namevar=ex19b
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log

# Example 20
      elif [ $Test -eq 20 ]; then
	Namevar=ex20a
	rm -f $Namevar.* ex20*tsv
	cp $DATA/ex20*.tsv .
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/iso.dat $Namevar.log
	
	Namevar=ex20b
	rm -f $Namevar.*
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/iso.dat $Namevar.log


# Example 21
      elif [ $Test -eq 21 ]; then
	Namevar=ex21
	rm -f $Namevar.* ex21*tsv
	cp $DATA/ex21*.tsv .
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log

# Example 22
      elif [ $Test -eq 22 ]; then
	Namevar=ex22
	rm -f $Namevar.* co2.tsv
	cp $DATA/co2.tsv .
	infile=$DATA/$Namevar
	outfile=$Namevar.out   
	$PROG $infile $outfile $DB/phreeqc.dat $Namevar.log

     fi
     
    Test=`expr $Test + 1`
  done
  Test=`expr $Test - 1`
#
  if [ $Test -ge $Start -a $Test -le $Stop ]; then
    echo
    echo $DIVD$DIVD
    echo "Completed $PROGNM test runs $Start to $Test"
    echo
  fi

# check output against original output in $TOPDIR/data directory
$CHECK 
