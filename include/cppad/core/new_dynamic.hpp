# ifndef CPPAD_CORE_NEW_DYNAMIC_HPP
# define CPPAD_CORE_NEW_DYNAMIC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin new_dynamic}

Change the Dynamic Parameters
#############################

Syntax
******
| *f* . ``new_dynamic`` ( *dynamic* )

Purpose
*******
Often one is only interested in computing derivatives with respect
to a subset of arguments to a function.
In this case, it is easier to make all the arguments to the function
:ref:`independent variables<glossary@Tape@Independent Variable>` .
It is more efficient,
will use less memory and be faster,
if the only the argument were are computing derivatives with respect to
are independent variables and the other arguments are
:ref:`glossary@Parameter@Dynamic` parameters.
The ``new_dynamic`` method is used to change the value
of the dynamic parameters in *f* .

f
*
The object *f* has prototype

   ``ADFun`` < *Base* > *f*

Note that the :ref:`ADFun-name` object *f* is not ``const`` .

dynamic
*******
This argument has prototype

   ``const`` *BaseVector* & *dynamic*

(see *BaseVector* below).
It specifies a new value for the independent
:ref:`glossary@Parameter@Dynamic` parameters.
It size must be the same as the size of the independent
:ref:`Independent@dynamic` parameter vector
in the call to ``Independent`` that started
the recording for *f* ; see
:ref:`fun_property@size_dyn_ind` .

BaseVector
**********
The type *BaseVector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
*Base* .

Taylor Coefficients
*******************
The Taylor coefficients computed by previous calls to
:ref:`f.Forward<Forward-name>` are lost after this operation; including the
order zero coefficients (because they may depend on the dynamic parameters).
In order words;
:ref:`f.size_order<size_order-name>` returns zero directly after
*f* . ``new_dynamic`` is called.
{xrst_toc_hidden
   example/general/new_dynamic.cpp
}
Example
*******
The file :ref:`new_dynamic.cpp-name`
contains an example and test of this operation.

{xrst_end new_dynamic}
*/
# include <cppad/local/sweep/dynamic.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file new_dynamic.hpp
User interface to ADFun dynamic_parameter member function.
*/

/*!
Change the dynamic parameters in this ADFun object

\param dynamic
is the vector of new values for the dynamic parameters.
*/
template <class Base, class RecBase>
template <class BaseVector>
void ADFun<Base,RecBase>::new_dynamic(const BaseVector& dynamic)
{  using local::pod_vector;
   CPPAD_ASSERT_KNOWN(
      size_t( dynamic.size() ) == play_.num_dynamic_ind() ,
      "f.new_dynamic: dynamic.size() different from corresponding "
      "call to Independent"
   );
   // check BaseVector is Simple Vector class with Base elements
   CheckSimpleVector<Base, BaseVector>();

   // retrieve player information about the dynamic parameters
   local::pod_vector_maybe<Base>&     all_par_vec( play_.all_par_vec() );
   const pod_vector<bool>&            dyn_par_is ( play_.dyn_par_is()  );
   const pod_vector<local::opcode_t>& dyn_par_op ( play_.dyn_par_op()  );
   const pod_vector<addr_t>&          dyn_par_arg( play_.dyn_par_arg() );
   const pod_vector<addr_t>&     dyn_ind2par_ind ( play_.dyn_ind2par_ind() );

   // set the dependent dynamic parameters
   RecBase not_used_rec_base(0.0);
   local::sweep::dynamic(
      all_par_vec         ,
      dynamic             ,
      dyn_par_is          ,
      dyn_ind2par_ind     ,
      dyn_par_op          ,
      dyn_par_arg         ,
      not_used_rec_base
   );

   // the existing Taylor coefficients are no longer valid
   num_order_taylor_ = 0;

   return;
}


} // END_CPPAD_NAMESPACE
# endif
