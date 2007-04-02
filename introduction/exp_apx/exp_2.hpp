# ifndef CPPAD_EXP_2_INCLUDED
# define CPPAD_EXP_2_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin exp_2$$
$spell
	cppad-%yy%-%mm%-%dd%
	hpp
	Apx
	cpp
	const
	exp
	bool
$$

$section Second Order Exponential Approximation$$

$index exp_2$$
$index example, algorithm$$
$index algorithm, example$$
$index exp, example$$

$head Syntax$$
$syntax%# include "exp_2.hpp"%$$
$pre
$$
$syntax%%y% = exp_2(%x%)%$$


$head Purpose$$
This is a simple example algorithm that is used to demonstrate 
Algorithmic Differentiation
(see $cref exp_eps$$ for a more complex example).

$head Mathematical Form$$
The exponential function can be defined by
$latex \[
	\exp (x) = 1 + x^1 / 1 ! + x^2 / 2 ! + \cdots 
\] $$
The second order approximation for the exponential function is
$latex \[
{\rm exp\_2} (x) =  1 + x + x^2 / 2 
\] $$


$head include$$
The include command in the syntax is relative to 
$syntax%
	cppad-%yy%-%mm%-%dd%/introduction/exp_apx
%$$
where $syntax%cppad-%yy%-%mm%-%dd%$$ is the distribution directory
created during the beginning steps of the
$cref%installation%Install%$$ of CppAD.

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const %Type% &%x%
%$$
(see $italic Type$$ below).
It specifies the point at which to evaluate the 
approximation for the second order exponential approximation.

$head y$$
The result $italic y$$ has prototype
$syntax%
	%Type% %y%
%$$
It is the value of the exponential function 
approximation defined above.

$head Type$$
If $italic u$$ and $italic v$$ are $italic Type$$ objects and $italic i$$
is an $code int$$: 

$table
$bold Operation$$  $cnext $bold Result Type$$ $cnext $bold Description$$
$rnext
$syntax%%Type%(%i%)%$$ 
	$cnext $italic Type$$
	$cnext object with value equal to $italic i$$ 
$rnext
$syntax%%u% = %v%$$
	$cnext $italic Type$$ 
	$cnext new $italic u$$ (and result) is value of $italic v$$
$rnext
$syntax%%u% * %v%$$
	$cnext $italic Type$$
	$cnext result is value of $latex u * v$$
$rnext
$syntax%%u% / %v%$$
	$cnext $italic Type$$
	$cnext result is value of $latex u / v$$
$rnext
$syntax%%u% + %v%$$
	$cnext $italic Type$$
	$cnext result is value of $latex u + v$$
$tend

$childtable%
	introduction/exp_apx/exp_2.omh%
	introduction/exp_apx/exp_2_cppad.cpp
%$$


$head Implementation$$
The file $xref/exp_2.hpp/$$
contains a C++ implementation of this function.

$head Test$$
The file $xref/exp_2.cpp/$$ 
contains a test of this implementation.
It returns true for success and false for failure.


$head Exercises$$
$list number$$
Suppose that we make the call 
$codep
	double x = .1;
	double y = exp_2(x);
$$
What is the value assigned to 
$code v1$$, $code v2$$, ... ,$code v5$$ in $cref/exp_2.hpp/$$ ?
$lnext
Extend the routine $code exp_2.hpp$$ to
a routine $code exp_3.hpp$$ that computes
$latex \[
	1 + x^2 / 2 ! + x^3 / 3 !
\] $$
Do this in a way that only assigns one value to each variable
(as $code exp_2$$ does).
$lnext
Suppose that we make the call
$codep
	double x = .5;
	double y = exp_3(x);
$$
using $code exp_3$$ created in the previous problem.
What is the value assigned to the new variables in $code exp_3$$
(variables that are in $code exp_3$$ and not in $code exp_2$$) ?
$lend

$end
------------------------------------------------------------------------------
*/
// BEGIN PROGRAM
template <class Type>
Type exp_2(const Type &x) 
{       Type v1  = x;                // v1 = x
        Type v2  = Type(1) + v1;     // v2 = 1 + x
        Type v3  = v1 * v1;          // v3 = x^2
        Type v4  = v3 / Type(2);     // v4 = x^2 / 2 
        Type v5  = v2 + v4;          // v5 = 1 + x + x^2 / 2
        return v5;                   // exp_2(x) = 1 + x + x^2 / 2
}
// END PROGRAM

# endif
