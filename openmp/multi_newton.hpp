# ifndef CPPAD_MULTI_NEWTON_INCLUDED
# define CPPAD_MULTI_NEWTON_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin multi_newton$$
$spell
	xout
	xlow
	xup
	itr
	CppAD
	const
$$

$index OpenMP, Newton's method$$
$index multi-thread, Newton's method$$
$index example, OpenMP Newton's method$$

$section Multi-Threaded Newton's Method Routine$$

$head Syntax$$
$syntax%multi_newton(%
	xout%, %fun%, %n_grid%, %xlow%, %xup%, %epsilon%, %max_itr%)%$$


$head Purpose$$
Determine the argument values $latex x \in [a, b]$$ (where $latex a < b$$)
such that $latex f(x) = 0$$.

$head Method$$
For $latex i = 0 , \ldots , n$$,  
we define the $th i$$ grid point $latex g_i$$ 
and the $th i$$ interval $latex I_i$$ by
$latex \[
\begin{array}{rcl}
	g_i & = & a \frac{n - i}{n} +  b \frac{i}{n}
	\\
	I_i & = & [ g_i , g_{i+1} ]
\end{array}
\] $$
Newton's method is applied starting
at the center of each of the intervals $latex I_i$$ for
$latex i = 0 , \ldots , n-1$$
and at most one zero is found for each interval.


$head xout$$
The argument $italic xout$$ has the prototype
$syntax%
	CppAD::vector<double> &%xout%
%$$
The input size and value of the elements of $italic xout$$ do not matter.
Upon return from $code multi_newton$$,
the size of $italic xout$$ is less than $latex n$$ and
$latex \[
	| f( xout[i] ) | \leq epsilon
\] $$ 
for each valid index $italic i$$.
Two $latex x$$ solutions are considered equal (and joined as one) if
the absolute difference between the solutions is less than
$latex (b - a) / n$$.

$head fun$$
The argument $italic fun$$ has prototype
$syntax%
	%Fun% &%fun%
%$$
This argument must evaluate the function $latex f(x)$$ 
using the syntax
$syntax%
	%f% = %fun%(%x%)
%$$
where the argument $italic x$$ and the result $italic f$$
have the prototypes
$syntax%
	const AD<double> &%x% 
	AD<double>        %f%
%$$.


$head n_grid$$
The argument $italic n_grid$$ has prototype
$syntax%
	size_t %n_grid%
%$$
It specifies the number of grid points; i.e., $latex n$$ 
in the $cref/method/multi_newton/Method/$$ above.

$head xlow$$
The argument $italic xlow$$ has prototype
$syntax%
	double %xlow%
%$$
It specifies the lower limit for the entire search; i.e., $latex a$$
in the $cref/method/multi_newton/Method/$$ above.

$head xup$$
The argument $italic xup$$ has prototype
$syntax%
	double %xup%
%$$
It specifies the upper limit for the entire search; i.e., $latex b$$
in the $cref/method/multi_newton/Method/$$ above.

$head epsilon$$
The argument $italic epsilon$$ has prototype
$syntax%
	double %epsilon%
%$$
It specifies the convergence criteria for Newton's method in terms
of how small the function value must be.

$head max_itr$$
The argument $italic max_itr$$ has prototype
$syntax%
	size_t %max_itr%
%$$
It specifies the maximum number of iterations of Newton's method to try
before giving up on convergence.

$end
---------------------------------------------------------------------------
$begin multi_newton.hpp$$

$index multi_newton, source$$
$index source, multi_newton$$
$index example, OpenMP$$
$index example, multi-thread$$
$index OpenMP, example$$
$index multi-thread, example$$


$section OpenMP Multi-Threading Newton's Method Source Code$$

$code
$verbatim%openmp/multi_newton.hpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$
$end
---------------------------------------------------------------------------
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <cassert>

# ifdef _OPENMP
# include <omp.h>
# endif

