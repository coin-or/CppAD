/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin ipopt_ode_fast.cpp$$
$spell
	ipopt_cppad_nlp
	nd
	ny
	na
	ns
$$

$section ODE Fitting Using Fast Representation: Source Code$$

$index ipopt_cppad_nlp, ode example source$$
$index ode, ipopt_cppad_nlp example source$$
$index example, ipopt_cppad_nlp ode source$$
$index source, ipopt_cppad_nlp ode example$$

$code
$verbatim%ipopt_cppad/ipopt_ode_fast.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/

// BEGIN PROGRAM
# include "ipopt_cppad_nlp.hpp"

namespace {
	//------------------------------------------------------------------
	// This section of the code is the same in ipopt_ode_simple.cpp
	// simulated data
	typedef Ipopt::Number Number;
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
	// Number of time grid points for each measurement interval
	size_t N[] = { 0,            5,          5,          5,          5  }; 
	// S[i] = N[0] + ... + N[i]
	size_t S[] = { 0,            5,         10,         15,         20  }; 
	// Number of measurement values
	size_t Nz  = sizeof(z) / sizeof(z[0]) - 1;
	// Number of components in the function y(t, a)
	size_t Ny  = 2;
	// Number of components in the vectro a
	size_t Na  = 3;

	// Initial Condition function, F(a) = y(t, a) at t = 0
	// (for this particular example)
	template <class Vector>
	Vector eval_F(const Vector &a)
	{	Vector F(Ny);
		// y_0 (t) = a[0]*exp(-a[1] * t)
		F[0] = a[0];
		// y_1 (t) = 
		// a[0]*a[1]*(exp(-a[2] * t) - exp(-a[1] * t))/(a[1] - a[2])
		F[1] = 0.; 
		return F;
	}
	// G(y, a) =  \partial_t y(t, a); i.e. the differential equation
	// (for this particular example)
	template <class Vector>
	Vector eval_G(const Vector &y , const Vector &a)
	{	Vector G(Ny);
		// y_0 (t) = a[0]*exp(-a[1] * t)
		G[0] = -a[1] * y[0];  
		// y_1 (t) = 
		// a[0]*a[1]*(exp(-a[2] * t) - exp(-a[1] * t))/(a[1] - a[2])
		G[1] = +a[1] * y[0] - a[2] * y[1]; 
		return G;
	} 
	// H(i, y, a) = contribution to objective at i-th data point
	// (for this particular example)
	template <class Scalar, class Vector>
	Scalar eval_H(size_t i, const Vector &y, const Vector &a)
	{	// This particular H is for a case where y_1 (t) is measured
		Scalar diff = z[i] - y[1];
	 	return diff * diff;
	}

	//------------------------------------------------------------------
	// This section of the code is different in ipopt_ode_simple.cpp
size_t nd = sizeof(s)/sizeof(s[0]) - 1; // number of actual data values
size_t ny = 2;   // dimension of y(t, a) 
size_t na = 3;   // dimension of a 
size_t ns = 5;   // number of grid intervals between each data value 

