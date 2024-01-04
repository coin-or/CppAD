// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin atomic_four_norm_sq.cpp}
{xrst_spell
   euclidean
}

Atomic Euclidean Norm Squared: Example and Test
###############################################

Function
********
This example demonstrates using :ref:`atomic_four-name`
to define the operation
:math:`g : \B{R}^n \rightarrow \B{R}` where

.. math::

   g(x) =  x_0^2 + \cdots + x_{n-1}^2

Purpose
*******
This atomic function demonstrates the following cases:

#. an arbitrary number of arguments *n*
#. zero and first order forward mode.
#. first order derivatives using reverse mode.

Define Atomic Function
**********************
{xrst_literal
   // BEGIN_DEFINE_ATOMIC_FUNCTION
   // END_DEFINE_ATOMIC_FUNCTION
}

Use Atomic Function
*******************
{xrst_literal
   // BEGIN_USE_ATOMIC_FUNCTION
   // END_USE_ATOMIC_FUNCTION
}

{xrst_end atomic_four_norm_sq.cpp}
*/
# include <cppad/cppad.hpp>

// BEGIN_DEFINE_ATOMIC_FUNCTION
// empty namespace
namespace {
   // BEGIN CONSTRUCTOR
   class atomic_norm_sq : public CppAD::atomic_four<double> {
   public:
      atomic_norm_sq(const std::string& name) :
      CppAD::atomic_four<double>(name)
      { }
   // END CONSTRUCTOR
   private:
      // BEGIN FOR_TYPE
      bool for_type(
         size_t                                     call_id     ,
         const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
         CppAD::vector<CppAD::ad_type_enum>&        type_y      ) override
      {  assert( call_id == 0 );       // default value
         assert(type_y.size() == 1 );  // m
         //
         // type_y
         size_t n     = type_x.size();
         type_y[0] = CppAD::constant_enum;
         for(size_t j = 0; j < n; ++j)
            type_y[0] = std::max(type_y[0], type_x[j]);
         return true;
      }
      // END FOR_TYPE
      // BEGIN FORWARD
      bool forward(
         size_t                             call_id     ,
         const CppAD::vector<bool>&         select_y    ,
         size_t                             order_low   ,
         size_t                             order_up    ,
         const CppAD::vector<double>&       tx          ,
         CppAD::vector<double>&             ty          ) override
      {
         size_t q = order_up + 1;
         size_t n = tx.size() / q;
   # ifndef NDEBUG
         size_t m = ty.size() / q;
         assert( call_id == 0 );
         assert( m == 1 );
         assert( m == select_y.size() );
   # endif
         // ok
         bool ok = order_up <= 1 && order_low <= order_up;
         if ( ! ok )
            return ok;
         //
         // sum = x_0^0 * x_0^0 + x_1^0 * x_1^0 + ...
         double sum = 0.0;
         for(size_t j = 0; j < n; ++j)
         {  double xj0 = tx[ j * q + 0];
            sum       += xj0 * xj0;
         }
         //
         // ty[0] = sum
         if( order_low <= 0 )
            ty[0] = sum;
         if( order_up < 1 )
            return ok;

         // sum = x_0^0 * x_0^1 + x_1^0 ^ x_1^1 + ...
         sum   = 0.0;
         for(size_t j = 0; j < n; ++j)
         {  double xj0 = tx[ j * q + 0];
            double xj1 = tx[ j * q + 1];
            sum       += xj0 * xj1;
         }
         // ty[1] = 2.0 * sum
         assert( order_up == 1 );
         ty[1] = 2.0 * sum;
         return ok;

         // Assume we are not using forward mode with order > 1
         assert( ! ok );
         return ok;
      }
      // END FORWARD
      // BEGIN REVERSE
      bool reverse(
         size_t                              call_id     ,
         const CppAD::vector<bool>&          select_x    ,
         size_t                              order_up    ,
         const CppAD::vector<double>&        tx          ,
         const CppAD::vector<double>&        ty          ,
         CppAD::vector<double>&              px          ,
         const CppAD::vector<double>&        py          ) override
      {
         size_t q = order_up + 1;
         size_t n = tx.size() / q;
   # ifndef NDEBUG
         size_t m = ty.size() / q;
         assert( call_id == 0 );
         assert( m == 1 );
         assert( px.size() == tx.size() );
         assert( py.size() == ty.size() );
         assert( n == select_x.size() );
   # endif
         // ok
         bool ok = order_up == 0;
         if ( ! ok )
            return ok;

         // first order reverse mode
         for(size_t j = 0; j < n; ++j)
         {  // x_0^0
            double xj0 = tx[ j * q + 0];
            //
            // H = G( F( { x_j^k } ) )
            double dF = 2.0 * xj0; // partial F w.r.t x_j^0
            double dG = py[0];     // partial of G w.r.t. y[0]
            double dH = dG * dF;   // partial of H w.r.t. x_j^0

            // px[j]
            px[j] = dH;
         }
         return ok;
      }
      // END REVERSE
      // BEGIN JAC_SPARSITY
      // Use deprecated version of this callback to test that is still works
      // (missing the ident_zero_x argument).
      bool jac_sparsity(
         size_t                                     call_id     ,
         bool                                       dependency  ,
         // const CppAD::vector<bool>&              ident_zero_x,
         const CppAD::vector<bool>&                 select_x    ,
         const CppAD::vector<bool>&                 select_y    ,
         CppAD::sparse_rc< CppAD::vector<size_t> >& pattern_out ) override
      {  size_t n = select_x.size();
         size_t m = select_y.size();
# ifndef NDEBUG
         assert( call_id == 0 );
         assert( m == 1 );
# endif
         // nnz
         size_t nnz = 0;
         if( select_y[0] )
         {  for(size_t j = 0; j < n; ++j)
            {  if( select_x[j] )
                  ++nnz;
            }
         }
         // pattern_out
         pattern_out.resize(m, n, nnz);
         size_t k = 0;
         if( select_y[0] )
         {  for(size_t j = 0; j < n; ++j)
            {  if( select_x[j] )
                  pattern_out.set(k++, 0, j);
            }
         }
         assert( k == nnz );
         return true;
      }
      // END JAC_SPARSITY
      // BEGIN HES_SPARSITY
      // Use deprecated version of this callback to test that is still works
      // (missing the ident_zero_x argument).
      bool hes_sparsity(
         size_t                                     call_id     ,
         // const CppAD::vector<bool>&              ident_zero_x,
         const CppAD::vector<bool>&                 select_x    ,
         const CppAD::vector<bool>&                 select_y    ,
         CppAD::sparse_rc< CppAD::vector<size_t> >& pattern_out ) override
      {  size_t n = select_x.size();
# ifndef NDEBUG
         size_t m = select_y.size();
         assert( call_id == 0 );
         assert( m == 1 );
# endif
         // nnz
         size_t nnz = 0;
         if( select_y[0] )
         {  for(size_t j = 0; j < n; ++j)
            {  if( select_x[j] )
                  ++nnz;
            }
         }
         // pattern_out
         pattern_out.resize(n, n, nnz);
         size_t k = 0;
         if( select_y[0] )
         {  for(size_t j = 0; j < n; ++j)
            {  if( select_x[j] )
                  pattern_out.set(k++, j, j);
            }
         }
         return true;
      }
      // END HES_SPARSITY
      // BEGIN REV_DEPEND
      bool rev_depend(
         size_t                                     call_id     ,
         CppAD::vector<bool>&                       depend_x    ,
         const CppAD::vector<bool>&                 depend_y    ) override
      {  size_t n = depend_x.size();
# ifndef NDEBUG
         size_t m = depend_y.size();
         assert( call_id == 0 );
         assert( m == 1 );
# endif
         for(size_t j = 0; j < n; ++j)
            depend_x[j] = depend_y[0];
         //
         return true;
      }
      // END REV_DEPEND
   };
}
// END_DEFINE_ATOMIC_FUNCTION

