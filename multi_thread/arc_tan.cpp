/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
-------------------------------------------------------------------------------
$begin arc_tan.cpp$$
$spell
	const
$$

$index arc_tan, multi_thread example$$
$index multi_thread, arc_tan example$$
$index example, arc_tan multi_thread$$

$section Two Argument Inverse Tangent Function$$

$head Syntax$$
$icode%theta% = arc_tan(%y%, %x%)%$$

$head Purpose$$
Determines an angle $latex \theta \in [ - \pi , + \pi ]$$
such that 
$latex \[
\begin{array}{rcl}
	\sin ( \theta )  & = & y / \sqrt{ x^2 + y^2 }  \\
	\cos ( \theta )  & = & x / \sqrt{ x^2 + y^2 }
\end{array}
\] $$

$head y$$
The argument $icode y$$ has one of the following prototypes
$codei%
	const AD<double>& %y%
%$$ 

$head x$$
The argument $icode x$$ has one of the following prototypes
$codei%
	const AD<double>& %y%
%$$ 

$head theta$$
The result $icode theta$$ has prototype
$codei%
	AD<double> %theta%
%$$

$head Operation Sequence$$
The operation sequence for this function depends on the value of
$icode x$$ and $icode y$$.
For a version of this function where the operation sequence does
not depend on $icode x$$ or $icode y$$; see $cref atan2$$.

$head Example$$
This function is used by the $cref simple_ad.cpp$$ example.

$head Source Code$$
$code
$verbatim%multi_thread/arc_tan.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
-------------------------------------------------------------------------------
*/

# include <cppad/cppad.hpp>
# include "arc_tan.hpp"

// BEGIN PROGRAM
CppAD::AD<double> 
arc_tan(const CppAD::AD<double>& x, const CppAD::AD<double>& y)
{	using CppAD::AD;

	double pi  = 4. * atan(1.);
	AD<double> theta;

	// valid for first quadrant 
	if( abs(x) > abs(y) )
		theta = atan(abs(y) / abs(x));
	else	theta = pi / 2. - atan(abs(x) / abs(y) ) ;

	// valid for first or second quadrant
	if( x < 0. )
		theta = pi - theta;

	// valid for any quadrant
	if( y < 0. )
		theta = - theta;

	return theta;
}
// END PROGRAM