	// ------------------------------------------------------------------
	class FG_info : public ipopt_cppad_fg_info
	{
	private:
		bool retape_;
	public:
		// derived class part of constructor
		FG_info(bool retape)
		: retape_ (retape)
		{ }
		// r^k for k = 0, 1, ..., Nz-1 used for measurements
		// r^k for k = Nz              use for initial condition
		// r^k for k = Nz+1, ..., 2*Nz used for trapezoidal approx
		size_t number_functions(void)
		{	return Nz + 1 + Nz; }
		ADVector eval_r(size_t k, const ADVector &u)
		{	size_t j;
			ADVector y(Ny), a(Na);
			// objective function --------------------------------
			if( k < Nz )
			{	// used for measurement with index k+1
				ADVector r(1); // return value is a scalar
				size_t j;
				// u is [y( s[k+1] ) , a] 
				for(j = 0; j < Ny; j++)
					y[j] = u[j];
				for(j = 0; j < Na; j++)
					a[j] = u[Ny + j];
				r[0] = eval_H<ADNumber>(k+1, y, a);
				return r;
			}
			// initial condition ---------------------------------
			if( k == Nz )
			{	ADVector r(Ny), F(Ny);
				// u is [y(t), a] at t = 0 
				for(j = 0; j < Ny; j++)
					y[j] = u[j];
				for(j = 0; j < Na; j++)
					a[j] = u[Ny + j];
				F    = eval_F(a);
				for(j = 0; j < Ny; j++)
					r[j]   = y[j] - F[j]; 
				return  r;
			}
			// trapezoidal approximation -------------------------
			ADVector ym(Ny), G(Ny), Gm(Ny), r(Ny);
			// r^k for k = Nz+1, ... , 2*Nz 
			// interval between data samples
			Number T = s[k-Nz] - s[k-Nz-1];
			// integration step size
			Number dt = T / Number( N[k-Nz] );
			// u = [ y(t[i-1], a) , y(t[i], a), a ) 
			for(j = 0; j < Ny; j++)
			{	ym[j] = u[j];
				y[j]  = u[Ny + j];
			}
			for(j = 0; j < Na; j++)
				a[j] = u[2 * Ny + j];
			Gm  = eval_G(ym, a);
			G   = eval_G(y,  a);
			for(j = 0; j < Ny; j++)
				r[j] = y[j] - ym[j] - (G[j] + Gm[j]) * dt / 2.;
			return r;
		}
		// The operations sequence for r_eval does not depend on u,
		// hence retape = false should work and be faster.
		bool retape(size_t k)
		{	return retape_; }
		// size of the vector u in eval_r
		size_t domain_size(size_t k)
		{	if( k < Nz )
				return Ny + Na;   // objective function
			if( k == nd )
				return Ny + Na;  // initial value constraint
			return 2 * Ny + Na;      // trapezodial constraints
		}
		// size of the return value from eval_r
		size_t range_size(size_t k)
		{	if( k < Nz )
				return 1;
			return Ny; 
		}
		// number of terms that use this value of k
		size_t number_terms(size_t k)
		{	if( k <= Nz )
				return 1;  // r^k used once for k <= Nz
			// r^k used N[k-Nz] times for k > Nz
			return N[k-Nz];
		}
		void index(size_t k, size_t ell, SizeVector& I, SizeVector& J)
		{	size_t i, j;
			// # of components of x corresponding to values for y
			size_t ny_inx = (S[Nz] + 1) * Ny;
			// objective function -------------------------------
			if( k < nd )
			{	// index in fg corresponding to objective	
				I[0] = 0;
				// u = [ y(t, a) , a ]
				// The first ny components of u is y(t) at 
				// 	t = s[k+1] = t[S[k+1]]
				// x indices corresponding to this value of y
				for(j = 0; j < Ny; j++)
					J[j] = S[k + 1] * Ny + j;
				// components of x correspondig to a
				for(j = 0; j < na; j++)
					J[Ny + j] = ny_inx + j; 
				return;
			}
			// initial conditions --------------------------------
			if( k == nd )
			{	// index in fg for inidial condition constraint
				for(j = 0; j < Ny; j++)
					I[j] = 1 + j;
				// u = [ y(t, a) , a ] where t = 0
				// x indices corresponding to this value of y
				for(j = 0; j < Ny; j++)
					J[j] = j;
				// following that, u contains the vector a 
				for(j = 0; j < Na; j++)
					J[Ny + j] = ny_inx + j;
				return;
			}
			// trapoziodal approximation -------------------------
			// index of first grid point in this approximation
			i = S[k - Nz - 1]  + ell;
			// There are Ny difference equations for each time 
			// point.  Add one for the objective function, and Ny
			// for the initial value constraints.
			for(j = 0; j < Ny; j++)
				I[j] = 1 + Ny + i * Ny + j;
			// u = [ y(t, a) , y(t+dt, a) , a ] at t = t[i]
			for(j = 0; j < Ny; j++)
			{	J[j]      = i * Ny  + j; // y^i indices
				J[Ny + j] = J[j] + Ny;   // y^{i+1} indices
			}
			for(j = 0; j < na; j++)
				J[2 * Ny + j] = ny_inx + j; // a indices
		} 
	};

} // End empty namespace


