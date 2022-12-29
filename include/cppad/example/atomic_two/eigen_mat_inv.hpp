# ifndef CPPAD_EXAMPLE_ATOMIC_TWO_EIGEN_MAT_INV_HPP
# define CPPAD_EXAMPLE_ATOMIC_TWO_EIGEN_MAT_INV_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin atomic_two_eigen_mat_inv.hpp app}
{xrst_spell
   tr
}

atomic_two Eigen Matrix Inversion Class
#######################################

Purpose
*******
Construct an atomic operation that computes the matrix inverse
:math:`R = A^{-1}`
for any positive integer :math:`p`
and invertible matrix :math:`A \in \B{R}^{p \times p}`.

Matrix Dimensions
*****************
This example puts the matrix dimension :math:`p`
in the atomic function arguments,
instead of the :ref:`constructor<atomic_two_ctor-name>` ,
so it can be different for different calls to the atomic function.

Theory
******

Forward
=======
The zero order forward mode Taylor coefficient is give by

.. math::

   R_0 = A_0^{-1}

For :math:`k = 1 , \ldots`,
the *k*-th order Taylor coefficient of :math:`A R` is given by

.. math::

   0 = \sum_{\ell=0}^k A_\ell R_{k-\ell}

Solving for :math:`R_k` in terms of the coefficients
for :math:`A` and the lower order coefficients for :math:`R` we have

.. math::

   R_k = - R_0 \left( \sum_{\ell=1}^k A_\ell R_{k-\ell} \right)

Furthermore, once we have :math:`R_k` we can compute the sum using

.. math::

   A_0 R_k = - \left( \sum_{\ell=1}^k A_\ell R_{k-\ell} \right)

Product of Three Matrices
=========================
Suppose :math:`\bar{E}` is the derivative of the
scalar value function :math:`s(E)` with respect to :math:`E`; i.e.,

.. math::

   \bar{E}_{i,j} = \frac{ \partial s } { \partial E_{i,j} }

Also suppose that :math:`t` is a scalar valued argument and

.. math::

   E(t) = B(t) C(t) D(t)

It follows that

.. math::

   E'(t) = B'(t) C(t) D(t) + B(t) C'(t) D(t) +  B(t) C(t) D'(t)

