// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin ipopt_solve_ode_inverse.cpp}
{xrst_spell
   np
   nz
   trapezoidal
}

ODE Inverse Problem Definitions: Source Code
############################################

Purpose
*******
This example demonstrates how to invert for parameters
in a ODE where the solution of the ODE is numerically approximated.

Forward Problem
***************
We consider the following ordinary differential equation:

.. math::
   :nowrap:

   \begin{eqnarray}
      \partial_t y_0 ( t , a ) & = & - a_1 * y_0 (t, a )
      \\
      \partial_t y_1 (t , a )  & = & + a_1 * y_0 (t, a ) - a_2 * y_1 (t, a )
   \end{eqnarray}

with the initial conditions

.. math::

   y_0 (0 , a) = ( a_0 , 0 )^\R{T}

Our forward problem is stated as follows:
Given :math:`a \in \B{R}^3`
determine the value of :math:`y ( t , a )`,
for :math:`t \in R`, that solves the initial value problem above.

Measurements
************
Suppose we are also given measurement times :math:`s \in \B{R}^5`
and  a measurement vector :math:`z \in \B{R}^4`
and for :math:`i = 0, \ldots, 3`, we model :math:`z_i` by

.. math::

   z_i = y_1 ( s_{i+1} , a) + e_i

where :math:`e_{i-1} \sim {\bf N} (0 , \sigma^2 )`
is the measurement noise,
and :math:`\sigma > 0` is the standard deviation of the noise.

Simulation Analytic Solution
============================
The following analytic solution to the forward problem is used
to simulate a data set:

.. math::
   :nowrap:

   \begin{eqnarray}
      y_0 (t , a) & = & a_0 * \exp( - a_1 * t )
      \\
      y_1 (t , a) & = &
      a_0 * a_1 * \frac{\exp( - a_2 * t ) - \exp( -a_1 * t )}{ a_1 - a_2 }
   \end{eqnarray}

Simulation Parameter Values
===========================

.. list-table::
   :widths: auto

   * - :math:`\bar{a}_0 = 1`
     - initial value of :math:`y_0 (t, a)`
   * - :math:`\bar{a}_1 = 2`
     - transfer rate from compartment zero to compartment one
   * - :math:`\bar{a}_2 = 1`
     - transfer rate from compartment one to outside world
   * - :math:`\sigma = 0`
     - standard deviation of measurement noise
   * - :math:`e_i = 0`
     - simulated measurement noise, :math:`i = 1 , \ldots , Nz`
   * - :math:`s_i = i * .5`
     - time corresponding to the *i*-th measurement,
       :math:`i = 0 , \ldots , 3`

Simulated Measurement Values
============================
The simulated measurement values are given by the equation

.. math::
   :nowrap:

   \begin{eqnarray}
   z_i
   & = &  e_i + y_1 ( s_{i+1} , \bar{a} )
   \\
   & = &
   \bar{a}_0 * \bar{a}_1 *
      \frac{\exp( - \bar{a}_2 * s_i ) - \exp( -\bar{a}_1 * s_i )}
         { \bar{a}_1 - \bar{a}_2 }
   \end{eqnarray}

for :math:`i = 0, \ldots , 3`.

Inverse Problem
***************
The maximum likelihood estimate for :math:`a` given :math:`z`
solves the following optimization problem

.. math::
   :nowrap:

   \begin{eqnarray}
   {\rm minimize} \;
      & \sum_{i=0}^3 ( z_i - y_1 ( s_{i+1} , a ) )^2
      & \;{\rm w.r.t} \; a \in \B{R}^3
   \end{eqnarray}

Trapezoidal Approximation
*************************
We are given a number of approximation points per measurement interval
:math:`np` and define the time grid :math:`t \in \B{R}^{4 \cdot np + 1}`
as follows:
:math:`t_0 = s_0` and
for :math:`i = 0 , 1 , 2, 3`, :math:`j = 1 , \ldots , np`

