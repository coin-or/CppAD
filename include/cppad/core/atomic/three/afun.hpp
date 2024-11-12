# ifndef CPPAD_CORE_ATOMIC_THREE_AFUN_HPP
# define CPPAD_CORE_ATOMIC_THREE_AFUN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_three_afun}

Using AD Version of an Atomic Function
######################################

Syntax
******
| *afun* ( *ax* , *ay* )

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
This is the *Base* type of the elements of *ax* and *ay*
in the call to the *afun* atomic operation.
To be specific, the elements of *ax* and *ay* have type
``AD`` < ``Base`` > .

ADVector
********
The type *ADVector* must be a
:ref:`simple vector class<SimpleVector-name>` with elements of type
``AD`` < *Base* > .

afun
****
is a :ref:`atomic_three_ctor@atomic_user` object
and this *afun* function call is implemented by the
:ref:`atomic_three_ctor@atomic_three` class.

ax
**
This argument has prototype

   ``const`` *ADVector* & *ax*

and size must be equal to *n* .
It specifies vector :math:`x \in \B{R}^n`
at which an ``AD`` < *Base* > version of
:math:`y = g(x)` is to be evaluated; see
:ref:`atomic_three_ctor@atomic_three@Base` .

ay
**
This argument has prototype

   *ADVector* & *ay*

and size must be equal to *m* .
The input values of its elements
are not specified (must not matter).
Upon return, it is an ``AD`` < *Base* > version of
:math:`y = g(x)`.

{xrst_end atomic_three_afun}
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/three_afun.hpp
Implement user call to an atomic_three function.
*/

/*!
Implement the user call to afun(ax, ay)

\tparam ADVector
A simple vector class with elements of type AD<Base>.

\param ax
is the argument vector for this call,
ax.size() determines the number of arguments.

\param ay
is the result vector for this call,
ay.size() determines the number of results.
*/
// BEGIN_PROTOTYPE
template <class Base>
template <class ADVector>
void atomic_three<Base>::operator()(
   const ADVector&  ax     ,
   ADVector&        ay     )
// END_PROTOTYPE
{

   size_t n = ax.size();
   size_t m = ay.size();
# ifndef NDEBUG
   bool ok = true;
   std::string msg = "atomic_three: " + atomic_name() + ".eval: ";
   if( (n == 0) || (m == 0) )
   {  msg += "ax.size() or ay.size() is zero";
      CPPAD_ASSERT_KNOWN(false, msg.c_str() );
   }
# endif
   size_t thread = thread_alloc::thread_num();
   allocate_work(thread);
   vector<Base>& taylor_x        = work_[thread]->taylor_x;
   vector<Base>& taylor_y        = work_[thread]->taylor_y;
   vector<ad_type_enum>& type_x  = work_[thread]->type_x;
   vector<ad_type_enum>& type_y  = work_[thread]->type_y;
   //
   type_x.resize(n);
   taylor_x.resize(n);
   //
   type_y.resize(m);
   taylor_y.resize(m);
   //
   // Determine tape corresponding to variables in ax
   tape_id_t            tape_id  = 0;
   local::ADTape<Base>* tape     = nullptr;
   for(size_t j = 0; j < n; j++)
   {  taylor_x[j]  = ax[j].value_;
      if( Constant( ax[j] ) )
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
   size_t need_y    = size_t(variable_enum) + 1;
   size_t order_low = 0;
   size_t order_up  = 0;
   CPPAD_ASSERT_UNKNOWN( need_y > size_t(variable_enum) );
# ifdef NDEBUG
   forward(taylor_x, type_x, need_y, order_low, order_up, taylor_x, taylor_y);
   for(size_t j = 0; j < n; ++j)
      if( type_x[j] == variable_enum )
         taylor_x[j] = CppAD::numeric_limits<Base>::quiet_NaN();
   for_type(taylor_x, type_x, type_y);
# else
   ok &= forward(
      taylor_x, type_x, need_y, order_low, order_up, taylor_x, taylor_y
   );
   for(size_t j = 0; j < n; ++j)
      if( type_x[j] == variable_enum )
         taylor_x[j] = CppAD::numeric_limits<Base>::quiet_NaN();
   ok &= for_type(taylor_x, type_x, type_y);
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
   {  size_t call_id = 0;
      tape->Rec_.put_dyn_atomic(
         tape_id, index_, call_id, type_x, type_y, ax, ay
      );
   }
   // case where result contains a variable
   if( record_variable )
   {  size_t call_id = 0; // atomic_three does not user call_id
      tape->Rec_.put_var_atomic(
         tape_id, index_, call_id, type_x, type_y, ax, ay);
   }
# ifndef NDEBUG
   for(size_t i = 0; i < m; ++i) switch( type_y[i] )
   {  //
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
         "atomic_three: for_type: type_y[i]: is not a valid type"
      );
      break;
   }
# endif
   return;
}

} // END_CPPAD_NAMESPACE
# endif
