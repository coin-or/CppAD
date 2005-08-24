# ifndef CppADRombergMulIncluded
# define CppADRombergMulIncluded

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
$begin RombergMul$$
$spell
	bool
	const
	Cpp
	RombergMulMul
$$

$section Multi-dimensional Romberg Integration$$

$index integrate, multi-dimensional Romberg$$
$index Romberg, multi-dimensional integrate$$
$index multi, dimensional Romberg integration$$
$index dimension, multi Romberg integration$$

$table
$bold Syntax$$
$cnext $code # include <CppAD/RombergMul.h>$$ 
$rnext $cnext
$syntax%RombergMul<%Fun%, %SizeVector%, %FloatVector%, %m%> %R%$$
$rnext $cnext
$syntax%%r% = %R%(%F%, %a%, %b%, %n%, %p%, %e%)%$$
$tend

$fend 20$$

$head Description$$
Returns the Romberg integration estimate
$latex r$$ for the multi-dimensional integral
$latex \[
r = 
\int_{a[0]}^{b[0]} \cdots \int_{a[m-1]}^{b[m-1]}
\; F(x) \;
{\bf d} x_0 \cdots {\bf d} x_{m-1} 
\; + \; 
\sum_{i=0}^{m-1} 
O \left[ ( b[i] - a[i] ) / 2^{n[i]-1} \right]^{2(p[i]+1)}
\] $$

$head Include$$
The file $code CppAD/RombergMul.h$$ is included by $code CppAD/CppAD.h$$
but it can also be included separately with out the rest of 
the $code CppAD$$ routines.

$head m$$
The template parameter $italic m$$ must be convertible to a $code size_t$$ 
object with a value that can be determined at compile time; for example
$code 2$$.
It determines the dimension of the domain space for the integration.

$head r$$
The return value $italic r$$ has prototype
$syntax%
	%Float% %r%
%$$ 
It is the estimate computed by $code RombergMul$$ for the integral above
(see description of $xref/RombergMul/Float/Float/$$ below). 

$head F$$
The object $italic F$$ has the prototype
$syntax%
	%Fun% &%F%
%$$
It must support the operation
$syntax%
	%F%(%x%)
%$$
The argument $italic x$$ to $italic F$$ has prototype
$syntax%
	const %Float% &%x%
%$$
The return value of $italic F$$ is a $italic Float$$ object

$head a$$
The argument $italic a$$ has prototype
$syntax%
	const %FloatVector% &%a%
%$$ 
It specifies the lower limit for the integration
(see description of $xref/RombergMul/FloatVector/FloatVector/$$ below). 

$head b$$
The argument $italic b$$ has prototype
$syntax%
	const %FloatVector% &%b%
%$$ 
It specifies the upper limit for the integration.

$head n$$
The argument $italic n$$ has prototype
$syntax%
	const %SizeVector% &%n%
%$$ 
A total number of $latex 2^{n[i]-1} + 1$$ 
evaluations of $syntax%%F%(%x%)%$$ are used to estimate the integral
with respect to $latex {\bf d} x_i$$.

$head p$$
The argument $italic p$$ has prototype
$syntax%
	const %SizeVector% &%p%
%$$ 
For $latex i = 0 , \ldots , m-1$$,
$latex n[i]$$ determines the accuracy order in the 
approximation for the integral 
that is returned by $code RombergMul$$.
The values in $italic p$$ must be less than or equal $italic n$$; i.e.,
$syntax%%p%[%i%] <= %n%[%i%]%$$.

$head e$$
The argument $italic e$$ has prototype
$syntax%
	%Float% &%e%
%$$ 
The input value of $italic e$$ does not matter
and its output value is an approximation for the absolute error in 
the integral estimate.

$head Float$$
The type float is defined as the type of the elements of
$xref/RombergMul/FloatVector/FloatVector/$$.
The following operations must be defined for $italic Float$$ objects
$italic x$$ and $italic y$$:


$table
$bold Operation$$       
	$cnext $bold Description$$ $rnext
$syntax%%Float%(%i%)%$$ 
	$cnext converts $italic i$$ to $italic Float$$    $rnext
$syntax%%x% = %y%$$     
	$cnext assigns current $italic y$$ to $italic x$$ $rnext
$syntax%%x% + %y%$$     
	$cnext returns sum of $italic x$$ and $italic y$$ $rnext
$syntax%%x% - %y%$$     
	$cnext returns difference of $italic x$$ and $italic y$$ $rnext
$syntax%%x% * %y%$$     
	$cnext returns quotient of $italic x$$ and $italic y$$ $rnext
$syntax%%x% / %y%$$     
	$cnext returns quotient of $italic x$$ and $italic y$$ $rnext
