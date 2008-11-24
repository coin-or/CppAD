/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin ipopt_cppad_ode.cpp$$
$spell
	CppAD
	ipopt_cppad_nlp
	eval_r
	nd
$$


$section Fitting an Ode Using the CppAD Interface to Ipopt$$

$index optimizing, ODE solution using AD$$
$index ODE, optimizing solution using AD$$
$index example, ipopt_cppad_nlp$$
$index ipopt_cppad_nlp, example$$

$head Purpose$$
The CppAD interface to Ipopt 
$cref/ipopt_cppad_nlp/$$
enables one to represent an optimization problem
with a large number of variables and constraints in terms of functions
with a few domain and range variables.
This is a demonstration of how to use this representation.

$head General Problem$$
This example solves a problem of the form
$latex \[
{\rm minimize} \; 
	\sum_{k=0}^{nd-1} H_k ( y( td[k+1] , a ) , a ) \;
		{\rm with \; respect \; to} \; a \in \R^{na}
\] $$
where the function 
$latex H_k : \R^{ny} \times \R^{na} \rightarrow \R$$ is known 
and the function $latex y : \R \times \R^{na} \rightarrow \R^{ny}$$ 
is the solution of the initial value ordinary differential equation
$latex \[
\begin{array}{rcl}
	y(0, a)            & = &  F(a)                 \\
	\partial_t y(t, a) & = & G( y(t, a) , a )
\end{array}
\] $$


$head ODE Discrete Approximation$$
The ODE solution is approximated using a trapezoidal rule with $latex ns$$
uniformly spaced time intervals before the first measurement time
and between all the other measurement times. 
We use $latex ts[M]$$ for $latex M = 0 , \ldots , ns * nd$$ to denote
the corresponding time points; i.e.,
for $latex k = 0 , \ldots , nd - 1$$ 
and for $latex \ell = 0 , \ldots , ns$$
$latex \[
\begin{array}{rcl}
	dt[k * ns + \ell ]  & = & \frac{td[k+1] - td[k]}{ns}
	\\
	ts[ k * ns + \ell ] & = & td[k] + dt[k * ns + \ell ] * \ell
\end{array}
\] $$
where the value $latex td[0]$$ is defined to be zero.
We use $latex y[M]$$ to denote our approximate value for 
$latex y( ts[M] , a)$$.
Our trapezoidal approximation to the differential equation is given by
$latex \[
	y[M+1] = y[M] + \frac{G( y[M] , a) + G( y[M+1] , a )}{2} * dt[M]
\] $$
for $latex M = 0 , \ldots , ns * nd - 1$$.

$head Optimization Problem$$
We use $latex x$$ for the argument to the  optimization problem where
$latex \[
	x = ( y[1] , y[2] , \ldots , y[ns * nd] , a )
\] $$
Note that the initial value $latex y[0]$$ is given by $latex F(a)$$.
The optimization problem that we solve includes our ODE approximation
as constraints; i.e.
$latex \[
\begin{array}{lcr}
{\rm minimize} & 
	\sum_{k=0}^{nd-1} H_k ( y[ns * (k+1)], a ) &
		{\rm w.r.t.} \; x \in \R^{ns * nd * ny + na}
\\
{\rm subject \; to} &
	0 = y[M+1] - y[M] - \frac{G( y[M] , a) + G( y[M+1] , a )}{2} * dt[M]
	& {\rm for} \; M = 0 , \ldots , ns * nd
\end{array}
\] $$

$head eval_r(k, u)$$
The $code eval_r$$ function in the code below uses the index $italic k$$
and the vector u in the following way:

$codei%

0 <= %k% < %nd%
%$$ 
In this case, $latex u = [ y[M], y[M+1], a] \in \R^{ny + ny + na}$$ and
$latex \[
r_k (u) = y[M+1] - y[M] - \frac{G( y[M] , a) + G( y[M+1] , a )}{2} * dt[M]
\] $$

$codei%
%k% = %nd% 
%$$
In this case, $latex u = [ y[1], a] \in \R^{ny + na}$$ and
$latex \[
r_k (u) = y[1] - F(a) - \frac{G( F(a) , a) + G( y[1] , a )}{2} * dt[M]
\] $$

