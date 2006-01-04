# ifndef CppADForSparseJacIncluded
# define CppADForSparseJacIncluded

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
$begin ForSparseJac$$
$spell
	xk
	Py 
	Px
	Jacobian
	Jac
	const
	Bool
	Dep
	proportional
$$

$section Forward Mode Jacobian Sparsity Pattern$$ 

$index forward, sparse Jacobian$$
$index sparse, forward Jacobian$$
$index pattern, forward Jacobian$$

$table
$bold Syntax$$ $cnext
$syntax%%Py% = %F%.ForSparseJac(%q%, %Px%)%$$
$rnext $cnext
$tend

$fend 20$$

$head Description$$
Given the function 
$latex F : B^n \rightarrow B^m$$ defined by the object $italic F$$, 
a linear function $latex X : B^q \rightarrow B^n$$, and a 
$xref/glossary/Sparsity Pattern/sparsity pattern/$$ 
$latex Px$$ for the Jacobian of $latex X$$,
$code ForSparseJac$$ returns a sparsity pattern for the Jacobian 
$latex \[
	J = F^{(1)} ( x^{(0)} ) X^{(1)}
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

$head x0$$
The value $latex x^{(0)} \in B^n$$ is the value of $italic xk$$
in the previous call to 
$syntax%
	%F%.Forward(%k%, %xk%)
%$$
where $latex k = 0$$.
If there was no previous call with $latex k = 0$$,
$latex x^{(0)}$$ is the value of the independent variables
when $italic F$$ was constructed as an 
$xref/ADFun/$$ object.
	

$head q$$
The argument $italic q$$ has prototype
$syntax%
	size_t %q%
%$$
It specifies the number of columns of $latex J$$ we are computing the 
sparsity pattern with for.
Note that the memory required for the calculation is proportional 
to $latex q$$ times the total number of variables on the tape.
Thus it may be desireable to break the sparsity calculation into 
groups that do not require to much memory. 

$head Px$$
The argument $italic Px$$ has prototype
$syntax%
	const %VectorBool% &%Px%
%$$
and is a vector with size $latex n * q$$.
A sparsity pattern for the matrix
$latex X^{(1)} \in B^{n \times q}$$ is given by
$latex \[
	Px_{i,j} = Px [ i * q + j ]
\] $$
for $latex i = 1 , \ldots , n$$ and $latex j = 1 , \ldots , q$$.

$head Py$$
The return value $italic Py$$ has prototype
$syntax%
	%VectorBool% %Py%
%$$
and is a vector with size $latex m * q$$.
A sparsity pattern for the matrix
$latex \[
	J = F^{(1)} ( x^{(0)} ) X^{(1)}
\] $$
is given by $latex Py$$ where
$latex \[
	Py_{i,j} = Py [ i * q + j ]
\] $$
for $latex i = 1 , \ldots , m$$ and $latex j = 1 , \ldots , q$$.

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
Suppose that $latex q = n$$ and the function 
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
In the case defined above, 
the corresponding value for $italic Py$$ is a 
sparsity pattern for the Jacobian 
$latex J = F^{(1)} ( x^{(0)} )$$.

$head Example$$
$children%
	Example/ForSparseJac.cpp
%$$
The file
$xref/ForSparseJac.cpp/$$
contains an example and a test of this operation.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
template <class VectorBool>
VectorBool ADFun<Base>::ForSparseJac(size_t q, const VectorBool &Px)
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
		"ForSparseJac: q (first arugment) is not greater than zero"
	);

	CppADUsageError(
		Px.size() == n * q,
		"ForSparseJac: Py (second argument) length is not equal to\n"
		"q (first argument) times domain dimension for ADFun object."
	);

	// number of packed values per variable on the tape
	size_t npv = 1 + (q - 1) / sizeof(Pack);

	// array that will hold packed values
	if( ForJacColDim < npv )
	{	if( ForJacColDim > 0 )
			CppADTrackDelVec(ForJac);
		ForJac       = CppADTrackNewVec(totalNumVar * npv, ForJac);
		ForJacColDim = npv;
	}

	// set values corresponding to independent variables
	Pack mask;
	for(i = 0; i < n; i++)
	{	CppADUnknownError( ind_taddr[i] < totalNumVar );
		// ind_taddr[i] is operator taddr for i-th independent variable
		CppADUnknownError( Rec->GetOp( ind_taddr[i] ) == InvOp );

		// initialize all bits as zero
		for(k = 0; k < npv; k++)
			ForJac[ ind_taddr[i] * npv + k ] = 0;

		// set bits that are true
		for(j = 0; j < q; j++) 
		{	k    = j / sizeof(Pack);
			p    = j - k * sizeof(Pack);
			mask = Pack(1) << p;
			if( Px[ i * q + j ] )
				ForJac[ ind_taddr[i] * npv + k ] |= mask;
		}
	}

	// evaluate the sparsity patterns
	ForJacSweep(npv, totalNumVar, Rec, TaylorColDim, Taylor, ForJac);

	// return values corresponding to dependent variables
	VectorBool Py(m * q);
	for(i = 0; i < m; i++)
	{	CppADUnknownError( dep_taddr[i] < totalNumVar );

		// set bits 
		for(j = 0; j < q; j++) 
		{	k     = j / sizeof(Pack);
			p     = j - k * sizeof(Pack);
			mask  = Pack(1) << p;
			mask &=	ForJac[ dep_taddr[i] * npv + k ];
			Py[ i * q + j ] = (mask != 0);
		}
	}

	// update number of bits currently stored in ForJac
	ForJacBitDim = q;

	return Py;
}

} // END CppAD namespace
	

# endif
