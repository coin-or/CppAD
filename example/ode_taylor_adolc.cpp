/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ode_taylor_adolc.cpp$$
$spell
	hpp
	Taylor
	Cpp
	const
	std
	AdolcDir
	adouble
	Vec
$$

$section Using Adolc with Taylor's Ode Solver: An Example and Test$$
$index ODE, Taylor Adolc$$
$index Taylor, ODE Adolc$$
$index Adolc, ODE$$

$head Purpose$$
This is a realistic example using 
two levels of taping (see $cref/mul_level/$$).
The first level of taping uses Adolc's $code adouble$$ type
to tape the solution of an ordinary differential equation.
This solution is then differentiated with respect to a parameter vector.
The second level of taping uses CppAD's type $code AD<adouble>$$ 
to take derivatives during the solution of the differential equation.
These derivatives are used in the application
of Taylor's method to the solution of the ODE.
The example $cref/ode_taylor.cpp/$$ computes the same values using
$code AD<double>$$ and $code AD< AD<double> >$$.

$head ODE$$
For this example the ODE's are defined by the function
$latex h : \R^n \times \R^n \rightarrow \R^n$$ where
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
and the function $latex g : \R^n \rightarrow \R^n$$ is used to
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

$head base_adolc.hpp$$
The file $cref/base_adolc.hpp/$$ is implements the
$cref/Base type requirements/base_require/$$ where $italic Base$$
is $code adolc$$.


$head Tracking New and Delete$$
Adolc uses raw memory arrays that depend on the number of 
dependent and independent variables, hence $code new$$ and $code delete$$
are used to allocate this memory.
The preprocessor macros 
$small
$cref/CPPAD_TRACK_NEW_VEC/TrackNewDel/TrackNewVec/$$ 
$$
and
$small
$cref/CPPAD_TRACK_DEL_VEC/TrackNewDel/TrackDelVec/$$ 
$$
are used to check for errors in the
use of $code new$$ and $code delete$$ when the example is compiled for
debugging (when $code NDEBUG$$ is not defined).

$head Configuration Requirement$$
This example will be compiled and tested provided that
the value $cref/AdolcDir/InstallUnix/AdolcDir/$$ is specified on the 
$cref/configure/InstallUnix/Configure/$$ command line.

$code
$verbatim%example/ode_taylor_adolc.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
--------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# include <adolc/adouble.h>
# include <adolc/taping.h>
# include <adolc/drivers/drivers.h>

// definitions not in Adolc distribution and required to use CppAD::AD<adouble>
# include "base_adolc.hpp"

