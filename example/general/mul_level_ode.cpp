/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin mul_level_ode.cpp$$
$spell
	Taylor
	Cpp
	const
	std
	Adolc
	adouble
$$

$section Taylor's Ode Solver: A Multi-Level AD Example and Test$$

$head Purpose$$
This is a realistic example using
two levels of AD; see $cref mul_level$$.
The first level uses $code AD<double>$$ to tape the solution of an
ordinary differential equation.
This solution is then differentiated with respect to a parameter vector.
The second level uses $code AD< AD<double> >$$
to take derivatives during the solution of the differential equation.
These derivatives are used in the application
of Taylor's method to the solution of the ODE.
The example $cref mul_level_adolc_ode.cpp$$ computes the same values using
Adolc's type $code adouble$$ and CppAD's type $code AD<adouble>$$.
The example $cref ode_taylor.cpp$$ is a simpler applications
of Taylor's method for solving an ODE.

$head ODE$$
For this example the ODE's are defined by the function
$latex h : \B{R}^n \times \B{R}^n \rightarrow \B{R}^n$$ where
$latex \[
	h[ x, y(t, x) ] =
	\left( \begin{array}{c}
			x_0                     \\
			x_1 y_0 (t, x)          \\
			\vdots                  \\
			x_{n-1} y_{n-2} (t, x)
	\end{array} \right)
	=
	\left( \begin{array}{c}
			\partial_t y_0 (t , x)      \\
			\partial_t y_1 (t , x)      \\
			\vdots                      \\
			\partial_t y_{n-1} (t , x)
	\end{array} \right)
\] $$
and the initial condition $latex y(0, x) = 0$$.
The value of $latex x$$ is fixed during the solution of the ODE
and the function $latex g : \B{R}^n \rightarrow \B{R}^n$$ is used to
define the ODE where
$latex \[
	g(y) =
	\left( \begin{array}{c}
			x_0     \\
			x_1 y_0 \\
			\vdots  \\
			x_{n-1} y_{n-2}
	\end{array} \right)
\] $$

$head ODE Solution$$
The solution for this example can be calculated by
starting with the first row and then using the solution
for the first row to solve the second and so on.
Doing this we obtain
$latex \[
	y(t, x ) =
	\left( \begin{array}{c}
		x_0 t                  \\
		x_1 x_0 t^2 / 2        \\
		\vdots                 \\
		x_{n-1} x_{n-2} \ldots x_0 t^n / n !
	\end{array} \right)
\] $$

$head Derivative of ODE Solution$$
Differentiating the solution above,
with respect to the parameter vector $latex x$$,
we notice that
$latex \[
\partial_x y(t, x ) =
\left( \begin{array}{cccc}
y_0 (t,x) / x_0      & 0                   & \cdots & 0      \\
y_1 (t,x) / x_0      & y_1 (t,x) / x_1     & 0      & \vdots \\
\vdots               & \vdots              & \ddots & 0      \\
y_{n-1} (t,x) / x_0  & y_{n-1} (t,x) / x_1 & \cdots & y_{n-1} (t,x) / x_{n-1}
\end{array} \right)
\] $$

$head Taylor's Method Using AD$$
An $th m$$ order Taylor method for
approximating the solution of an
ordinary differential equations is
$latex \[
	y(t + \Delta t , x)
	\approx
	\sum_{k=0}^p \partial_t^k y(t , x ) \frac{ \Delta t^k }{ k ! }
	=
	y^{(0)} (t , x ) +
	y^{(1)} (t , x ) \Delta t + \cdots +
	y^{(p)} (t , x ) \Delta t^p
\] $$
where the Taylor coefficients $latex y^{(k)} (t, x)$$ are defined by
$latex \[
	y^{(k)} (t, x) = \partial_t^k y(t , x ) / k !
\] $$
We define the function $latex z(t, x)$$ by the equation
$latex \[
	z ( t , x ) = g[ y ( t , x ) ] = h [ x , y( t , x ) ]
\] $$
It follows that
$latex \[
\begin{array}{rcl}
	\partial_t y(t, x) & = & z (t , x)
	\\
	 \partial_t^{k+1} y(t , x) & = & \partial_t^k z (t , x)
	\\
	y^{(k+1)} ( t , x) & = & z^{(k)} (t, x) / (k+1)
\end{array}
\] $$
where $latex  z^{(k)} (t, x)$$ is the
$th k$$ order Taylor coefficient
for $latex z(t, x)$$.
In the example below, the Taylor coefficients
$latex \[
	y^{(0)} (t , x) , \ldots , y^{(k)} ( t , x )
\] $$
are used to calculate the Taylor coefficient $latex z^{(k)} ( t , x )$$
which in turn gives the value for $latex  y^{(k+1)} y ( t , x)$$.

$head Source$$
$code
$srcfile%example/general/mul_level_ode.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
--------------------------------------------------------------------------
*/
// BEGIN C++

# include <cppad/cppad.hpp>

