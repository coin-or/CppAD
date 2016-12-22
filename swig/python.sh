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
# create swig_example_python.cpp
echo_eval swig -python -c++ -I.. -o swig_example_python.cpp ../swig_example.i
#
# build module that is loadable by python
echo_eval g++ -I.. ../swig_example.cpp swig_example_python.cpp \
	-c \
	-fPIC \
	-I /usr/include/python2.7
echo_eval g++ -shared swig_example.o swig_example_python.o \
	-o _swig_example.so
#
# test the module
cd ..
if python check_swig_example.py
then
	echo 'All tests passed'
else
	echo 'At least one test failed'
fi
