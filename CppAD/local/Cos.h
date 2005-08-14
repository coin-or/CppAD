# ifndef CppADCosIncluded
# define CppADCosIncluded

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
$begin cos$$
$spell
	Cpp
	namespace
	const
	cos
$$

$index cos$$
$index trigonometric, cosine$$

$section The Trigonometric Cosine Function$$

$table
$bold Syntax$$ 
$cnext 
$syntax%AD<%Base%> cos (const AD<%Base%> &%x%)%$$
$tend

$fend 20$$

$head Description$$
Returns an
$syntax%AD<%Base%>%$$ object that is equal to
$syntax%
	cos(%x%)
%$$
where $code cos$$ has the same interpretation as
for the corresponding $xref/glossary/Base Type/base type/$$.


$head Base Type Requirement$$
A definition for the $code cos$$ function
is automatically included (in the $code CppAD$$ namespace)
for the following 
$xref/glossary/Base Type/base types/$$:
$code float$$, $code double$$, $code complex<float>$$, 
and $code complex<double>$$.
If the $code cos$$ function is used with any other base type, 
it must be a defined for that base type.

$head Derivative Assumption$$
It is assumed that
$latex \[
\begin{array}{lcr}
	\D{[ \cos (x) ]}{x} & = & - \sin (x)  \\
	\D{[ \sin (x) ]}{x} & = & \cos (x)
\end{array}
\] $$



$head Example$$
$children%
	Example/Cos.cpp
%$$
The file
$xref/Cos.cpp/$$
contains an example and a test of this function.   
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

CppADStandardMathFun(cos)

template <class Base>
AD<Base> AD<Base>::Cos (void) const
{	using CppAD::cos;

	AD<Base> result;
	CppADUnknownError( result.id == 0 );

	result.value   = cos(value);

	if(	(Tape()->State() == Recording) & Variable(*this) ) 
	{
		// add this operation to the tape
		Tape()->RecordOp(CosOp, result, taddr);

	}

	return result;
}

template <class Base>
inline AD<Base> cos(const AD<Base> &x)
{	return x.Cos(); }

template <class Base>
inline AD<Base> cos(const VecADelem<Base> &x)
{	return cos( x.ADBase() ); }

} // END CppAD namespace

# endif 
