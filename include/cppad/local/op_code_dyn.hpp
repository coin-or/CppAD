# ifndef CPPAD_LOCAL_OP_CODE_DYN_HPP
# define CPPAD_LOCAL_OP_CODE_DYN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*!
{xrst_begin op_code_dyn dev}
{xrst_spell
   zmul
}

Dynamic Parameter Op Codes
##########################

Namespace
*********
The ``op_code_dyn`` enum type is in the ``CppAD::local`` namespace.

AD Type
*******
All the operators below have no variable arguments,
at least one dynamic parameter argument,
and at most one constant argument; see
:ref:`ad_type_enum-name` .
For example, all the unary operators have one dynamic parameter argument
and one dynamic parameter result.

Unary
*****
The number of arguments for a unary operator is one
and it is a parameter index.
All the unary operators have one result that is a dynamic parameter.

Binary
******
The number of arguments for a binary operator is two
and they are parameter indices.
All the binary operators have one result that is a dynamic parameter.
For binary operators the first argument is the left operand
and the second is the right operand.

zmul_dyn
========
This binary operator has a non-standard name; see :ref:`azmul-name` for
its definition.

ind_dyn
*******
This is an independent dynamic parameter operator.
It has no arguments and one result which is the value of the corresponding
independent dynamic parameter in the call to :ref:`new_dynamic-name` .

{xrst_comment ------------------------------------------------------------ }
atom_dyn
********
This operator is a call to an atomic function.
The number of arguments to this operator is
*arg* [4+ *n* + *m* ] ; see below.

arg[0]
======
This is the index that identifies this atomic function; see
``local/atomic_index.hpp`` .

arg[1]
======
This is the :ref:`atomic_four_call@call_id` for this
function call.

arg[2]
======
This is the number of arguments to this atomic function.
We use the notation *n* = *arg* [1] below.

arg[3]
======
This is the number of results for this atomic function.
We use the notation *m* = *arg* [2] below.

arg[4]
======
This is the number of result values that are dynamic parameters
for this function call.

arg[5+j]
========
For *j* = 0 , ... , *n* ``-1`` ,
this is the parameter index for the *j*-th argument to this atomic
function call.

arg[5+n+i]
==========
For *i* = 0 , ... , *m* ``-1`` ,
this is the parameter index for the *i*-th result to this atomic
function call.

arg[5+n+m]
==========
This is the number of arguments to this operator; i.e.,
6+ *n* + *m* .

result_dyn
**********
This is a place holder for a result of an atomic function call
that is a dynamic parameter.
It has no arguments, no results, and is only there so that the
number of dynamic parameters and the number of dynamic operators are equal.

{xrst_comment ------------------------------------------------------------ }
cond_exp_dyn
************
This is a conditional expression operator and has five arguments
and one result.

arg[0]
======
This is the
:ref:`base_cond_exp@CompareOp` value for this operator.

arg[1]
======
This is the parameter index for the left operand to the comparison.

arg[2]
======
This is the parameter index for the right operand to the comparison.

arg[3]
======
This is the index of the parameter equal to the operator result if
the comparison result is true.

arg[4]
======
This is the index of the parameter equal to the operator result if
the comparison result is false.

{xrst_comment ------------------------------------------------------------ }
dis_dyn
*******
This is a call to a discrete function.
The discrete function has one argument and one result.
This operator has two arguments and one result.
It is not a binary operator because the first argument
is not the index of a parameter.

arg[0]
======
Is the discrete function index which depends on the *Base*
type used when this function was recorded.

arg[1]
======
Is the parameter index for the argument to the function.

{xrst_comment ------------------------------------------------------------ }
Source
******
{xrst_literal
   // BEGIN_OP_CODE_DYN
   // END_OP_CODE_DYN
}

{xrst_end op_code_dyn}
*/

