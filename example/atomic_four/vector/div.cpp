// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_vector_div.cpp}

Atomic Vector Division Example
##############################

f(u, v, w)
**********
For this example,
:math:`f : \B{R}^{2m} \rightarrow \B{R}^m`
is defined by :math:`f(u, v) = u * u / v`.
where *u* and *v* are in :math:`\B{R}^m`.

g(u, v)
*******
For this example :math:`g : \B{R}^{2m} \rightarrow \B{R}^m`
is defined by :math:`g_i (u, v) = \partial_{v[i]}  f_i (u, v)`

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_vector_div.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cppad/example/atomic_four/vector/vector.hpp>
bool div(void)
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
   // size of u and v
   size_t m = 4;
   //
   // mul_op, div_op
   typedef CppAD::atomic_vector<double>::op_enum_t op_enum_t;
   op_enum_t mul_op = CppAD::atomic_vector<double>::mul_enum;
   op_enum_t div_op = CppAD::atomic_vector<double>::div_enum;
   // -----------------------------------------------------------------------
   // Record f(u, v) = u * u / v
   // -----------------------------------------------------------------------
   // Independent variable vector
   CPPAD_TESTVECTOR( CppAD::AD<double> ) auv(2 * m);
   for(size_t j = 0; j < 2 * m; ++j)
      auv[j] = AD<double>(1 + j);
   CppAD::Independent(auv);
   //
   // au, av, aw
   CPPAD_TESTVECTOR( CppAD::AD<double> ) au(m), av(m);
   for(size_t i = 0; i < m; ++i)
   {  au[i] = auv[0 * m + i];
      av[i] = auv[1 * m + i];
   }
   //
   // ax = (mul_op, au, au)
   CPPAD_TESTVECTOR( CppAD::AD<double> ) ax(2 * m);
   for(size_t i = 0; i < m; ++i)
   {  ax[i]     = au[i];
      ax[m + i] = au[i];
   }
   //
   // ay = u * u
   CPPAD_TESTVECTOR( CppAD::AD<double> ) ay(m);
   vec_op(mul_op, ax, ay);
   //
   // ax = (ay, av)
   for(size_t i = 0; i < m; ++i)
   {  ax[i]     = ay[i];
      ax[m + i] = av[i];
   }
   //
   // az = au / ay
   CPPAD_TESTVECTOR( CppAD::AD<double> ) az(m);
   vec_op(div_op, ax, az);
   //
   // f
   CppAD::ADFun<double> f(auv, az);
   // -----------------------------------------------------------------------
   // check forward mode on f
   // -----------------------------------------------------------------------
   //
   // uv, duv
   CPPAD_TESTVECTOR(double) uv(2 * m), duv(2 * m);
   for(size_t j = 0; j < 2 * m; ++j)
   {  uv[j]  = double(2 + j);
      duv[j] = 1.0;
   }
   //
   // z, dz
   CPPAD_TESTVECTOR(double) z(m), dz(m);
   z  = f.Forward(0, uv);
   dz = f.Forward(1, duv);
   //
   // ok
   for(size_t i = 0; i < m; ++i)
   {  double ui     = uv[0 * m + i];
      double vi     = uv[1 * m + i];
      double check  = ui * ui / vi;
      ok           &= NearEqual( z[i] ,  check,  eps99, eps99);
      check         = 2.0 * ui / vi - ui * ui / (vi * vi);
      ok         &= NearEqual( dz[i] ,  check,  eps99, eps99);
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
   CPPAD_TESTVECTOR(double) dweight(2 * m);
   f.Forward(0, uv);
   dweight = f.Reverse(1, weight);
   //
   // ok
   for(size_t i = 0; i < m; ++i)
   {  double ui      = uv[0 * m + i];
      double vi      = uv[1 * m + i];
      double dfi_dui = 2.0 * ui / vi;
      ok           &= NearEqual(dweight[0 * m + i], dfi_dui, eps99, eps99);
      double dfi_dvi = - ui * ui / (vi * vi);
      ok           &= NearEqual(dweight[1 * m + i], dfi_dvi, eps99, eps99);
   }
   // -----------------------------------------------------------------------
   // Record g_i (u, v) = \partial d/dv[i] f_i (u, v)
   // -----------------------------------------------------------------------
   //
   // af
   CppAD::ADFun< AD<double>, double > af = f.base2ad();
   //
   // auv
   CppAD::Independent(auv);
   //
   // aduv
   CPPAD_TESTVECTOR( AD<double> ) aduv(2 * m);
   for(size_t i = 0; i < m; ++i)
   {  aduv[0 * m + i]  = 0.0; // du[i]
      aduv[1 * m + i]  = 1.0; // dv[i]
   }
   //
   // az
   // use the fact that d_u[i] f_k (u, v, w) is zero when i != k
   af.Forward(0, auv);
   az = af.Forward(1, aduv);
   CppAD::ADFun<double> g(auv, az);
   // -----------------------------------------------------------------------
   // Record h (u, v) = sum f_i^(1) (u , v)
   // -----------------------------------------------------------------------
   //
   // auv
   CppAD::Independent(auv);
   //
   // aweight
   CPPAD_TESTVECTOR( AD<double> ) aweight(m);
   for(size_t i = 0; i < m; ++i)
      aweight[i] = 1.0;
   //
   // az
   CPPAD_TESTVECTOR( AD<double> ) adweight(3 * m);
   af.Forward(0, auv);
   az = af.Reverse(1, aweight);
   CppAD::ADFun<double> h(auv, az);
   // -----------------------------------------------------------------------
   // check forward mode on g
   // -----------------------------------------------------------------------
   //
   // z
   z = g.Forward(0, uv);
   //
   // ok
   for(size_t i = 0; i < m; ++i)
   {  double ui      = uv[0 * m + i];
      double vi      = uv[1 * m + i];
      double check   = - ui * ui / (vi * vi);
      ok           &= NearEqual( z[i] ,  check,  eps99, eps99);
   }
   // -----------------------------------------------------------------------
   // check forward mode on h
   // -----------------------------------------------------------------------
   //
   // z
   z = h.Forward(0, uv);
   //
   // ok
   for(size_t i = 0; i < m; ++i)
   {  double ui  = uv[0 * m + i];
      double vi  = uv[1 * m + i];
      //
      double dfi_dui  = 2.0 * ui / vi;
      ok             &= NearEqual(z[0 * m + i] ,  dfi_dui,  eps99, eps99);
      //
      double dfi_dvi  = - ui * ui / (vi * vi);
      ok             &= NearEqual(z[1 * m + i] ,  dfi_dvi,  eps99, eps99);
   }
   return ok;
}
// END C++
