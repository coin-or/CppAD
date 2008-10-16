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
	\sum_{j=1}^N H_j [ j ,  y( t_j , a ) , a ] \;
		{\rm with \; respect \; to} \; a \in \R^p
\] $$
where $latex y(t)$$ is the solution of the ODE
$latex \[
\begin{array}{rcl}
	y(0, a)  & = &  F(a)                 \\
	y'(t, a) & = & G[ t , y(t, a) , a ]
\end{array}
\] $$

$head Specific Case$$
Almost all the code below is for the general problem but some of it
for a specific case defined by the function $code y_one(t)$$.

$head Source Code$$
$code
$verbatim%example/ipopt_cppad_ode.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$


$end
------------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# include "ipopt_cppad_nlp.hpp"

// include a definition for Number.
typedef Ipopt::Number Number;

// ---------------------------------------------------------------------------
namespace { // Begin empty namespace 

size_t ny = 2;   // dimension of y(t, a) for this case
size_t na = 3;   // dimension of a for this case 
size_t ng = 5;   // number of grid intervals with in each data interval

// parameter vector component values used to simulate data
Number a0 = 1.;
Number a1 = 2.; 
Number a2 = 1.; 

// function used to simulate data
Number y_one(Number t)
{	Number y_1 =  a0 * a1 * (exp(-a2 * t) - exp(-a1 * t)) / (a1 - a2);
	return y_1;
}

// time points were we have data
double td[] = {        .5,         1.,         1.5  }; 
// Simulated data is for the case (no noise)
double yd[] = {  y_one(.5),  y_one(1.),  y_one(1.5) };
// number of data values
size_t nd   = sizeof(yd) / sizeof(yd[0]);

// F(a) = y(0, a); i.e., initial condition
template <class Vector>
Vector eval_F(Vector a)
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
Vector eval_G(Vector y , Vector a)
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
Scalar eval_H(size_t k, Vector y, Vector a)
{	// This particular H is for a case where y_1 (t) is measured
	Scalar diff = yd[k] - y[1];
 	return diff * diff;
}

/* 
-----------------------------------------------------------------------------
Time Grid: tg

For k = 0,...,nd - 1, 
	dtg[k] = (td[k] - td[k-1]) / ng 
where td[-1] is interpreted as zero. 

For k = 0 , ... , nd-1 and ell = 0 , ... , ng-1,
        tg[k * ng + ell] = td[k-1] + dtg[k] * ell

-----------------------------------------------------------------------------
Optimization Argument: 
	x = [ y_1 , y_2 , ... , y_{ng * nd} , a ]

The initial value for y(t) at t = tg[0] is denoted by
	y_0 = F(a)

For J = 1 , ... , ng * nd, the value of y(t) at t = tg[J] is denoted by
	y_J = ( x[(J-1)*ny] , ... , x[J*ny -1] ) 

We use the following difference approximation to solution of ODE
	0 = y_{J+1} - y_J - [G(y_{J+1}, a) + G(y_J , a)] * dtg[k] / 2
where J > 0 and k is chosen so that 
	td[k-1] <= tg[J] < td[k].
Note that td is the time grid for the data and tg is the time grid
for the disctetization of the ODE.

-----------------------------------------------------------------------------
Representation Index: k

For 0 <= k < nd, r_k (u) is for two sided difference equations. Note that 
there is one less such equation for k = 0 (because k = nd) is used for the
initial difference equation.  

For k = nd, r_k (u) is used for the initial difference equation.

For k = nd + 1 , ... , nd + nd, r_k (u) is used for the objective function 
contribution corresponding to the data pair 
	( td[k-nd-1], yd[k-nd-1] ).
*/
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
			// u is [y(t) , a] where t = td[ell]
			ADVector y_J(ny), a(na);
			for(j = 0; j < ny; j++)
				y_J[j] = u[j];
			for(j = 0; j < na; j++)
				a[j] = u[ny + j];
			r[0] = eval_H<ADNumber>(k - nd - 1, y_J, a);
			return r;
		}
		ADVector y_J(ny), y_J1(ny), a(na);
		Number dtg;
		if( k == nd )
		{	// u = [y_J1 , a] where y_J1 is y(t) at t = tg[1]
			for(j = 0; j < ny; j++)
				y_J1[j] = u[j];
			for(j = 0; j < na; j++)
				a[j] = u[ny + j];
			// y_J is value of y(t) at t = tg[0]
			y_J = eval_F(a);
			// size of subinterval
			dtg = (td[0] - 0.) / Number(ng);
		}
		else
		{	// u = [y_J, y_J1, a] where y_J is y(t) at 
			// t = tg[ k * ng + ell + 1 ] for k = 0 case 
			// t = tg[ k * ng + ell ]     for 0 < k < nd cases
			for(j = 0; j < ny; j++)
			{	y_J[j]  = u[j];
				y_J1[j] = u[ny + j];
			}
			for(j = 0; j < na; j++)
				a[j] = u[2 * ny + j];
			if( k == 0 )
				dtg = (td[0] - 0.) / Number(ng);
			else	dtg = (td[k] - td[k-1]) / Number(ng);
		}
		ADVector G_J   = eval_G(y_J,  a);
		ADVector G_J1  = eval_G(y_J1, a);
		ADVector r(ny);
		for(j = 0; j < ny; j++)
			r[j] = y_J1[j] - y_J[j] - (G_J1[j] + G_J[j]) * dtg/2.;
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
			return ng - 1;   // exclude initial difference equation
		return ng;               // number difference equations
	}
	// k is index of time interal between data values
	// ell is index of grid point within data interval
	void index(size_t k, size_t ell, SizeVector& I, SizeVector& J)
	{	size_t i, j;
		// number of constraints, one for each subinterval interval
		// times the number of components in y(t)
		size_t m = nd * ng * ny;
		if( k > nd )
		{	// must use range index zero for the objective function
			I[0] = 0;
			// The first ny components of u is y(t) at 
			// 	t = td[j] = tg[(j+1)*ng]
			// Note that x starts with y(t) at time tg[1]. 
			// Also Note that there are ng subintervals between 
			// each data point.
			for(j = 0; j < ny; j++)
				J[j] = ( (k - nd) * ng - 1) * ny + j;
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
			// (which corresponding to y(t) at tg[1])
			for(j = 0; j < ny; j++)
				J[j] = j;
			// following that, u contains the vector a 
			for(j = 0; j < na; j++)
				J[ny + j] = m + j;
			return;
		}
		// index of first grid point in tg for difference equation
		size_t grid_point; 
		if( k == 0 )
			grid_point = k * ng + ell + 1;
		else	grid_point = k * ng + ell;
		for(j = 0; j < ny; j++)
		{	J[j]          = (grid_point - 1) * ny  + j; // y_J
			J[ny + j]     = J[j] + ny;                  // y_J1
		}
		for(j = 0; j < na; j++)
			J[2 * ny + j] = m + j;                      // a
		// There are ny difference equations for each grid point.
		// (add one for the objective function index).
		for(i = 0; i < ny; i++)
			I[i] = grid_point * ny + 1 + i;
	} 
};

} // End empty namespace
// ---------------------------------------------------------------------------

