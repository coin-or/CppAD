# ifndef CPPAD_CORE_FUN_CONSTRUCT_HPP
# define CPPAD_CORE_FUN_CONSTRUCT_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin fun_construct}
{xrst_spell
   versa
}

Construct an ADFun Object and Stop Recording
############################################

Syntax
******
| ``ADFun`` < *Base* > *f* ( *ax* , *ay* );
| ``ADFun`` < *Base* > *f*
| *f* . ``swap`` ( *g* )
| ``f`` = ``g``

Purpose
*******
The ``ADFun`` < *Base* > object *f*
stores an AD of *Base*
:ref:`operation sequence<glossary@Operation@Sequence>` .
It can then be used to calculate derivatives of the corresponding
:ref:`glossary@AD Function`

.. math::

   F : \B{R}^n \rightarrow \B{R}^m

where :math:`B` is the space corresponding to objects of type *Base* .

ax
**
If the argument *ax* is present, it has prototype

   ``const`` *ADVector* & *ax*

It must be the vector argument in the previous call to
:ref:`Independent-name` .
Neither its size, or any of its values, are allowed to change
between calling

   ``Independent`` ( *ax* )

and

   ``ADFun`` < *Base* > *f* ( *ax* , *ay* )

ay
**
If the argument *ay* is present, it has prototype

   ``const`` *ADVector* & *ay*

The sequence of operations that map *ax*
to *ay* are stored in the ADFun object *f* .

ADVector
********
The type *ADVector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
``AD`` < *Base* > .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

Default Constructor
*******************
The default constructor

   ``ADFun`` < *Base* > *g*

creates an
``AD`` < *Base* > object with no corresponding operation sequence; i.e.,

   *g* . ``size_var`` ()

returns the value zero (see :ref:`fun_property@size_var` ).

Sequence Constructor
********************
The following constructor stores the current ``AD`` < *Base* > operation
sequence in *f* :

   ``ADFun`` < *Base* > *f* ( *ax* , *ay* )

To be specific, it is equivalent to the following
steps using the default constructor:

#. Create *f* with the default constructor

      ``ADFun`` < *Base* > *f* ;

#. Stop the recording and store the operation sequence using

      *f* . ``Dependent`` ( *ax* , *ay* );

   see :ref:`Independent@Start Recording` ,
   :ref:`Dependent@Stop Recording` , and
   :ref:`Dependent@Store Operation Sequence` .

#. Calculate the zero order Taylor coefficients for all
   the variables in the operation sequence using

      *y* = *f* . ``Forward`` ( 0 , *x* )

   see :ref:`forward_zero-name`.
   Here *x* and *y* are :ref:`simple vectors <SimpleVector-name>`
   with elements of type *Base* and the elements of *x*
   are equal to the corresponding elements in *ax*.

#. If NDEBUG is not defined, *y* is checked to make sure it's elements are
   nearly equal to the corresponding values in *ay* .

Copy Constructor
****************
It is an error to attempt to use the ``ADFun`` < *Base* > copy constructor;
i.e., the following syntax is not allowed:

   ``ADFun`` < *Base* > *g* ( *f* )

where *f* is an ``ADFun`` < *Base* > object.
Use its :ref:`fun_construct@Default Constructor` instead
and its assignment operator.

swap
****
The swap operation *f* . ``swap`` ( *g* ) exchanges the contents of
the two ``ADFun`` < *Base* > functions; i.e.,
*f* before the swap is identical to *g* after the swap and vise versa.

Assignment Operator
*******************
The ``ADFun`` < *Base* > assignment operation

   *g* = *f*

makes a copy of the operation sequence currently stored in *f*
in the object *g* .
The object *f* is not affected by this operation and
can be ``const`` .
All of information (state) stored in *f* is copied to *g*
and any information originally in *g* is lost.

Move Semantics
==============
In the special case where *f* is a temporary object,
this assignment will use move semantics.
This avoids the overhead of the copying all the information from
*f* to *g* .

Taylor Coefficients
===================
The Taylor coefficient information currently stored in *f*
(computed by :ref:`f.Forward<Forward-name>` ) is
copied to *g* .
Hence, directly after this operation

   *g* . ``size_order`` () == *f* . ``size_order`` ()

