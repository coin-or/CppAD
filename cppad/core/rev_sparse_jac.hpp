# ifndef CPPAD_CORE_REV_SPARSE_JAC_HPP
# define CPPAD_CORE_REV_SPARSE_JAC_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
===============================================================================
$begin rev_sparse_jac$$
$spell
	Jacobian
	jac
	bool
	const
	rc
	cpp
$$

$section Reverse Mode Spare Jacobian Patterns$$

$head Syntax$$
$icode%f%.rev_sparse_jac(
	%pattern_in%, %transpose%, %dependency%, %internal_bool%, %pattern_out%
)%$$

$head Purpose$$
We use $latex F : B^n \rightarrow B^m$$ to denote the
$cref/AD function/glossary/AD Function/$$ corresponding to
the operation sequence stored in $icode f$$.
Fix $latex R \in \B{R}^{\ell \times m}$$ and define the function
$latex \[
	S(x) = R * F^{(1)} ( x )
\] $$
Given the $cref/sparsity pattern/glossary/Sparsity Pattern/$$ for $latex R$$,
$code rev_sparse_jac$$ computes a sparsity pattern for $latex S(x)$$.

$head x$$
Note that the sparsity pattern $latex S(x)$$ corresponds to the
operation sequence stored in $icode f$$ and does not depend on
the argument $icode x$$.
(The operation sequence may contain
$cref CondExp$$ and  $cref VecAD$$ operations.)

$head SizeVector$$
The type $icode SizeVector$$ is a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$
$code size_t$$.

$head f$$
The object $icode f$$ has prototype
$codei%
	ADFun<%Base%> %f%
%$$
The $cref ADFun$$ object $icode f$$ is not $code const$$.
After a call to $code rev_sparse_jac$$, a sparsity pattern
for each of the variables in the operation sequence
is held in $icode f$$ for possible later use during
reverse Hessian sparsity calculations.

$head pattern_in$$
The argument $icode pattern_in$$ has prototype
$codei%
	const sparse_rc<%SizeVector%>& %pattern_in%
%$$
see $cref sparse_rc$$.
If $icode transpose$$ it is false (true),
$icode pattern_in$$ is a sparsity pattern for $latex R$$ ($latex R^\R{T}$$).

$head transpose$$
This argument has prototype
$codei%
	bool %transpose%
%$$
See $cref/pattern_in/rev_sparse_jac/pattern_in/$$ above and
$cref/pattern_out/rev_sparse_jac/pattern_out/$$ below.

$head dependency$$
This argument has prototype
$codei%
	bool %dependency%
%$$
It index pairs show dependency, instead of non-zero Jacobian value;
see $icode dependency$$ is true,
the $cref/dependency pattern/dependency.cpp/Dependency Pattern/$$
(instead of sparsity pattern) is computed.

$head internal_bool$$
If this is true, calculations are done with sets represented by a vector
of boolean values. Otherwise, a vector of standard sets is used.

$head pattern_out$$
This argument has prototype
$codei%
	sparse_rc<%SizeVector%>& %pattern_out%
%$$
This input value of $icode pattern_out$$ does not matter.
If $icode transpose$$ it is false (true),
upon return $icode pattern_out$$ is a sparsity pattern for
$latex S(x)$$ ($latex S(x)^\R{T}$$).

$head Sparsity for Entire Jacobian$$
Suppose that
$latex R$$ is the $latex n \times n$$ identity matrix.
In this case, $icode pattern_out$$ is a sparsity pattern for
$latex F^{(1)} ( x )$$  ( $latex F^{(1)} (x)^\R{T}$$ )
if $icode transpose$$ is false (true).

$head Example$$
$comment%
	example/sparse/rev_sparse_jac.cpp
%$$
The file
$cref rev_sparse_jac.cpp$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/
# include <cppad/core/ad_fun.hpp>
# include <cppad/local/sparse_internal.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*!
Reverse Jacobian sparsity patterns.

\tparam Base
is the base type for this recording.

\tparam SizeVector
is the simple vector with elements of type size_t that is used for
row, column index sparsity patterns.

\param pattern_in
is the sparsity pattern for for R or R^T depending on transpose.

