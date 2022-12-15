// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin atomic_three_reciprocal.cpp}

Reciprocal as an Atomic Operation: Example and Test
###################################################

Function
********
This example demonstrates using :ref:`atomic_three-name`
to define the operation
:math:`g : \B{R}^n \rightarrow \B{R}^m` where
:math:`n = 1`, :math:`m = 1`, and :math:`g(x) = 1 / x`.

Start Class Definition
**********************
{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/cppad.hpp>
namespace {           // isolate items below to this file
using CppAD::vector;  // abbreivate CppAD::vector as vector
//
class atomic_reciprocal : public CppAD::atomic_three<double> {
/* {xrst_code}
{xrst_spell_on}
Constructor
***********
{xrst_spell_off}
{xrst_code cpp} */
public:
   atomic_reciprocal(const std::string& name) :
   CppAD::atomic_three<double>(name)
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
      bool ok = type_x.size() == 1; // n
      ok     &= type_y.size() == 1; // m
      if( ! ok )
         return false;
      type_y[0] = type_x[0];
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
      const vector<double>&              parameter_x ,
      const vector<CppAD::ad_type_enum>& type_x      ,
      size_t                             need_y      ,
      size_t                             p           ,
      size_t                             q           ,
      const vector<double>&              tx          ,
      vector<double>&                    ty          ) override
   {
# ifndef NDEBUG
      size_t n = tx.size() / (q + 1);
      size_t m = ty.size() / (q + 1);
# endif
      assert( type_x.size() == n );
      assert( n == 1 );
      assert( m == 1 );
      assert( p <= q );

      // return flag
      bool ok = q <= 2;

      // Order zero forward mode.
      // This case must always be implemented
      // y^0 = g( x^0 ) = 1 / x^0
      double g = 1. / tx[0];
      if( p <= 0 )
         ty[0] = g;
      if( q <= 0 )
         return ok;

      // Order one forward mode.
      // This case needed if first order forward mode is used.
      // y^1 = g'( x^0 ) x^1
      double gp = - g / tx[0];
      if( p <= 1 )
         ty[1] = gp * tx[1];
      if( q <= 1 )
         return ok;

      // Order two forward mode.
      // This case needed if second order forward mode is used.
      // Y''(t) = X'(t)^\R{T} g''[X(t)] X'(t) + g'[X(t)] X''(t)
      // 2 y^2  = x^1 * g''( x^0 ) x^1 + 2 g'( x^0 ) x^2
      double gpp  = - 2.0 * gp / tx[0];
      ty[2] = tx[1] * gpp * tx[1] / 2.0 + gp * tx[2];
      if( q <= 2 )
         return ok;

      // Assume we are not using forward mode with order > 2
      assert( ! ok );
      return ok;
   }
/* {xrst_code}
{xrst_spell_on}
reverse
*******
{xrst_spell_off}
{xrst_code cpp} */
   // reverse mode routine called by CppAD
   bool reverse(
      const vector<double>&               parameter_x ,
      const vector<CppAD::ad_type_enum>&  type_x      ,
      size_t                              q           ,
      const vector<double>&               tx           ,
      const vector<double>&               ty           ,
      vector<double>&                     px           ,
      const vector<double>&               py           ) override
   {
# ifndef NDEBUG
      size_t n = tx.size() / (q + 1);
      size_t m = ty.size() / (q + 1);
# endif
      assert( px.size() == tx.size() );
      assert( py.size() == ty.size() );
      assert( n == 1 );
      assert( m == 1 );
      bool ok = q <= 2;

      double g, gp, gpp, gppp;
      switch(q)
      {  case 0:
         // This case needed if first order reverse mode is used
         // reverse: F^0 ( tx ) = y^0 = g( x^0 )
         g     = ty[0];
         gp    = - g / tx[0];
         px[0] = py[0] * gp;;
         assert(ok);
         break;

         case 1:
         // This case needed if second order reverse mode is used
         // reverse: F^1 ( tx ) = y^1 = g'( x^0 ) x^1
         g      = ty[0];
         gp     = - g / tx[0];
         gpp    = - 2.0 * gp / tx[0];
         px[1]  = py[1] * gp;
         px[0]  = py[1] * gpp * tx[1];
         // reverse: F^0 ( tx ) = y^0 = g( x^0 );
         px[0] += py[0] * gp;
         assert(ok);
         break;

         case 2:
         // This needed if third order reverse mode is used
         // reverse: F^2 ( tx ) = y^2 =
         //          = x^1 * g''( x^0 ) x^1 / 2 + g'( x^0 ) x^2
         g      = ty[0];
         gp     = - g / tx[0];
         gpp    = - 2.0 * gp / tx[0];
         gppp   = - 3.0 * gpp / tx[0];
         px[2]  = py[2] * gp;
         px[1]  = py[2] * gpp * tx[1];
         px[0]  = py[2] * tx[1] * gppp * tx[1] / 2.0 + gpp * tx[2];
         // reverse: F^1 ( tx ) = y^1 = g'( x^0 ) x^1
         px[1] += py[1] * gp;
         px[0] += py[1] * gpp * tx[1];
         // reverse: F^0 ( tx ) = y^0 = g( x^0 );
         px[0] += py[0] * gp;
         assert(ok);
         break;

         default:
         assert(!ok);
      }
      return ok;
   }
/* {xrst_code}
{xrst_spell_on}
jac_sparsity
************
{xrst_spell_off}
{xrst_code cpp} */
   // Jacobian sparsity routine called by CppAD
   bool jac_sparsity(
      const vector<double>&               parameter_x ,
      const vector<CppAD::ad_type_enum>&  type_x      ,
      bool                                dependency  ,
      const vector<bool>&                 select_x    ,
      const vector<bool>&                 select_y    ,
      CppAD::sparse_rc< vector<size_t> >& pattern_out ) override
   {
      size_t n = select_x.size();
      size_t m = select_y.size();
      assert( parameter_x.size() == n );
      assert( n == 1 );
      assert( m == 1 );

      // size of pattern_out
      size_t nr  = m;
      size_t nc  = n;
      size_t nnz = 0;
      if( select_x[0] & select_y[0] )
         ++nnz;
      pattern_out.resize(nr, nc, nnz);

      // set values in pattern_out
      size_t k = 0;
      if( select_x[0] & select_y[0] )
         pattern_out.set(k++, 0, 0);
      assert( k == nnz );

      return true;
   }
/* {xrst_code}
{xrst_spell_on}
hes_sparsity
************
{xrst_spell_off}
{xrst_code cpp} */
   // Hessian sparsity routine called by CppAD
   bool hes_sparsity(
      const vector<double>&               parameter_x ,
      const vector<CppAD::ad_type_enum>&  type_x      ,
      const vector<bool>&                 select_x    ,
      const vector<bool>&                 select_y    ,
      CppAD::sparse_rc< vector<size_t> >& pattern_out ) override
   {
      assert( parameter_x.size() == select_x.size() );
      assert( select_y.size() == 1 );
      size_t n = select_x.size();
      assert( n == 1 );

      // size of pattern_out
      size_t nr  = n;
      size_t nc  = n;
      size_t nnz = 0;
      if( select_x[0] & select_y[0] )
         ++nnz;
      pattern_out.resize(nr, nc, nnz);

      // set values in pattern_out
      size_t k = 0;
      if( select_x[0] & select_y[0] )
         pattern_out.set(k++, 0, 0);
      assert( k == nnz );

      return true;
   }
/* {xrst_code}
{xrst_spell_on}
End Class Definition
********************
{xrst_spell_off}
{xrst_code cpp} */
}; // End of atomic_reciprocal class
}  // End empty namespace

