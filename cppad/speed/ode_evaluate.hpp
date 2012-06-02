/* $Id$ */
# ifndef CPPAD_ODE_EVALUATE_INCLUDED
# define CPPAD_ODE_EVALUATE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ode_evaluate$$
$spell
	retaped
	Jacobian
	const
	Cpp
	cppad
	hpp	
	fm
	namespace
	exp
$$

$section Evaluate a Function Defined in Terms of an ODE$$

$index ode_evaluate, function$$
$index function, ode_evaluate$$

$head Syntax$$
$codei%# include <cppad/speed/ode_evaluate.hpp>
%$$
$codei%ode_evaluate(%x%, %m%, %fm%)%$$

$head Purpose$$
This routine evaluates a function $latex f : \B{R}^n \rightarrow \B{R}^n$$
defined by 
$latex \[
	f(x) = y(x, 1)
\] $$
where $latex y(x, t)$$ solves the ordinary differential equation
$latex \[
\begin{array}{rcl}
	y(x, 0)              & = & x
	\\
	\partial_t y (x, t ) & = & g[ y(x,t) , t ]
\end{array}
\] $$
where $latex g : \B{R}^n \times \B{R} \rightarrow \B{R}^n$$
is an unspecified function.

$head Inclusion$$
The template function $code ode_evaluate$$ 
is defined in the $code CppAD$$ namespace by including 
the file $code cppad/speed/ode_evaluate.hpp$$ 
(relative to the CppAD distribution directory).
It is only intended for example and testing purposes, 
so it is not automatically included by
$cref/cppad.hpp/cppad/$$.

$head Float$$
The type $icode Float$$ must be a $cref NumericType$$.
The $icode Float$$ operation sequence for this routine
will depend on the value of the argument $icode x$$
and hence it must be retaped for each new value of $latex x$$. 

$head x$$
The argument $icode x$$ has prototype
$codei%
	const CppAD::vector<%Float%>& %x%
%$$
It contains he argument value for which the function,
or its derivative, is being evaluated.
The value $latex n$$ is determined by the size of the vector $icode x$$.

$head m$$
The argument $icode m$$ has prototype
$icode%
	size_t %m%
%$$

$subhead m == 0$$
In this case a numerical method is used to solve the ode 
and obtain an accurate approximation for $latex y(x, 1)$$.
This numerical method has a fixed
$cref/operation sequence/glossary/Operation/Sequence/$$
that does not depend on $icode x$$.

$subhead m = 1$$
In this case an analytic solution for the partial derivative
$latex \partial_x y(x, 1)$$ is returned.

$head fm$$
The argument $icode fm$$ has prototype
$icode%
	CppAD::vector<%Float%>& %fm%
%$$
The input value of the elements of $icode fm$$ does not matter.

$subhead Function$$
If $icode m$$ is zero, $icode fm$$ has size equal to $latex n$$
and contains the value of $latex y(x, 1)$$.

$subhead Gradient$$
If $icode m$$ is one, $icode fm$$ has size equal to $icode n^2$$ 
and for $latex i = 0 , \ldots and n-1$$, $latex j = 0 , \ldots , n-1$$
$latex \[
	\D{y[i]}{x[j]} (x, 1) = fm [ i \cdot n + j ]
\] $$

$children%
	speed/example/ode_evaluate.cpp%
	omh/ode_evaluate.omh
%$$

$head Example$$
The file
$cref ode_evaluate.cpp$$
contains an example and test  of $code ode_evaluate.hpp$$.
It returns true if it succeeds and false otherwise.


$head Source Code$$
The file 
$cref ode_evaluate.hpp$$
contains the source code for this template function.

$end
*/
// BEGIN PROGRAM
# include <cppad/vector.hpp>
# include <cppad/ode_err_control.hpp>
# include <cppad/runge_45.hpp>

namespace CppAD { 

	template <class Float>
	class ode_evaluate_fun {
	public:
		// Given that y_i (0) = x_i, 
		// the following y_i (t) satisfy the ODE below:
		// y_0 (t) = x[0]
		// y_1 (t) = x[1] + x[0] * t 
		// y_2 (t) = x[2] + x[1] * t + x[0] * t^2/2
		// y_3 (t) = x[3] + x[2] * t + x[1] * t^2/2 + x[0] * t^3 / 3!
		// ...
		void Ode(
			const Float&                    t, 
			const CppAD::vector<Float>&     y, 
			CppAD::vector<Float>&           f)
		{	size_t n  = y.size();	
			f[0]      = 0.;
			for(size_t k = 1; k < n; k++)
				f[k] = y[k-1];
		}
	};
	//
	template <class Float>
	void ode_evaluate(
		const CppAD::vector<Float>& x  , 
		size_t                      m  , 
		CppAD::vector<Float>&       fm )
	{	using CppAD::vector;
		typedef vector<Float> VectorFloat;

		size_t n = x.size();
		CPPAD_ASSERT_KNOWN( m == 0 || m == 1,
			"ode_evaluate: m is not zero or one"
		);
		CPPAD_ASSERT_KNOWN( 
			((m==0) & (fm.size()==n)) || ((m==1) & (fm.size()==n*n)),
			"ode_evaluate: the size of fm is not correct"
		);
		if( m == 0 )
		{	// function that defines the ode
			ode_evaluate_fun<Float> F;

			// number of Runge45 steps to use
			size_t M = 10;

			// initial and final time
			Float ti = 0.0;
			Float tf = 1.0;

			// initial value for y(x, t); i.e. y(x, 0)
			// (is a reference to x)
			const VectorFloat& yi = x;

			// final value for y(x, t); i.e., y(x, 1)
			// (is a reference to fm)
			VectorFloat& yf = fm;
			
			// Use fourth order Runge-Kutta to solve ODE
			yf = CppAD::Runge45(F, M, ti, tf, yi);

			return;
		}
		/* Compute derivaitve of y(x, 1) w.r.t x
		y_0 (x, t) = x[0]
		y_1 (x, t) = x[1] + x[0] * t 
		y_2 (x, t) = x[2] + x[1] * t + x[0] * t^2/2
		y_3 (x, t) = x[3] + x[2] * t + x[1] * t^2/2 + x[0] * t^3 / 3!
		...
		*/
		size_t i, j, k;
		for(i = 0; i < n; i++)
		{	for(j = 0; j < n; j++)
				fm[ i * n + j ] = 0.0;
		}
		size_t factorial = 1;
		for(k = 0; k < n; k++)
		{	if( k > 1 )
				factorial *= k; 
			for(i = k; i < n; i++)
			{	// partial w.r.t x[i-k] of x[i-k] * t^k / k!
				j = i - k;
				fm[ i * n + j ] += 1.0 / Float(factorial);
			}
		}
	}
}
// END PROGRAM

# endif
