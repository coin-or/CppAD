/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/cppad.hpp>

/*
$begin change_const.cpp$$
$spell
	Jacobian
$$

$section Computing a Jacobian With Constants that Change$$
$index multiple, AD level$$
$index level, multiple AD$$
$index constant, that change$$
$index change, constant$$

$head Purpose$$
In this example we use two levels of taping so that a derivative
can have constant parameters that can be changed. To be specific,
we consider the function $latex f : \B{R}^2 \rightarrow \B{R}^2$$
$latex \[
f(x) = p \left( \begin{array}{c} 
	\sin( x_0 ) \\
	\sin( x_1 ) 
\end{array} \right)
\]$$
were $latex p \in \B{R}$$ is a parameter.
The Jacobian of this function is
$latex \[
g(x,p) = p \left( \begin{array}{cc}
	\cos( x_0 ) & 0 \\
	0           & \cos( x_1 )
\end{array} \right)
\] $$
In this example we use two levels of AD to avoid computing
the partial of $latex f(x)$$ with respect to $latex p$$,
but still allow for the evaluation of $latex g(x, p)$$
at different values of $latex p$$.

$end

*/

bool change_const(void) 
{	bool ok = true;                          // initialize test result

	typedef CppAD::AD<double>    A1_double;  // for first level of taping
	typedef CppAD::AD<A1_double> A2_double;  // for second level of taping

	size_t nu = 3;       // number components in u
	size_t nx = 2;       // number components in x
	size_t ny = 2;       // num components in f(x)
	size_t nJ = ny * nx; // number components in Jacobian of f(x)

	// temporary indices
	size_t j;

	// declare first level of independent variables
	CPPAD_TESTVECTOR(A1_double) a1_u(nu); 
	for(j = 0; j < nu; j++)
		a1_u[j] = 0.;
	CppAD::Independent(a1_u); 

	// parameter in computation of Jacobian
	A1_double a1_p = a1_u[2];

	// declare second level of independent variables
	CPPAD_TESTVECTOR(A2_double) a2_x(nx); 
	for(j = 0; j < nx; j++)
		a2_x[j] = 0.;
	CppAD::Independent(a2_x); 

	// compute dependent variables at second level
	CPPAD_TESTVECTOR(A2_double) a2_y(ny);
	a2_y[0] = sin( a2_x[0] ) * a1_p;
	a2_y[1] = sin( a2_x[1] ) * a1_p;

	// declare function object that computes values at the first level
	// (make sure we do not run zero order forward during constructor)
	CppAD::ADFun<A1_double> a1_f;
	a1_f.Dependent(a2_x, a2_y); 

	// compute the Jacobian of a1_f at a1_u[0], a1_u[1]
	CPPAD_TESTVECTOR(A1_double) a1_x(nx);
	a1_x[0] = a1_u[0];
	a1_x[1] = a1_u[1];
	CPPAD_TESTVECTOR(A1_double) a1_J(nJ);
	a1_J = a1_f.Jacobian( a1_x );
	
	// declare function object that maps u = (x, p) to Jacobian of f
	// (make sure we do not run zero order forward during constructor)
	CppAD::ADFun<double> g;
	g.Dependent(a1_u, a1_J);

	// remove extra variables used during the reconding of a1_f, 
	// but not needed any more.
	g.optimize();

	// compute the Jacobian of f using zero order forward
	// sweep with double values
	CPPAD_TESTVECTOR(double) J(nJ), u(nu);
	for(j = 0; j < nu; j++)
		u[j] = double(j+1);
	J = g.Forward(0, u);

	// accuracy for tests
	double eps = 100. * CppAD::numeric_limits<double>::epsilon();

	// y[0] = sin( x[0] ) * p
	// y[1] = sin( x[1] ) * p
	CPPAD_TESTVECTOR(double) x(nx);
	x[0]     = u[0];
	x[1]     = u[1];
	double p = u[2];

	// J[0] = partial y[0] w.r.t x[0] = cos( x[0] ) * p
	double check = cos( x[0] ) * p;
	ok   &= fabs( check - J[0] ) <= eps;

	// J[1] = partial y[0] w.r.t x[1] = 0.;
	check = 0.;
	ok   &= fabs( check - J[1] ) <= eps;

	// J[2] = partial y[1] w.r.t. x[0] = 0.
	check = 0.;
	ok   &= fabs( check - J[2] ) <= eps;

	// J[3] = partial y[1] w.r.t x[1] = cos( x[1] ) * p
	check = cos( x[1] ) * p;
	ok   &= fabs( check - J[3] ) <= eps;

	return ok;
}
// END PROGRAM
