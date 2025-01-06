// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin dependency.cpp}
{xrst_spell
  rl
}

Computing Dependency: Example and Test
######################################

Discussion
**********
The partial of an dependent variable with respect to an independent variable
might always be zero even though the dependent variable depends on the
value of the dependent variable. Consider the following case

.. math::

   f(x) = {\rm sign} (x) =
   \left\{ \begin{array}{rl}
      +1 & {\rm if} \; x > 0 \\
      0  & {\rm if} \; x = 0 \\
      -1 & {\rm if} \; x < 0
   \end{array} \right.

In this case the value of :math:`f(x)` depends on the value of :math:`x`
but CppAD always returns zero for the derivative of the :ref:`sign-name` function.

Dependency Pattern
******************
If the *i*-th dependent variables depends on the
value of the *j*-th independent variable,
the corresponding entry in the dependency pattern is non-zero (true).
Otherwise it is zero (false).
CppAD uses :ref:`sparsity patterns<glossary@Sparsity Pattern>`
to represent dependency patterns.

Computation
***********
The *dependency* argument to
:ref:`for_jac_sparsity<for_jac_sparsity@dependency>` and
:ref:`RevSparseJac<RevSparseJac@dependency>` is a flag that signals
that the dependency pattern (instead of the sparsity pattern) is computed.

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end dependency.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>
namespace {
   double heavyside(const double& x)
   {  if( x <= 0.0 )
         return 0.0;
      return 1.0;
   }
   CPPAD_DISCRETE_FUNCTION(double, heavyside)
}

bool dependency(void)
{  bool ok = true;
   using CppAD::AD;
   using CppAD::NearEqual;
   typedef CPPAD_TESTVECTOR(size_t)     SizeVector;
   typedef CppAD::sparse_rc<SizeVector> sparsity;

   // VecAD object for use later
   CppAD::VecAD<double> vec_ad(2);
   vec_ad[0] = 0.0;
   vec_ad[1] = 1.0;

   // domain space vector
   size_t n  = 5;
   CPPAD_TESTVECTOR(AD<double>) ax(n);
   for(size_t j = 0; j < n; j++)
      ax[j] = AD<double>(j + 1);

   // declare independent variables and start tape recording
   CppAD::Independent(ax);

   // some AD constants
   AD<double> azero(0.0), aone(1.0);

   // range space vector
   size_t m  = n;
   size_t m1 = n - 1;
   CPPAD_TESTVECTOR(AD<double>) ay(m);
   // Note that ay[m1 - j] depends on ax[j]
   ay[m1 - 0] = sign( ax[0] );
   ay[m1 - 1] = CondExpLe( ax[1], azero, azero, aone);
   ay[m1 - 2] = CondExpLe( azero, ax[2], azero, aone);
   ay[m1 - 3] = heavyside( ax[3] );
   ay[m1 - 4] = vec_ad[ ax[4] - AD<double>(4.0) ];

   // create f: x -> y and stop tape recording
   CppAD::ADFun<double> f(ax, ay);

   // sparsity pattern for n by n identity matrix
   size_t nr  = n;
   size_t nc  = n;
   size_t nnz = n;
   sparsity pattern_in(nr, nc, nnz);
   for(size_t k = 0; k < nnz; k++)
   {  size_t r = k;
      size_t c = k;
      pattern_in.set(k, r, c);
   }

   // compute dependency pattern
   bool transpose     = false;
   bool dependency    = true;  // would transpose dependency pattern
   bool internal_bool = true;  // does not affect result
   sparsity pattern_out;
   f.for_jac_sparsity(
      pattern_in, transpose, dependency, internal_bool, pattern_out
   );
   const SizeVector& row( pattern_out.row() );
   const SizeVector& col( pattern_out.col() );
   SizeVector col_major = pattern_out.col_major();

   // check result
   ok &= pattern_out.nr()  == n;
   ok &= pattern_out.nc()  == n;
   ok &= pattern_out.nnz() == n;
   for(size_t k = 0; k < n; k++)
   {  ok &= row[ col_major[k] ] == m1 - k;
      ok &= col[ col_major[k] ] == k;
   }
   // -----------------------------------------------------------
   // RevSparseJac and set dependency
   CppAD::vector<    std::set<size_t> > eye_set(m), depend_set(m);
   for(size_t i = 0; i < m; i++)
   {  ok &= eye_set[i].empty();
      eye_set[i].insert(i);
   }
   depend_set = f.RevSparseJac(n, eye_set, transpose, dependency);
   for(size_t i = 0; i < m; i++)
   {  std::set<size_t> check;
      check.insert(m1 - i);
      ok &= depend_set[i] == check;
   }
   dependency = false;
   depend_set = f.RevSparseJac(n, eye_set, transpose, dependency);
   for(size_t i = 0; i < m; i++)
      ok &= depend_set[i].empty();
   return ok;
}

// END C++
