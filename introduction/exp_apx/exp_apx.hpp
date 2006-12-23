# ifndef CPPAD_EXP_APX_INCLUDED
# define CPPAD_EXP_APX_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin exp_apx.hpp$$
$spell
	exp_apx
	const
	Exp
$$

$section An Example Exponential Approximation Source Code$$
$codep */
template <class Type>
Type exp_apx(const Type &x, const Type &e)
{	Type a, q, r, s, k;           // declare local variables
	r = s = k = Type(1);          // initialize as equal to one
	a = x;                        // a = |x|
	if( Type(0) > x ) a = -x;
	// while the previous term included in the summation is greater than e
	while(r > e)
	{	q  = a * r;           // q = |x|^k / (k-1)!
		r  = q / k;           // r = |x|^k / k!
		s  = s + r;           // s = 1 + |x| / 1! + ... + |x|^k / k!
		k  = k + Type(1);
	}
	// In the case where x is negative, use exp(x) = 1 / exp(-|x|)
	if( Type(0) > x ) s = Type(1) / s;
	return s;
}
/* $$
$end
*/

# endif
