# ifndef CPPAD_CORE_DEPENDENT_HPP
# define CPPAD_CORE_DEPENDENT_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin Dependent}

Stop Recording and Store Operation Sequence
###########################################

Syntax
******
| *f* . ``Dependent`` ( *x* , *y* )

Stop Recording
**************
The call stops the recording and the AD of *Base*
:ref:`operation sequence<glossary@Operation@Sequence>`
that started with the call

   ``Independent`` ( *x* )

Store Operation Sequence
************************
This call also stores the operation sequence in *f* .
The operation sequence defines an
:ref:`glossary@AD Function`

.. math::

   F : \B{R}^n \rightarrow \B{R}^m

where :math:`B` is the space corresponding to objects of type *Base* .
The value :math:`n` is the dimension of the
:ref:`fun_property@Domain` space for the operation sequence.
The value :math:`m` is the dimension of the
:ref:`fun_property@Range` space for the operation sequence
(which is determined by the size of *y* ).

f
*
The object *f* has prototype

   ``ADFun`` < *Base* > *f*

The AD of *Base* operation sequence is stored in *f* ; i.e.,
it becomes the operation sequence corresponding to *f* .
If a previous operation sequence was stored in *f* ,
it is deleted.

x
*
The argument *x*
must be the vector argument in a previous call to
:ref:`Independent-name` .
Neither its size, or any of its values, are allowed to change
between calling

   ``Independent`` ( *x* )

and

   *f* . ``Dependent`` ( *x* , *y* )

.

y
*
The vector *y* has prototype

   ``const`` *ADvector* & *y*

(see :ref:`ADvector<fun_construct-name>` below).
The length of *y* must be greater than zero
and is the dimension of the range space for *f* .

ADvector
********
The type *ADvector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
``AD`` < *Base* > .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

Taping
******
The tape,
that was created when ``Independent`` ( *x* ) was called,
will stop recording.
The AD operation sequence will be transferred from
the tape to the object *f* and the tape will then be deleted.

Forward
*******
No :ref:`Forward-name` calculation is preformed during this operation.
Thus, directly after this operation,

   *f* . ``size_order`` ()

is zero (see :ref:`size_order-name` ).

Parallel Mode
*************
The call to ``Independent`` ,
and the corresponding call to

   ``ADFun`` < *Base* > *f* ( *x* , *y* )

or

   *f* . ``Dependent`` ( *x* , *y* )

or :ref:`abort_recording-name` ,
must be preformed by the same thread; i.e.,
:ref:`thread_alloc::thread_num<ta_thread_num-name>` must be the same.

Example
*******
The file
:ref:`fun_check.cpp-name`
contains an example and test of this operation.

{xrst_end Dependent}
----------------------------------------------------------------------------
*/