// =========================================================================
// define types for each level
namespace { // BEGIN empty namespace
typedef CppAD::AD<double>   a1type;
typedef CppAD::AD<a1type>   a2type;

// -------------------------------------------------------------------------
// class definition for C++ function object that defines ODE
class Ode {
private:
	// copy of a that is set by constructor and used by g(y)
	CPPAD_TESTVECTOR(a1type) a1x_;
public:
	// constructor
	Ode(const CPPAD_TESTVECTOR(a1type)& a1x) : a1x_(a1x)
	{ }
	// the function g(y) is evaluated with two levels of taping
	CPPAD_TESTVECTOR(a2type) operator()
	( const CPPAD_TESTVECTOR(a2type)& a2y) const
	{	size_t n = a2y.size();
		CPPAD_TESTVECTOR(a2type) a2g(n);
		size_t i;
		a2g[0] = a1x_[0];
		for(i = 1; i < n; i++)
			a2g[i] = a1x_[i] * a2y[i-1];

		return a2g;
	}
};

// -------------------------------------------------------------------------
// Routine that uses Taylor's method to solve ordinary differential equaitons
// and allows for algorithmic differentiation of the solution.
CPPAD_TESTVECTOR(a1type) taylor_ode(
	Ode                            G       ,  // function that defines the ODE
	size_t                         order   ,  // order of Taylor's method used
	size_t                         nstep   ,  // number of steps to take
	const a1type&                  a1dt    ,  // Delta t for each step
	const CPPAD_TESTVECTOR(a1type)& a1y_ini)  // y(t) at the initial time
{
	// some temporary indices
	size_t i, k, ell;

	// number of variables in the ODE
	size_t n = a1y_ini.size();

	// copies of x and g(y) with two levels of taping
	CPPAD_TESTVECTOR(a2type)   a2y(n), a2z(n);

	// y, y^{(k)} , z^{(k)}, and y^{(k+1)}
	CPPAD_TESTVECTOR(a1type)  a1y(n), a1y_k(n), a1z_k(n), a1y_kp(n);

	// initialize x
	for(i = 0; i < n; i++)
		a1y[i] = a1y_ini[i];

	// loop with respect to each step of Taylors method
	for(ell = 0; ell < nstep; ell++)
	{	// prepare to compute derivatives using a1type
		for(i = 0; i < n; i++)
			a2y[i] = a1y[i];
		CppAD::Independent(a2y);

		// evaluate ODE in a2type
		a2z = G(a2y);

		// define differentiable version of a1g: y -> z
		// that computes its derivatives using a1type objects
		CppAD::ADFun<a1type> a1g(a2y, a2z);

		// Use Taylor's method to take a step
		a1y_k            = a1y;     // initialize y^{(k)}
		a1type   a1dt_kp = a1dt;  // initialize dt^(k+1)
		for(k = 0; k <= order; k++)
		{	// evaluate k-th order Taylor coefficient of y
			a1z_k = a1g.Forward(k, a1y_k);

			for(i = 0; i < n; i++)
			{	// convert to (k+1)-Taylor coefficient for x
				a1y_kp[i] = a1z_k[i] / a1type(k + 1);

				// add term for to this Taylor coefficient
				// to solution for y(t, x)
				a1y[i]    += a1y_kp[i] * a1dt_kp;
			}
			// next power of t
			a1dt_kp *= a1dt;
			// next Taylor coefficient
			a1y_k   = a1y_kp;
		}
	}
	return a1y;
}
} // END empty namespace
// ==========================================================================
// Routine that tests alogirhtmic differentiation of solutions computed
// by the routine taylor_ode.
bool mul_level_ode(void)
{	bool ok = true;
	double eps = 100. * std::numeric_limits<double>::epsilon();

	// number of components in differential equation
	size_t n = 4;

	// some temporary indices
	size_t i, j;

	// parameter vector in both double and a1type
	CPPAD_TESTVECTOR(double)  x(n);
	CPPAD_TESTVECTOR(a1type)  a1x(n);
	for(i = 0; i < n; i++)
		a1x[i] = x[i] = double(i + 1);

	// declare the parameters as the independent variable
	CppAD::Independent(a1x);

	// arguments to taylor_ode
	Ode G(a1x);                // function that defines the ODE
	size_t   order = n;      // order of Taylor's method used
	size_t   nstep = 2;      // number of steps to take
	a1type   a1dt  = double(1.);     // Delta t for each step
	// value of y(t, x) at the initial time
	CPPAD_TESTVECTOR(a1type) a1y_ini(n);
	for(i = 0; i < n; i++)
		a1y_ini[i] = 0.;

	// integrate the differential equation
	CPPAD_TESTVECTOR(a1type) a1y_final(n);
	a1y_final = taylor_ode(G, order, nstep, a1dt, a1y_ini);

	// define differentiable fucntion object f : x -> y_final
	// that computes its derivatives in double
	CppAD::ADFun<double> f(a1x, a1y_final);

	// check function values
	double check = 1.;
	double t     = double(nstep) * Value(a1dt);
	for(i = 0; i < n; i++)
	{	check *= x[i] * t / double(i + 1);
		ok &= CppAD::NearEqual(Value(a1y_final[i]), check, eps, eps);
	}

	// evaluate the Jacobian of h at a
	CPPAD_TESTVECTOR(double) jac ( f.Jacobian(x) );
	// There appears to be a bug in g++ version 4.4.2 because it generates
	// a warning for the equivalent form
	// CPPAD_TESTVECTOR(double) jac = f.Jacobian(x);

	// check Jacobian
	for(i = 0; i < n; i++)
	{	for(j = 0; j < n; j++)
		{	double jac_ij = jac[i * n + j];
			if( i < j )
				check = 0.;
			else	check = Value( a1y_final[i] ) / x[j];
			ok &= CppAD::NearEqual(jac_ij, check, eps, eps);
		}
	}
	return ok;
}

// END C++
