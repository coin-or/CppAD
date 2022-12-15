// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin sparse_rc.cpp}

sparse_rc: Example and Test
###########################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end sparse_rc.cpp}
*/

// BEGIN C++
# include <cppad/utility/sparse_rc.hpp>
# include <vector>

bool sparse_rc(void)
{  bool ok = true;
   typedef std::vector<size_t> SizeVector;

   // 3 by 3 identity matrix
   size_t nr  = 3;
   size_t nc  = 3;
   size_t nnz = 3;
   CppAD::sparse_rc<SizeVector> pattern(nr, nc, nnz);
   for(size_t k = 0; k < nnz; k++)
      pattern.set(k, k, k);

   // row and column vectors corresponding to pattern
   const SizeVector& row = pattern.row();
   const SizeVector& col = pattern.row();

   // check pattern
   ok &= pattern.nnz() == nnz;
   ok &= pattern.nr()  == nr;
   ok &= pattern.nc()  == nc;
   for(size_t k = 0; k < nnz; k++)
   {  ok &= row[k] == k;
      ok &= col[k] == k;
   }

   // change to sparsity pattern for a 5 by 5 diagonal matrix
   // using push_back instead of set
   nr  = 5;
   nc  = 5;
   pattern.resize(nr, nc, 0);
   for(size_t k = 0; k < nr; k++)
   {  size_t r = nr - k - 1; // reverse or row-major order
      size_t c = nr - k - 1;
      pattern.push_back(r, c);
   }
   nnz = pattern.nnz();

   // check nnz, row major by value row major by reference, row and column
   ok &= nnz == nr;
   SizeVector        row_major_value     = pattern.row_major();
   const SizeVector& row_major_reference = pattern.get_row_major();
   for(size_t k = 0; k < nnz; k++)
   {  ok &= row[ row_major_value[k] ] == k;
      ok &= col[ row_major_value[k] ] == k;
   }
   ok &= row_major_reference.size() == 0;

   // set_row_major
   pattern.set_row_major();
   ok &= row_major_reference.size() == nnz;
   for(size_t k = 0; k < nnz; ++k)
      ok &= row_major_reference[k] == row_major_value[k];

   // create an empty pattern
   CppAD::sparse_rc<SizeVector> other;
   ok &= other.nnz() == 0;
   ok &= other.nr()  == 0;
   ok &= other.nc()  == 0;

   // now swap other with pattern
   pattern.swap(other);
   ok &= pattern.nnz() == 0;
   ok &= pattern.nr()  == 0;
   ok &= pattern.nc()  == 0;
   for(size_t k = 0; k < nnz; k++)
   {  ok &= other.row()[k] == nnz - k - 1;
      ok &= other.col()[k] == nnz - k - 1;
   }

   // now use the assignment statement
   pattern = other;

   // set_col_major
   const SizeVector& col_major_reference( pattern.get_col_major() );
   ok &= col_major_reference.size() == 0;
   pattern.set_col_major();
   ok &= col_major_reference.size() == nnz;
   //
   // check equality
   ok &= pattern == other;
   //
   // Change pattern so it no longer corresponds to a diagonal matrix
   pattern.set(nnz-1, 0, 1);
   //
   // check equality
   ok &= ! (pattern == other);
   //
   return ok;
}

// END C++
