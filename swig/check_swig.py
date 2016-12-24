# $Id
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
# load the modules
import sys
import swig_cppad
#
# initialze exit status as OK
error_count = 0
# --------------------------------------------
# std::vector<double>
ok  = 1
vec = swig_cppad.vector_double(2)
# size
ok  = ok and vec.size() == 2
# resize
vec.resize(4)
ok  = ok and vec.size() == 4
# setting elements
for i in range( vec.size() ) :
	vec[i] = 2.0 * i
# getting elements
for i in range( vec.size() ) :
	ok = ok and vec[i] == 2.0 * i
if ok :
	print('swig_cppad.vector_double: OK')
else :
	print('swig_cppad.vector_double: Error')
	error_count = error_count + 1
# --------------------------------------------
# return error_count
sys.exit(error_count)
