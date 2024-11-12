# ifndef CPPAD_LOCAL_RECORD_PUT_VAR_ATOMIC_HPP
# define CPPAD_LOCAL_RECORD_PUT_VAR_ATOMIC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/record/recorder.hpp>

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*
{xrst_begin recorder_put_var_atomic dev}
{xrst_spell
   taddr
}

Put a Variable Atomic Call Operator in Recording
################################################

Syntax
******
| *rec* . ``put_var_atomic`` (
| |tab| *tape_id* , *atomic_index* , *call_id* , *type_x* , *type_y* , *ax* , *ay*
| )

Prototype
*********
{xrst_literal
   // BEGIN_PUT_VAR_ATOMIC
   // END_PROTOTYPE
}

tape_id
*******
identifies the tape that this recording corresponds to.
This is zero if and only if there is no tape for this recording; i.e.
``AD`` < *Base* >. ``tape_ptr`` () is null.

atomic_index
************
is the :ref:`atomic_index-name` for this atomic function.

call_id
*******
Is the :ref:`atomic_four_call@call_id` for this
atomic function call.

type_x
******
is the :ref:`ad_type_enum-name` for each of the atomic function arguments.
This is one of the rare cases where constants can have type
``identical_zero_enum`` .

type_y
******
is the ``ad_type_enum`` for each of the atomic function results.
This is one of the rare cases where constants can have type
``identical_zero_enum`` .

ax
**
is the atomic function argument vector for this call.

value\_
=======
The value *ax* [ *j* ]. ``value_`` is the proper value for all arguments.

taddr\_
=======
The value *ax* [ *j* ]. ``taddr_`` is the proper address
for dynamic parameters and variables and  does not matter for constants.

ay
**
is the atomic function result vector for this call.

Input
=====
On input, *ay* [ *i* ] has all the correct values for
parameters and does not matter for variables.

Output
======
Upon return, if the *i*-th result is a variable,

| |tab| *ay* [ *i* ]. ``ad_type_`` = ``dynamic_enum``
| |tab| *ay* [ *i* ]. ``tape_id_`` = *tape_id*
| |tab| *ay* [ *i* ]. ``taddr_`` = *v_index*

where *v_index* is the index of this variable
in the arrays containing all the variables.

{xrst_end recorder_put_var_atomic}
*/
// BEGIN_PUT_VAR_ATOMIC
template <class Base> template <class VectorAD>
void recorder<Base>::put_var_atomic(
   tape_id_t                   tape_id      ,
   size_t                      atomic_index ,
   size_t                      call_id      ,
   const vector<ad_type_enum>& type_x       ,
   const vector<ad_type_enum>& type_y       ,
   const VectorAD&             ax           ,
   VectorAD&                   ay           )
// END_PROTOTYPE
{  CPPAD_ASSERT_KNOWN(
      size_t( std::numeric_limits<addr_t>::max() ) >=
         std::max( std::max(atomic_index, ax.size() ), ay.size() ),
      "atomic_three: cppad_tape_addr_type maximum not large enough"
   );
   CPPAD_ASSERT_UNKNOWN(
      (tape_id == 0) == (AD<Base>::tape_ptr() == nullptr)
   );
   // Operator that marks beginning of this atomic operation
   CPPAD_ASSERT_NARG_NRES(local::AFunOp, 4, 0 );
   size_t n = ax.size();
   size_t m = ay.size();
   PutArg(addr_t(atomic_index), addr_t(call_id), addr_t(n), addr_t(m));
   PutOp(local::AFunOp);

   // Now put n operators, one for each element of argument vector
   CPPAD_ASSERT_NARG_NRES(local::FunavOp, 1, 0 );
   CPPAD_ASSERT_NARG_NRES(local::FunapOp, 1, 0 );
   for(size_t j = 0; j < n; j++)
   {  if( type_x[j] == variable_enum )
      {  // information for an argument that is a variable
         PutArg(ax[j].taddr_);
         PutOp(local::FunavOp);
      }
      else
      {  // information for an argument that is parameter
         addr_t par = ax[j].taddr_;
         if( type_x[j] <= constant_enum )
            par = put_con_par(ax[j].value_);
         PutArg(par);
         PutOp(local::FunapOp);
      }
   }

   // Now put m operators, one for each element of result vector
   CPPAD_ASSERT_NARG_NRES(local::FunrvOp, 0, 1);
   CPPAD_ASSERT_NARG_NRES(local::FunrpOp, 1, 0);
   for(size_t i = 0; i < m; i++)
   {  if( type_y[i] == variable_enum )
      {  ay[i].taddr_    = PutOp(local::FunrvOp);
         ay[i].tape_id_  = tape_id;
         ay[i].ad_type_  = variable_enum;
      }
      else
      {  addr_t par = ay[i].taddr_;
         if( type_y[i] <= constant_enum )
            par = put_con_par( ay[i].value_ );
         PutArg(par);
         PutOp(local::FunrpOp);
      }
   }

   // Put a duplicate AFunOp at end of AFunOp sequence
   PutArg(addr_t(atomic_index), addr_t(call_id), addr_t(n), addr_t(m));
   PutOp(local::AFunOp);
}

} } // END_CPPAD_LOCAL_NAMESPACE
# endif
