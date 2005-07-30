# ifndef CppADReverseIncluded
# define CppADReverseIncluded

// BEGIN SHORT COPYRIGHT
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
// END SHORT COPYRIGHT

/*
$begin Reverse$$
$spell
	typename
	xk
	xp
	dw
	Ind
	uj
	std
	arg
	const
	Taylor
$$

$section Reverse Mode$$ 


$index reverse, mode$$
$index mode, reverse$$
$index derivative, reverse mode$$
$index calculate, reverse mode$$

$table
$bold Syntax$$ $cnext
$syntax%%dw% = %F%.Reverse(%p%, %w%)%$$
$tend

$fend 20$$

$head Description$$
We are given the functions
$latex X : B \times B^{n \times p} \rightarrow B^n$$
and  $latex F : B^n \rightarrow B^m$$.
The $code Reverse$$ routine evaluates the partial derivatives 
with respect to $latex x^{(0)}$$
of the functions
$latex W^{(k)} : B \times B^{n \times p} \rightarrow B$$ 
defined by
$latex \[
\begin{array}{rcl}
	X(t, x^{(0)}, \ldots , x^{(p-1)} ) 
		& = & x^{(0)} + x^{(1)} t + \cdots + x^{(p-1)} t^{p-1} 
	\\
	W(t, x^{(0)}, \ldots , x^{(p-1)} ) 
		& = & \sum_{i=0}^{n-1} w_i 
			F_i [ X(t, x^{(0)}, \ldots , x^{(p-1)} ) ]
	\\
	W^{(k)} (t, x^{(0)}, \ldots , x^{(p-1)} ) 
		& = & \frac{1}{k !} \Dpow{k}{t} 
			W [ X(t, x^{(0)}, \ldots , x^{(p-1)} ) ]
\end{array}
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
$latex X : B \times B^{n \times p} \rightarrow B^n$$ is defined in terms of
$latex x^{(k)}$$ for $latex k = 0 , \ldots , p$$ by
$latex \[
	X(t, x^{(0)}, \ldots , x^{(p-1)} ) 
	= 
	x^{(0)} + x^{(1)} t + \cdots, + x^{(p)} t^p 
\] $$ 
Note that for $latex k = 0 , \ldots , p$$,
$latex x^{(k)}$$ is related to the $th k$$ partial of $latex X$$ 
with respect to $latex t$$ by
$latex \[
	x^{(k)} = 
		\frac{1}{k !} \Dpow{k}{t} 
			X(0, x^{(0)}, \ldots , x^{(p-1)} ) 
\] $$
For $latex k = 0 , \ldots , p-1$$,
the $th k$$ order Taylor coefficient 
$latex x^{(k)} \in B^n$$ is the value of $italic xk$$ in
the previous call to $xref/Forward/$$ with the syntax
$syntax%
	%F%.Forward(%k%, %xk%)
%$$ 
If there is no previous call with $latex k = 0$$,
$latex x^{(0)}$$ is the value of the independent variables when $italic F$$ 
was constructed as an $xref/ADFun/$$ object.


$head VectorBase$$
The type $italic VectorBase$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type Base/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head w$$
The argument $italic w$$ is a vector of length $latex m$$ and has prototype
$syntax%
	const %VectorBase% &%w%
%$$
It specifies the weighting vector
in the definition of $latex W$$ above.


$head Order$$
The function call
$xref/ADFun/Order/F.Order()/$$ returns the highest order of the 
Taylor coefficients currently stored in $italic F$$.
This must be greater than or equal to $latex p-1$$ when
$syntax%%F%.Forward(%p%, %xp%)%$$ is called.
The value $syntax%%F%.Order()%$$ is not affected by this operation.

$head Memory$$
If the function $syntax%%F%.Reverse%$$ has never been called 
with as large a value for $italic p$$,
a large amount of memory proportional to the 
$xref/ADFun/Size/size/$$ of $italic F$$ times $italic p$$
is allocated for this calculation.
You can use the 
$xref/ADFun/Memory/F.Memory()/$$ function to determine
how much memory is required.

$head dw$$
The return value $italic dw$$ has prototype
$syntax%
	%VectorBase% %dw%
%$$
and is a vector of length $latex n \times p$$.
For $latex j = 0, \ldots, n-1$$ and $latex k = 0 , \ldots , p-1$$
$latex \[
	dw[ j * p + k ] = 
		\D{ W^{(k)} }{ x_j^{(0)} } 
			(0, x^{(0)}, \ldots , x^{(p-1)} ) 
	\hspace{2cm} (\star)
\] $$

$subhead First Order$$
Suppose  $latex p = 1$$ and $latex w$$ is the $th i$$ 
$xref/glossary/Elementary Vector/elementary vector/$$.
It follows that $latex k = 0$$ in $latex (\star)$$ and
$latex \[
dw[ j ] 
= \D{ W^{(0)} }{ x_j^{(0)} } (0, x^{(0)} ) 
= \D{ W }{ x_j^{(0)} } (0, x^{(0} ) 
= \D{ F_i \circ X }{ x_j^{(0)} } (0, x^{(0)} ) 
= \D{ F_i }{ x_j^{(0)} } ( x^{(0)} )
\] $$

$subhead Second Order$$
Suppose  $latex p = 2$$,
$latex x^{(1)}$$ is the $th l$$
$xref/glossary/Elementary Vector/elementary vector/$$
and $latex w$$ is the $th i$$ elementary vector. 
It follows that for $latex j = 0, \ldots, n-1$$
$latex \[
\begin{array}{rcl}
dw[ j * 2 + 0 ] 
& = & \D{ F_i }{ x_j^{(0)} } ( x^{(0)} )
\\
dw[ j * 2 + 1 ] 
& = & \D{ W^{(1)} }{ x_j^{(0)} } ( 0, x^{(0)} , x^{(1)} )  
\\
& = & 
\D{ }{ x_j^{(0)} } 
\D{F_i \circ X }{t} (0, x^{(0)} , x^{(1)} )  
\\
& = &
\D{ }{ x_j^{(0)} } 
F_i^{(1)} ( x^{(0)} ) * x^{(1)}
\\
& = &
\D{ }{ x_j^{(0)} } 
\D{F_i}{x_\ell^{(0)}} ( x^{(0)} ) 
\\
& = &
\DD{F_i}{x_j^{(0)}}{x_\ell^{(0)}} ( x^{(0)} ) 
\end{array}
\] $$

$subhead Hessian Times Direction$$
Suppose that $italic p = 2$$ and
$italic m$$ is equal to one 
($italic F$$ is scalar valued).
It follows that for $latex j = 0, \ldots, n-1$$
$latex \[
\begin{array}{rcl}
dw[ j * 2 + 1 ] 
& = & 
\D{ W^{(1)} }{ x_j^{(0)} } (0, x^{(0)} , x^{(1)} )  
\\
& = &
\D{ }{ x_j^{(0)} } 
F^{(1)} ( x^{(0)} ) * x^{(1)}
\\
& = &
\sum_{\ell=0}^{n-1} 
	\DD{F}{x_j^{(0)}}{x_\ell^{(0)}} ( x^{(0)} )  
		* x^{(1)}_\ell
\\
& = &
[ F^{(2)} ( x^{(0)} ) * x^{(1)} ]_j
\end{array}
\]$$


$head Example$$
$children%
	Example/Reverse.h%
	Example/HesTimesDir.cpp
%$$
The file
$xref/Reverse.h/$$
contains an example and test of the 
$xref/Reverse/dw/First Order/first/1/$$ and
$xref/Reverse/dw/Second Order/second/1/$$ order
calculations described above.
It returns true if it succeeds and false otherwise.
The file
$xref/HesTimesDir.cpp/$$
contains an example and test of the 
$xref/Reverse/dw/Hessian Times Direction/Hessian times direction/1/$$
calculation described above.

$end
-----------------------------------------------------------------------------
*/

