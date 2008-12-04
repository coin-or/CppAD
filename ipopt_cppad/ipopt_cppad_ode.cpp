/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

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
	double s[] = { 0.0,        0.5,        1.0,        1.5,        2.0 }; 
	// Simulated data for case with no noise (first point is not used)
	double z[] = { 0.0,  y_one(0.5), y_one(1.0), y_one(1.5), y_one(2.0) };
}
// ---------------------------------------------------------------------------
namespace { // Begin empty namespace 

size_t nd = sizeof(s)/sizeof(s[0]) - 1; // number of actual data values
size_t ny = 2;   // dimension of y(t, a) 
size_t na = 3;   // dimension of a 
size_t ns = 5;   // number of grid intervals between each data value 

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
	G[1] = +a[1] * y[0] - a[2] * y[1]; 
	return G;
} 
// H(k, y, a) = contribution to objective at k-th data point
template <class Scalar, class Vector>
Scalar eval_H(size_t k, const Vector &y, const Vector &a)
{	// This particular H is for a case where y_1 (t) is measured
	Scalar diff = z[k] - y[1];
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
	// r^k for k = 0, 1, ..., nd-1 corresponds to the data value
	// r^k for k = nd corresponds to initial condition
	// r^k for k = nd+1 , ... , 2*nd is used for trapezoidal approximation
	size_t number_functions(void)
	{	return nd + 1 + nd; }
	ADVector eval_r(size_t k, const ADVector &u)
	{	size_t j;
		ADVector y_M(ny), a(na);
		if( k < nd )
		{	// r^k for k = 0, ... , nd-1
			// We use a differnent k for each data point
			ADVector r(1); // return value is a scalar
			size_t j;
			// u is [y( s[k+1] ) , a] 
			for(j = 0; j < ny; j++)
				y_M[j] = u[j];
			for(j = 0; j < na; j++)
				a[j] = u[ny + j];
			r[0] = eval_H<ADNumber>(k+1, y_M, a);
			return r;
		}
		if( k == nd )
		{	// r^k for k = nd corresponds to initial condition
			ADVector r(ny), F(ny);
			// u is [y(0), a] 
			for(j = 0; j < na; j++)
				a[j] = u[ny + j];
			F    = eval_F(a);
			for(j = 0; j < ny; j++)
			{	y_M[j] = u[j];
				// y(0) - F(a)
				r[j]   = y_M[j] - F[j]; 
			}
			return  r;
		}
		// r^k for k = nd+1, ... , 2*nd
		// corresponds to trapezoidal approximations in the 
		// data interval [ s[k-nd] , s[k-nd-1] ]
		ADVector y_M1(ny);
		// u = [y_M, y_M1, a] where y_M is y(t) at 
		// t = t[ (k-nd-1) * ns + ell ] 
		for(j = 0; j < ny; j++)
		{	y_M[j]  = u[j];
			y_M1[j] = u[ny + j];
		}
		for(j = 0; j < na; j++)
			a[j] = u[2 * ny + j];
		Number dt      = (s[k-nd] - s[k-nd-1]) / Number(ns);
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
	{	if( k < nd )
			return ny + na;   // objective function
		if( k == nd )
			return ny + na;  // initial value constraint
		return 2 * ny + na;      // trapezodial constraints
	}
	// size of the vector r in eval_r
	size_t range_size(size_t k)
	{	if( k < nd )
			return 1;
		return ny; 
	}
	size_t number_terms(size_t k)
	{	if( k <= nd )
			return 1;     // r^k used once for k <= nd
		return ns;            // r^k used ns times for k > nd
	}
	void index(size_t k, size_t ell, SizeVector& I, SizeVector& J)
	{	size_t i, j;
		// number of components of x corresponding to value of y
		size_t ny_inx = (nd * ns + 1) * ny;
		if( k < nd )
		{	// r^k for k = 0 , ... , nd-1 corresponds to objective
			I[0] = 0;
			// The first ny components of u is y(t) at 
			// 	t = s[k+1] = t[(k+1)*ns]
			// components of x corresponding to this value of y
			for(j = 0; j < ny; j++)
				J[j] = (k + 1) * ns * ny + j;
			// components of x correspondig to a
			for(j = 0; j < na; j++)
				J[ny + j] = ny_inx + j; 
			return;
		}
		if( k == nd )
		{	// r^k corresponds to initial condition
			for(i = 0; i < ny; i++)
				I[i] = 1 + i;
			// u starts with the first j components of x
			// (which corresponding to y(t) at t[0])
			for(j = 0; j < ny; j++)
				J[j] = j;
			// following that, u contains the vector a 
			for(j = 0; j < na; j++)
				J[ny + j] = ny_inx + j;
			return;
		}
		// index of first grid point in ts for difference equation
		size_t M = (k - nd - 1) * ns + ell;
		for(j = 0; j < ny; j++)
		{	J[j]          = M * ny  + j; // index of y_M in x
			J[ny + j]     = J[j] + ny;   // index of y_M1
		}
		for(j = 0; j < na; j++)
			J[2 * ny + j] = ny_inx + j;                      // a
		// There are ny difference equations for each grid point.
		// Add one for the objective function index, and ny for the
		// initial value constraint.
		for(i = 0; i < ny; i++)
			I[i] = 1 + ny + M * ny + i ;
	} 
};

} // End empty namespace
// ---------------------------------------------------------------------------

