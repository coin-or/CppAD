# ifndef CPPAD_CORE_ATOMIC_THREE_JAC_SPARSITY_HPP
# define CPPAD_CORE_ATOMIC_THREE_JAC_SPARSITY_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_three_jac_sparsity}

Atomic Function Jacobian Sparsity Patterns
##########################################

Syntax
******
| *ok* = *afun* . ``jac_sparsity`` (
| |tab| *parameter_x* , *type_x* , *dependency* , *select_x* , *select_y* , *pattern_out*
| )

Prototype
*********
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

Implementation
**************
This function must be defined if
:ref:`atomic_three_ctor@atomic_user@afun` is
used to define an :ref:`ADFun-name` object *f* ,
and Jacobian sparsity patterns are computed for *f* .
(Computing Hessian sparsity patterns and optimizing
requires Jacobian sparsity patterns.)

Base
****
See :ref:`atomic_three_afun@Base` .

parameter_x
***********
See :ref:`atomic_three_define@parameter_x` .

type_x
******
See :ref:`atomic_three_define@type_x` .

dependency
**********
If *dependency* is true,
then *pattern_out* is a
:ref:`dependency.cpp@Dependency Pattern`
for this atomic function.
Otherwise it is a
:ref:`glossary@Sparsity Pattern` for the
derivative of the atomic function.

select_x
********
This argument has size equal to the number of arguments to this
atomic function; i.e. the size of *ax* .
It specifies which domain components are included in
the calculation of *pattern_out* .
If *select_x* [ *j* ] is false, then there will be no indices
*k* such that

   *pattern_out* . ``col`` ()[ *k* ] == *j*

.

select_y
********
This argument has size equal to the number of results to this
atomic function; i.e. the size of *ay* .
It specifies which range components are included in
the calculation of *pattern_out* .
If *select_y* [ *i* ] is false, then there will be no indices
*k* such that

   *pattern_out* . ``row`` ()[ *k* ] == *i*

.

pattern_out
***********
This input value of *pattern_out* does not matter.
Upon return it is a
dependency or sparsity pattern for the Jacobian of :math:`g(x)`,
the function corresponding to
:ref:`atomic_three_ctor@atomic_user@afun` ;
*dependency* above.
To be specific, there are non-negative indices
*i* , *j* , *k* such that

| |tab| *pattern_out* . ``row`` ()[ *k* ] == *i*
| |tab| *pattern_out* . ``col`` ()[ *k* ] == *j*

if and only if
*select_x* [ *j* ] is true,
*select_y* [ *j* ] is true,
and :math:`g_i(x)` depends on the value of :math:`x_j`
(and the partial of :math:`g_i(x)` with respect to
:math:`x_j` is possibly non-zero).

ok
**
If this calculation succeeded, *ok* is true.
Otherwise it is false.
{xrst_toc_hidden
   example/atomic_three/jac_sparsity.cpp
}
Examples
********
The file :ref:`atomic_three_jac_sparsity.cpp-name` contains an example and test
that uses this routine.

{xrst_end atomic_three_jac_sparsity}
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/three_jac_sparsity.hpp
Third generation atomic Jacobian dependency and sparsity patterns.
*/
/*!
atomic_three to Jacobian dependency and sparsity calculations.

\param parameter_x [in]
contains the values for arguments that are parameters.

\param type_x [in]
what is the type, in afun(ax, ay), for each component of x.

\param dependency [in]
if true, calculate dependency pattern,
otherwise calcuate sparsity pattern.

\param select_x [in]
which domain components to include in the dependency or sparsity pattern.
The index zero is used for parameters.

\param select_y [in]
which range components to include in the dependency or sparsity pattern.
The index zero is used for parameters.

\param pattern_out [out]
is the dependency or sparsity pattern.
*/
// BEGIN_PROTOTYPE
template <class Base>
bool atomic_three<Base>::jac_sparsity(
   const vector<Base>&                     parameter_x  ,
   const vector<ad_type_enum>&             type_x       ,
   bool                                    dependency   ,
   const vector<bool>&                     select_x     ,
   const vector<bool>&                     select_y     ,
   sparse_rc< vector<size_t> >&            pattern_out  )
