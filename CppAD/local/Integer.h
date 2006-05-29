# ifndef CppADIntegerIncluded
# define CppADIntegerIncluded

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
------------------------------------------------------------------------------
$begin Integer$$
$spell
	std
	VecAD
	CppAD
	namespace
	inline
	const
	bool
$$

$index Integer$$

$index AD, convert to integer$$
$index convert, AD to integer$$

$section Convert From AD to Integer$$

$head Syntax$$
$syntax%%i% = Integer(%x%)%$$


$head Purpose$$
Converts from an AD type to the corresponding integer value.

$head i$$
The result $italic i$$ has prototype
$syntax%
	int %i%
%$$

$head x$$

$subhead Real Types$$
If the argument $italic x$$ has either of the following prototypes:
$syntax%
	const float                %%  &%x%
	const double               %%  &%x%
%$$
the fractional part is dropped to form the integer value.
For example, if $italic x$$ is 1.5, $italic i$$ is 1.
In general, if $latex x \geq 0$$, $italic i$$ is the 
greatest integer less than or equal $italic x$$.
If $latex x \leq 0$$, $italic i$$ is the 
smallest integer greater than or equal $italic x$$.

$subhead Complex Types$$
If the argument $italic x$$ has either of the following prototypes:
$syntax%
	const std::complex<float>  %%  &%x%
	const std::complex<double> %%  &%x%
%$$
The result $italic i$$ is given by
$syntax%
	%i% = Integer(%x%.real())
%$$

$subhead AD Types$$
If the argument $italic x$$ has either of the following prototypes:
$syntax%
	const AD<%Base%>               &%x%
	const VecAD<%Base%>::reference &%x%
%$$
$italic Base$$ must support the $code Integer$$ function and
the conversion has the same meaning as for $italic Base$$.

$head Operation Sequence$$
The result of this operation is not an
$xref/glossary/AD of Base/AD of/$$ $italic Base$$ object.
Thus it will not be recorded as part of the current
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$
(even if the current $xref/glossary/Tape State/tape state/$$ is Recording).

$head Example$$
$children%
	Example/Integer.cpp
%$$
The file
$xref/Integer.cpp/$$
contains an example and test of this operation.

$end 
------------------------------------------------------------------------------
*/


namespace CppAD {

	inline int Integer(const float &x)
	{	return static_cast<int>(x); }

	inline int Integer(const std::complex<float> &x)
	{	return static_cast<int>( x.real() ); }

	inline int Integer(const double &x)
	{	return static_cast<int>(x); }

	inline int Integer(const std::complex<double> &x)
	{	return static_cast<int>( x.real() ); }

	template <class Base>
	inline int Integer(const AD<Base> &x)
	{	return Integer(x.value); }

	template <class Base>
	inline int Integer(const VecAD_reference<Base> &x)
	{	return Integer( x.ADBase() ); }
}
# endif

