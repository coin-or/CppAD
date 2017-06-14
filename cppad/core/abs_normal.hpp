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
| \zeta_i [ x, a_0 (x) , \ldots , a_{i-1} (x) ] |
\] $$
This defines $latex a : \B{R}^n \rightarrow \B{R}^s$$.

$subhead z(x, u)$$
Define the smooth function
$latex z : \B{R}^{n + s} \rightarrow  \B{R}^s$$ by
$latex \[
z_i ( x , u ) = \zeta_i ( x , u_0 , \ldots , u_{i-1} )
\] $$

$subhead y(x, u)$$
There is a smooth function
$latex y : \B{R}^{n + s} \rightarrow  \B{R}^m$$
such that $latex y( x , u ) = f(x)$$ whenever $latex u = a(x)$$.

$head Abs-normal Approximation$$

$subhead Approximating a(x)$$
The function $latex a(x)$$ is not smooth, but it is equal to
$latex | z(x, u) |$$ when $latex u = a(x)$$.
Furthermore
$latex \[
z ( x + \Delta x, u + \Delta u )
=
z (x, u)
	+ \partial_x z (x, u) \Delta x
	+ \partial_u z (x, u) \Delta u
	+ o( \Delta x , \Delta u )
\] $$
Now the partial of $latex z_i$$ with respect to $latex u_j$$ is zero
for $latex j \geq i$$. It follows that
$latex \[
z_i ( x + \Delta x, u + \Delta u )
=
z_i (x, u)
	+ \partial_x z_i (x, u) \Delta x
	+ \sum_{j < i} \partial_{u(j)} z_i (x, u) \Delta u_j
	+ o( \Delta x , \Delta u )
\] $$
Choosing $latex u = a(x)$$ and
$latex \Delta u = a( x + \Delta x ) - a(x)$$,
and taking absolute values, we have
$latex \[
a_i ( x + \Delta x )
=
\left|
	z_i [x, a(x) ]
	+ \partial_x z_i [x, a(x) ] \Delta x
	+ \sum_{j < i} \partial_{u(j)} z_i [x, a(x) ]
		[ a_j ( x + \Delta x ) - a_j (x) ]
\right|
+ o( \Delta x )
\] $$
We define $latex \tilde{a}_i (x, \Delta x )$$,
for $latex i = 0, \ldots , s-1$$ by
$latex \[
\tilde{a}_i ( x , \Delta x )
=
\left|
	z_i [x, a(x) ]
	+ \partial_x z_i [x, a(x) ] \Delta x
	+ \sum_{j < i} \partial_{u(j)} z_i [x, a(x) ]
		[ \tilde{a}_j ( x , \Delta x ) - a_j (x) ]
\right|
\] $$
It follows that
$latex \[
	\tilde{a} ( x ,  \Delta x ) = a( x + \Delta x ) + o ( \Delta x )
\] $$
Note that in the case where $latex z(x, u)$$ and $latex y(x, u)$$ are
affine,
$latex \[
	\tilde{a} ( x ,  \Delta x ) = a( x + \Delta x )
\] $$


$subhead Approximating f(x)$$
The function $latex y(x, u)$$ is smooth
$latex \[
y ( x + \Delta x, u + \Delta u )
=
y (x, u)
	+ \partial_x y (x, u) \Delta x
	+ \partial_u y (x, u) \Delta u
	+ o( \Delta x , \Delta u )
\] $$
Choosing $latex u = a(x)$$ and
$latex \Delta u = a( x + \Delta x ) - a(x)$$ we have
$latex \[
f(x + \Delta x )
=
f(x)
+ \partial_x y [ x, a(x) ] \Delta x
+ \partial_u y [ x, a(x) ] [ a(x + \Delta x ) - a(x) ]
+ o( \Delta x )
\] $$
Replacing $latex a(x + \Delta x)$$ by $latex \tilde{a} ( x, \Delta x )$$
we have
$latex \[
f(x + \Delta x )
=
f(x)
+ \partial_x y [ x, a(x) ] \Delta x
+ \partial_u y [ x, a(x) ] [ \tilde{a}(x , \Delta x ) - a(x) ]
+ o( \Delta x )
\] $$

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
$latex x = \hat{x} + \Delta x$$ and $latex u = \hat{u} + \Delta u$$ we have
$latex \[
\begin{array}{rcl}
z(x, u)
& = &
z(\hat{x}, \hat{u})  - Z \hat{x} - L \hat{u} + Z x + L u
+ o( \Delta x , \Delta u)
\\
y(x, u)
& = &
y(\hat{x}, \hat{u}) - J \hat{x} - Y \hat{u} + J x + Y u
+ o( \Delta x , \Delta u)
\end{array}
\] $$
Using the notation
$latex c = z(\hat{x}, \hat{u}) - Z \hat{x} - L \hat{u}$$,
$latex b = y(\hat{x}, \hat{u}) - J \hat{x} - Y \hat{u}$$,
we have
$latex \[
\begin{array}{rcl}
z(x, u)
& = &
c + Z x + L u + o( \Delta x , \Delta u)
\\
y(x, u)
& = &
b + J x + Y u + o( \Delta x , \Delta u)
\end{array}
\] $$
Choosing $latex u = a(x) = |z(x, u)|$$, we obtain
$latex \[
\begin{array}{rcl}
z[ x , a(x) ]
& = &
c + Z x + L |z[ x , a(x) ]| + o( \Delta x )
\\
y[ x , a(x) ]
& = &
b + J x + Y |z[ x , a(x) ]| + o( \Delta x )
\end{array}
\] $$
In the case where $latex y(x, u)$$ and $latex z(x, u)$$ are affine,
this approximation is exact and we get Equation (2) of the reference.



$end
*/

# endif