$codei%
%k% > %nd%
%$$
For $latex k = nd + 1 , ... , nd + nd$$,  
define $latex M = (k - nd) * ns$$.
The argument $icode u$$ to $codei r_eval$$ is
given by $latex u = [ y[M], a ] \in \R^{ny + na}$$,
$latex \[
	r_k (u) = H_{nd + 1 - k} ( y[M], a )
\] $$


$head Source Code$$
Almost all the code below is for the general problem but some of it
for a specific case defined by the function $code y_one(t)$$.
$code
$verbatim%ipopt_cppad/ipopt_cppad_ode.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$


$end
------------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# include "ipopt_cppad_nlp.hpp"

// include a definition for Number.
typedef Ipopt::Number Number;

namespace {
	//------------------------------------------------------------------
	// simulated data
	Number a0 = 1.;  // simulation value for a[0]
	Number a1 = 2.;  // simulation value for a[1]
	Number a2 = 1.;  // simulatioln value for a[2]

	// function used to simulate data
	Number y_one(Number t)
	{	Number y_1 =  a0*a1 * (exp(-a2*t) - exp(-a1*t)) / (a1 - a2);
		return y_1;
	}

	// time points were we have data (no data at first point)
	double td[] = { 0.,        .5,         1.,         1.5  }; 
	// Simulated data for case with no noise (first point is not used)
	double yd[] = { 0.,  y_one(.5),  y_one(1.),  y_one(1.5) };
	// number of actual data values
	size_t nd   = sizeof(td) / sizeof(td[0]) - 1;
}
// ---------------------------------------------------------------------------
namespace { // Begin empty namespace 

size_t ny = 2;   // dimension of y(t, a) for this case
size_t na = 3;   // dimension of a for this case 
size_t ns = 5;   // number of grid intervals with in each data interval

// F(a) = y(0, a); i.e., initial condition
template <class Vector>
Vector eval_F(const Vector &a)
{	// This particual F is a case where ny == 2 and na == 3	
	Vector F(ny);
	// y_0 (t) = a[0]*exp(-a[1] * t)
	F[0] = a[0];
	// y_1 (t) = a[0]*a[1]*(exp(-a[2] * t) - exp(-a[1] * t))/(a[1] - a[2])
	F[1] = 0.; 
	return F;
}
// G(y, a) =  y'(t, a); i.e. ODE
template <class Vector>
Vector eval_G(const Vector &y , const Vector &a)
{	// This particular G is for a case where ny == 2 and na == 3
	Vector G(ny);
	// y_0 (t) = a[0]*exp(-a[1] * t)
	G[0] = -a[1] * y[0];  
	// y_1 (t) = a[0]*a[1]*(exp(-a[2] * t) - exp(-a[1] * t))/(a[1] - a[2])
	G[1] = - G[0] - a[2] * y[1]; 
	return G;
} 
// H(k, y, a) = contribution to objective at k-th data point
template <class Scalar, class Vector>
Scalar eval_H(size_t k, const Vector &y, const Vector &a)
{	// This particular H is for a case where y_1 (t) is measured
	Scalar diff = yd[k+1] - y[1];
 	return diff * diff;
}

// -----------------------------------------------------------------------------
class FG_info : public ipopt_cppad_fg_info
{
private:
	bool retape_;
public:
	// derived class part of constructor
	FG_info(bool retape)
	: retape_ (retape)
	{ }
	size_t number_functions(void)
	{	return nd + 1 + nd; }
	ADVector eval_r(size_t k, const ADVector &u)
	{	size_t j;
		// objective function case
		if( k > nd )
		{	// We use a differnent k for each data point
			// (there may be more efficient ways to do this).
			ADVector r(1);
			size_t j;
			// u is [y(t) , a] where t = td[ell + 1]
			ADVector y_M(ny), a(na);
			for(j = 0; j < ny; j++)
				y_M[j] = u[j];
			for(j = 0; j < na; j++)
				a[j] = u[ny + j];
			r[0] = eval_H<ADNumber>(k - nd - 1, y_M, a);
			return r;
		}
		ADVector y_M(ny), y_M1(ny), a(na);
		Number dt;
		if( k == nd )
		{	// u = [y_M1 , a] where y_M1 is y(t) at t = ts[1]
			for(j = 0; j < ny; j++)
				y_M1[j] = u[j];
			for(j = 0; j < na; j++)
				a[j] = u[ny + j];
			// y_M is value of y(t) at t = ts[0]
			y_M = eval_F(a);
			// size of subinterval
			dt = (td[1] - td[0]) / Number(ns);
		}
		else
		{	// u = [y_M, y_M1, a] where y_M is y(t) at 
			// t = ts[ k * ns + ell + 1 ] for k = 0 case 
			// t = ts[ k * ns + ell ]     for 0 < k < nd cases
			for(j = 0; j < ny; j++)
			{	y_M[j]  = u[j];
				y_M1[j] = u[ny + j];
			}
			for(j = 0; j < na; j++)
				a[j] = u[2 * ny + j];
			dt = (td[k+1] - td[k]) / Number(ns);
		}
		ADVector G_M   = eval_G(y_M,  a);
		ADVector G_M1  = eval_G(y_M1, a);
		ADVector r(ny);
		for(j = 0; j < ny; j++)
			r[j] = y_M1[j] - y_M[j] - (G_M1[j] + G_M[j]) * dt/2.;
		return r;
	}
	// Operation sequence does not depend on u so retape = false should
	// work and be faster. Allow for both for testing.
	bool retape(size_t k)
	{	return retape_; }
	// size of the vector u in eval_r
	size_t domain_size(size_t k)
	{	if( k > nd )
			return ny + na;   // objective function
		if( k == nd )
			return ny + na;  // initial difference equation
		return 2 * ny + na;      // other difference equations
	}
	// size of the vector r in eval_r
	size_t range_size(size_t k)
	{	if( k > nd )
			return 1;
		return ny; 
	}
	size_t number_terms(size_t k)
	{
		if( k > nd )
			return 1;        // one objective function term
		if( k == nd )
			return 1;        // one initial difference equation
		if( k == 0 )
			return ns - 1;   // exclude initial difference equation
		return ns;               // number difference equations
	}
	// k is index of time interal between data values
	// ell is index of grid point within data interval
	void index(size_t k, size_t ell, SizeVector& I, SizeVector& J)
	{	size_t i, j;
		// number of constraints, one for each subinterval interval
		// times the number of components in y(t)
		size_t m = nd * ns * ny;
		if( k > nd )
		{	// must use range index zero for the objective function
			I[0] = 0;
			// The first ny components of u is y(t) at 
			// 	t = td[k-nd] = ts[(k-nd)*ns]
			// Note that x starts with y(t) at time ts[1]. 
			// Also Note that there are ns subintervals between 
			// each data point.
			for(j = 0; j < ny; j++)
				J[j] = ( (k - nd) * ns - 1) * ny + j;
			// All of a (which is last na components of x)
			for(j = 0; j < na; j++)
				J[ny + j] = m + j; 
			return;
		}
		// special grid equaiton for initial value
		if( k == nd )
		{	// range index in fg for first ny components of g
			for(i = 0; i < ny; i++)
				I[i] = 1 + i;
			// u starts with the first j components of x
			// (which corresponding to y(t) at ts[1])
			for(j = 0; j < ny; j++)
				J[j] = j;
			// following that, u contains the vector a 
			for(j = 0; j < na; j++)
				J[ny + j] = m + j;
			return;
		}
		// index of first grid point in ts for difference equation
		size_t M; 
		if( k == 0 )
			M = k * ns + ell + 1;
		else	M = k * ns + ell;
		for(j = 0; j < ny; j++)
		{	J[j]          = (M - 1) * ny  + j; // index of y_M in x
			J[ny + j]     = J[j] + ny;         // index of y_M1
		}
		for(j = 0; j < na; j++)
			J[2 * ny + j] = m + j;                      // a
		// There are ny difference equations for each grid point.
		// (add one for the objective function index).
		for(i = 0; i < ny; i++)
			I[i] = M * ny + 1 + i;
	} 
};

} // End empty namespace
// ---------------------------------------------------------------------------