\param transpose
Is the input and returned sparsity pattern transposed.

\param dependency
Are the derivatives with respect to left and right of the expression below
considered to be non-zero:
\code
	CondExpRel(left, right, if_true, if_false)
\endcode
This is used by the optimizer to obtain the correct dependency relations.

\param internal_bool
If this is true, calculations are done with sets represented by a vector
of boolean values. Otherwise, a vector of standard sets is used.

\param pattern_out
The value of transpose is false (true),
the return value is a sparsity pattern for S(x) ( S(x)^T ) where
\f[
	S(x) = R * F^{(1)} (x)
\f]
Here F is the function corresponding to the operation sequence
and x is any argument value.
*/
template <class Base>
template <class SizeVector>
void ADFun<Base>::rev_sparse_jac(
	const sparse_rc<SizeVector>& pattern_in       ,
	bool                         transpose        ,
	bool                         dependency       ,
	bool                         internal_bool    ,
	sparse_rc<SizeVector>&       pattern_out      )
{	// number or rows, columns, and non-zeros in pattern_in
	size_t nr_in  = pattern_in.nr();
	size_t nc_in  = pattern_in.nc();
	//
	size_t ell = nr_in;
	size_t m   = nc_in;
	if( transpose )
		std::swap(ell, m);
	//
	CPPAD_ASSERT_KNOWN(
		m == Range() ,
		"rev_sparse_jac: number columns in R "
		"is not equal number of dependent variables."
	);
	// number of independent variables
	size_t n = Domain();
	//
	if( internal_bool )
	{	// allocate memory for bool sparsity calculation
		// (sparsity pattern is emtpy after a resize)
		local::sparse_pack var_sparsity;
		var_sparsity.resize(num_var_tape_, ell);
		//
		// set sparsity patttern for dependent variables
		local::set_internal_sparsity(
			! transpose           ,
			dep_taddr_            ,
			pattern_in            ,
			var_sparsity
		);

		// compute sparsity for other variables
		local::RevJacSweep(
			dependency,
			n,
			num_var_tape_,
			&play_,
			var_sparsity
		);
		// get sparstiy pattern for independent variables
		local::get_internal_sparsity(
			! transpose, ind_taddr_, var_sparsity, pattern_out
		);
	}
	else
	{	// allocate memory for bool sparsity calculation
		// (sparsity pattern is emtpy after a resize)
		local::sparse_list var_sparsity;
		var_sparsity.resize(num_var_tape_, ell);
		//
		// set sparsity patttern for dependent variables
		local::set_internal_sparsity(
			! transpose           ,
			dep_taddr_            ,
			pattern_in            ,
			var_sparsity
		);

		// compute sparsity for other variables
		local::RevJacSweep(
			dependency,
			n,
			num_var_tape_,
			&play_,
			var_sparsity
		);
		// get sparstiy pattern for independent variables
		local::get_internal_sparsity(
			! transpose, ind_taddr_, var_sparsity, pattern_out
		);
	}
	return;
}
} // END_CPPAD_NAMESPACE
// ============================================================================

