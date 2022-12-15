// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin base2vec_ad.cpp}

Using base2ad and VecAD together: Example and Test
##################################################

Purpose
*******
This example demonstrate that :ref:`base2ad-name` does not convert
:ref:`VecAD-name` operations as might be expected.
To be specific, this indexing into a VecAD object gets fixed
when zero order forward mode is run for a ``base2ad`` result; e.g.
*af* below.

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end base2vec_ad.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool base2vec_ad(void)
{  using CppAD::vector;
   using CppAD::AD;
   //
   bool ok = true;
   //
   // f(x) = x[0] if 0 <= x[2] < 1
   //      = x[1] if 1 <= x[2] < 2
   vector< AD<double> > ax(3), ay(1);
   ax[0] = 3.0;
   ax[1] = 2.0;
   ax[2] = 1.0;
   Independent(ax);
   CppAD::VecAD<double> av(2);
   av[ AD<double>(0) ] = ax[0];
   av[ AD<double>(1) ] = ax[1];
   ay[0] = av[ ax[2] ];
   CppAD::ADFun<double> f(ax, ay);
   //
   // ok
   // value during recording of f
   ok &= ay[0] == ax[1];
   //
   // ok
   // f zero order forward mode, the VecAD commands give expected result
   vector<double> x(3), y(1);
   x[0] = 2.0;
   x[1] = 1.0;
   x[2] = 0.0;
   y = f.Forward(0, x);
   ok &= y[0] == x[0];
   //
   // af
   CppAD::ADFun< AD<double>, double > af = f.base2ad();
   //
   // g
   ax[0] = 3.0;
   ax[1] = 2.0;
   ax[2] = 1.0;
   Independent(ax);
   ay = af.Forward(0, ax);
   CppAD::ADFun<double> g(ax, ay);
   //
   // ok
   // value during recording of g
   ok &= ay[0] == ax[1];
   //
   // ok
   // g zero order forward mode
   // Note that this index does not change, but the value does
   x[0] = 5.0;
   x[1] = 4.0;
   x[2] = 0.0;
   y = g.Forward(0, x);
   ok &= y[0] == x[1];
   //
   return ok;
}
// END C++
