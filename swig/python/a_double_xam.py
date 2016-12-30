
# This file can be automatically generaeted using the following command
# m4 ../python.m4 ../xam/a_double_xam.m4 > a_double_xam.py
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
# a_double
# -----------------------------------------------------------------------------
def a_double_xam() :
	#
	# load the cppad swig library
	import py_cppad
	#
	# initialize return variable
	ok = True
	two = py_cppad.a_double(2.0)
	three = py_cppad.a_double(3.0)
	#
	five = two + three
	six = two * three
	neg_one = two - three
	two_thirds = two / three
	#
	ok = ok and five.value() == 5.0
	ok = ok and six.value() == 6.0
	ok = ok and neg_one.value() == -1.0
	ok = ok and neg_one.value() == -1.0
	ok = ok and 0.5 < two_thirds.value()
	ok = ok and two_thirds.value() < 1.0
	ok = ok and five < six
	#
	return( ok )
#
