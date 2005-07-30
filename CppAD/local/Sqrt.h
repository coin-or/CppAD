# ifndef CppADSqrtIncluded
# define CppADSqrtIncluded

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
$begin Sqrt$$
$spell
	Cpp
	namespace
	const
	sqrt
$$

$index square root$$
$index root, square$$
$index sqrt$$

$section The Square Root Function$$

$table
$bold Syntax$$ 
$cnext 
$syntax%AD<%Base%> sqrt (const AD<%Base%> &%x%)%$$
$tend

$fend 20$$

$head Description$$
Returns an
$syntax%AD<%Base%>%$$ object that is equal to
$syntax%
	sqrt(%x%)
%$$
where $code sqrt$$ has the same interpretation as
for the corresponding $xref/glossary/Base Type/base type/$$.

$head Base Type Requirement$$
A definition for the $code sqrt$$ function
is automatically included (in the $code CppAD$$ namespace)
for the following 
$xref/glossary/Base Type/base types/$$:
$code float$$, $code double$$, $code complex<float>$$, 
and $code complex<double>$$.
If the $code sqrt$$ function is used with any other base type, 
it must be a defined for that base type.

$head Derivative Assumption$$
It is assumed that
$latex \[
\begin{array}{lcr}
	\D{[ {\rm sqrt} (x) ]}{x} & = & \frac{1}{2 {\rm sqrt} (x) }
\end{array}
\] $$



$head Example$$
$children%
	Example/Sqrt.cpp
%$$
The file
$xref/Sqrt.cpp/$$
contains an example and a test of this function.   
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

CppADStandardMathFun(sqrt)


template <class Base>
AD<Base> AD<Base>::Sqrt (void) const
{	using CppAD::sqrt;

	AD<Base> result;
	CppADUnknownError( result.id == 0 );

	result.value   = sqrt(value);

	if(	(Tape()->State() == Recording) & Variable(*this) ) 
	{	// add this operation to the tape
		Tape()->RecordOp(SqrtOp, result, taddr);
	}

	return result;
}

template <class Base>
inline AD<Base> sqrt(const AD<Base> &x)
{	return x.Sqrt(); }

template <class Base>
inline AD<Base> sqrt(const VecADelem<Base> &x)
{	return sqrt( x.ADBase() ); }

} // END CppAD namespace

# endif 