// BEGIN_SORT_THIS_LINE_PLUS_3
// BEGIN_OP_CODE_DYN
enum op_code_dyn {
   abs_dyn,       // unary
   acos_dyn,      // unary
   acosh_dyn,     // unary
   add_dyn,       // binary
   asin_dyn,      // unary
   asinh_dyn,     // unary
   atan_dyn,      // unary
   atanh_dyn,     // unary
   atom_dyn,      // ? arguments: atomic function call
   cond_exp_dyn,  // 5 arguments: conditional expression
   cos_dyn,       // unary
   cosh_dyn,      // unary
   dis_dyn,       // 2 arguments: discrete function
   div_dyn,       // binary
   erf_dyn,       // unary
   erfc_dyn,      // unary
   exp_dyn,       // unary
   expm1_dyn,     // unary
   fabs_dyn,      // unary
   ind_dyn,       // 0 arguments: independent parameter
   log1p_dyn,     // unary
   log_dyn,       // unary
   mul_dyn,       // binary
   neg_dyn,       // unary
   pow_dyn,       // binary
   result_dyn,    // 0 arguments: atomic function result
   sign_dyn,      // unary
   sin_dyn,       // unary
   sinh_dyn,      // unary
   sqrt_dyn,      // unary
   sub_dyn,       // binary
   tan_dyn,       // unary
   tanh_dyn,      // unary
   zmul_dyn,      // binary
   number_dyn     // number of operator codes and invalid operator value
};
// END_OP_CODE_DYN
// END_SORT_THIS_LINE_MINUS_4

/*
{xrst_begin num_arg_dyn dev}

Number of Arguments to a Dynamic Parameter Operator
###################################################

Syntax
******
| *n_arg* = ``local::num_arg_dyn`` ( *op* )

Prototype
*********
{xrst_literal
   // BEGIN_NUM_ARG_DYN_PROTOTYPE
   // END_NUM_ARG_DYN_PROTOTYPE
}

Parallel Mode
*************
This routine has static data so its first call cannot be in Parallel mode.

op
**
is the operator in question.

n_arg
*****
The return value is the number of arguments as commented in the
:ref:`op_code_dyn@Source` for ``enum op_code_dyn`` .
There is one exception: if *op* is ``atom_dyn`` ,
*n_arg* is zero; see :ref:`op_code_dyn@atom_dyn`
for the true number of arguments in this case.

atom_dyn
********
All of the dynamic parameter operators have a fixed number of arguments
except for the :ref:`op_code_dyn@atom_dyn`
operator which calls an atomic functions.
In this special case the return value *n_arg* is zero
which is not correct.

{xrst_end num_arg_dyn}
*/
// BEGIN_NUM_ARG_DYN_PROTOTYPE
inline size_t num_arg_dyn(op_code_dyn op)
// END_NUM_ARG_DYN_PROTOTYPE
{  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;

   // BEGIN_SORT_THIS_LINE_PLUS_2
   static const size_t num_arg_table[] = {
      /* abs_dyn */      1,
      /* acos_dyn */     1,
      /* acosh_dyn */    1,
      /* add_dyn */      2,
      /* asin_dyn */     1,
      /* asinh_dyn */    1,
      /* atan_dyn */     1,
      /* atanh_dyn */    1,
      /* atom_dyn */     0,
      /* cond_exp_dyn */ 5,
      /* cos_dyn */      1,
      /* cosh_dyn */     1,
      /* dis_dyn */      2,
      /* div_dyn */      2,
      /* erf_dyn */      1,
      /* erfc_dyn */     1,
      /* exp_dyn */      1,
      /* expm1_dyn */    1,
      /* fabs_dyn */     1,
      /* ind_dyn */      0,
      /* log1p_dyn */    1,
      /* log_dyn */      1,
      /* mul_dyn */      2,
      /* neg_dyn */      1,
      /* pow_dyn */      2,
      /* result_dyn */   0,
      /* sign_dyn */     1,
      /* sin_dyn */      1,
      /* sinh_dyn */     1,
      /* sqrt_dyn */     1,
      /* sub_dyn */      2,
      /* tan_dyn */      1,
      /* tanh_dyn */     1,
      /* zmul_dyn */     2,
      0  // number_dyn (not used)
   };
   // END_SORT_THIS_LINE_MINUS_3
   //
   static bool first = true;
   if( first )
   {  CPPAD_ASSERT_UNKNOWN(
      size_t(number_dyn)+1 == sizeof(num_arg_table)/sizeof(num_arg_table[0])
      );
      first = false;
   }
   return num_arg_table[op];
}

