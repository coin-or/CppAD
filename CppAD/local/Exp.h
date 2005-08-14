# ifndef CppADExpIncluded
# define CppADExpIncluded

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
$begin exp$$
$spell
	Cpp
	namespace
	const
	exp
$$

$index inverse, log$$
$index log, inverse$$
$index exp$$

$section The Exponential Function$$

$table
$bold Syntax$$ 
$cnext 
$syntax%AD<%Base%> exp (const AD<%Base%> &%x%)%$$
$tend

$fend 20$$

$head Description$$
Returns an
$syntax%AD<%Base%>%$$ object that is equal to
$syntax%
	exp(%x%)
%$$
where $code exp$$ has the same interpretation as
for the corresponding $xref/glossary/Base Type/base type/$$.

$head Base Type Requirement$$
A definition for the $code exp$$ function
is automatically included (in the $code CppAD$$ namespace)
for the following 
$xref/glossary/Base Type/base types/$$:
$code float$$, $code double$$, $code complex<float>$$, 
and $code complex<double>$$.
If the $code exp$$ function is used with any other base type, 
it must be a defined for that base type.

$head Derivative Assumption$$
It is assumed that
$latex \[
\begin{array}{lcr}
	\D{[ \exp (x) ]}{x} & = & \exp (x)
\end{array}
\] $$


$head Example$$
$children%
	Example/Exp.cpp
%$$
The file
$xref/Exp.cpp/$$
contains an example and a test of this function.   
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

CppADStandardMathFun(exp)

template <class Base>
AD<Base> AD<Base>::Exp (void) const
{	using CppAD::exp;

	AD<Base> result;
	CppADUnknownError( result.id == 0 );

	result.value   = exp(value);

	if(	(Tape()->State() == Recording) & Variable(*this) ) 
	{	// add this operation to the tape
		Tape()->RecordOp(ExpOp, result, taddr);
	}

	return result;
}

template <class Base>
inline AD<Base> exp(const AD<Base> &x)
{	return x.Exp(); }

template <class Base>
inline AD<Base> exp(const VecADelem<Base> &x)
{	return exp( x.ADBase() ); }

} // END CppAD namespace

# endif 
