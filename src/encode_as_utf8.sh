#!/bin/sh
# see https://en.wikipedia.org/wiki/Windows-1252
iconv --to-code=UTF-8 --from-code=WINDOWS-1252 print.cpp > print.utf-8.cpp
if grep -q '^// -\*- coding: windows-1252 -\*-$' print.utf-8.cpp; then
  sed -i '1d' print.utf-8.cpp
fi
