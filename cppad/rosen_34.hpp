# ifndef CPPAD_ROSEN_34_INCLUDED
# define CPPAD_ROSEN_34_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Rosen34$$
$spell
	cppad.hpp
	bool
	xf
	templated
	const
	Rosenbrock
	CppAD
	xi
	ti
	tf
	Karp
	Rosen
	Shampine
	ind
	dep
$$

$index Rosen34$$
$index ODE, Rosenbrock$$
$index Rosenbrock, ODE$$
$index solve, ODE$$
$index stiff, ODE$$
$index differential, equation$$
$index equation, differential$$
 
$section A 3rd and 4th Order Rosenbrock ODE Solver$$

$head Syntax$$
$syntax%# include <cppad/rosen_34.hpp>
%$$
$syntax%%xf% = Rosen34(%F%, %M%, %ti%, %tf%, %xi%)
%$$
$syntax%%xf% = Rosen34(%F%, %M%, %ti%, %tf%, %xi%, %e%)
%$$


$head Description$$
This is an embedded 3rd and 4th order Rosenbrock ODE solver 
(see Section 16.6 of $xref/Bib/Numerical Recipes/Numerical Recipes/$$
for a description of Rosenbrock ODE solvers).
In particular, we use the formulas taken from page 100 of
$xref/Bib/Shampine, L.F./Shampine, L.F./$$
(except that the fraction 98/108 has been correction to be 97/108).
$pre

$$
We use $latex n$$ for the size of the vector $italic xi$$.
Let $latex \R$$ denote the real numbers
and let $latex F : \R \times \R^n \rightarrow \R^n$$ be a smooth function.
The return value $italic xf$$ contains a 5th order
approximation for the value $latex X(tf)$$ where 
$latex X : [ti , tf] \rightarrow \R^n$$ is defined by 
the following initial value problem:
$latex \[
\begin{array}{rcl}
	X(ti)  & = & xi    \\
	X'(t)  & = & F[t , X(t)] 
\end{array}
\] $$
If your set of  ordinary differential equations are not stiff
an explicit method may be better (perhaps $xref/Runge45/$$.)

$head Include$$
The file $code cppad/rosen_34.hpp$$ is included by $code cppad/cppad.hpp$$
but it can also be included separately with out the rest of 
the $code CppAD$$ routines.

$head xf$$
The return value $italic xf$$ has the prototype
$syntax%
	%Vector% %xf%
%$$
and the size of $italic xf$$ is equal to $italic n$$ 
(see description of $xref/Rosen34/Vector/Vector/$$ below).
$latex \[
	X(tf) = xf + O( h^5 )
\] $$
where $latex h = (tf - ti) / M$$ is the step size.
If $italic xf$$ contains not a number $cref/nan/$$,
see the discussion of $cref/f/Rosen34/Fun/Nan/$$.

$head Fun$$
The class $italic Fun$$ 
and the object $italic F$$ satisfy the prototype
$syntax%
	%Fun% &%F%
%$$
This must support the following set of calls
$syntax%
	%F%.Ode(%t%, %x%, %f%)
	%F%.Ode_ind(%t%, %x%, %f_t%)
	%F%.Ode_dep(%t%, %x%, %f_x%)
%$$

$subhead t$$
In all three cases, 
the argument $italic t$$ has prototype
$syntax%
	const %Scalar% &%t%
%$$
(see description of $xref/Rosen34/Scalar/Scalar/$$ below). 

$subhead x$$
In all three cases,
the argument $italic x$$ has prototype
$syntax%
	const %Vector% &%x%
%$$
and has size $italic n$$
(see description of $xref/Rosen34/Vector/Vector/$$ below). 

$subhead f$$
The argument $italic f$$ to $syntax%%F%.Ode%$$ has prototype
$syntax%
	%Vector% &%f%
