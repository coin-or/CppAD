# ifndef CPPAD_CORE_ATOMIC_TWO_REV_SPARSE_HES_HPP
# define CPPAD_CORE_ATOMIC_TWO_REV_SPARSE_HES_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_two_rev_sparse_hes app}
{xrst_spell
   na
   vx
}

Atomic Reverse Hessian Sparsity Patterns
########################################

Syntax
******
| *ok* = *afun* . ``rev_sparse_hes`` ( *vx* , *s* , *t* , *q* , *r* , *u* , *v* , *x* )

Deprecated 2016-06-27
*********************
*ok* = *afun* . ``rev_sparse_hes`` ( *vx* , *s* , *t* , *q* , *r* , *u* , *v* )

Purpose
*******
This function is used by :ref:`RevSparseHes-name` to compute
Hessian sparsity patterns.
If you are using :ref:`RevSparseHes-name` to compute
one of the versions of this
virtual function muse be defined by the
:ref:`atomic_two_ctor@atomic_user` class.

There is an unspecified scalar valued function
:math:`g : \B{R}^m \rightarrow \B{R}`.
Given a :ref:`glossary@Sparsity Pattern` for
:math:`R \in \B{R}^{n \times q}`,
and information about the function :math:`z = g(y)`,
this routine computes the sparsity pattern for

.. math::

   V(x) = (g \circ f)^{(2)}( x ) R

Implementation
**************
If you are using and :ref:`RevSparseHes-name` ,
this virtual function must be defined by the
:ref:`atomic_two_ctor@atomic_user` class.

vx
==
The argument *vx* has prototype

   ``const CppAD:vector<bool>&`` *vx*

*vx* . ``size`` () == *n* , and
for :math:`j = 0 , \ldots , n-1`,
*vx* [ *j* ] is true if and only if
*ax* [ *j* ] is a :ref:`glossary@Variable`
or :ref:`dynamic parameter<glossary@Parameter@Dynamic>`
in the corresponding call to

   *afun* ( *ax* , *ay* )

s
=
The argument *s* has prototype

   ``const CppAD:vector<bool>&`` *s*

and its size is *m* .
It is a sparsity pattern for
:math:`S(x) = g^{(1)} [ f(x) ] \in \B{R}^{1 \times m}`.

t
=
This argument has prototype

   ``CppAD:vector<bool>&`` *t*

and its size is *m* .
The input values of its elements
are not specified (must not matter).
Upon return, *t* is a
sparsity pattern for
:math:`T(x) \in \B{R}^{1 \times n}` where

.. math::

   T(x) = (g \circ f)^{(1)} (x) = S(x) * f^{(1)} (x)

q
=
The argument *q* has prototype

   ``size_t`` *q*

It specifies the number of columns in
:math:`R \in \B{R}^{n \times q}`,
:math:`U(x) \in \B{R}^{m \times q}`, and
:math:`V(x) \in \B{R}^{n \times q}`.

r
=
This argument has prototype

   ``const`` *atomic_sparsity* & *r*

and is a :ref:`atomic_two_option@atomic_sparsity` pattern for
:math:`R \in \B{R}^{n \times q}`.

u
*
This argument has prototype

   ``const`` *atomic_sparsity* & *u*

and is a :ref:`atomic_two_option@atomic_sparsity` pattern for
:math:`U(x) \in \B{R}^{m \times q}` which is defined by

.. math::
   :nowrap:

   \begin{eqnarray}
   U(x)
   & = &
   \{ \partial_u \{ \partial_y g[ y + f^{(1)} (x) R u ] \}_{y=f(x)} \}_{u=0}
   \\
   & = &
   \partial_u \{ g^{(1)} [ f(x) + f^{(1)} (x) R u ] \}_{u=0}
   \\
   & = &
   g^{(2)} [ f(x) ] f^{(1)} (x) R
   \end{eqnarray}

v
=
This argument has prototype

   *atomic_sparsity* & *v*

The input value of its elements
are not specified (must not matter).
Upon return, *v* is a
:ref:`atomic_two_option@atomic_sparsity` pattern for
:math:`V(x) \in \B{R}^{n \times q}` which is defined by

.. math::
   :nowrap:

   \begin{eqnarray}
   V(x)
   & = &
   \partial_u [ \partial_x (g \circ f) ( x + R u )  ]_{u=0}
   \\
   & = &
   \partial_u [ (g \circ f)^{(1)}( x + R u )  ]_{u=0}
   \\
   & = &
   (g \circ f)^{(2)}( x ) R
   \\
   & = &
   f^{(1)} (x)^\R{T} g^{(2)} [ f(x) ] f^{(1)} (x)  R
   +
   \sum_{i=1}^m g_i^{(1)} [ f(x) ] \; f_i^{(2)} (x) R
   \\
   & = &
   f^{(1)} (x)^\R{T} U(x)
   +
   \sum_{i=1}^m S_i (x) \; f_i^{(2)} (x) R
   \end{eqnarray}

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

