/* $Id$ */
# ifndef CPPAD_REV_SPARSE_HES_INCLUDED
# define CPPAD_REV_SPARSE_HES_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin RevSparseHes$$
$spell
	VecAD
	Jacobian
	Jac
	Hessian
	Hes
	const
	Bool
	Dep
	proportional
	var
$$

$section Hessian Sparsity Pattern: Reverse Mode$$ 

$index RevSparseHes$$
$index reverse, sparse Hessian$$
$index sparse, reverse Hessian$$
$index pattern, reverse Hessian$$

$head Syntax$$
$syntax%%h% = %f%.RevSparseHes(%q%, %s%)%$$


$head Purpose$$
We use $latex F : B^n \rightarrow B^m$$ to denote the
$xref/glossary/AD Function/AD function/$$ corresponding to $italic f$$.
For a fixed $latex n \times q$$ matrix $latex R$$
and a fixed $latex 1 \times m$$ matrix $latex S$$,
the second partial of $latex S * F[ x + R * u ]$$
with respect to $latex u$$ at $latex u = 0$$ and with respect to x
$latex \[
	H(x)  =  R^T * (S * F)^{(2)} ( x )
\] $$
where $latex (S * F)^{(2)} (x)$$ is the Hessian of the scalar
valued function $latex S * F (x)$$.
Given a
$xref/glossary/Sparsity Pattern/sparsity pattern/$$ 
for $latex R$$ and $latex S$$,
$code RevSparseHes$$ returns a sparsity pattern for the $latex H(x)$$.

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

$head q$$
The argument $italic q$$ has prototype
$syntax%
	size_t %q%
%$$
It specifies the number of columns in the Jacobian $latex J(x)$$. 
It must be the same value as in the previous $xref/ForSparseJac/$$ call 
$syntax%
	%f%.ForSparseJac(%q%, %r%)
%$$
Note that the memory required for the calculation is proportional
to $latex q$$ times the total number of variables
in the AD operation sequence corresponding to $italic f$$
($xref/SeqProperty/size_var/f.size_var/$$).

$head r$$
The argument $italic r$$ in the previous call
$syntax%
	%f%.ForSparseJac(%q%, %r%)
%$$
is a sparsity pattern for the matrix $latex R$$ above; i.e.,
for $latex i = 0 , \ldots , n-1$$ and $latex j = 0 , \ldots , q-1$$.
$latex \[
	R_{i,j} \neq 0 ; \Rightarrow \; r [ i * q + j ] = {\rm true}
\] $$

$head s$$
The argument $italic s$$ has prototype
$syntax%
	const %Vector% &%s%
%$$
(see $xref/RevSparseHes/Vector/Vector/$$ below)
and its size is $latex m$$.
It specifies a 
$xref/glossary/Sparsity Pattern/sparsity pattern/$$ 
for the matrix $italic S$$ as follows:
for $latex j = 0 , \ldots , m-1$$.
$latex \[
	S_{0,j} \neq 0 ; \Rightarrow \; s [ j ] = {\rm true}
\] $$

$head h$$
The result $italic h$$ has prototype
$syntax%
	%Vector% &%h%
%$$
(see $xref/RevSparseHes/Vector/Vector/$$ below)
and its size is $latex q * n$$,
It specifies a 
$xref/glossary/Sparsity Pattern/sparsity pattern/$$ 
for the matrix $latex H(x)$$ as follows:
for $latex x \in B^n$$,
for $latex i = 0 , \ldots , q-1$$,
and $latex j = 0 , \ldots , n-1$$
$latex \[
	H(x)_{i,j} \neq 0 ; \Rightarrow \; h [ i * n + j ] = {\rm true}
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
Suppose that $latex q = n$$ and
$latex R$$ is the $latex n \times n$$ identity matrix,
If follows that 
$latex \[
r [ i * q + j ] = \left\{ \begin{array}{ll}
	{\rm true}  & {\rm if} \; i = j \\
	{\rm false} & {\rm otherwise}
\end{array} \right. 
\] $$
is an efficient sparsity pattern for $latex R$$; 
i.e., the choice for $italic r$$ has as few true values as possible.
Further suppose that the $latex S$$ is the $th k$$
$xref/glossary/Elementary Vector/elementary vector/$$
If follows that 
$latex \[
s [ j ] = \left\{ \begin{array}{ll}
	{\rm true}  & {\rm if} \; j = k \\
	{\rm false} & {\rm otherwise}
\end{array} \right. 
\] $$
is an efficient sparsity pattern for $latex S$$. 
In the case defined above,
the result $italic h$$ corresponds to a
sparsity pattern for the Hessian $latex F_k^{(2)} (x)$$.

$head Example$$
$children%
	example/rev_sparse_hes.cpp
%$$
The file
$xref/RevSparseHes.cpp/$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/
# include <algorithm>

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
template <class Vector>
Vector ADFun<Base>::RevSparseHes(size_t q,  const Vector &s)
{
	// temporary indices
	size_t i, j;

	// check Vector is Simple Vector class with bool elements
	CheckSimpleVector<bool, Vector>();

	// range and domain dimensions for F
	size_t m = dep_taddr_.size();
	size_t n = ind_taddr_.size();

	CPPAD_ASSERT_KNOWN(
		q == for_jac_sparsity_.limit(),
		"RevSparseHes: q (first argument) is not equal to its value"
		" in the previous call to ForSparseJac with this ADFun object."
	);
	CPPAD_ASSERT_KNOWN(
		s.size() == m,
		"RevSparseHes: s (third argument) length is not equal to\n"
		"range dimension for ADFun object."
	);

	// array that will hold packed reverse Jacobian values
	bool *RevJac = CPPAD_NULL;
	RevJac       = CPPAD_TRACK_NEW_VEC(total_num_var_, RevJac);	

	// vector of sets that will hold packed reverse Hessain values
	vector_pack      rev_hes_sparsity;
	rev_hes_sparsity.resize(total_num_var_, q);

	// initialize RevJac matrix to false
	for(i = 0; i < total_num_var_; i++)
		RevJac[i] = false;

	for(i = 0; i < m; i++)
	{	CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < total_num_var_ );
		RevJac[ dep_taddr_[i] ] = s[i];
	}

	// compute the Hessian sparsity patterns
	RevHesSweep(
		n,
		total_num_var_,
		&play_,
		for_jac_sparsity_, 
		RevJac,
		rev_hes_sparsity
	);

	// return values corresponding to independent variables
	Vector h(n * q);

	// j is index corresponding to reverse mode martial
	for(j = 0; j < n; j++)
	{	CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] < total_num_var_ );

		// ind_taddr_[j] is operator taddr for j-th independent variable
		CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] == j + 1 );
		CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[j] ) == InvOp );

		// i is index corresponding to forward mode partial
		for(i = 0; i < q; i++) 
			h[ i * n + j ] = false;

		CPPAD_ASSERT_UNKNOWN( rev_hes_sparsity.limit() == q );
		i = rev_hes_sparsity.next_element(j + 1);
		while( i < q )
		{	h[ i * n + j ] = true;
			i = rev_hes_sparsity.next_element(j + 1);
		}
	}

	// free memory used for the calculation
	CPPAD_TRACK_DEL_VEC(RevJac);

	return h;
}

} // END CppAD namespace
	
# endif
