/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include "../example/ipopt_cppad_nlp.hpp"

namespace {

	class my_FG_info : public ipopt_cppad_fg_info
	{
	public:
		// derived class part of constructor
		my_FG_info(void)
		{ }
		// Evaluation of the objective f(x), and constraints g(x)
		// using an Algorithmic Differentiation (AD) class.
		ADVector r_eval(const ADVector&  x)
		{	ADVector fg(2);

			// f(x)
			if( x[0] >= 1. )
				fg[0] = .5 * (x[0] * x[0] + x[1] * x[1]);
			else	fg[0] = x[0] + .5 * x[1] * x[1]; 
			// g (x)
			fg[1] = x[0];

			return fg;
		}
		bool retape(void)
		{	return true; }
	};
}
	
bool ipopt_cppad(void)
{	bool ok = true;
	size_t j;


	// number of independent variables (domain dimension for f and g)
	size_t n = 2;  
	// number of constraints (range dimension for g)
	size_t m = 1;
	// initial value of the independent variables
	NumberVector x_i(n);
	x_i[0] = 2.0;
	x_i[1] = 2.0;
	// lower and upper limits for x
	NumberVector x_l(n);
	NumberVector x_u(n);
	for(j = 0; j < n; j++)
	{	x_l[j] = -10.; x_u[j] = +10.;
	}
	// lower and upper limits for g
	NumberVector g_l(m);
	NumberVector g_u(m);
	g_l[0] = -1.;     g_u[0] = 1.0e19;

	// object in derived class
	my_FG_info my_fg_info;
	ipopt_cppad_fg_info *fg_info = &my_fg_info;  

	// create the Ipopt interface
	ipopt_cppad_solution solution;
	Ipopt::SmartPtr<Ipopt::TNLP> cppad_nlp = new ipopt_cppad_nlp(
		n, m, x_i, x_l, x_u, g_l, g_u, fg_info, &solution
	);

	// Create an instance of the IpoptApplication
	using Ipopt::IpoptApplication;
	Ipopt::SmartPtr<IpoptApplication> app = new IpoptApplication();

	// turn off any printing
	app->Options()->SetIntegerValue("print_level", -2);

	// maximum number of iterations
	app->Options()->SetIntegerValue("max_iter", 10);

	// approximate accuracy in first order necessary conditions;
	// see Mathematical Programming, Volume 106, Number 1, 
	// Pages 25-57, Equation (6)
	app->Options()->SetNumericValue("tol", 1e-9);

	// derivative testing
	app->Options()->
	SetStringValue("derivative_test", "second-order");

	// Initialize the IpoptApplication and process the options
	Ipopt::ApplicationReturnStatus status = app->Initialize();
	ok    &= status == Ipopt::Solve_Succeeded;

	// Run the IpoptApplication
	status = app->OptimizeTNLP(cppad_nlp);
	ok    &= status == Ipopt::Solve_Succeeded;

	/*
 	Check some of the solution values
 	*/
	ok &= solution.status == ipopt_cppad_solution::success;
	//
	double check_x[]   = { -1., 0. };
	double rel_tol     = 1e-6;  // relative tolerance
	double abs_tol     = 1e-6;  // absolute tolerance
	for(j = 0; j < n; j++)
	{	ok &= CppAD::NearEqual(
			check_x[j],   solution.x[j],   rel_tol, abs_tol
		);
	}

	return ok;
}
