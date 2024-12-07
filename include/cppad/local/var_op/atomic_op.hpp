# ifndef CPPAD_LOCAL_VAR_OP_ATOMIC_OP_HPP
# define CPPAD_LOCAL_VAR_OP_ATOMIC_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

// BEGIN_CPPAD_LOCAL_VAR_OP_NAMESPACE
namespace CppAD { namespace local { namespace var_op {

// sweep_type
enum sweep_type {
   forward_op_sweep,
   forward_dir_sweep,
   reverse_op_sweep,
   for_jac_sweep,
   rev_jac_sweep,
   for_hes_sweep,
   rev_hes_sweep
};

// atomic_op_work
template <class Base>
struct atomic_op_work {
   // parameter_x, taylor_x, type_x, taylor_y, index_y, variable_y
   CppAD::vector<Base>           parameter_x;
   CppAD::vector<ad_type_enum>   type_x;
   //
   CppAD::vector<Base>           taylor_x;
   CppAD::vector<Base>           taylor_y;
   //
   CppAD::vector<Base>           partial_x;
   CppAD::vector<Base>           partial_y;
   //
   CppAD::vector<size_t>         index_x;
   CppAD::vector<size_t>         index_y;
   //
   CppAD::vector<bool>           variable_x;
   CppAD::vector<bool>           variable_y;
   //
   // resize
   void resize(size_t m, size_t n, size_t n_order, sweep_type sweep)
   {  //
      // parameter_x, type_x
      parameter_x.resize(n);
      type_x.resize(n);
      //
      switch( sweep )
      {
         default:
         CPPAD_ASSERT_UNKNOWN(false);
         break;
         // ------------------------------------------------------------------
         // forward_op_sweep
         case forward_op_sweep:
         taylor_x.resize(n * n_order);
         taylor_y.resize(m * n_order);
         //
         partial_x.resize(0);
         partial_y.resize(0);
         //
         index_x.resize(0);
         index_y.resize(m);
         //
         variable_x.resize(0);
         variable_y.resize(m);
         break;
         // ------------------------------------------------------------------
         // forward_dir_sweep
         case forward_dir_sweep:
         taylor_x.resize(n * n_order);
         taylor_y.resize(m * n_order);
         //
         partial_x.resize(0);
         partial_y.resize(0);
         //
         index_x.resize(n);
         index_y.resize(m);
         //
         variable_x.resize(0);
         variable_y.resize(m);
         break;
         // ------------------------------------------------------------------
         // reverse_op_sweep
         case reverse_op_sweep:
         taylor_x.resize(n * n_order);
         taylor_y.resize(m * n_order);
         //
         partial_x.resize(n * n_order);
         partial_y.resize(m * n_order);
         //
         index_x.resize(n);
         index_y.resize(0);
         //
         variable_x.resize(n);
         variable_y.resize(0);
         break;
         // ------------------------------------------------------------------
         // for_jac_sweep, rev_jac_sweep
         // for_hes_sweep, rev_hes_sweep
         case for_jac_sweep:
         case rev_jac_sweep:
         case for_hes_sweep:
         case rev_hes_sweep:
         taylor_x.resize(0);
         taylor_y.resize(0);
         //
         partial_x.resize(0);
         partial_y.resize(0);
         //
         index_x.resize(n);
         index_y.resize(m);
         //
         variable_x.resize(0);
         variable_y.resize(0);
         break;
      }
   }
};
/*
------------------------------------------------------------------------------
{xrst_begin_parent var_atomic_op dev}
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
-------------------------------------------------------------------------------
{xrst_begin var_atomic_forward_any dev}

Any Order Forward Atomic Function Call
######################################

Prototype
*********
{xrst_literal
   // BEGIN_ATOMIC_FORWARD_ANY
   // END_ATOMIC_FORWARD_ANY
}

{xrst_template ;
   include/cppad/local/var_op/template/atomic_op.xrst
   headers: n_res, i_z, itr, play, parameter, trace, work

   @mode@ ; forward
}
{xrst_template ;
   include/cppad/local/var_op/template/forward_op.xrst
   headers: cap_order, order_low, order_up, taylor
}

{xrst_end var_atomic_forward_any}
*/


// BEGIN_ATOMIC_FORWARD_ANY
template <class Base, class RecBase>
void atomic_forward_any(
   play::const_sequential_iterator& itr        ,
   const player<Base>*              play       ,
   const Base*                      parameter  ,
   bool                             trace      ,
   atomic_op_work<Base>&            work       ,
   size_t                           cap_order  ,
   size_t                           order_low  ,
   size_t                           order_up   ,
   Base*                            taylor     )
// END_ATOMIC_FORWARD_ANY
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
   // parameter_x, type_x, taylor_x, taylor_y, index_y, variable_y
   work.resize(m, n, n_order, forward_op_sweep);
   vector<Base>&         parameter_x( work.parameter_x );
   vector<ad_type_enum>& type_x( work.type_x );
   //
   vector<Base>&         taylor_x( work.taylor_x );
   vector<Base>&         taylor_y( work.taylor_y );
   //
   vector<size_t>&       index_y( work.index_y );
   vector<bool>&         variable_y( work.variable_y );
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
   // index_y, variable_y
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
         index_y[i]    = std::numeric_limits<size_t>::max();
         variable_y[i] = false;
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
         // FunrvOp
         case FunrvOp:
         CPPAD_ASSERT_NARG_NRES(op_code, 0, 1);
         CPPAD_ASSERT_UNKNOWN( 0 < i_var );
         index_y[i]    = i_var;
         variable_y[i] = true;
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
      variable_y,
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
   {  if( variable_y[i] )
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
/*
-----------------------------------------------------------------------------
{xrst_begin var_atomic_forward_dir dev}

Multiple Direction Forward Atomic Function Call
###############################################

Prototype
*********
{xrst_literal
   // BEGIN_ATOMIC_FORWARD_DIR
   // END_ATOMIC_FORWARD_DIR
}

{xrst_template ;
   include/cppad/local/var_op/template/atomic_op.xrst
   headers: n_res, i_z, itr, play, parameter, trace, work

   @mode@ ; forward
}
{xrst_template ;
   include/cppad/local/var_op/template/forward_dir.xrst
   headers: n_dir, cap_order, order_up, taylor
}


{xrst_end var_atomic_forward_dir}
*/
// BEGIN_ATOMIC_FORWARD_DIR
template <class Base, class RecBase>
void atomic_forward_dir(
   play::const_sequential_iterator& itr        ,
   const player<Base>*              play       ,
   const Base*                      parameter  ,
   bool                             trace      ,
   atomic_op_work<Base>&            work       ,
   size_t                           cap_order  ,
   size_t                           order_up   ,
   size_t                           n_dir      ,
   Base*                            taylor     )
// END_ATOMIC_FORWARD_DIR
{  //
   // order_low
   size_t order_low = order_up;
   //
   // vector
   using CppAD::vector;
   //
   // taylor_index
   size_t per_variable = (cap_order - 1) * n_dir + 1;
   auto taylor_index = [per_variable, n_dir]
      (size_t variable_index, size_t order, size_t dir)
   {  size_t index = variable_index * per_variable;
      if( order > 0 )
         index += (order - 1) * n_dir + 1 + dir;
      return index;
   };
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
   // parameter_x, type_x, taylor_x, taylor_y, index_x, index_y, variable_y
   work.resize(m, n, n_order, forward_dir_sweep);
   vector<Base>&         parameter_x( work.parameter_x );
   vector<ad_type_enum>& type_x( work.type_x );
   //
   vector<Base>&         taylor_x( work.taylor_x );
   vector<Base>&         taylor_y( work.taylor_y );
   //
   vector<size_t>&       index_x( work.index_x );
   vector<size_t>&       index_y( work.index_y );
   vector<bool>&         variable_y(work.variable_y );

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
      // index_x, type_x, parameter_x
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
         index_x[j] = std::numeric_limits<size_t>::max();
         if( dyn_par_is[ arg[0] ] )
            type_x[j]    = dynamic_enum;
         else
            type_x[j]    = constant_enum;
         parameter_x[j]  = parameter[ arg[0] ];
         break;
         //
         // FunavOp
         case FunavOp:
         CPPAD_ASSERT_NARG_NRES(op_code, 1, 0);
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < play->num_var_rec() );
         index_x[j]      = size_t( arg[0] );
         type_x[j]       = variable_enum;
         parameter_x[j]  = CppAD::numeric_limits<Base>::quiet_NaN();
         break;
      }
   }
   //
   // index_y, variable_y
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
         index_y[i]    = size_t( arg[0] );
         variable_y[i] = false;
         if( trace )
         {  printOp<Base, RecBase>(
               std::cout, play, itr.op_index(), i_var, op_code, arg
            );
            std::cout << std::endl;
         }
         break;
         //
         // FunrvOp
         case FunrvOp:
         CPPAD_ASSERT_NARG_NRES(op_code, 0, 1);
         CPPAD_ASSERT_UNKNOWN( 0 < i_var );
         index_y[i]    = i_var;
         variable_y[i] = true;
         break;
      }
   }
   //
   // op_code
   (++itr).op_info(op_code, arg, i_var);
   CPPAD_ASSERT_UNKNOWN( op_code == AFunOp );
   //
   // dir
   for(size_t dir = 0; dir < n_dir; ++dir)
   {
      //
      // taylor_x
      for(size_t j = 0; j < n; ++j)
      {  if( type_x[j] != variable_enum )
         {  taylor_x[j * n_order + 0] = parameter_x[j];
            for(size_t k = 1; k < n_order; ++k)
               taylor_x[j * n_order + k] = Base(0.0);
         }
         else
         {  for(size_t k = 0; k < n_order; ++k)
            {  size_t index              = taylor_index(index_x[j], k, dir);
               taylor_x[j * n_order + k] = taylor[index];
            }
         }
      }
      //
      // taylor_y
      for(size_t i = 0; i < m; ++i)
      {  if( ! variable_y[i] )
         {  if( 0 < order_low )
               taylor_y[i * n_order + 0] = parameter[ index_y[i] ];
            for(size_t k = 1; k < order_low; ++k)
               taylor_y[i * n_order + k] = Base(0.0);
         }
         else
         {  for(size_t k = 0; k < order_low; ++k)
            {  size_t index              = taylor_index(index_y[i], k, dir);
               taylor_y[i * n_order + k] = taylor[index];
            }
         }
      }
      size_t need_y = size_t(variable_enum);
      sweep::call_atomic_forward<Base, RecBase>(
         parameter_x,
         type_x,
         need_y,
         variable_y,
         order_low,
         order_up,
         atom_index,
         call_id,
         taylor_x,
         taylor_y
      );
      //
      // taylor
      for(size_t i = 0; i < m; ++i) if( variable_y[i] )
      {  for(size_t k = order_low; k < n_order; ++k)
         {  size_t index  = taylor_index(index_y[i], k, dir);
            taylor[index] = taylor_y[i * n_order + k];
         }
         if( trace )
         {  const addr_t* null_addr = static_cast<addr_t*>( nullptr );
            if( dir == 0 )
            {  printOp<Base, RecBase>(
               std::cout, play, itr.op_index(), index_y[i], FunrvOp, null_addr
               );
               std::cout << std::endl;
            }
            Base* yi_ptr          = taylor_y.data() + i * n_order;
            const Base* null_base = static_cast<Base*>( nullptr );
            std::cout << "     ";
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
/*
-------------------------------------------------------------------------------
{xrst_begin var_atomic_reverse dev}

Reverse Atomic Function Call
############################

Prototype
*********
{xrst_literal
   // BEGIN_ATOMIC_REVERSE
   // END_ATOMIC_REVERSE
}

Iterator
********
this template parameter is either
``play::subgraph_iterator`` or
``play::const_sequential_iterator`` .

{xrst_template ;
   include/cppad/local/var_op/template/atomic_op.xrst
   headers: n_res, i_z, itr, play, parameter, trace, work
   headers if @mode@ is reverse: G and H

   @mode@ ; reverse
}

cap_order
*********
see atomic_forward_any :ref:`var_atomic_forward_any@cap_order`

n_order
*******
is the number of Taylor coefficient orders that we are
computing the partial derivatives with respect to.

taylor
******
The Taylor coefficient for the variable with index j and order k is::

   taylor[ j * cap_order + k ]

partial
*******
The partial derivative with respect to the order *k* Taylor coefficient
for the variable with index *j* is::

   partial[ j * n_order + k ]

On input, *partial* contains the partial derivatives of *G*
with respect to the Taylor coefficients of the arguments to *G* .
On output, *partial* contains the partial derivatives of *H*
with respect to the Taylor coefficients of the arguments to *H* .
We only have partials with respect to variables; i.e.,
*partial* does not included derivatives with respect to
the parameters in *x* or *y* .
~

{xrst_end var_atomic_reverse}
*/
// BEGIN_ATOMIC_REVERSE
template <class Base, class RecBase, class Iterator>
void atomic_reverse(
   Iterator&                        itr        ,
   const player<Base>*              play       ,
   const Base*                      parameter  ,
   bool                             trace      ,
   atomic_op_work<Base>&            work       ,
   size_t                           cap_order  ,
   size_t                           n_order    ,
   const Base*                      taylor     ,
   Base*                            partial    )
// END_ATOMIC_REVERSE
{  CPPAD_ASSERT_UNKNOWN( 0 < n_order );
   //
   // vector
   using CppAD::vector;
   //
   // dyn_par_is
   const pod_vector<bool>& dyn_par_is( play->dyn_par_is() );
   //
   // n_order
   size_t order_up = n_order - 1;
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
   // parameter_x, type_x,
   // taylor_x, taylor_y, partial_x, partial_y, variable_x, index_x
   work.resize(m, n, n_order, reverse_op_sweep);
   vector<Base>&         parameter_x( work.parameter_x );
   vector<ad_type_enum>& type_x( work.type_x );
   //
   vector<Base>&         taylor_x( work.taylor_x );
   vector<Base>&         taylor_y( work.taylor_y );
   //
   vector<Base>&         partial_x( work.partial_x );
   vector<Base>&         partial_y( work.partial_y );
   //
   vector<bool>&         variable_x( work.variable_x );
   vector<size_t>&       index_x( work.index_x );
   //
   // i
   for(size_t ip1 = m; ip1 > 0; --ip1)
   {  size_t i = ip1 - 1;
      //
      // op_code, arg, i_var
      (--itr).op_info(op_code, arg, i_var);
      if( trace )
      {  printOp<Base, RecBase>(
            std::cout, play, itr.op_index(), i_var, op_code, arg
         );
      }
      //
      // taylor_y, partial_y
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
         taylor_y[i * n_order + 0]  = parameter[ arg[0] ];
         partial_y[i * n_order + 0] = Base(0.0);
         for(size_t k = 1; k < n_order; ++k)
         {  partial_y[i * n_order + k] = Base(0.0);
            taylor_y[ i * n_order + k] = Base(0.0);
         }
         break;
         //
         // FunrvOp
         case FunrvOp:
         CPPAD_ASSERT_NARG_NRES(op_code, 0, 1);
         CPPAD_ASSERT_UNKNOWN( 0 < i_var );
         for(size_t k = 0; k < n_order; ++k)
         {  taylor_y[ i * n_order + k] = taylor[i_var * cap_order + k];
            partial_y[i * n_order + k] = partial[i_var * n_order + k];
         }
         if( trace )
         {  const Base* t_ptr = taylor + i_var * cap_order;
            const Base* p_ptr = partial + i_var * n_order;
            printOpResult(
               std::cout,
               n_order,
               t_ptr,
               n_order,
               p_ptr
            );
         }
         break;
      }
      if( trace )
         std::cout << std::endl;
   }
   //
   // j
   for(size_t jp1 = n; jp1 > 0; --jp1)
   {  size_t j = jp1 - 1;
      //
      // op_code, arg, i_var
      (--itr).op_info(op_code, arg, i_var);
      if( trace )
      {  printOp<Base, RecBase>(
            std::cout, play, itr.op_index(), i_var, op_code, arg
         );
         std::cout << std::endl;
      }
      //
      // variable_x, type_x
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
         variable_x[j] = false;
         index_x[j]    = size_t( arg[0] );
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
         variable_x[j]   = true;
         index_x[j]      = size_t( arg[0] );
         type_x[j]       = variable_enum;
         parameter_x[j]  = CppAD::numeric_limits<Base>::quiet_NaN();
         for(size_t k = 0; k < n_order; ++k)
            taylor_x[j * n_order + k] =
               taylor[ size_t(arg[0]) * cap_order  + k ];
         break;
      }
   }
   //
   // op_code
   (--itr).op_info(op_code, arg, i_var);
   CPPAD_ASSERT_UNKNOWN( op_code == AFunOp );
   //
   // partial_y
   sweep::call_atomic_reverse<Base, RecBase>(
      parameter_x,
      type_x,
      variable_x,
      order_up,
      atom_index,
      call_id,
      taylor_x,
      taylor_y,
      partial_x,
      partial_y
   );
   //
   // taylor
   for(size_t j = 0; j < n; ++j)
   {  if( variable_x[j] )
      {  for(size_t k = 0; k < n_order; ++k)
         {  size_t index = index_x[j] * n_order + k;
            partial[index] += partial_x[j * n_order + k];
         }
      }
   }
   //
   return;
}
/*
-------------------------------------------------------------------------------
{xrst_begin var_atomic_for_jac dev}

Forward Jacobian Sparsity Atomic Function Call
##############################################

Prototype
*********
{xrst_literal
   // BEGIN_ATOMIC_FOR_JAC
   // END_ATOMIC_FOR_JAC
}

{xrst_template ;
   include/cppad/local/var_op/template/atomic_op.xrst
   headers: n_res, i_z, itr, play, parameter, trace, work

   @mode@ ; forward
}

Vector_set
**********
is the type used for vectors of sets. It must satisfy the
:ref:`SetVector-name` concept.

var_sparsity
************

Input
=====
::

   for j = 0, ..., i_z - n_res
      The set with index j in var_sparsity

Output
======
::

   for j = i_z , ... , i_z - n_res + 1
      The set with index j in var_sparsity

{xrst_end var_atomic_for_jac}
*/
// BEGIN_ATOMIC_FOR_JAC
template <class Vector_set, class Base, class RecBase>
inline void atomic_for_jac(
   play::const_sequential_iterator& itr          ,
   const player<Base>*              play         ,
   const Base*                      parameter    ,
   bool                             trace        ,
   atomic_op_work<Base>&            work         ,
   bool                             dependency   ,
   Vector_set&                      var_sparsity )
// END_ATOMIC_FOR_JAC
{  //
   //
   // vector
   using CppAD::vector;
   //
   // dyn_par_is
   const pod_vector<bool>& dyn_par_is( play->dyn_par_is() );
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
   // parameter_x, type_x, index_x, index_y
   size_t n_order = 0;
   work.resize(m, n, n_order, for_jac_sweep);
   vector<Base>&         parameter_x( work.parameter_x );
   vector<ad_type_enum>& type_x( work.type_x );
   vector<size_t>&       index_x( work.index_x );
   vector<size_t>&       index_y( work.index_y );
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
      // type_x, parameter_x, index_x
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
         parameter_x[j]  = parameter[ arg[0] ];
         index_x[j]      = 0; // special variable index used for parameters
         break;
         //
         // FunavOp
         case FunavOp:
         CPPAD_ASSERT_NARG_NRES(op_code, 1, 0);
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < play->num_var_rec() );
         type_x[j]       = variable_enum;
         parameter_x[j]  = CppAD::numeric_limits<Base>::quiet_NaN();
         index_x[j]      = size_t(arg[0]);
         break;
      }
   }
   //
   // i
   for(size_t i = 0; i < m; ++i)
   {  //
      // op_code, arg, i_var
      (++itr).op_info(op_code, arg, i_var);
      //
      // index_y
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
         index_y[i]       = 0;  // special variable index used for parameters
         break;
         //
         // FunrvOp
         case FunrvOp:
         CPPAD_ASSERT_NARG_NRES(op_code, 0, 1);
         CPPAD_ASSERT_UNKNOWN( 0 < i_var );
         index_y[i]    = i_var;
         break;
      }
   }
   //
   // op_code
   (++itr).op_info(op_code, arg, i_var);
   CPPAD_ASSERT_UNKNOWN( op_code == AFunOp );
   //
   // varsparsity
   sweep::call_atomic_for_jac_sparsity<Base,RecBase>(
      atom_index,
      call_id,
      dependency,
      parameter_x,
      type_x,
      index_x,
      index_y,
      var_sparsity
   );
   //
   if( trace )
   {  size_t end = var_sparsity.end();
      CppAD::vectorBool this_row(end);
      addr_t            arg_tmp[1];
      for(size_t i = 0; i < m; ++i)
      {  size_t j_var = index_y[i];
         for(size_t j = 0; j < end; ++j)
            this_row[j] = false;
         typename Vector_set::const_iterator itr_sparse(var_sparsity, j_var);
         size_t j = *itr_sparse;
         while( j < end )
         {  this_row[j] = true;
            j = *(++itr_sparse);
         }
         if( 0 < j_var )
         {  printOp<Base, RecBase>(
               std::cout,
               play,
               itr.op_index() - m + i,
               j_var,
               FunrvOp,
               arg_tmp
            );
            printOpResult(
               std::cout,
               1,
               &this_row,
               0,
               (CppAD::vectorBool *) nullptr
            );
            std::cout << std::endl;
         }
      }
   }
   return;
}
/*
------------------------------------------------------------------------------
{xrst_begin var_atomic_rev_jac dev}

Reverse Jacobian Sparsity Atomic Function Call
##############################################

Prototype
*********
{xrst_literal
   // BEGIN_ATOMIC_REV_JAC
   // END_ATOMIC_REV_JAC
}

{xrst_template ;
   include/cppad/local/var_op/template/atomic_op.xrst
   headers: n_res, i_z, itr, play, parameter, trace, work
   headers if @mode@ is reverse: G and H

   @mode@ ; reverse
}

Vector_set
**********
is the type used for vectors of sets. It must satisfy the
:ref:`SetVector-name` concept.

var_sparsity
************
On input, *var_sparsity* contains the sparsity pattern for
:math:`G [ y, x, \cdots ]` .
On output it contains the sparsity pattern for
:math:`H ( x, \cdots )` .

{xrst_end var_atomic_rev_jac}
*/
// BEGIN_ATOMIC_REV_JAC
template <class Vector_set, class Base, class RecBase>
inline void atomic_rev_jac(
   play::const_sequential_iterator& itr          ,
   const player<Base>*              play         ,
   const Base*                      parameter    ,
   bool                             trace        ,
   atomic_op_work<Base>&            work         ,
   bool                             dependency   ,
   Vector_set&                      var_sparsity )
