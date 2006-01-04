# ifndef CppADRevSparseJacIncluded
# define CppADRevSparseJacIncluded

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
$begin RevSparseJac$$
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

$section Reverse Mode Jacobian Sparsity Pattern$$ 

$index reverse, sparse Jacobian$$
$index sparse, reverse Jacobian$$
$index pattern, reverse Jacobian$$

$table
$bold Syntax$$ $cnext
$syntax%%Px% = %F%.RevSparseJac(%p%, %Py%)%$$
$rnext $cnext
$tend

$fend 20$$


$head Description$$
Given the function 
$latex F : B^n \rightarrow B^m$$ defined by the object $italic F$$,
a linear function $latex Y : B^m \rightarrow B^p$$, and a
$xref/glossary/Sparsity Pattern/sparsity pattern/$$
$latex Py$$ for the Jacobian of $latex Y$$,
$code RevSparseJac$$ returns a sparsity pattern for the Jacobian 
$latex \[
	J = Y^{(1)} F^{(1)} ( x^{(0)} )
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

$head p$$
The argument $italic p$$ has prototype
$syntax%
	size_t %p%
%$$
It specifies the number of components we are computing the 
sparsity pattern with respect to.
Note that the memory required for the calculation is proportional 
to $latex p$$ times the total number of variables on the tape.
Thus it may be desireable to break the sparsity calculation into 
groups that do not require to much memory. 

$head Py$$
The argument $italic Py$$ has prototype
$syntax%
	const %VectorBool% &%Py%
%$$
and is a vector with size $latex p * m$$.
A sparsity pattern for the matrix
$latex Y^{(1}) \in B^{p \times m}$$ is given by
$latex \[
	Py_{i,j} = Py [ i * m + j ]
\] $$
for $latex i = 1 , \ldots , p$$ and $latex j = 1 , \ldots , m$$.

$head Px$$
The return value $italic Px$$ has prototype
$syntax%
	%VectorBool% %Px%
%$$
and is a vector with size $latex p * n$$.
A sparsity pattern for the Jacobian
$latex \[
	J = Y^{(1)} F^{(1)} ( x^{(0)} )
\] $$
is given by
$latex \[
	Py_{i,j} = Py [ i * n + j ]
\] $$
for $latex i = 1 , \ldots , p$$ and $latex j = 1 , \ldots , n$$.

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
Suppose that $latex p = m$$ and the function 
$latex Y : B^m \rightarrow B^p$$ is the identity; i.e., $latex Y(y) = y$$.
If follows that 
$latex \[
Py [ i * m + j ] = \left\{ \begin{array}{ll}
	{\rm true}  & {\rm if} \; i = j \\
	{\rm flase} & {\rm otherwise}
\end{array} \right. 
\] $$
is an efficient sparsity pattern for the Jacobian of $latex X$$; 
i.e., the choice for $italic Py$$ has as few true values as possible.
$pre

$$
In the case defined above, 
the corresponding value for $italic Px$$ is a 
sparsity pattern for the Jacobian 
$latex J = F^{(1)} ( x^{(0)} )$$.



$head Example$$
$children%
	Example/RevSparseJac.cpp
%$$
The file
$xref/RevSparseJac.cpp/$$
contains an example and a test of this operation.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
template <class VectorBool>
VectorBool ADFun<Base>::RevSparseJac(size_t p, const VectorBool &Py)
{
	// type used to pack bits (must support standard bit operations)
	typedef size_t Pack;

	// temporary indices
	size_t i, j, k, q;

	// check VectorBool is Simple Vector class with bool elements
	CheckSimpleVector<bool, VectorBool>();

	// range and domain dimensions for F
	size_t m = dep_taddr.size();
	size_t n = ind_taddr.size();

	CppADUsageError(
		p > 0,
		"RevSparseJac: p (first argument) is not greater than zero"
	);

	CppADUsageError(
		Py.size() == p * m,
		"RevSparseJac: Py (second argument) length is not equal to\n"
		"p (first argument) times range dimension for ADFun object."
	);

	
	// number of packed values per variable on the tape
	size_t npv = 1 + (p - 1) / sizeof(Pack);

	// array that will hold packed values
	Pack *RevJac = CppADNull;
	RevJac       = CppADTrackNewVec(totalNumVar * npv, RevJac);

	// update maximum memory requirement
	memoryMax = std::max( memoryMax, 
		Memory() + totalNumVar * npv * sizeof(Pack)
	);

	// initialize entire RevJac matrix as false
	for(i = 0; i < totalNumVar; i++)
		for(k = 0; k < npv; k++)
			RevJac[ i * npv + k ] = 0;

	// set values corresponding to dependent variables
	Pack mask;
	for(j = 0; j < m; j++)
	{	CppADUnknownError( dep_taddr[j] < totalNumVar );

		// set bits that are true
		for(i = 0; i < p; i++) 
		{	k    = i / sizeof(Pack);
			q    = i - k * sizeof(Pack);
			mask = Pack(1) << q;
			if( Py[ i * m + j ] )
				RevJac[ dep_taddr[j] * npv + k ] |= mask;
		}
	}

	// evaluate the sparsity patterns
	RevJacSweep(npv, totalNumVar, Rec, TaylorColDim, Taylor, RevJac);

	// return values corresponding to dependent variables
	VectorBool Px(p * n);
	for(j = 0; j < n; j++)
	{	CppADUnknownError( ind_taddr[j] < totalNumVar );
		// ind_taddr[j] is operator taddr for j-th independent variable
		CppADUnknownError( Rec->GetOp( ind_taddr[j] ) == InvOp );

		// set bits 
		for(i = 0; i < p; i++) 
		{	k     = i / sizeof(Pack);
			q     = i - k * sizeof(Pack);
			mask  = Pack(1) << q;
			mask &=	RevJac[ ind_taddr[j] * npv + k ];
			Px[ i * n + j ] = (mask != 0);
		}
	}

	// done with buffer
	CppADTrackDelVec(RevJac);

	return Px;
}

} // END CppAD namespace
	

# endif
