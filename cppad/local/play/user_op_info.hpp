# ifndef CPPAD_LOCAL_PLAY_USER_OP_INFO_HPP
# define CPPAD_LOCAL_PLAY_USER_OP_INFO_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
           GNU General Public License, Version 3.0.
---------------------------------------------------------------------------- */

// BEGIN_CPPAD_LOCAL_PLAY_NAMESPACE
namespace CppAD { namespace local { namespace play {

/*!
\file user_op_info.hpp
*/

/*!
\brief
Unpack extra information corresponding to a UserOp

\param op [in]
must be a UserOp

\param op_arg [in]
is the arguments for this operator

\param user_old [out]
is the extra information passed to the old style user atomic functions.

\param user_m   [out]
is the number of results for this user atmoic function.

\param user_n   [out]
is the number of arguments for this user atmoic function.

\return
Is a pointer to this user atomic function.
*/
template <class Base>
atomic_base<Base>* user_op_info(
	const OpCode     op         ,
	const addr_t*    op_arg     ,
	size_t&          user_old   ,
	size_t&          user_m     ,
	size_t&          user_n     )
{	atomic_base<Base>* user_atom;
	//
	CPPAD_ASSERT_UNKNOWN( op == UserOp );
	CPPAD_ASSERT_NARG_NRES(op, 4, 0);
	//
	user_old = size_t(op_arg[1]);
	user_n   = size_t(op_arg[2]);
	user_m   = size_t(op_arg[3]);
	CPPAD_ASSERT_UNKNOWN( user_n > 0 );
	//
	size_t user_index = size_t( op_arg[0] );
	user_atom = atomic_base<Base>::class_object(user_index);
# ifndef NDEBUG
	if( user_atom == CPPAD_NULL )
	{	// user_atom is null so cannot use user_atom->afun_name()
		std::string msg = atomic_base<Base>::class_name(user_index)
			+ ": atomic_base function has been deleted";
		CPPAD_ASSERT_KNOWN(false, msg.c_str() );
	}
# endif
	// the atomic_base object corresponding to this user function
	user_atom = atomic_base<Base>::class_object(user_index);
	CPPAD_ASSERT_UNKNOWN( user_atom != CPPAD_NULL );
	return user_atom;
}

} } } // END_CPPAD_LOCAL_PLAY_NAMESPACE

# endif
