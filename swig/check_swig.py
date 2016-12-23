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
# d_vector
ok  = 1
vec = swig_cppad.d_vector(2)
# size
ok  = ok and vec.size() == 2
# resize
vec.resize(4)
ok  = ok and vec.size() == 4
# data
for i in range(4) :
	a     = vec.data()
	swig_cppad.double_array_setitem(a, i, 3.0)
	value = swig_cppad.double_array_getitem(a, i)
	ok    = ok and value == 3.0
if ok :
	print('swig_cppad.d_vector: OK')
else :
	print('swig_cppad.d_vector: Error')
	error_count = error_count + 1
# --------------------------------------------
# return error_count
sys.exit(error_count)
