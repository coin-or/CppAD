// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin det_of_minor.cpp}

Determinant of a Minor: Example and Test
########################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end det_of_minor.cpp}
*/
// BEGIN C++
# include <vector>
# include <cstddef>
# include <cppad/speed/det_of_minor.hpp>

bool det_of_minor()
{  bool   ok = true;
   //
   // a
   // values in the matrix A in row major order
   std::vector<double> a = {
      1., 2., 3.,
      4., 5., 6.,
      7., 8., 10.
   };
   //
   // m
   // dimension of the matrix A
   size_t m = 3;
   assert( m * m == a.size() );
   //
   // r, c
   // index vectors set so minor is the entire matrix A
   std::vector<size_t> r(m + 1);
   std::vector<size_t> c(m + 1);
   for(size_t i= 0; i < m; i++)
   {  r[i] = i+1;
      c[i] = i+1;
   }
   r[m] = 0;
   c[m] = 0;
   //
   // n
   // size of minor that is the entire matrix A
   size_t n = m;
   //
   // det
   // evaluate the determinant of A
   double det = CppAD::det_of_minor(a, m, n, r, c);
   //
   // ok
   // check the value of the determinant of A
   ok &= (det == (double) (1*(5*10-6*8) - 2*(4*10-6*7) + 3*(4*8-5*7)) );
   //
   // M
   // minor where row 0 and column 1 are removed
   r[m] = 1;     // skip row index 0 by starting at row index 1
   c[0] = 2;     // skip column index 1 by pointing from index 0 to index 2
   n    = m - 1; // dimension of the minor M
   //
   // det
   // evaluate determinant of the minor
   det = CppAD::det_of_minor(a, m, m-1, r, c);
   //
   // ok
   // check the value of the determinant of the minor
   ok &= (det == (double) (4*10-6*7) );
   //
   return ok;
}
// END C++
