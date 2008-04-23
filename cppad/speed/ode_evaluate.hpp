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
	fm
	namespace
	exp
$$

$section Evaluate a Function Defined in Terms of an ODE$$

$index ode_evaluate, function$$
$index function, ode_evaluate$$

$head Syntax$$
$codei%# include <cppad/speed/ode_evaluate.hpp>
%$$
$codei%ode_evaluate(%x%, %m%, %fm%)%$$

$head Purpose$$
This routine evaluates a function that is defined by the 
following initial value problem:
$latex \[
\begin{array}{rcl}
	y(x, 0)                & = & b(x)
	\\
	\partial_t y ( x , t ) & = & g[ x , y(x,t) , t ]
\end{array}
\] $$
where $latex b : \R^n \rightarrow \R^n$$ and
$latex g : \R^n \times \R^n \times \R \rightarrow \R$$
are not any further specified. 
A numerical method is used to solve the ode and obtain an accurate
approximation for $latex y(x, 1)$$.
This in turn is used to compute values and gradients for the
function $latex f : \R^n \rightarrow \R$$ defined by
$latex \[
	f(x) = y_n ( x , 1)
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

$subhead Operation Sequence$$
The functions $latex b(x)$$, $latex g(x, y, t)$$ and 
the ODE solver are chosen so that the $icode Float$$ 
operation sequence does not depend on the value of $latex x$$.

$head x$$
The argument $icode x$$ has prototype
$codei%
	const CppAD::vector<%Float%> &%x%
%$$
It contains he argument value for which the function,
or its derivative, is being evaluated.
The value $latex n$$ is determined by the size of the vector $icode x$$.

$head m$$
The argument $italic m$$ has prototype
$icode%
	size_t %m%
%$$
It is either zero or one and
specifies the order of the derivative of $latex f(x)$$,
with respect to $latex x$$, 
that is being evaluated.

$head m = 1$$
In the case where $latex m = 1$$, 
the following extended system is solved:
$latex \[
\begin{array}{rcl}
y(x, 0)                & = & b(x)
\\
\partial_t y ( x , t ) & = & g[ x , y(x,t) , t ]
\\
y_x (x, 0)             & = & b^{(1)} (x)
\\
partial_t y_x (x,  t)  & = & \partial_x g[ x , y(x,t) , t ] 
                         +   \partial_y g[ x , y(x,t) , t ] y_x
\end{array}
\] $$

$head fm$$
The argument $italic fm$$ has prototype
$icode%
	CppAD::vector<%Float%> &%fm%
%$$
The input value of the elements of $icode fm$$ does not matter.

$subhead Function$$
If $icode m$$ is zero, $italic fm$$ has size equal to one
and $icode%fm%[0]%$$ is the value of $latex y(x, 1)$$.

$subhead Gradient$$
If $icode m$$ is one, $italic fm$$ has size equal to $italic n$$ 
and for $latex j = 0 , \ldots , n-1$$
$latex \[
	\D{y}{x[j]} (x, 1) = fm [ j ]
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
		if( m_ == 1 )
			ode_z(t, z, h);
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
		{	g[i]  = Float(int(i+1)) * x_[i] * yi1;
			yi1   = y[i];
		}

		// solution  for this equation is
		// y_0 (t) = x_0 * t
		// y_1 (t) = x_1 * x_0 * t^2 
		// y_2 (t) = x_2 * x_1 * x_0 * t^3
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
		{	h[i] = Float(int(i+1)) * x_[i] * zi1;
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
			gi_xi  = Float(int(i+1)) * z[i-1];
			ij     = n + i * n + i;	
			h[ij] += gi_xi;
			// partial g[i] w.r.t y[i-1] 
			gi_yi1 = Float(int(i+1)) * x_[i];
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
	CppAD::vector<Float> &fm )
{
	typedef CppAD::vector<Float> Vector;

	size_t n = x.size();
	size_t ell;
	CPPAD_ASSERT_KNOWN( m == 0 || m == 1,
		"ode_evaluate: m is not zero or one"
	);
	CPPAD_ASSERT_KNOWN( 
		((m==0) & (fm.size()==1) ) || ((m==1) & (fm.size()==n)),
		"ode_evaluate: the size of fm is not correct"
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

	size_t i;
	for(i = 0; i < ell; i++)
		yi[i] = Float(0);

	// construct ode equation
	ode_evaluate_fun<Float> f(m, x);

	// solve differential equation
	yf = Runge45(f, M, ti, tf, yi);

	if( m == 0 )
		fm[0] = yf[n-1];
	else
	{	for(i = 0; i < n; i++)
			fm[i] = yf[n + (n-1) * n + i];
	}
	return;
}

} // END CppAD namespace
// END PROGRAM

# endif
