#!/bin/bash
#xxxIntroductionxxx
#	phreeqc3-2.htm
#	Introduction</H2>
#<A NAME="50515975_pgfId-250038"></A><A NAME="50515975_39141"></A>Introduction</H2>
export INTRO=`grep 'Introduction</H2>' phreeqc3-2.htm | sed "s/^.*\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\_[0-9][0-9][0-9][0-9][0-9]\)/\1/" | sed "s/\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\).*/\1/"`
#echo $INTRO

#xxxDescriptionxxx
#	phreeqc3-5.htm
#	Description of Data Input</H2>
#<A NAME="50515969_pgfId-703777"></A><A NAME="50515969_81160"></A>Description of Data Input</H2>
export DESC=`grep 'Description of Data Input</H2>' phreeqc3-5.htm | sed "s/^.*\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\_[0-9][0-9][0-9][0-9][0-9]\)/\1/" | sed "s/\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\).*/\1/"`
#echo $DESC
	
#xxxBasicxxx
#	phreeqc3-54.htm
#	The Basic Interpreter</H2>
#<A NAME="50515973_pgfId-470977"></A><A NAME="50515973_44206"></A>The Basic Interpreter</H2>
export BAS=`grep 'The Basic Interpreter</H2>' phreeqc3-54.htm | sed "s/^.*\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\_[0-9][0-9][0-9][0-9][0-9]\)/\1/" | sed "s/\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\).*/\1/"`
#echo $BAS

	
#xxxExamplesxxx
#	phreeqc3-55.htm
#	Examples</H2>
export EXAM=`grep 'Examples</H2>' phreeqc3-55.htm | sed "s/^.*\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\_[0-9][0-9][0-9][0-9][0-9]\)/\1/" | sed "s/\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\).*/\1/"`
#echo $EXAM


#xxxReferencesxxx
#	phreeqc3-78.htm
#	References Cited</H2>
export REF=`grep 'References Cited</H2>' phreeqc3-78.htm | sed "s/^.*\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\_[0-9][0-9][0-9][0-9][0-9]\)/\1/" | sed "s/\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\).*/\1/"`
#echo $REF

#xxxProgrammersxxx
#	phreeqc3-79.htm
#	Keyword Data Blocks for Programmers</H2>
export PROG=`grep 'Keyword Data Blocks for Programmers</H2>' phreeqc3-79.htm | sed "s/^.*\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\_[0-9][0-9][0-9][0-9][0-9]\)/\1/" | sed "s/\([0-9][0-9][0-9][0-9][0-9][0-9][0-9][0-9]\).*/\1/"`
#echo $PROG

sed -e "s/xxxIntroductionxxx/$INTRO/g" -e "s/xxxDescriptionxxx/$DESC/g" -e "s/xxxBasicxxx/$BAS/g" -e "s/xxxExamplesxxx/$EXAM/g" -e "s/xxxReferencesxxx/$REF/g" -e "s/xxxProgrammersxxx/$PROG/g" ../contents.template.hhc > ../contents.hhc
