# ifndef CPPAD_CORE_ATOMIC_TWO_FOR_SPARSE_JAC_HPP
# define CPPAD_CORE_ATOMIC_TWO_FOR_SPARSE_JAC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_two_for_sparse_jac app}
{xrst_spell
   na
}

Atomic Forward Jacobian Sparsity Patterns
#########################################

Syntax
******
| *ok* = *afun* . ``for_sparse_jac`` ( *q* , *r* , *s* , *x* )

Deprecated 2016-06-27
*********************

   *ok* = *afun* . ``for_sparse_jac`` ( *q* , *r* , *s* )

Purpose
*******
This function is used by :ref:`ForSparseJac-name` to compute
Jacobian sparsity patterns.
For a fixed matrix :math:`R \in \B{R}^{n \times q}`,
the Jacobian of :math:`f( x + R * u)` with respect to :math:`u \in \B{R}^q` is

.. math::

   S(x) = f^{(1)} (x) * R

Given a :ref:`glossary@Sparsity Pattern` for :math:`R`,
``for_sparse_jac`` computes a sparsity pattern for :math:`S(x)`.

Implementation
**************
If you are using
:ref:`ForSparseJac-name` ,
:ref:`ForSparseHes-name` , or
:ref:`RevSparseHes-name` ,
one of the versions of this
virtual function must be defined by the
:ref:`atomic_two_ctor@atomic_user` class.

q
=
The argument *q* has prototype

   ``size_t`` *q*

It specifies the number of columns in
:math:`R \in \B{R}^{n \times q}` and the Jacobian
:math:`S(x) \in \B{R}^{m \times q}`.

r
=
This argument has prototype

   ``const`` *atomic_sparsity* & *r*

and is a :ref:`atomic_two_option@atomic_sparsity` pattern for
:math:`R \in \B{R}^{n \times q}`.

s
=
This argument has prototype

   *atomic_sparsity* & *s*

The input values of its elements
are not specified (must not matter).
Upon return, *s* is a
:ref:`atomic_two_option@atomic_sparsity` pattern for
:math:`S(x) \in \B{R}^{m \times q}`.

x
=
The argument has prototype

   ``const CppAD::vector<`` *Base* >& *x*

and size is equal to the *n* .
This is the :ref:`Value-name`  corresponding to the parameters in the
vector :ref:`atomic_two_afun@ax` (when the atomic function was called).
To be specific, if

| |tab| ``if`` ( ``Parameter`` ( *ax* [ *i* ]) == ``true`` )
| |tab| |tab| *x* [ *i* ] = ``Value`` ( *ax* [ *i* ] );
| |tab| ``else``
| |tab| |tab| *x* [ *i* ] = ``CppAD::numeric_limits<`` *Base* >:: ``quiet_NaN`` ();

The version of this function with out the *x* argument is deprecated;
i.e., you should include the argument even if you do not use it.

ok
**
The return value *ok* has prototype

   ``bool`` *ok*

If it is ``true`` , the corresponding evaluation succeeded,
otherwise it failed.

{xrst_end atomic_two_for_sparse_jac}
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/two_for_sparse_jac.hpp
Atomic forward Jacobian sparsity pattern.
*/
/*!
Link, after case split, from for_jac_sweep to atomic_base.

\param q
is the column dimension for the Jacobian sparsity partterns.

\param r
is the Jacobian sparsity pattern for the argument vector x

\param s
is the Jacobian sparsity pattern for the result vector y

\param x
is the integer value for x arguments that are parameters.
*/
template <class Base>
bool atomic_base<Base>::for_sparse_jac(
   size_t                                  q  ,
   const vector< std::set<size_t> >&       r  ,
          vector< std::set<size_t> >&       s  ,
   const vector<Base>&                     x  )
{  return false; }
template <class Base>
bool atomic_base<Base>::for_sparse_jac(
   size_t                                  q  ,
   const vector<bool>&                     r  ,
          vector<bool>&                     s  ,
   const vector<Base>&                     x  )
{  return false; }
template <class Base>
bool atomic_base<Base>::for_sparse_jac(
   size_t                                  q  ,
   const vectorBool&                       r  ,
          vectorBool&                       s  ,
   const vector<Base>&                     x  )
{  return false; }
// deprecated versions
template <class Base>
bool atomic_base<Base>::for_sparse_jac(
   size_t                                  q  ,
   const vector< std::set<size_t> >&       r  ,
          vector< std::set<size_t> >&       s  )
{  return false; }
template <class Base>
bool atomic_base<Base>::for_sparse_jac(
   size_t                                  q  ,
   const vector<bool>&                     r  ,
          vector<bool>&                     s  )
{  return false; }
template <class Base>
bool atomic_base<Base>::for_sparse_jac(
   size_t                                  q  ,
   const vectorBool&                       r  ,
          vectorBool&                       s  )
{  return false; }

