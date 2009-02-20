/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin runge_45_1.cpp$$
$spell
	Runge
$$

$section Runge45: Example and Test$$

$index Runge45, example$$
$index example, Runge45$$
$index test, Runge45$$

Define 
$latex X : \R \rightarrow \R^n$$ by
$latex \[
	X_i (t) =  t^{i+1}
\] $$ 
for $latex i = 1 , \ldots , n-1$$.
It follows that
$latex \[
\begin{array}{rclr}
X_i(0)       & = & 0                           & {\rm for \; all \;} i \\
X_i ' (t)  & = & 1                             & {\rm if \;} i = 0      \\
X_i '(t)   & = & (i+1) t^i = (i+1) X_{i-1} (t) & {\rm if \;} i > 0
\end{array}
\] $$
The example tests Runge45 using the relations above:

$code
$verbatim%example/runge_45_1.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cstddef>                 // for size_t
# include <cppad/runge_45.hpp>      // for CppAD::Runge45
# include <cppad/near_equal.hpp>    // for CppAD::NearEqual
# include <cppad/vector.hpp>        // for CppAD::vector

namespace {
	class Fun {
	public:
		// constructor
		Fun(bool use_x_) : use_x(use_x_) 
		{ }

		// set f = x'(t)
		void Ode(
			const double                &t, 
			const CppAD::vector<double> &x, 
			CppAD::vector<double>       &f)
		{	size_t n  = x.size();	
			double ti = 1.;
			f[0]      = 1.;
			size_t i;
			for(i = 1; i < n; i++)
			{	ti *= t;
				if( use_x )
					f[i] = (i+1) * x[i-1];
				else	f[i] = (i+1) * ti;
			}
		}
	private:
		const bool use_x;

	};
}

bool runge_45_1(void)
{	bool ok = true;     // initial return value
	size_t i;           // temporary indices

	size_t  n = 5;      // number components in X(t) and order of method
	size_t  M = 2;      // number of Runge45 steps in [ti, tf]
	double ti = 0.;     // initial time
	double tf = 2.;     // final time 

	// xi = X(0)
	CppAD::vector<double> xi(n); 
	for(i = 0; i <n; i++)
		xi[i] = 0.;

	size_t use_x;
	for( use_x = 0; use_x < 2; use_x++)
	{	// function object depends on value of use_x
		Fun F(use_x > 0); 

		// compute Runge45 approximation for X(tf)
		CppAD::vector<double> xf(n), e(n); 
		xf = CppAD::Runge45(F, M, ti, tf, xi, e);

		double check = tf;
		for(i = 0; i < n; i++)
		{	// check that error is always positive
			ok    &= (e[i] >= 0.);
			// 5th order method is exact for i < 5
			if( i < 5 ) ok &=
				CppAD::NearEqual(xf[i], check, 1e-10, 1e-10);
			// 4th order method is exact for i < 4
			if( i < 4 )
				ok &= (e[i] <= 1e-10);

			// check value for next i
			check *= tf;
		}
	}
	return ok;
}

// END PROGRAM
