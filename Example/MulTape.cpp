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
$begin MulTape.cpp$$
$spell
	abs
$$

$section Multiple Tapes: Example and Test$$
$index multiple, tape$$
$index tape, multiple$$

$head Description$$
This is an example and test of using multiple levels of taping to compute
the value
$latex \[
	\frac{d}{dx} \left[ f^{(1)} (x) * v \right]
\] $$
where $latex f : \R^n \rightarrow \R$$ and
$latex v \in \R^n$$.
The example $xref/HesTimesDir.cpp/$$ computes the same value using only
one level of taping (more efficient) and the identity
$latex \[
	\frac{d}{dx} \left[ f^{(1)} (x) * v \right] = f^{(2)} (x) * v
\] $$


$comment This file is in the Example subdirectory$$
$code
$verbatim%Example/MulTape.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

namespace { // put this function in the empty namespace
	// f(x) = |x|^2 = x[0]^2 + ... + x[n-1]^2
	template <class Type>
	Type f(CppADvector<Type> &x)
	{	Type sum = 0;
		size_t i = x.size();
	
		while(i--)
			sum += x[i] * x[i];
	
		return sum;
	} 
}

bool MulTape() 
{	bool ok = true;                   // initialize test result

	using namespace CppAD;            // so do not need CppAD::
	typedef AD<double>   ADdouble;    // type for one level of taping
	typedef AD<ADdouble> ADDdouble;   // type for two levels of taping
	size_t n = 5;                     // dimension for example
	size_t i;                         // a temporary index variable

	CppADvector<double>    x(n);
	CppADvector<ADdouble>  ax(n);
	CppADvector<ADDdouble> aax(n);

	// value of the independent variables
	for(i = 0; i < n; i++)
		ax[i] = x[i] = double(i);  // x[i] = i
	Independent(ax);                   // ax is indedendent for ADdouble
	for(i = 0; i < n; i++)
		aax[i] = ax[i];            // track how aax depends on ax
	Independent(aax);                  // aax is independent for ADDdouble

	// compute function
	CppADvector<ADDdouble> aaf(1);     // scalar valued fucntion
	aaf[0] = f(aax);                   // has only one component

	// declare inner function (corresponding to ADDdouble calculation)
	ADFun<ADdouble> aF(aax, aaf);

	// compute f'(x) 
	size_t p = 1;                 // order of derivative of aF
	CppADvector<ADdouble> aw(1);  // weight vector for aF
	CppADvector<ADdouble> ag(n);  // gradient of aF
	aw[0] = 1;                    // weighted fucntion is f
	ag    = aF.Reverse(p, aw);    // gradient of f

	// declare outter function (corresponding to ADdouble calculation)
	ADFun<double> G(ax, ag);

	// compute the derivative of f'(x) * v
	CppADvector<double> v(n);
	CppADvector<double> dgv(n);
	for(i = 0; i < n; i++)
		v[i] = double(n - i);
	dgv = G.Reverse(p, v);

	// f(x)      = x[0]^2 + x[1]^2 + ... + x[n-1]^2
	// f'(x)     = 2 (x[0], x[1], ... , x[n-1])
	// f'(x) * v = 2 ( x[0]*v[0] + x[1]*v[1] + ... + x[n-1]*v[n-1] )
	for(i = 0; i < n; i++)
		ok &= NearEqual(dgv[i], 2.*v[i], 1e-10, 1e-10);

	return ok;
}
// END PROGRAM
