// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin sparse_rcv.cpp}

sparse_rcv: Example and Test
############################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end sparse_rcv.cpp}
*/

// BEGIN C++
# include <cppad/utility/sparse_rcv.hpp>
# include <vector>

bool sparse_rcv(void)
{  bool ok = true;
   typedef std::vector<size_t> SizeVector;
   typedef std::vector<double> ValueVector;

   // sparsity pattern for a 5 by 5 diagonal matrix
   size_t nr  = 5;
   size_t nc  = 5;
   size_t nnz = 5;
   CppAD::sparse_rc<SizeVector> pattern(nr, nc, nnz);
   for(size_t k = 0; k < nnz; k++)
   {  size_t r = nnz - k - 1; // reverse or column-major order
      size_t c = nnz - k - 1;
      pattern.set(k, r, c);
   }

   // sparse matrix
   CppAD::sparse_rcv<SizeVector, ValueVector> matrix(pattern);
   for(size_t k = 0; k < nnz; k++)
   {  double v = double(k);
      matrix.set(nnz - k - 1, v);
   }

   // row, column, and value vectors
   const SizeVector&  row( matrix.row() );
   const SizeVector&  col( matrix.row() );
   const ValueVector& val( matrix.val() );
   SizeVector col_major = matrix.col_major();

   // check row,  column, and value
   for(size_t k = 0; k < nnz; k++)
   {  ok &= row[ col_major[k] ] == k;
      ok &= col[ col_major[k] ] == k;
      ok &= val[ col_major[k] ] == double(k);
   }

   // pat
   const CppAD::sparse_rc<SizeVector> pat( matrix.pat() );
   ok &= nnz == pat.nnz();
   ok &= nr  == pat.nr();
   ok &= nc  == pat.nc();
   for(size_t k = 0; k < nnz; k++)
   {  ok &= row[k] == pat.row()[k];
      ok &= col[k] == pat.col()[k];
   }

   // create an empty matrix
   CppAD::sparse_rcv<SizeVector, ValueVector> other;
   ok &= other.nnz() == 0;
   ok &= other.nr()  == 0;
   ok &= other.nc()  == 0;

   // now swap other with matrix
   matrix.swap(other);
   ok &= matrix.nnz() == 0;
   ok &= matrix.nr()  == 0;
   ok &= matrix.nc()  == 0;
   for(size_t k = 0; k < nnz; k++)
   {  ok &= other.row()[ col_major[k] ] == k;
      ok &= other.col()[ col_major[k] ] == k;
      ok &= other.val()[ col_major[k] ] == double(k);
   }

   // now use the assignment statement
   matrix = other;
   ok    &= other.nr()  == matrix.nr();
   ok    &= other.nc()  == matrix.nc();
   ok    &= other.nnz() == matrix.nnz();
   for(size_t k = 0; k < nnz; k++)
   {  ok &= matrix.row()[k] == other.row()[k];
      ok &= matrix.col()[k] == other.col()[k];
      ok &= matrix.val()[k] == other.val()[k];
   }

   // now use the copy constructor
   CppAD::sparse_rcv<SizeVector, ValueVector> copy(matrix);
   ok    &= copy.nr()  == matrix.nr();
   ok    &= copy.nc()  == matrix.nc();
   ok    &= copy.nnz() == matrix.nnz();
   for(size_t k = 0; k < nnz; k++)
   {  ok &= matrix.row()[k] == copy.row()[k];
      ok &= matrix.col()[k] == copy.col()[k];
      ok &= matrix.val()[k] == copy.val()[k];
   }
   return ok;
}

// END C++