// END_ATOMIC_REV_JAC
{  //
   //
   // vector
   using CppAD::vector;
   //
   // dyn_par_is
   const pod_vector<bool>& dyn_par_is( play->dyn_par_is() );
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
   // parameter_x, type_x, index_x, index_y
   size_t n_order = 0;
   work.resize(m, n, n_order, rev_jac_sweep);
   vector<Base>&         parameter_x( work.parameter_x );
   vector<ad_type_enum>& type_x( work.type_x );
   vector<size_t>&       index_x( work.index_x );
   vector<size_t>&       index_y( work.index_y );
   //
   // i
   for(size_t ip1 = m; ip1 > 0; --ip1)
   {  size_t i = ip1 - 1;
      //
      // op_code, arg, i_var
      (--itr).op_info(op_code, arg, i_var);
      //
      // index_y
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
         index_y[i]       = 0;  // special variable index used for parameters
         break;
         //
         // FunrvOp
         case FunrvOp:
         CPPAD_ASSERT_NARG_NRES(op_code, 0, 1);
         CPPAD_ASSERT_UNKNOWN( 0 < i_var );
         index_y[i]    = i_var;
         break;
      }
   }
   if( trace )
   {  size_t end = var_sparsity.end();
      CppAD::vectorBool this_row(end);
      const addr_t* arg_null = static_cast<addr_t*>(nullptr);
      for(size_t ip1 = m; ip1 > 0; --ip1)
      {  size_t i = ip1 - 1;
         if( index_y[i] > 0 )
         {  for(size_t j = 0; j < end; ++j)
               this_row[j] = false;
            //
            typedef typename Vector_set::const_iterator itr_sparse_t;
            itr_sparse_t itr_sparse(var_sparsity, index_y[i]);
            size_t j = *itr_sparse;
            while( j < end )
            {  this_row[j] = true;
               j = *(++itr_sparse);
            }
            printOp<Base, RecBase>(
               std::cout,
               play,
               itr.op_index() + i,
               index_y[i],
               FunrvOp,
               arg_null
            );
            printOpResult(
               std::cout,
               1,
               &this_row,
               0,
               (CppAD::vectorBool *) nullptr
            );
            std::cout << std::endl;
         }
      }
   }
   //
   // j
   for(size_t jp1 = n; jp1 > 0; --jp1)
   {  size_t j = jp1 - 1;
      //
      // op_code, arg, i_var
      (--itr).op_info(op_code, arg, i_var);
      if( trace )
      {  printOp<Base, RecBase>(
            std::cout, play, itr.op_index(), i_var, op_code, arg
         );
         std::cout << std::endl;
      }
      //
      // type_x, parameter_x, index_x
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
         parameter_x[j]  = parameter[ arg[0] ];
         index_x[j]      = 0; // special variable index used for parameters
         break;
         //
         // FunavOp
         case FunavOp:
         CPPAD_ASSERT_NARG_NRES(op_code, 1, 0);
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < play->num_var_rec() );
         type_x[j]       = variable_enum;
         parameter_x[j]  = CppAD::numeric_limits<Base>::quiet_NaN();
         index_x[j]      = size_t(arg[0]);
         break;
      }
   }
   //
   // op_code
   (--itr).op_info(op_code, arg, i_var);
   CPPAD_ASSERT_UNKNOWN( op_code == AFunOp );
   //
   // varsparsity
   sweep::call_atomic_rev_jac_sparsity<Base,RecBase>(
      atom_index,
      call_id,
      dependency,
      parameter_x,
      type_x,
      index_x,
      index_y,
      var_sparsity
   );
   return;
}
/*
------------------------------------------------------------------------------
{xrst_begin var_atomic_rev_hes dev}

Reverse Hessian Sparsity Atomic Function Call
#############################################

Prototype
*********
{xrst_literal
   // BEGIN_ATOMIC_REV_HES
   // END_ATOMIC_REV_HES
}

{xrst_template ;
   include/cppad/local/var_op/template/atomic_op.xrst
   headers: n_res, i_z, itr, play, parameter, trace, work
   headers if @mode@ is reverse: G and H

   @mode@ ; reverse
}

num_var
*******
We use the notation *num_var* for the number of variables on the tape
(including the phantom variable at index zero); i.e.::
``play->num_var_rec()`` .

for_jac_sparsity
****************
The set with index *j* is the forward Jacobian sparsity
pattern for the variable with index *j*.

rev_jac_include
***************
If the *j* element of this vector is true,
the variable with index *j* is included in the Hessian,
or affects the value of a variable that is included in the Hessian.

Input
=====
::

   for j = num_var, ... , i_z + 1
      rev_jac_include[j] is an input

Output
======
::

   for j = i_z , ... , i_z - n_res + 1
      rev_jac_include[j] is an output

rev_hes_sparsity
****************
On input (output), this is the sparsity pattern for *G* ( *H* ).
For each variable index *j* ,
*rev_hes_sparsity* [ *j* ] is the set of indices
that may have non-zero cross partials with variable index *j* .

Example
=======
If the indices in the sets correspond to the independent variables,
then *rev_hes_sparsity* ``.end()`` is the number of independent variables.
For *i* a variable index between 1 and the number of independent variables,
*i* - 1 is the corresponding independent variable index.
(The index *i* = 0 corresponds to the phantom variable at the beginning
of the tape. )

{xrst_end var_atomic_rev_hes}
*/
// BEGIN_ATOMIC_REV_HES
template <class Vector_set, class Base, class RecBase>
inline void atomic_rev_hes(
   play::const_sequential_iterator& itr               ,
   const player<Base>*              play              ,
   const Base*                      parameter         ,
   bool                             trace             ,
   atomic_op_work<Base>&            work              ,
   const Vector_set&                for_jac_sparsity  ,
   bool*                            rev_jac_include   ,
   Vector_set&                      rev_hes_sparsity  )
