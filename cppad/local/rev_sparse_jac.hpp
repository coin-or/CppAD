# ifndef CPPAD_REV_SPARSE_JAC_INCLUDED
# define CPPAD_REV_SPARSE_JAC_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin RevSparseJac$$
$spell
	VecAD
	var
	Jacobian
	Jac
	const
	Bool
	Dep
	proportional
$$

$section Jacobian Sparsity Pattern: Reverse Mode$$ 

$index RevSparseJac$$
$index reverse, sparse Jacobian$$
$index sparse, reverse Jacobian$$
$index pattern, reverse Jacobian$$

$head Syntax$$
$syntax%%r% = %F%.RevSparseJac(%p%, %s%)%$$
$pre
$$


$head Purpose$$
We use $latex F : B^n \rightarrow B^m$$ to denote the
$xref/glossary/AD Function/AD function/$$ corresponding to $italic f$$.
For a fixed $latex p \times m$$ matrix $latex S$$,
the Jacobian of $latex S * F( x )$$
with respect to $latex x$$ is
$latex \[
	J(x) = S * F^{(1)} ( x )
\] $$
Given a
$xref/glossary/Sparsity Pattern/sparsity pattern/$$ 
for $latex S$$,
$code RevSparseJac$$ returns a sparsity pattern for the $latex J(x)$$.

$head f$$
The object $italic f$$ has prototype
$syntax%
	const ADFun<%Base%> %f%
%$$

$head x$$
If no $xref/VecAD/$$ objects are used by the
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$ 
stored in $italic f$$,
the sparsity pattern is valid for all values $latex x \in B^n$$.
$pre

$$
If $xref/SeqProperty/use_VecAD/f.use_VecAD/$$ is true,
the sparsity patter is only valid for the value of $italic x$$
in the previous $xref/ForwardZero//zero order forward mode/$$ call
$syntax%
	%f%.Forward(0, %x%)
%$$
If there is no previous zero order forward mode call using $italic f$$,
the value of the $xref/Independent//independent/$$ variables 
during the recording of the AD sequence of operations is used
for $italic x$$.

$head p$$
The argument $italic p$$ has prototype
$syntax%
	size_t %p%
%$$
It specifies the number of rows in the Jacobian $latex J(x)$$. 
Note that the memory required for the calculation is proportional 
to $latex p$$ times the total number of variables 
in the AD operation sequence corresponding to $italic f$$
($xref/SeqProperty/size_var/f.size_var/$$).
Smaller values for $italic p$$ can be used to
break the sparsity calculation 
into groups that do not require to much memory. 

$head s$$
The argument $italic s$$ has prototype
$syntax%
	const %Vector% &%s%
%$$
(see $xref/RevSparseJac/Vector/Vector/$$ below)
and its size is $latex p * m$$.
It specifies a 
$xref/glossary/Sparsity Pattern/sparsity pattern/$$ 
for the matrix $italic S$$ as follows:
for $latex i = 0 , \ldots , p-1$$ and $latex j = 0 , \ldots , m-1$$.
$latex \[
	S_{i,j} \neq 0 ; \Rightarrow \; s [ i * m + j ] = {\rm true}
\] $$

$head r$$
The return value $italic r$$ has prototype
$syntax%
	%Vector% %r%
%$$
(see $xref/RevSparseJac/Vector/Vector/$$ below)
and its size is $latex p * n$$.
It specifies a 
$xref/glossary/Sparsity Pattern/sparsity pattern/$$ 
for the matrix $latex J(x)$$ as follows:
for $latex x \in B^n$$,
for $latex i = 0 , \ldots , p-1$$,
and $latex j = 0 , \ldots , n-1$$
$latex \[
	J(x)_{i,j} \neq 0 ; \Rightarrow \; r [ i * n + j ] = {\rm true}
\] $$

