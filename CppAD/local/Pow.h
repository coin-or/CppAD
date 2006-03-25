# ifndef CppADPowIncluded
# define CppADPowIncluded

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
$begin pow$$
$spell
	std
	namespace
	CppAD
	const
$$

$index pow, AD$$
$index exponent, AD function$$

$section The AD Power Function$$

$table
$bold Syntax$$ $cnext 
$syntax%%z% = pow(%x%, %y%)%$$
$tend

$fend 20$$

$head Purpose$$
Determines the value of the power function which is defined by
$latex \[
	{\rm pow} (x, y) = x^y
\] $$.

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const AD<%Base%> &%x%
%$$ 

$head y$$
The argument $italic y$$ has prototype
$syntax%
	const AD<%Base%> &%y%
%$$ 
(see $xref/PowInt/$$ for the case where $italic y$$ is an $code int$$).

$head z$$
The result $italic z$$ has prototype
$syntax%
	AD<%Base%> %z%
%$$

$head Standard Types$$
A definition for the $code pow$$ function is automatically included
in the CppAD namespace for the case where both $italic x$$
and $italic y$$ have the following standard types:
$code float$$, $code double$$, 
$code std::complex<float>$$, and
$code std::complex<double>$$.

$head Example$$
$children%
	Example/Pow.cpp
%$$
The file
$xref/Pow.cpp/$$
contains an example and test of this function.   
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN CppAD namespace

inline float pow(float x, float y)
{	return std::pow(x, y); }

inline double pow(double x, double y)
{	return std::pow(x, y); }

inline std::complex<float> 
pow(std::complex<float> x, std::complex<float> y)
{	return std::pow(x, y); }

inline std::complex<double> 
pow(std::complex<double> x, std::complex<double> y)
{	return std::pow(x, y); }

template <class Base>
inline AD<Base> pow (const AD<Base> &x, const AD<Base> &y)
{	using CppAD::exp;
	using CppAD::log;

	return exp( y * log(x) );
}

template <class Base>
inline AD<Base> pow (const VecAD_reference<Base> &y, const AD<Base> &x)
{	return pow( y.ADBase() , x ); }

template <class Base>
inline AD<Base> pow (const AD<Base> &y, const VecAD_reference<Base> &x)
{	return pow( y , x.ADBase() ); }

template <class Base>
inline AD<Base> pow 
(const VecAD_reference<Base> &y, const VecAD_reference<Base> &x)
{	return pow( y.ADBase() , x.ADBase() ); }

} // END CppAD namespace

# endif 
