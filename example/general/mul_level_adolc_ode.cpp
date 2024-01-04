// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin mul_level_adolc_ode.cpp}
{xrst_spell
   adouble
   cccc
}

Taylor's Ode Solver: A Multi-Level Adolc Example and Test
#########################################################

See Also
********
:ref:`taylor_ode.cpp-name` , :ref:`mul_level_ode.cpp-name`

Purpose
*******
This is a realistic example using
two levels of AD; see :ref:`mul_level-name` .
The first level uses Adolc's ``adouble`` type
to tape the solution of an ordinary differential equation.
This solution is then differentiated with respect to a parameter vector.
The second level uses CppAD's type ``AD<adouble>``
to take derivatives during the solution of the differential equation.
These derivatives are used in the application
of Taylor's method to the solution of the ODE.

ODE
***
For this example the function
:math:`y : \B{R} \times \B{R}^n \rightarrow \B{R}^n` is defined by
:math:`y(0, x) = 0` and
:math:`\partial_t y(t, x) = g(y, x)` where
:math:`g : \B{R}^n \times \B{R}^n \rightarrow \B{R}^n` is defined by

.. math::

   g(y, x) =
   \left( \begin{array}{c}
         x_0     \\
         x_1 y_0 \\
         \vdots  \\
         x_{n-1} y_{n-2}
   \end{array} \right)

ODE Solution
************
The solution for this example can be calculated by
starting with the first row and then using the solution
for the first row to solve the second and so on.
Doing this we obtain

.. math::

   y(t, x ) =
   \left( \begin{array}{c}
      x_0 t                  \\
      x_1 x_0 t^2 / 2        \\
      \vdots                 \\
      x_{n-1} x_{n-2} \ldots x_0 t^n / n !
   \end{array} \right)

Derivative of ODE Solution
**************************
Differentiating the solution above,
with respect to the parameter vector :math:`x`,
we notice that

.. math::

   \partial_x y(t, x ) =
   \left( \begin{array}{cccc}
   y_0 (t,x) / x_0      & 0                   & \cdots & 0      \\
   y_1 (t,x) / x_0      & y_1 (t,x) / x_1     & 0      & \vdots \\
   \vdots               & \vdots              & \ddots & 0      \\
   y_{n-1} (t,x) / x_0  & y_{n-1} (t,x) / x_1 & \cdots & y_{n-1} (t,x) / x_{n-1}
   \end{array} \right)

Taylor's Method Using AD
************************
We define the function :math:`z(t, x)` by the equation

.. math::

   z ( t , x ) = g[ y ( t , x ) ] = h [ x , y( t , x ) ]

see :ref:`taylor_ode-name` for the method used to compute the
Taylor coefficients w.r.t :math:`t` of :math:`y(t, x)`.

base_adolc.hpp
**************
The file :ref:`base_adolc.hpp-name` is implements the
:ref:`Base type requirements<base_require-name>` where *Base*
is ``adolc`` .

Memory Management
*****************
Adolc uses raw memory arrays that depend on the number of
dependent and independent variables.
The :ref:`thread_alloc-name` memory management utilities
:ref:`create_array<ta_create_array-name>` and
:ref:`delete_array<ta_delete_array-name>`
are used to manage this memory allocation.

Configuration Requirement
*************************
This example will be compiled and tested provided
:ref:`cmake@include_ipopt` is on the cmake command line.

Source
******

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end mul_level_adolc_ode.cpp}
--------------------------------------------------------------------------
*/
// BEGIN C++
// suppress conversion warnings before other includes
# include <cppad/wno_conversion.hpp>
//

# include <adolc/adouble.h>
# include <adolc/taping.h>
# include <adolc/drivers/drivers.h>

// definitions not in Adolc distribution and required to use CppAD::AD<adouble>
# include <cppad/example/base_adolc.hpp>

