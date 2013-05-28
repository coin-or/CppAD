#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Run one of the tests
if [ "$1" = "" ]
then
	echo "usage: test_one.sh file [extra]"
	echo "file is the *.cpp file name with extension"
	echo "and extra is extra options for g++ command"
	exit 1
fi
if [ ! -e "$1" ]
then
	echo "Cannot find the file $1"
	exit 1
fi
# determine the function name
fun=`grep "^bool *[a-zA-Z0-9_]*( *void *)" $1 | tail -1 | \
	sed -e "s/^bool *\([a-zA-Z0-9_]*\) *( *void *)/\1/"`
#
if [ -e test_one.exe ]
then
	rm test_one.exe
fi
if [ -e test_one.cpp ]
then
	rm test_one.cpp
fi
sed < atomic.cpp > test_one.cpp \
-e '/ok *\&= *Run( /d' \
-e "s/.*This line is used by test_one.sh.*/	ok \&= Run( $fun, \"$fun\");/"  
#
if echo "$fun" | grep 'eigen' > /dev/null
then
	cxxflags='-g -Wall -ansi -pedantic-errors -Wno-long-long'
else
	cxxflags='-g -Wall -ansi -pedantic-errors -Wshadow'
fi
cmd="g++ test_one.cpp $*
	-o test_one.exe
	$cxxflags
	-g
	-lboost_thread-mt
	-std=c++11 
	-I../.. 
"
echo $cmd
$cmd
#
echo "./test_one.exe"
if ! ./test_one.exe
then
	exit 1
fi
exit 0
