#!/bin/sh
files=$(find . -type f -name "*.cpp" -o -name "*.cxx" -o -name "*.h" -o -name "Makefile.am" -o -name "CMakeLists.txt" -o -name "configure.ac" -o -wholename "*/database/*.dat")
for f in $files
do
    iconv -f UTF-8 "$f" > /dev/null || file "$f"
done
