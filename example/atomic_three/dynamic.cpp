// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin atomic_three_dynamic.cpp}

Atomic Functions with Dynamic Parameters: Example and Test
##########################################################

Purpose
*******
This example demonstrates using dynamic parameters with an
:ref:`atomic_three-name` function.

Function
********
For this example, the atomic function
:math:`g : \B{R}^3 \rightarrow \B{R}^3` is defined by
:math:`g_0 (x) = x_0 * x_ 0`,
:math:`g_1 (x) = x_0 * x_ 1`,
:math:`g_2 (x) = x_1 * x_ 2`.

Start Class Definition
**********************
{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/cppad.hpp>  // CppAD include file
namespace {                  // start empty namespace
using CppAD::vector;         // abbreviate CppAD::vector using vector
// start definition of atomic derived class using atomic_three interface
class atomic_dynamic : public CppAD::atomic_three<double> {
/* {xrst_code}
{xrst_spell_on}
Constructor
***********
{xrst_spell_off}
{xrst_code cpp} */
public:
   // can use const char* name when calling this constructor
   atomic_dynamic(const std::string& name) : // can have more arguments
   CppAD::atomic_three<double>(name)             // inform base class of name
   { }

private:
/* {xrst_code}
{xrst_spell_on}
for_type
********
{xrst_spell_off}
{xrst_code cpp} */
   // calculate type_y
   bool for_type(
      const vector<double>&               parameter_x ,
      const vector<CppAD::ad_type_enum>&  type_x      ,
      vector<CppAD::ad_type_enum>&        type_y      ) override
   {  assert( parameter_x.size() == type_x.size() );
      bool ok = type_x.size() == 3; // n
      ok     &= type_y.size() == 3; // m
      if( ! ok )
         return false;
      type_y[0] = type_x[0];
      type_y[1] = std::max( type_x[0], type_x[1] );
      type_y[2] = std::max( type_x[1], type_x[2] );
      return true;
   }
/* {xrst_code}
{xrst_spell_on}
forward
*******
{xrst_spell_off}
{xrst_code cpp} */
   // forward mode routine called by CppAD
   bool forward(
      const vector<double>&               parameter_x  ,
      const vector<CppAD::ad_type_enum>&  type_x       ,
      size_t                              need_y       ,
      size_t                              order_low    ,
      size_t                              order_up     ,
      const vector<double>&               taylor_x     ,
      vector<double>&                     taylor_y     ) override
   {
# ifndef NDEBUG
      size_t n = taylor_x.size() / (order_up + 1);
      size_t m = taylor_y.size() / (order_up + 1);
# endif
      assert( n == 3 );
      assert( m == 3 );
      assert( order_low <= order_up );

      // return flag
      bool ok = order_up == 0;
      if( ! ok )
         return ok;

      // Order zero forward mode.
      // This case must always be implemented
      if( need_y > size_t(CppAD::variable_enum) )
      {  // g_0 = x_0 * x_0
         taylor_y[0] = taylor_x[0] * taylor_x[0];
         // g_1 = x_0 * x_1
         taylor_y[1] = taylor_x[0] * taylor_x[1];
         // g_2 = x_1 * x_2
         taylor_y[2] = taylor_x[1] * taylor_x[2];
      }
      else
      {  // This uses need_y to reduce amount of computation.
         // It is probably faster, for this case, to ignore need_y.
         vector<CppAD::ad_type_enum> type_y( taylor_y.size() );
         for_type(taylor_x, type_x, type_y);
         // g_0 = x_0 * x_0
         if( size_t(type_y[0]) == need_y )
            taylor_y[0] = taylor_x[0] * taylor_x[0];
         // g_1 = x_0 * x_1
         if( size_t(type_y[1]) == need_y )
            taylor_y[1] = taylor_x[0] * taylor_x[1];
         // g_2 = x_1 * x_2
         if( size_t(type_y[2]) == need_y )
            taylor_y[2] = taylor_x[1] * taylor_x[2];
      }

      return ok;
   }
/* {xrst_code}
{xrst_spell_on}
End Class Definition
********************
{xrst_spell_off}
{xrst_code cpp} */
}; // End of atomic_dynamic class
}  // End empty namespace

/* {xrst_code}
{xrst_spell_on}
Use Atomic Function
*******************
{xrst_spell_off}
{xrst_code cpp} */
bool dynamic(void)
{  bool ok = true;
   using CppAD::AD;
   using CppAD::NearEqual;
   double eps = 10. * CppAD::numeric_limits<double>::epsilon();
/* {xrst_code}
{xrst_spell_on}
Constructor
===========
{xrst_spell_off}
{xrst_code cpp} */
   // Create the atomic dynamic object corresponding to g(x)
   atomic_dynamic afun("atomic_dynamic");
/* {xrst_code}
{xrst_spell_on}
Recording
=========
{xrst_spell_off}
{xrst_code cpp} */
   // Create the function f(u) = g(c, p, u) for this example.
   //
   // constant parameter
   double c_0 = 2.0;
   //
   // indepndent dynamic parameter vector
   size_t np = 1;
   CPPAD_TESTVECTOR(double) p(np);
   CPPAD_TESTVECTOR( AD<double> ) ap(np);
   ap[0] = p[0] = 3.0;
   //
   // independent variable vector
   size_t  nu  = 1;
   double  u_0 = 0.5;
   CPPAD_TESTVECTOR( AD<double> ) au(nu);
   au[0] = u_0;

   // declare independent variables and start tape recording
   CppAD::Independent(au, ap);

   // range space vector
   size_t ny = 3;
   CPPAD_TESTVECTOR( AD<double> ) ay(ny);

   // call atomic function and store result in ay
   // y = ( c * c, c * p, p * x )
   CPPAD_TESTVECTOR( AD<double> ) ax(3);
   ax[0] = c_0;   // x_0
   ax[1] = ap[0]; // x_1
   ax[2] = au[0]; // x_2
   afun(ax, ay);

   // check type of result
   ok &= Constant( ay[0] );
   ok &= Dynamic(  ay[1] );
   ok &= Variable( ay[2] );

   // create f: x -> y and stop tape recording
   CppAD::ADFun<double> f;
   f.Dependent (au, ay);  // f(u) = (c * c, c * p, p * u)
/* {xrst_code}
{xrst_spell_on}
forward
=======
{xrst_spell_off}
{xrst_code cpp} */
   // check function value
   double check = c_0 * c_0;
   ok &= NearEqual( Value(ay[0]) , check,  eps, eps);
   check = c_0 * p[0];
   ok &= NearEqual( Value(ay[1]) , check,  eps, eps);
   check = p[0] * u_0;
   ok &= NearEqual( Value(ay[2]) , check,  eps, eps);

   // check zero order forward mode
   size_t q;
   CPPAD_TESTVECTOR( double ) u_q(nu), y_q(ny);
   q      = 0;
   u_q[0] = u_0;
   y_q    = f.Forward(q, u_q);
   check = c_0 * c_0;
   ok    &= NearEqual(y_q[0] , check,  eps, eps);
   check = c_0 * p[0];
   ok    &= NearEqual(y_q[1] , check,  eps, eps);
   check = p[0] * u_0;
   ok    &= NearEqual(y_q[2] , check,  eps, eps);

   // set new value for dynamic parameters
   p[0]   = 2.0 * p[0];
   f.new_dynamic(p);
   y_q    = f.Forward(q, u_q);
   check = c_0 * c_0;
   ok    &= NearEqual(y_q[0] , check,  eps, eps);
   check = c_0 * p[0];
   ok    &= NearEqual(y_q[1] , check,  eps, eps);
   check = p[0] * u_0;
   ok    &= NearEqual(y_q[2] , check,  eps, eps);

/* {xrst_code}
{xrst_spell_on}
Return Test Result
==================
{xrst_spell_off}
{xrst_code cpp} */
   return ok;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end atomic_three_dynamic.cpp}
*/
