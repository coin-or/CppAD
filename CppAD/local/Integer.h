# ifndef CppADIntegerIncluded
# define CppADIntegerIncluded

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
------------------------------------------------------------------------------
$begin Integer$$
$spell
	CppAD
	namespace
	inline
	const
	bool
$$

$index Integer, convert to$$
$index convert, to integer$$

$section Convert a Value to an Integer$$

$table
$bold Syntax$$ $cnext
$syntax%Integer(%x%)%$$
$tend

$fend 20$$

$head Description$$
This function returns a static cast of $italic x$$ to an integer.

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const %Type% &%x%
%$$
where $italic Type$$ is either $syntax%AD<%Base%>%$$,
$code float$$, 
$code double$$, 
$code complex<float>$$, or
$code complex<double>$$.

$head Built In$$
If $italic Type$$ is $code float$$ or $code double$$,
the fractional part is dropped to form the integer value; i.e.
$table
$bold Case$$        $cnext $bold Comparison$$           $rnext
$syntax%%x% >= 0%$$ $cnext $syntax%Integer(%x%) <= %x%$$ $rnext
$syntax%%x% <= 0%$$ $cnext $syntax%Integer(%x%) >= %x%$$ 
$tend
If $italic Type$$ is 
$code complex<float>$$, or $code complex<double>$$,
$syntax%
	Integer(%x%) == Integer(%x%.real())
%$$


$head AD Types$$
If $italic Type$$ is $syntax%AD%<%Base%>%$$,
$italic Base$$ must support the $code Integer$$ function and
the conversion has the same meaning as for $italic Base$$.

$head Example$$
$children%
	Example/Integer.cpp
%$$
The file
$xref/Integer.cpp/$$
contains an example and a test of this operation.

$end 
------------------------------------------------------------------------------
*/


namespace CppAD {

	template <class Base>
	inline int Integer(const AD<Base> &x)
	{	return Integer(x.value); }

	inline int Integer(const float &x)
	{	return static_cast<int>(x); }

	inline int Integer(const std::complex<float> &x)
	{	return static_cast<int>( x.real() ); }

	inline int Integer(const double &x)
	{	return static_cast<int>(x); }

	inline int Integer(const std::complex<double> &x)
	{	return static_cast<int>( x.real() ); }

}
# endif