// BEGIN CppAD namespace
namespace CppAD {

template <typename Base>
template <typename VectorBase>
VectorBase ADFun<Base>::Reverse(size_t p, const VectorBase &v)
{
	if( PartialColDim < p )
	{	if( Partial != CppADNull ) delete [] Partial;
		PartialColDim = p;
		Partial = new Base[totalNumVar * PartialColDim];
	}

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CppADUsageError(
		v.size() == depvar.size(),
		"Argument v to Reverse does not have length equal to\n"
		"the dimension of the range for the corresponding ADFun."
	);
	CppADUsageError(
		p > 0,
		"The first argument to Reverse must be greater than zero."
	);  
	CppADUsageError(
		order >= p-1,
		"Must first use Forward to compute (p-1)-th order Taylor\n"
		"coefficient matrix for this ADFun function."
	);  

	// initialize entire Partial matrix to zero
	size_t i;
	size_t j;
	for(i = 0; i < totalNumVar; i++)
		for(j = 0; j < p; j++)
			Partial[i * PartialColDim + j] = Base(0);

	// set the dependent variable direction
	// (use += because two dependent variables can point to same location)
	size_t n = depvar.size();
	for(i = 0; i < n; i++)
	{	CppADUnknownError( depvar[i] < totalNumVar );
		Partial[depvar[i] * PartialColDim + p - 1] += v[i];
	}

	// evaluate the derivatives
	ADReverse(p - 1, totalNumVar, Rec, 
		TaylorColDim, Taylor, PartialColDim, Partial);

	// return the derivative values
	size_t m = indvar.size();
	VectorBase value(m * p);
	for(i = 0; i < m; i++)
	{	CppADUnknownError( indvar[i] < totalNumVar );
		// independent variable taddr equals its operator taddr 
		CppADUnknownError( Rec->GetOp( indvar[i] ) == InvOp );

		// by the Reverse Identity Theorem 
		// partial of y^{(j)} w.r.t. u^{(0)} is equal to 				// partial of y^{(p-1)} w.r.t. u^{(p - 1 - j)}
		for(j = 0; j < p; j++)
			value[i * p + j ] = 
				Partial[indvar[i] * PartialColDim + p - 1 - j];
	}

	return value;
}
	

} // END CppAD namespace
	

# endif
