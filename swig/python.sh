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
if [ -e build ]
then
	echo_eval rm -r build
fi
echo_eval mkdir build
echo_eval cp my_example.* build
echo_eval cd build
# create my_example_wrap.cpp
echo_eval swig -python -c++ my_example.i
echo_eval mv my_example_wrap.cxx my_example_wrap.cpp
# build module that is loadable by python
echo_eval g++ my_example.cpp my_example_wrap.cpp \
	-c \
	-fPIC \
	-I /usr/include/python2.7
echo_eval g++ -shared my_example.o my_example_wrap.o \
	-o _my_example.so
# ---------------------------------------------------------------------------
# text module
cat << EOF > run.py
# load the modules
import sys
import my_example
# initialze exit status as OK
error_count = 0;
#
if my_example.my_fact(4) == 24 :
	print("my_example.fact: OK")
else :
	print("my_example.my_fact: Error")
	error_count = error_count + 1;
#
if my_example.my_mod(4,3) == 1 :
	print("my_example.my_mod: OK")
else :
	print("my_example.my_mod: Error")
	error_count = error_count + 1;
#
if my_example.my_message() == "OK" :
	print("my_example.my_message: OK")
else :
	print("my_example.my_message: Error")
	error_count = error_count + 1;
#
# return error_count
sys.exit(error_count)
EOF
# ---------------------------------------------------------------------------
if python run.py
then
	echo 'All tests passed'
else
	echo 'At least one test failed'
fi