%$$
On input and output, $italic f$$ is a vector of size $italic n$$
and the input values of the elements of $italic f$$ do not matter.
On output,
$italic f$$ is set equal to $latex F(t, x)$$
(see $italic F(t, x)$$ in $xref/Rosen34/Description/Description/$$). 

$subhead f_t$$
The argument $italic f_t$$ to $syntax%%F%.Ode_ind%$$ has prototype
$syntax%
	%Vector% &%f_t%
%$$
On input and output, $italic f_t$$ is a vector of size $italic n$$
and the input values of the elements of $italic f_t$$ do not matter.
On output, the $th i$$ element of
$italic f_t$$ is set equal to $latex \partial_t F_i (t, x)$$ 
(see $italic F(t, x)$$ in $xref/Rosen34/Description/Description/$$). 

$subhead f_x$$
The argument $italic f_x$$ to $syntax%%F%.Ode_dep%$$ has prototype
$syntax%
	%Vector% &%f_x%
%$$
On input and output, $italic f_x$$ is a vector of size $syntax%%n%*%n%$$
and the input values of the elements of $italic f_x$$ do not matter.
On output, the [$syntax%%i%*%n%+%j%$$] element of
$italic f_x$$ is set equal to $latex \partial_{x(j)} F_i (t, x)$$ 
(see $italic F(t, x)$$ in $xref/Rosen34/Description/Description/$$). 

$subhead Nan$$
If any of the elements of $italic f$$, $italic f_t$$, or $italic f_x$$
have the value not a number $code nan$$,
the routine $code Rosen34$$ returns with all the
elements of $italic xf$$ and $italic e$$ equal to $code nan$$.

$subhead Warning$$
The arguments $italic f$$, $italic f_t$$, and $italic f_x$$
must have a call by reference in their prototypes; i.e.,
do not forget the $code &$$ in the prototype for 
$italic f$$, $italic f_t$$ and $italic f_x$$.

$subhead Optimization$$
Every call of the form 
$syntax%
	%F%.Ode_ind(%t%, %x%, %f_t%)
%$$
is directly followed by a call of the form 
$syntax%
	%F%.Ode_dep(%t%, %x%, %f_x%)
%$$
where the arguments $italic t$$ and $italic x$$ have not changed between calls.
In many cases it is faster to compute the values of $italic f_t$$
and $italic f_x$$ together and then pass them back one at a time.

$head M$$
The argument $italic M$$ has prototype
$syntax%
	size_t %M%
%$$
It specifies the number of steps
to use when solving the differential equation.
This must be greater than or equal one.
The step size is given by $latex h = (tf - ti) / M$$, thus
the larger $italic M$$, the more accurate the
return value $italic xf$$ is as an approximation
for $latex X(tf)$$.

$head ti$$
The argument $italic ti$$ has prototype
$syntax%
	const %Scalar% &%ti%
%$$
(see description of $xref/Rosen34/Scalar/Scalar/$$ below). 
It specifies the initial time for $italic t$$ in the 
differential equation; i.e., 
the time corresponding to the value $italic xi$$.

$head tf$$
The argument $italic tf$$ has prototype
$syntax%
	const %Scalar% &%tf%
%$$
It specifies the final time for $italic t$$ in the 
differential equation; i.e., 
the time corresponding to the value $italic xf$$.

$head xi$$
The argument $italic xi$$ has the prototype
$syntax%
	const %Vector% &%xi%
%$$
and the size of $italic xi$$ is equal to $italic n$$.
It specifies the value of $latex X(ti)$$

$head e$$
The argument $italic e$$ is optional and has the prototype
$syntax%
	%Vector% &%e%
%$$
If $italic e$$ is present,
the size of $italic e$$ must be equal to $italic n$$.
The input value of the elements of $italic e$$ does not matter.
On output
it contains an element by element
estimated bound for the absolute value of the error in $italic xf$$
$latex \[
	e = O( h^4 )
\] $$
where $latex h = (tf - ti) / M$$ is the step size.

