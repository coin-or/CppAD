# ifndef CPPAD_CORE_ATOMIC_TWO_FOR_SPARSE_HES_HPP
# define CPPAD_CORE_ATOMIC_TWO_FOR_SPARSE_HES_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_two_for_sparse_hes app}
{xrst_spell
   na
   vx
}

Atomic Forward Hessian Sparsity Patterns
########################################

Syntax
******
| *ok* = *afun* . ``for_sparse_hes`` ( *vx* , *r* , *s* , *h* , *x* )

Deprecated 2016-06-27
*********************
*ok* = *afun* . ``for_sparse_hes`` ( *vx* , *r* , *s* , *h* )

Purpose
*******
This function is used by :ref:`ForSparseHes-name` to compute
Hessian sparsity patterns.
If you are using :ref:`ForSparseHes-name` ,
one of the versions of this
virtual function must be defined by the
:ref:`atomic_two_ctor@atomic_user` class.

Given a :ref:`glossary@Sparsity Pattern` for
a diagonal matrix :math:`R \in \B{R}^{n \times n}`, and
a row vector :math:`S \in \B{R}^{1 \times m}`,
this routine computes the sparsity pattern for

.. math::

   H(x) = R^\R{T} \cdot (S \cdot f)^{(2)}( x ) \cdot R

Implementation
**************
If you are using and :ref:`ForSparseHes-name` ,
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

r
=
This argument has prototype

   ``const CppAD:vector<bool>&`` *r*

and is a :ref:`atomic_two_option@atomic_sparsity` pattern for
the diagonal of :math:`R \in \B{R}^{n \times n}`.

s
=
The argument *s* has prototype

   ``const CppAD:vector<bool>&`` *s*

and its size is *m* .
It is a sparsity pattern for :math:`S \in \B{R}^{1 \times m}`.

h
=
This argument has prototype

   *atomic_sparsity* & *h*

The input value of its elements
are not specified (must not matter).
Upon return, *h* is a
:ref:`atomic_two_option@atomic_sparsity` pattern for
:math:`H(x) \in \B{R}^{n \times n}` which is defined above.

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

{xrst_end atomic_two_for_sparse_hes}
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/two_for_sparse_hes.hpp
Atomic forward mode Hessian sparsity patterns.
*/
/*!
Link, after case split, from for_hes_sweep to atomic_base.

\param vx [in]
which componens of x are variables.

\param r [in]
is the forward Jacobian sparsity pattern w.r.t the argument vector x.

\param s [in]
is the reverse Jacobian sparsity pattern w.r.t the result vector y.

\param h [out]
is the Hessian sparsity pattern w.r.t the argument vector x.

\param x
is the integer value of the x arguments that are parameters.
*/
template <class Base>
bool atomic_base<Base>::for_sparse_hes(
   const vector<bool>&             vx ,
   const vector<bool>&             r  ,
   const vector<bool>&             s  ,
   vector< std::set<size_t> >&     h  ,
   const vector<Base>&             x  )
{  return false; }
template <class Base>
bool atomic_base<Base>::for_sparse_hes(
   const vector<bool>&             vx ,
   const vector<bool>&             r  ,
   const vector<bool>&             s  ,
   vector<bool>&                   h  ,
   const vector<Base>&             x  )
{  return false; }
template <class Base>
bool atomic_base<Base>::for_sparse_hes(
   const vector<bool>&             vx ,
   const vector<bool>&             r  ,
   const vector<bool>&             s  ,
   vectorBool&                     h  ,
   const vector<Base>&             x  )
