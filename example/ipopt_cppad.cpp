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

$head Tracking New and Delete$$
This example uses raw memory arrays that depend on the number of 
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
	// Final solution infromation
	SolverReturn         final_status;
	Number*              final_x;
	Number*              final_z_l;
	Number*              final_z_u;
	Number*              final_g;
	Number*              final_lambda;
	Number               final_obj_value;
	void solution(
		SolverReturn               status     ,
		Index                      n          , 
		const Number*              x          , 
		const Number*              z_L        , 
		const Number*              z_U        ,
		Index                      m          , 
		const Number*              g          , 
		const Number*              lambda     ,
		Number                     obj_value  )
	{
		assert( n == 4 );
		assert( m == 2 );

		Index i, j;

		final_status = status;
		//
		final_x   = CPPAD_TRACK_NEW_VEC(n, final_x);
		final_z_l = CPPAD_TRACK_NEW_VEC(n, final_z_l);
		final_z_u = CPPAD_TRACK_NEW_VEC(n, final_z_u);
		for(j = 0; j < n; j++)
		{	final_x[j]    = x[j];
			final_z_l[j] = z_L[j];
			final_z_u[j] = z_U[j];
		}

		final_g      = CPPAD_TRACK_NEW_VEC(m, final_g);
		final_lambda = CPPAD_TRACK_NEW_VEC(m, final_lambda);
		for(i = 0; i < m; i++)
		{	final_g[i]      = g[i];
			final_lambda[i] = lambda[i];
		}
		final_obj_value = obj_value;
	}
}
	
bool ipopt_cppad(void)
{	bool ok = true;
	Index j;

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
	SmartPtr<TNLP> cppad_nlp = new ipopt_cppad_nlp(
		n, m, x_i, x_l, x_u, g_l, g_u, &fg, retape, solution
	);

	// Create an instance of the IpoptApplication
	SmartPtr<IpoptApplication> app = new IpoptApplication();

	// Change some options
	app->Options()->SetNumericValue("tol", 1e-9);
	app->Options()->SetStringValue("mu_strategy", "adaptive");
	app->Options()->SetIntegerValue("print_level", -2);

	// Initialize the IpoptApplication and process the options
	ApplicationReturnStatus status;
	status = app->Initialize();
	ok    &= status == Solve_Succeeded;

	status = app->OptimizeTNLP(cppad_nlp);

	ok &= CppAD::NearEqual(final_x[0], 1.000000, 1e-6, 1e-6);
	ok &= CppAD::NearEqual(final_x[1], 4.743000, 1e-6, 1e-6);
	ok &= CppAD::NearEqual(final_x[2], 3.821150, 1e-6, 1e-6);
	ok &= CppAD::NearEqual(final_x[3], 1.379408, 1e-6, 1e-6);
	
	CPPAD_TRACK_DEL_VEC(final_x);
	CPPAD_TRACK_DEL_VEC(final_z_l);
	CPPAD_TRACK_DEL_VEC(final_z_u);
	CPPAD_TRACK_DEL_VEC(final_g);
	CPPAD_TRACK_DEL_VEC(final_lambda);

	return ok;
}

// END PROGRAM
