// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_forward.cpp}

Atomic Functions and Forward Mode: Example and Test
###################################################

Purpose
*******
This example demonstrates forward mode derivative calculation
using an :ref:`atomic_four-name` function.

Function
********
For this example, the atomic function
:math:`g : \B{R}^3 \rightarrow \B{R}^2` is defined by

.. math::

   g(x) = \left( \begin{array}{c}
      x_2 * x_2 \\
      x_0 * x_1
   \end{array} \right)

Jacobian
********
The corresponding Jacobian is

.. math::

   g^{(1)} (x) = \left( \begin{array}{ccc}
     0  &   0 & 2 x_2 \\
   x_1  & x_0 & 0
   \end{array} \right)

Hessian
*******
The Hessians of the component functions are

.. math::

   g_0^{(2)} ( x ) = \left( \begin{array}{ccc}
      0 & 0 & 0  \\
      0 & 0 & 0  \\
      0 & 0 & 2
   \end{array} \right)
   \W{,}
   g_1^{(2)} ( x ) = \left( \begin{array}{ccc}
      0 & 1 & 0 \\
      1 & 0 & 0 \\
      0 & 0 & 0
   \end{array} \right)

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

{xrst_end atomic_four_forward.cpp}
*/
# include <cppad/cppad.hpp>

// BEGIN_DEFINE_ATOMIC_FUNCTION
// empty namespace
namespace {
   //
   class atomic_forward : public CppAD::atomic_four<double> {
   public:
      atomic_forward(const std::string& name) :
      CppAD::atomic_four<double>(name)
      { }
   private:
      // for_type
      bool for_type(
         size_t                                     call_id     ,
         const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
         CppAD::vector<CppAD::ad_type_enum>&        type_y      ) override
      {
         bool ok = type_x.size() == 3; // n
         ok     &= type_y.size() == 2; // m
         if( ! ok )
            return false;
         type_y[0] = type_x[2];
         type_y[1] = std::max(type_x[0], type_x[1]);
         return true;
      }
      // forward
      bool forward(
         size_t                                    call_id      ,
         const CppAD::vector<bool>&                select_y     ,
         size_t                                    order_low    ,
         size_t                                    order_up     ,
         const CppAD::vector<double>&              tx           ,
         CppAD::vector<double>&                    ty           ) override
      {
         size_t q = order_up + 1;
# ifndef NDEBUG
         size_t n = tx.size() / q;
         size_t m = ty.size() / q;
# endif
         assert( n == 3 );
         assert( m == 2 );
         assert( order_low <= order_up );

         // this example only implements up to second order forward mode
         bool ok = order_up <=  2;
         if( ! ok )
            return ok;

         // --------------------------------------------------------------
         // Zero forward mode.
         // This case must always be implemented
         // g(x) = [ x_2 * x_2 ]
         //        [ x_0 * x_1 ]
         // y^0  = f( x^0 )
         if( order_low <= 0 )
         {  // y_0^0 = x_2^0 * x_2^0
            ty[0 * q + 0] = tx[2 * q + 0] * tx[2 * q + 0];
            // y_1^0 = x_0^0 * x_1^0
            ty[1 * q + 0] = tx[0 * q + 0] * tx[1 * q + 0];
         }
         if( order_up <=  0 )
            return ok;
         // --------------------------------------------------------------
         // First order forward mode.
         // This case is needed if first order forward mode is used.
         // g'(x) = [   0,   0, 2 * x_2 ]
         //         [ x_1, x_0,       0 ]
         // y^1 =  f'(x^0) * x^1
         if( order_low <= 1 )
         {  // y_0^1 = 2 * x_2^0 * x_2^1
            ty[0 * q + 1] = 2.0 * tx[2 * q + 0] * tx[2 * q + 1];
            // y_1^1 = x_1^0 * x_0^1 + x_0^0 * x_1^1
            ty[1 * q + 1]  = tx[1 * q + 0] * tx[0 * q + 1];
            ty[1 * q + 1] += tx[0 * q + 0] * tx[1 * q + 1];
         }
         if( order_up <=  1 )
            return ok;
         // --------------------------------------------------------------
         // Second order forward mode.
         // This case is needed if second order forwrd mode is used.
         // g'(x) = [   0,   0, 2 x_2 ]
         //         [ x_1, x_0,     0 ]
         //
         //            [ 0 , 0 , 0 ]                  [ 0 , 1 , 0 ]
         // g_0''(x) = [ 0 , 0 , 0 ]  g_1^{(2)} (x) = [ 1 , 0 , 0 ]
         //            [ 0 , 0 , 2 ]                  [ 0 , 0 , 0 ]
         //
         //  y_0^2 = x^1 * g_0''( x^0 ) x^1 / 2! + g_0'( x^0 ) x^2
         //        = ( x_2^1 * 2.0 * x_2^1 ) / 2!
         //        + 2.0 * x_2^0 * x_2^2
         ty[0 * q + 2]  = tx[2 * q + 1] * tx[2 * q + 1];
         ty[0 * q + 2] += 2.0 * tx[2 * q + 0] * tx[2 * q + 2];
         //
         //  y_1^2 = x^1 * g_1''( x^0 ) x^1 / 2! + g_1'( x^0 ) x^2
         //        = ( x_1^1 * x_0^1 + x_0^1 * x_1^1) / 2
         //        + x_1^0 * x_0^2 + x_0^0 + x_1^2
         ty[1 * q + 2]  = tx[1 * q + 1] * tx[0 * q + 1];
         ty[1 * q + 2] += tx[1 * q + 0] * tx[0 * q + 2];
         ty[1 * q + 2] += tx[0 * q + 0] * tx[1 * q + 2];
         // --------------------------------------------------------------
         return ok;
      }
   };
}
// END_DEFINE_ATOMIC_FUNCTION

