# ifndef CppADAcosIncluded
# define CppADAcosIncluded

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
$begin acos$$
$spell
	cos
	Cpp
	namespace
	const
	acos
	Arccosine
$$

$index cos, inverse$$
$index inverse, cos$$
$index acos$$

$section The Inverse Cosine Function$$

$table
$bold Syntax$$ 
$cnext 
$syntax%AD<%Base%> acos (const AD<%Base%> &%x%)%$$
$tend

$fend 20$$

$head Description$$
Returns an
$syntax%AD<%Base%>%$$ object that is equal to
$syntax%
	acos(%x%)
%$$
where $code acos$$ has the same interpretation as
for the corresponding $xref/glossary/Base Type/base type/$$.


$head Base Type Requirement$$
A definition for the $code acos$$ function
is automatically included (in the $code CppAD$$ namespace)
for the following 
$xref/glossary/Base Type/base types/$$:
$code float$$ and $code double$$.
If the $code acos$$ function is used with any other base type, 
it must be a defined for that base type.

$head Derivative Assumption$$
It is assumed that
$latex \[
\begin{array}{lcr}
	\D{[ {\rm acos} (x) ]}{x} & = & - (1 - x * x)^{-1/2}
\end{array}
\] $$



$head Example$$
$children%
	Example/Acos.cpp
%$$
The file
$xref/Acos.cpp/$$
contains an example and a test of this function.   
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {


# ifdef _MSC_VER
	inline float acos(float x)
	{	return ::acos(x); }
	inline double acos(double x)
	{	return ::acos(x); }
# else
	inline float acos(float x)
	{	return std::acos(x); }
	inline double acos(double x)
	{	return std::acos(x); }
# endif

inline std::complex<float> acos(std::complex<float> x)
{	CppADUsageError(
		0,
		"Attempt to use acos with complex argument"
	);
	return x;
}
inline std::complex<double> acos(std::complex<double> x)
{	CppADUsageError(
		0,
		"Attempt to use acos with complex argument"
	);
	return x;
}


template <class Base>
AD<Base> AD<Base>::Acos (void) const
{	using CppAD::acos;

	AD<Base> result;
	CppADUnknownError( result.id == 0 );

	result.value   = acos(value);

	if(	(Tape()->State() == Recording) & Variable(*this) ) 
	{
		// add this operation to the tape
		Tape()->RecordOp(AcosOp, result, taddr);

	}

	return result;
}

template <class Base>
inline AD<Base> acos(const AD<Base> &x)
{	return x.Acos(); }

template <class Base>
inline AD<Base> acos(const VecADelem<Base> &x)
{	return acos( x.ADBase() ); }

} // END CppAD namespace

# endif 
