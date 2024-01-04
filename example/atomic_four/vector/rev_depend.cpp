// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_vector_rev_depend.cpp}

Example Optimizing Atomic Vector Usage
######################################

f(u, v)
*******
For this example,
:math:`f : \B{R}^{3m} \rightarrow \B{R}`
is defined by :math:`f(u, v, w) = - ( u_0 + v_0 ) * w_0`.
where *u* , *v* , and *w* are in :math:`\B{R}^m`.

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_vector_rev_depend.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cppad/example/atomic_four/vector/vector.hpp>
bool rev_depend(void)
{  bool ok = true;
   using CppAD::NearEqual;
   using CppAD::AD;
   //
   // vec_op
   // atomic vector_op object
   CppAD::atomic_vector<double> vec_op("atomic_vector");
   //
   // m
   // size of u, v, and w
   size_t m = 6;
   //
   // n
   size_t n = 3 * m;
   //
   // add_op, mul_op, neg_op
   typedef CppAD::atomic_vector<double>::op_enum_t op_enum_t;
   op_enum_t add_op = CppAD::atomic_vector<double>::add_enum;
   op_enum_t mul_op = CppAD::atomic_vector<double>::mul_enum;
   op_enum_t neg_op = CppAD::atomic_vector<double>::neg_enum;
   // -----------------------------------------------------------------------
   // Record f(u, v, w) = - (u + v) * w
   // -----------------------------------------------------------------------
   // Independent variable vector
   CPPAD_TESTVECTOR( CppAD::AD<double> ) a_ind(n);
   for(size_t j = 0; j < n; ++j)
      a_ind[j] = AD<double>(1 + j);
   CppAD::Independent(a_ind);
   //
   // au, av, aw
   CPPAD_TESTVECTOR( CppAD::AD<double> ) au(m), av(m), aw(m);
   for(size_t i = 0; i < m; ++i)
   {  au[i] = a_ind[0 * m + i];
      av[i] = a_ind[1 * m + i];
      aw[i] = a_ind[2 * m + i];
   }
   //
   // ax = (au, av)
   CPPAD_TESTVECTOR( CppAD::AD<double> ) ax(2 * m);
   for(size_t i = 0; i < m; ++i)
   {  ax[i]     = au[i];
      ax[m + i] = av[i];
   }
   //
   // ay = u + v
   CPPAD_TESTVECTOR( CppAD::AD<double> ) ay(m);
   vec_op(add_op, ax, ay);
   //
   // ax = (ay, aw)
   for(size_t i = 0; i < m; ++i)
   {  ax[i]     = ay[i];
      ax[m + i] = aw[i];
   }
   //
   // az = ay * w
   CPPAD_TESTVECTOR( CppAD::AD<double> ) az(m);
   vec_op(mul_op, ax, az);
   //
   // ay = - az
   vec_op(neg_op, az, ay);
   //
   // f
   CPPAD_TESTVECTOR( CppAD::AD<double> ) a_dep(1);
   a_dep[0] = ay[0];
   CppAD::ADFun<double> f(a_ind, a_dep);
   //
   // size_var
   // phantom variable, independent variables, operator results
   ok   &= f.size_var() == 1 + n + 3 * m;
   //
   // optimize
   // The atomic function rev_depend routine is called by optimizer
   f.optimize("val_graph no_conditional_skip");
   //
   // size_var
   // phantom variablem, independent variables, operator variables
   ok &= f.size_var() == 1 + n + 3;
   //
   //
   return ok;
}
// END C++
