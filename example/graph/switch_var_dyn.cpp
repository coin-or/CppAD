// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin switch_var_dyn.cpp}

Switching Between Variables and Dynamic Parameters: Example and Test
####################################################################

Function
********
For each :ref:`ADFun-name` object there is a corresponding function
:math:`f(x, p)` where
:ref:`independent@x` is the vector of independent variables
and *p* is the vector of
independent :ref:`Independent@dynamic` parameters.

Convert a Function to a Graph
*****************************
The :ref:`to_graph-name` routine can be used to convert a ``ADFun``
to a graph representation; see :ref:`cpp_ad_graph-name` .

Convert a Graph to a Function
*****************************
The :ref:`from_graph-name` routine can be used to convert a graph back
to a function. During this conversion, it is possible to change
dynamic parameters to variables and variables to dynamic parameters;
see :ref:`from_graph@dyn2var` and *var2dyn* in the
``from_graph`` documentation.
Note that many such conversions can be done
using the same ``cpp_ad_graph`` object.

Source Code
***********
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end switch_var_dyn.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool switch_var_dyn(void)
{  bool ok = true;
   using std::string;
   //
   // f(x_0, x_1, x_2) = y_0 = x_2 * ( x_0 + x_1 );
   CPPAD_TESTVECTOR( CppAD::AD<double> ) ax(3), ay(1);
   for(size_t j = 0; j < 3; ++j)
      ax[j] = CppAD::AD<double>(j);
   Independent(ax);
   ay[0] = ax[2] * ( ax[0] + ax[1] );
   CppAD::ADFun<double> f(ax, ay);
   ok &= f.Domain() == 3;
   ok &= f.Range() == 1;
   ok &= f.size_dyn_ind() == 0;
   //
   // set independent variables and parameters
   CPPAD_TESTVECTOR(double) p(0), x(3);
   x[0] = 2.0;
   x[1] = 3.0;
   x[2] = 4.0;
   //
   // compute y = f(x)
   f.new_dynamic(p);
   CPPAD_TESTVECTOR(double) y = f.Forward(0, x);
   //
   // check result
   ok &= y[0] == x[2] * ( x[0] + x[1] );
   // -----------------------------------------------------------------------
   //
   // C++ graph object
   CppAD::cpp_graph graph_obj;
   f.to_graph(graph_obj);
   //
   // change x[0]->p[0], x[1]->p[1], x[2]->x[0]
   CppAD::vector<bool> dyn2var(0), var2dyn(3);
   var2dyn[0] = true;
   var2dyn[1] = true;
   var2dyn[2] = false;
   f.from_graph(graph_obj, dyn2var, var2dyn);
   p.resize(2);
   x.resize(1);
   //
   ok &= f.Domain() == 1;
   ok &= f.Range() == 1;
   ok &= f.size_dyn_ind() == 2;
   //
   // set independent variables and parameters
   p[0] = 1.0;
   p[1] = 2.0;
   x[0] = 3.0;
   //
   // compute y = f(x, p)
   f.new_dynamic(p);
   y = f.Forward(0, x);
   //
   // check result
   ok &= y[0] == x[0] * ( p[0] + p[1] );
   //
   return ok;
}
// END C++
