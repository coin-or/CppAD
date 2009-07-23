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

CPPAD_END_NAMESPACE
# endif