/*!
Link, before case split, from for_jac_sweep to atomic_base.

\tparam InternalSparsity
Is the type used for internal sparsity calculations; i.e.,
sparse_pack or sparse_list.

\param x
is parameter arguments to the function, other components are nan.

\param x_index
is the variable index, on the tape, for the arguments to this function.
This size of x_index is n, the number of arguments to this function.

\param y_index
is the variable index, on the tape, for the results for this function.
This size of y_index is m, the number of results for this function.

\param var_sparsity
On input, for j = 0, ... , n-1, the sparsity pattern with index x_index[j],
is the sparsity for the j-th argument to this atomic function.
On output, for i = 0, ... , m-1, the sparsity pattern with index y_index[i],
is the sparsity for the i-th result for this atomic function.
*/
template <class Base>
template <class InternalSparsity>
bool atomic_base<Base>::for_sparse_jac(
   const vector<Base>&              x            ,
   const vector<size_t>&            x_index      ,
   const vector<size_t>&            y_index      ,
   InternalSparsity&                var_sparsity )
{  //
   // pod_x_index, pod_y_index
   local::pod_vector<size_t> pod_x_index( x_index.size() );
   local::pod_vector<size_t> pod_y_index( y_index.size() );
   for(size_t j = 0; j < x_index.size(); ++j)
      pod_x_index[j] = x_index[j];
   for(size_t i = 0; i < y_index.size(); ++i)
      pod_y_index[i] = y_index[i];
   //
   // intial results are empty during forward mode
   size_t q           = var_sparsity.end();
   bool   input_empty = true;
   bool   zero_empty  = true;
   bool   transpose   = false;
   size_t m           = pod_y_index.size();
   bool   ok          = false;
   size_t thread      = thread_alloc::thread_num();
   allocate_work(thread);
   //
   std::string msg    = ": atomic_base.for_sparse_jac: returned false";
   if( sparsity_ == pack_sparsity_enum )
   {  vectorBool& pack_r ( work_[thread]->pack_r );
      vectorBool& pack_s ( work_[thread]->pack_s );
      local::sparse::get_internal_pattern(
         transpose, pod_x_index, var_sparsity, pack_r
      );
      //
      pack_s.resize(m * q );
      ok = for_sparse_jac(q, pack_r, pack_s, x);
      if( ! ok )
         ok = for_sparse_jac(q, pack_r, pack_s);
      if( ! ok )
      {  msg = atomic_name() + msg + " sparsity = pack_sparsity_enum";
         CPPAD_ASSERT_KNOWN(false, msg.c_str());
      }
      local::sparse::set_internal_pattern(zero_empty, input_empty,
         transpose, pod_y_index, var_sparsity, pack_s
      );
   }
   else if( sparsity_ == bool_sparsity_enum )
   {  vector<bool>& bool_r ( work_[thread]->bool_r );
      vector<bool>& bool_s ( work_[thread]->bool_s );
      local::sparse::get_internal_pattern(
         transpose, pod_x_index, var_sparsity, bool_r
      );
      bool_s.resize(m * q );
      ok = for_sparse_jac(q, bool_r, bool_s, x);
      if( ! ok )
         ok = for_sparse_jac(q, bool_r, bool_s);
      if( ! ok )
      {  msg = atomic_name() + msg + " sparsity = bool_sparsity_enum";
         CPPAD_ASSERT_KNOWN(false, msg.c_str());
      }
      local::sparse::set_internal_pattern(zero_empty, input_empty,
         transpose, pod_y_index, var_sparsity, bool_s
      );
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( sparsity_ == set_sparsity_enum );
      vector< std::set<size_t> >& set_r ( work_[thread]->set_r );
      vector< std::set<size_t> >& set_s ( work_[thread]->set_s );
      local::sparse::get_internal_pattern(
         transpose, pod_x_index, var_sparsity, set_r
      );
      //
      set_s.resize(m);
      ok = for_sparse_jac(q, set_r, set_s, x);
      if( ! ok )
         ok = for_sparse_jac(q, set_r, set_s);
      if( ! ok )
      {  msg = atomic_name() + msg + " sparsity = set_sparsity_enum";
         CPPAD_ASSERT_KNOWN(false, msg.c_str());
      }
      local::sparse::set_internal_pattern(zero_empty, input_empty,
         transpose, pod_y_index, var_sparsity, set_s
      );
   }
   return ok;
}

} // END_CPPAD_NAMESPACE
# endif