// deprecated versions
{  return false; }
template <class Base>
bool atomic_base<Base>::for_sparse_hes(
   const vector<bool>&             vx ,
   const vector<bool>&             r  ,
   const vector<bool>&             s  ,
   vector< std::set<size_t> >&     h  )
{  return false; }
template <class Base>
bool atomic_base<Base>::for_sparse_hes(
   const vector<bool>&             vx ,
   const vector<bool>&             r  ,
   const vector<bool>&             s  ,
   vector<bool>&                   h  )
{  return false; }
template <class Base>
bool atomic_base<Base>::for_sparse_hes(
   const vector<bool>&             vx ,
   const vector<bool>&             r  ,
   const vector<bool>&             s  ,
   vectorBool&                     h  )
{  return false; }
/*!
Link, before case split, from for_hes_sweep to atomic_base.
2DO: move this functiton outside this file so can change
developer documentation to omhelp formating.

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

\param rev_jac_sparsity
On input, for i = 0, ... , m-1, the sparsity pattern with index y_index[i],
is the reverse Jacobian sparsity for the i-th result to this atomic function.
This shows which components of the result affect the function we are
computing the Hessian of.

\param for_hes_sparsity
This is the sparsity pattern for the Hessian. On input, the non-linear
terms in the atomic fuction have not been included. Upon return, they
have been included.
*/
template <class Base>
template <class InternalSparsity>
bool atomic_base<Base>::for_sparse_hes(
   const vector<Base>&              x                ,
   const vector<size_t>&            x_index          ,
   const vector<size_t>&            y_index          ,
   size_t                           np1              ,
   size_t                           numvar           ,
   const InternalSparsity&          rev_jac_sparsity ,
   InternalSparsity&                for_sparsity     )
{  typedef typename InternalSparsity::const_iterator const_iterator;
   CPPAD_ASSERT_UNKNOWN( rev_jac_sparsity.end() == 1 );
   CPPAD_ASSERT_UNKNOWN( for_sparsity.end() == np1 );
   CPPAD_ASSERT_UNKNOWN( for_sparsity.n_set() == np1 + numvar );
   size_t n      = x_index.size();
   size_t m      = y_index.size();
   bool   ok     = false;
   size_t thread = thread_alloc::thread_num();
   allocate_work(thread);
   //
   // vx
   vector<bool> vx(n);
   for(size_t j = 0; j < n; j++)
      vx[j] = x_index[j] != 0;
   //
   // bool_r
   vector<bool>& bool_r( work_[thread]->bool_r );
   bool_r.resize(n);
   for(size_t j = 0; j < n; j++)
   {  // check if we must compute row and column j of h
      const_iterator itr(for_sparsity, np1 + x_index[j]);
      size_t i = *itr;
      bool_r[j] = i < np1;
   }
   //
   // bool s
   vector<bool>& bool_s( work_[thread]->bool_s );
   bool_s.resize(m);
   for(size_t i = 0; i < m; i++)
   {  // check if row i of result is included in h
      bool_s[i] = rev_jac_sparsity.is_element(y_index[i], 0);
   }
   //
   // h
   vectorBool&                 pack_h( work_[thread]->pack_h );
   vector<bool>&               bool_h( work_[thread]->bool_h );
   vector< std::set<size_t> >& set_h(  work_[thread]->set_h );
   //
   // call user's version of atomic function
   std::string msg    = ": atomic_base.for_sparse_hes: returned false";
   if( sparsity_ == pack_sparsity_enum )
   {  pack_h.resize(n * n);
      ok = for_sparse_hes(vx, bool_r, bool_s, pack_h, x);
      if( ! ok )
         ok = for_sparse_hes(vx, bool_r, bool_s, pack_h);
      if( ! ok )
      {  msg = atomic_name() + msg + " sparsity = pack_sparsity_enum";
         CPPAD_ASSERT_KNOWN(false, msg.c_str());
      }
   }
   else if( sparsity_ == bool_sparsity_enum )
   {  bool_h.resize(n * n);
      ok = for_sparse_hes(vx, bool_r, bool_s, bool_h, x);
      if( ! ok )
         ok = for_sparse_hes(vx, bool_r, bool_s, bool_h);
      if( ! ok )
      {  msg = atomic_name() + msg + " sparsity = bool_sparsity_enum";
         CPPAD_ASSERT_KNOWN(false, msg.c_str());
      }
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( sparsity_ == set_sparsity_enum )
      set_h.resize(n);
      ok = for_sparse_hes(vx, bool_r, bool_s, set_h, x);
      if( ! ok )
         ok = for_sparse_hes(vx, bool_r, bool_s, set_h);
      if( ! ok )
      {  msg = atomic_name() + msg + " sparsity = set_sparsity_enum";
         CPPAD_ASSERT_KNOWN(false, msg.c_str());
      }
   }
   CPPAD_ASSERT_UNKNOWN( ok );
   //
   // modify hessian in calling routine
   for(size_t i = 0; i < n; i++)
   {  for(size_t j = 0; j < n; j++)
      {  if( (x_index[i] > 0) && (x_index[j] > 0) )
         {  bool flag = false;
            switch( sparsity_ )
            {  case pack_sparsity_enum:
               flag = pack_h[i * n + j];
               break;
               //
               case bool_sparsity_enum:
               flag = bool_h[i * n + j];
               break;
               //
               case set_sparsity_enum:
               flag = set_h[i].find(j) != set_h[i].end();
               break;
            }
            if( flag )
            {  const_iterator itr_i(for_sparsity, np1 + x_index[i]);
               size_t i_x = *itr_i;
               while( i_x < np1 )
               {  for_sparsity.binary_union(
                     i_x, i_x, np1 + x_index[j], for_sparsity
                  );
                  i_x = *(++itr_i);
               }
               const_iterator itr_j(for_sparsity, np1 + x_index[j]);
               size_t j_x = *itr_j;
               while( j_x < np1 )
               {  for_sparsity.binary_union(
                     j_x, j_x, np1 + x_index[i], for_sparsity
                  );
                  j_x = *(++itr_j);
               }
            }
         }
      }
   }
   return ok;
}

} // END_CPPAD_NAMESPACE
# endif
