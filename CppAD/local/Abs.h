# ifndef CppADAbsIncluded
# define CppADAbsIncluded

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
$begin Abs$$
$spell
	Cpp
	namespace
	const
	abs
$$

$index absolute, value$$
$index value, absolute$$
$index abs$$

$section The Absolute Value Function$$

$table
$bold Syntax$$ 
$cnext 
$syntax%AD<%Base%> abs (const AD<%Base%> &%x%)%$$
$tend

$fend 20$$

$head Description$$
Returns an
$syntax%AD<%Base%>%$$ object that is equal to
$syntax%
	abs(%x%)
%$$
where $code abs$$ has the same interpretation as
for the corresponding $xref/glossary/Base Type/base type/$$.


$head Base Type Requirement$$
A definition for the $code abs$$ function
is automatically included (in the $code CppAD$$ namespace)
for the following 
$xref/glossary/Base Type/base types/$$:
$code float$$, $code double$$ and the $code AD$$ types
built upon these base types.

$head Derivative Assumption$$
The derivative of $code abs$$ is computed using the formula
$latex \[
\D{[ {\rm abs} [x(t)] ]}{t} = 
\left\{ \begin{array}{ll} 
	x^{(1)} (t)   & {\rm if} \; x(t) > 0 \\
	x^{(1)} (t)   & {\rm if} \; x(t) = 0 \; {\rm and} \; x'(t) > 0 \\
	0             & {\rm if} \; x(t) = 0 \; {\rm and} \; x'(t) = 0 \\
	- x^{(1)} (t) & {\rm if} \; x(t) = 0 \; {\rm and} \; x'(t) > 0 \\
	- x^{(1)} (t) & {\rm if} \; x(t) < 0 
\end{array} \right.
\] $$



$head Example$$
$children%
	Example/Abs.cpp
%$$
The file
$xref/Abs.cpp/$$
contains an example and a test of this function.   
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

inline float abs(float x)
{	if( x > 0. )
		return x;
	else if( x < 0. )
		return -x;
	return 0.;
}

inline double abs(double x)
{	if( x > 0. )
		return x;
	else if( x < 0. )
		return -x;
	return 0.;
}

inline std::complex<float>  abs(std::complex<float> x)
{	CppADUsageError(
		0,
		"Attempt to use std::complex<float> as an ordered type"
	);
	return false;
}

inline std::complex<double>  abs(std::complex<double> x)
{	CppADUsageError(
		0,
		"Attempt to use std::complex<double> as an ordered type"
	);
	return false;
}


template <class Base>
AD<Base> AD<Base>::Abs (void) const
{	using CppAD::abs;

	AD<Base> result;
	CppADUnknownError( result.id == 0 );

	if( GreaterThanZero(value) )
		result.value = value;
	else if( LessThanZero (value) )
		result.value = - value;
	else	result.value = Base(0);
		

	if(	(Tape()->State() == Recording) & Variable(*this) ) 
	{
		// add this operation to the tape
		Tape()->RecordOp(AbsOp, result, taddr);

	}

	return result;
}

template <class Base>
inline AD<Base> abs(const AD<Base> &x)
{	return x.Abs(); }

template <class Base>
inline AD<Base> abs(const VecADelem<Base> &x)
{	return abs( x.ADBase() ); }

} // END CppAD namespace

# endif 
