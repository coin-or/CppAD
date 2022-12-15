// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin exp_eps_cppad}

exp_eps: CppAD Forward and Reverse Sweeps
#########################################

Purpose
*******
Use CppAD forward and reverse modes to compute the
partial derivative with respect to :math:`x`,
at the point :math:`x = .5` and :math:`\varepsilon = .2`,
of the function

   ``exp_eps`` ( *x* , *epsilon* )

as defined by the :ref:`exp_eps.hpp-name` include file.

Exercises
*********

#. Create and test a modified version of the routine below that computes
   the same order derivatives with respect to :math:`x`,
   at the point :math:`x = .1` and :math:`\varepsilon = .2`,
   of the function

      ``exp_eps`` ( *x* , *epsilon* )

#. Create and test a modified version of the routine below that computes
   partial derivative with respect to :math:`x`,
   at the point :math:`x = .1` and :math:`\varepsilon = .2`,
   of the function corresponding to the operation sequence
   for :math:`x = .5` and :math:`\varepsilon = .2`.
   Hint: you could define a vector u with two components and use

      *f* . ``Forward`` (0, *u* )

   to run zero order forward mode at a point different
   form the point where the operation sequence corresponding to
   *f* was recorded.

{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/cppad.hpp>  // http://www.coin-or.org/CppAD/
# include "exp_eps.hpp"      // our example exponential function approximation
bool exp_eps_cppad(void)
{  bool ok = true;
   using CppAD::AD;
   using CppAD::vector;    // can use any simple vector template class
   using CppAD::NearEqual; // checks if values are nearly equal

   // domain space vector
   size_t n = 2; // dimension of the domain space
   vector< AD<double> > U(n);
   U[0] = .5;    // value of x for this operation sequence
   U[1] = .2;    // value of e for this operation sequence

   // declare independent variables and start recording operation sequence
   CppAD::Independent(U);

   // evaluate our exponential approximation
   AD<double> x       = U[0];
   AD<double> epsilon = U[1];
   AD<double> apx = exp_eps(x, epsilon);

   // range space vector
   size_t m = 1;  // dimension of the range space
   vector< AD<double> > Y(m);
   Y[0] = apx;    // variable that represents only range space component

   // Create f: U -> Y corresponding to this operation sequence
   // and stop recording. This also executes a zero order forward
   // mode sweep using values in U for x and e.
   CppAD::ADFun<double> f(U, Y);

   // first order forward mode sweep that computes partial w.r.t x
   vector<double> du(n);      // differential in domain space
   vector<double> dy(m);      // differential in range space
   du[0] = 1.;                // x direction in domain space
   du[1] = 0.;
   dy    = f.Forward(1, du);  // partial w.r.t. x
   double check = 1.5;
   ok   &= NearEqual(dy[0], check, 1e-10, 1e-10);

   // first order reverse mode sweep that computes the derivative
   vector<double>  w(m);     // weights for components of the range
   vector<double> dw(n);     // derivative of the weighted function
   w[0] = 1.;                // there is only one weight
   dw   = f.Reverse(1, w);   // derivative of w[0] * exp_eps(x, epsilon)
   check = 1.5;              // partial w.r.t. x
   ok   &= NearEqual(dw[0], check, 1e-10, 1e-10);
   check = 0.;               // partial w.r.t. epsilon
   ok   &= NearEqual(dw[1], check, 1e-10, 1e-10);

   // second order forward sweep that computes
   // second partial of exp_eps(x, epsilon) w.r.t. x
   vector<double> x2(n);     // second order Taylor coefficients
   vector<double> y2(m);
   x2[0] = 0.;               // evaluate partial w.r.t x
   x2[1] = 0.;
   y2    = f.Forward(2, x2);
   check = 0.5 * 1.;         // Taylor coef is 1/2 second derivative
   ok   &= NearEqual(y2[0], check, 1e-10, 1e-10);

   // second order reverse sweep that computes
   // derivative of partial of exp_eps(x, epsilon) w.r.t. x
   dw.resize(2 * n);         // space for first and second derivative
   dw    = f.Reverse(2, w);
   check = 1.;               // result should be second derivative
   ok   &= NearEqual(dw[0*2+1], check, 1e-10, 1e-10);

   return ok;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end exp_eps_cppad}
*/
