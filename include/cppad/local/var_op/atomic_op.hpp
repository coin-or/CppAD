# ifndef CPPAD_LOCAL_VAR_OP_ATOMIC_OP_HPP
# define CPPAD_LOCAL_VAR_OP_ATOMIC_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

// BEGIN_CPPAD_LOCAL_VAR_OP_NAMESPACE
namespace CppAD { namespace local { namespace var_op {

/*
{xrst_begin var_atomic_op dev}
{xrst_spell
   funap
   funav
   funrp
   funrv
}

Atomic Function Call Operators
##############################

AFunOp
******
This operator appears at the start and end of every atomic function call.
It has four arguments and no results.

arg[0]
======
This is the :ref:`atomic_index-name` for this function.

arg[1]
======
If this is an atomic_four call,
arg[1] is the corresponding :ref:`atomic_four_call@call_id` .
If this is an atomic_one call (which has been deprecated)
arg[1] is corresponding :ref:`atomic_one@id`

arg[2], n
=========
is the number of arguments to this atomic function call.
We use the notation *n* = *arg* [2] .

arg[3], m
=========
is the number of results returned by this atomic function call.
We use the notation *m* = *arg* [3] .

FunapOp, FunavOp
****************
#. There are *n* of these operators directly after
   the AFunOp at the start of a function call,
   one for each argument to the function call.
#. Each of these operators has one argument and no result.
#. If the *j*-th argument in the function call is a parameter (variable)
   the corresponding operator is FunapOp ( FunavOp ).
#. The operator argument for FunapOp ( FunavOp )
   is the parameter index (variable index) for the corresponding
   argument in the function call.

FunrpOp, FunrvOp
****************
#. There are *m* of these operators directly after the
   FunapOp or FunavOp operators for a function call,
   one for each result returned by the function call.
#. If the *i*-th result is a parameter (variable)
   the corresponding operator is FunrpOp ( FunrvOp ).
#. The FunrpOp operator has one argument and no result
   (because the corresponding function result is not a variable).
   The operator argument is the parameter index for the corresponding
   result of the function call.
#. The FunrvOp operator has no arguments and one variable result.
   The new variable, created by this operator, gets its values from
   the corresponding result of the function.

{xrst_end var_atomic_op}
*/

// atomic_op_work
template <class Base>
struct atomic_op_work {
   // parameter_x, taylor_x, type_x, taylor_y, index_y, select_y
   CppAD::vector<Base>           parameter_x;
   CppAD::vector<Base>           taylor_x;
   CppAD::vector<ad_type_enum>   type_x;
   //
   CppAD::vector<Base>           taylor_y;
   CppAD::vector<size_t>         index_y;
   CppAD::vector<bool>           select_y;
   //
   // resize
   void resize(size_t m, size_t n, size_t n_order)
   {
      parameter_x.resize(n);
      taylor_x.resize(n * n_order);
      type_x.resize(n);
      //
      taylor_y.resize(m * n_order);
      index_y.resize(m);
      select_y.resize(m);
   }
};

template <class Base, class RecBase>
void atomic_forward_op(
   play::const_sequential_iterator& itr        ,
   Base*                            taylor     ,
   const player<Base>*              play       ,
   const Base*                      parameter  ,
   size_t                           cap_order  ,
   size_t                           order_low  ,
   size_t                           order_up   ,
   bool                             trace      ,
   atomic_op_work<Base>&            work       )
{  //
   // vector
   using CppAD::vector;
   //
   // dyn_par_is
   const pod_vector<bool>& dyn_par_is( play->dyn_par_is() );
   //
   // n_order
   size_t n_order = order_up + 1;
   //
   // op_code, i_var, arg
   op_code_var   op_code;
   size_t        i_var;
   const addr_t* arg;
   itr.op_info(op_code, arg, i_var);
   CPPAD_ASSERT_UNKNOWN( op_code == AFunOp );
   CPPAD_ASSERT_NARG_NRES(op_code, 4, 0);
   //
   if( trace )
   {  printOp<Base, RecBase>(
         std::cout, play, itr.op_index(), i_var, op_code, arg
      );
      std::cout << std::endl;
   }
   //
   // atom_index, call_id, m, n
   size_t atom_index, call_id, m, n;
   play::atom_op_info<RecBase>(op_code, arg, atom_index, call_id, m, n);
   //
   // parameter_x, taylor_x, type_x, taylor_y, index_y, select_y
   work.resize(m, n, n_order);
   vector<Base>&         parameter_x( work.parameter_x );
   vector<Base>&         taylor_x(    work.taylor_x );
   vector<ad_type_enum>& type_x(      work.type_x );
   vector<Base>&         taylor_y(    work.taylor_y );
   vector<size_t>&       index_y(     work.index_y );
   vector<bool>&         select_y(    work.select_y );
   //
   // j
   for(size_t j = 0; j < n; ++j)
   {  //
      // op_code, arg, i_var
      (++itr).op_info(op_code, arg, i_var);
      if( trace )
      {  printOp<Base, RecBase>(
            std::cout, play, itr.op_index(), i_var, op_code, arg
         );
         std::cout << std::endl;
      }
      //
      // type_x, parameter_x, taylor_x
      switch(op_code)
      {  //
         default:
         CPPAD_ASSERT_UNKNOWN(false);
         break;
         //
         // FunapOp
         case FunapOp:
         CPPAD_ASSERT_NARG_NRES(op_code, 1, 0);
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < play->num_par_rec() );
         if( dyn_par_is[ arg[0] ] )
            type_x[j]    = dynamic_enum;
         else
            type_x[j]    = constant_enum;
         parameter_x[j]             = parameter[ arg[0] ];
         taylor_x[j * n_order + 0]  = parameter[ arg[0] ];
         for(size_t k = 1; k < n_order; ++k)
            taylor_x[j * n_order + k] = Base(0.0);
         break;
         //
         // FunavOp
         case FunavOp:
         CPPAD_ASSERT_NARG_NRES(op_code, 1, 0);
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < play->num_var_rec() );
         type_x[j]       = variable_enum;
         parameter_x[j]  = CppAD::numeric_limits<Base>::quiet_NaN();
         for(size_t k = 0; k < n_order; ++k)
            taylor_x[j * n_order + k] =
               taylor[ size_t(arg[0]) * cap_order  + k ];
         break;
      }
   }
   //
   // index_y, select_y
   for(size_t i = 0; i < m; ++i)
   {  //
      // op_code, arg, i_var
      (++itr).op_info(op_code, arg, i_var);
      //
      //
      switch(op_code)
      {  //
         default:
         CPPAD_ASSERT_UNKNOWN(false);
         break;
         //
         // FunrpOp
         case FunrpOp:
         CPPAD_ASSERT_NARG_NRES(op_code, 1, 0);
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < play->num_par_rec() );
         index_y[i]  = 0;
         select_y[i] = false;
         if( 0 < order_low )
            taylor_y[i * n_order + 0] = parameter[ arg[0] ];
         for(size_t k = 1; k < order_low; ++k)
            taylor_y[i * n_order + k] = Base(0.0);
         if( trace )
         {  printOp<Base, RecBase>(
               std::cout, play, itr.op_index(), i_var, op_code, arg
            );
            std::cout << std::endl;
         }
         break;
         //
         // FunavOp
         case FunrvOp:
         CPPAD_ASSERT_NARG_NRES(op_code, 0, 1);
         CPPAD_ASSERT_UNKNOWN( 0 < i_var );
         index_y[i]  = i_var;
         select_y[i] = true;
         for(size_t k = 0; k < order_low; ++k)
            taylor_y[i * n_order + k] = taylor[i_var * cap_order + k];
         break;
      }
   }
   //
   // op_code
   (++itr).op_info(op_code, arg, i_var);
   CPPAD_ASSERT_UNKNOWN( op_code == AFunOp );
   //
   // taylor_y
   size_t need_y = size_t(variable_enum);
   sweep::call_atomic_forward<Base, RecBase>(
      parameter_x,
      type_x,
      need_y,
      select_y,
      order_low,
      order_up,
      atom_index,
      call_id,
      taylor_x,
      taylor_y
   );
   //
   // taylor
   for(size_t i = 0; i < m; ++i)
   {  if( index_y[i] > 0 )
      {  for(size_t k = order_low; k < n_order; ++k)
            taylor[ index_y[i] * cap_order  + k ] = taylor_y[i * n_order + k];
         if( trace )
         {  const addr_t* null_addr = static_cast<addr_t*>( nullptr );
            printOp<Base, RecBase>(
               std::cout, play, itr.op_index(), index_y[i], FunrvOp, null_addr
            );
            Base* yi_ptr          = taylor + index_y[i] * cap_order + 0;
            const Base* null_base = static_cast<Base*>( nullptr );
            printOpResult<Base>(
               std::cout, n_order, yi_ptr, 0, null_base
            );
            std::cout << std::endl;
         }
      }
   }
   //
   return;
}

} } } // END_CPPAD_LOCAL_VAR_OP_NAMESPACE

# endif
