/* $Id$ */
# ifndef CPPAD_STORE_OP_INCLUDED
# define CPPAD_STORE_OP_INCLUDED
CPPAD_BEGIN_NAMESPACE

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*!
\file store_op.hpp
Changing the current value of a VecAD element.
*/

/*!
Zero order forward mode implementation of op = StppOp.

\copydetails forward_store_op_0
*/
template <class Base>
inline void forward_store_pp_op_0(
	size_t         i_z         ,
	const size_t*  arg         , 
	size_t         num_par     ,
	size_t         nc_taylor   ,
	Base*          taylor      ,
	size_t         nc_combined ,
	bool*          variable    ,
	size_t*        combined    )
{	size_t i_vec = arg[1];

	// Because the index is a parameter, this indexing error should be 
	// caught and reported to the user when the tape is recording.
	CPPAD_ASSERT_UNKNOWN( i_vec < combined[ arg[0] - 1 ] );

	CPPAD_ASSERT_UNKNOWN( variable != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( combined != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( NumArg(StppOp) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(StppOp) == 0 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( arg[0] + i_vec < nc_combined );
	CPPAD_ASSERT_UNKNOWN( arg[2] < num_par );

	variable[ arg[0] + i_vec ] = false;
	combined[ arg[0] + i_vec ] = arg[2];
}

/*!
Zero order forward mode implementation of op = StpvOp.

\copydetails forward_store_op_0
*/
template <class Base>
inline void forward_store_pv_op_0(
	size_t         i_z         ,
	const size_t*  arg         , 
	size_t         num_par     ,
	size_t         nc_taylor   ,
	Base*          taylor      ,
	size_t         nc_combined ,
	bool*          variable    ,
	size_t*        combined    )
{	size_t i_vec = arg[1];

	// Because the index is a parameter, this indexing error should be 
	// caught and reported to the user when the tape is recording.
	CPPAD_ASSERT_UNKNOWN( i_vec < combined[ arg[0] - 1 ] );

	CPPAD_ASSERT_UNKNOWN( variable != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( combined != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( NumArg(StpvOp) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(StpvOp) == 0 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( arg[0] + i_vec < nc_combined );
	CPPAD_ASSERT_UNKNOWN( arg[2] < i_z );

	variable[ arg[0] + i_vec ] = true;
	combined[ arg[0] + i_vec ] = arg[2];
}

/*!
Zero order forward mode implementation of op = StvpOp.

\copydetails forward_store_op_0
*/
template <class Base>
inline void forward_store_vp_op_0(
	size_t         i_z         ,
	const size_t*  arg         , 
	size_t         num_par     ,
	size_t         nc_taylor   ,
	Base*          taylor      ,
	size_t         nc_combined ,
	bool*          variable    ,
	size_t*        combined    )
{	
	CPPAD_ASSERT_UNKNOWN( arg[1] < i_z );
	size_t i_vec = Integer( taylor[ arg[1] * nc_taylor + 0 ] );
	CPPAD_ASSERT_KNOWN( 
		i_vec < combined[ arg[0] - 1 ] ,
		"VecAD: index during zero order forward sweep is out of range"
	);

	CPPAD_ASSERT_UNKNOWN( variable != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( combined != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( NumArg(StvpOp) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(StvpOp) == 0 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( arg[0] + i_vec < nc_combined );
	CPPAD_ASSERT_UNKNOWN( arg[2] < num_par );

	variable[ arg[0] + i_vec ] = false;
	combined[ arg[0] + i_vec ] = arg[2];
}

/*!
Zero order forward mode implementation of op = StvvOp.

\copydetails forward_store_op_0
*/
template <class Base>
inline void forward_store_vv_op_0(
	size_t         i_z         ,
	const size_t*  arg         , 
	size_t         num_par     ,
	size_t         nc_taylor   ,
	Base*          taylor      ,
	size_t         nc_combined ,
	bool*          variable    ,
	size_t*        combined    )
{	
	CPPAD_ASSERT_UNKNOWN( arg[1] < i_z );
	size_t i_vec = Integer( taylor[ arg[1] * nc_taylor + 0 ] );
	CPPAD_ASSERT_KNOWN( 
		i_vec < combined[ arg[0] - 1 ] ,
		"VecAD: index during zero order forward sweep is out of range"
	);

	CPPAD_ASSERT_UNKNOWN( variable != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( combined != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( NumArg(StvpOp) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(StvpOp) == 0 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( arg[0] + i_vec < nc_combined );
	CPPAD_ASSERT_UNKNOWN( arg[2] < i_z );

	variable[ arg[0] + i_vec ] = true;
	combined[ arg[0] + i_vec ] = arg[2];
}

/*!
Forward mode sparsity operations for StpvOp and StvvOp

The C++ source code corresponding to this operation is
\verbatim
	v[x] = y
\endverbatim
where v is a VecAD<Base> vector, x is an AD<Base> object,
and y is AD<Base> or Base objects. 
We define the index corresponding to v[x] by
\verbatim
	i_v_x = combined[ arg[0] + i_vec ]
\endverbatim
where i_vec is defined under the heading \a arg[1] below:

\tparam Pack
is the type used to pack the sparsity pattern bit values; i.e.,
there is more that one bit per Pack value.

\param op
is the code corresponding to this operator; i.e., StpvOp or StvvOp
(only used for error checking).

\param arg
\n
\a arg[0]
is the offset corresponding to this VecAD vector in the combined array.
\n
\n 
\a arg[2]
\n
is the AD variable index corresponding to the variable y.
It is also the from node index for y in the \a var_sparsity connection.

\param num_combined
is the total number of elements in the VecAD address array.

\param combined
\a combined [ arg[0] - 1 ]
is the from node index for the VecAD vector v 
in the \a vecad_sparsity connection.
We use the notation i_v below which is defined by
\verbatim
	i_v = combined[ \a arg[0] - 1 ]
\endverbatim

\param num_vec
is the number of VecAD vectors corresponding to the tape;
this is also the number of rows in the VecAD sparsity pattern matrix.

\param num_var
is the total number of variables in the tape. 
This is also the number of row indices in the \a var_sparsity matrix.

\param var_sparsity
The from node with index \a arg[2] in \a var_sparsity
contains the sparsity bit pattern for y.
This is an input for this operation.
The sparsity pattern for y is added to the spartisy pattern for v.

\param vecad_sparsity
The from node with index \a i_v in \a vecad_sparsity
contains the sparsity bit pattern for the vector v.
The sparsity pattern for y is added
to the sparsity pattern for the vector v.

\n
\par Checked Assertions 
\li NumArg(op) == 3
\li NumRes(op) == 0
\li 0 <  \a arg[0]
\li \a arg[0] < \a num_combined
\li \a arg[2] < \a num_var
\li i_v < \a num_vec
*/
template <class Pack>
inline void forward_sparse_store_op(
	OpCode         op                  ,
	const size_t*  arg                 , 
	size_t         num_combined        ,
	const size_t*  combined            ,
	size_t         num_vec             ,
	size_t         num_var             ,
	connection<Pack>&   var_sparsity   ,
	connection<Pack>&   vecad_sparsity )
{
	CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(op) == 0 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( arg[0] < num_combined );
	size_t i_v = combined[ arg[0] - 1 ];
	CPPAD_ASSERT_UNKNOWN( i_v < num_vec );
	CPPAD_ASSERT_UNKNOWN( arg[2] < num_var );

	vecad_sparsity.binary_union(i_v, i_v, arg[2], var_sparsity);

	return;
}

/*!
Reverse mode sparsity operations for StpvOp and StvvOp

\copydetails sparse_store_op
*/
template <class Pack>
inline void reverse_sparse_store_op(
	OpCode         op           ,
	const size_t*  arg          , 
	size_t         num_combined ,
	const size_t*  combined     ,
	size_t         num_vec      ,
	size_t         num_var      ,
	size_t         nc_sparsity  ,
	Pack*          var_sparsity ,
	Pack*          vec_sparsity )
{
	CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
	CPPAD_ASSERT_UNKNOWN( NumRes(op) == 0 );
	CPPAD_ASSERT_UNKNOWN( 0 < arg[0] );
	CPPAD_ASSERT_UNKNOWN( arg[0] < num_combined );
	size_t i_v = combined[ arg[0] - 1 ];
	CPPAD_ASSERT_UNKNOWN( i_v < num_vec );
	CPPAD_ASSERT_UNKNOWN( arg[2] < num_var );

	Pack* y = var_sparsity + nc_sparsity * arg[2];
	Pack* v = vec_sparsity + nc_sparsity * i_v;

	size_t j = nc_sparsity;
	while(j--)
		y[j] |= v[j];

	return;
}

CPPAD_END_NAMESPACE
# endif
