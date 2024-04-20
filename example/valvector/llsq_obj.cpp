// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ---------------------------------------------------------------------------
/*
{xrst_begin valvector_llsq_obj.cpp}

Using valvector to Represent Linear Least Squares Objective
###########################################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector_llsq_obj.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/example/valvector/sum.hpp>
# include <cppad/example/valvector/class.hpp>
# include <cppad/cppad.hpp>
bool llsq_obj(void)
{  // ok
   bool ok = true;
   //
   // scalar_type
   typedef valvector::scalar_type scalar_type;
   //
   // eps99
   scalar_type eps99 = CppAD::numeric_limits<scalar_type>::epsilon();
   eps99            *= scalar_type(99);
   //
   // ad_valvector
   typedef CppAD::AD<valvector> ad_valvector;
   //
   // asum
   valvector_ad_sum asum;
   //
   // n_data
   // number of data points in the fit
   size_t n_data = 100;
   //
   // time, data
   // argument and data values in the function being fit
   valvector time, data;
   if( n_data == 1 )
   {  time[0] = 0.0;
      data[0] = 0.0;
   }
   else
   {  time.resize(n_data);
      data.resize(n_data);
      //
      for(size_t i = 0; i < n_data; ++i)
      {  time[i] = -1.0 + scalar_type(2 * i) / scalar_type(n_data - 1);
         if( time[i] < 0.0 )
            data[i] = -1.0;
         else if( 0.0 < time[i] )
            data[i] = +1.0;
         else
            data[i] = 0.0;
      }
   }
   // data
   if( n_data % 2 == 1 )
   {  // in case time[n_data/2] was not exactly zero due to roundoff
      data[n_data / 2] = 0.0;
   }
   //
   // nx
   // number of coefficients in the function being fit
   size_t nx = 3;
   //
   // ax
   // coefficients in the function being fit
   CPPAD_TESTVECTOR( ad_valvector ) ax(nx);
   for(size_t j = 0; j < nx; ++j)
      ax[j] = valvector(0.0);
   CppAD::Independent(ax);
   //
   // amodel
   valvector time_j(1.0);
   ad_valvector amodel(0.0);
   for(size_t  j = 0; j < nx; ++j)
   {  amodel += time_j * ax[j];
      time_j *= time;
   }
   //
   // aobj
   ad_valvector ares = data - amodel;
   ad_valvector asq  = ares * ares;
   ad_valvector aobj;
   asum(asq, aobj);
   //
   // ay
   CPPAD_TESTVECTOR( ad_valvector ) ay(1);
   ay[0] = aobj;
   CppAD::ADFun<valvector> f(ax, ay);
   //
   // x
   CPPAD_TESTVECTOR( valvector ) x(nx);
   for(size_t j = 0; j < nx; ++j)
      x[j][0] = 1.0;
   //
   // y
   CPPAD_TESTVECTOR( valvector ) y(1);
   y = f.Forward(0, x);
   //
   // res
   CPPAD_TESTVECTOR( scalar_type ) res(n_data);
   for(size_t i = 0; i < n_data; ++i)
   {  scalar_type tj = 1.0;
      scalar_type model = 0.0;
      for(size_t j = 0; j < nx; ++j)
      {  model += tj * x[j][0];
         tj    *= time[i];
      }
      res[i] = data[i] - model;
   }
   //
   // ok
   scalar_type check = 0.0;
   for(size_t i = 0; i < n_data; ++i)
      check += res[i] * res[i];
   ok   &= CppAD::NearEqual(y[0][0], check, eps99, eps99);
   //
   // dw
   CPPAD_TESTVECTOR( valvector ) w(1), dw(nx);
   w[0][0] = 1.0;
   dw = f.Reverse(1, w);
   //
   //
   // dres_sq
   CPPAD_TESTVECTOR( scalar_type ) dres_sq(nx);
   for(size_t j = 0; j < nx; ++j)
      dres_sq[j] = 0.0;
   for(size_t i = 0; i < n_data; ++i)
   {  scalar_type tj = 1.0;
      for(size_t j = 0; j < nx; ++j)
      {  dres_sq[j] += - 2.0 * res[i] * tj;
         tj         *= time[i];
      }
   }
   //
   // ok
   // reverse mode does not know that x[j] had size zero and represents
   // a valvector of size n_data and with x[j][k] constant w.r.t k.
   for(size_t j = 0; j < nx; ++j)
   {  /*
      std::cout << "j = " << j;
      std::cout << ", dw[j] = " << dw[j];
      std::cout << ", dres_sq[j] = " << dres_sq[j] << "\n";
      */
      ok   &= dw[j].size() == n_data;
      ok   &= CppAD::NearEqual(dw[j].sum(), dres_sq[j], eps99, eps99);
   }
   return ok;
}
// END C++
