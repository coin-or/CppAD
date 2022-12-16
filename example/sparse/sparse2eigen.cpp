// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin sparse2eigen.cpp}

Converting CppAD Sparse Matrix to Eigen Format: Example and Test
################################################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end sparse2eigen.cpp}
*/
// BEGIN C++
# include <cppad/utility/sparse2eigen.hpp>
bool sparse2eigen(void)
{  bool ok = true;
   //
   typedef CppAD::vector<size_t>                 s_vector;
   typedef CppAD::vector<double>                 d_vector;
   typedef CppAD::sparse_rc<s_vector>            sparse_rc;
   typedef CppAD::sparse_rcv<s_vector, d_vector> sparse_rcv;
   //
   // sparsity pattern
   size_t nr  = 3;
   size_t nc  = 4;
   size_t nnz = 2 * nr;
   sparse_rc pattern(nr, nc, nnz);
   for(size_t i = 0; i < nr; i++)
   {  for(size_t j = i; j <= i + 1; j++)
      {  size_t k = i + j;
         pattern.set(k, i, j);
      }
   }
   //
   // sparse matrix
   sparse_rcv source(pattern);
   for(size_t i = 0; i < nr; i++)
   {  for(size_t j = i; j <= i + 1; j++)
      {  size_t k = i + j;
         double v = double(k) / 2.0;
         source.set(k, v);
      }
   }
   //
   //  example using row major order
   {  Eigen::SparseMatrix<double, Eigen::RowMajor> destination;
      CppAD::sparse2eigen(source, destination);
      //
      typedef
      Eigen::SparseMatrix<double, Eigen::RowMajor>::InnerIterator iterator;
      //
      // check result
      const double* d_value = destination.valuePtr();
      size_t k = 0;
      for(int i = 0; i < destination.outerSize(); ++i)
      {
         for(iterator itr(destination, i); itr; ++itr)
         {  // check row
            ok &= itr.row() == i;
            //
            // check column
            if( k % 2 == 0 )
               ok &= itr.col() == i;
            else
               ok &= itr.col() == (i+1);
            //
            // check value
            ok  &= itr.value() == double( itr.row() + itr.col() ) / 2.0;
            ok  &= itr.value() == d_value[k];
            //
            ++k;
         }
      }
      ok &= k == nnz;
   }
   //
   //  example using column major order
   {  Eigen::SparseMatrix<double, Eigen::ColMajor> destination;
      CppAD::sparse2eigen(source, destination);
      //
      typedef
      Eigen::SparseMatrix<double, Eigen::ColMajor>::InnerIterator iterator;
      //
      // check result
      const double* d_value = destination.valuePtr();
      size_t k = 0;
      for(int j = 0; j < destination.outerSize(); ++j)
      {  for(iterator itr(destination, j); itr; ++itr)
         {  // check column
            ok &= itr.col() == j;
            //
            // check row
            if( j == 0 )
            {  assert( k == 0 );
               ok &= itr.row() == 0;
            }
            else if( k % 2 == 1 )
               ok &= itr.row() == j - 1;
            else
               ok &= itr.row() == j;
            //
            // check value
            ok  &= itr.value() == double( itr.row() + itr.col() ) / 2.0;
            ok  &= itr.value() == d_value[k];
            //
            ++k;
         }
      }
      ok &= k == nnz;
   }
   //
   return ok;
}
// END C++
