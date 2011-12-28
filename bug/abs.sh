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
echo "Timing test of abs"
cat << EOF > abs_time.cpp
# include <cppad/cppad.hpp>

namespace { // empty namespace
	bool ok_ = true;

	void test_once(void)
	{	size_t n = 1000;
		size_t m = 1;
		CppAD::vector< CppAD::AD<double> > ax(n), ay(m);
		size_t j;
		for(j = 0; j < n; j++)
			ax[j] = 0.; 
		CppAD::Independent(ax);
		for(j = 0; j < n; j++)
			ay[0] += abs(ax[j]);
		CppAD::ADFun<double> f(ax, ay);
		CppAD::vector<double> x(n), dy(n);
		for(j = 0; j < n; j++)
			x[j] = double(n / 3 - j); 
		dy =f.Jacobian(x); 
		for(j = 0; j < n; j++)
		{	if( x[j] < 0. )
				ok_ &= dy[j] == -1.;
			else if( x[j] == 0. )
				ok_ &= dy[j] == 0.;
			else
				ok_ &= dy[j] == 1.;
		}
		return;
	}
	void test_repeat(size_t repeat)
	{	size_t i;
		for(i = 0; i < repeat; i++)
			test_once();
		return;
	}
} // end empty namespace

int main(void)
{
	// run the test case and set the time return value
	double test_time = 1.;
	double time_out = CppAD::time_test(test_repeat, test_time);

	size_t rate = size_t( 1. / time_out );
	std::cout << "repeats per second = " << rate << std::endl;

	// Correctness check
	assert( ok_ );

	return 0;
}
EOF
echo "g++ -I $HOME/cppad/trunk -O2 -DNDEBUG abs_time.cpp -o abs_time"
g++ -I $HOME/cppad/trunk -O2 -DNDEBUG abs_time.cpp -o abs_time
#
echo "rm abs_time.cpp"
rm abs_time.cpp
#
echo "./abs_time"
./abs_time
# ---------------------------------------------------------------------------
echo "Demonstrate bug in multi level ad with abs function"
cat << EOF > abs_bug.cpp
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
echo "g++ -I $HOME/cppad/trunk abs_bug.cpp -o abs"
g++ -I $HOME/cppad/trunk abs_bug.cpp -o abs
#
echo "rm abs_bug.cpp"
rm abs_bug.cpp
#
echo "./abs"
./abs
