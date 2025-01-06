// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin mul_level_adolc.cpp}
{xrst_spell
  adouble
  dx
}

Using Adolc with Multiple Levels of Taping: Example and Test
############################################################

Purpose
*******
In this example, we use ``AD< adouble> >`` (level two taping),
the compute values of the function :math:`f : \B{R}^n \rightarrow \B{R}` where

.. math::

   f(x) = \frac{1}{2} \left( x_0^2 + \cdots + x_{n-1}^2 \right)

We then use Adolc's ``adouble`` (level one taping) to compute
the directional derivative

.. math::

   f^{(1)} (x) * v  = x_0 v_0 + \cdots + x_{n-1} v_{n-1}

where :math:`v \in \B{R}^n`.
We then use ``double`` (no taping) to compute

.. math::

   \frac{d}{dx} \left[ f^{(1)} (x) * v \right] = v

This is only meant as an example of multiple levels of taping.
The example :ref:`hes_times_dir.cpp-name` computes the same value more
efficiently by using the identity:

.. math::

   \frac{d}{dx} \left[ f^{(1)} (x) * v \right] = f^{(2)} (x) * v

The example :ref:`mul_level.cpp-name` computes the same values using
``AD< AD<double> >`` and ``AD<double>`` .

Memory Management
*****************
Adolc uses raw memory arrays that depend on the number of
dependent and independent variables.
The memory management utility :ref:`thread_alloc-name`
is used to manage this memory allocation.

Configuration Requirement
*************************
This example will be compiled and tested provided
:ref:`cmake@include_adolc` is true on the cmake command line.

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end mul_level_adolc.cpp}
*/
// BEGIN C++
// suppress conversion warnings before other includes
# include <cppad/wno_conversion.hpp>
//

# include <adolc/adouble.h>
# include <adolc/taping.h>
# include <adolc/interfaces.h>

// adouble definitions not in Adolc distribution and
// required in order to use CppAD::AD<adouble>
# include <cppad/example/base_adolc.hpp>

# include <cppad/cppad.hpp>

namespace {
   // f(x) = |x|^2 / 2 = .5 * ( x[0]^2 + ... + x[n-1]^2 )
   template <class Type>
   Type f(const CPPAD_TESTVECTOR(Type)& x)
   {  Type sum;

      sum  = 0.;
      size_t i = size_t(x.size());
      for(i = 0; i < size_t(x.size()); i++)
         sum += x[i] * x[i];

      return .5 * sum;
   }
}

bool mul_level_adolc(void)
{  bool ok = true;                // initialize test result
   using CppAD::thread_alloc;        // The CppAD memory allocator

   typedef adouble           a1type;  // for first level of taping
   typedef CppAD::AD<a1type> a2type; // for second level of taping
   size_t n = 5;                          // number independent variables
   size_t j;

   // 10 times machine epsilon
   double eps = 10. * std::numeric_limits<double>::epsilon();

   CPPAD_TESTVECTOR(double) x(n);
   CPPAD_TESTVECTOR(a1type) a1x(n);
   CPPAD_TESTVECTOR(a2type) a2x(n);

   // Values for the independent variables while taping the function f(x)
   for(j = 0; j < n; j++)
      a2x[j] = double(j);
   // Declare the independent variable for taping f(x)
   CppAD::Independent(a2x);

   // Use AD<adouble> to tape the evaluation of f(x)
   CPPAD_TESTVECTOR(a2type) a2y(1);
   a2y[0] = f(a2x);

   // Declare a1f as the corresponding ADFun<adouble> function f(x)
   // (make sure we do not run zero order forward during constructor)
   CppAD::ADFun<a1type> a1f;
   a1f.Dependent(a2x, a2y);

   // Value of the independent variables whitle taping f'(x) * v
   short tag = 0;
   int keep = 1;
   trace_on(tag, keep);
   for(j = 0; j < n; j++)
      a1x[j] <<= double(j);

   // set the argument value x for computing f'(x) * v
   a1f.Forward(0, a1x);

   // compute f'(x) * v
   CPPAD_TESTVECTOR(a1type) a1v(n);
   CPPAD_TESTVECTOR(a1type) a1df(1);
   for(j = 0; j < n; j++)
      a1v[j] = double(n - j);
   a1df = a1f.Forward(1, a1v);

   // declare Adolc function corresponding to f'(x) * v
   double df;
   a1df[0] >>= df;
   trace_off();

   // compute the d/dx of f'(x) * v = f''(x) * v
   size_t m      = 1;                     // # dependent in f'(x) * v

   // w = new double[capacity] where capacity >= m
   size_t capacity;
   double* w  = thread_alloc::create_array<double>(m, capacity);

   // dw = new double[capacity] where capacity >= n
   double* dw = thread_alloc::create_array<double>(n, capacity);

   w[0]  = 1.;
   fos_reverse(tag, int(m), int(n), w, dw);

   for(j = 0; j < n; j++)
   {  double vj = a1v[j].value();
      ok &= CppAD::NearEqual(dw[j], vj, eps, eps);
   }

   // make memory avaialble for other use by this thread
   thread_alloc::delete_array(w);
   thread_alloc::delete_array(dw);
   return ok;
}
// END C++
