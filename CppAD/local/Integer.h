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
	inline
	const
	bool
$$

$index int, convert to$$
$index convert, to int$$

$section Convert a Value to an Integer$$

$table
$bold Syntax$$ $cnext
$syntax%inline int Integer(const %Type% &%x%)%$$
$tend

$head Description$$
This function returns a static cast of $italic x$$ to an integer.

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

