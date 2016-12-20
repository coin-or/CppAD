#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# create new build directory
if [ ! -e build ]
then
	echo_eval mkdir build
fi
echo_eval cd build
#
# create example_octave.cpp
echo_eval swig -octave -I.. -c++ -o example_octave.cpp ../example.i
echo 'fix warnings about unitialized variables in example_octave.cpp'
sed \
	-e '/^ *int *val1 *;/s/;/= 0 ;/' \
	-e '/^ *int *val2 *;/s/;/= 0 ;/' \
	-e '/^ *int *val3 *;/s/;/= 0 ;/' \
	-i example_octave.cpp
#
# build module that is loadable by octave
echo_eval mkoctfile -I.. ../example.cpp example_octave.cpp \
	-o example.oct
#
cd ..
if octave -q test_example.m
then
	echo 'All tests passed'
else
	echo 'At least one test failed'
fi
