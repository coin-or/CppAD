# ifndef CppADForwardIncluded
# define CppADForwardIncluded

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

/*
$begin Forward$$
$spell
	typename
	xk
	xp
	yp
	Ind
	vp
	uj
	Taylor
	std
	arg
	const
$$

$section Forward Mode$$ 

$index forward, mode$$
$index mode, forward$$
$index derivative, forward mode$$
$index calculate, forward mode$$

$table
$bold Syntax$$ $cnext
$syntax%%yp% = %F%.Forward(%p%, %xp% )%$$
$rnext $cnext
$tend

$fend 20$$

$head Description$$
Given the functions $latex X : B \rightarrow B^n$$
and $latex F : B^n \rightarrow B^m$$,
$code Forward$$ computes the $th p$$ order 
$xref/glossary/Taylor Coefficient/Taylor coefficient/$$ 
for a function $latex F \circ X : B \rightarrow B^m$$ 
which is given by
$latex \[
	y^{(p)} = \frac{1}{p !} (F \circ X)^{(p)} (0)
\] $$

$head F$$
The object $italic F$$ has prototype
$syntax%
	ADFun<%Base%> %F%
%$$
It defines a function
$latex F : B^n \rightarrow B^m$$,
where $latex n$$ is the dimension of the 
$xref/ADFun/Domain/domain/$$ space for $italic F$$, and
$latex m$$ is the dimension of the 
$xref/ADFun/Range/range/$$ space for $italic F$$.

$head X$$
The function
$latex X : B \rightarrow B^m$$ is defined in terms of
$latex x^{(k)}$$ for $latex k = 0 , \ldots , p$$ by
$latex \[
	X(t) = x^{(0)} + x^{(1)} t + \cdots, + x^{(p)} t^p 
\] $$ 
Note that for $latex k = 0 , \ldots , p$$,
$latex x^{(k)}$$ is related to the $th k$$ derivative of $latex X(t)$$ by
$latex \[
	x^{(k)} = \frac{1}{k !} X^{(k)} (0) 
\] $$
For $latex k = 0 , \ldots , p-1$$,
the $th k$$ order Taylor coefficient 
$latex x^{(k)} \in B^n$$ is the value of $italic xk$$ in
the previous call to 
$syntax%
	%F%.Forward(%k%, %xk%)
%$$ 
If there is no previous call with $latex k = 0$$,
$latex x^{(0)}$$ is the value of the independent variables when $italic F$$ 
was constructed as an $xref/ADFun/$$ object.

$head p$$
The argument $italic p$$ has prototype
$syntax%
	size_t %p%
%$$
It specifies the order of the forward mode calculation.

$head xp$$
The argument $italic xp$$ has prototype
$syntax%
	const %VectorBase% &%xp%
%$$
and is a vector with size $italic n$$.
In this call to $code Forward$$,
$latex xp$$ specifies $latex x^{(p)}$$; i.e.,
the $th p$$ order Taylor coefficient for the function $latex X(t)$$.

$head VectorBase$$
The type $italic VectorBase$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type Base/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head Order$$
The function call
$xref/ADFun/Order/F.Order()/$$ returns the highest order of the 
Taylor coefficients currently stored in $italic F$$.
This must be greater than or equal to $latex p-1$$ when
$syntax%%F%.Forward(%p%, %xp%)%$$ is called.
It will be equal to $italic p$$ after this call.


$head yp$$
The return value $latex yp$$ has prototype
$syntax%
	%VectorBase% %yp%
%$$
is a vector of size $latex m$$ and contains
the $th p$$ order Taylor coefficient for $latex F \circ X$$; i.e.,
$latex \[
	y^{(p)} = \frac{1}{p !} * (F \circ X)^{(p)} (0) 
\] $$ 
The column vector corresponding to $italic yp$$
is denoted by $latex y^{(p)} \in B^m$$ below.

$subhead Zero Order$$
In the case where $italic p$$ is zero,
the return value $latex y^{(p)} \in B^m$$ is
$latex \[
\begin{array}{rcl}
y^{(p)} & = & y^{(0)} \\
	& = & (F \circ X) ( 0 ) \\
F[ x^{(0)} ]
	& = & y^{(p)}
\end{array}
\] $$

$subhead First Order$$
In the case where $italic p$$ is one,
the return value $latex y^{(p)} \in B^m$$ is
$latex \[
\begin{array}{rcl}
y^{(p)} & = & y^{(1)} \\
	& = & (F \circ X)^{(1)} ( 0 ) \\
	& = & F^{(1)} [ X(0) ] *  X^{(1)} (0) \\
	& = & F^{(1)} ( x^{(0)} ) *  x^{(1)}
\end{array}
\] $$
Note that if $latex x^{(1)}$$ is the $th j$$ 
$xref/glossary/Elementary Vector/elementary vector/$$
$latex \[
\D{F}{x_j} ( x^{(0)} ) = y^{(p)}
\] $$

$subhead Second Order$$
In the case where $italic p$$ is two,
the $th i$$ element of 
the return value $latex y^{(p)} \in B^m$$ is
$latex \[
\begin{array}{rcl}
y_i^{(p)} 
& = & \frac{1}{2} (F_i \circ X)^{(2)} ( 0 ) 
\\
& = & \frac{1}{2} \left[ F_i^{(1)} [ X(0) ] * X^{(2)} (0) 
  + X^{(1)} (0)^T * F_i^{(2)} [ X(0) ] * X^{(1)} (0) \right]
\\
& = & \frac{1}{2}  \left[
	F_i^{(1)} ( x^{(0)} ) * x^{(2)}
	+
	( x^{(1)} )^T * F_i^{(2)} ( x^{(0)} ) * x^{(1)}
\right  ]
\end{array}
\] $$
Note that if $latex x^{(1)}$$ is the $th j$$ 
$xref/glossary/Elementary Vector/elementary vector/$$
and $latex x^{(2)}$$ is zero,
$latex \[
\begin{array}{rcl}
	\DD{F_i}{x_j}{x_j} ( x^{(0)} ) = 2 y_i^{(p)} 
\end{array}
\] $$
If $latex x^{(1)}$$ is the sum of the $th j$$ and $th l$$ 
$xref/glossary/Elementary Vector/elementary vectors/$$
and $latex x^{(2)}$$ is zero,
$latex \[
\begin{array}{rcl}
	y_i^{(p)} 
	& = & \frac{1}{2} \left[
		\DD{F_i}{x_j}{x_j} ( x^{(0)} )
		+
		\DD{F_i}{x_j}{x_\ell} ( x^{(0)} )
		+
		\DD{F_i}{x_\ell}{x_j} ( x^{(0)} )
		+
		\DD{F_i}{x_\ell}{x_\ell} ( x^{(0)} )
	\right]
	\\
	\DD{F_i}{x_\ell}{x_j} ( x^{(0)} )
	& = & 
	y_i^{(p)} 
	-
	\frac{1}{2} \DD{F_i}{x_j}{x_j} ( x^{(0)} )
	-
	\frac{1}{2} \DD{F_i}{x_\ell}{x_\ell} ( x^{(0)} )
\end{array} 
\] $$

$head Example$$
$children%
	Example/Forward.cpp
%$$
The file
$xref/Forward.cpp/$$
contains an example and a test of this operation.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/

// BEGIN CppAD namespace
namespace CppAD {

template <typename Base>
template <typename VectorBase>
VectorBase ADFun<Base>::Forward(size_t p, const VectorBase &up)
{	size_t i;
	size_t j;

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CppADUsageError(
		up.size() == indvar.size(),
		"Second argument to Forward does not have length equal to\n"
		"the dimension of the domain for the corresponding ADFun."
	);
	CppADUsageError(
		p <= order + 1,
		"The Taylor coefficient currently stored\n"
		"in this ADFun has order less than p-1."
	);  
	if( TaylorColDim <= p )
	{	CppADUnknownError(TaylorColDim == p);
		TaylorColDim = p+1;
		Base *Tmp = new Base[totalNumVar * TaylorColDim];
		for(i = 0; i < totalNumVar; i++)
		{	for(j = 0; j < p; j++)
				Tmp[i * TaylorColDim + j] = Taylor[i * p + j];
		}
		delete [] Taylor;
		Taylor = Tmp;
	}

	// set the p-th order Taylor coefficients for independent variables
	size_t m = indvar.size();
	for(i = 0; i < m; i++)
	{	CppADUnknownError( indvar[i] < totalNumVar );
		// indvar[i] is operator taddr for i-th independent variable
		CppADUnknownError( Rec->GetOp( indvar[i] ) == InvOp );
		// It is also variable taddr for i-th independent variable
		Taylor[indvar[i] * TaylorColDim + p] = up[i];
	}

	// evaluate the derivatives
	compareChange = ForwardSweep(true, p, totalNumVar, Rec, TaylorColDim, Taylor);

	// return the p-th order Taylor coefficients for dependent variables
	size_t n = depvar.size();
	VectorBase vp(n);
	for(i = 0; i < n; i++)
	{	CppADUnknownError( depvar[i] < totalNumVar );
		vp[i] = Taylor[depvar[i] * TaylorColDim + p];
	}

	// order of the Taylor coefficient matrix currently in this ADFun
	order = p;

	return vp;
}

} // END CppAD namespace
	

# endif