/* {xrst_code}
{xrst_spell_on}
Use Atomic Function
*******************
{xrst_spell_off}
{xrst_code cpp} */
bool reciprocal(void)
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
   // --------------------------------------------------------------------
   // Create the atomic reciprocal object
   atomic_reciprocal afun("atomic_reciprocal");
/* {xrst_code}
{xrst_spell_on}
Recording
=========
{xrst_spell_off}
{xrst_code cpp} */
   // Create the function f(x) = 1 / g(x) = x
   //
   // domain space vector
   size_t n  = 1;
   double x0 = 0.5;
   vector< AD<double> > ax(n);
   ax[0]     = x0;

   // declare independent variables and start tape recording
   CppAD::Independent(ax);

   // range space vector
   size_t m = 1;
   vector< AD<double> > av(m);

   // call atomic function and store g(x) in au[0]
   vector< AD<double> > au(m);
   afun(ax, au);        // u = 1 / x

   // now use AD division to invert to invert the operation
   av[0] = 1.0 / au[0]; // v = 1 / u = x

   // create f: x -> y and stop tape recording
   CppAD::ADFun<double> f;
   f.Dependent (ax, av);  // g(x) = x
/* {xrst_code}
{xrst_spell_on}
forward
=======
{xrst_spell_off}
{xrst_code cpp} */
   // check function value
   double check = x0;
   ok &= NearEqual( Value(av[0]) , check,  eps, eps);

   // check zero order forward mode
   size_t q;
   vector<double> x_q(n), v_q(m);
   q      = 0;
   x_q[0] = x0;
   v_q    = f.Forward(q, x_q);
   ok &= NearEqual(v_q[0] , check,  eps, eps);

   // check first order forward mode
   q      = 1;
   x_q[0] = 1;
   v_q    = f.Forward(q, x_q);
   check  = 1.0;
   ok &= NearEqual(v_q[0] , check,  eps, eps);

   // check second order forward mode
   q      = 2;
   x_q[0] = 0;
   v_q    = f.Forward(q, x_q);
   check  = 0.;
   ok &= NearEqual(v_q[0] , check,  eps, eps);
