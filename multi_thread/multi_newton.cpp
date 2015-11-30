// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin multi_newton.cpp$$
$spell
	CppAD
	xout
	num
	xlow
	xup
	itr
	bool
	df
$$


$section A Multi-Threaded Newton's Method$$
$mindex multi_newton threading thread multi-threading$$

$head Syntax$$
$icode%ok% = %multi_newton(%xout%,
	%fun%, %num_sub%, %xlow%, %xup%, %epsilon%, %max_itr%, %num_threads%
)%$$

$head Purpose$$
Multi-threaded determination of the argument values $latex x$$,
in the interval $latex [a, b]$$ (where $latex a < b$$),
such that $latex f(x) = 0$$.

$head Method$$
For $latex i = 0 , \ldots , n$$,
we define the $th i$$ grid point $latex g_i$$ by
$latex \[
	g_i = a \frac{n - i}{n} +  b \frac{i}{n}
\] $$
For $latex i = 0 , \ldots , n-1$$,
we define the $th i$$ sub-interval of $latex [a, b]$$ by
$latex \[
	I_i = [ g_i , g_{i+1} ]
\] $$
Newton's method is applied starting
at the center of each of the sub-intervals $latex I_i$$ for
$latex i = 0 , \ldots , n-1$$
and at most one zero is found for each sub-interval.

$head ok$$
The return value $icode ok$$ has prototype
$codei%
	bool %ok%
%$$
If an error occurs, it is false, otherwise it is true.

$head xout$$
The argument $icode xout$$ has the prototype
$codei%
	CppAD::vector<double>& %xout%
%$$
The input size and value of the elements of $icode xout$$ do not matter.
Upon return from $code multi_newton$$,
the size of $icode xout$$ is less than or equal
the number of sub-intervals $latex n$$ and
$latex \[
	| f( xout[i] ) | \leq epsilon
\] $$
for each valid index $codei%0% <= %i% < %xout%.size()%$$.
Two $latex x$$ solutions are considered equal (and joined as one) if
the absolute difference between the solutions is less than
$latex (b - a) / n$$.

$head fun$$
The argument $icode fun$$ has prototype
$codei%
	void %fun% (double %x%, double& %f%, double& %df%)
%$$
This function must evaluate $latex f(x)$$,
and its derivative $latex f^{(1)} (x)$$,
using the syntax
$codei%
	%fun%(%x%, %f%, %df%)
%$$
where the arguments to $icode fun$$ have the prototypes
$codei%
	double    %x%
	double&   %f%
	double&   %df%
%$$.
The input values of $icode f$$ and $icode df$$ do not matter.
Upon return they are $latex f(x)$$ and $latex f^{(1)} (x)$$ respectively.

$head num_sub$$
The argument $icode num_sub$$ has prototype
$codei%
	size_t %num_sub%
%$$
It specifies the number of sub-intervals; i.e., $latex n$$.

$head xlow$$
The argument $icode xlow$$ has prototype
$codei%
	double %xlow%
%$$
It specifies the lower limit for the entire search interval; i.e., $latex a$$.

$head xup$$
The argument $icode xup$$ has prototype
$codei%
	double %xup%
%$$
It specifies the upper limit for the entire search interval; i.e., $latex b$$.

$head epsilon$$
The argument $icode epsilon$$ has prototype
$codei%
	double %epsilon%
%$$
It specifies the convergence criteria for Newton's method in terms
of how small the function value must be.

$head max_itr$$
The argument $icode max_itr$$ has prototype
$codei%
	size_t %max_itr%
%$$
It specifies the maximum number of iterations of Newton's method to try
before giving up on convergence (on each sub-interval).

$head num_threads$$
This argument has prototype
$codei%
	size_t %num_threads%
%$$
It specifies the number of threads that are available for this test.
If it is zero, the test is run without the multi-threading environment.

$childtable%
	multi_thread/multi_newton_time.cpp%
	multi_thread/multi_newton_work.cpp
%$$

$head Source$$
$code
$verbatim%multi_thread/multi_newton.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
---------------------------------------------------------------------------
*/
// BEGIN C++
// general purpose multi-threading interface
# include "team_thread.hpp"
// special utilities for the multi_newton problem
# include "multi_newton_work.hpp"

bool multi_newton(
	CppAD::vector<double> &xout                ,
	void fun(double x, double& f, double& df)  ,
	size_t num_sub                             ,
	double xlow                                ,
	double xup                                 ,
	double epsilon                             ,
	size_t max_itr                             ,
	size_t num_threads                         )
{
	bool ok = true;
	using CppAD::AD;
	using CppAD::vector;

	// setup the work for num_threads threads
	ok &= multi_newton_setup(
		fun, num_sub, xlow, xup, epsilon, max_itr, num_threads
	);

	// now do the work for each thread
	if( num_threads > 0 )
		team_work( multi_newton_worker );
	else	multi_newton_worker();

	// now combine the result for all the threads
	ok &= multi_newton_combine(xout);

	return ok;
}
// END C++
