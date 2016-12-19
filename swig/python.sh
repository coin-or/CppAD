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
# create my_example_python.cpp
echo_eval swig -python -c++ -I.. -o my_example_python.cpp ../my_example.i
#
# build module that is loadable by python
echo_eval g++ -I.. ../my_example.cpp my_example_python.cpp \
	-c \
	-fPIC \
	-I /usr/include/python2.7
echo_eval g++ -shared my_example.o my_example_python.o \
	-o _my_example.so
# ---------------------------------------------------------------------------
# text module
cat << EOF > run.py
# load the modules
import sys
import my_example
#
# initialze exit status as OK
error_count = 0
# --------------------------------------------
if my_example.my_fact(4) == 24 :
	print("my_example.fact: OK")
else :
	print("my_example.my_fact: Error")
	error_count = error_count + 1
# --------------------------------------------
if my_example.my_mod(4,3) == 1 :
	print("my_example.my_mod: OK")
else :
	print("my_example.my_mod: Error")
	error_count = error_count + 1
# --------------------------------------------
if my_example.my_message() == "OK" :
	print("my_example.my_message: OK")
else :
	print("my_example.my_message: Error")
	error_count = error_count + 1
# --------------------------------------------
ptr = my_example.int_ctor()
my_example.my_add(3, 4, ptr)
if ptr.value() == 7 :
	print("my_example.my_add: OK")
else :
	print("my_example.my_add: Error")
	error_count = error_count + 1
# --------------------------------------------
n   = 10
vec = my_example.new_int_vector(n)
for i in range(n) :
	my_example.int_vector_setitem(vec, i, 2 * i)
#
if my_example.my_max(n, vec) == 18 :
	print("my_example.my_max: OK")
else :
	print("my_example.my_max: Error")
	error_count = error_count + 1
my_example.delete_int_vector(vec)
# --------------------------------------------
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
