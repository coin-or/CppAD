# ifndef CPPAD_LOCAL_OPTIMIZE_RECORD_VV_HPP
# define CPPAD_LOCAL_OPTIMIZE_RECORD_VV_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*!
\file record_vv.hpp
Record an operation of the form (variable op variable).
*/
// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {
/*!
Record an operation of the form (variable op variable).

\param play
player object corresponding to the old recroding.

\param random_itr
random iterator corresponding to the old recording.

\param new_var
mapping from old operator index to variable index in new recording.

\param i_op
is the index in the old operation sequence for this operator.
the must be one of the following:
AddvvOp, DivvvOp, MulvvOp, PowvvOp, SubvvOp, ZmulvvOp.

\param rec
is the object that will record the new operations.

\return
is the operator and variable indices in the new operation sequence.
*/
template <class Addr, class Base>
struct_size_pair record_vv(
   const player<Base>*                                play           ,
   const play::const_random_iterator<Addr>&           random_itr     ,
   const pod_vector<addr_t>&                          new_var        ,
   size_t                                             i_op           ,
   recorder<Base>*                                    rec            )
{
   // get_op_info
   op_code_var   op;
   const addr_t* arg;
   size_t        i_var;
   random_itr.op_info(i_op, op, arg, i_var);
   //
# ifndef NDEBUG
   switch(op)
   {  case AddvvOp:
      case DivvvOp:
      case MulvvOp:
      case PowvvOp:
      case SubvvOp:
      case ZmulvvOp:
      break;

      default:
      CPPAD_ASSERT_UNKNOWN(false);
   }
# endif
   CPPAD_ASSERT_UNKNOWN( NumRes(op) > 0 );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < i_var ); // DAG condition
   CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < i_var ); // DAG condition
   //
   addr_t new_arg[2];
   new_arg[0]   = new_var[ random_itr.var2op(size_t(arg[0])) ];
   new_arg[1]   = new_var[ random_itr.var2op(size_t(arg[1])) ];
   rec->PutArg( new_arg[0], new_arg[1] );
   //
   struct_size_pair ret;
   ret.i_op  = rec->num_op_rec();
   ret.i_var = size_t(rec->PutOp(op));
   CPPAD_ASSERT_UNKNOWN( 0 < new_arg[0] && size_t(new_arg[0]) < ret.i_var );
   CPPAD_ASSERT_UNKNOWN( 0 < new_arg[1] && size_t(new_arg[1]) < ret.i_var );
   return ret;
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE


# endif