/*
$begin RevSparseJac$$
$spell
	optimizer
	nz
	CondExpRel
	std
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
$mindex RevSparseJac sparse$$

$head Syntax$$
$icode%s% = %f%.RevSparseJac(%q%, %r%)
%$$
$icode%s% = %f%.RevSparseJac(%q%, %r%, %transpose%, %dependency%)%$$

$head Purpose$$
We use $latex F : B^n \rightarrow B^m$$ to denote the
$cref/AD function/glossary/AD Function/$$ corresponding to $icode f$$.
For a fixed matrix $latex R \in B^{q \times m}$$,
the Jacobian of $latex R * F( x )$$
with respect to $latex x$$ is
$latex \[
	S(x) = R * F^{(1)} ( x )
\] $$
Given a
$cref/sparsity pattern/glossary/Sparsity Pattern/$$
for $latex R$$,
$code RevSparseJac$$ returns a sparsity pattern for the $latex S(x)$$.

$head f$$
The object $icode f$$ has prototype
$codei%
	ADFun<%Base%> %f%
%$$

$head x$$
If the operation sequence in $icode f$$ is
$cref/independent/glossary/Operation/Independent/$$ of
the independent variables in $latex x \in B^n$$,
the sparsity pattern is valid for all values of
(even if it has $cref CondExp$$ or $cref VecAD$$ operations).

$head q$$
The argument $icode q$$ has prototype
$codei%
	size_t %q%
%$$
It specifies the number of rows in
$latex R \in B^{q \times m}$$ and the
Jacobian $latex S(x) \in B^{q \times n}$$.

$head transpose$$
The argument $icode transpose$$ has prototype
$codei%
	bool %transpose%
%$$
The default value $code false$$ is used when $icode transpose$$ is not present.

$head dependency$$
The argument $icode dependency$$ has prototype
$codei%
	bool %dependency%
%$$
If $icode dependency$$ is true,
the $cref/dependency pattern/dependency.cpp/Dependency Pattern/$$
(instead of sparsity pattern) is computed.

$head r$$
The argument $icode s$$ has prototype
$codei%
	const %VectorSet%& %r%
%$$
see $cref/VectorSet/RevSparseJac/VectorSet/$$ below.

$subhead transpose false$$
If $icode r$$ has elements of type $code bool$$,
its size is $latex q * m$$.
If it has elements of type $code std::set<size_t>$$,
its size is $icode q$$ and all its set elements are between
zero and $latex m - 1$$.
It specifies a
$cref/sparsity pattern/glossary/Sparsity Pattern/$$
for the matrix $latex R \in B^{q \times m}$$.

$subhead transpose true$$
If $icode r$$ has elements of type $code bool$$,
its size is $latex m * q$$.
If it has elements of type $code std::set<size_t>$$,
its size is $icode m$$ and all its set elements are between
zero and $latex q - 1$$.
It specifies a
$cref/sparsity pattern/glossary/Sparsity Pattern/$$
for the matrix $latex R^\R{T} \in B^{m \times q}$$.

$head s$$
The return value $icode s$$ has prototype
$codei%
	%VectorSet% %s%
%$$
see $cref/VectorSet/RevSparseJac/VectorSet/$$ below.

$subhead transpose false$$
If it has elements of type $code bool$$,
its size is $latex q * n$$.
If it has elements of type $code std::set<size_t>$$,
its size is $icode q$$ and all its set elements are between
zero and $latex n - 1$$.
It specifies a
$cref/sparsity pattern/glossary/Sparsity Pattern/$$
for the matrix $latex S(x) \in {q \times n}$$.

$subhead transpose true$$
If it has elements of type $code bool$$,
its size is $latex n * q$$.
If it has elements of type $code std::set<size_t>$$,
its size is $icode n$$ and all its set elements are between
zero and $latex q - 1$$.
It specifies a
$cref/sparsity pattern/glossary/Sparsity Pattern/$$
for the matrix $latex S(x)^\R{T} \in {n \times q}$$.

$head VectorSet$$
The type $icode VectorSet$$ must be a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$
$code bool$$ or $code std::set<size_t>$$;
see $cref/sparsity pattern/glossary/Sparsity Pattern/$$ for a discussion
of the difference.

$head Entire Sparsity Pattern$$
Suppose that $latex q = m$$ and
$latex R$$ is the $latex m \times m$$ identity matrix.
In this case,
the corresponding value for $icode s$$ is a
sparsity pattern for the Jacobian $latex S(x) = F^{(1)} ( x )$$.

$head Example$$
$children%
	example/sparse/rev_sparse_jac.cpp
%$$
The file
$cref rev_sparse_jac.cpp$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/

# include <cppad/local/std_set.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file rev_sparse_jac.hpp
Reverse mode Jacobian sparsity patterns.
*/
// =========================================================================
// RevSparseJacCase

/*!
Private helper function for RevSparseJac(q, r, transpose) boolean sparsity.

All of the description in the public member function
\c RevSparseJac(q, r, transpose) apply.

\param set_type
is a \c bool value.
This argument is used to dispatch to the proper source code
depending on the value of \c VectorSet::value_type.

\param transpose
See \c RevSparseJac(q, r, transpose, dependency)

\param dependency
See \c RevSparseJac(q, r, transpose, dependency)

\param q
See \c RevSparseJac(q, r, transpose, dependency)

\param r
See \c RevSparseJac(q, r, transpose, dependency)

\param s
is the return value for the corresponding call to
RevSparseJac(q, r, transpose).
*/

