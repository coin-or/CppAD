# ifndef CppADAsinIncluded
# define CppADAsinIncluded

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
$begin Asin$$
$spell
	Cpp
	namespace
	const
	asin
	Arcsine
$$


$index inverse, sin$$
$index sin, inverse$$
$index asin$$

$section The Inverse Sine Function$$

$table
$bold Syntax$$ 
$cnext 
$syntax%AD<%Base%> asin (const AD<%Base%> &%x%)%$$
$tend

$fend 20$$

$head Description$$
Returns an
$syntax%AD<%Base%>%$$ object that is equal to
$syntax%
	asin(%x%)
%$$
where $code asin$$ has the same interpretation as
for the corresponding $xref/glossary/Base Type/base type/$$.


$head Base Type Requirement$$
A definition for the $code asin$$ function
is automatically included (in the $code CppAD$$ namespace)
for the following 
$xref/glossary/Base Type/base types/$$:
$code float$$ and $code double$$.
If the $code asin$$ function is used with any other base type, 
it must be a defined for that base type.

$head Derivative Assumption$$
It is assumed that
$latex \[
\begin{array}{lcr}
	\D{[ {\rm asin} (x) ]}{x} & = & (1 - x * x)^{-1/2}
\end{array}
\] $$



$head Example$$
$children%
	Example/Asin.cpp
%$$
The file
$xref/Asin.cpp/$$
contains an example and a test of this function.   
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {


# ifdef _MSC_VER
	inline float asin(float x)
	{	return ::asin(x); }
	inline double asin(double x)
	{	return ::asin(x); }
# else
	inline float asin(float x)
	{	return std::asin(x); }
	inline double asin(double x)
	{	return std::asin(x); }
# endif

inline std::complex<float> asin(std::complex<float> x)
{	CppADUsageError(
		0,
		"Attempt to use asin with complex argument"
	);
	return x;
}
inline std::complex<double> asin(std::complex<double> x)
{	CppADUsageError(
		0,
		"Attempt to use asin with complex argument"
	);
	return x;
}


template <class Base>
AD<Base> AD<Base>::Asin (void) const
{	using CppAD::asin;

	AD<Base> result;
	CppADUnknownError( result.id == 0 );

	result.value   = asin(value);

	if(	(Tape()->State() == Recording) & Variable(*this) ) 
	{
		// add this operation to the tape
		Tape()->RecordOp(AsinOp, result, index);
	}

	return result;
}

template <class Base>
inline AD<Base> asin(const AD<Base> &x)
{	return x.Asin(); }

template <class Base>
inline AD<Base> asin(const VecADelem<Base> &x)
{	return asin( x.ADBase() ); }

} // END CppAD namespace

# endif 
