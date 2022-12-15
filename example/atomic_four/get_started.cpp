// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin atomic_four_get_started.cpp}

Getting Started with Atomic Functions: Example and Test
#######################################################

Purpose
*******
This example demonstrates the minimal amount of information
necessary for a :ref:`atomic_four-name` function.

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

{xrst_end atomic_four_get_started.cpp}
*/
# include <cppad/cppad.hpp>  // CppAD include file

// BEGIN_DEFINE_ATOMIC_FUNCTION
// empty namespace
namespace {
   // atomic_get_started
   class atomic_get_started : public CppAD::atomic_four<double> {
   public:
      // can use const char* name when calling this constructor
      atomic_get_started(const std::string& name) :
      CppAD::atomic_four<double>(name) // inform base class of name
      { }
   private:
      // for_type
      bool for_type(
         size_t                                     call_id     ,
         const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
         CppAD::vector<CppAD::ad_type_enum>&        type_y      ) override
      {
         assert( call_id == 0 );       // default value
         assert( type_x.size() == 1 ); // n
         assert( type_y.size() == 1 ); // m
         //
         type_y[0] = type_x[0];
         return true;
      }
      // forward
      bool forward(
         size_t                              call_id      ,
         const CppAD::vector<bool>&          select_y     ,
         size_t                              order_low    ,
         size_t                              order_up     ,
         const CppAD::vector<double>&        taylor_x     ,
         CppAD::vector<double>&              taylor_y     ) override
      {
# ifndef NDEBUG
         size_t q = order_up + 1;
         size_t n = taylor_x.size() / q;
         size_t m = taylor_y.size() / q;
         assert( call_id == 0 );
         assert( order_low == 0);
         assert( order_up == 0);
         assert( n == 1 );
         assert( m == 1 );
         assert( m == select_y.size() );
# endif
         // return flag
         bool ok = order_up == 0;
         if( ! ok )
            return ok;

         // taylor_y
         // Order zero forward mode: y^0 = g( x^0 ) = 1 / x^0
         taylor_y[0] = 1.0 / taylor_x[0];
         //
         return ok;
      }
   };
}
// END_DEFINE_ATOMIC_FUNCTION

// BEGIN_USE_ATOMIC_FUNCTION
bool get_started(void)
{
   // ok, eps
   bool ok = true;
   double eps = 10. * CppAD::numeric_limits<double>::epsilon();
   //
   // afun
   atomic_get_started afun("atomic_get_started");
   //
   // n, m
   size_t n = 1;
   size_t m = 1;
   //
   // x0
   double  x0 = 0.5;
   //
   // ax
   CPPAD_TESTVECTOR( CppAD::AD<double> ) ax(n);
   ax[0]     = x0;
   CppAD::Independent(ax);
   //
   // au
   // call atomic function and store result in au
   CPPAD_TESTVECTOR( CppAD::AD<double> ) au(m);
   afun(ax, au);
   //
   // ay
   CPPAD_TESTVECTOR( CppAD::AD<double> ) ay(m);
   ay[0] = 1.0 + au[0];
   //
   // f
   // create f: x -> y and stop tape recording
   CppAD::ADFun<double> f(ax, ay);
   //
   // check
   double check = 1.0 + 1.0 / x0;
   //
   // ok
   // check ay[0]
   ok &= CppAD::NearEqual( Value(ay[0]) , check,  eps, eps);
   //
   // ok
   // check zero order forward mode
   CPPAD_TESTVECTOR( double ) x(n), y(m);
   x[0] = x0;
   y    = f.Forward(0, x);
   ok    &= CppAD::NearEqual(y[0] , check,  eps, eps);
   //
   return ok;
}
// END_USE_ATOMIC_FUNCTION