template <class Base>
template <class VectorSet>
void ADFun<Base>::RevSparseJacCase(
	bool                set_type          ,
	bool                transpose         ,
	bool                dependency        ,
	size_t              q                 ,
	const VectorSet&    r                 ,
	VectorSet&          s                 )
{	size_t n = Domain();
	size_t m = Range();

	// dimension of the result vector
	s.resize( q * n );

	// temporary indices
	size_t i, j;

	// check VectorSet is Simple Vector class with bool elements
	CheckSimpleVector<bool, VectorSet>();
	//
	CPPAD_ASSERT_KNOWN(
		q > 0,
		"RevSparseJac: q is not greater than zero"
	);
	CPPAD_ASSERT_KNOWN(
		size_t(r.size()) == q * m,
		"RevSparseJac: size of r is not equal to\n"
		"q times range dimension for ADFun object."
	);
	//
	// vector of sets that will hold the results
	local::sparse_pack    var_sparsity;
	var_sparsity.resize(num_var_tape_, q);

	// The sparsity pattern corresponding to the dependent variables
	for(i = 0; i < m; i++)
	{	CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < num_var_tape_ );
		if( transpose )
		{	for(j = 0; j < q; j++) if( r[ i * q + j ] )
				var_sparsity.add_element( dep_taddr_[i], j );
		}
		else
		{	for(j = 0; j < q; j++) if( r[ j * m + i ] )
				var_sparsity.add_element( dep_taddr_[i], j );
		}
	}

	// evaluate the sparsity patterns
	local::RevJacSweep(
		dependency,
		n,
		num_var_tape_,
		&play_,
		var_sparsity
	);

	// return values corresponding to dependent variables
	CPPAD_ASSERT_UNKNOWN( size_t(s.size()) == q * n );
	for(j = 0; j < n; j++)
	{	CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] == (j+1) );

		// ind_taddr_[j] is operator taddr for j-th independent variable
		CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[j] ) == local::InvOp );

		// extract the result from var_sparsity
		if( transpose )
		{	for(i = 0; i < q; i++)
				s[ j * q + i ] = false;
		}
		else
		{	for(i = 0; i < q; i++)
				s[ i * n + j ] = false;
		}
		CPPAD_ASSERT_UNKNOWN( var_sparsity.end() == q );
		local::sparse_pack::const_iterator itr(var_sparsity, j+1);
		i = *itr;
		while( i < q )
		{	if( transpose )
				s[ j * q + i ] = true;
			else	s[ i * n + j ] = true;
			i  = *(++itr);
		}
	}
}

/*!
Private helper function for RevSparseJac(q, r, transpose) set sparsity

All of the description in the public member function
\c RevSparseJac(q, r, transpose) apply.

\param set_type
is a \c std::set<size_t> object.
This argument is used to dispatch to the proper source code
depending on the value of \c VectorSet::value_type.

\param transpose
See \c RevSparseJac(q, r, transpose, dependency)

\param dependency
See \c RevSparseJac(q, r, transpose, dependency)

\param q
See \c RevSparseJac(q, r, transpose, dependency)

\param r
See \c RevSparseJac(q, r, transpose, dependency)

\param s
is the return value for the corresponding call to RevSparseJac(q, r, transpose)
*/

