# ifndef CPPAD_LOCAL_VAR_OP_PAR_OP_HPP
# define CPPAD_LOCAL_VAR_OP_PAR_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

namespace CppAD { namespace local { namespace var_op {
/*
{xrst_begin_parent var_par_op dev}

Create a Variable From a Parameter Operator
###########################################

ParOp
*****
is the op code for this operator.

Purpose
*******
If one of the :ref:`dependent@y` components in an ADFun object is a parameter,
it is converted to a variable using this operator.

n_res
*****
This operator has one variable result (the variable that is created).

i_z
***
is the variable index corresponding the result of this operator.

arg
***

arg[0]
======
is the index of the parameter that is converted to a variable

{xrst_end var_par_op}
------------------------------------------------------------------------------
{xrst_begin var_par_forward_0 dev}

Zero Order Forward Create a Variable From a Parameter
#####################################################

Prototype
*********
{xrst_literal
   // BEGIN_PAR_FORWARD_0
   // END_PAR_FORWARD_0
}

i_z, arg
********
see
ref:`var_par_op@i_z` ,
ref:`var_par_op@arg`

num_par
*******
is the total number of values in the *parameter* vector.

parameter
*********
maps parameter indices to parameter values .

taylor
******
The Taylor coefficient corresponding to variable *i* and order *k* is

   *taylor* [ *i* * *cap_order* + *k*  ]

Input
=====
The zero order Taylor coefficients
for variables with index *i* less than or equal *i_z* .

Output
======
The zero order Taylor coefficients for variables with index *i_z* ; i.e.,
the value of the parameter.

{xrst_end var_par_forward_0}
*/
// BEGIN_PAR_FORWARD_0
template <class Base>
inline void par_forward_0(
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        num_par     ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
// END_PAR_FORWARD_0
{  //
   //
   // check assumptions
   CPPAD_ASSERT_NARG_NRES(ParOp, 1, 1);
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
   CPPAD_ASSERT_UNKNOWN( 0 < cap_order );

   Base* z = taylor + i_z * cap_order;

   z[0]  = parameter[ arg[0] ];
}
/*
------------------------------------------------------------------------------
{xrst_begin var_par_forward_any dev}

Any Order Forward Create a Variable From a Parameter
####################################################

n_res
*****
see
:ref:`var_par_op@n_res` .

Prototype
*********
{xrst_literal
   // BEGIN_PAR_FORWARD_ANY
   // END_PAR_FORWARD_ANY
}

i_z, arg
********
see
ref:`var_par_op@i_z` ,
ref:`var_par_op@arg`

num_par
*******
is the total number of values in the *parameter* vector.

parameter
*********
maps parameter indices to parameter values .

{xrst_template ;
   include/cppad/local/var_op/template/forward_op.xrst
   headers: cap_order, order_low, order_up, taylor
}

{xrst_end var_par_forward_any}
*/
// BEGIN_PAR_FORWARD_ANY
template <class Base>
inline void par_forward_any(
   size_t        order_low   ,
   size_t        order_up    ,
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        num_par     ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
// END_PAR_FORWARD_ANY
{  //
   //
   // check assumptions
   CPPAD_ASSERT_NARG_NRES(ParOp, 1, 1);
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
   CPPAD_ASSERT_UNKNOWN( 0 < cap_order );
   CPPAD_ASSERT_UNKNOWN( order_low <= order_up );

   Base* z = taylor + i_z * cap_order;
   if( order_low == 0 )
   {  z[0]  = parameter[ arg[0] ];
      ++order_low;
   }
   for(size_t k = order_low; k <= order_up; ++k)
      z[k] = Base(0.0);
}
/*
------------------------------------------------------------------------------
{xrst_begin var_par_forward_dir dev}

Multiple Direction Forward Create a Variable From a Parameter
#############################################################

n_res
*****
see
:ref:`var_par_op@n_res` .

Prototype
*********
{xrst_literal
   // BEGIN_PAR_FORWARD_DIR
   // END_PAR_FORWARD_DIR
}

i_z, arg
********
see
ref:`var_par_op@i_z` ,
ref:`var_par_op@arg`

num_par
*******
is the total number of values in the *parameter* vector.

parameter
*********
maps parameter indices to parameter values .

{xrst_template ;
   include/cppad/local/var_op/template/forward_dir.xrst
   headers: n_dir, cap_order, order_up, taylor
}

{xrst_end var_par_forward_dir}
*/
// BEGIN_PAR_FORWARD_DIR
template <class Base>
inline void par_forward_dir(
   size_t        order_up    ,
   size_t        n_dir       ,
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        num_par     ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
// END_PAR_FORWARD_DIR
{  //
   //
   // check assumptions
   CPPAD_ASSERT_NARG_NRES(ParOp, 1, 1);
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
   CPPAD_ASSERT_UNKNOWN( 0 < cap_order );
   CPPAD_ASSERT_UNKNOWN( 0 < order_up );

   // per_variable
   size_t per_variable = (cap_order - 1) * n_dir + 1;
   //
   // z
   Base* z  = taylor + i_z * per_variable;
   //
   // m
   size_t m = (order_up - 1) * n_dir + 1;
   //
   // taylor
   for(size_t ell = 0; ell < n_dir; ++ell)
      z[m + ell]   = Base(0.0);
}

} } } // END namespace
# endif
