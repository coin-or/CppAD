# ifndef CppADRevSparseHesIncluded
# define CppADRevSparseHesIncluded

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
$begin RevSparseHes$$
$spell
	Py 
	Px
	Pxx
	Jacobian
	Jac
	Hessian
	Hes
	const
	Bool
	Dep
	proportional
$$

$section Reverse Mode Hessian Sparsity Pattern$$ 

$index reverse, sparse Hessian$$
$index sparse, reverse Hessian$$
$index pattern, reverse Hessian$$

$table
$bold Syntax$$ $cnext
$syntax%%Pxx% = %F%.RevSparseHes(%q%, %Py%)%$$
$rnext $cnext
$tend

$fend 20$$


$head Description$$
Given the function 
$latex F : B^n \rightarrow B^m$$ defined by the object $italic F$$,
a linear function $latex X : B^q \rightarrow B^n$$, 
a linear function $latex Y : B^m \rightarrow B$$, a
$xref/glossary/Sparsity Pattern/sparsity pattern/$$
$latex Px$$ for the Jacobian of $latex X$$,
and a sparsity pattern $latex Py$$ for the Jacobian of $latex Y$$, 
$code RevSparseHes$$ returns a sparsity pattern for the Hessian of
$latex (Y \circ F \circ X) : B^q \rightarrow B$$. 

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

$head q$$
The argument $italic q$$ has prototype
$syntax%
	size_t %q%
%$$
It specifies the number of argument components we are computing the 
sparsity pattern with respect to.
It must be the same value as in the previous call to
$syntax%
	%F%.ForSparseJac(%q%, %Px%)
%$$
Note that the memory required for the calculation is proportional 
to $latex q$$ times the total number of variables on the tape.
Thus it may be desireable to break the sparsity calculation into 
groups that do not require to much memory. 

$head Px$$
We use $italic Px$$ to denote its value in the previous call of the form
$syntax%
	%F%.ForSparseJac(%q%, %Px%)
%$$
A sparsity pattern for the Jacobian of
$latex X^{(1}) : B^q \rightarrow B^{n \times q}$$ is given by
$latex \[
	Px_{i,j} = Px [ i * n + j ]
\] $$
for $latex i = 1 , \ldots , n$$ and $latex j = 1 , \ldots , q$$.

$head Py$$
The argument value $italic Py$$ has prototype
$syntax%
	const %VectorBool% %Py%
%$$
and is a vector with size $latex m$$.
A sparsity pattern for the Jacobian of
$latex Y^{(1)} : B^m \rightarrow B$$ is given by
$latex \[
	Py_{0,j} = Py [ j ]
\] $$
for $latex j = 1 , \ldots , m$$.

$head Pxx$$
The return value $italic Pxx$$ has prototype
$syntax%
	%VectorBool% &%Pxx%
%$$
and is a vector with size $latex n * q$$.
It is a sparsity pattern for the function
$latex H : B^q \rightarrow B^{q \times n}$$
defined by
$latex \[
H(u) = \frac{d}{d u} \frac{d}{d x}  Y \{ F[ x + X(u) ] \}
\] $$
is given by
$latex \[
	Pxx_{i,j} = Px [ i * n + j ]
\] $$
for $latex i = 1 , \ldots , q$$ and $latex j = 1 , \ldots , n$$.
(Note that in the definition of $latex H$$ 
rows correspond to the derivatives with respect to $latex u \in B^q$$ 
and the columns correspond to derivatives with respect to $latex x \in B^n$$.)

$head VectorBool$$
The type $italic VectorBool$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type bool/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.
In order to save memory, 
you may want to use a class that packs multiple elements into one
storage location; for example,
$xref/CppAD_vector/vectorBool/vectorBool/$$.

