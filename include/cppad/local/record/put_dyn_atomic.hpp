# ifndef CPPAD_LOCAL_RECORD_PUT_DYN_ATOMIC_HPP
# define CPPAD_LOCAL_RECORD_PUT_DYN_ATOMIC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/record/recorder.hpp>

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*
{xrst_begin recorder_put_dyn_atomic dev}
{xrst_spell
   taddr
}

Put a Dynamic Parameter Atomic Call Operator in Recording
#########################################################

Syntax
******
| *rec* . ``put_dyn_atomic`` (
| |tab| *tape_id* , *atomic_index* , *call_id* , *type_x* , *type_y* , *ax* , *ay*
| )

Prototype
*********
{xrst_literal
   // BEGIN_PUT_DYN_ATOMIC
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
is the :ref:`atomic_four_call@call_id` for this atomic function.

type_x
******
is the :ref:`ad_type_enum-name` for each of the atomic function arguments.

type_y
******
is the ``ad_type_enum`` for each of the atomic function results.

ax
**
is the atomic function argument vector for this call.

value\_
=======
The value *ax* [ *j* ]. ``value_`` is the proper value for
parameters and does not matter for variables.

taddr\_
=======
The value *ax* [ *j* ]. ``taddr_`` is the proper address for
dynamic parameters and does not matter for constants or variables.

ay
**
is the atomic function result vector for this call.

Input
=====
On input, *ay* [ *j* ]. ``value_`` has the proper value for parameters
(result for the atomic function).

Output
======
Upon return, if the *i*-th result is a dynamic parameter,

| |tab| *ay* [ *i* ]. ``ad_type_`` = ``dynamic_enum``
| |tab| *ay* [ *i* ]. ``tape_id_`` = *tape_id*
| |tab| *ay* [ *i* ]. ``taddr_`` = *p_index*

where *p_index* is the index of this dynamic parameter
in the vector of all parameters.

{xrst_end recorder_put_dyn_atomic}
*/

// BEGIN_PUT_DYN_ATOMIC
template <class Base> template <class VectorAD>
void recorder<Base>::put_dyn_atomic(
   tape_id_t                   tape_id      ,
   size_t                      atomic_index ,
   size_t                      call_id      ,
   const vector<ad_type_enum>& type_x       ,
   const vector<ad_type_enum>& type_y       ,
   const VectorAD&             ax           ,
   VectorAD&                   ay           )
// END_PROTOTYPE
{  CPPAD_ASSERT_UNKNOWN(
      (tape_id == 0) == (AD<Base>::tape_ptr() == nullptr)
   );
   CPPAD_ASSERT_UNKNOWN( ax.size() == type_x.size() );
   CPPAD_ASSERT_UNKNOWN( ay.size() == type_y.size() );
   size_t n       = ax.size();
   size_t m       = ay.size();
   size_t num_dyn = 0;
   for(size_t i = 0; i < m; ++i)
      if( type_y[i] == dynamic_enum )
         ++num_dyn;
   CPPAD_ASSERT_UNKNOWN( num_dyn > 0 );
   //
   dyn_par_arg_.push_back( addr_t(atomic_index )); // arg[0] = atomic_index
   dyn_par_arg_.push_back( addr_t(call_id ));      // arg[1] = call_id
   dyn_par_arg_.push_back( addr_t( n ) );          // arg[2] = n
   dyn_par_arg_.push_back( addr_t( m ) );          // arg[3] = m
   dyn_par_arg_.push_back( addr_t( num_dyn ) );    // arg[4] = num_dyn
   // arg[5 + j] for j = 0, ... , n-1
   for(size_t j = 0; j < n; ++j)
   {  addr_t arg = 0;
      switch( type_x[j] )
      {  case identical_zero_enum:
         case constant_enum:
         arg = put_con_par( ax[j].value_ );
         break;

         case dynamic_enum:
         arg = ax[j].taddr_;
         break;

         case variable_enum:
         arg = 0; // phantom parameter index
         CPPAD_ASSERT_UNKNOWN( CppAD::isnan( all_par_vec_[arg] ) )
         break;

         default:
         arg = 0;
         CPPAD_ASSERT_UNKNOWN( false );
      }
      dyn_par_arg_.push_back( arg );              // arg[5 + j]
   }
   // arg[5 + n + i] for i = 0, ... , m-1
   bool first_dynamic_result = true;
   for(size_t i = 0; i < m; ++i)
   {  addr_t arg;
      switch( type_y[i] )
      {  case identical_zero_enum:
         case constant_enum:
         arg = 0; // phantom parameter index
         break;

         case dynamic_enum:
         // one operator for each dynamic parameter result
         // so number of operators is equal number of dynamic parameters
         if( first_dynamic_result )
            arg = put_dyn_par(ay[i].value_, atom_dyn );    // atom_dyn
         else
            arg = put_dyn_par(ay[i].value_, result_dyn );  // result_dyn
         ay[i].ad_type_ = dynamic_enum;
         ay[i].taddr_   = arg;
         ay[i].tape_id_ = tape_id;
         first_dynamic_result = false;
         break;

         case variable_enum:
         arg = 0; // phantom parameter (has value nan)
         break;

         default:
         arg = 0;
         CPPAD_ASSERT_UNKNOWN( false );
      }
      dyn_par_arg_.push_back( arg );              // arg[5 + n + i]
   }
   dyn_par_arg_.push_back( addr_t(6 + n + m) );    // arg[5 + n + m]
}

} } // END_CPPAD_LOCAL_NAMESPACE
# endif
