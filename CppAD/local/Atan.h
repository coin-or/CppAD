# ifndef CppADAtanIncluded
# define CppADAtanIncluded

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
$begin Atan$$
$spell
	Cpp
	namespace
	Arctangent
	const
	atan
$$

$index tan, inverse$$
$index inverse, tan$$
$index atan$$

$section The Inverse Tangent Function$$

$table
$bold Syntax$$ 
$cnext 
$syntax%AD<%Base%> atan (const AD<%Base%> &%x%)%$$
$tend

$fend 20$$

$head Description$$
Returns an
$syntax%AD<%Base%>%$$ object that is equal to
$syntax%
	atan(%x%)
%$$
where $code atan$$ has the same interpretation as
for the corresponding $xref/glossary/Base Type/base type/$$.

$head Base Type Requirement$$
A definition for the $code atan$$ function
is automatically included (in the $code CppAD$$ namespace)
for the following 
$code float$$ and $code double$$
$xref/glossary/Base Type/base types/$$.
If the $code atan$$ function is used with any other base type, 
it must be a defined for that base type.

$head Derivative Assumption$$
It is assumed that
$latex \[
\begin{array}{lcr}
	\D{[ {\rm atan} (x) ]}{x} & = & \frac{1}{1 + x^2}  
\end{array}
\] $$



$head Example$$
$children%
	Example/Atan.cpp
%$$
The file
$xref/Atan.cpp/$$
contains an example and a test of this function.   
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

# ifdef _MSC_VER
	inline float atan(float x)
	{	return ::atan(x); }
	inline double atan(double x)
	{	return ::atan(x); }
# else
	inline float atan(float x)
	{	return std::atan(x); }
	inline double atan(double x)
	{	return std::atan(x); }
# endif

inline std::complex<float> atan(std::complex<float> x)
{	CppADUsageError(
		0,
		"Attempt to use atan with complex argument"
	);
	return x;
}
inline std::complex<double> atan(std::complex<double> x)
{	CppADUsageError(
		0,
		"Attempt to use atan with complex argument"
	);
	return x;
}


template <class Base>
AD<Base> AD<Base>::Atan (void) const
{	using CppAD::atan;

	AD<Base> result;
	CppADUnknownError( result.id == 0 );

	result.value   = atan(value);

	if(	(Tape()->State() == Recording) & Variable(*this) ) 
	{
		// add this operation to the tape
		Tape()->RecordOp(AtanOp, result, taddr);
	}

	return result;
}

template <class Base>
inline AD<Base> atan(const AD<Base> &x)
{	return x.Atan(); }

template <class Base>
inline AD<Base> atan(const VecADelem<Base> &x)
{	return atan( x.ADBase() ); }

} // END CppAD namespace

# endif 
