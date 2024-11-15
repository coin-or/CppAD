// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------


# include <cppad/cppad.hpp>

namespace { // Begin empty namespace

// --------------------------------------------------------------------------
bool test_old_interface()
{  volatile bool ok = true;
   using namespace CppAD;

   // dimension of the domain space
   size_t n = 14;

   // dimension of the range space
   size_t m = 1;

   // temporary indices
   size_t i, j;

   // for testing load and store operations
   CppAD::VecAD<double> ad_vec(2);
   ad_vec[0] = 3.0;
   ad_vec[1] = 4.0;

   // initialize check values to false
   CPPAD_TESTVECTOR(bool) check(n * n);
   for(j = 0; j < n * n; j++)
      check[j] = false;

   // independent variable vector
   CPPAD_TESTVECTOR(AD<double>) ax(n);
   for(j = 0; j < n; j++)
      ax[j] = AD<double>(j);
   Independent(ax);

   // accumulate sum here
   AD<double> sum(0.);

   // first operand
   size_t F = 0;

   // ad_vec[variable] when ad_vec is a parameter
   sum += ad_vec[ax[F]]; // use fact ax[F] is zero
   F += 1;

   // ad_vec[parameter] when ad_vec depends on a variable
   // (CppAD sparsity does not separate elements of ad_vec)
   ad_vec[ AD<double>(0) ] = ax[F] * ax[F];
   sum += ad_vec[ ax[F] ];  // user fact that ax[F] is one
   check[F * n + F] = true;
   F += 1;

   // parameter / variable
   sum += 2.0 / ax[F];
   check[F * n + F] = true;
   F += 1;

   // erf(variable)
   sum += erf( ax[F] );
   check[F * n + F] = true;
   F += 1;

   // pow(parameter, variable)
   sum += pow( 2.0 , ax[F] );
   check[F * n + F] = true;
   F += 1;

   // pow(variable, parameter)
   sum += pow( ax[F] , 2.0 );
   check[F * n + F] = true;
   F += 1;
   // second operand
   size_t S = F + 1;

   // variable * variable
   sum += ax[F] * ax[S];
   check[F * n + S] = check[S * n + F] = true;
   F += 2;
   S += 2;

   // azmul(variable, variable)
   sum += azmul(ax[F], ax[S]);
   check[F * n + S] = check[S * n + F] = true;
   F += 2;
   S += 2;

   // variable / variable
   sum += ax[F] / ax[S];
   check[F * n + S] = check[S * n + F] = check[S * n + S] = true;
   F += 2;
   S += 2;

   // pow( variable , variable )
   sum += pow( ax[F] , ax[S] );
   check[F * n + F] = check[S * n + S] = true;
   check[F * n + S] = check[S * n + F] = true;
   F += 2;
   S += 2;

   ok &= F == n;
   CPPAD_TESTVECTOR(AD<double>) ay(m);
   ay[0] = sum;

   // create function object f : x -> y
   ADFun<double> f(ax, ay);

   // ------------------------------------------------------------------
   // compute sparsity
   CPPAD_TESTVECTOR(bool) r(n), s(m), h(n * n);
   for(j = 0; j < n; j++)
      r[j] = true;
   for(i = 0; i < m; i++)
      s[i] = true;
   h = f.ForSparseHes(r, s);
   // check result
   for(i = 0; i < n; i++)
      for(j = 0; j < n; j++)
      {
         if(h[i * n + j] != check[i * n + j])
         {
            std::cout << "i: " << i << std::endl;
            std::cout << "j: " << j << std::endl;
            std::cout << "h[i * n + j]: " << h[i * n + j] << std::endl;
            std::cout << "check[i * n + j]: " << check[i * n + j] << std::endl;
         }
         ok &= h[i * n + j] == check[i * n + j];
      }
   // ------------------------------------------------------------------

   return ok;
}
// --------------------------------------------------------------------------
// This case demonstrated a bug
bool test_csum(void)
{
   // ok
   bool ok = true;
   //
   // n, ax
   size_t n = 5;
   CPPAD_TESTVECTOR( CppAD::AD<double> ) ax(n);
   for(size_t j = 0; j < n; ++j)
      ax[j] = double(j + 1);
   CppAD::Independent(ax);
   //
   // ay
   size_t m = 1;
   CPPAD_TESTVECTOR( CppAD::AD<double> ) ay(m);
   CppAD::AD<double> sum = ax[0];
   for(size_t j = 1; j < n; ++j)
      sum += ax[j];
   ay[0] = sum * sum;
   //
   // f
   // the optimize step should create a CSumOp operation
   CppAD::ADFun<double> f(ax, ay);
   f.optimize();
   //
   // sparse_rc
   typedef CppAD::sparse_rc< CppAD::vector<size_t> > sparse_rc;
   //
   // pattern_hes
   sparse_rc pattern_hes;
   CPPAD_TESTVECTOR(bool) select_domain(n), select_range(m);
   for(size_t j = 0; j < n; ++j)
      select_domain[j] = true;
   select_range[0] = true;
   bool internal_bool = false;
   f.for_hes_sparsity(
      select_domain, select_range, internal_bool, pattern_hes
   );
   //
   // pattern_check
   size_t nr = n, nc = n, nnz = n * n;
   sparse_rc pattern_check(nr, nc, nnz);
   size_t k = 0;
   for(size_t i = 0; i < n; ++i)
   {  for(size_t j = 0; j < n; ++j)
      {  pattern_check.set(k, i, j);
         ++k;
      }
   }
   //
   // ok
   ok &= pattern_hes == pattern_check;
   //
   return ok;
}
} // End of empty namespace

// ---------------------------------------------------------------------------
bool for_hes_sparsity(void)
{  bool ok = true;
   //
   ok &= test_old_interface();
   //
   // 2DO: fix this case
   ok &= ! test_csum();
   //
   return ok;
}
