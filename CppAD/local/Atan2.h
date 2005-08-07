# ifndef CppADAtan2Included
# define CppADAtan2Included

// BEGIN SHORT COPYRIGHT
/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

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
// END SHORT COPYRIGHT

/*
-------------------------------------------------------------------------------
$begin Atan2$$
$spell
	std
	Cpp
	namespace
	Arctangent
	inline
	const
	atan2
$$

$index tan, inverse$$
$index inverse, tan$$
$index atan2$$

$section The Two Argument Inverse Tangent Function$$

$table
$bold Syntax$$ 
$cnext 
$syntax%inline AD<%Base%> atan2 (const AD<%Base%> &%x%)%$$
$tend

$fend 20$$

$head Description$$
Returns an
$syntax%AD<%Base%>%$$ value $latex \theta$$ 
such that $latex - \pi \leq \theta$$ and $latex \theta \leq \pi$$ and
$latex \[
\begin{array}{rcl}
	\sin ( \theta )  & = & y / \sqrt{ x^2 + y^2 }  \\
	\cos ( \theta )  & = & x  / \sqrt{ x^2 + y^2 }
\end{array}
\] $$
The type $italic Base$$ must support 
$xref/CondExp/Type/conditional expressions/$$; 
for example, $italic Base$$ can be $code float$$ or $code double$$.

$head Example$$
$children%
	Example/Atan2.cpp
%$$
The file
$xref/Atan2.cpp/$$
contains an example and a test of this function.   
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
AD<Base> atan2 (
	const AD<Base> &y, const AD<Base> &x)
{	AD<Base> alpha;
	AD<Base> beta;
	AD<Base> theta;

	AD<Base> zero = 0;
	AD<Base> pi2  = 2. * atan(1.);
	AD<Base> pi   = 2. * pi2;

	AD<Base> ax = abs(x);
	AD<Base> ay = abs(y);

	// if( ax > ay )
	// 	theta = atan(ay / ax);
	// else	theta = pi2 - atan(ax / ay);
	alpha = atan(ay / ax);
	beta  = pi2 - atan(ax / ay);
	theta = CondExpGt(ax, ay, alpha, beta); 

	// if( x <= 0 )
	// 	theta = pi - theta;
	theta = CondExpLe(x, zero, pi - theta, theta);
	
	// if( y <= 0 )
	// 	theta = - theta;
	theta = CondExpLe(y, zero, -theta, theta);

	return theta;
}

template <class Base>
inline AD<Base> atan2 (
	const VecADelem<Base> &y, const AD<Base> &x)
{	return atan2( y.ADBase() , x ); }

template <class Base>
inline AD<Base> atan2 (
	const AD<Base> &y, const VecADelem<Base> &x)
{	return atan2( y , x.ADBase() ); }

template <class Base>
inline AD<Base> atan2 (
	const VecADelem<Base> &y, const VecADelem<Base> &x)
{	return atan2( y.ADBase() , x.ADBase() ); }

} // END CppAD namespace

# endif 