# include <cppad/cppad.hpp>
// ==========================================================================
namespace { // BEGIN empty namespace
// define types for each level
typedef adouble            ADdouble;
typedef CppAD::AD<adouble> ADDdouble;

// -------------------------------------------------------------------------
// class definition for C++ function object that defines ODE
class Ode {
private:
	// copy of a that is set by constructor and used by g(y)
	CPPAD_TEST_VECTOR< ADdouble > x_; 
public:
	// constructor
	Ode( CPPAD_TEST_VECTOR< ADdouble > x) : x_(x)
	{ }
	// the function g(y) is evaluated with two levels of taping
	CPPAD_TEST_VECTOR< ADDdouble > operator()
	( const CPPAD_TEST_VECTOR< ADDdouble > &y) const
	{	size_t n = y.size();
		CPPAD_TEST_VECTOR< ADDdouble > g(n);
		size_t i;
		g[0] = x_[0];
		for(i = 1; i < n; i++)
			g[i] = x_[i] * y[i-1];

		return g;
	}
};

// -------------------------------------------------------------------------
// Routine that uses Taylor's method to solve ordinary differential equaitons
// and allows for algorithmic differentiation of the solution. 
CPPAD_TEST_VECTOR < ADdouble > taylor_ode_adolc(
	Ode                     G       ,  // function that defines the ODE
	size_t                  order   ,  // order of Taylor's method used
	size_t                  nstep   ,  // number of steps to take
	ADdouble                &dt     ,  // Delta t for each step
	CPPAD_TEST_VECTOR< ADdouble > &y_ini  )  // y(t) at the initial time
{
	// some temporary indices
	size_t i, k, ell;

	// number of variables in the ODE
	size_t n = y_ini.size();

	// copies of x and g(y) with two levels of taping
	CPPAD_TEST_VECTOR< ADDdouble >   Y(n), Z(n);

	// y, y^{(k)} , z^{(k)}, and y^{(k+1)}
	CPPAD_TEST_VECTOR< ADdouble >  y(n), y_k(n), z_k(n), y_kp(n);
	
	// initialize x
	for(i = 0; i < n; i++)
		y[i] = y_ini[i];

	// loop with respect to each step of Taylors method
	for(ell = 0; ell < nstep; ell++)
	{	// prepare to compute derivatives of in ADdouble
		for(i = 0; i < n; i++)
			Y[i] = y[i];
		CppAD::Independent(Y);

		// evaluate ODE in ADDdouble
		Z = G(Y);

		// define differentiable version of g: X -> Y
		// that computes its derivatives in ADdouble
		CppAD::ADFun<ADdouble> g(Y, Z);

		// Use Taylor's method to take a step
		y_k            = y;     // initialize y^{(k)}
		ADdouble dt_kp = dt;    // initialize dt^(k+1)
		for(k = 0; k <= order; k++)
		{	// evaluate k-th order Taylor coefficient of y
			z_k = g.Forward(k, y_k);
 
			for(i = 0; i < n; i++)
			{	// convert to (k+1)-Taylor coefficient for x
				y_kp[i] = z_k[i] / ADdouble(k + 1);

				// add term for to this Taylor coefficient
				// to solution for y(t, x)
				y[i]    += y_kp[i] * dt_kp;
			}
			// next power of t
			dt_kp *= dt;
			// next Taylor coefficient
			y_k   = y_kp;
		}
	}
	return y;
}
} // END empty namespace
// ==========================================================================
// Routine that tests algorithmic differentiation of solutions computed
// by the routine taylor_ode.
bool ode_taylor_adolc(void)
{	// initialize the return value as true	
	bool ok = true;

	// number of components in differential equation
	size_t n = 4;

	// some temporary indices
	size_t i, j;

	// parameter vector in both double and ADdouble
	double *x;
	x = CPPAD_TRACK_NEW_VEC(n, x);  // track x = new double[n];
	CPPAD_TEST_VECTOR<ADdouble> X(n);
	for(i = 0; i < n; i++)
		X[i] = x[i] = double(i + 1);

	// declare the parameters as the independent variable
	int tag = 0;                     // Adolc setup
	int keep = 1;
	trace_on(tag, keep);
	for(i = 0; i < n; i++)
		X[i] <<= double(i + 1);  // X is independent for adouble type

	// arguments to taylor_ode_adolc 
	Ode G(X);                // function that defines the ODE
	size_t   order = n;      // order of Taylor's method used
	size_t   nstep = 2;      // number of steps to take
	ADdouble DT    = 1.;     // Delta t for each step
	// value of y(t, x) at the initial time
	CPPAD_TEST_VECTOR< ADdouble > Y_INI(n);
	for(i = 0; i < n; i++)
		Y_INI[i] = 0.;

	// integrate the differential equation
	CPPAD_TEST_VECTOR< ADdouble > Y_FINAL(n);
 	Y_FINAL = taylor_ode_adolc(G, order, nstep, DT, Y_INI);

	// declare the differentiable fucntion f : A -> Y_FINAL
	// (corresponding to the tape of adouble operations)
	double *y_final;
	y_final = CPPAD_TRACK_NEW_VEC(n, y_final); // y_final= new double[m]
	for(i = 0; i < n; i++)
		Y_FINAL[i] >>= y_final[i];
	trace_off();

	// check function values
	double check = 1.;
	double t     = nstep * DT.value();
	for(i = 0; i < n; i++)
	{	check *= x[i] * t / double(i + 1);
		ok &= CppAD::NearEqual(y_final[i], check, 1e-10, 1e-10);
	}

	// memory where Jacobian will be returned
	double *jac_;
	jac_ = CPPAD_TRACK_NEW_VEC(n * n, jac_); // jac_ = new double[n*n]
	double **jac;
	jac  = CPPAD_TRACK_NEW_VEC(n, jac);      // jac = new (*double)[n]
	for(i = 0; i < n; i++)
		jac[i] = jac_ + i * n;

	// evaluate Jacobian of h at a
	size_t m = n;              // # dependent variables
	jacobian(tag, int(m), int(n), x, jac); 
	
	// check Jacobian 
	for(i = 0; i < n; i++)
	{	for(j = 0; j < n; j++)
		{	if( i < j )
				check = 0.;
			else	check = y_final[i] / x[j];
			ok &= CppAD::NearEqual(jac[i][j], check, 1e-10, 1e-10);
		}
	}

	CPPAD_TRACK_DEL_VEC(x);        // check usage of delete
	CPPAD_TRACK_DEL_VEC(y_final);
	CPPAD_TRACK_DEL_VEC(jac_);
	CPPAD_TRACK_DEL_VEC(jac);
	return ok;
}

// END PROGRAM
