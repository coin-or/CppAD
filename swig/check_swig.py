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
# --------------------------------------------
ok  = True
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
# a_double
# --------------------------------------------
ok         = True
two        = swig_cppad.a_double(2.0)
three      = swig_cppad.a_double(3.0)
five       = two + three
ok         = ok and 4.5 < five.value() and five.value() < 5.5
six        = two * three
ok         = ok and 5.5 < six.value() and six.value() < 6.5
neg_one    = two - three
ok         = ok and -1.5 < neg_one.value() and neg_one.value() < -0.5
two_thirds = two / three
ok         = ok and 0.5 < two_thirds.value() and two_thirds.value() < 1.0
if ok :
	print('swig_cppad.a_double: OK')
else :
	print('swig_cppad.a_double: Error')
	error_count = error_count + 1
# --------------------------------------------
# std::vector<a_double>
# --------------------------------------------
ok    = True
a_vec = swig_cppad.vector_ad(2)
# size
ok  = ok and a_vec.size() == 2
# resize
a_vec.resize(4)
ok  = ok and a_vec.size() == 4
# setting elements
for i in range( a_vec.size() ) :
	a_vec[i] = swig_cppad.a_double(2.0 * i)
# getting elements
for i in range( a_vec.size() ) :
	ok = ok and a_vec[i].value() == 2.0 * i
if ok :
	print('swig_cppad.vector_ad:  OK')
else :
	print('swig_cppad.vector_ad:  Error')
	error_count = error_count + 1
# --------------------------------------------
# afun(ax, ay)
ok = True
n  = 2
m  = 1
x  = swig_cppad.vector_double(n)
for i in range(n) :
	x[i] = i + 1
ax     = swig_cppad.independent(x)
ay     = swig_cppad.vector_ad(m)
ay[0]  = ax[0] - ax[1]
ay[0] += ax[0]
af     = swig_cppad.a_fun(ax, ay)
xp     = swig_cppad.vector_double(n)
xp[0]  = 3.0
xp[1]  = 1.0;
p      = 0
yp     = af.forward(p, xp)
ok     = ok and yp[0] == 5.0
xp[0]  = 0.0
xp[1]  = 1.0;
p      = 1
yp     = af.forward(p, xp)
ok     = ok and yp[0] == -1.0
if ok :
	print('swig_cppad.a_fun:  OK')
else :
	print('swig_cppad.a_fun:  Error')
	error_count = error_count + 1
# --------------------------------------------
# return error_count
sys.exit(error_count)