// BEGIN_USE_ATOMIC_FUNCTION
bool norm_sq(void)
{  // ok, eps
   bool ok    = true;
   double eps = 10. * CppAD::numeric_limits<double>::epsilon();
   //
   // atom_norm_sq
   atomic_norm_sq afun("atomic_norm_sq");
   //
   // n, m
   size_t n = 2;
   size_t m = 1;
   //
   // x
   CPPAD_TESTVECTOR(double) x(n);
   for(size_t j = 0; j < n; ++j)
      x[j] = 1.0 / (double(j) + 1.0);
   //
   // ax
   CPPAD_TESTVECTOR( CppAD::AD<double> ) ax(n);
   for(size_t j = 0; j < n; ++j)
      ax[j] = x[j];
   CppAD::Independent(ax);
   //
   // ay
   CPPAD_TESTVECTOR( CppAD::AD<double> ) ay(m);
   afun(ax, ay);
   //
   // f
   CppAD::ADFun<double> f;
   f.Dependent (ax, ay);
   //
   // check
   double check = 0.0;
   for(size_t j = 0; j < n; ++j)
      check += x[j] * x[j];
   //
   // ok
   // check ay[0]
   ok &= CppAD::NearEqual( Value(ay[0]) , check,  eps, eps);
   //
   // ok
   // check zero order forward mode
   CPPAD_TESTVECTOR(double) y(m);
   y   = f.Forward(0, x);
   ok &= CppAD::NearEqual(y[0] , check,  eps, eps);
   //
   // n2, check
   size_t n2  = n / 2;
   check      = 2.0 * x[n2];
   //
   // ok
   // check first order forward mode partial w.r.t. x[n2]
   CPPAD_TESTVECTOR(double) x1(n), y1(m);
   for(size_t j = 0; j < n; ++j)
         x1[j] = 0.0;
   x1[n2] = 1.0;
   y1     = f.Forward(1, x1);
   ok    &= CppAD::NearEqual(y1[0] , check,  eps, eps);
   //
   // ok
   // first order reverse mode
   size_t q = 1;
   CPPAD_TESTVECTOR(double)  w(m), dw(n * q);
   w[0]  = 1.;
   dw    = f.Reverse(q, w);
   for(size_t j = 0; j < n; ++j)
   {  check = 2.0 * x[j];
      ok &= CppAD::NearEqual(dw[j] , check,  eps, eps);
   }
   //
   // pattern_out
   // reverse mode Jacobian sparstiy pattern
   CppAD::sparse_rc< CPPAD_TESTVECTOR(size_t) > pattern_in, pattern_out;
   pattern_in.resize(m, m, m);
   for(size_t i = 0; i < m; ++i)
      pattern_in.set(i, i, i);
   bool transpose     = false;
   bool dependency    = false;
   bool internal_bool = false;
   f.rev_jac_sparsity(
      pattern_in, transpose, dependency, internal_bool, pattern_out
   );
   //
   // ok
   ok &= pattern_out.nnz() == n;
   CPPAD_TESTVECTOR(size_t) row_major  = pattern_out.row_major();
   for(size_t j = 0; j < n; ++j)
   {  size_t r = pattern_out.row()[ row_major[j] ];
      size_t c = pattern_out.col()[ row_major[j] ];
      ok      &= r == 0 && c == j;
   }
   //
   // pattern_out
   // forward mode Hessian sparsity pattern
   CPPAD_TESTVECTOR(bool) select_x(n), select_y(m);
   for(size_t j = 0; j < n; ++j)
      select_x[j] = true;
   for(size_t i = 0; i < m; ++i)
      select_y[i] = true;
   internal_bool = false;
   f.for_hes_sparsity(
      select_x, select_y, internal_bool, pattern_out
   );
   //
   // ok
   ok &= pattern_out.nnz() == n;
   row_major  = pattern_out.row_major();
   for(size_t j = 0; j < n; ++j)
   {  size_t r   = pattern_out.row()[ row_major[j] ];
      size_t c   = pattern_out.col()[ row_major[j] ];
      ok        &= r == j && c == j;
   }
   //
   // optimize
   // this uses the rev_depend overide above
   f.optimize("val_graph no_conditional_skip");
   //
   // ok
   // check zero order forward mode (on optimized version of f)
   y     = f.Forward(0, x);
   check = 0.0;
   for(size_t j = 0; j < n; ++j)
      check += x[j] * x[j];
   ok &= CppAD::NearEqual(y[0] , check,  eps, eps);
   //
   return ok;
}
// END_USE_ATOMIC_FUNCTION
