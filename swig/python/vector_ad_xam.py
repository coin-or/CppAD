
# This file can be automatically generaeted using the following command
# m4 ../python.m4 ../xam/vector_ad_xam.m4 > vector_ad_xam.py
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
# std::vector<a_double>
# -----------------------------------------------------------------------------
def vector_ad_xam() :
	#
	# load the cppad swig library
	import py_cppad
	#
	# initialize return variable
	ok = True
	n = 4
	a_vec = py_cppad.vector_ad(n)
	#
	# check size
	ok = ok and a_vec.size() == n
	#
	# setting elements
	for i in range( n  ) :
		ad = py_cppad.a_double(2.0 * i)
		a_vec[i] = ad
	#
	# getting elements
	for i in range( n  ) :
		a_element = a_vec[i]
		ok = ok and a_element.value() == 2.0 * i
	#
	return( ok )
#