template <class Base>
template <class VectorSet>
void ADFun<Base>::RevSparseJacCase(
	const std::set<size_t>&      set_type          ,
	bool                         transpose         ,
	bool                         dependency        ,
	size_t                       q                 ,
	const VectorSet&             r                 ,
	VectorSet&                   s                 )
{	// dimension of the result vector
	if( transpose )
		s.resize( Domain() );
	else	s.resize( q );

	// temporary indices
	size_t i, j;
	std::set<size_t>::const_iterator itr_1;

	// check VectorSet is Simple Vector class with sets for elements
	CheckSimpleVector<std::set<size_t>, VectorSet>(
		local::one_element_std_set<size_t>(), local::two_element_std_set<size_t>()
	);

	// domain dimensions for F
	size_t n = ind_taddr_.size();
	size_t m = dep_taddr_.size();

	CPPAD_ASSERT_KNOWN(
		q > 0,
		"RevSparseJac: q is not greater than zero"
	);
	CPPAD_ASSERT_KNOWN(
		size_t(r.size()) == q || transpose,
		"RevSparseJac: size of r is not equal to q and transpose is false."
	);
	CPPAD_ASSERT_KNOWN(
		size_t(r.size()) == m || ! transpose,
		"RevSparseJac: size of r is not equal to m and transpose is true."
	);

	// vector of lists that will hold the results
	local::sparse_list           var_sparsity;
	var_sparsity.resize(num_var_tape_, q);

	// The sparsity pattern corresponding to the dependent variables
	if( transpose )
	{	for(i = 0; i < m; i++)
		{	itr_1 = r[i].begin();
			while(itr_1 != r[i].end())
			{	j = *itr_1++;
				CPPAD_ASSERT_KNOWN(
				j < q,
				"RevSparseJac: transpose is true and element of the set\n"
				"r[i] has value greater than or equal q."
				);
				CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < num_var_tape_ );
				var_sparsity.add_element( dep_taddr_[i], j );
			}
		}
	}
	else
	{	for(i = 0; i < q; i++)
		{	itr_1 = r[i].begin();
			while(itr_1 != r[i].end())
			{	j = *itr_1++;
				CPPAD_ASSERT_KNOWN(
				j < m,
				"RevSparseJac: transpose is false and element of the set\n"
				"r[i] has value greater than or equal range dimension."
				);
				CPPAD_ASSERT_UNKNOWN( dep_taddr_[j] < num_var_tape_ );
				var_sparsity.add_element( dep_taddr_[j], i );
			}
		}
	}
	// evaluate the sparsity patterns
	local::RevJacSweep(
		dependency,
		n,
		num_var_tape_,
		&play_,
		var_sparsity
	);

	// return values corresponding to dependent variables
	CPPAD_ASSERT_UNKNOWN( size_t(s.size()) == q || transpose );
	CPPAD_ASSERT_UNKNOWN( size_t(s.size()) == n || ! transpose );
	for(j = 0; j < n; j++)
	{	CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] == (j+1) );

		// ind_taddr_[j] is operator taddr for j-th independent variable
		CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[j] ) == local::InvOp );

		CPPAD_ASSERT_UNKNOWN( var_sparsity.end() == q );
		local::sparse_list::const_iterator itr_2(var_sparsity, j+1);
		i = *itr_2;
		while( i < q )
		{	if( transpose )
				s[j].insert(i);
			else	s[i].insert(j);
			i = *(++itr_2);
		}
	}
}