/* {xrst_code}
{xrst_spell_on}
reverse
=======
{xrst_spell_off}
{xrst_code cpp} */
   // third order reverse mode
   q     = 3;
   vector<double> w(m), dw(n * q);
   w[0]  = 1.;
   dw    = f.Reverse(q, w);
   check = 1.;
   ok &= NearEqual(dw[0] , check,  eps, eps);
   check = 0.;
   ok &= NearEqual(dw[1] , check,  eps, eps);
   ok &= NearEqual(dw[2] , check,  eps, eps);
/* {xrst_code}
{xrst_spell_on}
for_jac_sparsity
================
{xrst_spell_off}
{xrst_code cpp} */
   // forward mode Jacobian sparstiy pattern
   CppAD::sparse_rc< CPPAD_TESTVECTOR(size_t) > pattern_in, pattern_out;
   pattern_in.resize(1, 1, 1);
   pattern_in.set(0, 0, 0);
   bool transpose     = false;
   bool dependency    = false;
   bool internal_bool = false;
   f.for_jac_sparsity(
      pattern_in, transpose, dependency, internal_bool, pattern_out
   );
   ok &= pattern_out.nnz() == 1;
   ok &= pattern_out.row()[0] == 0;
   ok &= pattern_out.col()[0] == 0;
/* {xrst_code}
{xrst_spell_on}
rev_sparse_jac
==============
{xrst_spell_off}
{xrst_code cpp} */
   // reverse mode Jacobian sparstiy pattern
   f.rev_jac_sparsity(
      pattern_in, transpose, dependency, internal_bool, pattern_out
   );
   ok &= pattern_out.nnz() == 1;
   ok &= pattern_out.row()[0] == 0;
   ok &= pattern_out.col()[0] == 0;
/* {xrst_code}
{xrst_spell_on}
rev_sparse_hes
==============
{xrst_spell_off}
{xrst_code cpp} */
   // Hessian sparsity (using previous for_jac_sparsity call)
   CPPAD_TESTVECTOR(bool) select_y(m);
   select_y[0] = true;
   f.rev_hes_sparsity(
      select_y, transpose, internal_bool, pattern_out
   );
   ok &= pattern_out.nnz() == 1;
   ok &= pattern_out.row()[0] == 0;
   ok &= pattern_out.col()[0] == 0;
/* {xrst_code}
{xrst_spell_on}
for_sparse_hes
==============
{xrst_spell_off}
{xrst_code cpp} */
   // Hessian sparsity
   CPPAD_TESTVECTOR(bool) select_x(n);
   select_x[0] = true;
   f.for_hes_sparsity(
      select_x, select_y, internal_bool, pattern_out
   );
   ok &= pattern_out.nnz() == 1;
   ok &= pattern_out.row()[0] == 0;
   ok &= pattern_out.col()[0] == 0;
   return ok;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end atomic_three_reciprocal.cpp}
*/
