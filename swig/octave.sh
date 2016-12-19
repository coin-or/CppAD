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
#! /bin/bash -e
# ---------------------------------------------------------------------------
# create new build directory
if [ ! -e build ]
then
	echo_eval mkdir build
fi
echo_eval cd build
#
# create my_example_octave.cpp
echo_eval swig -octave -I.. -c++ -o my_example_octave.cpp ../my_example.i
echo 'fix warnings about unitialized variables in my_example_octave.cpp'
sed \
	-e '/^ *int *val1 *;/s/;/= 0 ;/' \
	-e '/^ *int *val2 *;/s/;/= 0 ;/' \
	-e '/^ *int *val3 *;/s/;/= 0 ;/' \
	-i my_example_octave.cpp
#
# build module that is loadable by octave
echo_eval mkoctfile -I.. ../my_example.cpp my_example_octave.cpp \
	-o my_example.oct
#
cd ..
if octave -q test_my_example.m
then
	echo 'All tests passed'
else
	echo 'At least one test failed'
fi
