# ifndef CppADFunCheckIncluded
# define CppADFunCheckIncluded

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
$begin FunCheck$$
$spell
	bool
	const
	Taylor
$$

$index FunCheck$$
$index check, ADFun$$
$index ADFun, check$$

$section Check an ADFun Sequence of Operations$$

$table
$bold Syntax$$ $cnext
$syntax%%ok% = FunCheck(%f%, %g%, %x%, %r%, %a%)%$$
$tend

$fend 20$$

$head Purpose$$
We use $latex F : B^n \rightarrow B^m$$ to denote the
$xref/glossary/AD Function/AD function/$$ corresponding to $italic f$$.
We use $latex G : B^n \rightarrow B^m$$ to denote the
function corresponding to the C++ function object $italic g$$.
This routine checks that,
at the independent variable values specified by $italic x$$,
the AD of $italic Base$$ 
$xref/glossary/Operation/Sequence/operation sequence/1/$$
corresponding to $italic f$$ properly represents the algorithm used
by $italic g$$ to calculate values for $latex G$$.

$head f$$
The $code FunCheck$$ argument $italic f$$ has prototype
$syntax%
	ADFun<%Base%> %f%
%$$
Note that the $xref/ADFun/$$ object $italic f$$ is not $code const$$
(see $xref/FunCheck/FunCheck Uses Forward/Forward/$$ below).

$head g$$
The $code FunCheck$$ argument $italic g$$ has prototype
$syntax%
	%Fun% &%g%
%$$
($italic Fun$$ is defined the properties of $italic g$$).
The C++ function object $italic g$$ supports the syntax
$syntax%
	%y% = %g%(%x%)
%$$
which computes $latex y = G(x)$$.

$subhead x$$
The $italic g$$ argument $italic x$$ has prototype
$syntax%
	const %Vector% &%x%
%$$
(see $xref/FunCheck/Vector/Vector/$$ below)
and its size 
must be equal to $italic n$$, the dimension of the
$xref/FunConstruct/x/Domain Space/domain space/1/$$ for $italic f$$.

$head y$$
The $italic g$$ result $italic y$$ has prototype
$syntax%
	%Vector% %y%
%$$
and its value is $latex G(x)$$.
The size of $italic y$$ 
is equal to $italic m$$, the dimension of the
$xref/FunConstruct/y/Range Space/range space/1/$$ for $italic f$$.

$head x$$
The $code FunCheck$$ argument $italic x$$ has prototype
$syntax%
	const %Vector% &%x%
%$$
and its size 
must be equal to $italic n$$, the dimension of the
$xref/FunConstruct/x/Domain Space/domain space/1/$$ for $italic f$$.
This specifies that point at which to compare the derivative
calculated using $italic f$$ with the central difference approximation
for the derivative of $latex G$$.

$head r$$
The $code FunCheck$$ argument $italic r$$ has prototype
$syntax%
	const %Base% &%r%
%$$
It specifies the relative error the element by element
comparison of the derivatives of $latex F$$ and $italic G$$.

$head a$$
The $code FunCheck$$ argument $italic a$$ has prototype
$syntax%
	const %Base% &%a%
%$$
It specifies the absolute error the element by element
comparison of the derivative of $latex F(x)$$ with $italic D$$.

$head ok$$
The $code FunCheck$$ result $italic ok$$ has prototype
$syntax%
	bool %ok%
%$$
It is true, if for $latex i = 0 , \ldots , m-1$$ 
either the relative error bound is satisfied
$latex \[
| F_i (x) - G_i (x) | 
\leq 
r ( | F_i (x) | + | G_i (x) | ) 
\] $$
or the absolute error bound is satisfied
$latex \[
	| F_i (x) - G_i (x) | \leq a
\] $$
It is false if for some $latex (i, j)$$ neither
of these bounds is satisfied.

$head Vector$$
The type $italic Vector$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type/$$
$italic Base$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head FunCheck Uses Forward$$
After each call to $xref/Forward/$$,
the object $italic f$$ contains the corresponding 
$xref/glossary/Taylor Coefficient/Taylor coefficients/$$.
After $code FunCheck$$,
the previous calls to $xref/Forward/$$ are undefined.


$head Example$$
$children%
	Example/FunCheck.cpp
%$$
The file
$xref/FunCheck.cpp/$$
contains an example and test of this function.   
It returns true if it succeeds and false otherwise.

$end
---------------------------------------------------------------------------
*/

namespace CppAD {
	template <class Base, class Fun, class Vector>
	bool FunCheck(
		ADFun<Base>  &f , 
		Fun          &g , 
		const Vector &x , 
		const Base   &r ,
		const Base   &a )
	{	bool ok = true;
	
		size_t m   = f.Range();
		Vector yf  = f.Forward(0, x); 
		Vector yg  = g(x);
	
		size_t i;
		for(i = 0; i < m; i++)
			ok  &= NearEqual(yf[i], yg[i], r, a);
		return ok;
	}
}

# endif