// ---------------------------------------------------------------------------
bool ipopt_ode_fast(void)
// The rest of the code is the same as in ipopt_ode_simple.cpp
{	bool ok = true;
	size_t i, j;

	// number of components of x corresponding to values for y
	size_t ny_inx = (S[Nz] + 1) * Ny;
	// number of constraints (range dimension of g)
	size_t m      = ny_inx;
	// number of components in x (domain dimension for f and g)
	size_t n      = ny_inx + Na;
	// the argument vector for the optimization is 
	// y(t) at t[0] , ... , t[S[Nz]] , followed by a
	NumberVector x_i(n), x_l(n), x_u(n);
	for(j = 0; j < ny_inx; j++)
	{	x_i[j] = 0.;       // initial y(t) for optimization
		x_l[j] = -1.0e19;  // no lower limit
		x_u[j] = +1.0e19;  // no upper limit
	}
	for(j = 0; j < Na; j++)
	{	x_i[ny_inx + j ] = .5;       // initiali a for optimization
		x_l[ny_inx + j ] =  -1.e19;  // no lower limit
		x_u[ny_inx + j ] =  +1.e19;  // no upper
	}
	// all of the difference equations are constrained to the value zero
	NumberVector g_l(m), g_u(m);
	for(i = 0; i < m; i++)
	{	g_l[i] = 0.;
		g_u[i] = 0.;
	}
	// derived class object
	
	for(size_t icase = 0; icase <= 1; icase++)
	{	// Retaping is slow, so only do icase = 0 for large values 
		// of N[].
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
		app->Options()->SetIntegerValue("print_level", 0);

		// maximum number of iterations
		app->Options()->SetIntegerValue("max_iter", 30);

		// approximate accuracy in first order necessary conditions;
		// see Mathematical Programming, Volume 106, Number 1, 
		// Pages 25-57, Equation (6)
		app->Options()->SetNumericValue("tol", 1e-9);

		// Derivative testing is very slow for large problems
		// so comment this out if you use a large value for N[].
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
		NumberVector a(Na), y_0(Ny), y_1(Ny), y_2(Ny);
		for(j = 0; j < Na; j++)
			a[j] = solution.x[ny_inx+j];
		for(j = 0; j < Ny; j++)
		{	y_0[j] = solution.x[j];
			y_1[j] = solution.x[Ny + j];
			y_2[j] = solution.x[2 * Ny + j];
		} 

		// Check some of the return values
		Number rel_tol = 1e-2; // use a larger value of N[]
		Number abs_tol = 1e-2; // to get better accuracy here.
		Number check_a[] = {a0, a1, a2}; // see the y_one function
		for(j = 0; j < Na; j++)
		{
			ok &= CppAD::NearEqual( 
				check_a[j], a[j], rel_tol, abs_tol
			);
		}
		rel_tol = 1e-9;
		abs_tol = 1e-9;

		// check the initial value constraint
		NumberVector F = eval_F(a);
		for(j = 0; j < Ny; j++)
			ok &= CppAD::NearEqual(F[j], y_0[j], rel_tol, abs_tol);

		// check the first trapezoidal equation
		NumberVector G_0 = eval_G(y_0, a);
		NumberVector G_1 = eval_G(y_1, a);
		Number dt = (s[1] - s[0]) / Number(N[1]);
		Number check;
		for(j = 0; j < Ny; j++)
		{	check = y_1[j] - y_0[j] - (G_1[j]+G_0[j])*dt/2;
			ok &= CppAD::NearEqual( check, 0., rel_tol, abs_tol);
		}
		//
		// check the second trapezoidal equation
		NumberVector G_2 = eval_G(y_2, a);
		if( N[1] == 1 )
			dt = (s[2] - s[1]) / Number(N[2]);
		for(j = 0; j < Ny; j++)
		{	check = y_2[j] - y_1[j] - (G_2[j]+G_1[j])*dt/2;
			ok &= CppAD::NearEqual( check, 0., rel_tol, abs_tol);
		}
		//
		// check the objective function (specialized to this case)
		check = 0.;
		NumberVector y_M(Ny);
		for(size_t k = 0; k < Nz; k++)
		{	for(j = 0; j < Ny; j++)
			{	size_t M = S[k + 1];
				y_M[j] =  solution.x[M * Ny + j];
			}
			check += eval_H<Number>(k + 1, y_M, a);
		}
		Number obj_value = solution.obj_value;
		ok &= CppAD::NearEqual(check, obj_value, rel_tol, abs_tol);
	}
	return ok;
}
// END PROGRAM
