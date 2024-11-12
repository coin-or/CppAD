# ifndef CPPAD_CORE_ATOMIC_FOUR_CALL_HPP
# define CPPAD_CORE_ATOMIC_FOUR_CALL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_call}

Calling an Atomic Function
##########################

Syntax
******
| *afun* ( *ax* , *ay* )
| *ay* = *afun* ( *call_id* , *ax* , *ay* )

Prototype
*********
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

Purpose
*******
Given *ax* , this call computes the corresponding value of *ay* .
If ``AD`` < *Base* > operations are being recorded,
it enters the computation as an atomic operation in the recording;
see :ref:`Independent@Start Recording` .

Base
****
This is the :ref:`atomic_four_ctor@atomic_four@Base`
in the *afun* constructor.
It is also the *Base* type of the elements of
*ax* and *ay* in the atomic function call.
To be specific, the elements of *ax* and *ay* have type
``AD`` < ``Base`` > .

ADVector
********
The type *ADVector* must be a
:ref:`simple vector class<SimpleVector-name>` with elements of type
``AD`` < *Base* > .

afun
****
is a :ref:`atomic_four_ctor@atomic_user` object
and this *afun* function call is implemented by the
:ref:`atomic_four_ctor@atomic_four` class.

ax
**
This vector is ``const`` and passed by reference and
its size determines *n* .
It specifies vector :math:`x \in \B{R}^n`
at which an ``AD`` < *Base* > version of
:math:`y = g(x)` is to be evaluated.

ay
**
This vector is passed by reference and its size determines *m* .
The input values of its elements
are not specified (must not matter).
Upon return, it is an ``AD`` < *Base* > version of
:math:`y = g(x)`.

call_id
*******
This optional argument has default value zero.
It can be used to specify additional information about this call to
*afun* . For example, it could specify the index in vector of structures
in the *afun* object where the actual information is placed.

for_type
********
The :ref:`for_type<atomic_four_for_type-name>` routine will be called once,
for each call to an atomic function,
before any other callbacks corresponding to the atomic function call.
This enables you to store, during the ``for_type`` routine,
the values in
:ref:`atomic_four_for_type@type_x` and or
:ref:`atomic_four_for_type@type_y` corresponding
to this atomic function call.

Restriction
===========
The value of *call_id* must be less than or equal

   ``std::numeric_limits<`` *cppad_tape_id_type* >:: ``max`` ()

see :ref:`cmake@cppad_tape_id_type` .

{xrst_end atomic_four_call}
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_PROTOTYPE
template <class Base> template <class ADVector>
void atomic_four<Base>::operator()(
   size_t           call_id ,
   const ADVector&  ax      ,
   ADVector&        ay      )