{xrst_end atomic_two_rev_sparse_hes}
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/two_rev_sparse_hes.hpp
Atomic reverse mode Hessian sparsity patterns.
*/
/*!
Link from reverse Hessian sparsity sweep to atomic_base

\param vx [in]
which componens of x are variables.

\param s [in]
is the reverse Jacobian sparsity pattern w.r.t the result vector y.

\param t [out]
is the reverse Jacobian sparsity pattern w.r.t the argument vector x.

\param q [in]
is the column dimension for the sparsity partterns.

\param r [in]
is the forward Jacobian sparsity pattern w.r.t the argument vector x

\param u [in]
is the Hessian sparsity pattern w.r.t the result vector y.

\param v [out]
is the Hessian sparsity pattern w.r.t the argument vector x.

\param x [in]
is the integer value of the x arguments that are parameters.
*/
template <class Base>
bool atomic_base<Base>::rev_sparse_hes(
   const vector<bool>&                     vx ,
   const vector<bool>&                     s  ,
          vector<bool>&                     t  ,
   size_t                                  q  ,
   const vector< std::set<size_t> >&       r  ,
   const vector< std::set<size_t> >&       u  ,
          vector< std::set<size_t> >&       v  ,
   const vector<Base>&                     x  )
{  return false; }
template <class Base>
bool atomic_base<Base>::rev_sparse_hes(
   const vector<bool>&                     vx ,
   const vector<bool>&                     s  ,
          vector<bool>&                     t  ,
   size_t                                  q  ,
   const vector<bool>&                     r  ,
   const vector<bool>&                     u  ,
          vector<bool>&                     v  ,
   const vector<Base>&                     x  )
{  return false; }
template <class Base>
bool atomic_base<Base>::rev_sparse_hes(
   const vector<bool>&                     vx ,
   const vector<bool>&                     s  ,
          vector<bool>&                     t  ,
   size_t                                  q  ,
   const vectorBool&                       r  ,
   const vectorBool&                       u  ,
          vectorBool&                       v  ,
   const vector<Base>&                     x  )
{  return false; }
// deprecated
template <class Base>
bool atomic_base<Base>::rev_sparse_hes(
   const vector<bool>&                     vx ,
   const vector<bool>&                     s  ,
          vector<bool>&                     t  ,
   size_t                                  q  ,
   const vector< std::set<size_t> >&       r  ,
   const vector< std::set<size_t> >&       u  ,
          vector< std::set<size_t> >&       v  )
{  return false; }
template <class Base>
bool atomic_base<Base>::rev_sparse_hes(
   const vector<bool>&                     vx ,
   const vector<bool>&                     s  ,
          vector<bool>&                     t  ,
   size_t                                  q  ,
   const vector<bool>&                     r  ,
   const vector<bool>&                     u  ,
          vector<bool>&                     v  )
{  return false; }
template <class Base>
bool atomic_base<Base>::rev_sparse_hes(
   const vector<bool>&                     vx ,
   const vector<bool>&                     s  ,
          vector<bool>&                     t  ,
   size_t                                  q  ,
   const vectorBool&                       r  ,
   const vectorBool&                       u  ,
          vectorBool&                       v  )
{  return false; }
/*!
Link, before case split, from rev_hes_sweep to atomic_base.

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

\param for_jac_sparsity
On input, for j = 0, ... , n-1, the sparsity pattern with index x_index[j],
is the forward Jacobian sparsity for the j-th argument to this atomic function.

\param rev_jac_flag
This shows which variables affect the function we are
computing the Hessian of.
On input, for i = 0, ... , m-1, the rev_jac_flag[ y_index[i] ] is true
if the Jacobian of function (we are computing sparsity for) is no-zero.
Upon return, for j = 0, ... , n-1, rev_jac_flag [ x_index[j] ]
as been adjusted to accound removing this atomic function.

\param rev_hes_sparsity
This is the sparsity pattern for the Hessian.
On input, for i = 0, ... , m-1, row y_index[i] is the reverse Hessian sparsity
with one of the partials with respect to to y_index[i].
*/
template <class Base>
template <class InternalSparsity>
bool atomic_base<Base>::rev_sparse_hes(
   const vector<Base>&              x                ,
   const vector<size_t>&            x_index          ,
   const vector<size_t>&            y_index          ,
   const InternalSparsity&          for_jac_sparsity ,
   bool*                            rev_jac_flag     ,
   InternalSparsity&                rev_hes_sparsity )
{  CPPAD_ASSERT_UNKNOWN( for_jac_sparsity.end() == rev_hes_sparsity.end() );
   //
   // pod_x_index, pod_y_index
   local::pod_vector<size_t> pod_x_index( x_index.size() );
   local::pod_vector<size_t> pod_y_index( y_index.size() );
   for(size_t j = 0; j < x_index.size(); ++j)
      pod_x_index[j] = x_index[j];
   for(size_t i = 0; i < y_index.size(); ++i)
      pod_y_index[i] = y_index[i];
   //
   size_t q           = rev_hes_sparsity.end();
   size_t n           = pod_x_index.size();
   size_t m           = pod_y_index.size();
   bool   ok          = false;
   size_t thread      = thread_alloc::thread_num();
   allocate_work(thread);
   bool   zero_empty  = true;
   bool   input_empty = false;
   bool   transpose   = false;
   //
   // vx
   vector<bool> vx(n);
   for(size_t j = 0; j < n; j++)
      vx[j] = pod_x_index[j] != 0;
   //
   // note that s and t are vectors so transpose does not matter for bool case
   vector<bool> bool_s( work_[thread]->bool_s );
   vector<bool> bool_t( work_[thread]->bool_t );
   //
   bool_s.resize(m);
   bool_t.resize(n);
   //
   for(size_t i = 0; i < m; i++)
   {  if( pod_y_index[i] > 0  )
         bool_s[i] = rev_jac_flag[ pod_y_index[i] ];
   }
   //
   std::string msg = ": atomic_base.rev_sparse_hes: returned false";
   if( sparsity_ == pack_sparsity_enum )
   {  vectorBool&  pack_r( work_[thread]->pack_r );
      vectorBool&  pack_u( work_[thread]->pack_u );
      vectorBool&  pack_v( work_[thread]->pack_h );
      //
      pack_v.resize(n * q);
      //
      local::sparse::get_internal_pattern(
         transpose, pod_x_index, for_jac_sparsity, pack_r
      );
      local::sparse::get_internal_pattern(
         transpose, pod_y_index, rev_hes_sparsity, pack_u
      );
      //
      ok = rev_sparse_hes(vx, bool_s, bool_t, q, pack_r, pack_u, pack_v, x);
      if( ! ok )
         ok = rev_sparse_hes(vx, bool_s, bool_t, q, pack_r, pack_u, pack_v);
      if( ! ok )
      {  msg = atomic_name() + msg + " sparsity = pack_sparsity_enum";
         CPPAD_ASSERT_KNOWN(false, msg.c_str());
      }
      local::sparse::set_internal_pattern(zero_empty, input_empty,
         transpose, pod_x_index, rev_hes_sparsity, pack_v
      );
   }
   else if( sparsity_ == bool_sparsity_enum )
   {  vector<bool>&  bool_r( work_[thread]->bool_r );
      vector<bool>&  bool_u( work_[thread]->bool_u );
      vector<bool>&  bool_v( work_[thread]->bool_h );
      //
      bool_v.resize(n * q);
      //
      local::sparse::get_internal_pattern(
         transpose, pod_x_index, for_jac_sparsity, bool_r
      );
      local::sparse::get_internal_pattern(
         transpose, pod_y_index, rev_hes_sparsity, bool_u
      );
      //
      ok = rev_sparse_hes(vx, bool_s, bool_t, q, bool_r, bool_u, bool_v, x);
      if( ! ok )
         ok = rev_sparse_hes(vx, bool_s, bool_t, q, bool_r, bool_u, bool_v);
      if( ! ok )
      {  msg = atomic_name() + msg + " sparsity = bool_sparsity_enum";
         CPPAD_ASSERT_KNOWN(false, msg.c_str());
      }
      local::sparse::set_internal_pattern(zero_empty, input_empty,
         transpose, pod_x_index, rev_hes_sparsity, bool_v
      );
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( sparsity_ == set_sparsity_enum );
      vector< std::set<size_t> >&  set_r( work_[thread]->set_r );
      vector< std::set<size_t> >&  set_u( work_[thread]->set_u );
      vector< std::set<size_t> >&  set_v( work_[thread]->set_h );
      //
      set_v.resize(n);
      //
      local::sparse::get_internal_pattern(
         transpose, pod_x_index, for_jac_sparsity, set_r
      );
      local::sparse::get_internal_pattern(
         transpose, pod_y_index, rev_hes_sparsity, set_u
      );
      //
      ok = rev_sparse_hes(vx, bool_s, bool_t, q, set_r, set_u, set_v, x);
      if( ! ok )
         ok = rev_sparse_hes(vx, bool_s, bool_t, q, set_r, set_u, set_v);
      if( ! ok )
      {  msg = atomic_name() + msg + " sparsity = set_sparsity_enum";
         CPPAD_ASSERT_KNOWN(false, msg.c_str());
      }
      local::sparse::set_internal_pattern(zero_empty, input_empty,
         transpose, pod_x_index, rev_hes_sparsity, set_v
      );
   }
   for(size_t j = 0; j < n; j++)
   {  if( pod_x_index[j] > 0  )
         rev_jac_flag[ pod_x_index[j] ] |= bool_t[j];
   }
   return ok;
}

} // END_CPPAD_NAMESPACE
# endif
