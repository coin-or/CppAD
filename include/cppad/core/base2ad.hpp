# ifndef CPPAD_CORE_BASE2AD_HPP
# define CPPAD_CORE_BASE2AD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin base2ad}

Create an AD<Base> Function From a Base Function
################################################

Syntax
******
| *af* = *f* . ``base2ad`` ()

See Also
********
:ref:`mul_level-name`

Base
****
This is the base type used to recorded the operation sequence in *f*
and *af* ; i.e., the type ``AD`` < *Base* > was used to record
the operation sequence.

f
*
This object has prototype

   ``ADFun`` < *Base* > *f*

It does it's derivative calculations using the type *Base* .

af
**
This object has prototype

   ``ADFun< AD<`` *Base* > , *Base* > *af*

It has the same operation sequence as *f* ,
but it does it's derivative calculations using the type
``AD`` < *Base>* .
This enables one to record new functions that are defined
using derivatives of the function *f* .
Initially, there are no Taylor coefficients stored in *af* and
:ref:`af.size_order()<size_order-name>` is zero.
{xrst_toc_hidden
   example/general/base2ad.cpp
   example/general/base2vec_ad.cpp
}
Example
*******
The file :ref:`base2ad.cpp-name`
contains an example and test of this operation.

VecAD
*****
Forward mode on a ``base2ad`` function does not preserve
:ref:`VecAD-name` operations (which might be expected); see the
:ref:`base2vec_ad.cpp-name` example.

{xrst_end base2ad}
----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file base2ad.hpp
*/
/// Create an ADFun< AD<Base>, Base > from this ADFun<Base>
template <class Base, class RecBase>
ADFun< AD<Base>, RecBase > ADFun<Base,RecBase>::base2ad(void) const
{  ADFun< AD<Base>, RecBase > fun;
   //
   // bool values
   fun.has_been_optimized_        = has_been_optimized_;
   fun.check_for_nan_             = check_for_nan_;
   //
   // size_t values
   fun.compare_change_count_      = compare_change_count_;
   fun.compare_change_number_     = compare_change_number_;
   fun.compare_change_op_index_   = compare_change_op_index_;
   CPPAD_ASSERT_UNKNOWN( fun.num_order_taylor_ == 0 ) ;
   CPPAD_ASSERT_UNKNOWN( fun.cap_order_taylor_ == 0 );
   CPPAD_ASSERT_UNKNOWN( fun.num_direction_taylor_ == 0 );
   fun.num_var_tape_              = num_var_tape_;
   //
   // pod_vector objects
   fun.ind_taddr_                 = ind_taddr_;
   fun.dep_taddr_                 = dep_taddr_;
   fun.dep_parameter_             = dep_parameter_;
   fun.cskip_op_                  = cskip_op_;
   fun.load_op2var_               = load_op2var_;
   //
   // pod_maybe_vector< AD<Base> > = pod_maybe_vector<Base>
   CPPAD_ASSERT_UNKNOWN( fun.taylor_.size() == 0 );
   //
   // player
   // (uses move semantics)
   fun.play_ = play_.base2ad();
   //
   // subgraph
   fun.subgraph_info_ = subgraph_info_;
   //
   // sparse_pack
   fun.for_jac_sparse_pack_ = for_jac_sparse_pack_;
   //
   // sparse_list
   fun.for_jac_sparse_set_  = for_jac_sparse_set_;
   //
   return fun;
}

} // END_CPPAD_NAMESPACE
# endif
