
# This file can be automatically generaeted using the following command
# m4 ../python.m4 ../xam/vector_double_xam.m4 > vector_double_xam.py
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
# std::vector<double>
# -----------------------------------------------------------------------------
def vector_double_xam() :
	#
	# load the cppad swig library
	import py_cppad
	#
	# initialize return variable
	ok = True
	n = 4
	vec = py_cppad.vector_double(n)
	#
	# check size
	ok = ok and vec.size() == n
	#
	# setting elements
	for i in range( n  ) :
		vec[i] = 2.0 * i
	#
	# getting elements
	for i in range( n  ) :
		element = vec[i]
		ok = ok and element == 2.0 * i
	#
	return( ok )
#
