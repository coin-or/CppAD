// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin sparsity_sub.cpp}

Sparsity Patterns For a Subset of Variables: Example and Test
#############################################################

See Also
********
:ref:`sparse_sub_hes.cpp-name` , :ref:`sub_sparse_hes.cpp-name` .

ForSparseJac
************
The routine :ref:`ForSparseJac-name` is used to compute the
sparsity for both the full Jacobian (see *s* )
and a subset of the Jacobian (see *s2* ).

RevSparseHes
************
The routine :ref:`RevSparseHes-name` is used to compute both
sparsity for both the full Hessian (see *h* )
and a subset of the Hessian (see *h2* ).

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end sparsity_sub.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool sparsity_sub(void)
{  // C++ source code
   bool ok = true;
   //
   using std::cout;
   using CppAD::vector;
   using CppAD::AD;
   using CppAD::vectorBool;

   size_t n = 4;
   size_t m = n-1;
   vector< AD<double> > ax(n), ay(m);
   for(size_t j = 0; j < n; j++)
      ax[j] = double(j+1);
   CppAD::Independent(ax);
   for(size_t i = 0; i < m; i++)
      ay[i] = (ax[i+1] - ax[i]) * (ax[i+1] - ax[i]);
   CppAD::ADFun<double> f(ax, ay);

   // Evaluate the full Jacobian sparsity pattern for f
   vectorBool r(n * n), s(m * n);
   for(size_t j = 0 ; j < n; j++)
   {  for(size_t i = 0; i < n; i++)
         r[i * n + j] = (i == j);
   }
   s = f.ForSparseJac(n, r);

   // evaluate the sparsity for the Hessian of f_0 + ... + f_{m-1}
   vectorBool t(m), h(n * n);
   for(size_t i = 0; i < m; i++)
      t[i] = true;
   h = f.RevSparseHes(n, t);

   // evaluate the Jacobian sparsity pattern for first n/2 components of x
   size_t n2 = n / 2;
   vectorBool r2(n * n2), s2(m * n2);
   for(size_t j = 0 ; j < n2; j++)
   {  for(size_t i = 0; i < n; i++)
         r2[i * n2 + j] = (i == j);
   }
   s2 = f.ForSparseJac(n2, r2);

   // evaluate the sparsity for the subset of Hessian of
   // f_0 + ... + f_{m-1} where first partial has only first n/2 components
   vectorBool h2(n2 * n);
   h2 = f.RevSparseHes(n2, t);

   // check sparsity pattern for Jacobian
   for(size_t i = 0; i < m; i++)
   {  for(size_t j = 0; j < n2; j++)
         ok &= s2[i * n2 + j] == s[i * n + j];
   }

   // check sparsity pattern for Hessian
   for(size_t i = 0; i < n2; i++)
   {  for(size_t j = 0; j < n; j++)
         ok &= h2[i * n + j] == h[i * n + j];
   }
   return ok;
}
// END C++