.. math::

   t_{i \cdot np + j} = s_i + (s_{i+1} - s{i}) \frac{i}{np}

We note that for :math:`i = 1 , \ldots , 4`,
:math:`t_{i \cdot np} = s_i`.
This example uses a trapezoidal approximation to solve the ODE.
Given :math:`a \in \B{R}^3` and :math:`y^{k-1} \approx y( t_{k-1} , a )`,
the a trapezoidal method approximates :math:`y ( t_j , a )`
by the value :math:`y^k \in \B{R}^2` ) that solves the equation

.. math::

   y^k  =  y^{k-1} + \frac{G( y^k , a ) + G( y^{k-1} , a ) }{2} * (t_k - t_{k-1})

where :math:`G : \B{R}^2 \times \B{R}^3 \rightarrow \B{R}^2` is defined by

.. math::
   :nowrap:

   \begin{eqnarray}
      G_0 ( y , a ) & = & - a_1 * y_0
      \\
      G_1 ( y , a ) & = & + a_1 * y_0  - a_2 * y_1
   \end{eqnarray}

Solution Method
***************
We use constraints to embed the
forward problem in the inverse problem.
To be specific, we solve the optimization problem

.. math::
   :nowrap:

   \begin{eqnarray}
   {\rm minimize}
   & \sum_{i=0}^3 ( z_i - y_1^{(i+1) \cdot np} )^2
   & \; {\rm w.r.t} \; a \in \B{R}^3
      \; y^0 \in \B{R}^2 , \ldots , y^{3 \cdot np -1} \in \B{R}^2
   \\
   {\rm subject \; to}
      0 = y^0 - ( a_0 , 0 )^\R{T}
      \\
      & 0 = y^k  -  y^{k-1} -
      \frac{G( y^k , a ) + G( y^{k-1} , a ) }{2}  (t_k - t_{k-1})
      & \; {\rm for} \; k = 1 , \ldots , 4 \cdot np
   \end{eqnarray}

The code below we using the notation
:math:`x \in \B{3 + (4 \cdot np + 1) \cdot 2}` defined by

.. math::

   x = \left(
      a_0, a_1, a_2 ,
      y_0^0, y_1^0,
      \ldots ,
      y_0^{4 \cdot np}, y_1^{4 \cdots np}
   \right)

Source
******
The following source code
implements the ODE inversion method proposed above:
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end ipopt_solve_ode_inverse.cpp}
------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/ipopt/solve.hpp>

namespace {
   using CppAD::AD;

   // value of a during simulation a[0], a[1], a[2]
   double a_[] =                   {2.0,  1.0, 0.5};
   // number of components in a
   size_t na_ = sizeof(a_) / sizeof(a_[0]);

   // function used to simulate data
   double yone(double t)
   {  return
         a_[0]*a_[1] * (exp(-a_[2]*t) - exp(-a_[1]*t)) / (a_[1] - a_[2]);
   }

   // time points were we have data (no data at first point)
   double s_[] = {0.0,   0.5,        1.0,          1.5,         2.0 };

   // Simulated data for case with no noise (first point is not used)
   double z_[] = {yone(s_[1]), yone(s_[2]), yone(s_[3]), yone(s_[4])};
   size_t nz_  = sizeof(z_) / sizeof(z_[0]);

   // number of trapozoidal approximation points per measurement interval
   size_t np_  = 40;


   class FG_eval
   {
   private:
   public:
      // derived class part of constructor
      typedef CPPAD_TESTVECTOR( AD<double> ) ADvector;

