
# This file can be automatically generaeted using the following command
# m4 ../python.m4 ../xam/a_fun_xam.m4 > a_fun_xam.py
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
def a_fun_xam() :
	#
	# load the cppad swig library
	import py_cppad
	#
	# initialize return variable
	ok = True
	n = 2
	m = 1
	#
	# create ax
	x = py_cppad.vector_double(n)
	for i in range( n  ) :
		x[i] = i + 1.0
	ax = py_cppad.independent(x)
	#
	# create af
	ax0 = ax[0]
	ax1 = ax[1]
	ay = py_cppad.vector_ad(1)
	ay[0] = ax0 + ax0 - ax1
	af = py_cppad.a_fun(ax, ay)
	#
	# zero order forward
	x[0] = 3.0
	x[1] = 1.0
	y = af.forward(0, x)
	ok = ok and y[0] == 5.0
	#
	# first order forward
	x[0] = 0.0
	x[1] = 1.0
	y = af.forward(1, x)
	ok = ok and y[0] == -1.0
	#
	return( ok )
#
