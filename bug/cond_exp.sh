#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
cat << EOF
This example works for type zdouble, but not double.
We want it to work for type double so as to avoid the overhead of zdouble.

f(x) = 1 / x[0] if x[0] > 0 else 0.0
EOF
cat << EOF > bug.$$
#include <cppad/cppad.hpp>
namespace {
	template <class Float>
	bool test_float(void)
	{
		bool ok = true;
		using CppAD::vector;
		Float eps = Float( 10. * std::numeric_limits<double>::epsilon() );

		typedef CppAD::AD<Float>   a1float;
		typedef CppAD::AD<a1float> a2float;

		// --------------------------------------------------------------------
		// create a1f = f(x)
		vector<a2float> a2x(1), a2y(1);
		a2x[0] = a2float( 5.0 );
		Independent(a2x);

		a2float a2zero = a2float(0.0);
		a2float a2one  = a2float(1.0);
		a2y[0]  = CondExpGt(a2x[0], a2zero, a2one / a2x[0], a2zero);

		CppAD::ADFun<a1float> a1f;
		a1f.Dependent(a2x, a2y);
		// --------------------------------------------------------------------
		// create g = f'(x)
		vector<a1float> a1x(1), a1dy(1), a1w(1);
		a1x[0] = 2.0;
		a1w[0] = 1.0;
		Independent(a1x);

		a1f.Forward(0, a1x);
		a1dy   = a1f.Reverse(1, a1w);

		CppAD::ADFun<Float> g;
		g.Dependent(a1x, a1dy);
		// --------------------------------------------------------------------
		// check g where f(x) = 1 / x
		vector<Float> x(1), y(1);
		x[0] = 2.0;
		y    = g.Forward(0, x);
		ok  &= CppAD::NearEqual(y[0], - Float(1.0) / (x[0] * x[0]), eps, eps);
		// --------------------------------------------------------------------
		// check g at edge case where 1 / x is infinity
		x[0] = 0.0;
		y    = g.Forward(0, x);
		ok  &= CppAD::NearEqual(y[0], Float(0.0), eps, eps);
		// --------------------------------------------------------------------
		return ok;
	}
}
int main(void)
{	bool ok = true;
	if( test_float<CppAD::zdouble>() )
		std::cout << "zdouble: OK" << std::endl;
	else
	{	ok = false;
		std::cout << "zdouble: Error" << std::endl;
	}
	if( test_float<double>() )
		std::cout << "double: OK" << std::endl;
	else
	{	ok = false;
		std::cout << "double: Error" << std::endl;
	}
	if( ! ok )
		return 1;
	return 0;
}
EOF
# -----------------------------------------------------------------------------
if [ ! -e build ]
then
	mkdir build
fi
cd build
echo "$0"
name=`echo $0 | sed -e 's|.*/||' -e 's|\..*||'`
mv ../bug.$$ $name.cpp
echo "g++ -I../.. --std=c++11 -g $name.cpp -o $name"
g++ -I../.. --std=c++11 -g $name.cpp -o $name
#
echo "./$name"
if ! ./$name
then
	echo
	echo "$name.sh: Error"
	exit 1
fi
echo
echo "$name.sh: OK"
exit 0