$head Vector$$
The type $italic Vector$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type bool/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.
In order to save memory, 
you may want to use a class that packs multiple elements into one
storage location; for example,
$xref/CppAD_vector/vectorBool/vectorBool/$$.


$head Entire Sparsity Pattern$$
Suppose that $latex p = m$$ and
$latex S$$ is the $latex m \times m$$ identity matrix,
If follows that 
$latex \[
s [ i * q + j ] = \left\{ \begin{array}{ll}
	{\rm true}  & {\rm if} \; i = j \\
	{\rm flase} & {\rm otherwise}
\end{array} \right. 
\] $$
is an efficient sparsity pattern for the Jacobian of $latex S$$;
i.e., the choice for $italic s$$ has as few true values as possible.
In this case, 
the corresponding value for $italic r$$ is a 
sparsity pattern for the Jacobian $latex J(x) = F^{(1)} ( x )$$.


$head Example$$
$children%
	example/rev_sparse_jac.cpp
%$$
The file
$xref/RevSparseJac.cpp/$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
template <class Vector>
Vector ADFun<Base>::RevSparseJac(size_t p, const Vector &s) const
{
	// type used to pack bits (must support standard bit operations)
	typedef size_t Pack;

	// temporary indices
	size_t i, j, k, q;

	// check Vector is Simple Vector class with bool elements
	CheckSimpleVector<bool, Vector>();

	// range and domain dimensions for F
	size_t m = dep_taddr_.size();
	size_t n = ind_taddr_.size();

	CPPAD_ASSERT_KNOWN(
		p > 0,
		"RevSparseJac: p (first argument) is not greater than zero"
	);

	CPPAD_ASSERT_KNOWN(
		s.size() == p * m,
		"RevSparseJac: s (second argument) length is not equal to\n"
		"p (first argument) times range dimension for ADFun object."
	);

	
	// number of packed values per variable on the tape
	size_t npv = 1 + (p - 1) / sizeof(Pack);

	// array that will hold packed values
	Pack *RevJac = CPPAD_NULL;
	RevJac       = CPPAD_TRACK_NEW_VEC(total_num_var_ * npv, RevJac);

	// update maximum memory requirement
	// memoryMax = std::max( memoryMax, 
	// 	Memory() + total_num_var_ * npv * sizeof(Pack)
	// );

	// initialize entire RevJac matrix as false
	for(i = 0; i < total_num_var_; i++)
		for(k = 0; k < npv; k++)
			RevJac[ i * npv + k ] = 0;

	// set values corresponding to dependent variables
	Pack mask;
	for(j = 0; j < m; j++)
	{	CPPAD_ASSERT_UNKNOWN( dep_taddr_[j] < total_num_var_ );

		// set bits that are true
		for(i = 0; i < p; i++) 
		{	k    = i / sizeof(Pack);
			q    = i - k * sizeof(Pack);
			mask = Pack(1) << q;
			if( s[ i * m + j ] )
				RevJac[ dep_taddr_[j] * npv + k ] |= mask;
		}
	}

	// evaluate the sparsity patterns
	RevJacSweep(
		npv,
		total_num_var_,
		&play_,
		taylor_col_dim_,
		taylor_,
		RevJac
	);

	// return values corresponding to dependent variables
	Vector r(p * n);
	for(j = 0; j < n; j++)
	{	CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] < total_num_var_ );
		// ind_taddr_[j] is operator taddr for j-th independent variable
		CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[j] ) == InvOp );

		// set bits 
		for(i = 0; i < p; i++) 
		{	k     = i / sizeof(Pack);
			q     = i - k * sizeof(Pack);
			mask  = Pack(1) << q;
			mask &=	RevJac[ ind_taddr_[j] * npv + k ];
			r[ i * n + j ] = (mask != 0);
		}
	}

	// done with buffer
	CPPAD_TRACK_DEL_VEC(RevJac);

	return r;
}

} // END CppAD namespace
	

# endif
