# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_HES_SPARSITY_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_HES_SPARSITY_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
$begin atomic_four_lin_ode_hes_sparsity.hpp$$
$spell
   jac
   hes
   nnz
   vk
$$

$section
Atomic Linear ODE Hessian Sparsity Pattern: Example Implementation
$$

$head Purpose$$
The $code hes_sparsity$$ routine overrides the virtual functions
used by the atomic_four base class for Hessian sparsity calculations; see
$cref/hes_sparsity/atomic_four_hes_sparsity/$$.

$head Notation$$
We use the notation:
$cref/call_id/atomic_four_lin_ode/call_id/$$
$cref/r/atomic_four_lin_ode/r/$$
$cref/pattern/atomic_four_lin_ode/pattern/$$
$cref/transpose/atomic_four_lin_ode/transpose/$$
$cref/nnz/atomic_four_lin_ode/pattern/nnz/$$,
$cref/row/atomic_four_lin_ode/pattern/row/$$,
$cref/col/atomic_four_lin_ode/pattern/col/$$,
$cref/x/atomic_four_lin_ode/x/$$,
$cref/n/atomic_four_lin_ode/x/n/$$,
$cref/A(x)/atomic_four_lin_ode/x/A(x)/$$,
$cref/b(x)/atomic_four_lin_ode/x/b(x)/$$,
$cref/y(x)/atomic_four_lin_ode/y(x)/$$,
$cref/m/atomic_four_lin_ode/y(x)/m/$$,
$cref/vk(x)/atomic_four_lin_ode/vk(x)/$$,
and the following additional notation:

$subhead wk(x)$$
Because we are using the $cref Rosen34$$ solver, our actual sequence
of operations is only fourth order accurate.
So it suffices to compute the sparsity pattern for
$latex \[
\tilde{y} (x) = \sum_{k=0}^4 v^k (x)
\] $$
Note that the factor $latex r / k$$,
in the definition of $latex v^k (x)$$,
is constant (with respect to the variables).
Hence it suffices to compute the sparsity pattern for
$latex \[
h (x) = \sum_{k=0}^4 w^k (x)
\] $$
where $latex w^0 (x) = b(x)$$ and for $latex k = 1, 2, \ldots$$,
$latex w^k (x) = A(x) w^{k-1} (x)$$.

$head Example$$
The file $cref atomic_four_lin_ode_sparsity.cpp$$
contains an example and test using this operator.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// hes_sparsity override
template <class Base>
bool atomic_lin_ode<Base>::hes_sparsity(
   size_t                                         call_id      ,
   const CppAD::vector<bool>&                     ident_zero_x ,
   const CppAD::vector<bool>&                     select_x     ,
   const CppAD::vector<bool>&                     select_y     ,
   CppAD::sparse_rc< CppAD::vector<size_t> >&     pattern_out  )
{  //
   // adouble
   typedef AD<double> adouble;
   //
   // pattern_A, transpose, nnz
   Base      r;
   Base      step;
   sparse_rc pattern_A;
   bool      transpose;
   get(call_id, r, step, pattern_A, transpose);
   size_t nnz = pattern_A.nnz();
   //
   // m, n
   size_t m = select_y.size();
   size_t n = select_x.size();
   //
   // au
   vector<adouble> au(n);
   for(size_t j = 0; j < n; ++j)
      au[j] = 1.0;
   Independent( au );
   //
   // ax
   vector<adouble> ax(n);
   for(size_t j = 0; j < n; ++j)
      if( ident_zero_x[j] )
         ax[j] = 0.0;
      else
         ax[j] = au[j];
   //
   // aw
   // aw = w^0 (x)
   vector<adouble> aw(m);
   for(size_t i = 0; i < m; ++i)
      aw[i] = ax[nnz + i];
   //
   // ah = w^0 (x)
   vector<adouble> ah = aw;
   //
   // ah = sum_k=0^4 w^k (x)
   vector<adouble> awk(m);
   for(size_t k = 1; k < 5; ++k)
   {  // aw = w^{k-1} (x)
      //
      // awk = w^k (x)
      for(size_t i = 0; i < m; ++i)
         awk[i] = 0.0;
      for(size_t p = 0; p < nnz; ++p)
      {  //
         // i, j
         size_t i = pattern_A.row()[p];
         size_t j = pattern_A.col()[p];
         if( transpose )
            std::swap(i, j);
         //
         // awk
         awk[i] += ax[p] * aw[j];
      }
      //
      // ah = ah + w^k(x)
      for(size_t i = 0; i < m; ++i)
         ah[i] += awk[i];
      //
      // aw = w^k (x)
      aw = awk;
   }
   //
   // h
   ADFun<double> h;
   h.Dependent(au, ah);
   //
   // pattern_out
   // can use h.for_hes_sparsity or h.rev_hes_sparsity
   bool internal_bool = false;
   h.for_hes_sparsity(select_x, select_y, internal_bool, pattern_out);
   //
   return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
