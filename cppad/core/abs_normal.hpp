# ifndef CPPAD_CORE_ABS_NORMAL_HPP
# define CPPAD_CORE_ABS_NORMAL_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin abs_normal$$
$spell
	Andreas Griewank
	Jens-Uwe Bernt
	Manuel Radons
	Tom Streubel
	const
$$
$latex \newcommand{\B}[1]{ {\bf #1} }$$

$section Create An Abs-normal Representation of a Function$$

$head Under Construction$$
This is an in-progress design, and does not yet have an implementation.

$head Syntax$$
$icode%g%.abs_normal(%f%)%$$

$head Reference$$
Andreas Griewank, Jens-Uwe Bernt, Manuel Radons, Tom Streubel,
$italic Solving piecewise linear systems in abs-normal form$$,
Linear Algebra and its Applications,
vol. 471 (2015), pages 500-530.

$head f$$
The object $icode f$$ has prototype
$codei%
	const ADFun<%Base%>& %f%
%$$
It represents a function $latex f : \B{R}^n \rightarrow \B{R}^m$$.
We assume that the only non-smooth terms in the representation are
absolute value functions and use $latex s \in \B{Z}_+$$
to represent the number of these terms.

$head g$$
The object $icode g$$ has prototype
$codei%
	ADFun<%Base%> %g%
%$$
The initial function representation in $icode g$$ is lost.
Upon return it represents the smooth function
$latex g : \B{R}^{n + s} \rightarrow  \B{R}^{m + s}$$ is defined by
$latex \[
g( x , u )
=
\left[ \begin{array}{c} y(x, u) \\ z(x, u) \end{array} \right]
\] $$
were $latex y(x, u)$$ and $latex z(x, u)$$ are defined below.

$subhead a(x)$$
Let $latex \zeta_0 ( x )$$
denote the argument for the first absolute value term in $latex f(x)$$,
$latex \zeta_1 ( x , |\zeta_0 (x)| )$$ for the second term, and so on.
For $latex i = 0 , \ldots , {s-1}$$ define
$latex \[
a_i (x)
=
| \zeta_i | \left[ x, a_0 (x) , \ldots , a_{i-1} (x) \right]
\] $$

$subhead z(x, u)$$
Define the smooth function
$latex z : \B{R}^{n + s} \rightarrow  \B{R}^s$$ by
$latex \[
z_i ( x , u ) = \zeta_i ( x , u_0 , \ldots , u_{i-1} )
\] $$

$subhead y(x, u)$$
We define
$latex y : \B{R}^{n + s} \rightarrow  \B{R}^m$$ as the smooth function
such that $latex y( x , u ) = f(x)$$ whenever $latex u = a(x)$$.

$head Abs-normal Approximation$$
Suppose we are given a point $latex x \in \B{R}^n$$,
and an increment $latex \Delta x$$,
and define
$latex u = a(x)$$,
$latex \Delta u = a( x + \Delta x) - a(x)$$.
$latex \[
\begin{array}{rcl}
z_i ( x + \Delta x, u + \Delta u )
& = &
z_i (x, u)
	+ \partial_x z_i (x, u) \Delta x
	+ \partial_u z_i (x, u) \Delta u
	+ o( \Delta x )
\end{array}
\] $$
Now the partial of $latex z_i$$ with respect to $latex u_j$$ is zero
for $latex i \leq j$$. It follows that
$latex \[
\begin{array}{rcl}
z_i ( x + \Delta x, u + \Delta u )
& = &
z_i (x, u)
	+ \partial_x z_i (x, u) \Delta x
	+ \sum_{j < i} \partial_{u(j)} z_i (x, u) \Delta u_j
	+ o( \Delta x )
\\
a_i ( x + \Delta x )
& = &
\left| z_i (x, u)
	+ \partial_x z_i (x, u) \Delta x
	+ \sum_{j < i} \partial_{u(j)} z_i (x, u) \Delta u_j
\right|
+ o( \Delta x )
\\
\Delta u_i
& = &
\left| z_i (x, u)
	+ \partial_x z_i (x, u) \Delta x
	+ \sum_{j < i} \partial_{u(j)} z_i (x, u) \Delta u_j
\right|
- | z_i (x, u) |
+ o( \Delta x )
\end{array}
\] $$
Given $latex u = a(x)$$ and $latex \Delta u_j$$ for $latex j < i$$,
we can use the formula above to solve for $latex \Delta u_i$$ to
within $latex o( \Delta x)$$.
We call the resulting vector $latex \Delta \tilde{u}$$.
$latex \[
\begin{array}{rcl}
y ( x + \Delta x, u + \Delta u )
& = &
y(x, u)
	+ \partial_x y (x, u) \Delta x
	+ \partial_u y (x, u) \Delta u
	+ o( \Delta x )
\\
f( x + \Delta x )
& = &
f(x)
	+ \partial_x y (x, u) \Delta x
	+ \partial_u y (x, u) \Delta \tilde{u}
	+ o( \Delta x )
\end{array}
\] $$
This is the abs-normal approximation for $latex f$$ near $latex x$$.

$head Correspondence to Literature$$
Using the notation
$latex Z = \partial_x z(\hat{x}, \hat{u})$$,
$latex L = \partial_u z(\hat{x}, \hat{u})$$,
$latex J = \partial_x y(\hat{x}, \hat{u})$$,
$latex Y = \partial_u y(\hat{x}, \hat{u})$$,
the approximation for $latex z$$ and $latex y$$ near $latex \hat{x}$$
and $latex \hat{u}$$ is
$latex \[
\begin{array}{rcl}
z(\hat{x} + \Delta x, \hat{u} + \Delta u )
& = &
z(\hat{x}, \hat{u}) + Z \Delta x + L \Delta u  + o( \Delta x , \Delta u)
\\
y(\hat{x} + \Delta x, \hat{u} + \Delta u )
& = &
y(\hat{x}, \hat{u}) + J \Delta x + Y \Delta u + o( \Delta x , \Delta u)
\end{array}
\] $$
Using the notation
$latex x = \hat{x} + \Delta x$$,
$latex u = \hat{u} + \Delta u$$,
$latex c = z(\hat{x}, \hat{u}) - Z \hat{x} - L \hat{u}$$,
$latex b = y(\hat{x}, \hat{u}) - J \hat{x} - Y \hat{u}$$,
we have
$latex \[
\begin{array}{rcl}
z(x, u)
& = &
c + Z x + L u  + o( \Delta x , \Delta u)
\\
y(x, u)
& = &
b + J x + Y u + o( \Delta x , \Delta u)
\end{array}
\] $$
Choosing
$latex \[
	u = a(x) = |z(x, u)|
\] $$
Thus we obtain
$latex \[
\begin{array}{rcl}
z(x, u)
& = &
c + Z x + L | z(x, u) |  + o( \Delta x )
\\
y(x, u)
& = &
b + J x + Y | z(x, u) | + o( \Delta x )
\end{array}
\] $$
This shows the correspondence between the notation above and
Equation (2) of the reference.


$end
*/

# endif