// END_PROTOTYPE
{
   size_t n = ax.size();
   size_t m = ay.size();
# ifndef NDEBUG
   bool ok = true;
   std::string msg = "atomic_four: call " + atomic_name() + " ";
   if( (n == 0) || (m == 0) )
   {  msg += "ax.size() or ay.size() is zero";
      CPPAD_ASSERT_KNOWN(false, msg.c_str() );
   }
# endif
   //
   // type_x, type_y, taylor_x, taylor_y, select_y
   size_t thread = thread_alloc::thread_num();
   allocate_work(thread);
   vector<ad_type_enum>& type_x   = work_[thread]->type_x;
   vector<ad_type_enum>& type_y   = work_[thread]->type_y;
   vector<Base>&         taylor_x = work_[thread]->taylor_x;
   vector<Base>&         taylor_y = work_[thread]->taylor_y;
   vector<bool>&         select_y = work_[thread]->select_y;
   type_x.resize(n);
   taylor_x.resize(n);
   type_y.resize(m);
   taylor_y.resize(m);
   select_y.resize(m);
   //
   // tape_id, tape, taylor_x, type_x
   tape_id_t            tape_id  = 0;
   local::ADTape<Base>* tape     = nullptr;
   for(size_t j = 0; j < n; j++)
   {  taylor_x[j]  = ax[j].value_;
      if( IdenticalZero( ax[j] ) )
         type_x[j] = identical_zero_enum;
      else if( Constant( ax[j] ) )
         type_x[j] = constant_enum;
      else
      {  type_x[j] = ax[j].ad_type_;
         if( tape_id == 0 )
         {  tape    = ax[j].tape_this();
            tape_id = ax[j].tape_id_;
            CPPAD_ASSERT_UNKNOWN( tape != nullptr );
         }
# ifndef NDEBUG
         if( Dynamic( ax[j] ) )
         {    CPPAD_ASSERT_UNKNOWN( type_x[j] == dynamic_enum );
         }
         else
         {  CPPAD_ASSERT_UNKNOWN( Variable( ax[j] ) );
            CPPAD_ASSERT_UNKNOWN( type_x[j] == variable_enum );
         }
         if( tape_id != ax[j].tape_id_ )
         {  msg += atomic_name() +
            ": ax contains non-constant values from different threads.";
            CPPAD_ASSERT_KNOWN(false, msg.c_str());
         }
# endif
      }
   }
   // Use zero order forward mode to compute all the components of y
   for(size_t i = 0; i < m; ++i)
      select_y[i] = true;
   size_t order_low   = 0;
   size_t order_up    = 0;
# ifdef NDEBUG
   for_type(
      call_id, type_x, type_y
   );
   forward(
      call_id, select_y, order_low, order_up, taylor_x, taylor_y
   );
# else
   ok &= for_type(
      call_id, type_x, type_y
   );
   ok &= forward(
      call_id, select_y, order_low, order_up, taylor_x, taylor_y
   );
   if( ! ok )
   {  msg += atomic_name() + ": ok is false for "
         "type or zero order forward mode calculation.";
      CPPAD_ASSERT_KNOWN(false, msg.c_str());
   }
# endif
   bool record_dynamic = false;
   bool record_variable = false;
   //
   // set ay to be vector of constant parameters with correct value
   for(size_t i = 0; i < m; i++)
   {  // pass back values
      ay[i].value_ = taylor_y[i];

      // initialize entire vector as constants
      ay[i].tape_id_ = 0;
      ay[i].taddr_   = 0;

      // we need to record this operation if
      // any of the elemnts of ay are dynamics or variables,
      record_dynamic  |= type_y[i] == dynamic_enum;
      record_variable |= type_y[i] == variable_enum;
   }
# ifndef NDEBUG
   if( (record_dynamic || record_variable) && tape == nullptr )
   {  msg +=
      "all elements of x are constants but y contains a non-constant";
      CPPAD_ASSERT_KNOWN(false, msg.c_str() );
   }
# endif
   if( record_dynamic)
   {  tape->Rec_.put_dyn_atomic(
         tape_id, index_, call_id, type_x, type_y, ax, ay
      );
   }
   // case where result contains a variable
   if( record_variable )
   {  tape->Rec_.put_var_atomic(
         tape_id, index_, call_id, type_x, type_y, ax, ay
      );
   }
# ifndef NDEBUG
   for(size_t i = 0; i < m; ++i) switch( type_y[i] )
   {  //
      case identical_zero_enum:
      case constant_enum:
      CPPAD_ASSERT_UNKNOWN( Constant( ay[i] ) );
      break;
      //
      case dynamic_enum:
      CPPAD_ASSERT_UNKNOWN( Dynamic( ay[i] ) );
      break;
      //
      case variable_enum:
      CPPAD_ASSERT_UNKNOWN( Variable( ay[i] ) );
      break;
      //
      default:
      CPPAD_ASSERT_KNOWN( false,
         "atomic_four: for_type: type_y[i]: is not a valid type"
      );
      break;
   }
# endif
   return;
}
template <class Base> template <class ADVector>
void atomic_four<Base>::operator()(
   const ADVector&  ax      ,
   ADVector&        ay      )
{  size_t call_id = 0;
   (*this)(call_id, ax, ay);
}

} // END_CPPAD_NAMESPACE
# endif