bool ipopt_cppad_ode(void)
{	bool ok = true;
	size_t j, I;

	// number of constraints (range dimension of g)
	size_t m = nd * ng * ny;
	// number of independent variables (domain dimension for f and g)
	size_t n = m + na;
	// the argument vector for the optimization is 
	// y(t) at t = tg[1] , ... , tg[nd*ng] , followed by a
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
		// of ng.
		bool retape = bool(icase);

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
		// so comment this out if you use a large value for ng.
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
		Number rel_tol = 1e-2; // use a larger value of ng
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
		Number dtg = td[0] / Number(ng);
		for(j = 0; j < ny; j++)
		{	Number check = y_1[j] - y_0[j] - (G_1[j]+G_0[j])*dtg/2;
			ok &= CppAD::NearEqual( check, 0., rel_tol, abs_tol);
		}
		//
		// check the second difference equation
		NumberVector G_2 = eval_G(y_2, a);
		if( ng == 1 )
			dtg = (td[1] - td[0]) / Number(ng);
		for(j = 0; j < ny; j++)
		{	Number check = y_2[j] - y_1[j] - (G_2[j]+G_1[j])*dtg/2;
			ok &= CppAD::NearEqual( check, 0., rel_tol, abs_tol);
		}
		//
		// check the objective function (specialized to this case)
		Number check = 0.;
		NumberVector y_J(ny);
		for(size_t k = 0; k < nd; k++)
		{	for(j = 0; j < ny; j++)
			{	size_t grid_point = (k + 1) * ng;
				y_J[j] =  solution.x[(grid_point-1) * ny + j];
			}
			check += eval_H<Number>(k, y_J, a);
		}
		Number obj_value = solution.obj_value;
		ok &= CppAD::NearEqual(check, obj_value, rel_tol, abs_tol);
	}
	return ok;
}
// END PROGRAM
