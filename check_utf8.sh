#!/bin/sh
EXIT_CODE=0
srcs=$(find src -type f -name "*.cpp" -o -name "*.cxx")
for f in $srcs
do
    if ! g++ -DNO_UTF8_ENCODING -E -I src -I src/common -I src/PhreeqcKeywords "$f" | iconv -f ASCII -t ASCII > /dev/null; then
        EXIT_CODE=1
        file "$f"
    fi
done
dbs=$(find . -type f -name "Makefile.am" -o -name "CMakeLists.txt" -o -name "configure.ac" -o -wholename "*/database/*.dat")
for f in $dbs
do
    if ! iconv -f UTF-8 "$f" > /dev/null; then
        EXIT_CODE=1
        file "$f"
    fi
done
exit $EXIT_CODE
