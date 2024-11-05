# ifndef CPPAD_LOCAL_PLAY_ATOM_OP_INFO_HPP
# define CPPAD_LOCAL_PLAY_ATOM_OP_INFO_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

// BEGIN_CPPAD_LOCAL_PLAY_NAMESPACE
namespace CppAD { namespace local { namespace play {

/*!
\file atom_op_info.hpp
*/

/*!
\brief
Unpack extra information corresponding to a AFunOp

\param op [in]
must be a AFunOp

\param op_arg [in]
is the arguments for this operator

\param atom_index [out]
is the index in local::atomic_index corresponding to this atomic functions.

\param call_id  [out]
is the call_id for this atomic function call.

\param atom_m   [out]
is the number of results for this user atmoic function.

\param atom_n   [out]
is the number of arguments for this user atmoic function.

\return
Is a pointer to this atomic function.
*/
// MUSTDO: change return to void once all sweeps switch to use call_atomic.
template <class Base>
atomic_base<Base>* atom_op_info(
   const op_code_var op         ,
   const addr_t*    op_arg     ,
   size_t&          atom_index ,
   size_t&          call_id    ,
   size_t&          atom_m     ,
   size_t&          atom_n     )
{  atomic_base<Base>* atom_fun;
   //
   CPPAD_ASSERT_UNKNOWN( op == AFunOp );
   CPPAD_ASSERT_NARG_NRES(op, 4, 0);
   //
   atom_index = size_t(op_arg[0]);
   call_id    = size_t(op_arg[1]);
   atom_n     = size_t(op_arg[2]);
   atom_m     = size_t(op_arg[3]);
   CPPAD_ASSERT_UNKNOWN( atom_n > 0 );
   //
   bool         set_null = false;
   size_t       type     = 0;          // set to avoid warning
   std::string* name_ptr = nullptr;
   void*        v_ptr    = nullptr; // set to avoid warning
   local::atomic_index<Base>(set_null, atom_index, type, name_ptr, v_ptr);
   if( type == 3 )
      return nullptr;
# ifndef NDEBUG
   if( v_ptr == nullptr )
   {  // atom_fun is null so cannot use atom_fun->atomic_name()
      std::string msg = atomic_base<Base>::class_name(atom_index)
         + ": atomic_base function has been deleted";
      CPPAD_ASSERT_KNOWN(false, msg.c_str() );
   }
# endif
   // the atomic_base object corresponding to this atomic function
   atom_fun = reinterpret_cast< atomic_base<Base>* >( v_ptr );
   return atom_fun;
}

} } } // END_CPPAD_LOCAL_PLAY_NAMESPACE

# endif