// END_PROTOTYPE
{  return false; }
/*!
Link from forward Jacobian sparsity calcuations to atomic_three

\tparam InternalSparsity
Is the type used for internal sparsity calculations; i.e.,
sparse_pack or sparse_list.

\param dependency
if true, calcuate dependency pattern,
otherwise calcuate sparsity pattern.

\param parameter_x
is parameter arguments to the function, other components are nan.

\param type_x [in]
what is the type, in afun(ax, ay), for each component of x.

\param x_index
is the variable index, on the tape, for the arguments to this atomic function.
This size of x_index is n, the number of arguments to this atomic function.
The index zero is used for parameters.

\param y_index
is the variable index, on the tape, for the results for this atomic function.
This size of y_index is m, the number of results for this atomic function.
The index zero is used for parameters.

\param var_sparsity
On input, for j = 0, ... , n-1, the sparsity pattern with index x_index[j],
is the sparsity for the j-th argument to this atomic function.
On output, for i = 0, ... , m-1, the sparsity pattern with index y_index[i],
is the sparsity for the i-th result for this atomic function.

\return
is true if the computation succeeds.
*/
template <class Base>
template <class InternalSparsity>
bool atomic_three<Base>::for_jac_sparsity(
   bool                             dependency   ,
   const vector<Base>&              parameter_x  ,
   const vector<ad_type_enum>&      type_x       ,
   const vector<size_t>&            x_index      ,
   const vector<size_t>&            y_index      ,
   InternalSparsity&                var_sparsity )
{  typedef typename InternalSparsity::const_iterator iterator;

   // number of arguments and resutls for this atomic function
   size_t n = x_index.size();
   size_t m = y_index.size();

   // select_y
   vector<bool> select_y(m);
   for(size_t i = 0; i < m; ++i)
      select_y[i] = y_index[i] != 0;

   // determine select_x
   vector<bool> select_x(n);
   for(size_t j = 0; j < n; ++j)
   {  // check if x_j depends on any previous variable
      iterator itr(var_sparsity, x_index[j]);
      size_t ell = *itr;
      select_x[j] = ell < var_sparsity.end();
      CPPAD_ASSERT_UNKNOWN( x_index[j] > 0 || ! select_x[j] );
   }
   sparse_rc< vector<size_t> > pattern_out;
   bool ok = jac_sparsity(
      parameter_x, type_x, dependency, select_x, select_y, pattern_out
   );
   if( ! ok )
      return false;
   //
   // transfer sparsity patterns from pattern_out to var_sparsity
   size_t                nnz = pattern_out.nnz();
   const vector<size_t>& row( pattern_out.row() );
   const vector<size_t>& col( pattern_out.col() );
   for(size_t k = 0; k < nnz; ++k)
   {  size_t i = row[k];
      size_t j = col[k];
      CPPAD_ASSERT_KNOWN(
         select_y[i] && select_x[j],
         "atomic: jac_sparsity: pattern_out not in "
         "select_x or select_y range"
      );
      iterator itr(var_sparsity, x_index[j]);
      size_t ell = *itr;
      while( ell < var_sparsity.end() )
      {  var_sparsity.post_element( y_index[i], ell );
         ell = *(++itr);
      }
   }
   for(size_t i = 0; i < m; ++i)
      var_sparsity.process_post( y_index[i] );
   //
   return true;
}
/*!
Link from reverse Jacobian sparsity calcuations to atomic_three

\tparam InternalSparsity
Is the type used for internal sparsity calculations; i.e.,
sparse_pack or sparse_list.

\param dependency
if true, calcuate dependency pattern,
otherwise calcuate sparsity pattern.

\param parameter_x
is parameter arguments to the function, other components are nan.

\param type_x [in]
what is the type, in afun(ax, ay), for each component of x.

\param x_index
is the variable index, on the tape, for the arguments to this atomic function.
This size of x_index is n, the number of arguments to this atomic function.

\param y_index
is the variable index, on the tape, for the results for this atomic function.
This size of y_index is m, the number of results for this atomic function.

\param var_sparsity
On input, for i = 0, ... , m-1, the sparsity pattern with index y_index[i],
is the sparsity of the outter function with respect to the i-th
result for this atomic function.
On input, for j = 0, ... , n-1, the sparsity pattern with index x_index[j],
is the sparsity for the outter function with repsect to the j-th
argument to this atomic function.
On output, for j = 0, ... , n-1, the sparsity pattern with index x_index[j],
is the sparsity for the outter function with repsect to the j-th
argument to this atomic function with the atomic function results
removed as arguments to the outter function.

\return
is true if the computation succeeds.
*/
template <class Base>
template <class InternalSparsity>
bool atomic_three<Base>::rev_jac_sparsity(
   bool                             dependency   ,
   const vector<Base>&              parameter_x  ,
   const vector<ad_type_enum>&      type_x       ,
   const vector<size_t>&            x_index      ,
   const vector<size_t>&            y_index      ,
   InternalSparsity&                var_sparsity )
{  typedef typename InternalSparsity::const_iterator iterator;

   // number of arguments and resutls for this atomic function
   size_t n = x_index.size();
   size_t m = y_index.size();

   // selection vectors
   vector<bool> select_x(n), select_y(m);

   // 2DO: perhaps we could use for_type(type_x, type_y)
   // to reduce the true components in select_x
   for(size_t j = 0; j < n; ++j)
      select_x[j] = true;

   // determine select_y
   for(size_t i = 0; i < m; ++i)
   {  // check if y_i has sparsity is non-empty
      iterator itr(var_sparsity, y_index[i]);
      size_t ell = *itr;
      select_y[i] = ell < var_sparsity.end();
   }
   sparse_rc< vector<size_t> > pattern_out;
   bool ok = jac_sparsity(
      parameter_x, type_x, dependency, select_x, select_y, pattern_out
   );
   if( ! ok )
      return false;
   //
   // transfer sparsity patterns from pattern_out to var_sparsity
   size_t                nnz = pattern_out.nnz();
   const vector<size_t>& row( pattern_out.row() );
   const vector<size_t>& col( pattern_out.col() );
   for(size_t k = 0; k < nnz; ++k)
   {  size_t i = row[k];
      size_t j = col[k];
      CPPAD_ASSERT_KNOWN(
         select_y[i] && select_x[j],
         "atomic: jac_sparsity: pattern_out not in "
         "select_x or select_y range"
      );
      iterator itr(var_sparsity, y_index[i]);
      size_t ell = *itr;
      while( ell < var_sparsity.end() )
      {  var_sparsity.post_element( x_index[j], ell );
         ell = *(++itr);
      }
   }
   for(size_t j = 0; j < n; ++j)
      var_sparsity.process_post( x_index[j] );
   //
   return true;
}

} // END_CPPAD_NAMESPACE
# endif
