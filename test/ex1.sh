#! /bin/sh
# Test ex1

NAME=ex1
DB=phreeqc.dat

: ${PHREEQC=phreeqc}
${PHREEQC} $DATADIR/$NAME $NAME.out $DBDIR/$DB $NAME.log
result=$?

exit $result