# include <cppad/cppad.hpp>
// ==========================================================================
namespace { // BEGIN empty namespace
// define types for each level
typedef adouble           a1type;
typedef CppAD::AD<a1type> a2type;

// -------------------------------------------------------------------------
// class definition for C++ function object that defines ODE
class Ode {
private:
   // copy of a that is set by constructor and used by g(y)
   CPPAD_TESTVECTOR(a1type) a1x_;
public:
   // constructor
   Ode(const CPPAD_TESTVECTOR(a1type)& a1x) : a1x_(a1x)
   { }
   // the function g(y) is evaluated with two levels of taping
   CPPAD_TESTVECTOR(a2type) operator()
   ( const CPPAD_TESTVECTOR(a2type)& a2y) const
   {  size_t n = a2y.size();
      CPPAD_TESTVECTOR(a2type) a2g(n);
      size_t i;
      a2g[0] = a1x_[0];
      for(i = 1; i < n; i++)
         a2g[i] = a1x_[i] * a2y[i-1];

      return a2g;
   }
};

// -------------------------------------------------------------------------
// Routine that uses Taylor's method to solve ordinary differential equaitons
// and allows for algorithmic differentiation of the solution.
CPPAD_TESTVECTOR(a1type) taylor_ode_adolc(
   Ode                            G       ,  // function that defines the ODE
   size_t                         order   ,  // order of Taylor's method used
   size_t                         nstep   ,  // number of steps to take
   const a1type                   &a1dt   ,  // Delta t for each step
   const CPPAD_TESTVECTOR(a1type) &a1y_ini)  // y(t) at the initial time
{
   // some temporary indices
   size_t i, k, ell;

   // number of variables in the ODE
   size_t n = a1y_ini.size();

   // copies of x and g(y) with two levels of taping
   CPPAD_TESTVECTOR(a2type)   a2y(n), Z(n);

   // y, y^{(k)} , z^{(k)}, and y^{(k+1)}
   CPPAD_TESTVECTOR(a1type)  a1y(n), a1y_k(n), a1z_k(n), a1y_kp(n);

   // initialize x
   for(i = 0; i < n; i++)
      a1y[i] = a1y_ini[i];

   // loop with respect to each step of Taylors method
   for(ell = 0; ell < nstep; ell++)
   {  // prepare to compute derivatives using a1type
      for(i = 0; i < n; i++)
         a2y[i] = a1y[i];
      CppAD::Independent(a2y);

      // evaluate ODE using a2type
      Z = G(a2y);

      // define differentiable version of g: X -> Y
      // that computes its derivatives using a1type
      CppAD::ADFun<a1type> a1g(a2y, Z);

      // Use Taylor's method to take a step
      a1y_k            = a1y;     // initialize y^{(k)}
      a1type dt_kp = a1dt;    // initialize dt^(k+1)
      for(k = 0; k <= order; k++)
      {  // evaluate k-th order Taylor coefficient of y
         a1z_k = a1g.Forward(k, a1y_k);

         for(i = 0; i < n; i++)
         {  // convert to (k+1)-Taylor coefficient for x
            a1y_kp[i] = a1z_k[i] / a1type(k + 1);

            // add term for to this Taylor coefficient
            // to solution for y(t, x)
            a1y[i]    += a1y_kp[i] * dt_kp;
         }
         // next power of t
         dt_kp *= a1dt;
         // next Taylor coefficient
         a1y_k   = a1y_kp;
      }
   }
   return a1y;
}
} // END empty namespace
// ==========================================================================
// Routine that tests algorithmic differentiation of solutions computed
// by the routine taylor_ode.
bool mul_level_adolc_ode(void)
{  bool ok = true;
   double eps = 100. * std::numeric_limits<double>::epsilon();

   // number of components in differential equation
   size_t n = 4;

   // some temporary indices
   size_t i, j;

   // set up for thread_alloc memory allocator
   using CppAD::thread_alloc; // the allocator
   size_t capacity;           // capacity of an allocation

   // the vector x with length n (or greater) in double
   double* x = thread_alloc::create_array<double>(n, capacity);

   // the vector x with length n in a1type
   CPPAD_TESTVECTOR(a1type) a1x(n);
   for(i = 0; i < n; i++)
      a1x[i] = x[i] = double(i + 1);

   // declare the parameters as the independent variable
   short tag = 0;                     // Adolc setup
   int keep = 1;
   trace_on(tag, keep);
   for(i = 0; i < n; i++)
      a1x[i] <<= double(i + 1);  // a1x is independent for adouble type

   // arguments to taylor_ode_adolc
   Ode G(a1x);                // function that defines the ODE
   size_t   order = n;      // order of Taylor's method used
   size_t   nstep = 2;      // number of steps to take
   a1type   a1dt  = 1.;     // Delta t for each step
   // value of y(t, x) at the initial time
   CPPAD_TESTVECTOR(a1type) a1y_ini(n);
   for(i = 0; i < n; i++)
      a1y_ini[i] = 0.;

   // integrate the differential equation
   CPPAD_TESTVECTOR(a1type) a1y_final(n);
   a1y_final = taylor_ode_adolc(G, order, nstep, a1dt, a1y_ini);

   // declare the differentiable function f : x -> y_final
   // (corresponding to the tape of adouble operations)
   double* y_final = thread_alloc::create_array<double>(n, capacity);
   for(i = 0; i < n; i++)
      a1y_final[i] >>= y_final[i];
   trace_off();

   // check function values
   double check = 1.;
   double t     = nstep * a1dt.value();
   for(i = 0; i < n; i++)
   {  check *= x[i] * t / double(i + 1);
      ok &= CppAD::NearEqual(y_final[i], check, eps, eps);
   }

   // memory where Jacobian will be returned
   double* jac_ = thread_alloc::create_array<double>(n * n, capacity);
   double** jac = thread_alloc::create_array<double*>(n, capacity);
   for(i = 0; i < n; i++)
      jac[i] = jac_ + i * n;

   // evaluate Jacobian of h at a
   size_t m = n;              // # dependent variables
   jacobian(tag, int(m), int(n), x, jac);

   // check Jacobian
   for(i = 0; i < n; i++)
   {  for(j = 0; j < n; j++)
      {  if( i < j )
            check = 0.;
         else
            check = y_final[i] / x[j];
         ok &= CppAD::NearEqual(jac[i][j], check, eps, eps);
      }
   }

   // make memroy avaiable for other use by this thread
   thread_alloc::delete_array(x);
   thread_alloc::delete_array(y_final);
   thread_alloc::delete_array(jac_);
   thread_alloc::delete_array(jac);
   return ok;
}

// END C++