$head Scalar$$
The type $italic Scalar$$ must satisfy the conditions
for a $xref/NumericType/$$ type.
The routine $xref/CheckNumericType/$$ will generate an error message
if this is not the case.
In addition, the following operations must be defined for 
$italic Scalar$$ objects $italic a$$ and $italic b$$:

$table
$bold Operation$$ $cnext $bold Description$$  $rnext
$syntax%%a% < %b%$$ $cnext
	less than operator (returns a $code bool$$ object)
$tend

$head Vector$$
The type $italic Vector$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type Scalar/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head Example$$
$children%
	example/rosen_34.cpp
%$$
The file
$xref/Rosen34.cpp/$$
contains an example and test a test of using this routine.
It returns true if it succeeds and false otherwise.

$head Source Code$$
The source code for this routine is in the file
$code cppad/rosen_34.hpp$$.

$end
--------------------------------------------------------------------------
*/

# include <cstddef>
# include <cppad/local/cppad_assert.hpp>
# include <cppad/check_simple_vector.hpp>
# include <cppad/check_numeric_type.hpp>
# include <cppad/vector.hpp>
# include <cppad/lu_factor.hpp>
# include <cppad/lu_invert.hpp>

namespace CppAD { // BEGIN CppAD namespace

template <typename Scalar, typename Vector, typename Fun>
Vector Rosen34(
	Fun           &F , 
	size_t         M , 
	const Scalar &ti , 
	const Scalar &tf , 
	const Vector &xi )
{	Vector e( xi.size() );
	return Rosen34(F, M, ti, tf, xi, e);
}

template <typename Scalar, typename Vector, typename Fun>
Vector Rosen34(
	Fun           &F , 
	size_t         M , 
	const Scalar &ti , 
	const Scalar &tf , 
	const Vector &xi ,
	Vector       &e )
{
	// check numeric type specifications
	CheckNumericType<Scalar>();

	// check simple vector class specifications
	CheckSimpleVector<Scalar, Vector>();

	// Parameters for Shampine's Rosenbrock method
	// are static to avoid recalculation on each call and 
	// do not use Vector to avoid possible memory leak
	static Scalar a[3] = {
		Scalar(0),
		Scalar(1),
		Scalar(3)   / Scalar(5)
	};
	static Scalar b[2 * 2] = {
		Scalar(1),
		Scalar(0),
		Scalar(24)  / Scalar(25),
		Scalar(3)   / Scalar(25)
	};
	static Scalar ct[4] = {
		Scalar(1)   / Scalar(2),
		- Scalar(3) / Scalar(2),
		Scalar(121) / Scalar(50),
		Scalar(29)  / Scalar(250)
	};
	static Scalar cg[3 * 3] = {
		- Scalar(4),
		Scalar(0),
		Scalar(0),
		Scalar(186) / Scalar(25),
		Scalar(6)   / Scalar(5),
		Scalar(0),
		- Scalar(56) / Scalar(125),
		- Scalar(27) / Scalar(125),
		- Scalar(1)  / Scalar(5)
	};
	static Scalar d3[3] = {
		Scalar(97) / Scalar(108),
		Scalar(11) / Scalar(72),
		Scalar(25) / Scalar(216)
	};
	static Scalar d4[4] = {
		Scalar(19)  / Scalar(18),
		Scalar(1)   / Scalar(4),
		Scalar(25)  / Scalar(216),
		Scalar(125) / Scalar(216)
	};
	CPPAD_ASSERT_KNOWN(
		M >= 1,
		"Error in Rosen34: the number of steps is less than one"
	);
	CPPAD_ASSERT_KNOWN(
		e.size() == xi.size(),
		"Error in Rosen34: size of e not equal to size of xi"
	);
	size_t i, j, k, l, m;             // indices

	size_t  n    = xi.size();         // number of components in X(t)
	Scalar  ns   = Scalar(double(M)); // number of steps as Scalar object
	Scalar  h    = (tf - ti) / ns;    // step size 
	Scalar  zero = Scalar(0);         // some constants
	Scalar  one  = Scalar(1);
	Scalar  two  = Scalar(2);

	// permutation vectors needed for LU factorization routine
	CppAD::vector<size_t> ip(n), jp(n);

	// vectors used to store values returned by F
	Vector E(n * n), Eg(n), f_t(n);
	Vector g(n * 3), x3(n), x4(n), xf(n), ftmp(n), xtmp(n), nan_vec(n);

	// initialize e = 0, nan_vec = nan
	for(i = 0; i < n; i++)
	{	e[i]       = zero;
		nan_vec[i] = nan(zero);
	}

	xf = xi;           // initialize solution
	for(m = 0; m < M; m++)
	{	// time at beginning of this interval
		Scalar t = ti * (Scalar(int(M - m)) / ns) 
		         + tf * (Scalar(int(m)) / ns);

		// value of x at beginning of this interval
		x3 = x4 = xf;

		// evaluate partial derivatives at beginning of this interval
		F.Ode_ind(t, xf, f_t);
		F.Ode_dep(t, xf, E);    // E = f_x
		if( hasnan(f_t) || hasnan(E) )
		{	e = nan_vec;
			return nan_vec;
		}

		// E = I - f_x * h / 2
		for(i = 0; i < n; i++)
		{	for(j = 0; j < n; j++)
				E[i * n + j] = - E[i * n + j] * h / two;
			E[i * n + i] += one;
		}

		// LU factor the matrix E
		int sign = LuFactor(ip, jp, E);
		CPPAD_ASSERT_KNOWN(
			sign != 0,
			"Error in Rosen34: I - f_x * h / 2 not invertible"
		);

		// loop over integration steps
		for(k = 0; k < 3; k++)
		{	// set location for next function evaluation
			xtmp = xf; 
			for(l = 0; l < k; l++)
			{	// loop over previous function evaluations
				Scalar bkl = b[(k-1)*2 + l];
				for(i = 0; i < n; i++)
				{	// loop over elements of x
					xtmp[i] += bkl * g[i*3 + l] * h;
				}
			}
			// ftmp = F(t + a[k] * h, xtmp)
			F.Ode(t + a[k] * h, xtmp, ftmp); 
			if( hasnan(ftmp) )
			{	e = nan_vec;
				return nan_vec;
			}

			// Form Eg for this integration step
			for(i = 0; i < n; i++)
				Eg[i] = ftmp[i] + ct[k] * f_t[i] * h;
			for(l = 0; l < k; l++)
			{	for(i = 0; i < n; i++)
					Eg[i] += cg[(k-1)*3 + l] * g[i*3 + l];
			}

			// Solve the equation E * g = Eg
			LuInvert(ip, jp, E, Eg);

			// save solution and advance x3, x4
			for(i = 0; i < n; i++)
			{	g[i*3 + k]  = Eg[i];
				x3[i]      += h * d3[k] * Eg[i];
				x4[i]      += h * d4[k] * Eg[i];
			}
		}
		// Form Eg for last update to x4 only
		for(i = 0; i < n; i++)
			Eg[i] = ftmp[i] + ct[3] * f_t[i] * h;
		for(l = 0; l < 3; l++)
		{	for(i = 0; i < n; i++)
				Eg[i] += cg[2*3 + l] * g[i*3 + l];
		}

		// Solve the equation E * g = Eg
		LuInvert(ip, jp, E, Eg);

		// advance x4 and accumulate error bound
		for(i = 0; i < n; i++)
		{	x4[i] += h * d4[3] * Eg[i];

			// cant use abs because cppad.hpp may not be included
			Scalar diff = x4[i] - x3[i];
			if( diff < zero )
				e[i] -= diff;
			else	e[i] += diff;
		}

		// advance xf for this step using x4
		xf = x4;
	}
	return xf;
}

} // End CppAD namespace 

# endif
