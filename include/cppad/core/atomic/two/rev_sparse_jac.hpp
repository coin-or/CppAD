# ifndef CPPAD_CORE_ATOMIC_TWO_REV_SPARSE_JAC_HPP
# define CPPAD_CORE_ATOMIC_TWO_REV_SPARSE_JAC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_two_rev_sparse_jac app}
{xrst_spell
   na
   rt
   st
}

Atomic Reverse Jacobian Sparsity Patterns
#########################################

Syntax
******
| *ok* = *afun* . ``rev_sparse_jac`` ( *q* , *rt* , *st* , *x* )

Deprecated 2016-06-27
*********************

   *ok* = *afun* . ``rev_sparse_jac`` ( *q* , *rt* , *st* )

Purpose
*******
This function is used by
:ref:`RevSparseJac-name` to compute
Jacobian sparsity patterns.
If you are using :ref:`RevSparseJac-name` ,
one of the versions of this
virtual function must be defined by the
:ref:`atomic_two_ctor@atomic_user` class.

For a fixed matrix :math:`R \in \B{R}^{q \times m}`,
the Jacobian of :math:`R * f( x )` with respect to :math:`x \in \B{R}^n` is

.. math::

   S(x) = R * f^{(1)} (x)

Given a :ref:`glossary@Sparsity Pattern` for :math:`R`,
``rev_sparse_jac`` computes a sparsity pattern for :math:`S(x)`.

Implementation
**************
If you are using
:ref:`RevSparseJac-name` or :ref:`ForSparseHes-name` ,
this virtual function must be defined by the
:ref:`atomic_two_ctor@atomic_user` class.

q
=
The argument *q* has prototype

   ``size_t`` *q*

It specifies the number of rows in
:math:`R \in \B{R}^{q \times m}` and the Jacobian
:math:`S(x) \in \B{R}^{q \times n}`.

rt
==
This argument has prototype

   ``const`` *atomic_sparsity* & *rt*

and is a
:ref:`atomic_two_option@atomic_sparsity` pattern for
:math:`R^\R{T} \in \B{R}^{m \times q}`.

st
==
This argument has prototype

   *atomic_sparsity* & *st*

The input value of its elements
are not specified (must not matter).
Upon return, *s* is a
:ref:`atomic_two_option@atomic_sparsity` pattern for
:math:`S(x)^\R{T} \in \B{R}^{n \times q}`.

x
=
The argument has prototype

   ``const CppAD::vector<`` *Base* >& *x*

and size is equal to the *n* .
This is the :ref:`Value-name` corresponding to the parameters in the
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

{xrst_end atomic_two_rev_sparse_jac}
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/two_rev_sparse_jac.hpp
Atomic reverse mode Jacobian sparsity patterns.
*/
/*!
Link, after case split, from rev_jac_sweep to atomic_base

\param q [in]
is the row dimension for the Jacobian sparsity partterns

\param rt [out]
is the tansposed Jacobian sparsity pattern w.r.t to range variables y

\param st [in]
is the tansposed Jacobian sparsity pattern for the argument variables x

\param x
is the integer value for x arguments that are parameters.
*/
template <class Base>
bool atomic_base<Base>::rev_sparse_jac(
   size_t                                  q  ,
   const vector< std::set<size_t> >&       rt ,
          vector< std::set<size_t> >&       st ,
   const vector<Base>&                     x  )
{  return false; }
template <class Base>
bool atomic_base<Base>::rev_sparse_jac(
   size_t                                  q  ,
   const vector<bool>&                     rt ,
          vector<bool>&                     st ,
   const vector<Base>&                     x  )
{  return false; }
template <class Base>
bool atomic_base<Base>::rev_sparse_jac(
   size_t                                  q  ,
   const vectorBool&                       rt ,
          vectorBool&                       st ,
   const vector<Base>&                     x  )
{  return false; }
// deprecated versions
template <class Base>
bool atomic_base<Base>::rev_sparse_jac(
   size_t                                  q  ,
   const vector< std::set<size_t> >&       rt ,
          vector< std::set<size_t> >&       st )
{  return false; }
template <class Base>
bool atomic_base<Base>::rev_sparse_jac(
   size_t                                  q  ,
   const vector<bool>&                     rt ,
          vector<bool>&                     st )
{  return false; }
template <class Base>
bool atomic_base<Base>::rev_sparse_jac(
   size_t                                  q  ,
   const vectorBool&                       rt ,
          vectorBool&                       st )
{  return false; }

