# ! /bin/bash
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#
echo "Checking difference between OMhelp include directives and file names."
echo "----------------------------------------------------------------------"
if [ -e junk.1 ]
then
	rm junk.1
fi
list="
	cpl1.0.txt
	doc.omh
	dev.omh
	cppad/PowInt.h
	cppad/*.hpp
	cppad/local/*.hpp
	cppad/speed/*.hpp
	example/*.cpp
	example/*.hpp
	introduction/get_started/*.cpp
	introduction/exp_apx/*.cpp
	introduction/exp_apx/*.hpp
	introduction/exp_apx/*.omh
	openmp/*.hpp
	openmp/*.cpp
	openmp/*.sh
	print_for/*.cpp
	speed_cppad/*.cpp
	speed/*.cpp
	speed/adolc/*.cpp
	speed/cppad/*.cpp
	speed/example/*.cpp
	speed/fadbad/*.cpp
	omh/*.omh
"
for file in $list
do
	# assume $childtable, ... , $verbatim use % for delimiter
	sed -n < $file >> junk.1 \
		-e 's/^#[ \t][ \t]*//' \
		-e '/$childtable%/,/%$\$/p' \
		-e '/$children%/,/%$\$/p' \
		-e '/$contents%/,/%$\$/p' \
		-e '/$verbatim%/p' 
done
sed < junk.1 > junk.2 \
	-e 's/$childtable//' \
	-e 's/$children//' \
	-e 's/$contents//' \
	-e 's/$verbatim%\([^%]*\)%.*/\1/' \
	-e 's/%//' \
	-e 's/$\$//' \
	-e '/^ *$/d' \
	-e 's/^[ 	]*//' \
	-e 's|\\|/|g'
#
ls $list > junk.3
#
for file in `cat junk.2`
do
	if [ "`grep $file junk.3`" == "" ]
	then
		echo $file
	fi
done
echo "----------------------------------------------------------------------"
echo "Nothing should be between the two dashed lines above"
