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
$pre
$$
$syntax%%s% = %f%.ForSparseJac(%q%, %r%, %packed%)%$$

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
	const %VectorBool% &%r%
%$$
(see $xref/ForSparseJac/VectorBool/VectorBool/$$ below)
and its size is $latex n * q$$.
It specifies a 
$xref/glossary/Sparsity Pattern/sparsity pattern/$$ 
for the matrix $italic R$$ as follows:
for $latex i = 0 , \ldots , n-1$$ and $latex j = 0 , \ldots , q-1$$.
$latex \[
	R_{i,j} \neq 0 ; \Rightarrow \; r [ i * q + j ] = {\rm true}
\] $$

$head packed$$
If $italic packed$$ is true,
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

$head s$$
The return value $italic s$$ has prototype
$syntax%
	%VectorBool% %s%
%$$
(see $xref/ForSparseJac/VectorBool/VectorBool/$$ below)
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
CPPAD_BEGIN_NAMESPACE

/*!
\file for_sparse_jac.hpp
Forward mode Jacobian sparsity patterns.
*/

/*!
Calculate Jacobian sparsity patterns using forward mode.

The C++ source code corresponding to this operation is
\verbatim
	s = f.ForSparseJac(q, r, packed)
\endverbatim

\tparam Base
is the base type for this recording.

\tparam VectorBool
is a simple vector with elements of type bool.

\tparam VectorSet
is either \c vector_pack or \c vector_set. 

\param q
is the number of columns in the matrix \f$ R \f$.

\param r
is a sparsity pattern for the matrix \f$ R \f$.

\param s
the input value of \a s must be a vector with size \c m*q
where \c m is the number of dependent variables
corresponding to the operation sequence stored in \a play. 
The input value of its elements does not matter.
On output, the \a s is the sparsity pattern for the matrix
\f[
	J(x) = F^{(1)} (x) * R
\f]
where \f$ F \f$ is the function corresponding to the operation sequence
and \a x is any argument value.

\param total_num_var
is the total number of variable in this recording.

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
the input value of \a for_jac_sparsity does not matter.
On output, \a for_jac_sparsity.n_set() == \a total_num_var
and \a for_jac_sparsity.end() == \a q.
It contains the forward sparsity pattern for all of the variables on the
tape (given the sparsity pattern for the independent variables is \f$ R \f$).
*/
template <class Base, class VectorBool, class VectorSet> 
void ForSparseJac(
	size_t                 q                , 
	const VectorBool&      r                ,
	VectorBool&            s                ,
	size_t                 total_num_var    ,
	CppAD::vector<size_t>& dep_taddr        ,
	CppAD::vector<size_t>& ind_taddr        ,
	CppAD::player<Base>&   play             ,
	VectorSet&             for_jac_sparsity )
{
	// temporary indices
	size_t i, j;

	// check VectorBool is Simple Vector class with bool elements
	CheckSimpleVector<bool, VectorBool>();

	// range and domain dimensions for F
	size_t m = dep_taddr.size();
	size_t n = ind_taddr.size();

	CPPAD_ASSERT_KNOWN(
		q > 0,
		"ForSparseJac: q (first arugment) is not greater than zero"
	);

	CPPAD_ASSERT_KNOWN(
		r.size() == n * q,
		"ForSparseJac: r (second argument) length is not equal to\n"
		"q (first argument) times domain dimension for ADFun object."
	);

	// allocate memory for the requested sparsity calculation
	for_jac_sparsity.resize(total_num_var, q);

	// set values corresponding to independent variables
	for(i = 0; i < n; i++)
	{	CPPAD_ASSERT_UNKNOWN( ind_taddr[i] < total_num_var );
		// ind_taddr[i] is operator taddr for i-th independent variable
		CPPAD_ASSERT_UNKNOWN( play.GetOp( ind_taddr[i] ) == InvOp );

		// set bits that are true
		for(j = 0; j < q; j++) if( r[ i * q + j ] )
			for_jac_sparsity.add_element( ind_taddr[i], j);
	}

	// evaluate the sparsity patterns
	ForJacSweep(
		n,
		total_num_var,
		&play,
		for_jac_sparsity
	);

	// return values corresponding to dependent variables
	CPPAD_ASSERT_UNKNOWN( s.size() == m * q );
	for(i = 0; i < m; i++)
	{	CPPAD_ASSERT_UNKNOWN( dep_taddr[i] < total_num_var );

		// set bits 
		for(j = 0; j < q; j++)
			s[ i * q + j ] = false;
		CPPAD_ASSERT_UNKNOWN( for_jac_sparsity.end() == q );
		for_jac_sparsity.begin( dep_taddr[i] );
		j = for_jac_sparsity.next_element();
		while( j < q )
		{	s[i * q + j ] = true;
			j = for_jac_sparsity.next_element();
		}
	}
}