$syntax%%x% < %y%$$     
	$cnext returns the $code bool$$ value true if 
	$italic x$$ is less than $italic y$$                   $rnext
$tend

$head FloatVector$$
The type $italic FloatVector$$ must be a $xref/SimpleVector/$$ class.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.


$children%
	Example/RombergMul.cpp
%$$
$head Example$$
$comment%
	Example/RombergMul.cpp
%$$
The file
$xref/RombergMul.cpp/$$
contains an example and test a test of using this routine.
It returns true if it succeeds and false otherwise.

$head Source Code$$
The source code for this routine is in the file
$code CppAD/RombergMul.h$$.

$end
*/

# include <CppAD/RombergOne.h>
# include <CppAD/CheckSimpleVector.h>

namespace CppAD { // BEGIN CppAD namespace

template <class Fun, class FloatVector>
class SliceLast {
	typedef typename FloatVector::value_type Float;
private:
	Fun        *F;
	size_t      last;
	FloatVector x;
public:
	SliceLast( Fun *F_, size_t last_, const FloatVector &x_ ) 
	: F(F_) , last(last_), x(last + 1)
	{	size_t i;
		for(i = 0; i < last; i++)
			x[i] = x_[i];
	}
	double operator()(const Float &xlast)
	{	x[last] = xlast;
		return (*F)(x);
	}
};

template <class Fun, class SizeVector, class FloatVector, class Float>
class IntegrateLast {
private:
	Fun                 *F; 
	const size_t        last;
	const FloatVector   a; 
	const FloatVector   b; 
	const SizeVector    n; 
	const SizeVector    p; 
	Float               esum;
	size_t              ecount;

public:
	IntegrateLast(
		Fun                *F_    , 
		size_t              last_ ,
		const FloatVector  &a_    , 
		const FloatVector  &b_    , 
		const SizeVector   &n_    , 
		const SizeVector   &p_    ) 
	: F(F_) , last(last_), a(a_) , b(b_) , n(n_) , p(p_) 
	{ }		
	Float operator()(const FloatVector           &x)
	{	Float r, e;
		SliceLast<Fun, FloatVector           > S(F, last, x);
		r     = CppAD::RombergOne(
			S, a[last], b[last], n[last], p[last], e
		);
		esum = esum + e;
		ecount++;
		return r;
	}
	void ClearEsum(void)
	{	esum   = 0.; }
	Float GetEsum(void)
	{	return esum; }

	void ClearEcount(void)
	{	ecount   = 0; }
	size_t GetEcount(void)
	{	return ecount; }
};

template <class Fun, class SizeVector, class FloatVector, size_t m>
class RombergMul {
	typedef typename FloatVector::value_type Float;
public:
	RombergMul(void)
	{	}
	Float operator() (
		Fun                 &F  , 
		const FloatVector   &a  ,
		const FloatVector   &b  ,
		const SizeVector    &n  ,
		const SizeVector    &p  ,
		Float               &e  )
	{	Float r;

		typedef IntegrateLast<
			Fun         , 
			SizeVector  , 
			FloatVector , 
			Float       > IntegrateOne;

		IntegrateOne Fm1(&F, m-1, a, b, n, p);
		RombergMul<
			IntegrateOne, 
			SizeVector  ,
			FloatVector ,
			m-1         > RombergMulM1;

		Fm1.ClearEsum();
		Fm1.ClearEcount();

		r  = RombergMulM1(Fm1, a, b, n, p, e);

		size_t i, j;
		Float prod = 1;
		size_t pow2 = 1;
		for(i = 0; i < m-1; i++)
		{	prod *= (b[i] - a[i]);
			for(j = 0; j < (n[i] - 1); j++)
				pow2 *= 2;
		}
		assert( Fm1.GetEcount() == (pow2+1) );
		
		e = e + Fm1.GetEsum() * prod / Fm1.GetEcount();

		return r;
	}
};

template <class Fun, class SizeVector, class FloatVector>
class RombergMul <Fun, SizeVector, FloatVector, 1> {
	typedef typename FloatVector::value_type Float;
public:
	Float operator() (
		Fun                 &F  , 
		const FloatVector   &a  ,
		const FloatVector   &b  ,
		const SizeVector    &n  ,
		const SizeVector    &p  ,
		Float               &e  )
	{	Float r;
		typedef IntegrateLast<
			Fun         , 
			SizeVector  , 
			FloatVector , 
			Float       > IntegrateOne;

		// check simple vector class specifications
		CheckSimpleVector<Float, FloatVector>();

		IntegrateOne F0(&F, 0, a, b, n, p);

		F0.ClearEsum();
		F0.ClearEcount();

		r  = F0(a);

		assert( F0.GetEcount() == 1 );
		e = F0.GetEsum();

		return r;
	}
};

} // END CppAD namespace

# endif
