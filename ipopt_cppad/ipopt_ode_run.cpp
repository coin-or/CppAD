/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include "ipopt_ode_simple.hpp"
# include "ipopt_ode_fast.hpp"

namespace { // BEGIN empty namespace -----------------------------------------

template <class FG_info>
bool ipopt_ode_case(void)
{	bool ok = true;
	size_t i, j;

	// specify the number of grid points between measurement values
	SizeVector N(Nz+1), S(Nz+1);
	N[0] = S[0] = 0.;
	for(i = 1; i <= Nz; i++)
	{	N[i] = 5;
		S[i] = S[i-1] + N[i];
	}

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
		FG_info fg_info(retape, N);

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
} // END empty namespace ----------------------------------------------------

bool ipopt_ode_run(void)
{	bool ok = true;
	ok &= ipopt_ode_case<FG_simple>();
	ok &= ipopt_ode_case<FG_fast>();
	return ok;
}
