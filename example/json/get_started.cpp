// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin json_get_started.cpp}
{xrst_spell
   dx
}

Json Get Started: Example and Test
##################################

Notation
********

.. csv-table::
   :widths: auto

   Notation,Description,size
   :ref:`json_ad_graph@Node Indices@p`,vector of dynamic parameters,1
   :ref:`json_ad_graph@Node Indices@x`,vector of independent variables,1
   :ref:`json_ad_graph@Node Indices@c`,vector of constants,1
   y,vector of dependent variables,1

Node Table
**********

.. csv-table::
   :widths: auto

   index,value
   1,p[0]
   2,x[0]
   3,c[0]
   4,sin(p[0])
   5,sin(x[0])
   6,sin(c[0])
   7,sin(p[0]) + sin(x[0]) + sin(c[0])
   y[0],sin(p[0]) + sin(x[0]) + sin(c[0])

Include
*******
Include the CppAD core functions:
{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/cppad.hpp>
/* {xrst_code}
{xrst_spell_on}

Syntax
******
*ok* = ``get_started`` ()
{xrst_spell_off}
{xrst_code cpp} */
bool get_started(void)
{
/* {xrst_code}
{xrst_spell_on}

Setup
*****
{xrst_spell_off}
{xrst_code cpp} */
   bool ok = true;
   using CppAD::vector;
   using CppAD::AD;
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
/* {xrst_code}
{xrst_spell_on}
Function
********

Begin Function
==============
See :ref:`json_ad_graph@AD Graph@function` :
{xrst_spell_off}
{xrst_code cpp} */
   std::string json =
      "{\n"
      "   'function_name'  : 'get_started example',\n"
/* {xrst_code}
{xrst_spell_on}
Begin op_define_vec
===================
see :ref:`json_ad_graph@op_define_vec` :
{xrst_spell_off}
{xrst_code cpp} */
      "   'op_define_vec'  : [ 2, [\n"
/* {xrst_code}
{xrst_spell_on}
Define Unary
============
see :ref:`json_graph_op@Unary Operators` :
{xrst_spell_off}
{xrst_code cpp} */
      "       { 'op_code':1, 'name':'sin', 'n_arg':1 } ,\n"
/* {xrst_code}
{xrst_spell_on}
Define Sum
==========
see :ref:`json_graph_op@sum` :
{xrst_spell_off}
{xrst_code cpp} */
      "       { 'op_code':2, 'name':'sum'            } ]\n"
/* {xrst_code}
{xrst_spell_on}
End op_define_vec
=================
{xrst_spell_off}
{xrst_code cpp} */
      "   ],\n"
/* {xrst_code}
{xrst_spell_on}
n_dynamic_ind
=============
see :ref:`json_ad_graph@dynamic_ind_vec@n_dynamic_ind` :
{xrst_spell_off}
{xrst_code cpp} */
      "   'n_dynamic_ind'  : 1,\n"
/* {xrst_code}
{xrst_spell_on}
n_variable_ind
==============
see :ref:`json_ad_graph@variable_ind_vec@n_variable_ind` :
{xrst_spell_off}
{xrst_code cpp} */
      "   'n_variable_ind' : 1,\n"
/* {xrst_code}
{xrst_spell_on}
constant_vec
============
see :ref:`json_ad_graph@constant_vec` :
{xrst_spell_off}
{xrst_code cpp} */
      "   'constant_vec'   : [ 1, [ -0.1 ] ],\n" // c[0]
/* {xrst_code}
{xrst_spell_on}
Begin op_usage_vec
==================
see :ref:`json_ad_graph@op_usage_vec` :
{xrst_spell_off}
{xrst_code cpp} */
      "   'op_usage_vec'   : [ 4, [\n"
/* {xrst_code}
{xrst_spell_on}
op_usage
========
see op_usage with
:ref:`json_ad_graph@op_usage@n_arg In Definition` :
{xrst_spell_off}
{xrst_code cpp} */
      "       [ 1, 1]                ,\n" // sin(p[0])
      "       [ 1, 2]                ,\n" // sin(x[0])
      "       [ 1, 3]                ,\n" // sin(c[0])
/* {xrst_code}
{xrst_spell_on}
see op_usage with
:ref:`json_ad_graph@op_usage@n_arg Not In Definition` :

{xrst_spell_off}
{xrst_code cpp} */
      "       [ 2, 1, 3, [4, 5, 6] ] ]\n" // sin(p[0])+sin(x[0])+sin(c[0])
/* {xrst_code}
{xrst_spell_on}
End op_usage_vec
================
{xrst_spell_off}
{xrst_code cpp} */
      "   ],\n"
/* {xrst_code}
{xrst_spell_on}
dependent_vec
=============
see :ref:`dependent_var<json_ad_graph@dependent_vec>`
{xrst_spell_off}
{xrst_code cpp} */
      "   'dependent_vec' : [ 1, [7] ] \n"
/* {xrst_code}
{xrst_spell_on}
End Function
============
{xrst_spell_off}
{xrst_code cpp} */
      "}\n";
/* {xrst_code}
{xrst_spell_on}
Convert Single to Double Quotes
*******************************
{xrst_spell_off}
{xrst_code cpp} */
   for(size_t i = 0; i < json.size(); ++i)
      if( json[i] == '\'' ) json[i] = '"';
/* {xrst_code}
{xrst_spell_on}
double f(x, p)
**************
{xrst_spell_off}
{xrst_code cpp} */
   CppAD::ADFun<double> f;
   f.from_json(json);
/* {xrst_code}
{xrst_spell_on}
Check f(x, p)
*************
{xrst_spell_off}
{xrst_code cpp} */
   vector<double> c(1), p(1), x(1), y(1);
   c[0] = -0.1; // must match value in graph
   p[0] = 0.2;  // can be any value
   x[0] = 0.3;  // can be any value
   //
   // compute y = f(x, p)
   f.new_dynamic(p);
   y = f.Forward(0, x);
   //
   // f(x, p) = sin(p_0) + sin(x_0) + sin(c_0)
   double check = std::sin(p[0]) + std::sin(x[0]) + std::sin(c[0]);
   ok &= CppAD::NearEqual(y[0], check, eps99, eps99);

/* {xrst_code}
{xrst_spell_on}
AD<double> f(x, p)
******************
{xrst_spell_off}
{xrst_code cpp} */
   CppAD::ADFun< AD<double>, double > af( f.base2ad() );
/* {xrst_code}
{xrst_spell_on}
Evaluate Derivative
*******************
{xrst_spell_off}
{xrst_code cpp} */
   // set independent variables and parameters
   vector< AD<double> > ap(1), ax(1);
   ap[0] = 0.2;
   ax[0] = 0.3;
   //
   // record computation of z = d/dx f(x, p)
   CppAD::Independent(ax, ap);
   af.new_dynamic(ap);
   vector< AD<double> > az = af.Jacobian(ax);
/* {xrst_code}
{xrst_spell_on}
double g(x, p) = d/dx f(x, p)
*****************************
{xrst_spell_off}
{xrst_code cpp} */
   CppAD::ADFun<double> g(ax, az);
/* {xrst_code}
{xrst_spell_on}
Convert to Json and Back
************************
{xrst_spell_off}
{xrst_code cpp} */
   json = g.to_json();
   g.from_json(json);
/* {xrst_code}
{xrst_spell_on}
Check g(x, p)
*************
{xrst_spell_off}
{xrst_code cpp} */
   c[0] = -0.1; // must match value in graph
   p[0] = 0.3;  // can be any value
   x[0] = 0.4;  // can be any value
   //
   // compute z = g(x, p)
   g.new_dynamic(p);
   vector<double> z = g.Forward(0, x);
   //
   // g(x, p) = d/dx f(x, p) = d/dx sin(x) = cos(x)
   check = std::cos(x[0]);
   ok &= CppAD::NearEqual(z[0], check, eps99, eps99);
   //
   return ok;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end json_get_started.cpp}
*/
