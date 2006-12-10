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
	echo "usage: test_one.sh file function [extra]"
	echo "file is the *.cpp file name with extension"
	echo "function is the name of the function being tested"
	echo "and extra is extra options for g++ command"
	exit 1
fi
if [ -e test_one.exe ]
then
	rm test_one.exe
fi
sed < example.cpp > test_one.cpp \
-e '/ok *\&= *Run( /d' \
-e "s/.*This line is used by test_one.sh.*/	ok \&= Run( $2, \"$2\");/"  
#
echo "g++ test_one.cpp $1 $3  -o test_one.exe  \\"
echo "  -g -Wall -ansi -pedantic-errors -std=c++98 \\"
echo "  -I.. -I/usr/include/boost-1_33"
g++ test_one.cpp $1 $3  -o test_one.exe  \
        -g -Wall -ansi -pedantic-errors -std=c++98 \
        -I.. -I/usr/include/boost-1_33 
#
echo "./test_one.exe"
if ! ./test_one.exe
then
	exit 1
fi
exit 0

