/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*

$begin ipopt_cppad.cpp$$
$spell
	cppad_nlp
	IpoptDir
	CppAD
$$
$index nonlinear, programming$$
$index programming, nonlinear$$
$index ipopt$$

$section Nonlinear Programming Using CppAD and Ipopt$$

$head Purpose$$
This example program demonstrates how to use the class ipopt_cppad_nlp to 
solve the example problem in the Ipopt documentation; i.e., the problem
$latex \[
\begin{array}{lc}
{\rm minimize \; }      &  x_1 * x_4 * (x_1 + x_2 + x_3) + x_3
\\
{\rm subject \; to \; } &  x_1 * x_2 * x_3 * x_4  \geq 25
\\
                        &  x_1^2 + x_2^2 + x_3^2 + x_4^2 = 40
\\
                        &  1 \leq x_1, x_2, x_3, x_4 \leq 5  
\end{array}
\] $$


$head Configuration Requirement$$
This example will be compiled and tested provided that
the value $cref/IpoptDir/InstallUnix/IpoptDir/$$ is specified on the 
$cref/configure/InstallUnix/Configure/$$ command line.

$code
$verbatim%example/ipopt_cppad.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$


$end
*/
// BEGIN PROGRAM

# include <coin/IpIpoptApplication.hpp>
# include "ipopt_cppad_nlp.hpp"

namespace {
	// objective function f(x), and constraints g(x)
	ADVector fg(const ADVector&  x_vec)
	{	ADVector fg_vec(3);

		// Fortran style indexing 
		ADNumber x1 = x_vec[0];
		ADNumber x2 = x_vec[1];
		ADNumber x3 = x_vec[2];
		ADNumber x4 = x_vec[3];
		// f(x)
		fg_vec[0] = x1 * x4 * (x1 + x2 + x3) + x3;
		// g_1 (x)
		fg_vec[1] = x1 * x2 * x3 * x4;
		// g_2 (x)
		fg_vec[2] = x1 * x1 + x2 * x2 + x3 * x3 + x4 * x4;
		return fg_vec;
	}
}
	
bool ipopt_cppad(void)
{	bool ok = true;
	Index j;
	using Ipopt::SmartPtr;
	using Ipopt::IpoptApplication;
	using Ipopt::ApplicationReturnStatus;
	using Ipopt::IpoptCalculatedQuantities;

	// number of independent variables (domain dimension for f and g)
	Index n = 4;  
	// number of constraints (range dimension for g)
	Index m = 2;
	// initial value of the independent variables
	NumberVector x_i(n);
	x_i[0] = 1.0;
	x_i[1] = 5.0;
	x_i[2] = 5.0;
	x_i[3] = 1.0;
	// lower and upper limits for x
	NumberVector x_l(n);
	NumberVector x_u(n);
	for(j = 0; j < n; j++)
	{	x_l[j] = 1.0;
		x_u[j] = 5.0;
	}
	// lower and upper limits for g
	NumberVector g_l(m);
	NumberVector g_u(m);
	g_l[0] = 25.0;     g_u[0] = 1.0e19;
  	g_l[1] = 40.0;     g_u[1] = 40.0;
	// does the ADNumber sequence of operations depend on the value of x
	bool retape = true;
	//
	ipopt_cppad_solution solution;
	SmartPtr<Ipopt::TNLP> cppad_nlp = new ipopt_cppad_nlp(
		n, m, x_i, x_l, x_u, g_l, g_u, &fg, retape, &solution
	);

	// Create an instance of the IpoptApplication
	SmartPtr<IpoptApplication> app = new IpoptApplication();

	// turn off any printing
	app->Options()->SetIntegerValue("print_level", -2);

	// maximum number of iterations
	app->Options()->SetIntegerValue("max_iter", 10);

	// approximate accuracy in first order necessary conditions;
	// see Mathematical Programming, Vol 106, Number 1, Pages 25-57, 
	// Equation (6)
	app->Options()->SetNumericValue("tol", 1e-9);

	// Initialize the IpoptApplication and process the options
	ApplicationReturnStatus status;
	status = app->Initialize();
	ok    &= status == Ipopt::Solve_Succeeded;

	// Run the IpoptApplication
	status = app->OptimizeTNLP(cppad_nlp);
	ok    &= status == Ipopt::Solve_Succeeded;


	/*
 	Get isome of the solution values
 	*/
	ok &= solution.status == Ipopt::SUCCESS;
	//
	double check_x[]   = { 1.000000, 4.743000, 3.82115, 1.379408 };
	double check_z_l[] = { 1.087871, 0.,       0.,      0.       };
	double check_z_u[] = { 0.,       0.,       0.,      0.       };
	using CppAD::NearEqual;
	for(j = 0; j < n; j++)
	{	ok &= NearEqual(check_x[j],   solution.x[j],   1e-6, 1e-6);
		ok &= NearEqual(check_z_l[j], solution.z_l[j], 1e-6, 1e-6);
		ok &= NearEqual(check_z_u[j], solution.z_u[j], 1e-6, 1e-6);
	}

	return ok;
}

// END PROGRAM
