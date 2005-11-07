# ifndef CppADForSparseJacIncluded
# define CppADForSparseJacIncluded

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
$begin ForSparseJac$$
$spell
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

$head Jacobian Sparsity Pattern$$
$index sparse, pattern$$
Suppose that $latex G : B^q \rightarrow B^p$$ is a differentiable function.
A boolean valued $latex p \times q$$ matrix $latex P$$ is a 
sparsity pattern for $latex G$$ if 
for $latex i = 1, \ldots , p$$ and $latex j = 1 , \ldots q$$,
$latex \[
P_{i,j} = {\rm false} \Rightarrow G^{(1)}_{i,j} (x) = 0  
\hspace{1cm} ({\rm for \; all}) \; x \in B^p
\] $$


$head Description$$
Given the function 
$latex F : B^n \rightarrow B^m$$ defined by the argument $italic F$$,
and a Jacobian sparsity pattern $latex Px$$ for a function
$latex X : B^q \rightarrow B^n$$,
$code ForSparseJac$$ returns a sparsity pattern for the function
$latex F \circ X : B^q \rightarrow B^m$$. 

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
It specifies the number of components we are computing the 
sparsity pattern with respect to.
Note that the memory required for the calculation is proportional 
to $latex q$$ times the total number of variables on the tape.
Thus it may be desireable to break the sparsity calculation into 
groups that do not require to much memory. 

$head Px$$
The argument $italic Px$$ has prototype
$syntax%
	const %VectorBool% &%Px%
%$$
and is a vector with size $latex q * n$$.
The sparsity pattern for $latex X$$ is given by
$latex \[
	Px_{i,j} = Px [ i * q + j ]
\] $$
for $latex i = 1 , \ldots , n$$ and $latex j = 1 , \ldots , q$$.

$head Py$$
The return value $italic Py$$ has prototype
$syntax%
	const %VectorBool% &%Py%
%$$
and is a vector with size $latex q * m$$.
The sparsity pattern for $latex F \circ X$$ is given by
$latex \[
	Py_{i,j} = Py [ i * q + j ]
\] $$
for $latex i = 1 , \ldots , m$$ and $latex j = 1 , \ldots , q$$.

$head VectorBool$$
The type $italic VectorBool$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type bool/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

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
is an efficient sparsity pattern for $latex X$$; i.e., the choice
for $italic Px$$ has a few true values as possible.
In addition, the corresponding value for $italic Py$$ is a 
sparsity pattern for $italic F$$.



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

	// check VectorBool is Simple Vector class with bpp; type elements
	CheckSimpleVector<bool, VectorBool>();

	// range and domain dimensions for F
	size_t m = depvar.size();
	size_t n = indvar.size();

	CppADUsageError(
	Px.size() == n * q,
	"Second argument to ForDep does not have length equal to\n"
	"first argument times domain dimension for the corresponding ADFun."
	);

	
	// number of packed values per variable on the tape
	size_t npv = 1 + q / sizeof(Pack);

	// array that will hold packed values
	if( ForJacColDim < npv )
	{	try
		{	ForJac = new Pack[totalNumVar * npv]; 
		}
		catch(...)
		{	CppADUsageError(0, "cannot allocate sufficient memory");
			abort();
		}
	}

	// set values corresponding to independent variables
	Pack mask;
	for(i = 0; i < n; i++)
	{	CppADUnknownError( indvar[i] < totalNumVar );
		// indvar[i] is operator taddr for i-th independent variable
		CppADUnknownError( Rec->GetOp( indvar[i] ) == InvOp );

		// initialize all bits as zero
		for(k = 0; k < npv; k++)
			ForJac[ indvar[i] * npv + k ] = 0;

		// set bits that are true
		for(j = 0; j < q; j++) 
		{	k    = j / sizeof(Pack);
			p    = j - k * sizeof(Pack);
			mask = Pack(1) << p;
			if( Px[ i * q + j ] )
				ForJac[ indvar[i] * npv + k ] |= mask;
		}
	}

	// evaluate the sparsity patterns
	ForJacSweep(npv, totalNumVar, Rec, ForJac);

	// return values corresponding to dependent variables
	VectorBool Py(m * q);
	for(i = 0; i < m; i++)
	{	CppADUnknownError( depvar[i] < totalNumVar );

		// set bits 
		for(j = 0; j < q; j++) 
		{	k     = j / sizeof(Pack);
			p     = j - k * sizeof(Pack);
			mask  = Pack(1) << p;
			mask &=	ForJac[ depvar[i] * npv + k ];
			Py[ i * q + j ] = (mask != 0);
		}
	}

	return Py;
}

} // END CppAD namespace
	

# endif
