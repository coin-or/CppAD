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
$latex
\newcommand{\B}[1]{ {\bf #1} }
\newcommand{\W}[1]{ \; #1 \; }
$$


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
| \zeta_i ( x , a_0 (x) , \ldots , a_{i-1} (x ) ) |
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

$head Affine Approximation$$
We define the affine approximations
$latex \[
\begin{array}{rcl}
\tilde{y}( \hat{x} \W{:} x , u )
& = &
y ( \hat{x}, a( \hat{x} ) )
	+ \partial_x y ( \hat{x}, a( \hat{x} ) ) ( x - \hat{x} )
	+ \partial_u y ( \hat{x}, a( \hat{x} ) ) ( u - a( \hat{x} ) )
\\
\tilde{z}( \hat{x} \W{:} x , u )
& = &
z ( \hat{x}, a( \hat{x} ) )
	+ \partial_x z ( \hat{x}, a( \hat{x} ) ) ( x - \hat{x} )
	+ \partial_u z ( \hat{x}, a( \hat{x} ) ) ( u - a( \hat{x} ) )
\end{array}
\] $$
It follows that
$latex \[
\begin{array}{rcl}
y( x , u )
& = &
\tilde{y}( \hat{x} \W{:} x , u ) + o ( x - \hat{x}, u - a( \hat{x} ) )
\\
z( x , u )
& = &
\tilde{z}( \hat{x} \W{:} x , u ) + o ( x - \hat{x}, u - a( \hat{x} ) )
\end{array}
\] $$

$head Abs-normal Approximation$$

$subhead Approximating a(x)$$
The function $latex a(x)$$ is not smooth, but it is equal to
$latex | z(x, u) |$$ when $latex u = a(x)$$.
Furthermore
$latex \[
\tilde{z}( \hat{x} \W{:} x , u )
=
z ( \hat{x}, a( \hat{x} ) )
	+ \partial_x z ( \hat{x}, a( \hat{x} ) ) ( x - \hat{x} )
	+ \partial_u z ( \hat{x}, a( \hat{x} ) ) ( u - a( \hat{x} ) )
\] $$
Now the partial of $latex z_i$$ with respect to $latex u_j$$ is zero
for $latex j \geq i$$. It follows that
$latex \[
\tilde{z}_i ( \hat{x} \W{:} x , u )
=
z_i ( \hat{x}, a( \hat{x} ) )
	+ \partial_x z_i ( \hat{x}, a( \hat{x} ) ) ( x - \hat{x} )
	+ \sum_{j < i} \partial_{u(j)}
		z_i ( \hat{x}, a( \hat{x} ) ) ( u_j - a_j ( \hat{x} ) )
\] $$
Considering the case $latex i = 0$$ we define
$latex \[
\tilde{a}_0 ( \hat{x} \W{:} x )
=
| \tilde{z}_0 ( \hat{x} \W{:} x , u ) |
=
\left|
	z_0 ( \hat{x}, a( \hat{x} ) )
	+ \partial_x z_0 ( \hat{x}, a( \hat{x} ) ) ( x - \hat{x} )
\right|
\] $$
It follows that
$latex \[
	a_0 (x) = \tilde{a}_0 ( \hat{x} \W{:} x ) + o ( x - \hat{x} )
\] $$
In general, we define $latex \tilde{a}_i$$ using
$latex \tilde{a}_j$$ for $latex j < i$$ as follows:
$latex \[
\tilde{a}_i ( \hat{x} \W{:} x )
=
\left |
	z_i ( \hat{x}, a( \hat{x} ) )
	+ \partial_x z_i ( \hat{x}, a( \hat{x} ) ) ( x - \hat{x} )
	+ \sum_{j < i} \partial_{u(j)}
		z_i ( \hat{x}, a( \hat{x} ) )
			( \hat{a}_j ( \hat{x} \W{:} x )  - a_j ( \hat{x} ) )
\right|
\] $$
It follows that
$latex \[
	a (x) = \tilde{a} ( \hat{x} \W{:} x ) + o ( x - \hat{x} )
\] $$
Note that in the case where $latex z(x, u)$$ and $latex y(x, u)$$ are
affine,
$latex \[
	\tilde{a} ( \hat{x} \W{:} x ) = a( x )
\] $$


$subhead Approximating f(x)$$
$latex \[
f(x)
=
y ( x , a(x ) )
=
\tilde{y} ( \hat{x} \W{:} x , \tilde{a} ( \hat{x} \W{:}  x ) )
+ o( \Delta x )
\] $$

$head Correspondence to Literature$$
Using the notation
$latex Z = \partial_x z(\hat{x}, \hat{u})$$,
$latex L = \partial_u z(\hat{x}, \hat{u})$$,
$latex J = \partial_x y(\hat{x}, \hat{u})$$,
$latex Y = \partial_u y(\hat{x}, \hat{u})$$,
the approximation for $latex z$$ and $latex y$$ are
$latex \[
\begin{array}{rcl}
\tilde{z} ( \hat{x} \W{:} x , u )
& = &
z ( \hat{x}, a( \hat{x} ) ) + Z ( x - \hat{x} ) + L ( u - a( \hat{x} ) )
\\
\tilde{y} ( \hat{x} \W{:} x , u )
& = &
y ( \hat{x}, a( \hat{x} ) ) + J ( x - \hat{x} ) + Y ( u - a( \hat{x} ) )
\end{array}
\] $$
Moving the terms with $latex \hat{x}$$ together, we have
$latex \[
\begin{array}{rcl}
\tilde{z} ( \hat{x} \W{:} x , u )
& = &
z ( \hat{x}, a( \hat{x} ) ) - Z \hat{x} - L a( \hat{x} )  + Z x + L u
\\
\tilde{y} ( \hat{x} \W{:} x , u )
& = &
y ( \hat{x}, a( \hat{x} ) ) - J \hat{x} - Y a( \hat{x} )  + J x + Y u
\end{array}
\] $$
Using the notation
$latex c = z ( \hat{x}, \hat{u} ) - Z \hat{x} - L \hat{u}$$,
$latex b = y ( \hat{x}, \hat{u} ) - J \hat{x} - Y \hat{u}$$,
we have
$latex \[
\begin{array}{rcl}
\tilde{z} ( \hat{x} \W{:} x , u ) & = & c + Z x + L u
\\
\tilde{y} ( \hat{x} \W{:} x , u ) & = & b + J x + Y u
\end{array}
\] $$
Considering the affine case, where the approximations are exact,
and choosing $latex u = a(x) = |z(x, u)|$$, we obtain
$latex \[
\begin{array}{rcl}
z( x , a(x ) ) & = & c + Z x + L |z( x , a(x ) )|
\\
y( x , a(x ) ) & = & b + J x + Y |z( x , a(x ) )|
\end{array}
\] $$
This is Equation (2) of the reference.



$end
*/

# endif
