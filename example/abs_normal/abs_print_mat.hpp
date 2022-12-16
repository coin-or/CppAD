# ifndef CPPAD_EXAMPLE_ABS_NORMAL_ABS_PRINT_MAT_HPP
# define CPPAD_EXAMPLE_ABS_NORMAL_ABS_PRINT_MAT_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin abs_print_mat}
{xrst_spell
   cout
   nr
}
abs_normal: Print a Vector or Matrix
####################################

Syntax
******
``abs_print_mat`` ( *name* , *nr* , *nc* , *mat* )

Prototype
*********
{xrst_literal
   // BEGIN PROTOTYPE
   // END PROTOTYPE
}

Purpose
*******
This routine is used by the :ref:`abs_normal<example_abs_normal-name>` examples to print
vectors and matrices.
A new-line is printed at the end of this output.

name
****
This is a name that is printed before the vector or matrix.

nr
**
This is the number of rows in the matrix. Use *nr*  = 1 for
row vectors.

nc
**
This is the number of columns in the matrix. Use *nc*  = 1 for
column vectors.

mat
***
This is a
:ref:`row-major<glossary@Row-major Representation>` representation
of the matrix (hence a :ref:`SimpleVector-name` ).
The syntax

   *std::cout <<* ``mat`` [ ``i`` ]

must output the *i*-th element of the simple vector *mat* .

{xrst_end abs_print_mat}
-----------------------------------------------------------------------------
*/
# include <cppad/cppad.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN PROTOTYPE
template <class Vector>
void abs_print_mat(
   const std::string& name ,
   size_t             nr   ,
   size_t             nc   ,
   const Vector&      mat  )
// END PROTOTYPE
{
   CPPAD_ASSERT_KNOWN(
      size_t(mat.size()) == nr * nc,
      "abs_print_mat: size of mat is not nr * nc"
   );
   // output name
   std::cout << name << " =";
   //
   // handel empty case
   if( nr == 0 || nc == 0 )
   {  std::cout << " " << nr << " by " << nc << " empty matrix\n";
      return;
   }
   //
   // handle vector case
   if( nr == 1 || nc == 1 )
   {  std::cout << " [";
      for(size_t i = 0; i < nr * nc; i++)
      {  if( i > 0 )
            std::cout << ", ";
         std::cout << mat[i];
      }
      std::cout << "]";
      //
      // column vectors are printed as row vectors with a transpose at end
      if( nr > 1 )
         std::cout << "^T";
      //
      std::cout << "\n";
      return;
   }
   // non-empty matrix
   std::cout << "\n";
   for(size_t i = 0; i < nr; i++)
   {  std::cout << "[";
      for(size_t j = 0; j < nc; j++)
      {  if( j > 0 )
            std::cout << ", ";
         std::cout << mat[i * nc + j];
      }
      std::cout << "]\n";
   }
   return;
}

} // END_CPPAD_NAMESPACE
# endif
