#! /bin/sh
# Test ex5

NAME=ex5
DB=phreeqc.dat

: ${PHREEQC=phreeqc}
${PHREEQC} $DATADIR/$NAME $NAME.out $DBDIR/$DB $NAME.log
result=$?

exit $result
