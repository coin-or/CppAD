/* $Id$ */
# ifndef CPPAD_REV_SPARSE_JAC_INCLUDED
# define CPPAD_REV_SPARSE_JAC_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

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
$syntax%%r% = %F%.RevSparseJac(%p%, %s%, %packed%)%$$


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
	ADFun<%Base%> %f%
%$$

$head x$$
the sparsity pattern is valid for all values of the independent 
variables in $latex x \in B^n$$
(even if you use $cref/CondExp/$$ or $cref/VecAD/$$).

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
	const %VectorBool% &%s%
%$$
(see $xref/RevSparseJac/VectorBool/VectorBool/$$ below)
and its size is $latex p * m$$.
It specifies a 
$xref/glossary/Sparsity Pattern/sparsity pattern/$$ 
for the matrix $italic S$$ as follows:
for $latex i = 0 , \ldots , p-1$$ and $latex j = 0 , \ldots , m-1$$.
$latex \[
	S_{i,j} \neq 0 ; \Rightarrow \; s [ i * m + j ] = {\rm true}
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

$head r$$
The return value $italic r$$ has prototype
$syntax%
	%VectorBool% %r%
%$$
(see $xref/RevSparseJac/VectorBool/VectorBool/$$ below)
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
CPPAD_BEGIN_NAMESPACE

/*!
\file rev_sparse_jac.hpp
Reverse mode Jacobian sparsity patterns.
*/

/*!
Calculate Jacobian sparsity patterns using reverse mode.

The C++ source code corresponding to this operation is
\verbatim
	s = f.RevSparseJac(q, r, packed)
\endverbatim

\tparam Base
is the base type for this recording.

\tparam VectorBool
is a simple vector with elements of type bool.

\tparam VectorSet
is either \c vector_pack or \c vector_set. 

\param p
is the number of rows in the matrix \f$ S \f$.

\param s
is a sparsity pattern for the matrix \f$ S \f$.

\param r
the input value of \a r must be a vector with size \c p*n
where \c n is the number of independent variables
corresponding to the operation sequence stored in \a play. 
The input value of its elements does not matter.
On output, \a r is the sparsity pattern for the matrix
\f[
	J(x) = S * F^{(1)} (x)
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
*/

template <class Base, class VectorBool, class VectorSet> 
void ForSparseJac(
	size_t                 p                , 
	const VectorBool&      s                ,
	VectorBool&            r                ,
	size_t                 total_num_var    ,
	CppAD::vector<size_t>& dep_taddr        ,
	CppAD::vector<size_t>& ind_taddr        ,
	CppAD::player<Base>&   play             )
{
	// temporary indices
	size_t i, j;

	// check VectorBool is Simple Vector class with bool elements
	CheckSimpleVector<bool, VectorBool>();

	// range and domain dimensions for F
	size_t m = dep_taddr.size();
	size_t n = ind_taddr.size();

	CPPAD_ASSERT_KNOWN(
		p > 0,
		"RevSparseJac: p (first argument) is not greater than zero"
	);

	CPPAD_ASSERT_KNOWN(
		s.size() == p * m,
		"RevSparseJac: s (second argument) length is not equal to\n"
		"p (first argument) times range dimension for ADFun object."
	);

	// vector of sets that will hold the results
	VectorSet      var_sparsity;
	var_sparsity.resize(total_num_var, p);

	// The sparsity pattern corresponding to the dependent variables
	for(i = 0; i < m; i++)
	{	CPPAD_ASSERT_UNKNOWN( dep_taddr[i] < total_num_var );

		for(j = 0; j < p; j++) if( s[ i * m + j ] )
			var_sparsity.add_element( dep_taddr[i], j );
	}

	// evaluate the sparsity patterns
	RevJacSweep(
		n,
		total_num_var,
		&play,
		var_sparsity
	);

	// return values corresponding to dependent variables
	CPPAD_ASSERT_UNKNOWN( r.size() == p * n );
	for(j = 0; j < n; j++)
	{	CPPAD_ASSERT_UNKNOWN( ind_taddr[j] == (j+1) );

		// ind_taddr[j] is operator taddr for j-th independent variable
		CPPAD_ASSERT_UNKNOWN( play.GetOp( ind_taddr[j] ) == InvOp );

		// set bits 
		for(i = 0; i < p; i++) 
			r[ i * n + j ] = false;

		CPPAD_ASSERT_UNKNOWN( var_sparsity.end() == p );
		var_sparsity.begin(j+1);
		i = var_sparsity.next_element();
		while( i < p )
		{	r[ i * n + j ] = true;
			i              = var_sparsity.next_element();
		}
	}
}

/*!
User API for Jacobian sparsity patterns using reverse mode.

The C++ source code corresponding to this operation is
\verbatim
	s = f.RevSparseJac(q, r, packed)
\endverbatim

\tparam Base
is the base type for this recording.

\tparam VectorBool
is a simple vector with elements of type bool.

\param p
is the number of rows in the matrix \f$ S \f$.

\param s
is a sparsity pattern for the matrix \f$ S \f$.

\param packed
If \a packed is true, the type \c vector_pack is used for the calculations.
Otherwise the type \c vector_set is used for the calculations.

\return
the return value \c r is a vector with size \c p*n
where \c n is the number of independent variables
corresponding to the operation sequence stored in \c f. 
The value of \a r is the sparsity pattern for the matrix
\f[
	J(x) = S * F^{(1)} (x)
\f]
where \f$ F \f$ is the function corresponding to the operation sequence
and \a x is any argument value.
*/
template <class Base>
template <class VectorBool>
VectorBool ADFun<Base>::RevSparseJac(
	size_t              p      , 
	const VectorBool&   s      ,
	bool                packed ) 
{	size_t n = ind_taddr_.size();
	VectorBool r( p * n );

	if( packed )
	{	CppAD::ForSparseJac<Base, VectorBool, vector_pack>(
			p                , 
			s                ,
			r                ,
			total_num_var_   ,
			dep_taddr_       ,
			ind_taddr_       ,
			play_
		);
	}
	else
	{	CppAD::ForSparseJac<Base, VectorBool, vector_set>(
			p                , 
			s                ,
			r                ,
			total_num_var_   ,
			dep_taddr_       ,
			ind_taddr_       ,
			play_
		);
	}
	return r;
}

CPPAD_END_NAMESPACE
# endif
