/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin mul_level_adolc.cpp$$
$spell
	CppAD
	Adolc
	adouble
	abs
$$

$section Using Adolc with Multiple Levels of Taping: Example and Test$$
$index multiple, Adolc$$
$index level, multiple Adolc$$
$index Adolc, multiple level$$

$head Purpose$$
This is an example and test of using Adolc's $code adouble$$ type,
together with CppAD's $syntax%AD<adouble>%$$ type,
for multiple levels of taping.
The example computes
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
The example $cref/mul_level/$$ computes the same values using
$code AD<double>$$ and $code AD< AD<double> >$$.

$code
$verbatim%example/mul_level_adolc.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>

# include <adolc/adouble.h>
# include <adolc/interfaces.h>

// adouble definitions not in Adolc distribution and 
// required in order to use CppAD<adouble>

inline bool IdenticalZero(const adouble &x)
{	return false; }

inline bool IdenticalOne(const adouble &x)
{	return false; }

inline bool IdenticalEqual(const adouble &x, const adouble &y)
{	return false; }

inline adouble CondExpOp(
	enum CppAD::CompareOp      cop ,
	const adouble            &left ,
	const adouble           &right ,
	const adouble        &trueCase ,
	const adouble       &falseCase )
{	return CppAD::CondExpTemplate(cop, left, right, trueCase, falseCase);
}

inline bool GreaterThanZero(const adouble &x)
{    return (x > 0); }

inline bool GreaterThanOrZero(const adouble &x)
{    return (x >= 0); }

inline bool LessThanZero(const adouble &x)
{    return (x < 0); }

inline bool LessThanOrZero(const adouble &x)
{    return (x <= 0); }

inline int Integer(const adouble &x)
{    return static_cast<int>( x.value() ); }


namespace { // put this function in the empty namespace

	// f(x) = |x|^2 = .5 * ( x[0]^2 + ... + x[n-1]^2 + .5 )
	template <class Type>
	Type f(CppADvector<Type> &x)
	{	Type sum;

		// check assignment of AD< AD<double> > = double
		sum  = .5;
		sum += .5;

		size_t i = x.size();
		while(i--)
			sum += x[i] * x[i];

		// check computed assignment AD< AD<double> > -= int
		sum -= 1; 
	
		// check double * AD< AD<double> > 
		return .5 * sum;
	} 
}

bool mul_level_adolc() 
{	bool ok = true;                   // initialize test result

	using namespace CppAD;            // so do not need CppAD::
	typedef adouble      ADdouble;    // type for one level of taping
	typedef AD<ADdouble> ADDdouble;   // type for two levels of taping
	size_t n = 5;                     // number of independent variables

	CppADvector<double>       x(n);
	CppADvector<ADdouble>   a_x(n);
	CppADvector<ADDdouble> aa_x(n);

	// value of the independent variables
	int tag = 0;                         // Adolc setup
	int keep = 1;
	trace_on(tag, keep);
	size_t j;
	for(j = 0; j < n; j++)
	{	x[j] = double(j);           // x[j] = j
		a_x[j] <<= x[j];            // a_x is independent for ADdouble
	}
	for(j = 0; j < n; j++)
		aa_x[j] = a_x[j];          // track how aa_x depends on a_x
	Independent(aa_x);                 // aa_x is independent for ADDdouble

	// compute function
	CppADvector<ADDdouble> aa_f(1);    // scalar valued function
	aa_f[0] = f(aa_x);                 // has only one component

	// declare inner function (corresponding to ADDdouble calculation)
	ADFun<ADdouble> a_F(aa_x, aa_f);

	// compute f'(x) 
	size_t p = 1;                  // order of derivative of a_F
	CppADvector<ADdouble> a_w(1);  // weight vector for a_F
	CppADvector<ADdouble> a_df(n); // value of derivative
	a_w[0] = 1;                    // weighted function is same as a_F
	a_df   = a_F.Reverse(p, a_w);  // gradient of f

	// declare outter function (corresponding to ADdouble calculation)
	double *df = new double[n];
	for(j = 0; j < n; j++)
		a_df[j] >>= df[j];
	trace_off();

	// compute the d/dx of f'(x) * v = f''(x) * v
	size_t m      = n;               // # dependent variables in f'(x)
	double *v     = new double[m];   // vector of weights
	double *ddf_v = new double[n];   // result for f''(x) * v
	for(j = 0; j < n; j++)
		v[j] = double(n - j);
	fos_reverse(tag, m, n, v, ddf_v);

	// f(x)       = .5 * ( x[0]^2 + x[1]^2 + ... + x[n-1]^2 )
	// f'(x)      = (x[0], x[1], ... , x[n-1])
	// f''(x) * v = ( v[0], v[1],  ... , x[n-1] )
	for(j = 0; j < n; j++)
		ok &= NearEqual(ddf_v[j], v[j], 1e-10, 1e-10);

	return ok;
}
// END PROGRAM
