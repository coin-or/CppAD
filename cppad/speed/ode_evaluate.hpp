# ifndef CPPAD_ODE_EVALUATE_INCLUDED
# define CPPAD_ODE_EVALUATE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ode_evaluate$$
$spell
	const
	Cpp
	cppad
	hpp	
	ym
	namespace
	exp
$$

$section Evaluate a Function Defined in Terms of an ODE$$

$index ode_evaluate, function$$
$index function, ode_evaluate$$

$head Syntax$$
$codei%# include <cppad/speed/ode_evaluate.hpp>
%$$
$codei%ode_evaluate(%x%, %m%, %ym%)%$$

$head Purpose$$
This routine evaluates either $latex y(x, 1) \in \R^1$$ or 
$latex \partial_x y(x, 1) \in \R^n$$
where $latex y(x, t)$$ is defined as the solution of an ordinary 
differential equation with parameter vector $latex x$$. 
To be more specific, the function $latex y(x, t)$$ is defined by
$latex \[
\begin{array}{rcl}
	\partial_t y(x, t)  & = & g[ t , y(x, t) ] \\
	y(x, 0)             & = & g_0 (x)
\end{array}
\] $$

$head Inclusion$$
The template function $code ode_evaluate$$ 
is defined in the $code CppAD$$ namespace by including 
the file $code cppad/speed/ode_evaluate.hpp$$ 
(relative to the CppAD distribution directory).
It is only intended for example and testing purposes, 
so it is not automatically included by
$cref/cppad.hpp/cppad/$$.

$head Float$$
The type $icode Float$$ must be a $cref/NumericType/$$.
In addition, if $icode y$$ and $icode z$$ are $icode Float$$ objects,
$codei%
	%y% = exp(%z%)
%$$ 
must set the $icode y$$ equal the exponential of $icode z$$, i.e.,
the derivative of $icode y$$ with respect to $icode z$$ is equal to $icode y$$.

$head x$$
The argument $icode x$$ has prototype
$codei%
	const CppAD::vector<%Float%> &%x%
%$$
It contains he argument value for which the function,
or its derivative, is being evaluated.
We use $latex n$$ to denote the size of the vector $icode x$$.

$head m$$
The argument $italic m$$ has prototype
$icode%
	size_t %m%
%$$
It is either zero or one and
specifies the order of the derivative of $latex y(x, 1)$$,
with respect to $latex x$$,
that is being evaluated.

$head ym$$
The argument $italic ym$$ has prototype
$icode%
	CppAD::vector<%Float%> &%ym%
%$$
The input value of the elements of $icode ym$$ does not matter.

$subhead Function$$
If $icode m$$ is zero, $italic ym$$ has size one and
$syntax%%ym%[0]%$$ is the value of $latex y(x, 1)$$.

$subhead Gradient$$
If $icode m$$ is one, $italic ym$$ has size $italic n$$ and 
for $latex j = 0 , \ldots , n-1$$
$latex \[
	\D{y}{x[j]} (x, 1) = ym [ j ]
\] $$

$children%
	speed/example/ode_evaluate.cpp%
	omh/ode_evaluate.omh
%$$

$head Example$$
The file
$cref/ode_evaluate.cpp/$$
contains an example and test  of $code ode_evaluate.hpp$$.
It returns true if it succeeds and false otherwise.


$head Source Code$$
The file 
$cref/ode_evaluate.hpp/$$
contains the source code for this template function.


$end
*/
// BEGIN PROGRAM
# include <cppad/vector.hpp>
# include <cppad/runge_45.hpp>

namespace CppAD {  // BEGIN CppAD namespace

template <class Float>
class ode_evaluate_fun {
private:
	const size_t m_;
	const CppAD::vector<Float> x_;
public:
	ode_evaluate_fun(size_t m, const CppAD::vector<Float> &x) 
	: m_(m), x_(x)
	{ }
	void Ode(
		const Float                  &t, 
		const CppAD::vector<Float>   &z, 
		CppAD::vector<Float>         &h) 
	{
		if( m_ == 0 )
			ode_y(t, z, h);
		else	ode_z(t, z, h);
	}
	void ode_y(
		const Float                  &t, 
		const CppAD::vector<Float>   &y, 
		CppAD::vector<Float>         &g) 
	{	// y_t = g(t, x, y)
		CPPAD_ASSERT_UNKNOWN( y.size() == x_.size() );

		size_t i, n = x_.size();
		Float yi1 = Float(1);
		for(i = 0; i < n; i++)
		{	g[i]  = x_[i] * yi1;
			yi1   = y[i];
		}

		// solution  for this equation is
		// y_0 (t) = x_0 * t / 1 !
		// y_1 (t) = x_1 * x_0 * t^2 / 2 !
		// y_2 (t) = x_2 * x_1 * x_0 * t^3 / 3 !
		// ...
	}
	void ode_z(
		const Float                  &t , 
		const CppAD::vector<Float>   &z , 
		CppAD::vector<Float>         &h ) 
	{	// z    = [ y ; y_x ]
		// z_t  = h(t, x, z) = [ y_t , y_x_t ]
		size_t i, j, n = x_.size();
		CPPAD_ASSERT_UNKNOWN( z.size() == n + n * n );

		// y_t
		Float zi1 = Float(1);
		for(i = 0; i < n; i++)
		{	h[i] = x_[i] * zi1;
			for(j = 0; j < n; j++)
				h[n + i * n + j] = 0.;
			zi1 = z[i];
		}
		size_t ij;
		Float gi_xi, gi_yi1, yi1_xj;

		// y0_x0_t
		h[n] += 1.;

		// yi_xj_t
		for(i = 1; i < n; i++)
		{	// partial g[i] w.r.t. x[i]
			gi_xi  = z[i-1];
			ij     = n + i * n + i;	
			h[ij] += gi_xi;
			// partial g[i] w.r.t y[i-1] 
			gi_yi1 = x_[i];
			// multiply by partial y[i-1] w.r.t x[j];
			for(j = 0; j < n; j++)
			{	ij     = n + (i-1) * n + j;
				yi1_xj = z[ij];
				ij     = n + i * n + j;
				h[ij] += gi_yi1 * yi1_xj;
			} 
		}
	}
};

template <class Float>
void ode_evaluate(
	CppAD::vector<Float> &x  , 
	size_t m                 , 
	CppAD::vector<Float> &ym )
{
	typedef CppAD::vector<Float> Vector;

	size_t n = x.size();
	size_t ell;
	CPPAD_ASSERT_KNOWN( m == 0 || m == 1,
		"ode_evaluae: m is not zero or one"
	);
	if( m == 0 )
		ell = n;
	else	ell = n + n * n;

	// set up the case we are integrating
	size_t M  = 10;
	Float  ti = 0.;
	Float  tf = 1.;
	Vector yi(ell);
	Vector yf(ell);
	Vector e(ell);

	size_t i;
	for(i = 0; i < ell; i++)
		yi[i] = Float(0);

	// construct ode equation
	ode_evaluate_fun<Float> f(m, x);

	// solve differential equation
	yf = Runge45(f, M, ti, tf, yi);

	if( m == 0 )
		ym[0] = yf[n-1];
	else
	{	for(i = 0; i < n; i++)
			ym[i] = yf[n + (n-1) * n + i];
	}
	return;
}

} // END CppAD namespace
// END PROGRAM

# endif
