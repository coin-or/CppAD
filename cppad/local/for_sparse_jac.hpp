# ifndef CPPAD_FOR_SPARSE_JAC_INCLUDED
# define CPPAD_FOR_SPARSE_JAC_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

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
$pre
$$


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
If no $xref/VecAD/$$ objects are used by the
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$ 
stored in $italic f$$,
the sparsity pattern is valid for all values $latex x \in B^n$$.
$pre

$$
If $xref/SeqProperty/use_VecAD/f.useVecAD/$$ is true,
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
	// type used to pack bits (must support standard bit operations)
	typedef size_t Pack;

	// temporary indices
	size_t i, j, k, p;

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

	// number of packed values per variable on the tape
	size_t npv = 1 + (q - 1) / sizeof(Pack);

	// array that will hold packed values
	if( for_jac_col_dim_ < npv )
	{	if( for_jac_col_dim_ > 0 )
			CPPAD_TRACK_DEL_VEC(for_jac_);
		for_jac_ = CPPAD_TRACK_NEW_VEC(total_num_var_ * npv, for_jac_);
		for_jac_col_dim_ = npv;
	}

	// set values corresponding to independent variables
	Pack mask;
	for(i = 0; i < n; i++)
	{	CPPAD_ASSERT_UNKNOWN( ind_taddr_[i] < total_num_var_ );
		// ind_taddr_[i] is operator taddr for i-th independent variable
		CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[i] ) == InvOp );

		// initialize all bits as zero
		for(k = 0; k < npv; k++)
			for_jac_[ ind_taddr_[i] * npv + k ] = 0;

		// set bits that are true
		for(j = 0; j < q; j++) 
		{	k    = j / sizeof(Pack);
			p    = j - k * sizeof(Pack);
			mask = Pack(1) << p;
			if( r[ i * q + j ] )
				for_jac_[ ind_taddr_[i] * npv + k ] |= mask;
		}
	}

	// evaluate the sparsity patterns
	ForJacSweep(
		npv,
		total_num_var_,
		&play_,
		taylor_col_dim_,
		taylor_,
		for_jac_
	);

	// return values corresponding to dependent variables
	Vector s(m * q);
	for(i = 0; i < m; i++)
	{	CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < total_num_var_ );

		// set bits 
		for(j = 0; j < q; j++) 
		{	k     = j / sizeof(Pack);
			p     = j - k * sizeof(Pack);
			mask  = Pack(1) << p;
			mask &=	for_jac_[ dep_taddr_[i] * npv + k ];
			s[ i * q + j ] = (mask != 0);
		}
	}

	// update number of bits currently stored in for_jac_
	for_jac_bit_dim_ = q;

	return s;
}

} // END CppAD namespace

# endif
