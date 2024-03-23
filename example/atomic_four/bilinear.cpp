// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin atomic_four_bilinear.cpp}

Bilinear Interpolation Atomic Function: Example and Test
########################################################

See Also
********
:ref:`interp_onetape.cpp-name` .

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

{xrst_end atomic_four_bilinear.cpp}
*/
# include <cppad/cppad.hpp>  // CppAD include file

// BEGIN_DEFINE_ATOMIC_FUNCTION
// empty namespace
namespace {
   // atomic_bilinear
   class atomic_bilinear : public CppAD::atomic_four<double> {
   private:
      // u_grid_, v_grid_; y_grid_
      CppAD::vector<double>& u_grid_;
      CppAD::vector<double>& v_grid_;
      CppAD::vector<double>& y_grid_;
      //
      // u_index_, v_index
      size_t u_index_;
      size_t v_index_;
      //
      // set_index
      void set_index(double u, double v)
      {  //
         // u_index_
         while( u < u_grid_[u_index_] && u_index_ > 0 )
            --u_index_;
         while( u > u_grid_[u_index_+1] && u_index_ < u_grid_.size() - 2 )
            ++u_index_;
         //
         // v_index_
         while( v < v_grid_[v_index_] && v_index_ > 0 )
            --v_index_;
         while( v > v_grid_[v_index_+1] && v_index_ < v_grid_.size() - 2 )
            ++v_index_;
      }
   public:
      // can use const char* name when calling this constructor
      atomic_bilinear(
         const std::string&     name   ,
         CppAD::vector<double>& u_grid ,
         CppAD::vector<double>& v_grid ,
         CppAD::vector<double>& y_grid )  :
      CppAD::atomic_four<double>(name) , // inform base class of name
      u_grid_(u_grid)                  ,
      v_grid_(v_grid)                  ,
      y_grid_(y_grid)                  ,
      u_index_(0)                      ,
      v_index_(0)
      {  assert( u_grid_.size() >= 2 );
         assert( v_grid_.size() >= 2 );
         assert( y_grid_.size() == u_grid_.size() * v_grid_.size() );
      }
   private:
      // for_type
      bool for_type(
         size_t                                     call_id     ,
         const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
         CppAD::vector<CppAD::ad_type_enum>&        type_y      ) override
      {
         assert( call_id == 0 );       // default value
         assert( type_x.size() == 2 ); // n
         assert( type_y.size() == 1 ); // m
         //
         type_y[0] = std::max(type_x[0], type_x[1]);
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
         // ok
         bool ok = order_up <= 1;
         if( ! ok )
            return ok;
         //
         // q
         size_t q = order_up + 1;
         //
# ifndef NDEBUG
         size_t n = taylor_x.size() / q;
         size_t m = taylor_y.size() / q;
         assert( call_id == 0 );
         assert( n == 2 );
         assert( m == 1 );
         assert( m == select_y.size() );
# endif
         // u, v
         double u = taylor_x[0 * q + 0];
         double v = taylor_x[1 * q + 0];
         //
         // u_index_, v_index_
         set_index(u, v);
         //
         // u_0, u_1, v_0, v_1
         double u_0 = u_grid_[ u_index_ + 0 ];
         double u_1 = u_grid_[ u_index_ + 1 ];
         double v_0 = v_grid_[ v_index_ + 0 ];
         double v_1 = v_grid_[ v_index_ + 1 ];
         //
         // y_00, y_01, y_10, y_11
         double y_00 = y_grid_[ (u_index_+0) * v_grid_.size() + v_index_+0 ];
         double y_01 = y_grid_[ (u_index_+0) * v_grid_.size() + v_index_+1 ];
         double y_10 = y_grid_[ (u_index_+1) * v_grid_.size() + v_index_+0 ];
         double y_11 = y_grid_[ (u_index_+1) * v_grid_.size() + v_index_+1 ];
         //
         // taylor_y
         // function value
         if( order_low <= 0 )
         {  double sum  = 0.0;
            sum        += y_00 * (u_1 - u)   * (v_1 - v);
            sum        += y_01 * (u_1 - u)   * (v   - v_0);
            sum        += y_10 * (u   - u_0) * (v_1 - v);
            sum        += y_11 * (u   - u_0) * (v   - v_0);
            taylor_y[0] = sum / ( (u_1 - u_0) * (v_1 - v_0) );
         }
         //
         // taylor_y
         // first order derivatives
         if( order_low <= 1 && 1 <= order_up )
         {  //
            // du, dv
            double du   = taylor_x[0 * q + 1];
            double dv   = taylor_x[1 * q + 1];
            double dsum = 0.0;
            //
            dsum        -= y_00 * du * (v_1 - v);
            dsum        -= y_01 * du * (v   - v_0);
            dsum        += y_10 * du * (v_1 - v);
            dsum        += y_11 * du * (v   - v_0);

            dsum        -= y_00 * (u_1 - u)   * dv;
            dsum        += y_01 * (u_1 - u)   * dv;
            dsum        -= y_10 * (u   - u_0) * dv;
            dsum        += y_11 * (u   - u_0) * dv;
            taylor_y[1] = dsum / ( (u_1 - u_0) * (v_1 - v_0) );
         }
         //
         return ok;
      }
   };
}
// END_DEFINE_ATOMIC_FUNCTION

