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
	H(x)  =  R^T * (S * F)^{(2)} ( x ) * R
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
It specifies the number of rows and columns in the Hessian $latex H(x)$$.
It must be the same value as in the previous $xref/ForSparseJac/$$ call 
$syntax%
	%f%.ForSparseJac(%q%, %r%, %packed%)
%$$
Note that if $italic packed$$ was true,
the memory required for the calculation is proportional
to $latex q$$ times the total number of variables
in the AD operation sequence corresponding to $italic f$$
($xref/SeqProperty/size_var/f.size_var/$$).

$subhead packed$$
If $italic packed$$ was true in the call to $code ForSparseJac$$,
during the sparsity calculation sets of indices are represented
as vectors of bits that packed into words and operations are done
on multiple bits at a time (the number of bits in a word is unspecified).
Otherwise, sets of indices are represents using a sparse structure
that only includes the non-zero indices and operations are done
one index at a time. 
$pre

$$
The default value for $italic packed$$ is true; i.e.,
the value used if it is not present.

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
	const %VectorBool% &%s%
%$$
(see $xref/RevSparseHes/VectorBool/VectorBool/$$ below)
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
	%VectorBool% &%h%
%$$
(see $xref/RevSparseHes/VectorBool/VectorBool/$$ below)
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

CPPAD_BEGIN_NAMESPACE
/*!
\file rev_sparse_hes.hpp
Reverse mode Hessian sparsity patterns.
*/

/*!
Calculate Hessian sparsity patterns using reverse mode.

The C++ source code corresponding to this operation is
\verbatim
	h = f.RevSparseHes(q, s)
\endverbatim

\tparam Base
is the base type for this recording.

\tparam VectorBool
is a simple vector with elements of type bool.

\tparam VectorSet
is either \c vector_pack or \c vector_set. 

\param q
is the value of \a q in the 
by the previous call of the form 
\verbatim
	f.ForSparseJac(q, r, packed)
\endverbatim
The value \c r in this call is a sparsity pattern for the matrix \f$ R \f$.

\param s
is a vector with size \c m that specifies the sparsity pattern
for the vector \f$ S \f$,
where \c m is the number of dependent variables
corresponding to the operation sequence stored in \a play. 

\param h
the input value of \a h must be a vector with size \c q*q.
The input value of its elements does not matter.
On output, \a h is the sparsity pattern for the matrix
\f[
	H(x) = R^T ( S * F)^{(2)} (x) * R
\f]
where \f$ F \f$ is the function corresponding to the operation sequence
and \a x is any argument value.

\param total_num_var
is the total number of variables in this recording.

\param dep_taddr
maps dependendent variable index
to the corresponding variable in the tape.

\param ind_taddr
maps independent variable index
to the corresponding variable in the tape.

\param play
is the recording that defines the function we are computing the sparsity 
pattern for.

\param for_jac_sparsity
is a  vector of sets containing the 
the forward sparsity pattern for all of the variables on the tape. 
*/