      // Evaluation of the objective f(x), and constraints g(x)
      void operator()(ADvector& fg, const ADvector& x)
      {  CPPAD_TESTVECTOR( AD<double> ) a(na_);
         size_t i, j, k;

         // extract the vector a
         for(i = 0; i < na_; i++)
            a[i] = x[i];

         // compute the object f(x)
         fg[0] = 0.0;
         for(i = 0; i < nz_; i++)
         {  k = (i + 1) * np_;
            AD<double> y_1 = x[na_ + 2 * k + 1];
            AD<double> dif = z_[i] - y_1;
            fg[0]         += dif * dif;
         }

         // constraint corresponding to initial value y(0, a)
         // Note that this constraint is invariant with size of dt
         fg[1] = x[na_+0] - a[0];
         fg[2] = x[na_+1] - 0.0;

         // constraints corresponding to trapozoidal approximation
         for(i = 0; i < nz_; i++)
         {  // spacing between grid point
            double dt = (s_[i+1] - s_[i]) / static_cast<double>(np_);
            for(j = 1; j <= np_; j++)
            {  k = i * np_ + j;
               // compute derivative at y^k
               AD<double> y_0  = x[na_ + 2 * k + 0];
               AD<double> y_1  = x[na_ + 2 * k + 1];
               AD<double> G_0  = - a[1] * y_0;
               AD<double> G_1  = + a[1] * y_0 - a[2] * y_1;

               // compute derivative at y^{k-1}
               AD<double> ym_0  = x[na_ + 2 * (k-1) + 0];
               AD<double> ym_1  = x[na_ + 2 * (k-1) + 1];
               AD<double> Gm_0  = - a[1] * ym_0;
               AD<double> Gm_1  = + a[1] * ym_0 - a[2] * ym_1;

               // constraint should be zero
               fg[1 + 2*k ] = y_0  - ym_0 - dt*(G_0 + Gm_0)/2.;
               fg[2 + 2*k ] = y_1  - ym_1 - dt*(G_1 + Gm_1)/2.;

               // scale g(x) so it has similar size as f(x)
               fg[1 + 2*k ] /= dt;
               fg[2 + 2*k ] /= dt;
            }
         }
      }
   };
}
bool ode_inverse(void)
{  bool ok = true;
   size_t i;
   typedef CPPAD_TESTVECTOR( double ) Dvector;

   // number of components in the function g
   size_t ng = (np_ * nz_ + 1) * 2;
   // number of independent variables
   size_t nx = na_ + ng;
   // initial vlaue for the variables we are optimizing w.r.t
   Dvector xi(nx), xl(nx), xu(nx);
   for(i = 0; i < nx; i++)
   {  xi[i] =   0.0; // initial value
      xl[i] = -1e19; // no lower limit
      xu[i] = +1e19; // no upper limit
   }
   for(i = 0; i < na_; i++)
      xi[0] = 1.5;   // initial value for a

   // all the difference equations are constrainted to be zero
   Dvector gl(ng), gu(ng);
   for(i = 0; i < ng; i++)
   {  gl[i] = 0.0;
      gu[i] = 0.0;
   }
   // object defining both f(x) and g(x)
   FG_eval fg_eval;

   // options
   std::string options;
   // Use sparse matrices for calculation of Jacobians and Hessians
   // with forward mode for Jacobian (seems to be faster for this case).
   options += "Sparse  true        forward\n";
   // turn off any printing
   options += "Integer print_level 0\n";
   options += "String  sb        yes\n";
   // maximum number of iterations
   options += "Integer max_iter    30\n";
   // approximate accuracy in first order necessary conditions;
   // see Mathematical Programming, Volume 106, Number 1,
   // Pages 25-57, Equation (6)
   options += "Numeric tol         1e-6\n";

   // place to return solution
   CppAD::ipopt::solve_result<Dvector> solution;

   // solve the problem
   CppAD::ipopt::solve<Dvector, FG_eval>(
      options, xi, xl, xu, gl, gu, fg_eval, solution
   );
   //
   // Check some of the solution values
   //
   ok &= solution.status == CppAD::ipopt::solve_result<Dvector>::success;
   //
   double rel_tol    = 1e-4;  // relative tolerance
   double abs_tol    = 1e-4;  // absolute tolerance
   for(i = 0; i < na_; i++)
      ok &= CppAD::NearEqual( a_[i],  solution.x[i],   rel_tol, abs_tol);

   return ok;
}
// END C++
