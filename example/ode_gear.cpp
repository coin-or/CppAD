/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin OdeGear.cpp$$
$spell
	Rosen
$$

$section OdeGear: Example and Test$$

$index OdeGear, example$$
$index example, OdeGear$$
$index test, OdeGear$$

Define 
$latex x : \R \rightarrow \R^n$$ by
$latex \[
	x_i (t) =  t^{i+1}
\] $$ 
for $latex i = 1 , \ldots , n-1$$.
It follows that
$latex \[
\begin{array}{rclr}
x_i(0)     & = & 0                             & {\rm for \; all \;} i \\
x_i ' (t)  & = & 1                             & {\rm if \;} i = 0      \\
x_i '(t)   & = & (i+1) t^i = (i+1) x_{i-1} (t) & {\rm if \;} i > 0
\end{array}
\] $$
The example tests OdeGear using the relations above:

$code
$verbatim%example/ode_gear.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/ode_gear.hpp>
# include <cppad/cppad.hpp>        // For automatic differentiation

namespace {
	class Fun {
	public:
		// constructor
		Fun(bool use_x_) : use_x(use_x_) 
		{ }

		// compute f(t, x) both for double and AD<double>
		template <typename Scalar>
		void Ode(
			const Scalar                    &t, 
			const CPPAD_TEST_VECTOR<Scalar> &x, 
			CPPAD_TEST_VECTOR<Scalar>       &f)
		{	size_t n  = x.size();	
			Scalar ti(1);
			f[0]   = Scalar(1);
			size_t i;
			for(i = 1; i < n; i++)
			{	ti *= t;
				// convert int(size_t) to avoid warning
				// on _MSC_VER systems
				if( use_x )
					f[i] = int(i+1) * x[i-1];
				else	f[i] = int(i+1) * ti;
			}
		}

		void Ode_dep(
			const double                    &t, 
			const CPPAD_TEST_VECTOR<double> &x, 
			CPPAD_TEST_VECTOR<double>       &f_x)
		{	using namespace CppAD;

			size_t n  = x.size();	
			CPPAD_TEST_VECTOR< AD<double> > T(1);
			CPPAD_TEST_VECTOR< AD<double> > X(n);
			CPPAD_TEST_VECTOR< AD<double> > F(n);

			// set argument values
			T[0] = t;
			size_t i, j;
			for(i = 0; i < n; i++)
				X[i] = x[i];

			// declare independent variables
			Independent(X);

			// compute f(t, x)
			this->Ode(T[0], X, F);

			// define AD function object
			ADFun<double> Fun(X, F);

			// compute partial of f w.r.t x
			CPPAD_TEST_VECTOR<double> dx(n);
			CPPAD_TEST_VECTOR<double> df(n);
			for(j = 0; j < n; j++)
				dx[j] = 0.;
			for(j = 0; j < n; j++)
			{	dx[j] = 1.;
				df = Fun.Forward(1, dx);
				for(i = 0; i < n; i++)
					f_x [i * n + j] = df[i];
				dx[j] = 0.;
			}
		}

	private:
		const bool use_x;

	};
}

bool OdeGear(void)
{	bool ok = true; // initial return value
	size_t i, j;    // temporary indices

	size_t  m = 4;  // index of next value in X
	size_t  n = m;  // number of components in x(t)

	// vector of times
	CPPAD_TEST_VECTOR<double> T(m+1); 
	double step = .1;
	T[0]        = 0.;
	for(j = 1; j <= m; j++)
	{	T[j] = T[j-1] + step;
		step = 2. * step;
	}

	// initial values for x( T[m-j] ) 
	CPPAD_TEST_VECTOR<double> X((m+1) * n);
	for(j = 0; j < m; j++)
	{	double ti = T[j];
		for(i = 0; i < n; i++)
		{	X[ j * n + i ] = ti;
			ti *= T[j];
		}
	}

	// error bound
	CPPAD_TEST_VECTOR<double> e(n);

	size_t use_x;
	for( use_x = 0; use_x < 2; use_x++)
	{	// function object depends on value of use_x
		Fun F(use_x > 0); 

		// compute OdeGear approximation for x( T[m] )
		CppAD::OdeGear(F, m, n, T, X, e);

		double check = T[m];
		for(i = 0; i < n; i++)
		{	// method is exact up to order m and x[i] = t^{i+1}
			if( i + 1 <= m ) ok &= CppAD::NearEqual(
				X[m * n + i], check, 1e-10, 1e-10
			);
			// error bound should be zero up to order m-1
			if( i + 1 < m ) ok &= CppAD::NearEqual(
				e[i], 0., 1e-10, 1e-10
			);
			// check value for next i
			check *= T[m];
		}
	}
	return ok;
}

// END PROGRAM
