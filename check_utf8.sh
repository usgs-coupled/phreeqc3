#!/bin/sh
EXIT_CODE=0
files=$(find . -type f -name "*.cpp" -o -name "*.cxx" -o -name "*.h" -o -name "Makefile.am" -o -name "CMakeLists.txt" -o -name "configure.ac" -o -wholename "*/database/*.dat")
for f in $files
do
    if ! iconv -f UTF-8 "$f" > /dev/null; then
	EXIT_CODE=1
	file "$f"
    fi
done
exit $EXIT_CODE