Sparsity Patterns
=================
The forward Jacobian sparsity pattern currently stored in *f*
(computed by :ref:`f.ForSparseJac<ForSparseJac-name>` ) is
copied to *g* .
Hence, directly after this operation

| |tab| *g* . ``size_forward_bool`` () == *f* . ``size_forward_bool`` ()
| |tab| *g* . ``size_forward_set`` ()  == *f* . ``size_forward_set`` ()

Parallel Mode
*************
The call to ``Independent`` ,
and the corresponding call to

   ``ADFun`` < *Base* > *f* ( *ax* , *ay* )

or

   *f* . ``Dependent`` ( *ax* , *ay* )

or :ref:`abort_recording-name` ,
must be preformed by the same thread; i.e.,
:ref:`thread_alloc::thread_num<ta_thread_num-name>` must be the same.

Example
*******

Sequence Constructor
====================
The file
:ref:`independent.cpp-name`
contains an example and test of the sequence constructor.

Default Constructor
===================
The files
:ref:`fun_check.cpp-name`
and
:ref:`hes_lagrangian.cpp-name`
contain an examples and tests using the default constructor.
They return true if they succeed and false otherwise.
{xrst_toc_hidden
   example/general/fun_assign.cpp
}
Assignment Operator
===================
The file
:ref:`fun_assign.cpp-name`
contains an example and test of the ``ADFun`` < *Base* >
assignment operator.

{xrst_end fun_construct}
----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file fun_construct.hpp
ADFun function constructors and assignment operator.
*/

/*!
ADFun default constructor

The C++ syntax for this operation is
\verbatim
   ADFun<Base> f
\endverbatim
An empty ADFun object is created.
The Dependent member function,
or the ADFun<Base> assingment operator,
can then be used to put an operation sequence in this ADFun object.

\tparam Base
is the base for the recording that can be stored in this ADFun object;
i.e., operation sequences that were recorded using the type AD<Base>.
*/
template <class Base, class RecBase>
ADFun<Base,RecBase>::ADFun(void) :
function_name_(""),
exceed_collision_limit_(false),
has_been_optimized_(false),
check_for_nan_(true) ,
compare_change_count_(0),
compare_change_number_(0),
compare_change_op_index_(0),
num_order_taylor_(0),
cap_order_taylor_(0),
num_direction_taylor_(0),
num_var_tape_(0)
{ }
//
// move semantics version of constructor
// (none of the defualt constructor values matter to the destructor)
template <class Base, class RecBase>
ADFun<Base,RecBase>::ADFun(ADFun&& f)
{  swap(f); }
//
// destructor
template <class Base, class RecBase>
ADFun<Base,RecBase>::~ADFun(void)
{ }
/*!
ADFun assignment operator

The C++ syntax for this operation is
\verbatim
   g = f
\endverbatim
where g and f are ADFun<Base> ADFun objects.
A copy of the the operation sequence currently stored in f
is placed in this ADFun object (called g above).
Any information currently stored in this ADFun object is lost.

\tparam Base
is the base for the recording that can be stored in this ADFun object;
i.e., operation sequences that were recorded using the type AD<Base>.

\param f
ADFun object containing the operation sequence to be copied.
*/
template <class Base, class RecBase>
void ADFun<Base,RecBase>::operator=(const ADFun& f)
{
   // go through member variables in ad_fun.hpp order
   //
   // string objects
   function_name_             = f.function_name_;
   //
   // bool objects
   exceed_collision_limit_    = f.exceed_collision_limit_;
   has_been_optimized_        = f.has_been_optimized_;
   check_for_nan_             = f.check_for_nan_;
   //
   // size_t objects
   compare_change_count_      = f.compare_change_count_;
   compare_change_number_     = f.compare_change_number_;
   compare_change_op_index_   = f.compare_change_op_index_;
   num_order_taylor_          = f.num_order_taylor_;
   cap_order_taylor_          = f.cap_order_taylor_;
   num_direction_taylor_      = f.num_direction_taylor_;
   num_var_tape_              = f.num_var_tape_;
   //
   // pod_vector objects
   ind_taddr_                 = f.ind_taddr_;
   dep_taddr_                 = f.dep_taddr_;
   dep_parameter_             = f.dep_parameter_;
   cskip_op_                  = f.cskip_op_;
   load_op2var_               = f.load_op2var_;
   //
   // pod_vector_maybe_vectors
   taylor_                    = f.taylor_;
   subgraph_partial_          = f.subgraph_partial_;
   //
   // player
   play_                      = f.play_;
   //
   // subgraph
   subgraph_info_             = f.subgraph_info_;
   //
   // sparse_pack
   for_jac_sparse_pack_       = f.for_jac_sparse_pack_;
   //
   // sparse_list
   for_jac_sparse_set_        = f.for_jac_sparse_set_;
}
/// swap
template <class Base, class RecBase>
void ADFun<Base,RecBase>::swap(ADFun& f)
{
   // string objects
   function_name_.swap( f.function_name_ );
   //
   // bool objects
   std::swap( exceed_collision_limit_    , f.exceed_collision_limit_);
   std::swap( has_been_optimized_        , f.has_been_optimized_);
   std::swap( check_for_nan_             , f.check_for_nan_);
   //
   // size_t objects
   std::swap( compare_change_count_      , f.compare_change_count_);
   std::swap( compare_change_number_     , f.compare_change_number_);
   std::swap( compare_change_op_index_   , f.compare_change_op_index_);
   std::swap( num_order_taylor_          , f.num_order_taylor_);
   std::swap( cap_order_taylor_          , f.cap_order_taylor_);
   std::swap( num_direction_taylor_      , f.num_direction_taylor_);
   std::swap( num_var_tape_              , f.num_var_tape_);
   //
   // pod_vector objects
   ind_taddr_.swap(      f.ind_taddr_);
   dep_taddr_.swap(      f.dep_taddr_);
   dep_parameter_.swap(  f.dep_parameter_);
   taylor_.swap(         f.taylor_);
   cskip_op_.swap(       f.cskip_op_);
   load_op2var_.swap(    f.load_op2var_);
   //
   // player
   play_.swap(f.play_);
   //
   // subgraph_info
   subgraph_info_.swap(f.subgraph_info_);
   //
   // sparse_pack
   for_jac_sparse_pack_.swap( f.for_jac_sparse_pack_);
   //
   // sparse_list
   for_jac_sparse_set_.swap( f.for_jac_sparse_set_);
}
/// Move semantics version of constructor and assignment
template <class Base, class RecBase>
void ADFun<Base,RecBase>::operator=(ADFun&& f)
{  swap(f); }