.. math::

   (s \circ E)'(t)
   =
   \R{tr} [ \bar{E}^\R{T} E'(t) ]

.. math::

   =
   \R{tr} [ \bar{E}^\R{T} B'(t) C(t) D(t) ] +
   \R{tr} [ \bar{E}^\R{T} B(t) C'(t) D(t) ] +
   \R{tr} [ \bar{E}^\R{T} B(t) C(t) D'(t) ]

.. math::

   =
   \R{tr} [ B(t) D(t) \bar{E}^\R{T} B'(t) ] +
   \R{tr} [ D(t) \bar{E}^\R{T} B(t) C'(t) ] +
   \R{tr} [ \bar{E}^\R{T} B(t) C(t) D'(t) ]

.. math::

   \bar{B} = \bar{E} (C D)^\R{T} \W{,}
   \bar{C} = \B{R}^\R{T} \bar{E} D^\R{T} \W{,}
   \bar{D} = (B C)^\R{T} \bar{E}

Reverse
=======
For :math:`k > 0`, reverse mode
eliminates :math:`R_k` and expresses the function values
:math:`s` in terms of the coefficients of :math:`A`
and the lower order coefficients of :math:`R`.
The effect on :math:`\bar{R}_0`
(of eliminating :math:`R_k`) is

.. math::

   \bar{R}_0
   = \bar{R}_0 - \bar{R}_k \left( \sum_{\ell=1}^k A_\ell R_{k-\ell} \right)^\R{T}
   = \bar{R}_0 + \bar{R}_k ( A_0 R_k )^\R{T}

For :math:`\ell = 1 , \ldots , k`,
the effect on :math:`\bar{R}_{k-\ell}` and :math:`A_\ell`
(of eliminating :math:`R_k`) is

.. math::

   \bar{A}_\ell = \bar{A}_\ell - R_0^\R{T} \bar{R}_k R_{k-\ell}^\R{T}

.. math::

   \bar{R}_{k-\ell} = \bar{R}_{k-\ell} - ( R_0 A_\ell )^\R{T} \bar{R}_k

We note that

.. math::

   R_0 '(t) A_0 (t) + R_0 (t) A_0 '(t) = 0

.. math::

   R_0 '(t) = - R_0 (t) A_0 '(t) R_0 (t)

The reverse mode formula that eliminates :math:`R_0` is

.. math::

   \bar{A}_0
   = \bar{A}_0 - R_0^\R{T} \bar{R}_0 R_0^\R{T}

Start Class Definition
**********************
{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/cppad.hpp>
# include <Eigen/Core>
# include <Eigen/LU>

/* {xrst_code}
{xrst_spell_on}
Public
******

Types
=====
{xrst_spell_off}
{xrst_code cpp} */
namespace { // BEGIN_EMPTY_NAMESPACE

template <class Base>
class atomic_eigen_mat_inv : public CppAD::atomic_base<Base> {
public:
   // -----------------------------------------------------------
   // type of elements during calculation of derivatives
   typedef Base              scalar;
   // type of elements during taping
   typedef CppAD::AD<scalar> ad_scalar;
   // type of matrix during calculation of derivatives
   typedef Eigen::Matrix<
      scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>     matrix;
   // type of matrix during taping
   typedef Eigen::Matrix<
      ad_scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor > ad_matrix;
/* {xrst_code}
{xrst_spell_on}
Constructor
===========
{xrst_spell_off}
{xrst_code cpp} */
   // constructor
   atomic_eigen_mat_inv(void) : CppAD::atomic_base<Base>(
      "atom_eigen_mat_inv"                             ,
      CppAD::atomic_base<Base>::set_sparsity_enum
   )
   { }
/* {xrst_code}
{xrst_spell_on}
op
==
{xrst_spell_off}
{xrst_code cpp} */
   // use atomic operation to invert an AD matrix
   ad_matrix op(const ad_matrix& arg)
   {  size_t nr = size_t( arg.rows() );
      size_t ny = nr * nr;
      size_t nx = 1 + ny;
      assert( nr == size_t( arg.cols() ) );
      // -------------------------------------------------------------------
      // packed version of arg
      CPPAD_TESTVECTOR(ad_scalar) packed_arg(nx);
      packed_arg[0] = ad_scalar( nr );
      for(size_t i = 0; i < ny; i++)
         packed_arg[1 + i] = arg.data()[i];
      // -------------------------------------------------------------------
      // packed version of result = arg^{-1}.
      // This is an atomic_base function call that CppAD uses to
      // store the atomic operation on the tape.
      CPPAD_TESTVECTOR(ad_scalar) packed_result(ny);
      (*this)(packed_arg, packed_result);
      // -------------------------------------------------------------------
      // unpack result matrix
      ad_matrix result(nr, nr);
      for(size_t i = 0; i < ny; i++)
         result.data()[i] = packed_result[i];
      return result;
   }
   /* {xrst_code}
{xrst_spell_on}
Private
*******

Variables
=========
{xrst_spell_off}
{xrst_code cpp} */
private:
   // -------------------------------------------------------------
   // one forward mode vector of matrices for argument and result
   CppAD::vector<matrix> f_arg_, f_result_;
   // one reverse mode vector of matrices for argument and result
   CppAD::vector<matrix> r_arg_, r_result_;
   // -------------------------------------------------------------
/* {xrst_code}
{xrst_spell_on}
forward
=======
{xrst_spell_off}
{xrst_code cpp} */
   // forward mode routine called by CppAD
   virtual bool forward(
      // lowest order Taylor coefficient we are evaluating
      size_t                          p ,
      // highest order Taylor coefficient we are evaluating
      size_t                          q ,
      // which components of x are variables
      const CppAD::vector<bool>&      vx ,
      // which components of y are variables
      CppAD::vector<bool>&            vy ,
      // tx [ j * (q+1) + k ] is x_j^k
      const CppAD::vector<scalar>&    tx ,
      // ty [ i * (q+1) + k ] is y_i^k
      CppAD::vector<scalar>&          ty
   )
   {  size_t n_order = q + 1;
      size_t nr      = size_t( CppAD::Integer( tx[ 0 * n_order + 0 ] ) );
      size_t ny      = nr * nr;
# ifndef NDEBUG
      size_t nx      = 1 + ny;
# endif
      assert( vx.size() == 0 || nx == vx.size() );
      assert( vx.size() == 0 || ny == vy.size() );
      assert( nx * n_order == tx.size() );
      assert( ny * n_order == ty.size() );
      //
      // -------------------------------------------------------------------
      // make sure f_arg_ and f_result_ are large enough
      assert( f_arg_.size() == f_result_.size() );
      if( f_arg_.size() < n_order )
      {  f_arg_.resize(n_order);
         f_result_.resize(n_order);
         //
         for(size_t k = 0; k < n_order; k++)
         {  f_arg_[k].resize( long(nr), long(nr) );
            f_result_[k].resize( long(nr), long(nr) );
         }
      }
      // -------------------------------------------------------------------
      // unpack tx into f_arg_
      for(size_t k = 0; k < n_order; k++)
      {  // unpack arg values for this order
         for(size_t i = 0; i < ny; i++)
            f_arg_[k].data()[i] = tx[ (1 + i) * n_order + k ];
      }
      // -------------------------------------------------------------------
      // result for each order
      // (we could avoid recalculting f_result_[k] for k=0,...,p-1)
      //
      f_result_[0] = f_arg_[0].inverse();
      for(size_t k = 1; k < n_order; k++)
      {  // initialize sum
         matrix f_sum = matrix::Zero( long(nr), long(nr) );
         // compute sum
         for(size_t ell = 1; ell <= k; ell++)
            f_sum -= f_arg_[ell] * f_result_[k-ell];
         // result_[k] = arg_[0]^{-1} * sum_
         f_result_[k] = f_result_[0] * f_sum;
      }
      // -------------------------------------------------------------------
      // pack result_ into ty
      for(size_t k = 0; k < n_order; k++)
      {  for(size_t i = 0; i < ny; i++)
            ty[ i * n_order + k ] = f_result_[k].data()[i];
      }
      // -------------------------------------------------------------------
      // check if we are computing vy
      if( vx.size() == 0 )
         return true;
      // ------------------------------------------------------------------
      // This is a very dumb algorithm that over estimates which
      // elements of the inverse are variables (which is not efficient).
      bool var = false;
      for(size_t i = 0; i < ny; i++)
         var |= vx[1 + i];
      for(size_t i = 0; i < ny; i++)
         vy[i] = var;
      return true;
   }
/* {xrst_code}
{xrst_spell_on}
reverse
=======
{xrst_spell_off}
{xrst_code cpp} */
   // reverse mode routine called by CppAD
   virtual bool reverse(
      // highest order Taylor coefficient that we are computing derivative of
      size_t                     q ,
      // forward mode Taylor coefficients for x variables
      const CppAD::vector<double>&     tx ,
      // forward mode Taylor coefficients for y variables
      const CppAD::vector<double>&     ty ,
      // upon return, derivative of G[ F[ {x_j^k} ] ] w.r.t {x_j^k}
      CppAD::vector<double>&           px ,
      // derivative of G[ {y_i^k} ] w.r.t. {y_i^k}
      const CppAD::vector<double>&     py
   )
   {  size_t n_order = q + 1;
      size_t nr      = size_t( CppAD::Integer( tx[ 0 * n_order + 0 ] ) );
      size_t ny      = nr * nr;
# ifndef NDEBUG
      size_t nx      = 1 + ny;
# endif
      //
      assert( nx * n_order == tx.size() );
      assert( ny * n_order == ty.size() );
      assert( px.size()    == tx.size() );
      assert( py.size()    == ty.size() );
      // -------------------------------------------------------------------
      // make sure f_arg_ is large enough
      assert( f_arg_.size() == f_result_.size() );
      // must have previous run forward with order >= n_order
      assert( f_arg_.size() >= n_order );
      // -------------------------------------------------------------------
      // make sure r_arg_, r_result_ are large enough
      assert( r_arg_.size() == r_result_.size() );
      if( r_arg_.size() < n_order )
      {  r_arg_.resize(n_order);
         r_result_.resize(n_order);
         //
         for(size_t k = 0; k < n_order; k++)
         {  r_arg_[k].resize( long(nr), long(nr) );
            r_result_[k].resize( long(nr), long(nr) );
         }
      }
      // -------------------------------------------------------------------
      // unpack tx into f_arg_
      for(size_t k = 0; k < n_order; k++)
      {  // unpack arg values for this order
         for(size_t i = 0; i < ny; i++)
            f_arg_[k].data()[i] = tx[ (1 + i) * n_order + k ];
      }
      // -------------------------------------------------------------------
      // unpack py into r_result_
      for(size_t k = 0; k < n_order; k++)
      {  for(size_t i = 0; i < ny; i++)
            r_result_[k].data()[i] = py[ i * n_order + k ];
      }
      // -------------------------------------------------------------------
      // initialize r_arg_ as zero
      for(size_t k = 0; k < n_order; k++)
         r_arg_[k]   = matrix::Zero( long(nr), long(nr) );
      // -------------------------------------------------------------------
      // matrix reverse mode calculation
      //
      for(size_t k1 = n_order; k1 > 1; k1--)
      {  size_t k = k1 - 1;
         // bar{R}_0 = bar{R}_0 + bar{R}_k (A_0 R_k)^T
         r_result_[0] +=
         r_result_[k] * f_result_[k].transpose() * f_arg_[0].transpose();
         //
         for(size_t ell = 1; ell <= k; ell++)
         {  // bar{A}_l = bar{A}_l - R_0^T bar{R}_k R_{k-l}^T
            r_arg_[ell] -= f_result_[0].transpose()
               * r_result_[k] * f_result_[k-ell].transpose();
            // bar{R}_{k-l} = bar{R}_{k-1} - (R_0 A_l)^T bar{R}_k
            r_result_[k-ell] -= f_arg_[ell].transpose()
               * f_result_[0].transpose() * r_result_[k];
         }
      }
      r_arg_[0] -=
      f_result_[0].transpose() * r_result_[0] * f_result_[0].transpose();
      // -------------------------------------------------------------------
      // pack r_arg into px
      for(size_t k = 0; k < n_order; k++)
      {  for(size_t i = 0; i < ny; i++)
            px[ (1 + i) * n_order + k ] = r_arg_[k].data()[i];
      }
      //
      return true;
   }
/* {xrst_code}
{xrst_spell_on}
End Class Definition
********************
{xrst_spell_off}
{xrst_code cpp} */
}; // End of atomic_eigen_mat_inv class

}  // END_EMPTY_NAMESPACE
/* {xrst_code}
{xrst_spell_on}

{xrst_end atomic_two_eigen_mat_inv.hpp}
*/


# endif