/*!
User API for Jacobian sparsity patterns using forward mode.

The C++ source code corresponding to this operation is
\verbatim
	s = f.ForSparseJac(q, r, packed)
\endverbatim

\tparam Base
is the base type for this recording.

\tparam VectorBool
is a simple vector with elements of type bool.

\param q
is the number of columns in the matrix \f$ R \f$.

\param r
is a sparsity pattern for the matrix \f$ R \f$.

\param packed
If \a packed is true, the type \c vector_pack is used for the calculations.
Otherwise the type \c vector_set is used for the calculations.

\return
the return value \c s is a vector with size \c m*q
where \c m is the number of dependent variables
corresponding to the operation sequence stored in \c f. 
The value of \a s is the sparsity pattern for the matrix
\f[
	J(x) = F^{(1)} (x) * R
\f]
where \f$ F \f$ is the function corresponding to the operation sequence
and \a x is any argument value.

\par Side Effects
If \a packed is true,
the forward sparsity pattern for all of the variables on the
tape is stored in \c for_jac_sparse_pack__.
In this case 
\verbatim
	for_jac_sparse_pack_.n_set() == total_num_var_
	for_jac_sparse_pack_.end() == q
	for_jac_sparse_set_.n_set()  == 0
	for_jac_sparse_set_.end()  == 0
\endverbatim
\n
\n
If \a packed is false,
the forward sparsity pattern for all of the variables on the
tape is stored in \c for_jac_sparse_set__.
In this case 
\verbatim
	for_jac_sparse_set_.n_set()   == total_num_var_
	for_jac_sparse_set_.end()   == q
	for_jac_sparse_pack_.n_set()  == 0
	for_jac_sparse_pack_.end()  == 0
\endverbatim
*/
template <class Base>
template <class VectorBool>
VectorBool ADFun<Base>::ForSparseJac(
	size_t             q             , 
	const VectorBool&  r             ,
	bool               packed        )
{	size_t m = dep_taddr_.size();
	VectorBool s( m * q );

	if( packed )
	{	// free any results stored in for_jac_sparse_set_	
		for_jac_sparse_set_.resize(0, 0);
		// store results in for_jac_sparse_pack_
		CppAD::ForSparseJac(
			q                , 
			r                ,
			s                ,
			total_num_var_   ,
			dep_taddr_       ,
			ind_taddr_       ,
			play_            ,
			for_jac_sparse_pack_ 
		);
	}
	else
	{	// free any results stored in for_jac_sparse_pack_
		for_jac_sparse_pack_.resize(0, 0);
		// store results in for_jac_sparse_set_
		CppAD::ForSparseJac(
			q                , 
			r                ,
			s                ,
			total_num_var_   ,
			dep_taddr_       ,
			ind_taddr_       ,
			play_            ,
			for_jac_sparse_set_
		);
	}
	return s;
}

CPPAD_END_NAMESPACE
# endif
