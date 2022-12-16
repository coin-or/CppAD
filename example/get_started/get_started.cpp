// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin get_started.cpp}

Getting Started Using CppAD to Compute Derivatives
##################################################

Purpose
*******
Demonstrate the use of CppAD by computing the derivative
of a simple example function.

Function
********
The example function :math:`f : \B{R} \rightarrow \B{R}` is defined by

.. math::

   f(x) = a_0 + a_1 * x^1 + \cdots + a_{k-1} * x^{k-1}

where *a* is a fixed vector of length *k* .

Derivative
**********
The derivative of :math:`f(x)` is given by

.. math::

   f' (x) = a_1 + 2 * a_2 * x +  \cdots + (k-1) * a_{k-1} * x^{k-2}

Value
*****
For the particular case in this example,
:math:`k` is equal to 5,
:math:`a = (1, 1, 1, 1, 1)`, and
:math:`x = 3`.
If follows that

.. math::

   f' ( 3 ) = 1 + 2 * 3 + 3 * 3^2 + 4 * 3^3 = 142

Include File
************
The following command, in the program below, includes the CppAD package:

   # ``include <cppad/cppad.hpp>``

Poly
****
The routine ``Poly`` , defined below, evaluates a polynomial.
A general purpose polynomial evaluation routine is documented and
distributed with CppAD; see :ref:`Poly-name` .

CppAD Namespace
***************
All of the functions and objects defined by CppAD are in the
``CppAD`` namespace. In the example below,

   ``using CppAD::AD`` ;

enables one to abbreviate ``CppAD::AD`` using just ``AD`` .

CppAD Preprocessor Symbols
**************************
All the :ref:`preprocessor-name` symbols defined by CppAD begin with
``CPPAD_`` (some deprecated symbols begin with ``CppAD_`` ).
The preprocessor symbol :ref:`CPPAD_TESTVECTOR<testvector-name>`
is used in the example below.

Program
*******
{xrst_spell_off}
{xrst_code cpp} */
# include <iostream>        // standard input/output
# include <vector>          // standard vector
# include <cppad/cppad.hpp> // the CppAD package

namespace { // begin the empty namespace
   // define the function Poly(a, x) = a[0] + a[1]*x[1] + ... + a[k-1]*x[k-1]
   template <class Type>
   Type Poly(const CPPAD_TESTVECTOR(double) &a, const Type &x)
   {  size_t k  = a.size();
      Type y   = 0.;  // initialize summation
      Type x_i = 1.;  // initialize x^i
      for(size_t i = 0; i < k; i++)
      {  y   += a[i] * x_i;  // y   = y + a_i * x^i
         x_i *= x;           // x_i = x_i * x
      }
      return y;
   }
}
// main program
int main(void)
{  using CppAD::AD;   // use AD as abbreviation for CppAD::AD
   using std::vector; // use vector as abbreviation for std::vector

   // vector of polynomial coefficients
   size_t k = 5;                  // number of polynomial coefficients
   CPPAD_TESTVECTOR(double) a(k); // vector of polynomial coefficients
   for(size_t i = 0; i < k; i++)
      a[i] = 1.;                 // value of polynomial coefficients

   // domain space vector
   size_t n = 1;               // number of domain space variables
   vector< AD<double> > ax(n); // vector of domain space variables
   ax[0] = 3.;                 // value at which function is recorded

   // declare independent variables and start recording operation sequence
   CppAD::Independent(ax);

   // range space vector
   size_t m = 1;               // number of ranges space variables
   vector< AD<double> > ay(m); // vector of ranges space variables
   ay[0] = Poly(a, ax[0]);     // record operations that compute ay[0]

   // store operation sequence in f: X -> Y and stop recording
   CppAD::ADFun<double> f(ax, ay);

   // compute derivative using operation sequence stored in f
   vector<double> jac(m * n); // Jacobian of f (m by n matrix)
   vector<double> x(n);       // domain space vector
   x[0] = 3.;                 // argument value for computing derivative
   jac  = f.Jacobian(x);      // Jacobian for operation sequence

   // print the results
   std::cout << "f'(3) computed by CppAD = " << jac[0] << std::endl;

   // check if the derivative is correct
   int error_code;
   if( jac[0] == 142. )
      error_code = 0;      // return code for correct case
   else  error_code = 1;    // return code for incorrect case

   return error_code;
}
/* {xrst_code}
{xrst_spell_on}
Output
******
Executing the program above will generate the following output:
::

   f'(3) computed by CppAD = 142

Running
*******
After you configure your system using the :ref:`cmake-name` command,
you compile and run this example by executing the command

   ``make check_example_get_started``

in the build directory; i.e., the directory where the cmake command
was executed.

Exercises
*********
Modify the program above to accomplish the following tasks
using CppAD:

#. Compute and print the derivative of :math:`f(x) = 1 + x + x^2 + x^3 + x^4`
   at the point :math:`x = 2`.
#. Compute and print the derivative of :math:`f(x) = 1 + x + x^2 / 2`
   at the point :math:`x = .5`.
#. Compute and print the derivative of :math:`f(x) = \exp (x) - 1 - x - x^2 / 2`
   at the point :math:`x = .5`.

{xrst_end get_started.cpp}
*/