$head Entire Sparsity Pattern$$
If $latex q = n$$ and the function 
$latex X : B^q \rightarrow B^n$$ is the identity; i.e., $latex X(x) = x$$.
If follows that 
$latex \[
Px [ i * q + j ] = \left\{ \begin{array}{ll}
	{\rm true}  & {\rm if} \; i = j \\
	{\rm flase} & {\rm otherwise}
\end{array} \right. 
\] $$
is an efficient sparsity pattern for the Jacobian of $latex X$$; 
i.e., the choice for $italic Px$$ has as few true values as possible.
$pre

$$
Further suppose that the function $latex Y : B^m \rightarrow B$$
is just the value of the $th k$$ component of $latex Y$$; i.e.,
$latex Y(y) = y_k$$.
If follows that 
$latex \[
Py [ j ] = \left\{ \begin{array}{ll}
	{\rm true}  & {\rm if} \; j = k \\
	{\rm flase} & {\rm otherwise}
\end{array} \right. 
\] $$
is an efficient sparsity pattern for the Jacobian of $latex Y$$. 
$pre

$$
In the case defined above,
the corresponding value for $italic Pxx$$ is a 
sparsity pattern for the Hessian $latex F_k^{(2)} (x)$$.

$head Example$$
$children%
	Example/RevSparseHes.cpp
%$$
The file
$xref/RevSparseHes.cpp/$$
contains an example and a test of this operation.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/
# include <algorithm>

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
template <class VectorBool>
VectorBool ADFun<Base>::RevSparseHes(
	size_t            q  , 
	const VectorBool &Py )
{
	// type used to pack bits (must support standard bit operations)
	typedef size_t Pack;

	// temporary indices
	size_t i, j, k, p;

	// check VectorBool is Simple Vector class with bool elements
	CheckSimpleVector<bool, VectorBool>();

	// range and domain dimensions for F
	size_t m = dep_taddr.size();
	size_t n = ind_taddr.size();

	CppADUsageError(
		q > 0,
		"RevSparseHes: q (first argument) is not greater than zero"
	);
	CppADUsageError(
		q == ForJacBitDim,
		"RevSparseHes: q (first argument) is not equal to its value"
		" in the previous call to ForSparseJac with this ADFun object."
	);
	CppADUsageError(
		Py.size() == m,
		"RevSparseHes: Py (third argument) length is not equal to\n"
		"range dimension for ADFun object."
	);
	
	// number of packed values per variable on the tape
	size_t npv = 1 + (q - 1) / sizeof(Pack);
	CppADUnknownError( npv <= ForJacColDim );

	// array that will hold packed reverse Jacobian values
	Pack *RevJac = CppADNull;
	RevJac       = CppADTrackNewVec(totalNumVar, RevJac);	

	// array that will hold packed reverse Hessain values
	Pack *RevHes = CppADNull;
	RevHes       = CppADTrackNewVec(totalNumVar * npv, RevHes);	

	// update maximum memory requirement
	memoryMax = std::max( memoryMax, 
		Memory() + totalNumVar * (npv + 1) * sizeof(Pack)
	);

	// initialize entire RevHes and RevJac matrix to false
	for(i = 0; i < totalNumVar; i++)
	{	RevJac[i] = 0;
		for(k = 0; k < npv; k++)
			RevHes[ i * npv + k ] = 0;
	}
	for(i = 0; i < m; i++)
	{	CppADUnknownError( dep_taddr[i] < totalNumVar );
		if( Py[i] )
		{	// set all the bits to true
			RevJac[ dep_taddr[i] ] = ~0;
		}
	}

	// comput the reverse mode Jacobian sparsity
	RevJacSweep(1, totalNumVar, Rec, TaylorColDim, Taylor, RevJac);

	// compute the Hessian sparsity patterns
	RevHesSweep(npv, totalNumVar, Rec, TaylorColDim, Taylor, ForJac, 
		RevJac, RevHes
	);

	// return values corresponding to independent variables
	Pack mask;
	VectorBool Pxx(n * q);

	// j is index corresponding to reverse mode martial
	for(j = 0; j < n; j++)
	{	CppADUnknownError( ind_taddr[j] < totalNumVar );

		// ind_taddr[j] is operator taddr for j-th independent variable
		CppADUnknownError( Rec->GetOp( ind_taddr[j] ) == InvOp );

		// i is index corresponding to forward mode partial
		for(i = 0; i < q; i++) 
		{	k     = i / sizeof(Pack);
			p     = i - k * sizeof(Pack);
			mask  = Pack(1) << p;
			mask &=	RevHes[ ind_taddr[j] * npv + k ];
			Pxx[ i * n + j ] = (mask != 0);
		}
	}

	// free memory used for the calculation
	CppADTrackDelVec(RevJac);
	CppADTrackDelVec(RevHes);

	return Pxx;
}

} // END CppAD namespace
	
# endif
