# ifndef CppADAbsIncluded
# define CppADAbsIncluded

/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */

/*
-------------------------------------------------------------------------------
$begin abs$$
$spell
	Taylor
	Cpp
	namespace
	const
	abs
$$

$index absolute, AD value$$
$index value, AD absolute$$
$index abs, AD$$
$index directional, derivative for abs$$
$index derivative, directional for abs$$

$section The AD Absolute Value Function$$

$table
$bold Syntax$$ $cnext 
$syntax% %y% = abs(%x%)%$$
$tend

$fend 20$$

$head Purpose$$
Returns an
$syntax%AD<%Base%>%$$ object that is equal to
$syntax%
	abs(%x%)
%$$
where $code abs$$ has the same interpretation as
for the $italic Base$$ type.

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const AD<%Base%> &%x%
%$$

$head y$$
The result $italic y$$ has prototype
$syntax%
	AD<%Base%> %y%
%$$



$head Base$$ 
A definition for the $code abs$$ function
is automatically included (in the $code CppAD$$ namespace)
for the types 
$code float$$, $code double$$ and the 
$xref/glossary/AD Type Sequence/AD type sequences/$$
above $code float$$ or $code double$$; for example;
$code AD<double>$$.

$head Derivative$$
The derivative of the absolute value function is one for 
$latex x > 0$$ and minus one for $latex x < 0$$.
The subtitle issue is 
how to compute directional derivatives
what $latex x = 0$$.
$pre

$$
The function corresponding to the argument $italic x$$ 
and the result $italic y$$ are represented
by their Taylor coefficients; i.e.,
$latex \[
\begin{array}{rcl}
	X(t) & = & x^{(0)} (t) + x^{(1)} t + \cdots + x^{(p)} t^p
	\\
	Y(t) & = & y^{(0)} (t) + y^{(1)} t + \cdots + y^{(p)} t^p
\end{array}
\] $$
Note that $latex x^{(0)} = X(0)$$ is the value of $italic x$$ and
$latex y^{(0)} = Y(0)$$ is the value of $italic y$$.
In the equations above, the order $latex p$$ is specified
by a call to $xref/Forward/$$ or $xref/Reverse/$$ as follows:
$syntax%
	%f%.Forward(%p%, %dx%)
	%f%.Reverse(%p%+1, %w%)
%$$
If all of the Taylor coefficients of $latex X(t)$$ are zero,
we define $latex k = p$$.
Otherwise, we define $latex k$$ to be the minimal index such that 
$latex x^{(k)} \neq 0$$.
Note that if $latex x \neq 0$$, $latex k = 0$$.
The Taylor coefficient representation of $latex Y(t)$$
(and hence it's derivatives) are computed as
$latex \[
y^{(\ell)}
=
\left\{ \begin{array}{ll} 
	 x^{(\ell)}   & {\rm if} \; x^{(k)} > 0         \\
	 0                    & {\rm if} \; x^{(k)} = 0 \\
	- x^{(\ell)}  & {\rm if} \; x^{(k)} < 0
\end{array} \right.
\] $$


$head Example$$
$children%
	Example/Abs.cpp
%$$
The file
$xref/Abs.cpp/$$
contains an example and test of this function.   
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

inline float abs(float x)
{	if( x > 0. )
		return x;
	else if( x < 0. )
		return -x;
	return 0.;
}

inline double abs(double x)
{	if( x > 0. )
		return x;
	else if( x < 0. )
		return -x;
	return 0.;
}

inline std::complex<float>  abs(std::complex<float> x)
{	CppADUsageError(
		0,
		"Attempt to use std::complex<float> as an ordered type"
	);
	return false;
}

inline std::complex<double>  abs(std::complex<double> x)
{	CppADUsageError(
		0,
		"Attempt to use std::complex<double> as an ordered type"
	);
	return false;
}


template <class Base>
AD<Base> AD<Base>::Abs (void) const
{	using CppAD::abs;

	AD<Base> result;
	CppADUnknownError( result.id == 0 );

	if( GreaterThanZero(value) )
		result.value = value;
	else if( LessThanZero (value) )
		result.value = - value;
	else	result.value = Base(0);
		

	if(	(Tape()->State() == Recording) & Variable(*this) ) 
	{
		// add this operation to the tape
		Tape()->RecordOp(AbsOp, result, taddr);

	}

	return result;
}

template <class Base>
inline AD<Base> abs(const AD<Base> &x)
{	return x.Abs(); }

template <class Base>
inline AD<Base> abs(const VecADelem<Base> &x)
{	return abs( x.ADBase() ); }

} // END CppAD namespace

# endif 
