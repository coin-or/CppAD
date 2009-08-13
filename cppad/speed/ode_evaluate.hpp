/* $Id$ */
# ifndef CPPAD_ODE_EVALUATE_INCLUDED
# define CPPAD_ODE_EVALUATE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ode_evaluate$$
$spell
	retaped
	Jacobian
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
This routine evaluates a function $latex f : \R^n \rightarrow \R^n$$
defined by 
$latex \[
	f(x) = y(x, 1)
\] $$
where $latex y(x, t)$$ solves the ordinary differential equation
$latex \[
\begin{array}{rcl}
	y(x, 0)                & = & b(x)
	\\
	\partial_t y ( x , t ) & = & g[ x , y(x,t) , t ]
\end{array}
\] $$
where $latex b : \R^n \rightarrow \R^n$$ and
$latex g : \R^n \times \R^n \times \R \rightarrow \R^n$$
are not any further specified. 
A numerical method is used to solve the ode and obtain an accurate
approximation for $latex y(x, 1)$$.
This in turn is used to compute values and Jacobian of the
function $latex f(x)$$.

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
The $icode Float$$ operation sequence for this routine
will depend on the value of the argument $icode x$$
and hence it must be retaped for each new value of $latex x$$. 

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

$head m = 0$$
In this case the function $latex f(x)$$ is evaluated as described above.

$head m = 1$$
In this case 
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
If $icode m$$ is zero, $italic fm$$ has size equal to $latex n$$
and contains the value of $latex y(x, 1)$$.

$subhead Gradient$$
If $icode m$$ is one, $italic fm$$ has size equal to $italic n^2$$ 
and for $latex i = 0 , \ldots and n-1$$, $latex j = 0 , \ldots , n-1$$
$latex \[
	\D{y[i]}{x[j]} (x, 1) = fm [ i \cdot n + j ]
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
# include <cppad/ode_err_control.hpp>
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

		size_t i;
		size_t n = x_.size();
		for(i = 0; i < n; i++)
			g[i]  = x_[i] * y[i];
		// because y_i(0) = 1, solution for this equation is
		// y_0 (t) = t
		// y_1 (t) = exp(x_1 * t)
		// y_2 (t) = exp(2 * x_2 * t)
		// ...
	}
	void ode_z(
		const Float                  &t , 
		const CppAD::vector<Float>   &z , 
		CppAD::vector<Float>         &h ) 
	{	// z    = [ y ; y_x ]
		// z_t  = h(t, x, z) = [ y_t , y_x_t ]
		size_t i, j;
		size_t n = x_.size();
		CPPAD_ASSERT_UNKNOWN( z.size() == n + n * n );

		// y_t
		for(i = 0; i < n; i++)
		{	h[i] = x_[i] * z[i];

			// initialize y_x_t as zero
			for(j = 0; j < n; j++)
				h[n + i * n + j] = 0.;
		}
		for(i = 0; i < n; i++)
		{	// partial of g_i w.r.t y_i
			Float gi_yi = x_[i]; 
			// partial of g_i w.r.t x_i
			Float gi_xi = z[i];
			// partial of y_i w.r.t x_i
			Float yi_xi = z[n + i * n + i];
			// derivative of yi_xi with respect to t 
			h[n + i * n + i] = gi_xi + gi_yi * yi_xi;
		}
	}
};

template <class Float>
class ode_evaluate_method {
private:
	ode_evaluate_fun<Float> F;
public:
	// constructor
	ode_evaluate_method(size_t m, const CppAD::vector<Float> &x) 
	: F(m, x)
	{ }
	void step(
		Float                 ta ,
		Float                 tb ,
		CppAD::vector<Float> &xa ,
		CppAD::vector<Float> &xb ,
		CppAD::vector<Float> &eb )
	{	xb = CppAD::Runge45(F, 1, ta, tb, xa, eb);
	}
	size_t order(void)
	{	return 4; }
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
		((m==0) & (fm.size()==n)) || ((m==1) & (fm.size()==n*n)),
		"ode_evaluate: the size of fm is not correct"
	);
	if( m == 0 )
		ell = n;
	else	ell = n + n * n;

	// set up the case we are integrating
	Float  ti   = 0.;
	Float  tf   = 1.;
	Float  smin = 1e-5;
	Float smax  = 1.;
	Float scur  = 1.;
	Float erel  = 0.;
	vector<Float> yi(ell), eabs(ell);
	size_t i, j;
	for(i = 0; i < ell; i++)
	{	eabs[i] = 1e-10;
		if( i < n )
			yi[i] = 1.;
		else	yi[i]  = 0.;
	}

	// return values
	Vector yf(ell), ef(ell), maxabs(ell);
	size_t nstep;

	// construct ode method for taking one step
	ode_evaluate_method<Float> method(m, x);

	// solve differential equation
	yf = OdeErrControl(method, 
		ti, tf, yi, smin, smax, scur, eabs, erel, ef, maxabs, nstep);

	if( m == 0 )
	{	for(i = 0; i < n; i++)
			fm[i] = yf[i];
	}
	else
	{	for(i = 0; i < n; i++)
			for(j = 0; j < n; j++)
				fm[i * n + j] = yf[n + i * n + j];
	}
	return;
}

} // END CppAD namespace
// END PROGRAM

# endif