// BEGIN_USE_ATOMIC_FUNCTION
bool forward(void)
{  // ok, eps
   bool ok = true;
   double eps = 10. * CppAD::numeric_limits<double>::epsilon();
   //
   // AD, NearEqual
   using CppAD::AD;
   using CppAD::NearEqual;
   //
   // afun
   atomic_forward afun("atomic_forward");
   //
   // Create the function f(u) = g(u) for this example.
   //
   // n, u, au
   size_t n  = 3;
   CPPAD_TESTVECTOR(double)       u(n);
   u[0] = 1.00;
   u[1] = 2.00;
   u[2] = 3.00;
   CPPAD_TESTVECTOR( AD<double> ) au(n);
   for(size_t j = 0; j < n; ++j)
      au[j] = u[j];
   CppAD::Independent(au);
   //
   // m, ay
   size_t m = 2;
   CPPAD_TESTVECTOR( AD<double> ) ay(m);
   CPPAD_TESTVECTOR( AD<double> ) ax = au;
   afun(ax, ay);
   //
   // f
   CppAD::ADFun<double> f;
   f.Dependent(au, ay);
   //
   // check function value
   double check = u[2] * u[2];
   ok &= NearEqual( Value(ay[0]) , check,  eps, eps);
   check = u[0] * u[1];
   ok &= NearEqual( Value(ay[1]) , check,  eps, eps);

   // ----------------------------------------------------------------
   // zero order forward
   //
   // u0, y0
   CPPAD_TESTVECTOR(double) u0(n), y0(m);
   u0 = u;
   y0   = f.Forward(0, u0);
   check = u[2] * u[2];
   ok &= NearEqual(y0[0] , check,  eps, eps);
   check = u[0] * u[1];
   ok &= NearEqual(y0[1] , check,  eps, eps);
   // ----------------------------------------------------------------
   // first order forward
   //
   // check_jac
   double check_jac[] = {
      0.0, 0.0, 2.0 * u[2],
      u[1], u[0],       0.0
   };
   //
   // u1
   CPPAD_TESTVECTOR(double) u1(n);
   for(size_t j = 0; j < n; j++)
      u1[j] = 0.0;
   //
   // y1, j
   CPPAD_TESTVECTOR(double) y1(m);
   for(size_t j = 0; j < n; j++)
   {  //
      // u1, y1
      // compute partial in j-th component direction
      u1[j] = 1.0;
      y1    = f.Forward(1, u1);
      u1[j] = 0.0;
      //
      // check this partial
      for(size_t i = 0; i < m; i++)
         ok &= NearEqual(y1[i], check_jac[i * n + j], eps, eps);
   }
   // ----------------------------------------------------------------
   // second order forward
   //
   // check_hes_0
   double check_hes_0[] = {
      0.0, 0.0, 0.0,
      0.0, 0.0, 0.0,
      0.0, 0.0, 2.0
   };
   //
   // check_hes_1
   double check_hes_1[] = {
      0.0, 1.0, 0.0,
      1.0, 0.0, 0.0,
      0.0, 0.0, 0.0
   };
   //
   // u2
   CPPAD_TESTVECTOR(double) u2(n);
   for(size_t j = 0; j < n; j++)
      u2[j] = 0.0;
   //
   // y2, j
   CPPAD_TESTVECTOR(double) y2(m);
   for(size_t j = 0; j < n; j++)
   {  //
      // u1, y2
      // first order forward in j-th direction
      u1[j] = 1.0;
      f.Forward(1, u1);
      y2 = f.Forward(2, u2);
      //
      // check y2 element of Hessian diagonal
      ok &= NearEqual(y2[0], check_hes_0[j * n + j] / 2.0, eps, eps);
      ok &= NearEqual(y2[1], check_hes_1[j * n + j] / 2.0, eps, eps);
      //
      // k
      for(size_t k = 0; k < n; k++) if( k != j )
      {  //
         // u1, y2
         u1[k] = 1.0;
         f.Forward(1, u1);
         y2 = f.Forward(2, u2);
         //
         // y2 = (H_jj + H_kk + H_jk + H_kj) / 2.0
         // y2 = (H_jj + H_kk) / 2.0 + H_jk
         //
         // check y2[0]
         double H_jj = check_hes_0[j * n + j];
         double H_kk = check_hes_0[k * n + k];
         double H_jk = y2[0] - (H_kk + H_jj) / 2.0;
         ok &= NearEqual(H_jk, check_hes_0[j * n + k], eps, eps);
         //
         // check y2[1]
         H_jj = check_hes_1[j * n + j];
         H_kk = check_hes_1[k * n + k];
         H_jk = y2[1] - (H_kk + H_jj) / 2.0;
         ok &= NearEqual(H_jk, check_hes_1[j * n + k], eps, eps);
         //
         // u1
         u1[k] = 0.0;
      }
      // u1
      u1[j] = 0.0;
   }
   // ----------------------------------------------------------------
   return ok;
}
// END_USE_ATOMIC_FUNCTION