/*!
Link, before case split, from rev_jac_sweep to atomic_base.

\tparam InternalSparsity
Is the used internaly for sparsity calculations; i.e.,
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
On input, for i = 0, ... , m-1, the sparsity pattern with index y_index[i],
is the sparsity for the i-th argument to this atomic function.
On output, for j = 0, ... , n-1, the sparsity pattern with index x_index[j],
the sparsity has been updated to remove y as a function of x.
*/
template <class Base>
template <class InternalSparsity>
bool atomic_base<Base>::rev_sparse_jac(
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
   // initial results may be non-empty during reverse mode
   size_t q           = var_sparsity.end();
   bool   input_empty = false;
   bool   zero_empty  = true;
   bool   transpose   = false;
   size_t n           = pod_x_index.size();
   bool   ok          = false;
   size_t thread      = thread_alloc::thread_num();
   allocate_work(thread);
   //
   std::string msg    = ": atomic_base.rev_sparse_jac: returned false";
   if( sparsity_ == pack_sparsity_enum )
   {  vectorBool& pack_rt ( work_[thread]->pack_r );
      vectorBool& pack_st ( work_[thread]->pack_s );
      local::sparse::get_internal_pattern(
         transpose, pod_y_index, var_sparsity, pack_rt
      );
      //
      pack_st.resize(n * q );
      ok = rev_sparse_jac(q, pack_rt, pack_st, x);
      if( ! ok )
         ok = rev_sparse_jac(q, pack_rt, pack_st);
      if( ! ok )
      {  msg = atomic_name() + msg + " sparsity = pack_sparsity_enum";
         CPPAD_ASSERT_KNOWN(false, msg.c_str());
      }
      local::sparse::set_internal_pattern(zero_empty, input_empty,
         transpose, pod_x_index, var_sparsity, pack_st
      );
   }
   else if( sparsity_ == bool_sparsity_enum )
   {  vector<bool>& bool_rt ( work_[thread]->bool_r );
      vector<bool>& bool_st ( work_[thread]->bool_s );
      local::sparse::get_internal_pattern(
         transpose, pod_y_index, var_sparsity, bool_rt
      );
      bool_st.resize(n * q );
      ok = rev_sparse_jac(q, bool_rt, bool_st, x);
      if( ! ok )
         ok = rev_sparse_jac(q, bool_rt, bool_st);
      if( ! ok )
      {  msg = atomic_name() + msg + " sparsity = bool_sparsity_enum";
         CPPAD_ASSERT_KNOWN(false, msg.c_str());
      }
      local::sparse::set_internal_pattern(zero_empty, input_empty,
         transpose, pod_x_index, var_sparsity, bool_st
      );
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( sparsity_ == set_sparsity_enum );
      vector< std::set<size_t> >& set_rt ( work_[thread]->set_r );
      vector< std::set<size_t> >& set_st ( work_[thread]->set_s );
      local::sparse::get_internal_pattern(
         transpose, pod_y_index, var_sparsity, set_rt
      );
      set_st.resize(n);
      ok = rev_sparse_jac(q, set_rt, set_st, x);
      if( ! ok )
         ok = rev_sparse_jac(q, set_rt, set_st);
      if( ! ok )
      {  msg = atomic_name() + msg + " sparsity = set_sparsity_enum";
         CPPAD_ASSERT_KNOWN(false, msg.c_str());
      }
      local::sparse::set_internal_pattern(zero_empty, input_empty,
         transpose, pod_x_index, var_sparsity, set_st
      );
   }
   return ok;
}

} // END_CPPAD_NAMESPACE
# endif
