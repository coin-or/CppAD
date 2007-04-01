# ifndef CPPAD_EXP_EPS_INCLUDED
# define CPPAD_EXP_EPS_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin exp_eps$$
$spell
	cppad-%yy%-%mm%-%dd%
	hpp
	Apx
	cpp
	const
	exp_eps
	bool
$$

$section An Epsilon Accurate Exponential Approximation$$

$index exp_eps$$
$index example, algorithm$$
$index algorithm, example$$
$index exp, example$$

$head Syntax$$
$syntax%# include "exp_eps.hpp"%$$
$pre
$$
$syntax%%y% = exp_eps(%x%, %epsilon%)%$$


$head Purpose$$
This is a an example algorithm that is used to demonstrate 
how Algorithmic Differentiation works with loops and  
boolean decision variables
(see $cref/exp_2/$$ for a simpler example). 

$head Mathematical Function$$
The exponential function can be defined by
$latex \[
	\exp (x) = 1 + x^1 / 1 ! + x^2 / 2 ! + \cdots 
\] $$
We define $latex k ( x, \varepsilon )  $$ as the smallest
non-negative integer such that $latex \varepsilon \geq x^k / k !$$; i.e.,
$latex \[
k( x, \varepsilon ) = 
	\min \{ k \in {\rm Z}_+ \; | \; \varepsilon \geq x^k / k ! \}
\] $$
The mathematical form for our approximation of the exponential function is
$latex \[
\begin{array}{rcl}
{\rm exp\_eps} (x , \varepsilon ) & = & \left\{
\begin{array}{ll}
\frac{1}{ {\rm exp\_eps} (-x , \varepsilon ) } 
	& {\rm if} \; x < 0 
\\
1 + x^1 / 1 ! + \cdots + x^{k( x, \varepsilon)} / k( x, \varepsilon ) !
	& {\rm otherwise}
\end{array}
\right.
\end{array}
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
approximation for the exponential function.

$head epsilon$$
The argument $italic epsilon$$ has prototype
$syntax%
	const %Type% &%epsilon%
%$$
It specifies the accuracy with which
to approximate the exponential function value; i.e.,
it is the value of $latex \varepsilon$$ in the 
exponential function approximation defined above.

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
$syntax%%u% > %v%$$
	$cnext $code bool$$
	$cnext true,
	if $italic u$$ greater than $italic v$$, an false otherwise
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
$rnext
$syntax%-%u%$$
	$cnext $italic Type$$
	$cnext result is value of $latex - u$$
$tend

$children%
	introduction/exp_apx/exp_eps.omh%
	introduction/exp_apx/exp_eps_cppad.cpp
%$$

$head Implementation$$
The file $xref/exp_eps.hpp/$$
contains a C++ implementation of this function.

$head Test$$
The file $xref/exp_eps.cpp/$$ 
contains a test of this implementation.
It returns true for success and false for failure.

$head Exercises$$
$list number$$
Using the definition of $latex k( x, \varepsilon )$$ above,
what is the value of 
$latex k(.5, 1)$$, $latex k(.5, .1)$$, and $latex k(.5, .01)$$ ?
$lnext
Suppose that we make the following call to $code exp_eps$$:
$codep
	double x       = 1.;
	double epsilon = .01;
	double y = exp_eps(x, epsilon);
$$
What is the value assigned to 
$code k$$, $code temp$$, $code term$$, and $code sum$$
the first time through the $code while$$ loop in $cref/exp_eps.hpp/$$ ?
$lnext
Continuing the previous exercise, 
what is the value assigned to 
$code k$$, $code temp$$, $code term$$, and $code sum$$
the second time through the $code while$$ loop in $cref/exp_eps.hpp/$$ ?
$lend


$end
-----------------------------------------------------------------------------
*/
// BEGIN PROGRAM
template <class Type>
Type exp_eps(const Type &x, const Type &epsilon)
{	// abs_x = |x|
	Type abs_x = x;
	if( Type(0) > x )
		abs_x = - x;
	// initialize
	int  k    = 0;          // initial order 
	Type term = 1.;         // term = |x|^k / k !
	Type sum  = term;       // initial sum
	while(term > epsilon)
	{	k         = k + 1;          // order for next term
		Type temp = term * abs_x;   // term = |x|^k / (k-1)!
		term      = temp / Type(k); // term = |x|^k / k !
		sum       = sum + term;     // sum  = 1 + ... + |x|^k / k !
	}
	// In the case where x is negative, use exp(x) = 1 / exp(-|x|)
	if( Type(0) > x ) 
		sum = Type(1) / sum;
	return sum;
}
// END PROGRAM

# endif
