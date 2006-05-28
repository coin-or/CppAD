/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

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
/*
$begin BenderQuad$$
$spell
	gx
	gxx
	CppAD
	Fy
	dy
	Jacobian
	ADvector
	const
	dg
	ddg
$$


$index Hessian, Bender$$
$index Jacobian, Bender$$
$index BenderQuad$$
$section Computing Jacobian and Hessian of Bender's Reduced Objective$$

$head Syntax$$
$syntax%%
%# include <CppAD/CppAD.h>
BenderQuad(%x%, %y%, %F%, %Fy%, %Newton%, %g%, %gx%, %gxx%)%$$  

$head Purpose$$
We are given the optimization problem
$latex \[
\begin{array}{rcl}
	{\rm minimize} & F(x, y) & {\rm w.r.t.} \; (x, y) \in \R^n \times \R^m
\end{array}
\] $$
together with a routine that can relative quickly minimize
with respect to $latex y$$; i.e., that can evaluate
$latex \[
	Y(x) = {\rm argmin} \; F(x , y) \; {\rm w.r.t.} \; y \in \R^m
\] $$
The purpose of this routine is to compute the 
value, Jacobian, and Hessian of the reduced objective function
$latex \[
	G(x) = {\rm min} \; F(x, y) \; {\rm w.r.t.} \;  y \in \R^m
\] $$
Note that if only the value and Jacobian are needed, they can be
computed more quickly using the relations
$latex \[
\begin{array}{rcl}
	G(x)        & = & F [ x , Y (x) ]
	\\
	G^{(1)} (x) & = & \partial_x F [x, Y(x) ]
\end{array}
\] $$ 

$head x$$
The $code BenderQuad$$ argument $italic x$$ has prototype
$syntax%
        const %Vector% &%x%
%$$
(see $xref/BenderQuad/Vector/Vector/$$ below)
and its size must be equal to $italic n$$.
It specifies the point at which we evaluating 
the reduced objective function and its derivatives.


$head y$$
The $code BenderQuad$$ argument $italic y$$ has prototype
$syntax%
        const %Vector% &%y%
%$$
and its size must be equal to $italic m$$.
It must be equal to $latex Y(x)$$; i.e., 
it must solve the problem in $latex y$$ for this given value of $latex x$$
$latex \[
\begin{array}{rcl}
	{\rm minimize} & F(x, y) & {\rm w.r.t.} \; y \in \R^m
\end{array}
\] $$

$head F$$
The argument $italic F$$ supports the syntax
$syntax%
	%f% = %F%(%x%, %y%)
%$$
(The type of $italic F$$ is unspecified; i.e., it is a C++ function object.)

$subhead x$$
The $italic F$$ argument $italic x$$ has prototype
$syntax%
        const %ADvector% &%x%
%$$
(see $xref/BenderQuad/ADvector/ADvector/$$ below)
and its size must be equal to $italic n$$.

$subhead y$$
The $italic F$$ argument $italic y$$ has prototype
$syntax%
        const %ADvector% &%y%
%$$
and its size must be equal to $italic m$$.

$subhead f$$
The $italic F$$ return value $italic f$$ has prototype
$syntax%
	%ADvector% %f%
%$$
and its size must be equal to one.
The value of $italic f$$ is
$latex \[
	f = F(x, y)
\] $$.


$head Fy$$
The argument $italic Fy$$ supports the syntax
$syntax%
	%fy% = %Fy%(%x%, %y%)
%$$
(The type of $italic Fy$$ is unspecified; i.e., it is a C++ function object.)

$subhead x$$
The $italic Fy$$ argument $italic x$$ has prototype
$syntax%
        const %ADvector% &%x%
%$$
and its size must be equal to $italic n$$.

$subhead y$$
The $italic Fy$$ argument $italic y$$ has prototype
$syntax%
        const %ADvector% &%y%
%$$
and its size must be equal to $italic m$$.

$subhead fy$$
The $italic Fy$$ return value $italic fy$$ has prototype
$syntax%
	%ADvector% %fy%
%$$
The value of $italic fy$$ is
$latex \[
	fy = \partial_y F(x, y)
\] $$.


$head Newton$$
The argument $italic Newton$$ supports the syntax
$syntax%
	%dy% = %Newton%(%x%, %y%, %fy%)
%$$
(The type of $italic Newton$$ is unspecified; 
i.e., it is a C++ function object.)

$subhead x$$
The $italic Newton$$ argument $italic x$$ has prototype
$syntax%
        const %ADvector% &%x%
%$$
and its size must be equal to $italic n$$.

$subhead y$$
The $italic Newton$$ argument $italic y$$ has prototype
$syntax%
        const %ADvector% &%y%
%$$
and its size must be equal to $italic m$$.

$subhead fy$$
The $italic Newton$$ argument $italic fy$$ has prototype
$syntax%
        const %ADvector% &%fy%
%$$
and its size must be equal to $italic m$$.

$subhead dy$$
The $italic Newton$$ return value $italic dy$$ has prototype
$syntax%
	%ADvector% %dy%
%$$
and its size must be equal to $italic m$$.
The return value $italic dy$$ is given by
$latex \[
	dy = - [ \partial_y \partial_y F (x , y) ]^{-1} fy
\] $$
Note that if $italic fy$$ is equal to the gradient of
$latex F(x, y)$$ with respect to $latex y$$,
$latex dy$$ is the Newton step for finding a zero
of the gradient of $latex F(x, y)$$ with respect to $latex y$$;
i.e., 
$latex y + dy$$ is an approximate solution for the equation
$latex \partial_y F (x, y + dy) = 0$$. 

$head g$$
The argument $italic g$$ has prototype
$syntax%
	%Vector% &%g%
%$$
and has size one.
The input value of its element does not matter.
On output,
it contains the value of $latex G (x)$$; i.e.,
$latex \[
	g[0] = G (x)
\] $$

$head gx$$
The argument $italic gx$$ has prototype
$syntax%
	%Vector% &%gx%
%$$
and has size $latex n $$.
The input values of its elements do not matter.
On output,
it contains the Jacobian of $latex G (x)$$; i.e.,
for $latex j = 0 , \ldots , n-1$$, 
$latex \[
	gx[ j ] = G^{(1)} (x)_j
\] $$

$head gxx$$
The argument $italic gx$$ has prototype
$syntax%
	%Vector% &%gxx%
%$$
and has size $latex n \times n$$.
The input values of its elements do not matter.
On output,
it contains the Hessian of $latex G (x)$$; i.e.,
for $latex i = 0 , \ldots , n-1$$, and
$latex j = 0 , \ldots , n-1$$, 
$latex \[
	gxx[ i * n + j ] = G^{(2)} (x)_{i,j}
\] $$

$head Vector$$
The type $italic Vector$$ must be a 
$xref/SimpleVector/$$ class. 
We use $italic Base$$ to refer to the type of the elements of $italic Vector$$; i.e.,
$syntax%
	%Vector%::value_type
%$$

$head ADvector$$
The type $italic ADvector$$ must be a 
$xref/SimpleVector/$$ class with elements of type 
$syntax%AD<%Base%>%$$; i.e.,
$syntax%
	%ADVector%::value_type
%$$
must be the same type as
$syntax%
	AD< %Vector%::value_type >
%$$.

$head Tape State$$
The tape that records $syntax%AD<%Base%>%$$ operations is in the
empty $xref/glossary/Tape State/state/$$ when $code BenderQuad$$ is called.
It will be in the empty state when $code BenderQuad$$ returns.



$head Example$$
$children%
	Example/BenderQuad.cpp
%$$
The file
$xref/BenderQuad.cpp/$$
contains an example and test of this operation.   
It returns true if it succeeds and false otherwise.


$end
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN CppAD namespace

template <class Vector, class F_type, class Fy_type, class Newton_type>
void BenderQuad(
	const Vector   &x     , 
	const Vector   &y     , 
	F_type          F     , 
	Fy_type         Fy    , 
	Newton_type     Newton,
	Vector         &g     ,
	Vector         &gx    ,
	Vector         &gxx   )
{	// determine the base type
	typedef typename Vector::value_type Base;

	// check that Vector is a SimpleVector class
	CheckSimpleVector<Base, Vector>();

	// declare the ADvector type
	typedef CppADvector< AD<Base> > ADvector;

	// size of the x and y spaces
	size_t n = x.size();
	size_t m = y.size();

	// check the size of gx and gxx
	CppADUsageError(
		gx.size() == n,
		"BenderQuad: size of the vector gx is not equal to n"
	);
	CppADUsageError(
		gxx.size() == n * n,
		"BenderQuad: size of the vector gxx is not equal to n * n"
	);

	// some temporary indices
	size_t i, j;

	// parameter and variable versions x
	ADvector px(n), vx(n);
	for(j = 0; j < n; j++)
		vx[j] = px[j] = x[j];
	
	// declare the independent variables
	Independent(vx);

	// parameter version of y
	ADvector py(m);
	for(j = 0; j < m; j++)
		py[j] = y[j];

	// evaluate fy = the partial of F(x, y) w.r.t y as a function of x
	ADvector fy(m);
	fy = Fy(vx, py);

	// evaluate dy (x) = Newton step as a function of x through fy only
	ADvector dy(m);
	dy = Newton(px, py, fy);

	// variable version of y
	ADvector vy(m);
	for(j = 0; j < m; j++)
		vy[j] = y[j] + dy[j];

	// evaluate G~ (x) = F [ x , y + dy(x) ] 
	ADvector gtilde(1);
	gtilde = F(vx, vy);

	// AD function object that corresponds to G~ (x)
	ADFun<Base> Gtilde(vx, gtilde); 

	// value of G(x)
	g[0] = Value( gtilde[0] );

	// initial forward direction vector as zero
	Vector dx(n);
	for(j = 0; j < n; j++)
		dx[j] = Base(0);

	// weight, first and second order derivative values
	Vector dg(1), w(1), ddw(2 * n);
	w[0] = 1.;

	// Jacobian and Hessian of G(x) is equal Jacobian and Hessian of Gtilde
	for(j = 0; j < n; j++)
	{	// compute partials in x[j] direction
		dx[j] = Base(1);
		dg    = Gtilde.Forward(1, dx);
		gx[j] = dg[0];

		// restore the dx vector to zero
		dx[j] = Base(0);

		// compute second partials w.r.t x[j] and x[l]  for l = 1, n
		ddw = Gtilde.Reverse(2, w);
		for(i = 0; i < n; i++)
			gxx[ i * n + j ] = ddw[ i * 2 + 1 ];
	}

	return;
}
	
} // END CppAD namespace