// END_ATOMIC_REV_HES
{  //
   //
   CPPAD_ASSERT_UNKNOWN( for_jac_sparsity.n_set() == play->num_var_rec() );
   CPPAD_ASSERT_UNKNOWN( rev_hes_sparsity.n_set() == play->num_var_rec() );
   CPPAD_ASSERT_UNKNOWN( for_jac_sparsity.end()   == rev_hes_sparsity.end() );
   //
   // vector
   using CppAD::vector;
   //
   // dyn_par_is
   const pod_vector<bool>& dyn_par_is( play->dyn_par_is() );
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
   // parameter_x, type_x, index_x, index_y
   size_t n_order = 0;
   work.resize(m, n, n_order, rev_hes_sweep);
   vector<Base>&         parameter_x( work.parameter_x );
   vector<ad_type_enum>& type_x( work.type_x );
   vector<size_t>&       index_x( work.index_x );
   vector<size_t>&       index_y( work.index_y );
   //
   // i
   for(size_t ip1 = m; ip1 > 0; --ip1)
   {  size_t i = ip1 - 1;
      //
      // op_code, arg, i_var
      (--itr).op_info(op_code, arg, i_var);
      //
      // index_y
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
         index_y[i]       = 0;  // special variable index used for parameters
         break;
         //
         // FunrvOp
         case FunrvOp:
         CPPAD_ASSERT_NARG_NRES(op_code, 0, 1);
         CPPAD_ASSERT_UNKNOWN( 0 < i_var );
         index_y[i]    = i_var;
         break;
      }
   }
   //
   // j
   for(size_t jp1 = n; jp1 > 0; --jp1)
   {  size_t j = jp1 - 1;
      //
      // op_code, arg, i_var
      (--itr).op_info(op_code, arg, i_var);
      if( trace )
      {  printOp<Base, RecBase>(
            std::cout, play, itr.op_index(), i_var, op_code, arg
         );
         std::cout << std::endl;
      }
      //
      // type_x, parameter_x, index_x
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
         parameter_x[j]  = parameter[ arg[0] ];
         index_x[j]      = 0; // special variable index used for parameters
         break;
         //
         // FunavOp
         case FunavOp:
         CPPAD_ASSERT_NARG_NRES(op_code, 1, 0);
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < play->num_var_rec() );
         type_x[j]       = variable_enum;
         parameter_x[j]  = CppAD::numeric_limits<Base>::quiet_NaN();
         index_x[j]      = size_t(arg[0]);
         break;
      }
   }
   //
   // op_code
   (--itr).op_info(op_code, arg, i_var);
   CPPAD_ASSERT_UNKNOWN( op_code == AFunOp );
   //
   // rev_jac_include, rev_hes_sparsity
   sweep::call_atomic_rev_hes_sparsity<Base,RecBase>(
      atom_index,
      call_id,
      parameter_x,
      type_x,
      index_x,
      index_y,
      for_jac_sparsity,
      rev_jac_include,
      rev_hes_sparsity
   );
   //
   if( trace )
   {  typedef typename Vector_set::const_iterator itr_sparse_t;
      size_t end = rev_hes_sparsity.end();
      CppAD::vectorBool jac_row(end), hes_row(end);
      addr_t            arg_tmp[1];
      for(size_t i = 0; i < m; ++i)
      {  size_t j_var = index_y[i];
         if( 0 < j_var)
         {
            for(size_t j = 0; j < end; ++j)
            {  jac_row[j]  = false;
               hes_row[j]  = false;
            }
            itr_sparse_t itr_jac(for_jac_sparsity, j_var);
            size_t j = *itr_jac;
            while( j < end )
            {  jac_row[j] = true;
               j = *(++itr_jac);
            }
            itr_sparse_t itr_hes(rev_hes_sparsity, j_var);
            j = *itr_hes;
            while( j < end )
            {  hes_row[j] = true;
               j = *(++itr_hes);
            }
            printOp<Base, RecBase>(
               std::cout,
               play,
               itr.op_index() - m + i,
               j_var,
               FunrvOp,
               arg_tmp
            );
            printOpResult(
               std::cout,
               1,
               &jac_row,
               1,
               &hes_row
            );
            std::cout << std::endl;
         }
      }
   }
   return;
}
/*
-------------------------------------------------------------------------------
{xrst_begin var_atomic_for_hes dev}

Forward Hessian Sparsity Atomic Function Call
#############################################

Prototype
*********
{xrst_literal
   // BEGIN_ATOMIC_FOR_HES
   // END_ATOMIC_FOR_HES
}

{xrst_template ;
   include/cppad/local/var_op/template/atomic_op.xrst
   headers: n_res, i_z, itr, play, parameter, trace, work

   @mode@ ; forward
}

Vector_set
**********
is the type used for vectors of sets. It must satisfy the
:ref:`SetVector-name` concept.

n_independent_p1
****************
is the number of independent variables (in the tape) plus one.

rev_jac_sparsity
****************
is the reverse Jacobian sparsity pattern for the scalar valued
function that the Hessian sparsity is being computed for.
Zero is the only possible element in each set;
i.e.. *ref_jac_sparsity* .end() == 1 .
If the set with index *j* is empty,
the derivative of the function w.r.t the variable with index *j* is zero.

for_hes_sparsity
****************
On input, all the linear and nonlinear interactions up to the
arguments to the atomic function have been take into account.
Upon return, the linear and nonlinear interactions in
the atomic function have been take into account.

Hessian Sparsity
================
For *j* equal 1 to *n_independent_p1* - 1,
if *i* is in set with index *j* ,
the Hessian may have a non-zero partial with respect to the
independent variables with indices ( *i* - 1, *j* - 1 ) .
Note that the index zero is not used because it corresponds to the
phantom variable on the tape.

Jacobian Sparsity
=================
If *i* is in the set with index *n_independent_p1* + *j* ,
the variable with index *j* may have a non-zero partial with resect to the
independent variable with index *i* - 1 .

{xrst_end var_atomic_for_hes}
*/
// BEGIN_ATOMIC_FOR_HES
template <class Vector_set, class Base, class RecBase>
inline void atomic_for_hes(
   play::const_sequential_iterator& itr              ,
   const player<Base>*              play             ,
   const Base*                      parameter        ,
   bool                             trace            ,
   atomic_op_work<Base>&            work             ,
   size_t                           n_independent_p1 ,
   const Vector_set&                rev_jac_sparsity ,
   Vector_set&                      for_hes_sparsity )
