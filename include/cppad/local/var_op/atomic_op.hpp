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
   nv
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

nv
**
We use *nv* to denote the number of variables created by this function call.
This is equal to the number of FunrvOp operators in function call.

i_z
***
We use *i_z* to denote the index of the last variable
created by this function call.


{xrst_end var_atomic_op}
-------------------------------------------------------------------------------
{xrst_begin var_atomic_forward_op dev}
{xrst_spell
   nv
}

Any Order Forward Atomic Function Call
######################################

Prototype
*********
{xrst_literal
   // BEGIN_ATOMIC_FORWARD_OP
   // END_ATOMIC_FORWARD_OP
}


nv
**
see
:ref:`var_atomic_op@nv`


itr
***
is an iterator for the recording in *play* .
On input (output), the operator corresponding to *itr* is the first (second)
:ref:`var_atomic_op@AfunOp` for this function call.

taylor
******

Input
=====
::

   for j = 0, ..., i_z - nv
      for k = 0 , ... , order_up
         taylor [ j * cap_order + k ] is an input

   for j = i_z , ... , i_z - nv + 1
      for  k = 0 , ... , order_up - 1
         taylor [ j * cap_order + k ] is an input

Output
======
::

   for j = i_z , ... , i_z - nv + 1
      for k = order_low , ... , order_up
         taylor [ i_z * cap_order + k ] is an output

play
****
is a player for the recording that contains the operation sequence
that contains this atomic function call.

parameter
*********
is the parameter vector for this operation sequence.

cap_order
*********
is the number of orders that can fit in *taylor* .

order_low
*********
lowest order of the Taylor coefficient that we are computing.

order_up
********
highest order of the Taylor coefficient that we are computing.

trace
*****
if *trace* is true (false) a trace of the evaluation of this
atomic function call is (is not) printed.

work
****
is unspecified work space.
It passed as an argument to reduce memory allocations.

{xrst_end var_atomic_forward_op}
*/
enum sweep_type {
   forward_op_sweep,
   forward_dir_sweep,
   reverse_op_sweep
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
   CppAD::vector<size_t>         index_x;
   CppAD::vector<size_t>         index_y;
   //
   CppAD::vector<bool>           variable_x;
   CppAD::vector<bool>           variable_y;
   //
   // resize
   void resize(size_t m, size_t n, size_t n_order, sweep_type sweep)
   {
      // parameter_x, type_x
      parameter_x.resize(n);
      type_x.resize(n);
      //
      // taylor_x, taylor_y
      taylor_x.resize(n * n_order);
      taylor_y.resize(m * n_order);
      //
      // index_x
      if( sweep == forward_dir_sweep || sweep == reverse_op_sweep )
         index_x.resize(n);
      else
         index_x.resize(0);
      //
      // index_y
      if( sweep == forward_op_sweep || sweep == forward_dir_sweep )
         index_y.resize(m);
      else
         index_y.resize(0);
      //
      // variable_x, variable_y
      if( sweep == reverse_op_sweep )
      {  variable_x.resize(n);
         variable_y.resize(0);
      }
      else
      {  variable_x.resize(0);
         variable_y.resize(m);
      }
   }
};


// BEGIN_ATOMIC_FORWARD_OP
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
// END_ATOMIC_FORWARD_OP
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
         // FunavOp
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
{xrst_spell
   nv
}

Multiple Direction Forward Atomic Function Call
###############################################

Prototype
*********
{xrst_literal
   // BEGIN_ATOMIC_FORWARD_DIR
   // END_ATOMIC_FORWARD_DIR
}


nv, i_z
*******
see
:ref:`var_atomic_op@nv` ,
:ref:`var_atomic_op@i_z`


itr
***
see atomic_forward_op :ref:`var_atomic_forward_op@itr`

play
****
see atomic_forward_op :ref:`var_atomic_forward_op@play`

parameter
*********
see atomic_forward_op :ref:`var_atomic_forward_op@parameter`

cap_order
*********
see atomic_forward_op :ref:`var_atomic_forward_op@cap_order`

order_low
*********
see atomic_forward_op :ref:`var_atomic_forward_op@order_low`

order_up
********
see atomic_forward_op :ref:`var_atomic_forward_op@order_up`

trace
*****
see atomic_forward_op :ref:`var_atomic_forward_op@trace`

work
****
see atomic_forward_op :ref:`var_atomic_forward_op@work`

n_dir
*****
is the number of directions we are computing during this
forward mode pass.
If *n_dir* is one, The inputs and outputs for ``atomic_forward_dir``
are the same as for ``atomic_forward_op`` with out the *n_dir* argument.


taylor
******

per_variable
============
For each variable there is one Taylor coefficient of order zero
and *n_dir* coefficients for orders greater than zero.
The taylor coefficients capacity per variable is::

   per_variable = (cap_order - 1) * n_dir + 1

(j, k, ell)
===========
For variable index j, order k, and direction index ell::

   if k == 0
      (j, k, ell) = j * per_variable
   else
      (j, k, ell) = j * per_variable + (k-1) * n_dir + 1 + ell

The value taylor[ (j, k, ell) ] is the
Taylor coefficient corresponding to
the variable with index j, the order k, and the direction with index ell.

n_dir = 1
=========
If *n_dir* is equal to one then *ell* is zero and::

   (j, k, ell) = j * cap_order + k


Input
=====
::

   for j = 0, ..., i_z - nv,
      for k = 0 , ... , order_up
         for ell = 0 , ... , n_dir - 1
            taylor [ (j, k, ell) ] is an input

   for j = i_z, ... , i_z - nv + 1
      for k = 0 , ... , order_up - 1
         for ell = 0 , ... , n_dir - 1
            taylor [ (j, k, ell) ] is an input

Output
======
::

   for j = i_z, ... , i_z - nv + 1
      for ell = 0, ... , n_dir - 1
         taylor [ (j, order_up, ell) ] is an output

{xrst_end var_atomic_forward_dir}
*/
// BEGIN_ATOMIC_FORWARD_DIR
template <class Base, class RecBase>
void atomic_forward_dir(
   play::const_sequential_iterator& itr        ,
   Base*                            taylor     ,
   const player<Base>*              play       ,
   const Base*                      parameter  ,
   size_t                           cap_order  ,
   size_t                           order_low  ,
   size_t                           order_up   ,
   size_t                           n_dir      ,
   bool                             trace      ,
   atomic_op_work<Base>&            work       )
// END_ATOMIC_FORWARD_DIR
{  //
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
         // FunavOp
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

} } } // END_CPPAD_LOCAL_VAR_OP_NAMESPACE

# endif
