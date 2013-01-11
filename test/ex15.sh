#! /bin/sh
# Test ex15

NAME=ex15
DB=ex15.dat

: ${PHREEQC=phreeqc}
${PHREEQC} $DATADIR/$NAME $NAME.out $DATADIR/$DB $NAME.log
result=$?

exit $result
