#! /bin/bash -e
# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#
echo "Demonstrate a bug in multi level ad with abs function"
cat << EOF > abs.cpp
# include <cppad/cppad.hpp>
int main(void)
{	
	using CppAD::vector;
	using CppAD::AD;

	// 100 times machine epsilon
	double eps = 100. * CppAD::epsilon<double>();

	typedef AD<double>      ADdouble; // for first level of taping
	typedef AD<ADdouble>   ADDdouble; // for second level of taping
	size_t n = 1;                     // number independent variables
	size_t m = 1;                     // number dependent variables

	vector<double>       x(n),   y(m);
	vector<ADdouble>    ax(n),  ay(m);
	vector<ADDdouble>  aax(n), aay(m);

	// create af(x) = abs(x)
	aax[0] = 1.;
	CppAD::Independent( aax );
	aay[0] = abs(aax[0]);
	CppAD::ADFun<ADdouble> af(aax, aay);

	// create g(x) = af'(x) with evaluation at x = 1.
	ax[0] = 1.;
	CppAD::Independent( ax );
	ay = af.Jacobian(ax);
	CppAD::ADFun<double> g(ax, ay);

	// evaluate derivative of abs function at x = 1
	x[0] = 1.; 
	y    = g.Forward(0, x);
	std::cout << "d/dx abs(x) at x = +1 is " << y[0] << std::endl;
	assert( CppAD::NearEqual(y[0], 1., eps, eps) );

	// evaluate derivative of abs function at x = -1
	x[0] = -1.;  // at x == -1
	y    = g.Forward(0, x);
	std::cout << "d/dx abs(x) at x = -1 is " << y[0] << std::endl;
	assert( CppAD::NearEqual(y[0], -1., eps, eps) );

	return 0;
}
EOF
#
echo "g++ -I $HOME/cppad/trunk abs.cpp -o abs"
g++ -I $HOME/cppad/trunk abs.cpp -o abs
#
echo "rm abs.cpp"
rm abs.cpp
#
echo "./abs"
./abs