// =========================================================================
// RevSparseJac
/*!
User API for Jacobian sparsity patterns using reverse mode.

The C++ source code corresponding to this operation is
\verbatim
	s = f.RevSparseJac(q, r, transpose, dependency)
\endverbatim

\tparam Base
is the base type for this recording.

\tparam VectorSet
is a simple vector with elements of type \c bool.
or \c std::set<size_t>.

\param q
is the number of rows in the matrix \f$ R \f$.

\param r
is a sparsity pattern for the matrix \f$ R \f$.

\param transpose
are the sparsity patterns for \f$ R \f$ and \f$ S(x) \f$ transposed.

\param dependency
Are the derivatives with respect to left and right of the expression below
considered to be non-zero:
\code
	CondExpRel(left, right, if_true, if_false)
\endcode
This is used by the optimizer to obtain the correct dependency relations.


\return
If \c transpose is false (true), the return value is a sparsity pattern
for \f$ S(x) \f$ (\f$ S(x)^T \f$) where
\f[
	S(x) = R * F^{(1)} (x)
\f]
and \f$ F \f$ is the function corresponding to the operation sequence
and \a x is any argument value.
If \c VectorSet::value_type is \c bool,
the return value has size \f$ q * n \f$ ( \f$ n * q \f$).
If \c VectorSet::value_type is \c std::set<size_t>,
the return value has size \f$ q \f$ ( \f$ n \f$)
and with all its elements between zero and \f$ n - 1 \f$ (\f$ q - 1 \f$).
*/
template <class Base>
template <class VectorSet>
VectorSet ADFun<Base>::RevSparseJac(
	size_t              q          ,
	const VectorSet&    r          ,
	bool                transpose  ,
	bool                dependency )
{
	VectorSet s;
	typedef typename VectorSet::value_type Set_type;

	RevSparseJacCase(
		Set_type()    ,
		transpose     ,
		dependency    ,
		q             ,
		r             ,
		s
	);
	return s;
}
// ===========================================================================
// RevSparseJacCheckpoint
/*!
Reverse mode Jacobian sparsity calculation used by checkpoint functions.

\tparam Base
is the base type for this recording.

\param transpose
is true (false) s is equal to \f$ S(x) \f$ (\f$ S(x)^T \f$)
where
\f[
	S(x) = R * F^{(1)} (x)
\f]
where \f$ F \f$ is the function corresponding to the operation sequence
and \f$ x \f$ is any argument value.

\param q
is the number of rows in the matrix \f$ R \f$.

\param r
is a sparsity pattern for the matrix \f$ R \f$.

\param transpose
are the sparsity patterns for \f$ R \f$ and \f$ S(x) \f$ transposed.

\param dependency
Are the derivatives with respect to left and right of the expression below
considered to be non-zero:
\code
	CondExpRel(left, right, if_true, if_false)
\endcode
This is used by the optimizer to obtain the correct dependency relations.

\param s
The input size and elements of s do not matter.
On output, s is the sparsity pattern for the matrix \f$ S(x) \f$
or \f$ S(x)^T \f$ depending on transpose.

*/
template <class Base>
void ADFun<Base>::RevSparseJacCheckpoint(
	size_t                        q          ,
	const local::sparse_list&     r          ,
	bool                          transpose  ,
	bool                          dependency ,
	local::sparse_list&                  s          )
{	size_t n = Domain();
	size_t m = Range();

# ifndef NDEBUG
	if( transpose )
	{	CPPAD_ASSERT_UNKNOWN( r.n_set() == m );
		CPPAD_ASSERT_UNKNOWN( r.end()   == q );
	}
	else
	{	CPPAD_ASSERT_UNKNOWN( r.n_set() == q );
		CPPAD_ASSERT_UNKNOWN( r.end()   == m );
	}
	for(size_t i = 0; i < m; i++)
		CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < num_var_tape_ );
# endif

	// holds reverse Jacobian sparsity pattern for all variables
	local::sparse_list var_sparsity;
	var_sparsity.resize(num_var_tape_, q);

	// set sparsity pattern for dependent variables
	if( transpose )
	{	for(size_t i = 0; i < m; i++)
		{	local::sparse_list::const_iterator itr(r, i);
			size_t j = *itr;
			while( j < q )
			{	var_sparsity.add_element( dep_taddr_[i], j );
				j = *(++itr);
			}
		}
	}
	else
	{	for(size_t j = 0; j < q; j++)
		{	local::sparse_list::const_iterator itr(r, j);
			size_t i = *itr;
			while( i < m )
			{	var_sparsity.add_element( dep_taddr_[i], j );
				i = *(++itr);
			}
		}
	}

	// evaluate the sparsity pattern for all variables
	local::RevJacSweep(
		dependency,
		n,
		num_var_tape_,
		&play_,
		var_sparsity
	);

	// dimension the return value
	if( transpose )
		s.resize(n, m);
	else
		s.resize(m, n);

	// return values corresponding to independent variables
	for(size_t j = 0; j < n; j++)
	{	CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] == (j+1) );

		// ind_taddr_[j] is operator taddr for j-th independent variable
		CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[j] ) == local::InvOp );

		// extract the result from var_sparsity
		CPPAD_ASSERT_UNKNOWN( var_sparsity.end() == q );
		local::sparse_list::const_iterator itr(var_sparsity, j+1);
		size_t i = *itr;
		while( i < q )
		{	if( transpose )
				s.add_element(j, i);
			else
				s.add_element(i, j);
			i  = *(++itr);
		}
	}

}

} // END_CPPAD_NAMESPACE
# endif
