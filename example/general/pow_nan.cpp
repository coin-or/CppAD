// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin pow_nan.cpp}

pow: Nan in Result of Pow Function: Example and Test
####################################################

Purpose
*******
The :ref:`pow(x, y)<pow-name>` function will work when :math:`x < 0` and
:math:`y`  is a parameter. It will often generate nan or infinity when
:math:`x < 0` and one tries to compute a derivatives
(even if :math:`y` is a positive integer).
This is because the derivative of the log is :math:`1 / x`
and the power function uses the representation

.. math::

   \R{pow}(x, y) = \exp [ y \cdot \log(x) ]

Problem
*******
There is a problem with this representation when :math:`y` is a parameter
and :math:`x = 0`. For example,
when :math:`x = 0` and :math:`y = 1`, it returns zero for the derivative,
but the actual derivative w.r.t :math:`x` is one.

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end pow_nan.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cmath>

bool pow_nan(void)
{  bool ok = true;

   using std::cout;
   using CppAD::AD;
   using CppAD::vector;
   //
   vector<double>       x(2), y(2), dx(2), dy(2), ddx(2), ddy(2);
   vector< AD<double> > ax(2), ay(2);
   //
   // variable vector
   ax[0] = x[0]  = -1.0;
   ax[1] = x[1] = 2.0;
   //
   CppAD::Independent(ax);
   //
   ay[0] = pow( ax[0], ax[1] );
   ay[1] = pow( ax[0], 2.0 );
   //
   CppAD::ADFun<double> f(ax, ay);
   //
   // check_for_nan is set false so it does not generate an assert
   // when compiling with debugging
   f.check_for_nan(false);
   //
   // Zero order forward does not generate nan
   y  = f.Forward(0, x);
   ok &= y[0] == 1.0;
   ok &= y[1] == 1.0;
   //
   // First order forward generates a nan
   dx[0] = 1.0;
   dx[1] = 1.0;
   dy = f.Forward(1, dx);
   ok &= std::isnan( dy[0] );
   ok &= dy[1] == -2.0;
   //
   // Second order Taylor coefficient is 1/2 times second derivative
   ddx[0] = 0.0;
   ddx[1] = 0.0;
   ddy = f.Forward(2, ddx);
   ok &= std::isnan( ddy[0] );
   ok &= ddy[1] == 1.0;
   //
   return ok;
}
// END C++
