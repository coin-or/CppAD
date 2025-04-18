# ifndef CPPAD_LOCAL_OPTIMIZE_RECORD_VP_HPP
# define CPPAD_LOCAL_OPTIMIZE_RECORD_VP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-25 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/new_optimize/subvector.hpp>

/*!
\file record_vp.hpp
Record an operation of the form (variable op parameter).
*/
// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {


/*!
Record an operation of the form (variable op parameter).

\param play
player object corresponding to the old recroding.

\param var_op_info
is the information for the old variable operation sequence.

\param new_par
mapping from old parameter index to parameter index in new recording.

\param new_var
mapping from old operator index to variable index in new recording.

\param i_op
is the index in the old operation sequence for this operator.
the must be one of the following:
DivvpOp, PowvpOp, SubvpOp, ZmulvpOp.

\param rec
is the object that will record the new operations.

\return
is the operator and variable indices in the new operation sequence.
*/
template <class Base>
struct_size_pair record_vp(
   const player<Base>*                                 play           ,
   const var_op_info_t< player<Base> >&                var_op_info    ,
   const pod_vector<addr_t>&                           new_par        ,
   const pod_vector<addr_t>&                           new_var        ,
   size_t                                              i_op           ,
   recorder<Base>*                                     rec            )
{
   // op, arg
   op_code_var   op  = var_op_info.op_enum(i_op);
   const addr_t* arg = var_op_info.arg_ptr(i_op);
   //
# ifndef NDEBUG
   // i_var
   size_t  i_var = var_op_info.var_index(i_op);
   //
   switch(op)
   {  case DivvpOp:
      case PowvpOp:
      case SubvpOp:
      case ZmulvpOp:
      break;

      default:
      CPPAD_ASSERT_UNKNOWN(false);
   }
   // number of parameters corresponding to the old operation sequence.
   size_t npar = play->num_par_all();
# endif

   // vector of length npar containing the parameters the old operation
   // sequence; i.e., given a parameter index i < npar, the corresponding
   // parameter value is par[i].
   //
   CPPAD_ASSERT_UNKNOWN( NumRes(op) > 0 );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < i_var ); // DAG condition
   CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < npar  );
   //
   addr_t new_arg[2];
   new_arg[0]   = new_var[ var_op_info.op_index( size_t(arg[0]) ) ];
   new_arg[1]   = new_par[ arg[1] ];
   rec->PutArg( new_arg[0], new_arg[1] );
   //
   struct_size_pair ret;
   ret.i_op  = rec->num_var_op();
   ret.i_var = size_t(rec->PutOp(op));
   CPPAD_ASSERT_UNKNOWN( 0 < new_arg[0] && size_t(new_arg[0]) < ret.i_var );
   return ret;
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE


# endif
