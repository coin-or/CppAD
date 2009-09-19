/* $Id$ */
# ifndef CPPAD_FOR_SPARSE_JAC_INCLUDED
# define CPPAD_FOR_SPARSE_JAC_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ForSparseJac$$
$spell
	var
	Jacobian
	Jac
	const
	Bool
	proportional
	VecAD
$$

$section Jacobian Sparsity Pattern: Forward Mode$$ 

$index ForSparseJac$$
$index forward, sparsity Jacobian$$
$index sparsity, forward Jacobian$$
$index pattern, forward Jacobian$$

$head Syntax$$
$syntax%%s% = %f%.ForSparseJac(%q%, %r%)%$$


$head Purpose$$
We use $latex F : B^n \rightarrow B^m$$ to denote the
$xref/glossary/AD Function/AD function/$$ corresponding to $italic f$$.
For a fixed $latex n \times q$$ matrix $latex R$$,
the Jacobian of $latex F[ x + R * u ]$$
with respect to $latex u$$ at $latex u = 0$$ is
$latex \[
	J(x) = F^{(1)} ( x ) * R
\] $$
Given a
$xref/glossary/Sparsity Pattern/sparsity pattern/$$ 
for $latex R$$,
$code ForSparseJac$$ returns a sparsity pattern for the $latex J(x)$$.

$head f$$
The object $italic f$$ has prototype
$syntax%
	ADFun<%Base%> %f%
%$$
Note that the $xref/ADFun/$$ object $italic f$$ is not $code const$$.
After this the sparsity pattern
for each of the variables in the operation sequence
is stored in the object $italic f$$.


$head x$$
the sparsity pattern is valid for all values of the independent 
variables in $latex x \in B^n$$
(even if you use $cref/CondExp/$$ or $cref/VecAD/$$).

$head q$$
The argument $italic q$$ has prototype
$syntax%
	size_t %q%
%$$
It specifies the number of columns in the Jacobian $latex J(x)$$. 
Note that the memory required for the calculation is proportional 
to $latex q$$ times the total number of variables 
in the AD operation sequence corresponding to $italic f$$
($xref/SeqProperty/size_var/f.size_var/$$).
Smaller values for $italic q$$ can be used to
break the sparsity calculation 
into groups that do not require to much memory. 

$head r$$
The argument $italic r$$ has prototype
$syntax%
	const %Vector% &%r%
%$$
(see $xref/ForSparseJac/Vector/Vector/$$ below)
and its size is $latex n * q$$.
It specifies a 
$xref/glossary/Sparsity Pattern/sparsity pattern/$$ 
for the matrix $italic R$$ as follows:
for $latex i = 0 , \ldots , n-1$$ and $latex j = 0 , \ldots , q-1$$.
$latex \[
	R_{i,j} \neq 0 ; \Rightarrow \; r [ i * q + j ] = {\rm true}
\] $$

$head s$$
The return value $italic s$$ has prototype
$syntax%
	%Vector% %s%
%$$
(see $xref/ForSparseJac/Vector/Vector/$$ below)
and its size is $latex m * q$$.
It specifies a 
$xref/glossary/Sparsity Pattern/sparsity pattern/$$ 
for the matrix $latex J(x)$$ as follows:
for $latex x \in B^n$$,
for $latex i = 0 , \ldots , m-1$$,
and $latex j = 0 , \ldots , q-1$$
$latex \[
	J(x)_{i,j} \neq 0 ; \Rightarrow \; s [ i * q + j ] = {\rm true}
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
	{\rm flase} & {\rm otherwise}
\end{array} \right. 
\] $$
is an efficient sparsity pattern for $latex R$$; 
i.e., the choice for $italic r$$ has as few true values as possible.
In this case, 
the corresponding value for $italic s$$ is a 
sparsity pattern for the Jacobian $latex J(x) = F^{(1)} ( x )$$.

$head Example$$
$children%
	example/for_sparse_jac.cpp
%$$
The file
$xref/ForSparseJac.cpp/$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
template <class Vector>
Vector ADFun<Base>::ForSparseJac(size_t q, const Vector &r)
{
	// temporary indices
	size_t i, j;

	// check Vector is Simple Vector class with bool elements
	CheckSimpleVector<bool, Vector>();

	// range and domain dimensions for F
	size_t m = dep_taddr_.size();
	size_t n = ind_taddr_.size();

	CPPAD_ASSERT_KNOWN(
		q > 0,
		"ForSparseJac: q (first arugment) is not greater than zero"
	);

	CPPAD_ASSERT_KNOWN(
		r.size() == n * q,
		"ForSparseJac: r (second argument) length is not equal to\n"
		"q (first argument) times domain dimension for ADFun object."
	);
	for_jac_sparsity_.resize(total_num_var_, q);

	// number of packed values per variable on the tape
	size_t npv = for_jac_sparsity_.n_pack();
	for_jac_col_dim_ = npv;

	// set values corresponding to independent variables
	for(i = 0; i < n; i++)
	{	CPPAD_ASSERT_UNKNOWN( ind_taddr_[i] < total_num_var_ );
		// ind_taddr_[i] is operator taddr for i-th independent variable
		CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[i] ) == InvOp );

		// set bits that are true
		for(j = 0; j < q; j++) if( r[ i * q + j ] )
			for_jac_sparsity_.set_element( ind_taddr_[i], j);
	}

	// evaluate the sparsity patterns
	ForJacSweep(
		n,
		total_num_var_,
		&play_,
		for_jac_sparsity_
	);

	// return values corresponding to dependent variables
	Vector s(m * q);
	for(i = 0; i < m; i++)
	{	CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < total_num_var_ );

		// set bits 
		for(j = 0; j < q; j++) s[ i * q + j ] = 
			for_jac_sparsity_.get_element( dep_taddr_[i], j);
	}

	// update number of bits currently stored in for_jac_
	for_jac_bit_dim_ = q;

	return s;
}

} // END CppAD namespace

# endif