// BEGIN_USE_ATOMIC_FUNCTION
bool bilinear(void)
{  //
   // ok, eps
   bool ok = true;
   double eps = 10. * CppAD::numeric_limits<double>::epsilon();
   //
   // nu, u_grid
   size_t nu = 4;
   CppAD::vector<double> u_grid(nu);
   for(size_t i = 0; i < nu; ++i)
      u_grid[i] = double(i) * 2.0;
   //
   // nv, v_grid
   size_t nv = 5;
   CppAD::vector<double> v_grid(nv);
   for(size_t j = 0; j < nv; ++j)
      v_grid[j] = double(j) * 3.0;
   //
   // y_grid
   CppAD::vector<double> y_grid( u_grid.size() * v_grid.size() );
   for(size_t i = 0; i < nu; ++i)
   {  for(size_t j = 0; j < nv; ++j)
      {  double u = u_grid[i];
         double v = v_grid[j];
         y_grid[i * v_grid.size() + j] = u * u + v * v;
      }
   }
   //
   // afun
   std::string name = "atomic_bilinear";
   atomic_bilinear afun(name, u_grid, v_grid, y_grid);
   //
   // n, m
   size_t n = 2;
   size_t m = 1;
   //
   // ax
   CPPAD_TESTVECTOR( CppAD::AD<double> ) ax(n);
   ax[0]     = 0.0;
   ax[1]     = 0.0;
   CppAD::Independent(ax);
   //
   // ay
   // call atomic function and store result in ay
   CPPAD_TESTVECTOR( CppAD::AD<double> ) ay(m);
   afun(ax, ay);
   //
   // f
   // create f: x -> y and stop tape recording
   CppAD::ADFun<double> f(ax, ay);
   //
   // y
   CPPAD_TESTVECTOR( double ) x(n), y(m);
   x[0] = 1.0;
   x[1] = 3.5;
   y    = f.Forward(0, x);
   //
   // u_0, u_1
   double u_0 = u_grid[0];
   double u_1 = u_grid[1];
   assert( u_0 < x[0] && x[0] < u_1 );
   //
   // v_0, v_1
   double v_0 = v_grid[1];
   double v_1 = v_grid[2];
   assert( v_0 < x[1] && x[1] < v_1 );
   //
   // y_00, y_01, y_10, y_11
   double y_00 = y_grid[ 0 * v_grid.size() + 1 ];
   double y_01 = y_grid[ 0 * v_grid.size() + 2 ];
   double y_10 = y_grid[ 1 * v_grid.size() + 1 ];
   double y_11 = y_grid[ 1 * v_grid.size() + 2 ];
   //
   // check, ok
   double sum   = 0.0;
   sum         += y_00 * (u_1  - x[0]) * (v_1  - x[1]);
   sum         += y_01 * (u_1  - x[0]) * (x[1] - v_0);
   sum         += y_10 * (x[0] - u_0)  * (v_1  - x[1]);
   sum         += y_11 * (x[0] - u_0)  * (x[1] - v_0);
   double check = sum / ( (u_1 - u_0)  * (v_1 - v_0) );
   ok          &= CppAD::NearEqual(y[0] , check,  eps, eps);
   //
   // dy
   CPPAD_TESTVECTOR( double ) dx(n), dy(m);
   dx[0] = 1.0;
   dx[1] = 0.0;
   dy   = f.Forward(1, dx);
   //
   // check
   sum    = 0.0;
   sum   -= y_00 * (v_1  - x[1]);
   sum   -= y_01 * (x[1] - v_0);
   sum   += y_10 * (v_1  - x[1]);
   sum   += y_11 * (x[1] - v_0);
   check  = sum / ( (u_1 - u_0)  * (v_1 - v_0) );
   ok    &= CppAD::NearEqual(dy[0] , check,  eps, eps);
   //
   // dy
   dx[0] = 0.0;
   dx[1] = 1.0;
   dy   = f.Forward(1, dx);
   //
   // check
   sum    = 0.0;
   sum   -= y_00 * (u_1  - x[0]);
   sum   += y_01 * (u_1  - x[0]);
   sum   -= y_10 * (x[0] - u_0);
   sum   += y_11 * (x[0] - u_0);
   check  = sum / ( (u_1 - u_0)  * (v_1 - v_0) );
   ok    &= CppAD::NearEqual(dy[0] , check,  eps, eps);
   //
   return ok;
}
// END_USE_ATOMIC_FUNCTION
