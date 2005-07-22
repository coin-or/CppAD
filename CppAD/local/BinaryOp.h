# ifndef CppADBinaryOpIncluded
# define CppADBinaryOpIncluded

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
$begin BinaryOp$$
$spell
	Op
	const
$$

$index operator, base binary$$
$index binary, base operator$$
$index base, binary operator$$

$section Base Type Binary Operators$$

$head Definition$$
An operator $italic Op$$ is a base type binary operator
if for each pair of $italic Base$$ objects $italic x$$ and $italic y$$,
$syntax%
	%x% %Op% %y%
%$$ is defined and returns a $italic Base$$ object.
For example, if $italic Base$$ is $code double$$,
$code +$$ is a base type binary operator
but $code ==$$ is not
(see $xref/Compare/Definition/comparison operator/$$).

$contents%
	CppAD/local/Add.h%
	CppAD/local/Sub.h%
	CppAD/local/Mul.h%
	CppAD/local/Div.h
%$$

$end
-------------------------------------------------------------------------------
*/
# include <CppAD/local/Add.h>
# include <CppAD/local/Sub.h>
# include <CppAD/local/Mul.h>
# include <CppAD/local/Div.h>

# endif
