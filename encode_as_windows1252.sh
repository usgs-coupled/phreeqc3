#!/bin/sh
# see https://en.wikipedia.org/wiki/Windows-1252
# and https://en.wikipedia.org/wiki/ISO/IEC_8859
iconv --to-code=WINDOWS-1252 --from-code=UTF-8 print.cpp > print.windows-1252.cpp
grep -q '^// -\*- coding: windows-1252 -\*-$' print.windows-1252.cpp \
  || echo '// -*- coding: windows-1252 -*-' | cat - print.windows-1252.cpp > print.tmp.cpp \
  && mv print.tmp.cpp print.windows-1252.cpp