/*
{xrst_begin op_name_dyn dev}

Number of Arguments to a Dynamic Parameter Operator
###################################################

Syntax
******

   *name* = ``local::op_name_dyn`` ( *op* )

Prototype
*********
{xrst_literal
   // BEGIN_OP_NAME_DYN_PROTOTYPE
   // END_OP_NAME_DYN_PROTOTYPE
}

Parallel Mode
*************
This routine has static data so its first call cannot be in Parallel mode.

op
**
is the operator in question.

name
****
The return value *name* is the same as the operator enum symbol
(see :ref:`op_code_dyn@Source` for ``enum op_code_dyn`` )
without the ``_dyn`` at the end. For example,
the name corresponding to the
:ref:`op_code_dyn@cond_exp_dyn` operator is ``cond_exp`` .

{xrst_end op_name_dyn}
*/
// BEGIN_OP_NAME_DYN_PROTOTYPE
inline const char* op_name_dyn(op_code_dyn op)
// END_OP_NAME_DYN_PROTOTYPE
{  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;

   // BEGIN_SORT_THIS_LINE_PLUS_2
   static const char* op_name_table[] = {
      /* abs_dyn */      "abs",
      /* acos_dyn */     "acos",
      /* acosh_dyn */    "acosh",
      /* add_dyn */      "add",
      /* asin_dyn */     "asin",
      /* asinh_dyn */    "asinh",
      /* atan_dyn */     "atan",
      /* atanh_dyn */    "atanh",
      /* atom_dyn */     "call",
      /* cond_exp_dyn */ "cond_exp",
      /* cos_dyn */      "cos",
      /* cosh_dyn */     "cosh",
      /* dis_dyn */      "dis",
      /* div_dyn */      "div",
      /* erf_dyn */      "erf",
      /* erfc_dyn */     "erfc",
      /* exp_dyn */      "exp",
      /* expm1_dyn */    "expm1",
      /* fabs_dyn */     "fabs",
      /* ind_dyn */      "ind",
      /* log1p_dyn */    "log1p",
      /* log_dyn */      "log",
      /* mul_dyn */      "mul",
      /* neg_dyn */      "neg",
      /* pow_dyn */      "pow",
      /* result_dyn */   "result",
      /* sign_dyn */     "sign",
      /* sin_dyn */      "sin",
      /* sinh_dyn */     "sinh",
      /* sqrt_dyn */     "sqrt",
      /* sub_dyn */      "sub",
      /* tan_dyn */      "tan",
      /* tanh_dyn */     "tanh",
      /* zmul_dyn */     "zmul",
      /* number_dyn */   "number"
   };
   // END_SORT_THIS_LINE_MINUS_3
   static bool first = true;
   if( first )
   {  CPPAD_ASSERT_UNKNOWN(
      size_t(number_dyn)+1 == sizeof(op_name_table)/sizeof(op_name_table[0])
      );
      first = false;
   }
   return op_name_table[op];
}

/*
{xrst_begin num_non_par_arg_dyn dev}

Number Non-Parameter Arguments to a Dynamic Parameters Operator
###############################################################

Syntax
******

   *num* = ``local::num_non_par_arg_dyn`` ( *op* )

Prototype
*********
{xrst_literal
   // BEGIN_NUM_NON_PAR_ARG_DYN
   // END_NUM_NON_PAR_ARG_DYN
}

op
**
is the operator in question.

num
***
The return value *num* is the number of arguments,
for this operator *op* , that are not parameters indices.
All of the non-parameter arguments come first
so *num* is also the offset for the
first argument that is a parameter index.

atom_dyn
********
The :ref:`op_code_dyn@atom_dyn` case is special,
*num* is zero for this case but it is not as documented above; see
:ref:`op_code_dyn@atom_dyn` .

{xrst_end num_non_par_arg_dyn}
*/
// BEGIN_NUM_NON_PAR_ARG_DYN
inline size_t num_non_par_arg_dyn(op_code_dyn op)
// END_NUM_NON_PAR_ARG_DYN
{
   size_t num;
   switch(op)
   {  case atom_dyn:
      num = 5;
      break;

      case cond_exp_dyn:
      case dis_dyn:
      num = 1;
      break;

      default:
      num = 0;
   }
   //
   return num;
}

} } // END_CPPAD_LOCAL_NAMESPACE

# endif
