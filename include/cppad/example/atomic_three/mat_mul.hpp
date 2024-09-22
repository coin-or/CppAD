# ifndef CPPAD_EXAMPLE_ATOMIC_THREE_MAT_MUL_HPP
# define CPPAD_EXAMPLE_ATOMIC_THREE_MAT_MUL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin atomic_three_mat_mul.hpp}
{xrst_spell
   nr
   px
   tx
   ty
}

Matrix Multiply as an Atomic Operation
######################################

See Also
********
:ref:`atomic_two_eigen_mat_mul.hpp-name`

Purpose
*******
Use scalar ``double`` operations in an :ref:`atomic_three-name` operation
that computes the matrix product for ``AD<double`` operations.

parameter_x
===========
This example demonstrates the use of the
:ref:`atomic_three_define@parameter_x`
argument to the :ref:`atomic_three-name` virtual functions.

type_x
======
This example also demonstrates the use of the
:ref:`atomic_three_define@type_x`
argument to the :ref:`atomic_three-name` virtual functions.

Matrix Dimensions
*****************
The first three components of the argument vector *ax*
in the call *afun* ( *ax* , *ay* )
are parameters and contain the matrix dimensions.
This enables them to be different for each use of the same atomic
function *afun* .
These dimensions are:

.. list-table::
   :widths: auto

   * - *ax* [0]
     - *nr_left*
     - number of rows in the left matrix and result matrix
   * - *ax* [1]
     - *n_middle*
     - columns in the left matrix and rows in right matrix
   * - *ax* [2]
     - *nc_right*
     - number of columns in the right matrix and result matrix

Left Matrix
***********
The number of elements in the left matrix is

   *n_left* = *nr_left* * *n_middle*

The elements are in
*ax* [3] through *ax* [2+ *n_left* ] in row major order.

Right Matrix
************
The number of elements in the right matrix is

   *n_right* = *n_middle* * *nc_right*

The elements are in
*ax* [3+ *n_left* ] through
*ax* [2+ *n_left* + *n_right* ] in row major order.

Result Matrix
*************
The number of elements in the result matrix is

   *n_result* = *nr_left* * *nc_right*

The elements are in
*ay* [0] through *ay* [ *n_result* ``-1`` ] in row major order.