// BEGIN CppAD namespace
namespace CppAD {

/*!
\file dependent.hpp
Different versions of Dependent function.
*/

/*!
Determine the tape corresponding to this exeuction thread and then use
<code>Dependent(tape, y)</code> to store this tapes recording in a function.

\param y [in]
The dependent variable vector for the corresponding function.
*/
template <class Base, class RecBase>
template <class ADvector>
void ADFun<Base,RecBase>::Dependent(const ADvector &y)
{  local::ADTape<Base>* tape = AD<Base>::tape_ptr();
   CPPAD_ASSERT_KNOWN(
      tape != nullptr,
      "Can't store current operation sequence in this ADFun object"
      "\nbecause there is no active tape (for this thread)."
   );

   // code above just determines the tape and checks for errors
   Dependent(tape, y);
}


/*!
Determine the tape corresponding to this exeuction thread and then use
<code>Dependent(tape, y)</code> to store this tapes recording in a function.

\param x [in]
The independent variable vector for this tape. This information is
also stored in the tape so a check is done to make sure it is correct
(if NDEBUG is not defined).

\param y [in]
The dependent variable vector for the corresponding function.
*/
template <class Base, class RecBase>
template <class ADvector>
void ADFun<Base,RecBase>::Dependent(const ADvector &x, const ADvector &y)
{
   CPPAD_ASSERT_KNOWN(
      x.size() > 0,
      "Dependent: independent variable vector has size zero."
   );
   CPPAD_ASSERT_KNOWN(
      Variable(x[0]),
      "Dependent: independent variable vector has been changed."
   );
   local::ADTape<Base> *tape = AD<Base>::tape_ptr(x[0].tape_id_);
   CPPAD_ASSERT_KNOWN(
      tape->size_independent_ == size_t( x.size() ),
      "Dependent: independent variable vector has been changed."
   );
# ifndef NDEBUG
   size_t i, j;
   for(j = 0; j < size_t(x.size()); j++)
   {  CPPAD_ASSERT_KNOWN(
      size_t(x[j].taddr_) == (j+1),
      "ADFun<Base>: independent variable vector has been changed."
      );
      CPPAD_ASSERT_KNOWN(
      x[j].tape_id_ == x[0].tape_id_,
      "ADFun<Base>: independent variable vector has been changed."
      );
   }
   for(i = 0; i < size_t(y.size()); i++)
   {  CPPAD_ASSERT_KNOWN(
      CppAD::Parameter( y[i] ) || (y[i].tape_id_ == x[0].tape_id_) ,
      "ADFun<Base>: dependent vector contains a variable for"
      "\na different tape (thread) than the independent variables."
      );
   }
# endif

   // code above just determines the tape and checks for errors
   Dependent(tape, y);
}

/*!
Replace the floationg point operations sequence for this function object.

\param tape
is a tape that contains the new floating point operation sequence
for this function.
After this operation, all memory allocated for this tape is deleted.

\param y
The dependent variable vector for the function being stored in this object.

\par
All of the private member data in ad_fun.hpp is set to correspond to the
new tape except for check_for_nan_.
*/

template <class Base, class RecBase>
template <class ADvector>
void ADFun<Base,RecBase>::Dependent(local::ADTape<Base> *tape, const ADvector &y)
{
   size_t   m = y.size();
   size_t   n = tape->size_independent_;

   // check ADvector is Simple Vector class with AD<Base> elements
   CheckSimpleVector< AD<Base>, ADvector>();

   CPPAD_ASSERT_KNOWN(
      y.size() > 0,
      "ADFun operation sequence dependent variable size is zero size"
   );
   // ---------------------------------------------------------------------
   // Begin setting ad_fun.hpp private member data
   // ---------------------------------------------------------------------
   // dep_parameter_, dep_taddr_
   CPPAD_ASSERT_UNKNOWN( local::NumRes(local::ParOp) == 1 );
   dep_parameter_.resize(m);
   dep_taddr_.resize(m);
   for(size_t i = 0; i < m; i++)
   {  dep_parameter_[i] = CppAD::Parameter(y[i]);
      addr_t y_taddr;
      if( dep_parameter_[i] )
      {  // make a tape copy of dependent variables that are parameters,
         y_taddr = tape->RecordParOp( y[i] );
      }
      else
         y_taddr = y[i].taddr_;

      CPPAD_ASSERT_UNKNOWN( y_taddr > 0 );
      dep_taddr_[i] = size_t( y_taddr );
   }

   // put an EndOp at the end of the tape
   tape->Rec_.PutOp(local::EndOp);

   // bool values in this object except check_for_nan_
   has_been_optimized_        = false;
   //
   // size_t values in this object
   compare_change_count_      = 1;
   compare_change_number_     = 0;
   compare_change_op_index_   = 0;
   num_order_taylor_          = 0;
   cap_order_taylor_          = 0;
   num_direction_taylor_      = 0;
   num_var_tape_              = tape->Rec_.num_var_rec();

   // taylor_
   taylor_.resize(0);

   // cskip_op_
   cskip_op_.resize( tape->Rec_.num_op_rec() );

   // load_op2var_
   load_op2var_.resize( tape->Rec_.num_var_load_rec() );

   // play_
   // Now that each dependent variable has a place in the tape,
   // and there is a EndOp at the end of the tape, we can transfer the
   // recording to the player and and erase the recording; i.e. ERASE Rec_.
   play_.get_recording(tape->Rec_, n);

   // ind_taddr_
   // Note that play_ has been set, we can use it to check operators
   ind_taddr_.resize(n);
   CPPAD_ASSERT_UNKNOWN( n < num_var_tape_);
   for(size_t j = 0; j < n; j++)
   {  CPPAD_ASSERT_UNKNOWN( play_.GetOp(j+1) == local::InvOp );
      ind_taddr_[j] = j+1;
   }

   // for_jac_sparse_pack_, for_jac_sparse_set_
   for_jac_sparse_pack_.resize(0, 0);
   for_jac_sparse_set_.resize(0,0);

   // resize subgraph_info_
   subgraph_info_.resize(
      ind_taddr_.size(),   // n_dep
      dep_taddr_.size(),   // n_ind
      play_.num_op_rec(),  // n_op
      play_.num_var_rec()  // n_var
   );
   // ---------------------------------------------------------------------
   // End set ad_fun.hpp private member data
   // ---------------------------------------------------------------------

   // now we can delete the tape
   AD<Base>::tape_manage(delete_tape_manage);

   // total number of varables in this recording
   CPPAD_ASSERT_UNKNOWN( num_var_tape_  == play_.num_var_rec() );

   // used to determine if there is an operation sequence in *this
   CPPAD_ASSERT_UNKNOWN( num_var_tape_  > 0 );

}

} // END CppAD namespace

# endif
