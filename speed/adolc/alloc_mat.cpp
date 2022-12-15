// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin adolc_alloc_mat}

Adolc Test Utility: Allocate and Free Memory For a Matrix
#########################################################

Syntax
******

| *mat* = ``adolc_alloc_mat`` ( *m* , *n* )
| *adolc_free_mat* ( ``mat`` )

Purpose
*******
Use the :ref:`thread_alloc-name` memory allocator to allocate and free
memory that can be used as a matrix with the Adolc package.

m
*
Is the number of rows in the matrix.

n
*
Is the number of columns in the matrix.

mat
***
Is the matrix.
To be specific,
between a call to ``adolc_alloc_mat`` ,
and the corresponding call to ``adolc_free_mat`` ,
for *i* = 0 , ... , *m* ``-1``
and *j* = 0 , ... , *n* ``-1`` ,
*mat* [ *i* ][ *j* ] is the element in row *i*
and column *j* .

{xrst_end adolc_alloc_mat}
*/
# include <cppad/utility/thread_alloc.hpp>

double** adolc_alloc_mat(size_t m, size_t n)
{  using CppAD::thread_alloc;
   size_t size_min = m * n, size_out;
   double*  vec = thread_alloc::create_array<double>(size_min, size_out);
   double** mat = thread_alloc::create_array<double*>(size_min, size_out);

   for(size_t i = 0; i < m; i++)
      mat[i] = vec + i * n;

   return mat;
}
void adolc_free_mat(double** mat)
{  using CppAD::thread_alloc;
   thread_alloc::delete_array(mat[0]);
   thread_alloc::delete_array(mat);
   return;
}