/*!
ADFun constructor from an operation sequence.

The C++ syntax for this operation is
\verbatim
   ADFun<Base> f(x, y)
\endverbatim
The operation sequence that started with the previous call
 Independent(x), and that ends with this operation, is stored
in this ADFun<Base> object f.

\tparam Base
is the base for the recording that will be stored in the object f;
i.e., the operations were recorded using the type AD<Base>.

\tparam ADVector
is a simple vector class with elements of typea AD<Base>.

\param x
is the independent variable vector for this ADFun object.
The domain dimension of this object will be the size of x.

\param y
is the dependent variable vector for this ADFun object.
The range dimension of this object will be the size of y.

\par Taylor Coefficients
A zero order forward mode sweep is done,
and if NDEBUG is not defined the resulting values for the
depenedent variables are checked against the values in y.
Thus, the zero order Taylor coefficients
corresponding to the value of the x vector
are stored in this ADFun object.
*/
template <class Base, class RecBase>
template <class ADVector>
ADFun<Base,RecBase>::ADFun(const ADVector &x, const ADVector &y)
{
   // used to identify the RecBase type in calls to sweeps
   RecBase not_used_rec_base(0.0);

   CPPAD_ASSERT_KNOWN(
      x.size() > 0,
      "ADFun<Base>: independent variable vector has size zero."
   );
   CPPAD_ASSERT_KNOWN(
      Variable(x[0]),
      "ADFun<Base>: independent variable vector has been changed."
   );
   local::ADTape<Base>* tape = AD<Base>::tape_ptr(x[0].tape_id_);
   CPPAD_ASSERT_KNOWN(
      tape->size_independent_ == size_t ( x.size() ),
      "ADFun<Base>: independent variable vector has been changed."
   );
   size_t j, n = x.size();
# ifndef NDEBUG
   size_t i, m = y.size();
   for(j = 0; j < n; j++)
   {  CPPAD_ASSERT_KNOWN(
      size_t(x[j].taddr_) == (j+1),
      "ADFun<Base>: independent variable vector has been changed."
      );
      CPPAD_ASSERT_KNOWN(
      x[j].tape_id_ == x[0].tape_id_,
      "ADFun<Base>: independent variable vector has been changed."
      );
   }
   for(i = 0; i < m; i++)
   {  CPPAD_ASSERT_KNOWN(
      CppAD::Parameter( y[i] ) || (y[i].tape_id_ == x[0].tape_id_) ,
      "ADFun<Base>: dependent vector contains variables for"
      "\na different tape than the independent variables."
      );
   }
# endif

   // stop the tape and store the operation sequence
   Dependent(tape, y);

   // This function has not yet been optimized
   exceed_collision_limit_    = false;

   // ad_fun.hpp member values not set by dependent
   check_for_nan_       = true;

   // allocate memory for one zero order taylor_ coefficient
   CPPAD_ASSERT_UNKNOWN( num_order_taylor_ == 0 );
   CPPAD_ASSERT_UNKNOWN( num_direction_taylor_ == 0 );
   size_t c = 1;
   size_t r = 1;
   capacity_order(c, r);
   CPPAD_ASSERT_UNKNOWN( cap_order_taylor_     == c );
   CPPAD_ASSERT_UNKNOWN( num_direction_taylor_ == r );

   // set zero order coefficients corresponding to indpendent variables
   CPPAD_ASSERT_UNKNOWN( n == ind_taddr_.size() );
   for(j = 0; j < n; j++)
   {  CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] == (j+1) );
      CPPAD_ASSERT_UNKNOWN( size_t(x[j].taddr_) == (j+1) );
      taylor_[ ind_taddr_[j] ]  = x[j].value_;
   }

   // use independent variable values to fill in values for others
   CPPAD_ASSERT_UNKNOWN( cskip_op_.size() == play_.num_op_rec() );
   CPPAD_ASSERT_UNKNOWN( load_op2var_.size()  == play_.num_var_load_rec() );
   bool print = false;
   local::sweep::forward_0(
      not_used_rec_base,
      &play_,
      num_var_tape_,
      cap_order_taylor_,
      cskip_op_.data(),
      load_op2var_,
      compare_change_count_,
      compare_change_number_,
      compare_change_op_index_,
      std::cout,
      print,
      taylor_.data()
   );
   CPPAD_ASSERT_UNKNOWN( compare_change_count_    == 1 );
   CPPAD_ASSERT_UNKNOWN( compare_change_number_   == 0 );
   CPPAD_ASSERT_UNKNOWN( compare_change_op_index_ == 0 );

   // now set the number of orders stored
   num_order_taylor_ = 1;

# ifndef NDEBUG
   // on MS Visual Studio 2012, CppAD required in front of isnan ?
   for(i = 0; i < m; i++)
   if( taylor_[dep_taddr_[i]] != y[i].value_ || CppAD::isnan( y[i].value_ ) )
   {  using std::endl;
      std::ostringstream buf;
      buf << "A dependent variable value is not equal to "
         << "its tape evaluation value," << endl
         << "perhaps it is nan." << endl
         << "Dependent variable value = "
         <<  y[i].value_ << endl
         << "Tape evaluation value    = "
         <<  taylor_[dep_taddr_[i]]  << endl
         << "Difference               = "
         <<  y[i].value_ -  taylor_[dep_taddr_[i]]  << endl
      ;
      // buf.str() returns a string object with a copy of the current
      // contents in the stream buffer.
      std::string msg_str       = buf.str();
      // msg_str.c_str() returns a pointer to the c-string
      // representation of the string object's value.
      const char* msg_char_star = msg_str.c_str();
      CPPAD_ASSERT_KNOWN(
         0,
         msg_char_star
      );
   }
# endif
}

} // END_CPPAD_NAMESPACE
# endif