bool ipopt_cppad_ode(void)
{	bool ok = true;
	size_t j, I;

	// number of constraints (range dimension of g)
	size_t m = nd * ns * ny;
	// number of independent variables (domain dimension for f and g)
	size_t n = m + na;
	// the argument vector for the optimization is 
	// y(t) at t = ts[1] , ... , ts[nd*ns] , followed by a
	NumberVector x_i(n), x_l(n), x_u(n);
	for(j = 0; j < m; j++)
	{	x_i[j] = 0.;       // initial y(t) for optimization
		x_l[j] = -1.0e19;  // no lower limit
		x_u[j] = +1.0e19;  // no upper limit
	}
	for(j = 0; j < na; j++)
	{	x_i[m + j ] = .5;       // initiali a for optimization
		x_l[m + j ] =  -1.e19;  // no lower limit
		x_u[m + j ] =  +1.e19;  // no upper
	}
	// all of the difference equations are constrained to the value zero
	NumberVector g_l(m), g_u(m);
	for(I = 0; I < m; I++)
	{	g_l[I] = 0.;
		g_u[I] = 0.;
	}
	// derived class object
	
	for(size_t icase = 0; icase <= 1; icase++)
	{	// Retaping is slow, so only do icase = 0 for large values 
		// of ns.
		bool retape = icase != 0;

		// object defining the objective f(x) and constraints g(x)
		FG_info fg_info(retape);

		// create the CppAD Ipopt interface
		ipopt_cppad_solution solution;
		Ipopt::SmartPtr<Ipopt::TNLP> cppad_nlp = new ipopt_cppad_nlp(
			n, m, x_i, x_l, x_u, g_l, g_u, &fg_info, &solution
		);

		// Create an Ipopt application
		using Ipopt::IpoptApplication;
		Ipopt::SmartPtr<IpoptApplication> app = new IpoptApplication();

		// turn off any printing
		app->Options()->SetIntegerValue("print_level", -2);

		// maximum number of iterations
		app->Options()->SetIntegerValue("max_iter", 30);

		// approximate accuracy in first order necessary conditions;
		// see Mathematical Programming, Volume 106, Number 1, 
		// Pages 25-57, Equation (6)
		app->Options()->SetNumericValue("tol", 1e-9);

		// Derivative testing is very slow for large problems
		// so comment this out if you use a large value for ns.
		app->Options()-> SetStringValue(
			"derivative_test", "second-order"
		);

		// Initialize the application and process the options
		Ipopt::ApplicationReturnStatus status = app->Initialize();
		ok    &= status == Ipopt::Solve_Succeeded;

		// Run the application
		status = app->OptimizeTNLP(cppad_nlp);
		ok    &= status == Ipopt::Solve_Succeeded;

		// Check some of the return values
		Number rel_tol = 1e-2; // use a larger value of ns
		Number abs_tol = 1e-2; // to get better accuracy here.
		Number check_a[] = {a0, a1, a2}; // see the y_one function
		for(j = 0; j < na; j++)
		{
			ok &= CppAD::NearEqual( 
				check_a[j], solution.x[m+j], rel_tol, abs_tol
			);
		}
		rel_tol = 1e-9;
		abs_tol = 1e-9;

		// split out return values
		NumberVector a(na), y_0(ny), y_1(ny), y_2(ny);
		for(j = 0; j < na; j++)
			a[j] = solution.x[m+j];
		y_0 = eval_F(a);
		for(j = 0; j < ny; j++)
		{	y_1[j] = solution.x[j];
			y_2[j] = solution.x[ny + j];
		} 
		//
		// check the initial difference equation
		NumberVector G_0 = eval_G(y_0, a);
		NumberVector G_1 = eval_G(y_1, a);
		Number dt = (td[1] - td[0]) / Number(ns);
		for(j = 0; j < ny; j++)
		{	Number check = y_1[j] - y_0[j] - (G_1[j]+G_0[j])*dt/2;
			ok &= CppAD::NearEqual( check, 0., rel_tol, abs_tol);
		}
		//
		// check the second difference equation
		NumberVector G_2 = eval_G(y_2, a);
		if( ns == 1 )
			dt = (td[2] - td[1]) / Number(ns);
		for(j = 0; j < ny; j++)
		{	Number check = y_2[j] - y_1[j] - (G_2[j]+G_1[j])*dt/2;
			ok &= CppAD::NearEqual( check, 0., rel_tol, abs_tol);
		}
		//
		// check the objective function (specialized to this case)
		Number check = 0.;
		NumberVector y_M(ny);
		for(size_t k = 0; k < nd; k++)
		{	for(j = 0; j < ny; j++)
			{	size_t M = (k + 1) * ns;
				y_M[j] =  solution.x[(M-1) * ny + j];
			}
			check += eval_H<Number>(k, y_M, a);
		}
		Number obj_value = solution.obj_value;
		ok &= CppAD::NearEqual(check, obj_value, rel_tol, abs_tol);
	}
	return ok;
}
// END PROGRAM