bool ipopt_cppad_ode(void)
{	bool ok = true;
	size_t j, I;

	// number of components of x corresponding to value of y
	size_t ny_inx = (nd * ns + 1) * ny;
	// number of constraints (range dimension of g)
	size_t m = ny + nd * ns * ny;
	// number of components in x (domain dimension for f and g)
	size_t n = ny_inx + na;
	// the argument vector for the optimization is 
	// y(t) at t[0] , ... , t[nd*ns] , followed by a
	NumberVector x_i(n), x_l(n), x_u(n);
	for(j = 0; j < ny_inx; j++)
	{	x_i[j] = 0.;       // initial y(t) for optimization
		x_l[j] = -1.0e19;  // no lower limit
		x_u[j] = +1.0e19;  // no upper limit
	}
	for(j = 0; j < na; j++)
	{	x_i[ny_inx + j ] = .5;       // initiali a for optimization
		x_l[ny_inx + j ] =  -1.e19;  // no lower limit
		x_u[ny_inx + j ] =  +1.e19;  // no upper
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

		// split out return values
		NumberVector a(na), y_0(ny), y_1(ny), y_2(ny);
		for(j = 0; j < na; j++)
			a[j] = solution.x[ny_inx+j];
		for(j = 0; j < ny; j++)
		{	y_0[j] = solution.x[j];
			y_1[j] = solution.x[ny + j];
			y_2[j] = solution.x[2 * ny + j];
		} 

		// Check some of the return values
		Number rel_tol = 1e-2; // use a larger value of ns
		Number abs_tol = 1e-2; // to get better accuracy here.
		Number check_a[] = {a0, a1, a2}; // see the y_one function
		for(j = 0; j < na; j++)
		{
			ok &= CppAD::NearEqual( 
				check_a[j], a[j], rel_tol, abs_tol
			);
		}
		rel_tol = 1e-9;
		abs_tol = 1e-9;

		// check the initial value constraint
		NumberVector F = eval_F(a);
		for(j = 0; j < ny; j++)
			ok &= CppAD::NearEqual(F[j], y_0[j], rel_tol, abs_tol);

		// check the first trapezoidal equation
		NumberVector G_0 = eval_G(y_0, a);
		NumberVector G_1 = eval_G(y_1, a);
		Number dt = (s[1] - s[0]) / Number(ns);
		Number check;
		for(j = 0; j < ny; j++)
		{	check = y_1[j] - y_0[j] - (G_1[j]+G_0[j])*dt/2;
			ok &= CppAD::NearEqual( check, 0., rel_tol, abs_tol);
		}
		//
		// check the second trapezoidal equation
		NumberVector G_2 = eval_G(y_2, a);
		if( ns == 1 )
			dt = (s[2] - s[1]) / Number(ns);
		for(j = 0; j < ny; j++)
		{	check = y_2[j] - y_1[j] - (G_2[j]+G_1[j])*dt/2;
			ok &= CppAD::NearEqual( check, 0., rel_tol, abs_tol);
		}
		//
		// check the objective function (specialized to this case)
		check = 0.;
		NumberVector y_M(ny);
		for(size_t k = 0; k < nd; k++)
		{	for(j = 0; j < ny; j++)
			{	size_t M = (k + 1) * ns;
				y_M[j] =  solution.x[M * ny + j];
			}
			check += eval_H<Number>(k + 1, y_M, a);
		}
		Number obj_value = solution.obj_value;
		ok &= CppAD::NearEqual(check, obj_value, rel_tol, abs_tol);
	}
	return ok;
}
