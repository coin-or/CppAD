# ifndef CPPAD_LOCAL_VAR_OP_PRI_OP_HPP
# define CPPAD_LOCAL_VAR_OP_PRI_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

namespace CppAD { namespace local { namespace var_op {
/*
{xrst_begin_parent var_pri_op dev}
{xrst_spell
   pos
}

Print A Variable or Parameter Operator
######################################

PriOp
*****
is the op code for this operator.

User Syntax
***********
| ``PrintFor`` ( *pos* , *before* , *value* , *after* )

pos
***
This value is expected to be positive and
*value* is printed if *pos* is not positive.

before
******
is the text printed before the value.

value
*****
is the value that is printed.

after
*****
is the text printed after the value.

Base
****
base type for the operator; i.e., this operation was recorded
using AD< Base > and computations by this routine are done using type Base .

arg
***

arg[0]
******
The first two bits of this value are used a flags; see below.

arg[1]
******
If arg[0] & 1 is zero (is one), arg[1] is the parameter (variable) index
corresponding to *pos* .

arg[2]
******
is the text index corresponding to *before* .

arg[3]
******
If arg[0] & 2 is zero (is one), arg[3] is the parameter (variable) index
corresponding to *value* .

arg[4]
******
is the text index corresponding to *after* .

{xrst_end var_pri_op}
------------------------------------------------------------------------------
{xrst_begin var_pri_forward_0 dev}

Zero Order Forward Print A Variable or Parameter
################################################

Prototype
*********
{xrst_literal
   // BEGIN_PRI_FORWARD_0
   // END_PRI_FORWARD_0
}

s_out
*****
the results are printed on this output stream.

num_text
********
is the total number of text characters on the tape
(only used for error checking).

text
****
is the tape text vector.
The value *before* ( *after* ) is stored in this vector
as a null terminated string start at index arg[2] ( arg[4] ).

num_par
*******
is the total number of values in the parameter vector .

parameter
*********
maps parameter indices to parameter values.

cap_order
*********
is the maximum number of orders that can fit in taylor .

taylor
******
The zero order Taylor coefficient corresponding to the variable with
index j* is taylor[ j * cap_order + 0] .

{xrst_end var_pri_forward_0}
*/
// BEGIN_PRI_FORWARD_0
template <class Base>
inline void pri_forward_0(
   std::ostream& s_out       ,
   const addr_t* arg         ,
   size_t        num_text    ,
   const char*   text        ,
   size_t        num_par     ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   const Base*   taylor      )
// END_PRI_FORWARD_0
{  Base pos, value;
   //
   const char* before;
   const char* after;
   CPPAD_ASSERT_NARG_NRES(PriOp, 5, 0);

   // pos
   if( arg[0] & 1 )
   {  pos = taylor[ size_t(arg[1]) * cap_order + 0 ];
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < num_par );
      pos = parameter[ arg[1] ];
   }

   // before
   CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < num_text );
   before = text + arg[2];

   // value
   if( arg[0] & 2 )
   {  value = taylor[ size_t(arg[3]) * cap_order + 0 ];
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[3]) < num_par );
      value = parameter[ arg[3] ];
   }

   // after
   CPPAD_ASSERT_UNKNOWN( size_t(arg[4]) < num_text );
   after = text + arg[4];

   if( ! GreaterThanZero( pos ) )
      s_out << before << value << after;
}

} } } // END namespace
# endif