template <class Base, class VectorBool, class VectorSet>
void RevSparseHes(
	size_t                    q                 ,
	const VectorBool&         s                 ,
	VectorBool&               h                 ,
	size_t                    total_num_var     ,
	CppAD::vector<size_t>&    dep_taddr         ,
	CppAD::vector<size_t>&    ind_taddr         ,
	CppAD::player<Base>&      play              ,
	VectorSet&                for_jac_sparsity  )
{
	// temporary indices
	size_t i, j;

	// check Vector is Simple VectorBool class with bool elements
	CheckSimpleVector<bool, VectorBool>();

	// range and domain dimensions for F
	size_t m = dep_taddr.size();
	size_t n = ind_taddr.size();

	CPPAD_ASSERT_KNOWN(
		q == for_jac_sparsity.end(),
		"RevSparseHes: q (first argument) is not equal to its value"
		" in the previous call to ForSparseJac with this ADFun object."
	);
	CPPAD_ASSERT_KNOWN(
		s.size() == m,
		"RevSparseHes: s (third argument) length is not equal to\n"
		"range dimension for ADFun object."
	);

	// Array that will hold reverse Jacobian dependency flag.
	// Initialize as true for the dependent variables.
	bool *RevJac = CPPAD_NULL;
	RevJac       = CPPAD_TRACK_NEW_VEC(total_num_var, RevJac);	
	for(i = 0; i < total_num_var; i++)
		RevJac[i] = false;
	for(i = 0; i < m; i++)
	{	CPPAD_ASSERT_UNKNOWN( dep_taddr[i] < total_num_var );
		RevJac[ dep_taddr[i] ] = s[i];
	}


	// vector of sets that will hold packed reverse Hessain values
	VectorSet      rev_hes_sparsity;
	rev_hes_sparsity.resize(total_num_var, q);

	// compute the Hessian sparsity patterns
	RevHesSweep(
		n,
		total_num_var,
		&play,
		for_jac_sparsity, 
		RevJac,
		rev_hes_sparsity
	);

	// return values corresponding to independent variables
	CPPAD_ASSERT_UNKNOWN( h.size() == n * q );

	// j is index corresponding to reverse mode partial
	for(j = 0; j < n; j++)
	{	CPPAD_ASSERT_UNKNOWN( ind_taddr[j] < total_num_var );

		// ind_taddr[j] is operator taddr for j-th independent variable
		CPPAD_ASSERT_UNKNOWN( ind_taddr[j] == j + 1 );
		CPPAD_ASSERT_UNKNOWN( play.GetOp( ind_taddr[j] ) == InvOp );

		// set all bits false
		for(i = 0; i < q; i++) 
			h[ i * n + j ] = false;

		// set bits that are true
		CPPAD_ASSERT_UNKNOWN( rev_hes_sparsity.end() == q );
		rev_hes_sparsity.begin(j + 1);
		i = rev_hes_sparsity.next_element();
		while( i < q )
		{	h[ i * n + j ] = true;
			i = rev_hes_sparsity.next_element();
		}
	}

	// free memory used for the calculation
	CPPAD_TRACK_DEL_VEC(RevJac);

	return;
}


/*!
User API for Hessian sparsity patterns using reverse mode.

The C++ source code corresponding to this operation is
\verbatim
	h = f.RevSparseHes(q, r, packed)
\endverbatim

\tparam Base
is the base type for this recording.

\tparam VectorBool
is a simple vector with elements of type bool.

\param q
is the value of \a q in the 
by the previous call of the form 
\verbatim
	f.ForSparseJac(q, r, packed)
\endverbatim
The value \c r in this call is a sparsity pattern for the matrix \f$ R \f$.

\param s
is a vector with size \c m that specifies the sparsity pattern
for the vector \f$ S \f$,
where \c m is the number of dependent variables
corresponding to the operation sequence stored in \a play. 

\return
is a vector with size \c q*q.
containing a sparsity pattern for the matrix
\f[
	H(x) = R^T ( S * F)^{(2)} (x) * R
\f]
where \f$ F \f$ is the function corresponding to the operation sequence
and \a x is any argument value.
*/

template <class Base>
template <class VectorBool>
VectorBool ADFun<Base>::RevSparseHes(size_t q,  const VectorBool &s)
{	size_t n = ind_taddr_.size();	
	VectorBool h( n * q );

	if( for_jac_sparse_pack_.n_set() > 0 )
	{	CPPAD_ASSERT_UNKNOWN( for_jac_sparse_set_.n_set() == 0 );
		// use vector_pack for the calculation
		CppAD::RevSparseHes( 
			q                 ,
			s                 ,
			h                 ,
			total_num_var_    ,
			dep_taddr_        ,
			ind_taddr_        ,
			play_             ,
			for_jac_sparse_pack_ 
		);
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( for_jac_sparse_pack_.n_set() == 0 );
		// use vector_pack for the calculation
		CppAD::RevSparseHes( 
			q                    ,
			s                    ,
			h                    ,
			total_num_var_       ,
			dep_taddr_           ,
			ind_taddr_           ,
			play_                ,
			for_jac_sparse_set_
		);
	}
	return h;
}

CPPAD_END_NAMESPACE
# endif