// END_ATOMIC_FOR_HES
{  CPPAD_ASSERT_UNKNOWN( rev_jac_sparsity.end() == 1 );
   //
   CPPAD_ASSERT_UNKNOWN( for_hes_sparsity.end() == n_independent_p1 );
   //
   // vector
   using CppAD::vector;
   //
   // dyn_par_is
   const pod_vector<bool>& dyn_par_is( play->dyn_par_is() );
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
   // parameter_x, type_x, index_x, index_y
   size_t n_order = 0;
   work.resize(m, n, n_order, for_hes_sweep);
   vector<Base>&         parameter_x( work.parameter_x );
   vector<ad_type_enum>& type_x( work.type_x );
   vector<size_t>&       index_x( work.index_x );
   vector<size_t>&       index_y( work.index_y );
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
      // type_x, parameter_x, index_x
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
         parameter_x[j]  = parameter[ arg[0] ];
         index_x[j]      = 0; // special variable index used for parameters
         break;
         //
         // FunavOp
         case FunavOp:
         CPPAD_ASSERT_NARG_NRES(op_code, 1, 0);
         CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < play->num_var_rec() );
         type_x[j]       = variable_enum;
         parameter_x[j]  = CppAD::numeric_limits<Base>::quiet_NaN();
         index_x[j]      = size_t(arg[0]);
         break;
      }
   }
   //
   // i
   for(size_t i = 0; i < m; ++i)
   {  //
      // op_code, arg, i_var
      (++itr).op_info(op_code, arg, i_var);
      //
      // index_y
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
         index_y[i]       = 0;  // special variable index used for parameters
         break;
         //
         // FunrvOp
         case FunrvOp:
         CPPAD_ASSERT_NARG_NRES(op_code, 0, 1);
         CPPAD_ASSERT_UNKNOWN( 0 < i_var );
         index_y[i]    = i_var;
         break;
      }
   }
   //
   // op_code
   (++itr).op_info(op_code, arg, i_var);
   CPPAD_ASSERT_UNKNOWN( op_code == AFunOp );
   //
   // varsparsity
   size_t num_var = play->num_var_rec();
   sweep::call_atomic_for_hes_sparsity<Base,RecBase>(
      atom_index,
      call_id,
      parameter_x,
      type_x,
      index_x,
      index_y,
      n_independent_p1,
      num_var,
      rev_jac_sparsity,
      for_hes_sparsity
   );
   //
   if( trace )
   {  typedef typename Vector_set::const_iterator itr_sparse_t;
      size_t np1 = n_independent_p1;
      CPPAD_ASSERT_UNKNOWN( np1 == for_hes_sparsity.end() );
      CppAD::vectorBool jac_row(np1);
      addr_t            arg_tmp[1];
      for(size_t i = 0; i < m; ++i)
      {  size_t j_var = index_y[i];
         if( 0 < j_var )
         {  for(size_t j = 0; j < np1; ++j)
               jac_row[j] = false;
            itr_sparse_t itr_jac(for_hes_sparsity, np1 + j_var);
            size_t j = *itr_jac;
            while( j < np1 )
            {  jac_row[j] = true;
               j = *(++itr_jac);
            }
            printOp<Base, RecBase>(
               std::cout,
               play,
               itr.op_index() - m + i,
               j_var,
               FunrvOp,
               arg_tmp
            );
            printOpResult(
               std::cout,
               1,
               &jac_row,
               0,
               (CppAD::vectorBool *) nullptr
            );
            std::cout << std::endl;
         }
      }
      CppAD::vector< CppAD::vectorBool > hes(np1);
      for(size_t i = 0; i < np1; ++i)
      {  hes[i].resize(np1);
         for(size_t j = 0; j < np1; ++j)
            hes[i][j] = false;
         itr_sparse_t itr_hes(for_hes_sparsity, i);
         size_t j = *itr_hes;
         while( j < np1 )
         {  hes[i][j] = true;
            j = *(++itr_hes);
         }
      }
      printOpResult(
         std::cout,
         np1,
         hes.data(),
         0,
         (CppAD::vectorBool *) nullptr
      );
      std::cout << std::endl;
   }
   return;
}


} } } // END_CPPAD_LOCAL_VAR_OP_NAMESPACE

# endif