Start Class Definition
**********************
{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/cppad.hpp>
namespace { // Begin empty namespace
using CppAD::vector;
//
// matrix result = left * right
class atomic_mat_mul : public CppAD::atomic_three<double> {
/* {xrst_code}
{xrst_spell_on}
Constructor
***********
{xrst_spell_off}
{xrst_code cpp} */
public:
   // ---------------------------------------------------------------------
   // constructor
   atomic_mat_mul(void) : CppAD::atomic_three<double>("mat_mul")
   { }
private:
/* {xrst_code}
{xrst_spell_on}
Left Operand Element Index
**************************
Index in the Taylor coefficient matrix *tx* of a left matrix element.
{xrst_spell_off}
{xrst_code cpp} */
   size_t left(
      size_t i        , // left matrix row index
      size_t j        , // left matrix column index
      size_t k        , // Taylor coeffocient order
      size_t nk       , // number of Taylor coefficients in tx
      size_t nr_left  , // rows in left matrix
      size_t n_middle , // rows in left and columns in right
      size_t nc_right ) // columns in right matrix
   {  assert( i < nr_left );
      assert( j < n_middle );
      return (3 + i * n_middle + j) * nk + k;
   }
/* {xrst_code}
{xrst_spell_on}
Right Operand Element Index
***************************
Index in the Taylor coefficient matrix *tx* of a right matrix element.
{xrst_spell_off}
{xrst_code cpp} */
   size_t right(
      size_t i        , // right matrix row index
      size_t j        , // right matrix column index
      size_t k        , // Taylor coeffocient order
      size_t nk       , // number of Taylor coefficients in tx
      size_t nr_left  , // rows in left matrix
      size_t n_middle , // rows in left and columns in right
      size_t nc_right ) // columns in right matrix
   {  assert( i < n_middle );
      assert( j < nc_right );
      size_t offset = 3 + nr_left * n_middle;
      return (offset + i * nc_right + j) * nk + k;
   }
/* {xrst_code}
{xrst_spell_on}
Result Element Index
********************
Index in the Taylor coefficient matrix *ty* of a result matrix element.
{xrst_spell_off}
{xrst_code cpp} */
   size_t result(
      size_t i        , // result matrix row index
      size_t j        , // result matrix column index
      size_t k        , // Taylor coeffocient order
      size_t nk       , // number of Taylor coefficients in ty
      size_t nr_left  , // rows in left matrix
      size_t n_middle , // rows in left and columns in right
      size_t nc_right ) // columns in right matrix
   {  assert( i < nr_left  );
      assert( j < nc_right );
      return (i * nc_right + j) * nk + k;
   }
/* {xrst_code}
{xrst_spell_on}
Forward Matrix Multiply
***********************
Forward mode multiply Taylor coefficients in *tx* and sum into
*ty* (for one pair of left and right orders)
{xrst_spell_off}
{xrst_code cpp} */
   void forward_multiply(
      size_t                 k_left   , // order for left coefficients
      size_t                 k_right  , // order for right coefficients
      const vector<double>&  tx       , // domain space Taylor coefficients
              vector<double>&  ty       , // range space Taylor coefficients
      size_t                 nr_left  , // rows in left matrix
      size_t                 n_middle , // rows in left and columns in right
      size_t                 nc_right ) // columns in right matrix
   {
      size_t nx       = 3 + (nr_left + nc_right) * n_middle;
      size_t nk       = tx.size() / nx;
# ifndef NDEBUG
      size_t ny       = nr_left * nc_right;
      assert( nk == ty.size() / ny );
# endif
      //
      size_t k_result = k_left + k_right;
      assert( k_result < nk );
      //
      for(size_t i = 0; i < nr_left; i++)
      {  for(size_t j = 0; j < nc_right; j++)
         {  double sum = 0.0;
            for(size_t ell = 0; ell < n_middle; ell++)
            {  size_t i_left  = left(
                  i, ell, k_left, nk, nr_left, n_middle, nc_right
               );
               size_t i_right = right(
                  ell, j,  k_right, nk, nr_left, n_middle, nc_right
               );
               sum           += tx[i_left] * tx[i_right];
            }
            size_t i_result = result(
               i, j, k_result, nk, nr_left, n_middle, nc_right
            );
            ty[i_result]   += sum;
         }
      }
   }
/* {xrst_code}
{xrst_spell_on}
Reverse Matrix Multiply
***********************
Reverse mode partials of Taylor coefficients and sum into *px*
(for one pair of left and right orders)
{xrst_spell_off}
{xrst_code cpp} */
   void reverse_multiply(
      size_t                 k_left  , // order for left coefficients
      size_t                 k_right , // order for right coefficients
      const vector<double>&  tx      , // domain space Taylor coefficients
      const vector<double>&  ty      , // range space Taylor coefficients
              vector<double>&  px      , // partials w.r.t. tx
      const vector<double>&  py      , // partials w.r.t. ty
      size_t                 nr_left  , // rows in left matrix
      size_t                 n_middle , // rows in left and columns in right
      size_t                 nc_right ) // columns in right matrix
   {
      size_t nx       = 3 + (nr_left + nc_right) * n_middle;
      size_t nk       = tx.size() / nx;
# ifndef NDEBUG
      size_t ny       = nr_left * nc_right;
      assert( nk == ty.size() / ny );
# endif
      assert( tx.size() == px.size() );
      assert( ty.size() == py.size() );
      //
      size_t k_result = k_left + k_right;
      assert( k_result < nk );
      //
      for(size_t i = 0; i < nr_left; i++)
      {  for(size_t j = 0; j < nc_right; j++)
         {  size_t i_result = result(
               i, j, k_result, nk, nr_left, n_middle, nc_right
            );
            for(size_t ell = 0; ell < n_middle; ell++)
            {  size_t i_left  = left(
                  i, ell, k_left, nk, nr_left, n_middle, nc_right
               );
               size_t i_right = right(
                  ell, j,  k_right, nk, nr_left, n_middle, nc_right
               );
               // sum        += tx[i_left] * tx[i_right];
               px[i_left]    += tx[i_right] * py[i_result];
               px[i_right]   += tx[i_left]  * py[i_result];
            }
         }
      }
      return;
   }
/* {xrst_code}
{xrst_spell_on}
for_type
********
Routine called by CppAD during :ref:`afun(ax, ay)<atomic_three_afun-name>` .
{xrst_spell_off}
{xrst_code cpp} */
   // calculate type_y
   virtual bool for_type(
      const vector<double>&               parameter_x ,
      const vector<CppAD::ad_type_enum>&  type_x      ,
      vector<CppAD::ad_type_enum>&        type_y      )
   {  assert( parameter_x.size() == type_x.size() );
      bool ok = true;
      ok &= type_x[0] == CppAD::constant_enum;
      ok &= type_x[1] == CppAD::constant_enum;
      ok &= type_x[2] == CppAD::constant_enum;
      if( ! ok )
         return false;
      //
      size_t nr_left  = size_t( parameter_x[0] );
      size_t n_middle = size_t( parameter_x[1] );
      size_t nc_right = size_t( parameter_x[2] );
      //
      ok &= type_x.size() == 3 + (nr_left + nc_right) * n_middle;
      ok &= type_y.size() == n_middle * nc_right;
      if( ! ok )
         return false;
      //
      // commpute type_y
      size_t nk = 1; // number of orders
      size_t k  = 0; // order
      for(size_t i = 0; i < nr_left; ++i)
      {  for(size_t j = 0; j < nc_right; ++j)
         {  // compute type for result[i, j]
            CppAD::ad_type_enum type_yij = CppAD::constant_enum;
            for(size_t ell = 0; ell < n_middle; ++ell)
            {  // index for left(i, ell)
               size_t i_left = left(
                  i, ell, k, nk, nr_left, n_middle, nc_right
               );
               // indx for right(ell, j)
               size_t i_right = right(
                  ell, j, k, nk, nr_left, n_middle, nc_right
               );
               // multiplication on left or right by the constant zero
               // always results in a constant
               bool zero_left  = type_x[i_left] == CppAD::constant_enum;
               zero_left      &= parameter_x[i_left] == 0.0;
               bool zero_right = type_x[i_right] == CppAD::constant_enum;
               zero_right     &= parameter_x[i_right] == 0.0;
               if( ! (zero_left | zero_right) )
               {  type_yij = std::max(type_yij, type_x[i_left] );
                  type_yij = std::max(type_yij, type_x[i_right] );
               }
            }
            size_t i_result = result(
               i, j, k, nk, nr_left, n_middle, nc_right
            );
            type_y[i_result] = type_yij;
         }
      }
      return true;
   }
/* {xrst_code}
{xrst_spell_on}
forward
*******
Routine called by CppAD during :ref:`Forward-name` mode.
{xrst_spell_off}
{xrst_code cpp} */
   virtual bool forward(
      const vector<double>&              parameter_x ,
      const vector<CppAD::ad_type_enum>& type_x ,
      size_t                             need_y ,
      size_t                             q      ,
      size_t                             p      ,
      const vector<double>&              tx     ,
      vector<double>&                    ty     )
   {  size_t n_order  = p + 1;
      size_t nr_left  = size_t( tx[ 0 * n_order + 0 ] );
      size_t n_middle = size_t( tx[ 1 * n_order + 0 ] );
      size_t nc_right = size_t( tx[ 2 * n_order + 0 ] );
# ifndef NDEBUG
      size_t nx       = 3 + (nr_left + nc_right) * n_middle;
      size_t ny       = nr_left * nc_right;
# endif
      assert( nx * n_order == tx.size() );
      assert( ny * n_order == ty.size() );
      size_t i, j, ell;

      // initialize result as zero
      size_t k;
      for(i = 0; i < nr_left; i++)
      {  for(j = 0; j < nc_right; j++)
         {  for(k = q; k <= p; k++)
            {  size_t i_result = result(
                  i, j, k, n_order, nr_left, n_middle, nc_right
               );
               ty[i_result] = 0.0;
            }
         }
      }
      for(k = q; k <= p; k++)
      {  // sum the produces that result in order k
         for(ell = 0; ell <= k; ell++)
            forward_multiply(
               ell, k - ell, tx, ty, nr_left, n_middle, nc_right
            );
      }

      // all orders are implemented, so always return true
      return true;
   }
/* {xrst_code}
{xrst_spell_on}
reverse
*******
Routine called by CppAD during :ref:`Reverse-name` mode.
{xrst_spell_off}
{xrst_code cpp} */
   virtual bool reverse(
      const vector<double>&              parameter_x ,
      const vector<CppAD::ad_type_enum>& type_x      ,
      size_t                             p           ,
      const vector<double>&              tx          ,
      const vector<double>&              ty          ,
      vector<double>&                    px          ,
      const vector<double>&              py          )
   {  size_t n_order  = p + 1;
      size_t nr_left  = size_t( tx[ 0 * n_order + 0 ] );
      size_t n_middle = size_t( tx[ 1 * n_order + 0 ] );
      size_t nc_right = size_t( tx[ 2 * n_order + 0 ] );
# ifndef NDEBUG
      size_t nx       = 3 + (nr_left + nc_right) * n_middle;
      size_t ny       = nr_left * nc_right;
# endif
      assert( nx * n_order == tx.size() );
      assert( ny * n_order == ty.size() );
      assert( px.size() == tx.size() );
      assert( py.size() == ty.size() );

      // initialize summation
      for(size_t i = 0; i < px.size(); i++)
         px[i] = 0.0;

      // number of orders to differentiate
      size_t k = n_order;
      while(k--)
      {  // differentiate the produces that result in order k
         for(size_t ell = 0; ell <= k; ell++)
            reverse_multiply(
               ell, k - ell, tx, ty, px, py, nr_left, n_middle, nc_right
            );
      }

      // all orders are implented, so always return true
      return true;
   }
/* {xrst_code}
{xrst_spell_on}
jac_sparsity
************
{xrst_spell_off}
{xrst_code cpp} */
   // Jacobian sparsity routine called by CppAD
   virtual bool jac_sparsity(
      const vector<double>&               parameter_x ,
      const vector<CppAD::ad_type_enum>&  type_x      ,
      bool                                dependency  ,
      const vector<bool>&                 select_x    ,
      const vector<bool>&                 select_y    ,
      CppAD::sparse_rc< vector<size_t> >& pattern_out )
   {
      size_t n = select_x.size();
      size_t m = select_y.size();
      assert( parameter_x.size() == n );
      assert( type_x.size() == n );
      //
      size_t nr_left  = size_t( parameter_x[0] );
      size_t n_middle = size_t( parameter_x[1] );
      size_t nc_right = size_t( parameter_x[2] );
      size_t nk       = 1; // only one order
      size_t k        = 0; // order zero
      //
      // count number of non-zeros in sparsity pattern
      size_t nnz = 0;
      for(size_t i = 0; i < nr_left; ++i)
      {  for(size_t j = 0; j < nc_right; ++j)
         {  size_t i_result = result(
               i, j, k, nk, nr_left, n_middle, nc_right
            );
            if( select_y[i_result] )
            {  for(size_t ell = 0; ell < n_middle; ++ell)
               {  size_t i_left = left(
                     i, ell, k, nk, nr_left, n_middle, nc_right
                  );
                  size_t i_right = right(
                     ell, j, k, nk, nr_left, n_middle, nc_right
                  );
                  bool zero_left  =
                     type_x[i_left] == CppAD::constant_enum;
                  zero_left      &= parameter_x[i_left] == 0.0;
                  bool zero_right =
                     type_x[i_right] == CppAD::constant_enum;
                  zero_right     &= parameter_x[i_right] == 0.0;
                  if( ! (zero_left | zero_right ) )
                  {  bool var_left  =
                        type_x[i_left] == CppAD::variable_enum;
                     bool var_right =
                        type_x[i_right] == CppAD::variable_enum;
                     if( select_x[i_left] & var_left )
                        ++nnz;
                     if( select_x[i_right] & var_right )
                        ++nnz;
                  }
               }
            }
         }
      }
      //
      // fill in the sparsity pattern
      pattern_out.resize(m, n, nnz);
      size_t idx = 0;
      for(size_t i = 0; i < nr_left; ++i)
      {  for(size_t j = 0; j < nc_right; ++j)
         {  size_t i_result = result(
               i, j, k, nk, nr_left, n_middle, nc_right
            );
            if( select_y[i_result] )
            {  for(size_t ell = 0; ell < n_middle; ++ell)
               {  size_t i_left = left(
                     i, ell, k, nk, nr_left, n_middle, nc_right
                  );
                  size_t i_right = right(
                     ell, j, k, nk, nr_left, n_middle, nc_right
                  );
                  bool zero_left  =
                     type_x[i_left] == CppAD::constant_enum;
                  zero_left      &= parameter_x[i_left] == 0.0;
                  bool zero_right =
                     type_x[i_right] == CppAD::constant_enum;
                  zero_right     &= parameter_x[i_right] == 0.0;
                  if( ! (zero_left | zero_right ) )
                  {  bool var_left  =
                        type_x[i_left] == CppAD::variable_enum;
                     bool var_right =
                        type_x[i_right] == CppAD::variable_enum;
                     if( select_x[i_left] & var_left )
                        pattern_out.set(idx++, i_result, i_left);
                     if( select_x[i_right] & var_right )
                        pattern_out.set(idx++, i_result, i_right);
                  }
               }
            }
         }
      }
      assert( idx == nnz );
      //
      return true;
   }
/* {xrst_code}
{xrst_spell_on}
hes_sparsity
************
{xrst_spell_off}
{xrst_code cpp} */
   // Jacobian sparsity routine called by CppAD
   virtual bool hes_sparsity(
      const vector<double>&               parameter_x ,
      const vector<CppAD::ad_type_enum>&  type_x      ,
      const vector<bool>&                 select_x    ,
      const vector<bool>&                 select_y    ,
      CppAD::sparse_rc< vector<size_t> >& pattern_out )
   {
      size_t n = select_x.size();
      assert( parameter_x.size() == n );
      assert( type_x.size() == n );
      //
      size_t nr_left  = size_t( parameter_x[0] );
      size_t n_middle = size_t( parameter_x[1] );
      size_t nc_right = size_t( parameter_x[2] );
      size_t nk       = 1; // only one order
      size_t k        = 0; // order zero
      //
      // count number of non-zeros in sparsity pattern
      size_t nnz = 0;
      for(size_t i = 0; i < nr_left; ++i)
      {  for(size_t j = 0; j < nc_right; ++j)
         {  size_t i_result = result(
               i, j, k, nk, nr_left, n_middle, nc_right
            );
            if( select_y[i_result] )
            {  for(size_t ell = 0; ell < n_middle; ++ell)
               {  // i_left depends on i, ell
                  size_t i_left = left(
                     i, ell, k, nk, nr_left, n_middle, nc_right
                  );
                  // i_right depens on ell, j
                  size_t i_right = right(
                     ell, j, k, nk, nr_left, n_middle, nc_right
                  );
                  bool var_left   = select_x[i_left] &&
                     (type_x[i_left] == CppAD::variable_enum);
                  bool var_right  = select_x[i_right] &&
                     (type_x[i_right] == CppAD::variable_enum);
                  if( var_left & var_right )
                        nnz += 2;
               }
            }
         }
      }
      //
      // fill in the sparsity pattern
      pattern_out.resize(n, n, nnz);
      size_t idx = 0;
      for(size_t i = 0; i < nr_left; ++i)
      {  for(size_t j = 0; j < nc_right; ++j)
         {  size_t i_result = result(
               i, j, k, nk, nr_left, n_middle, nc_right
            );
            if( select_y[i_result] )
            {  for(size_t ell = 0; ell < n_middle; ++ell)
               {  size_t i_left = left(
                     i, ell, k, nk, nr_left, n_middle, nc_right
                  );
                  size_t i_right = right(
                     ell, j, k, nk, nr_left, n_middle, nc_right
                  );
                  bool var_left   = select_x[i_left] &&
                     (type_x[i_left] == CppAD::variable_enum);
                  bool var_right  = select_x[i_right] &&
                     (type_x[i_right] == CppAD::variable_enum);
                  if( var_left & var_right )
                  {  // Cannot possibly set the same (i_left, i_right)
                     // pair twice.
                     assert( i_left != i_right );
                     pattern_out.set(idx++, i_left, i_right);
                     pattern_out.set(idx++, i_right, i_left);
                  }
               }
            }
         }
      }
      assert( idx == nnz );
      //
      return true;
   }
/* {xrst_code}
{xrst_spell_on}
rev_depend
**********
Routine called when a function using ``mat_mul`` is optimized.
{xrst_spell_off}
{xrst_code cpp} */
   // calculate depend_x
   virtual bool rev_depend(
      const vector<double>&              parameter_x ,
      const vector<CppAD::ad_type_enum>& type_x      ,
      vector<bool>&                      depend_x    ,
      const vector<bool>&                depend_y    )
   {  assert( parameter_x.size() == depend_x.size() );
      assert( parameter_x.size() == type_x.size() );
      bool ok = true;
      //
      size_t nr_left  = size_t( parameter_x[0] );
      size_t n_middle = size_t( parameter_x[1] );
      size_t nc_right = size_t( parameter_x[2] );
      //
      ok &= depend_x.size() == 3 + (nr_left + nc_right) * n_middle;
      ok &= depend_y.size() == n_middle * nc_right;
      if( ! ok )
         return false;
      //
      // initialize depend_x
      for(size_t ell = 0; ell < 3; ++ell)
         depend_x[ell] = true; // always need these parameters
      for(size_t ell = 3; ell < depend_x.size(); ++ell)
         depend_x[ell] = false; // initialize as false
      //
      // commpute depend_x
      size_t nk = 1; // number of orders
      size_t k  = 0; // order
      for(size_t i = 0; i < nr_left; ++i)
      {  for(size_t j = 0; j < nc_right; ++j)
         {  // check depend for result[i, j]
            size_t i_result = result(
               i, j, k, nk, nr_left, n_middle, nc_right
            );
            if( depend_y[i_result] )
            {  for(size_t ell = 0; ell < n_middle; ++ell)
               {  // index for left(i, ell)
                  size_t i_left = left(
                     i, ell, k, nk, nr_left, n_middle, nc_right
                  );
                  // indx for right(ell, j)
                  size_t i_right = right(
                     ell, j, k, nk, nr_left, n_middle, nc_right
                  );
                  bool zero_left  =
                     type_x[i_left] == CppAD::constant_enum;
                  zero_left      &= parameter_x[i_left] == 0.0;
                  bool zero_right =
                     type_x[i_right] == CppAD::constant_enum;
                  zero_right     &= parameter_x[i_right] == 0.0;
                  if( ! zero_right )
                     depend_x[i_left]  = true;
                  if( ! zero_left )
                     depend_x[i_right] = true;
               }
            }
         }
      }
      return true;
   }
/* {xrst_code}
{xrst_spell_on}
End Class Definition
********************
{xrst_spell_off}
{xrst_code cpp} */
}; // End of mat_mul class
}  // End empty namespace
/* {xrst_code}
{xrst_spell_on}

{xrst_end atomic_three_mat_mul.hpp}
*/


# endif
