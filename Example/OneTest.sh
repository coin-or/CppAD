# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#
# Run one of the tests
if [ "$1" = "" ]
then
	echo "OneTest test [extra]"
	echo "where test and is a *.cpp file name without extension"
	echo "and extra is extra options for g++ command"
	exit
fi
if [ -e TestOne.exe ]
then
	rm TestOne.exe
fi
sed < Example.cpp > TestOne.cpp \
	-e '/ok *\&= *Run( /d' \
	-e '/^	RunDouble/d' \
	-e '/^	RunADdouble/d' \
	-e "s/.*OneTest.*/	ok \&= Run( $1, \"$1\");/"  
#
echo "g++ TestOne.cpp $1.cpp $2  -o TestOne.exe  \\"
echo "	-g -Wall -ansi -pedantic-errors -std=c++98 \\"
echo "	-I.. -I/usr/include/boost-1_33"
g++ TestOne.cpp $1.cpp $2  -o TestOne.exe  \
	-g -Wall -ansi -pedantic-errors -std=c++98 \
	-I.. -I/usr/include/boost-1_33 
#
echo "./TestOne.exe"
./TestOne.exe
