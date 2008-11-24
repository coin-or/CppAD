/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include "../ipopt_cppad/ipopt_cppad_nlp.hpp"

namespace { // Begin empty namespace
// ---------------------------------------------------------------------------
class FG_retape : public ipopt_cppad_fg_info
{
public:
	// derived class part of constructor
	FG_retape(void)
	{ }
	// Evaluation of the objective f(x), and constraints g(x)
	// using an Algorithmic Differentiation (AD) class.
	ADVector eval_r(size_t k, const ADVector&  x)
	{	ADVector fg(2);

		// f(x)
		if( x[0] >= 1. )
			fg[0] = .5 * (x[0] * x[0] + x[1] * x[1]);
		else	fg[0] = x[0] + .5 * x[1] * x[1]; 
		// g (x)
		fg[1] = x[0];

		return fg;
	}
	bool retape(size_t k)
	{	return true; }
}; 

bool ipopt_cppad_retape(void)
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
	FG_retape fg_retape;
	ipopt_cppad_fg_info *fg_info = &fg_retape;  

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
// ---------------------------------------------------------------------------
/*
This solve the same problem as 
../ipopt_cppad/ipopt_cppad_simple.cpp (repository revision 
1276) in a convoluted way in order to test the representation code.
*/
class FG_K_gt_1 : public ipopt_cppad_fg_info
{
private:
	bool retape_;
public:
	// derived class part of constructor
	FG_K_gt_1(bool retape)
	: retape_ (retape)
	{ }
	// Evaluation of the objective f(x), and constraints g(x)
	// using an Algorithmic Differentiation (AD) class.
	ADVector eval_r(size_t k, const ADVector&  u)
	{

		// Fortran style indexing 
		ADNumber x1 = u[3];
		ADNumber x2 = u[2];
		ADNumber x3 = u[1];
		ADNumber x4 = u[0];
		if( k == 0 )
		{	ADVector r(1);
			// f(x)
			r[0] = x1 * x4 * (x1 + x2 + x3) + x3;
			return r;
		}
		ADVector r(2);
		// g_1 (x)
		r[0] = x1 * x2 * x3 * x4;
		// g_2 (x)
		r[1] = x1 * x1 + x2 * x2 + x3 * x3 + x4 * x4;
		return r;
	}
	bool retape(size_t k)
	{	return retape_; }
	size_t number_functions(void)
	{	return 2; }
	size_t domain_size(size_t k)
	{	return 4; }
	size_t range_size(size_t k)
	{	if( k == 0 )
			return 1;
		return 2;
	}
	size_t number_terms(size_t k)
	{	return 1; }
	void index(size_t k, size_t ell, SizeVector& I, SizeVector& J)
	{
		if( k == 0 )
			I[0] = 0;
		else
		{	I[0] = 1;
			I[1] = 2; 
		}
		// reverse the order of the variables in u from that in x
		for(size_t j = 0; j < 4; j++)
			J[j] = 3-j;
	}
};
	
bool ipopt_cppad_K_gt_1(void)
{	bool ok = true;
	size_t j;


	// number of independent variables (domain dimension for f and g)
	size_t n = 4;  
	// number of constraints (range dimension for g)
	size_t m = 2;
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

	// known solution to check against
	double check_x[]   = { 1.000000, 4.743000, 3.82115, 1.379408 };

	size_t icase;
	for(icase = 0; icase <= 1; icase++)
	{	// Should ipopt_cppad_nlp retape the operation sequence for
		// every new x. Can test both true and false cases because 
		// the operation sequence does not depend on x (for this case).
		bool retape = bool(icase);

		// check case where upper and lower limits are equal
		if( icase == 1 )
		{	x_l[2] = check_x[2];
			x_u[2] = check_x[2];
		}

		// object in derived class
		FG_K_gt_1 my_fg_info(retape);
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
		double check_z_l[] = { 1.087871, 0.,       0.,      0.       };
		double check_z_u[] = { 0.,       0.,       0.,      0.       };
		double rel_tol     = 1e-6;  // relative tolerance
		double abs_tol     = 1e-6;  // absolute tolerance
		for(j = 0; j < n; j++)
		{	ok &= CppAD::NearEqual(
			check_x[j],   solution.x[j],   rel_tol, abs_tol
			);
			ok &= CppAD::NearEqual(
			check_z_l[j], solution.z_l[j], rel_tol, abs_tol
			);
			ok &= CppAD::NearEqual(
			check_z_u[j], solution.z_u[j], rel_tol, abs_tol
			);
		}
	}

	return ok;
}
// ---------------------------------------------------------------------------
/*
f(x)    = x[1]; k=0, ell=0, I[0] = 0, J[0] = 1
g_0 (x) = x[0]; k=0, ell=1, I[0] = 1, J[0] = 0
g_1 (x) = x[1]; k=0, ell=2, I[0] = 2, J[0] = 1

minimize   f(x)
subject to -1 <= g_0(x)  <= 0
            0 <= g_1 (x) <= 1

The solution is x[1] = 0 and x[0] arbitrary.
*/

namespace
{
class FG_J_changes : public ipopt_cppad_fg_info
{
private:
	bool retape_;
public:
	// constructor
	FG_J_changes(bool retape)
	: retape_ (retape)
	{ }
	size_t number_functions(void)
	{	return 1; }
	size_t domain_size(size_t k)
	{	size_t q;
		switch(k)
		{	case 0:  q = 1;  break;  
			default: assert(0);
		}
		return q;
	}
	size_t range_size(size_t k)
	{	size_t p;
		switch(k)
		{	case 0:  p = 1;  break;
			default: assert(0);
		}
		return p;
	}
	size_t number_terms(size_t k)
	{	size_t L;
		switch(k)
		{	case 0:  L = 3;   break;
			default: assert(0);
		}
		return L;
	}
	void index(size_t k, size_t ell, SizeVector&I, SizeVector& J)
	{	assert( I.size() >= 1 );
		assert( J.size() >= 1 );
		I[0] = ell;
		if( ell == 0 )
		{	J[0] = 1;
			return;
		}
		J[0] = ell - 1;
		return;
	}
	// retape function
	bool retape(size_t k)
	{	return retape_; }
	ADVector eval_r(size_t k, const ADVector&  u)
	{
		assert( u.size() == 1 );
		ADVector r(1);
		r[0] = u[0] ; 
		return r;
	}
};
}

bool ipopt_cppad_J_changes(void)
{
	bool ok = true;
	// number of independent variables (domain dimension for f and g)
	size_t n = 2;
	// number of constraints (range dimension for g)
	size_t m = 2; 
	// initial value of the independent variables
	NumberVector x_i(n);
	NumberVector x_l(n);
	NumberVector x_u(n);

	size_t i = 0;
	for(i = 0; i < n; i++)
	{	x_i[i] = 0.;
		x_l[i] = -1.0;
		x_u[i] = +1.0;
	}

	// lower and upper limits for g
	NumberVector g_l(m);
	NumberVector g_u(m);
	g_l[0] = -1; g_u[0] = 0.;
	g_l[1] = 0.; g_u[1] = 1.;

	// object for evaluating function
	bool retape = false;
	FG_J_changes my_fg_info(retape);
	ipopt_cppad_fg_info *fg_info = &my_fg_info;

	ipopt_cppad_solution solution;
	Ipopt::SmartPtr<Ipopt::TNLP> cppad_nlp = new ipopt_cppad_nlp(
		n, m, x_i, x_l, x_u, g_l, g_u, fg_info, &solution
	);

	// Create an instance of the IpoptApplication
	using Ipopt::IpoptApplication;
	Ipopt::SmartPtr<IpoptApplication> app = new IpoptApplication();

	// turn off any printing
	app->Options()->SetIntegerValue("print_level", -2);

	// approximate accuracy in first order necessary conditions;
	// see Mathematical Programming, Volume 106, Number 1,
	// Pages 25-57, Equation (6)
	app->Options()->SetNumericValue("tol", 1e-9);
	app->Options()-> SetStringValue("derivative_test", "second-order");

	// Initialize the IpoptApplication and process the options
	Ipopt::ApplicationReturnStatus status = app->Initialize();
	ok    &= status == Ipopt::Solve_Succeeded;

	// Run the IpoptApplication
	status = app->OptimizeTNLP(cppad_nlp);
	ok    &= status == Ipopt::Solve_Succeeded;

	/*
	 Check solution status
	 */
	ok &= solution.status == ipopt_cppad_solution::success;
	ok &= CppAD::NearEqual(solution.x[1], 0., 1e-6, 1e-6);

	return ok;
}
// ---------------------------------------------------------------------------

} // End empty namespace

bool ipopt_cppad(void)
{	bool ok = true;
	ok &= ipopt_cppad_retape();
	ok &= ipopt_cppad_K_gt_1();
	ok &= ipopt_cppad_J_changes();
	return ok;
}