namespace { // BEGIN CppAD namespace

template <class Fun>
void one_newton(double &fcur, double &xcur, Fun &fun, 
	double xlow, double xin, double xup, double epsilon, size_t max_itr)
{	using CppAD::AD;
	using CppAD::vector;
	using CppAD::abs;

	// domain space vector
	size_t n = 1;
	vector< AD<double> > X(n);
	// range space vector
	size_t m = 1;
	vector< AD<double> > Y(m);
	// domain and range differentials
	vector<double> dx(n), dy(m);

	size_t itr;
	xcur = xin;
	for(itr = 0; itr < max_itr; itr++)
	{	// domain space vector
		X[0] = xcur;
		CppAD::Independent(X);
		// range space vector
		Y[0] = fun(X[0]);
		// F : X -> Y
		CppAD::ADFun<double> F(X, Y);
		// fcur = F(xcur)
		fcur  = Value(Y[0]);
		// evaluate dfcur = F'(xcur)
		dx[0] = 1;
		dy = F.Forward(1, dx);
		double dfcur = dy[0];
		// check end of iterations
		if( abs(fcur) <= epsilon )
			return;
		if( (xcur == xlow) & (fcur * dfcur > 0.) )
			return; 
		if( (xcur == xup)  & (fcur * dfcur < 0.) )
			return; 
		if( dfcur == 0. )
			return;
		// next Newton iterate
		double delta_x = - fcur / dfcur;
		if( xlow - xcur >= delta_x )
			xcur = xlow;
		else if( xup - xcur <= delta_x )
			xcur = xup;
		else	xcur = xcur + delta_x;
	}
	return;
}

template <class Fun>
void multi_newton(
	CppAD::vector<double> &xout , 
	Fun &fun                    , 
	size_t n_grid               , 
	double xlow                 , 
	double xup                  , 
	double epsilon              , 
	size_t max_itr              )
{	using CppAD::AD;
	using CppAD::vector;
	using CppAD::abs;

	// check argument values
	assert( xlow < xup );
	assert( n_grid > 0 );

	// OpenMP uses integers in place of size_t
	int i, n = int(n_grid);

	// set up grid
	vector<double> grid(n_grid + 1);
	vector<double> fcur(n_grid), xcur(n_grid), xmid(n_grid);
	double dx = (xup - xlow) / double(n_grid);
	for(i = 0; size_t(i) < n_grid; i++)
	{	grid[i] = xlow + i * dx;
		xmid[i] = xlow + (i + .5) * dx;
	}
	grid[n_grid] = xup;

# ifdef _OPENMP
# pragma omp parallel for 
# endif
	for(i = 0; i < n; i++) 
	{	one_newton(
			fcur[i]   ,
			xcur[i]   ,
			fun       , 
			grid[i]   , 
			xmid[i]   , 
			grid[i+1] , 
			epsilon   , 
			max_itr
		);
	}
// end omp parallel for

	// remove duplicates and points that are not solutions
	double xlast  = xlow;
	size_t ilast  = 0;
	size_t n_zero = 0;
	for(i = 0; size_t(i) < n_grid; i++)
	{	if( abs( fcur[i] ) <= epsilon )
		{	if( n_zero == 0 )
			{	xcur[n_zero++] = xlast = xcur[i];
				ilast = i;
			}
			else if( fabs( xcur[i] - xlast ) > dx ) 
			{	xcur[n_zero++] = xlast = xcur[i];
				ilast = i;
			}
			else if( fabs( fcur[i] ) < fabs( fcur[ilast] ) )
			{	xcur[n_zero - 1] = xlast = xcur[i]; 
				ilast = i;
			}
		}
	}

	// resize output vector and set its values
	xout.resize(n_zero);
	for(i = 0; size_t(i) < n_zero; i++)
		xout[i] = xcur[i];

	return;
}

} // END CppAD namespace

// END PROGRAM

# endif
