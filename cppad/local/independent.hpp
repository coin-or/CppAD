# ifndef CPPAD_LOCAL_INDEPENDENT_HPP
# define CPPAD_LOCAL_INDEPENDENT_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
namespace CppAD { namespace local { //  BEGIN_CPPAD_LOCAL_NAMESPACE
/*
\file local/independent.hpp
Start recording AD<Base> operations.
*/

/*!
Start recording AD<Base> operations: Implementation in local namespace.

\tparam VectorAD
This is simple vector type with elements of type AD<Base>.

\param x
Vector of the independent variables.

\param abort_op_index
operator index at which execution will be aborted (during  the recording

\param record_compare
should comparison operators be recorded.
of operations). The value zero corresponds to not aborting (will not match).

\param dynamic
Vector of dynamic parameters.
*/
template <typename Base>
template <typename VectorAD>
void ADTape<Base>::Independent(
	VectorAD&    x               ,
	size_t       abort_op_index  ,
	bool         record_compare  ,
	VectorAD&    dynamic
) {
	// check VectorAD is Simple Vector class with AD<Base> elements
	CheckSimpleVector< AD<Base>, VectorAD>();

	// dimension of the domain space
	size_t n = x.size();
	CPPAD_ASSERT_KNOWN(
		n > 0,
		"Indepdendent: the argument vector x has zero size"
	);
	CPPAD_ASSERT_UNKNOWN( Rec_.num_var_rec() == 0 );
	CPPAD_ASSERT_UNKNOWN( Rec_.get_abort_op_index() == 0 );
	CPPAD_ASSERT_UNKNOWN( Rec_.get_record_compare() == true );
	CPPAD_ASSERT_UNKNOWN( Rec_.get_num_dynamic()    == 0 );

	// set record_compare and abort_op_index before doing anything else
	Rec_.set_record_compare(record_compare);
	Rec_.set_abort_op_index(abort_op_index);
	Rec_.set_num_dynamic( dynamic.size() );

	// mark the beginning of the tape and skip the first variable index
	// (zero) because parameters use taddr zero
	CPPAD_ASSERT_NARG_NRES(BeginOp, 1, 1);
	Rec_.PutOp(BeginOp);
	Rec_.PutArg(0);

	// place each of the independent variables in the tape
	CPPAD_ASSERT_NARG_NRES(InvOp, 0, 1);
	for(size_t j = 0; j < n; j++)
	{	// tape address for this independent variable
		CPPAD_ASSERT_UNKNOWN( ! Variable(x[j] ) );
		x[j].taddr_     = Rec_.PutOp(InvOp);
		x[j].tape_id_   = id_;
		x[j].dynamic_   = false;
		CPPAD_ASSERT_UNKNOWN( size_t(x[j].taddr_) == j+1 );
		CPPAD_ASSERT_UNKNOWN( Variable(x[j] ) );
	}

	// done specifying all of the independent variables
	size_independent_ = n;

	// Place the dynamic parameters at the beginning of the parameter vector
	for(size_t j = 0; j < Rec_.get_num_dynamic(); ++j)
	{	CPPAD_ASSERT_UNKNOWN( ! Dynamic( dynamic[j] ) );
		CPPAD_ASSERT_UNKNOWN( Parameter( dynamic[j] ) );
# ifndef NDEBUG
		// dynamic parameters can not match previous parameters so i == j
		size_t i = Rec_.PutPar( dynamic[j].value_ );
		CPPAD_ASSERT_UNKNOWN(i == j );
# else
		Rec_.PutPar( dynamic[j].value_ );
# endif
		dynamic[j].taddr_   = static_cast<addr_t>(j);
		dynamic[j].tape_id_ = id_;
		dynamic[j].dynamic_ = true;
		CPPAD_ASSERT_UNKNOWN( Dynamic( dynamic[j] ) );
	}
}
} } // END_CPPAD_LOCAL_NAMESPACE

# endif
