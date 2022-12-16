// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_vector_mul.cpp}

Atomic Vector Multiplication Example
####################################

f(u, v, w)
**********
For this example,
:math:`f : \B{R}^{3m} \rightarrow \B{R}^m`
is defined by :math:`f(u, v, w) = u * v * w`.
where *u* , *v* , and *w* are in :math:`\B{R}^m`.

g(u, v, w)
**********
For this example :math:`g : \B{R}^{3m} \rightarrow \B{R}^m`
is defined by :math:`g_i (u, v, w) = \partial_{u[i]}  f_i (u, v, w)`

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_vector_mul.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cppad/example/atomic_four/vector/vector.hpp>
bool mul(void)
{  bool ok = true;
   using CppAD::NearEqual;
   using CppAD::AD;
   double eps99 = 99.0 * CppAD::numeric_limits<double>::epsilon();
   //
   // vec_op
   // atomic vector_op object
   CppAD::atomic_vector<double> vec_op("atomic_vector");
   //
   // m
   // size of u, v, and w
   size_t m = 5;
   //
   // mul_op
   typedef CppAD::atomic_vector<double>::op_enum_t op_enum_t;
   op_enum_t mul_op = CppAD::atomic_vector<double>::mul_enum;
   // -----------------------------------------------------------------------
   // Record f(u, v, w) = u * v * w
   // -----------------------------------------------------------------------
   // Independent variable vector
   CPPAD_TESTVECTOR( CppAD::AD<double> ) auvw(3 * m);
   for(size_t j = 0; j < 3 * m; ++j)
      auvw[j] = AD<double>(1 + j);
   CppAD::Independent(auvw);
   //
   // au, av, aw
   CPPAD_TESTVECTOR( CppAD::AD<double> ) au(m), av(m), aw(m);
   for(size_t i = 0; i < m; ++i)
   {  au[i] = auvw[0 * m + i];
      av[i] = auvw[1 * m + i];
      aw[i] = auvw[2 * m + i];
   }
   //
   // ax = (mul_op, au, av)
   CPPAD_TESTVECTOR( CppAD::AD<double> ) ax(2 * m);
   for(size_t i = 0; i < m; ++i)
   {  ax[i]     = au[i];
      ax[m + i] = av[i];
   }
   //
   // ay = u * v
   CPPAD_TESTVECTOR( CppAD::AD<double> ) ay(m);
   vec_op(mul_op, ax, ay);
   //
   // ax = (ay, aw)
   for(size_t i = 0; i < m; ++i)
   {  ax[i]     = ay[i];
      ax[m + i] = aw[i];
   }
   //
   // az = ay * aw
   CPPAD_TESTVECTOR( CppAD::AD<double> ) az(m);
   vec_op(mul_op, ax, az);
   //
   // f
   CppAD::ADFun<double> f(auvw, az);
   // -----------------------------------------------------------------------
   // check forward mode on f
   // -----------------------------------------------------------------------
   //
   // uvw, duvw
   CPPAD_TESTVECTOR(double) uvw(3 * m), duvw(3 * m);
   for(size_t j = 0; j < 3 * m; ++j)
   {  uvw[j]  = double(1 + j);
      duvw[j] = 1.0;
   }
   //
   // z, dz
   CPPAD_TESTVECTOR(double) z(m), dz(m);
   z  = f.Forward(0, uvw);
   dz = f.Forward(1, duvw);
   //
   // ok
   for(size_t i = 0; i < m; ++i)
   {  double ui  = uvw[0 * m + i];
      double vi  = uvw[1 * m + i];
      double wi  = uvw[2 * m + i];
      //
      double check_z  = ui * vi * wi;
      ok             &= NearEqual( z[i] ,  check_z,  eps99, eps99);
      //
      double check_dz = (vi * wi) + (ui * wi) + (ui * vi);
      ok             &= NearEqual( dz[i] ,  check_dz,  eps99, eps99);
   }
   // -----------------------------------------------------------------------
   // check reverse mode on f
   // -----------------------------------------------------------------------
   //
   // weight
   CPPAD_TESTVECTOR(double) weight(m);
   for(size_t i = 0; i < m; ++i)
      weight[i] = 1.0;
   //
   // dweight
   CPPAD_TESTVECTOR(double) dweight(3 * m);
   f.Forward(0, uvw);
   dweight = f.Reverse(1, weight);
   //
   // ok
   for(size_t i = 0; i < m; ++i)
   {  double ui  = uvw[0 * m + i];
      double vi  = uvw[1 * m + i];
      double wi  = uvw[2 * m + i];
      //
      double dfi_dui = vi * wi;
      ok           &= NearEqual(dweight[0 * m + i], dfi_dui, eps99, eps99);
      double dfi_dvi = ui * wi;
      ok           &= NearEqual(dweight[1 * m + i], dfi_dvi, eps99, eps99);
      double dfi_dwi = ui * vi;
      ok           &= NearEqual(dweight[2 * m + i], dfi_dwi, eps99, eps99);
   }
   // -----------------------------------------------------------------------
   // Record g_i (u, v, w) = \partial d/dv[i] f_i (u , v , w)
   // -----------------------------------------------------------------------
   //
   // af
   CppAD::ADFun< AD<double>, double > af = f.base2ad();
   //
   // auvw
   CppAD::Independent(auvw);
   //
   // aduvw
   CPPAD_TESTVECTOR( AD<double> ) aduvw(3 * m);
   for(size_t i = 0; i < m; ++i)
   {  aduvw[0 * m + i]  = 1.0; // du[i]
      aduvw[1 * m + i]  = 0.0; // dv[i]
      aduvw[2 * m + i]  = 0.0; // dw[i]
   }
   //
   // az
   // use the fact that d_u[i] f_k (u, v, w) is zero when i != k
   af.Forward(0, auvw);
   az = af.Forward(1, aduvw);
   CppAD::ADFun<double> g(auvw, az);
   // -----------------------------------------------------------------------
   // Record h (u, v, w) = sum f_i^(1) (u , v , w)
   // -----------------------------------------------------------------------
   //
   // auvw
   CppAD::Independent(auvw);
   //
   // aweight
   CPPAD_TESTVECTOR( AD<double> ) aweight(m);
   for(size_t i = 0; i < m; ++i)
      aweight[i] = 1.0;
   //
   // az
   CPPAD_TESTVECTOR( AD<double> ) adweight(3 * m);
   af.Forward(0, auvw);
   az = af.Reverse(1, aweight);
   CppAD::ADFun<double> h(auvw, az);
   // -----------------------------------------------------------------------
   // check forward mode on g
   // -----------------------------------------------------------------------
   //
   // z
   z = g.Forward(0, uvw);
   //
   // ok
   for(size_t i = 0; i < m; ++i)
   {  double vi       = uvw[1 * m + i];
      double wi       = uvw[2 * m + i];
      double check_z  = vi * wi;
      ok             &= NearEqual( z[i] ,  check_z,  eps99, eps99);
   }
   return ok;
   // -----------------------------------------------------------------------
   // check forward mode on h
   // -----------------------------------------------------------------------
   //
   // z
   z = h.Forward(0, uvw);
   //
   // ok
   for(size_t i = 0; i < m; ++i)
   {  double ui  = uvw[0 * m + i];
      double vi  = uvw[1 * m + i];
      double wi  = uvw[2 * m + i];
      //
      double dfi_dui  = vi * wi;
      ok             &= NearEqual(z[0 * m + i] ,  dfi_dui,  eps99, eps99);
      //
      double dfi_dvi  = ui * wi;
      ok             &= NearEqual(z[1 * m + i] ,  dfi_dvi,  eps99, eps99);
      //
      double dfi_dwi  = ui * vi;
      ok             &= NearEqual(z[2 * m + i] ,  dfi_dwi,  eps99, eps99);
   }
   return ok;
}
// END C++
