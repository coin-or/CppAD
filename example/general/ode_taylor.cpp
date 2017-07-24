/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ode_taylor.cpp$$
$spell
	Taylor
	Cpp
	const
	std
	Adolc
	adouble
$$

$section Taylor's Ode Solver: An Example and Test$$
$mindex Taylor$$

$head Purpose$$
This example solves an ordinary differential equation using Taylor's method;
i.e.,
$latex \[
	Z(t + \Delta t)
	\approx
	Z^{(0)} (t) +
	\frac{ Z^{(1)} (t) }{ 1 !} \Delta t + \cdots +
	\frac{ Z^{(p)} (t) }{ p !} ( \Delta t )^p )
\] $$

$head ODE$$
The ODE is defined by the function
$latex h : \B{R}^n \rightarrow \B{R}^n$$,
which for this example is given by
$latex \[
	Z^{(1)} (t) = H[ Z(t) ] =
	\left( \begin{array}{c}
			1                       \\
			Z_1 (t)                 \\
			\vdots                  \\
			Z_{n-1} (t)
	\end{array} \right)
\] $$
and the initial condition is $latex z(0) = 0$$.

$head ODE Solution$$
The solution for this example can be calculated by
starting with the first row and then using the solution
for the first row to solve the second and so on.
Doing this we obtain
$latex \[
	Z(t) =
	\left( \begin{array}{c}
		t           \\
		t^2 / 2     \\
		\vdots      \\
		t^n / n !
	\end{array} \right)
\] $$

$head Forward Mode$$
Given the Taylor coefficients for $latex k = 0 , \ldots , K$$
$latex \[
	z^{(k)} = \frac{ Z^{(k)} }{ k !} (t)
\]$$
we note that
$latex \[
\begin{array}{rcl}
Z^{(1)} (t)
& = &
H( z^{(0)} + z^{(1)} t + \cdots + z^{(K)} t^K ) + O( t^{K+1} )
\\
& = &
h^{(0)} + h^{(1)} t + \cdots + h^{(K)} t^K + O( t^{K+1} )
\end{array}
\] $$
where $latex h^{(k)}$$ is the $th k$$ order Taylor coefficient for
$latex H( Z(t) )$$.
Taking $th K$$ order derivatives of both sides we obtain
$latex \[
\begin{array}{rcl}
Z^{(K+1)} (t) & = & K ! h^{(K)}  \\
z^{(K+1)}     & = & h^{(K)} / K
\end{array}
\] $$
The code below uses this relationship to implement Taylor's
method for approximating the solution of an ODE.

$code
$srcfile%example/general/ode_taylor.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
--------------------------------------------------------------------------
*/
// BEGIN C++

# include <cppad/cppad.hpp>

// =========================================================================
// define types for each level
namespace { // BEGIN empty namespace
	using CppAD::AD;

	CPPAD_TESTVECTOR( AD<double> ) ode(
		const CPPAD_TESTVECTOR( AD<double> )& Z )
	{	size_t n = Z.size();
		CPPAD_TESTVECTOR( AD<double> ) y(n);
		y[0] = 1;
		for(size_t k = 1; k < n; k++)
			y[k] = Z[k-1];
		return y;
	}

}

// -------------------------------------------------------------------------
// Example that uses Taylor's method to solve ordinary differential equaitons
bool ode_taylor(void)
{	// initialize the return value as true
	bool ok = true;

	// some temporary indices
	size_t i, j, k;

	// The ODE does not depend on the arugment values
	// so only tape once, also note that ode does not depend on t
	size_t n = 5;    // number of independent and dependent variables
	CPPAD_TESTVECTOR( AD<double> ) a_x(n), a_y(n);
	CppAD::Independent( a_x );
	a_y = ode(a_x);
	CppAD::ADFun<double> H(a_x, a_y);

	// initialize the solution vector at time zero
	CPPAD_TESTVECTOR( double ) z(n);
	for(j = 0; j < n; j++)
		z[j] = 0.0;

	size_t order   = n;   // order of the Taylor method
	size_t n_step  = 4;   // number of time steps
	double dt      = 0.5; // step size in time
	// Taylor coefficients of order k
	CPPAD_TESTVECTOR( double ) hk(n), zk(n);

	// loop with respect to each step of Taylor's method
	for(size_t i_step = 0; i_step < n_step; i_step++)
	{	// Use Taylor's method to take a step
		zk           = z;     // initialize z^{(k)}  for k = 0
		double dt_kp = dt;    // initialize dt^(k+1) for k = 0
		for(k = 0; k < order; k++)
		{	// evaluate k-th order Taylor coefficient of H
			hk = H.Forward(k, zk);

			for(j = 0; j < n; j++)
			{	// convert to (k+1)-Taylor coefficient for z
				zk[j] = hk[j] / double(k + 1);

				// add term for to this Taylor coefficient
				// to solution for y(t, x)
				z[j] += zk[j] * dt_kp;
			}
			// next power of t
			dt_kp *= dt;
		}
	}

	// check solution of the ODE,
	// Taylor's method should have no truncation error for this case
	double eps   = 100. * std::numeric_limits<double>::epsilon();
	double check = 1.;
	double t     = double(n_step) * dt;
	for(i = 0; i < n; i++)
	{	check *= t / double(i + 1);
		ok &= CppAD::NearEqual(z[i], check, eps, eps);
	}

	return ok;
}

// END C++
