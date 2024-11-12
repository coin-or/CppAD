# ifndef CPPAD_CORE_NUM_SKIP_HPP
# define CPPAD_CORE_NUM_SKIP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin number_skip}

Number of Variables that Can be Skipped
#######################################

Syntax
******
| *n* = *f* . ``number_skip`` ()

See Also
========
:ref:`fun_property-name`

Purpose
*******
The :ref:`conditional expressions<CondExp-name>` use either the
:ref:`if_true<CondExp-name>` or :ref:`if_false<CondExp-name>` .
Hence, some terms only need to be evaluated
depending on the value of the comparison in the conditional expression.
The :ref:`optimize-name` option is capable of detecting some of these
case and determining variables that can be skipped.
This routine returns the number such variables.

n
*
The return value *n* has type ``size_t``
is the number of variables that the optimizer has determined can be skipped
(given the independent variable values specified by the previous call to
:ref:`f.Forward<Forward-name>` for order zero).

f
*
The object *f* has prototype

   ``ADFun`` < *Base* > *f*

{xrst_toc_hidden
   example/general/number_skip.cpp
}
Example
*******
The file :ref:`number_skip.cpp-name`
contains an example and test of this function.

{xrst_end number_skip}
-----------------------------------------------------------------------------
*/

# include <cppad/local/play/atom_op_info.hpp>

// BEGIN CppAD namespace
namespace CppAD {

// This routine is not const because it runs through the operations sequence
// 2DO: compute this value during zero order forward operations.
template <class Base, class RecBase>
size_t ADFun<Base,RecBase>::number_skip(void)
{  // must pass through operation sequence to map operations to variables

   // information defined by atomic forward
   size_t atom_index=0, atom_old=0, atom_m=0, atom_n=0;

   // number of variables skipped
   size_t num_var_skip = 0;

   // start playback
   local::play::const_sequential_iterator itr = play_.begin();
   local::op_code_var op;
   size_t        i_var;
   const addr_t* arg;
   itr.op_info(op, arg, i_var);
   CPPAD_ASSERT_UNKNOWN(op == local::BeginOp)
   while(op != local::EndOp)
   {  // next op
      (++itr).op_info(op, arg, i_var);
      //
      if( op == local::AFunOp )
      {  // skip only appears at front or back AFunOp of atomic function call
         bool skip_call = cskip_op_[ itr.op_index() ];
         local::play::atom_op_info<Base>(
            op, arg, atom_index, atom_old, atom_m, atom_n
         );
         CPPAD_ASSERT_UNKNOWN( NumRes(op) == 0 );
         size_t num_op = atom_m + atom_n + 1;
         for(size_t i = 0; i < num_op; i++)
         {  CPPAD_ASSERT_UNKNOWN(
               op != local::CSkipOp && op != local::CSumOp
            );
            (++itr).op_info(op, arg, i_var);
            if( skip_call )
               num_var_skip += NumRes(op);
         }
         CPPAD_ASSERT_UNKNOWN( op == local::AFunOp );
      }
      else
      {  if( cskip_op_[ itr.op_index() ] )
            num_var_skip += NumRes(op);
         //
         if( (op == local::CSkipOp) || (op == local::CSumOp) )
            itr.correct_before_increment();
      }
   }
   return num_var_skip;
}

} // END CppAD namespace


# endif
