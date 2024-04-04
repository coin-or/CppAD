# ifndef CPPAD_EXAMPLE_VALVECTOR_CLASS_HPP
# define CPPAD_EXAMPLE_VALVECTOR_CLASS_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin_parent valvector}
{xrst_spell
   jax
   numpy
   pytorch
}

valvector: An Example Machine Learning Base Class
#################################################
This CppAD Base class preforms numerical operations on vectors; e.g.,
only one CppAD operation represents the element-by-element addition
of two vectors. This is similar to the Pytorch Tensors or Jax Numpy Arrays.

scalar_type
***********
The type ``valvector::scalar_type``
is the type corresponding to each element of a valvector .
We use *scalar_type* to denote this type.

Getting Started
===============
The file :ref:`valvector_get_started.cpp-name` is an example
that computes derivatives using valvector as the base class.

Other Examples
==============
The file :ref:`valvector.cpp-name` tests that all of the valvector examples
git the expected results.

Operations
**********
{xrst_toc_table after
   include/cppad/example/valvector/split_join.hpp
   include/cppad/example/valvector/sum.hpp
   example/valvector/get_started.cpp
   example/valvector/llsq_obj.cpp
}


{xrst_end valvector}
*/
# include <cmath>
# include <iostream>
# include <cassert>
# include <cppad/utility/vector.hpp>
# include <cppad/base_require.hpp>
// ============================================================================
// Macros
// ============================================================================
# define CPPAD_VALVECTOR_ASSERT_KNOWN(exp, msg) \
   if( ! (exp ) ) \
   {  std::cerr << "valvector: " << msg << "\n"; \
      assert( exp ); \
   }
//
// BEGIN_NOT_AVAILABLE
# define CPPAD_VALVECTOR_NOT_AVAILABLE(fun) \
   inline bool fun(const valvector& x) \
   {  CPPAD_VALVECTOR_ASSERT_KNOWN( false, #fun " is not available" ) \
      return false; \
   }
// END_NOT_AVAILABLE
//
# define CPPAD_VALVECTOR_UNARY_STD_MATH(fun) \
   inline valvector fun(const valvector &x) \
   {  valvector result; \
      result.resize( x.size() ); \
      for(size_t i = 0; i < x.size(); ++i) \
         result[i] = std::fun( x[i] ); \
      return result; \
   }
//
# define CPPAD_VALVECTOR_BINARY_NUMERIC_OP(op, compound_op) \
   valvector operator op(const valvector& other) const \
   {  CPPAD_VALVECTOR_ASSERT_KNOWN(  \
         size() == 1 || other.size() == 1 || size() == other.size() , \
         "size error using " #op " operator" \
      ) \
      valvector result; \
      result.resize( std::max( size(), other.size() ) );   \
      for(size_t i = 0; i < result.size(); ++i) \
         result[i] = (*this)[i] op other[i]; \
      return result; \
   } \
   valvector& operator compound_op(const valvector& other) \
   {  CPPAD_VALVECTOR_ASSERT_KNOWN(  \
         size() == 1 || other.size() == 1 || size() == other.size() , \
         "size error using " #compound_op " operator" \
      ) \
      if( size() == 1 && 1 < other.size()) \
      {  vec_.resize( other.size() ); \
         for(size_t i = 0; i < size(); ++i) \
            vec_[i] = scalar_; \
      } \
      for(size_t i = 0; i < size(); ++i) \
         (*this)[i] compound_op other[i]; \
      return *this; \
   }
# define CPPAD_VALVECTOR_BINARY_ORDER_OP(op) \
   bool operator op(const valvector& other) const \
   {  CPPAD_VALVECTOR_ASSERT_KNOWN( \
         size() == 1 && other.size() == 1, \
         "binary " #op " operator is only availble when both sizes are one" \
      ) \
      return (*this)[0] op other[0]; \
   }
// ============================================================================
// valvector
// ============================================================================
class valvector {
public:
   //
   // scalar_type
   typedef double                     scalar_type;
   //
private:
   //
   // vector_type
   typedef CppAD::vector<scalar_type> vector_type;
   //
   // vec_
   vector_type vec_;
   //
   // scalar_
   scalar_type scalar_;
public:
   /*
   ---------------------------------------------------------------------------
   {xrst_begin valvector_ctor}
   {xrst_spell
      initializer
      sj
   }

   The valvector Constructors
   ##########################
   Creates a valvector (called *x* below).

   Syntax
   ******
   | ``valvector`` *x*
   | ``valvector`` *x*( *s* )
   | ``valvector`` *x*( *v* )
   | ``valvector`` *x*( { *s0* , *s1* , ... } )

   Default
   *******
   The default constructor (no argument)
   creates the valvector *x* with one element using the
   :ref:`valvector@scalar_type` default constructor.

   Scalar
   ******
   The scalar constructor (argument is *s* )
   creates the valvector *x* with one element that is equal to
   *scalar_type* ( *s* ), where
   *s* has type :ref:`valvector@scalar_type` ,
   `int``, ``long int``, ``double``, ``long_double`` or ``size_t`` .

   Vector
   ******
   The vector constructor (argument is *v*)
   creates a copy of the valvector *v* .

   List
   ****
   In the standard initializer list constructor
   ( argument is { *s0* , *s1* , .. } )
   *s0* , *s1* , have valvector :ref:`valvector@scalar_type` .
   This create a valvector with size equal the length of the list
   and j-th element equal to *sj* .

   {xrst_toc_hidden
      example/valvector/ctor.cpp
   }
   Example
   =======
   The file :ref:`valvector_ctor.cpp-name` is an example and test of these
   constructors.

   {xrst_end valvector_ctor}
   ---------------------------------------------------------------------------
   */
   // default ctor
   valvector(void) : vec_(0), scalar_()
   { }
   //
   // ctor of scalar
   valvector(size_t s) : vec_(0), scalar_( scalar_type(s) )
   { }
   valvector(int s) : vec_(0), scalar_( scalar_type(s) )
   { }
   valvector(long int s) : vec_(0), scalar_( scalar_type(s) )
   { }
   valvector(double s) : vec_(0), scalar_( scalar_type(s) )
   { }
   valvector(long double s) : vec_(0), scalar_( scalar_type(s) )
   { }
   //
   valvector(const valvector& other)
   : vec_( other.vec_), scalar_( other.scalar_)
   { }
   valvector(valvector&& other)
   {  vec_.swap( other.vec_ );
      scalar_ = other.scalar_;
   }
   valvector(std::initializer_list<scalar_type> list) : vec_(0)
   {  CPPAD_VALVECTOR_ASSERT_KNOWN(
         list.size() != 0,
         "Cannot create a valvector with size zero."
      )
      std::initializer_list<scalar_type>::iterator itr;
      if( list.size() == 1 )
      {  itr     = list.begin();
         scalar_ = *itr;
      }
      else
      {  vec_.resize( list.size() );
         itr = list.begin();
         for(size_t i = 0; i < vec_.size(); ++i)
         {  vec_[i] = *itr;
            ++itr;
         }
      }
   }
   /*
   ----------------------------------------------------------------------------
   {xrst_begin valvector_resize}
   {xrst_spell
      valvectors
   }

   Resize a valvector
   ##################
   Sets the size of this valvector.

   Prototype
   *********
   {xrst_literal ,
      // BEGIN_RESIZE , // END_RESIZE
   }

   Use
   ***
   This size of a valvector directly after its
   :ref:`constructor<valvector_ctor-name>` is always one.
   This function must be used to create valvectors with other sizes.

   n
   *
   The argument *n* must not be zero and specifies the number of elements
   in the valvector.

   Element Values
   **************
   Directly after this operation,
   none of the element values are specified.

   {xrst_toc_hidden
      example/valvector/resize.cpp
   }
   Example
   =======
   The file :ref:`valvector_resize.cpp-name` is an example and test of
   valvector resize.


   {xrst_end valvector_resize}
   ----------------------------------------------------------------------------
   */
   // BEGIN_RESIZE
   void resize(size_t n)
   // END_RESIZE
   {  assert( n != 0 );
      if( n == size() )
         return;
      if( n == 1 )
         vec_.clear();
      else
         vec_.resize(n);
   }
   /*
   ----------------------------------------------------------------------------
   {xrst_begin valvector_assign}

   The valvector Assignment Operator
   #################################
   Set the value of this valvector equal to another.

   Prototype
   *********
   {xrst_literal ,
      // BEGIN_ASSIGN_ONE , // END_ASSIGN_ONE
      // BEGIN_ASSIGN_TWO , // END_ASSIGN_TWO
   }

   {xrst_toc_hidden
      example/valvector/assign.cpp
   }
   Example
   =======
   The file :ref:`valvector_assign.cpp-name` is an example and test of
   valvector assignment.

   {xrst_end valvector_assign}
   ----------------------------------------------------------------------------
   */
   // BEGIN_ASSIGN_ONE
   valvector& operator=(const valvector& other)
   // END_ASSIGN_ONE
   {  vec_    = other.vec_;
      scalar_ = other.scalar_;
      return *this;
   }
   // BEGIN_ASSIGN_TWO
   valvector& operator=(valvector&& other)
   // END_ASSIGN_TWO
   {  vec_.swap( other.vec_ );
      scalar_ = other.scalar_;
      return *this;
   }
   /*
   ----------------------------------------------------------------------------
   {xrst_begin valvector_size}

   Size of a valvector
   ###################
   Returns the number or elements in this valvector.

   Prototype
   *********
   {xrst_literal ,
      // BEGIN_SIZE , // END_SIZE
   }

   {xrst_toc_hidden
      example/valvector/size.cpp
   }
   Example
   =======
   The file :ref:`valvector_size.cpp-name` is an example and test of
   valvector size.

   {xrst_end valvector_size}
   ----------------------------------------------------------------------------
   */
   //
   // BEGIN_SIZE
   size_t size(void) const
   // END_SIZE
   {  assert( vec_.size() != 1 );
      if( vec_.size() == 0 )
         return 1;
      return vec_.size();
   }
   /*
   ----------------------------------------------------------------------------
   {xrst_begin valvector_sum}

   Sum elements of a valvector
   ###########################
   Returns the sum of the elements in this valvector.

   Prototype
   *********
   {xrst_literal ,
      // BEGIN_SUM , // END_SUM
   }

   {xrst_toc_hidden
      example/valvector/sum.cpp
   }
   Example
   =======
   The file :ref:`valvector_sum.cpp-name` is an example and test of
   valvector sum.

   {xrst_end valvector_sum}
   ----------------------------------------------------------------------------
   */
   //
   // BEGIN_SUM
   scalar_type sum(void) const
   // END_SUM
   {  scalar_type result = 0.0;
      for(size_t i = 0; i < size(); ++i)
         result += (*this)[i];
      return result;
   }
   /*
   ----------------------------------------------------------------------------
   {xrst_begin valvector_element}

   Accessing Elements of a valvector
   #################################
   Returns a reference to the specified element of this valvector.

   Prototype
   *********
   {xrst_literal ,
      // BEGIN_ELEMENT , // END_ELEMENT
      // BEGIN_CONST_ELEMENT , // END_CONST_ELEMENT
   }

   j
   *
   This is the index of the element we are accessing.

   #. If the size this valvector is one, *j* can have any value
      and the return is the single element in this valvector.
   #. If the size of this valvector is not one, *j* must be less than
      its size and the return is the j-th element in this valvector.

   {xrst_toc_hidden
      example/valvector/element.cpp
   }
   Example
   =======
   The file :ref:`valvector_element.cpp-name` is an example and test of
   valvector element access.

   {xrst_end valvector_element}
   ----------------------------------------------------------------------------
   */
   // BEGIN_ELEMENT
   scalar_type& operator[](size_t j)
   // END_ELEMENT
   {  CPPAD_VALVECTOR_ASSERT_KNOWN(
         size() == 1 || j < size(),
         "size is not one and index is greater than or equal size"
      );
      if( size() == 1 )
         return scalar_;
      return vec_[j];
   }
   // BEGIN_CONST_ELEMENT
   const scalar_type& operator[](size_t j) const
   // END_CONST_ELEMENT
   {  CPPAD_VALVECTOR_ASSERT_KNOWN(
         size() == 1 || j < size(),
         "size is not one and index is greater than or equal size"
      );
      if( size() == 1 )
         return scalar_;
      return vec_[j];
   }
   /*
   ----------------------------------------------------------------------------
   {xrst_begin valvector_unary_op}

   The valvector Numeric Unary Operators
   #####################################
   Returns the element-by-element result of the unary operators for
   this valvector.

   Prototype
   *********
   {xrst_literal ,
      // BEGIN_PLUS , END_PLUS
      // BEGIN_MINUS , END_MINUS
   }

   {xrst_toc_hidden
      example/valvector/unary_op.cpp
   }
   Example
   =======
   The file :ref:`valvector_unary_op.cpp-name` is an example and test of the
   valvector unary operators.

   {xrst_end valvector_unary_op}
   ----------------------------------------------------------------------------
   */
   // BEGIN_PLUS
   valvector operator+(void) const
   // END_PLUS
   {  return *this; }
   // BEGIN_MINUS
   valvector operator-(void) const
   // END_MINUS
   {  valvector result;
      result.resize( size() );
      for(size_t i = 0; i < size(); ++i)
         result[i] = - (*this)[i];
      return result;
   }
   /*
   ----------------------------------------------------------------------------
   {xrst_begin valvector_binary_op}
   {xrst_spell
      valvectors
   }

   The valvector Numeric Binary Operators
   ######################################
   Returns a valvector that is the element-by-element result
   of the numeric binary operators.

   Syntax
   ******
   | *x* *op* *y*

   op
   **
   The numeric binary operator *op* is
   ``+`` (addition) ,
   ``-`` (subtraction),
   ``*`` (multiplication) , or
   ``/`` (division) .

   x, y
   ****
   Both *x* and *y* are  ``const`` valvectors.

   {xrst_toc_hidden
      example/valvector/binary_op.cpp
   }
   Example
   =======
   The file :ref:`valvector_binary_op.cpp-name` is an example and test of the
   valvector binary operators.

   {xrst_end valvector_binary_op}
   ----------------------------------------------------------------------------
   {xrst_begin valvector_compound_op}

   The valvector Numeric Compound Assignment Operators
   ###################################################
   Computes the element-by-element result
   of the numeric compound assignment operators.

   Syntax
   ******
   | *y* *op* *x*

   op
   **
   The compound operator *op* is
   ``+=`` (compound addition) ,
   ``-=`` (compound subtraction),
   ``*=`` (compound multiplication) , or
   ``/=`` (compound division) .

   x
   *
   The operand *x* is a ``const`` valvector.

   y
   *
   The operand and result *y* is a valvector.

   Return
   ******
   The value returned by each of these operators is a reference to *y* .

   {xrst_toc_hidden
      example/valvector/compound_op.cpp
   }
   Example
   =======
   The file :ref:`valvector_compound_op.cpp-name` is an example and test of the
   valvector compound assignment operators.

   {xrst_end valvector_compound_op}
   */
   CPPAD_VALVECTOR_BINARY_NUMERIC_OP(+, +=)
   CPPAD_VALVECTOR_BINARY_NUMERIC_OP(-, -=)
   CPPAD_VALVECTOR_BINARY_NUMERIC_OP(*, *=)
   CPPAD_VALVECTOR_BINARY_NUMERIC_OP(/, /=)
   /*
   ----------------------------------------------------------------------------
   {xrst_begin valvector_compare_op}

   The valvector Compare Operators
   ###############################

   Syntax
   ******
   | *y* *op* *x*

   op
   **

   Equality Operators
   ==================
   == , !=

   Ordered Operators
   =================
   < , <= , >= , >

   x
   *
   The operand *x* is a ``const`` valvector.
   If *op* is an equality operator,
   *x* can have any size.
   Otherwise it must be size one.

   y
   *
   The operand *y* is a ``const`` valvector.
   If *op* is an equality operator,
   *y* can have any size.
   Otherwise it must be size one.

   Return
   ******
   The value returned by each of these operators is a bool

   {xrst_toc_hidden
      example/valvector/compare_op.cpp
   }
   Example
   =======
   The file :ref:`valvector_compare_op.cpp-name` is an example and test of the
   valvector compare operators.

   {xrst_end valvector_compare_op}
   */
   //
   bool operator==(const valvector& other) const
   {  bool result = true;
      CPPAD_VALVECTOR_ASSERT_KNOWN(
         size() == 1 || other.size() == 1 || size() == other.size() ,
         "size error using == operator"
      )
      for(size_t i = 0; i < size(); ++i)
         result &= (*this)[i] == other[i];
      return result;
   }
   bool operator!=(const valvector& other) const
   {  return ! (*this == other);
   }
   CPPAD_VALVECTOR_BINARY_ORDER_OP(<)
   CPPAD_VALVECTOR_BINARY_ORDER_OP(<=)
   CPPAD_VALVECTOR_BINARY_ORDER_OP(>=)
   CPPAD_VALVECTOR_BINARY_ORDER_OP(>)
};
/*
------------------------------------------------------------------------------
{xrst_begin valvector_output}

Outputting a valvector
######################

Prototype
*********
{xrst_literal ,
   // BEGIN_OUTPUT , // END_OUTPUT
}

{xrst_toc_hidden
   example/valvector/output.cpp
}
Example
=======
The file :ref:`valvector_output.cpp-name` is an example and test of the
valvector output.

{xrst_end valvector_output}
*/
// BEGIN_OUTPUT
inline std::ostream& operator << (std::ostream& os, const valvector& x)
// END_OUTPUT
{  os << "{";
   for(size_t i = 0; i < x.size(); ++i)
   {  os << x[i];
      if( i + 1 < x.size() )
         os << ", ";
   }
   os << "}";
   return os;
}
// ============================================================================
// CppAD namespace
// ============================================================================
namespace CppAD {
   /*
   --------------------------------------------------------------------------
   {xrst_begin valvector_unary_math}
   {xrst_spell
      erfc
      expm
      signum
   }

   The valvector Unary Math Functions
   ##################################

   Syntax
   ******
   | *y* = ``CppAD`` :: *fun* ( *x* )

   x
   *
   The argument *x* is a ``const`` valvector that is passed by reference.

   y
   *
   The result *y* is a valvector with the same size as *x* .

   fun
   ***

   Standard Math Functions
   =======================
   The function name *fun* can be any of the following:
   ``acos``,
   ``acosh``,
   ``asin``,
   ``asinh``,
   ``atan``,
   ``atanh``,
   ``cos``,
   ``cosh``,
   ``erf``,
   ``erfc``,
   ``exp``,
   ``expm1``,
   ``fabs``,
   ``log``,
   ``log1p``,
   ``log10``,
   ``sin``,
   ``sinh``,
   ``sqrt``,
   ``tan``,
   ``tanh``

   abs
   ===
   The function name *fun* can be ``abs`` ,
   which acts the same as the standard function ``fabs`` .

   sign
   ====
   The function name *fun* can be ``sign`` ,
   which computes the sign (or signum) function.

   {xrst_toc_hidden
      example/valvector/unary_math.cpp
   }
   Example
   =======
   The file :ref:`valvector_unary_math.cpp-name` is an example and test of the
   valvector unary math functions.

   {xrst_end valvector_unary_math}
   */
   //
   // standard math function
   CPPAD_VALVECTOR_UNARY_STD_MATH(acos)
   CPPAD_VALVECTOR_UNARY_STD_MATH(acosh)
   CPPAD_VALVECTOR_UNARY_STD_MATH(asin)
   CPPAD_VALVECTOR_UNARY_STD_MATH(asinh)
   CPPAD_VALVECTOR_UNARY_STD_MATH(atan)
   CPPAD_VALVECTOR_UNARY_STD_MATH(atanh)
   CPPAD_VALVECTOR_UNARY_STD_MATH(cos)
   CPPAD_VALVECTOR_UNARY_STD_MATH(cosh)
   CPPAD_VALVECTOR_UNARY_STD_MATH(erf)
   CPPAD_VALVECTOR_UNARY_STD_MATH(erfc)
   CPPAD_VALVECTOR_UNARY_STD_MATH(exp)
   CPPAD_VALVECTOR_UNARY_STD_MATH(expm1)
   CPPAD_VALVECTOR_UNARY_STD_MATH(fabs)
   CPPAD_VALVECTOR_UNARY_STD_MATH(log)
   CPPAD_VALVECTOR_UNARY_STD_MATH(log1p)
   CPPAD_VALVECTOR_UNARY_STD_MATH(log10)
   CPPAD_VALVECTOR_UNARY_STD_MATH(sin)
   CPPAD_VALVECTOR_UNARY_STD_MATH(sinh)
   CPPAD_VALVECTOR_UNARY_STD_MATH(sqrt)
   CPPAD_VALVECTOR_UNARY_STD_MATH(tan)
   CPPAD_VALVECTOR_UNARY_STD_MATH(tanh)
   //
   // abs
   inline valvector abs(const valvector& x)
   {  return fabs(x); }
   //
   // sign
   inline valvector sign(const valvector& x)
   {  typedef valvector::scalar_type scalar_type;
      //
      valvector result;
      result.resize( x.size() );
      scalar_type zero = scalar_type(0);
      for(size_t i = 0; i < x.size(); ++i)
      {  if( x[i] < zero )
            result[i] = scalar_type(-1);
         if( x[i] == zero )
            result[i] = zero;
         if( x[i] > zero )
            result[i] = scalar_type(1);
      }
      return result;
   }
   /*
   --------------------------------------------------------------------------
   {xrst_begin valvector_pow}

   The valvector Pow Function
   ##########################

   Syntax
   ******
   | *z* = ``CppAD::pow`` ( *x* , *y* )

   x
   *
   The argument *x* is a ``const`` valvector that is passed by reference.

   y
   *
   The argument *y* is a ``const`` valvector that is passed by reference.
   If the size of *x* is not one,
   and the size of *y* is not one,
   the size of *x* and *y* must be equal.

   z
   *
   The result *z* is a valvector with size equal to the maximum of the
   size of *x* and the size of *y* .

   {xrst_toc_hidden
      example/valvector/pow.cpp
   }
   Example
   =======
   The file :ref:`valvector_pow.cpp-name` is an example and test of the
   valvector pow function.

   {xrst_end valvector_pow}
   */
   inline valvector pow(const valvector& x, const valvector& y)
   {  CPPAD_VALVECTOR_ASSERT_KNOWN(
         x.size() == 1 || y.size() == 1 || x.size() == y.size() ,
         "size error using pow function"
      )
      valvector  result;
      result.resize( std::max( x.size(), y.size() ) );
      for(size_t i = 0; i < x.size(); ++i)
         result[i] = std::pow( x[i] , y[i] );
      return result;
   }
   /*
   --------------------------------------------------------------------------
   {xrst_begin valvector_azmul}
   {xrst_spell
      valvectors
   }

   Absolute Zero Multiply of valvectors
   ####################################

   Syntax
   ******
   | *z* = ``CppAD::azmul`` ( *x* , *y* )

   x
   *
   The argument *x* is a ``const`` valvector that is passed by reference.

   y
   *
   The argument *y* is a ``const`` valvector that is passed by reference.
   If the size of *x* is not one,
   and the size of *y* is not one,
   the size of *x* and *y* must be equal.

   z
   *
   In the special where all the elements of *z* are zero,
   the result *z* is a valvector with size one.
   Otherwise, *z* is a valvector with size equal to the maximum of the
   size of *x* and the size of *y* .

   {xrst_toc_hidden
      example/valvector/azmul.cpp
   }
   Example
   =======
   The file :ref:`valvector_azmul.cpp-name` is an example and test of the
   valvector absolute zero multiply function.

   {xrst_end valvector_azmul}
   */
   inline valvector azmul(const valvector& x, const valvector& y)
   {  typedef valvector::scalar_type scalar_type;
      //
      CPPAD_VALVECTOR_ASSERT_KNOWN(
         x.size() == 1 || y.size() == 1 || x.size() == y.size() ,
         "size error using azmul function"
      )
      //
      // special case
      valvector vec_zero = valvector(0);
      if( x == vec_zero )
         return vec_zero;
      //
      // elememt-by-element
      scalar_type scalar_zero(0);
      valvector  result;
      result.resize( std::max( x.size(), y.size() ) );
      for(size_t i = 0; i < result.size(); ++i)
      {  if( x[i] == scalar_zero )
            result[i] = scalar_zero;
          else
            result[i] = x[i] * y[i];
      }
      return result;
   }
   /*
   ------------------------------------------------------------------------
   {xrst_begin valvector_condexp}
   {xrst_spell
      ge
      valvectors
   }

   The valvector Conditional Expressions
   #####################################
   Computes element-by-element :ref:`CondExp-name` values where
   the arguments are valvectors.

   Syntax
   ******
   *result* = ``CondExp`` *Rel* ( *left* , *right* , *if_true* , *if_false* )

   Discussion
   **********
   For each valid index *i* , this computes the result

   | |tab| ``if`` ( *left* [ *i* ] *op* *right* [ *i* ]  )
   | |tab| |tab| *result* [ *i* ] = *if_true* [ *i* ]
   | |tab| ``else``
   | |tab| |tab| *result* [ *i* ] = *if_false* [ *i* ]

   where the relational *Rel* and the operator *op*
   have the following correspondence:

   .. csv-table::

      *Rel* , ``Lt`` , ``Le`` , ``Eq`` , ``Ge`` , ``Gt``
      *op*  ,  <     , <=     , ==     , >=     , >

   Arguments
   *********
   All of the argument are ``const`` valvectors.

   result
   ******
   The result has size equal to the maximum of the size of
   *left* , *right* , *if_true* and *if_false* .
   The size of each argument must be one, or the same as the size of *result*.

   {xrst_toc_hidden
      example/valvector/condexp.cpp
   }
   Example
   =======
   The file :ref:`valvector_condexp.cpp-name` is an example and test of the
   valvector conditional expressions.

   {xrst_end valvector_condexp}
   */
   inline valvector CondExpOp(
      enum CompareOp         cop          ,
      const valvector&       left         ,
      const valvector&       right        ,
      const valvector&       if_true      ,
      const valvector&       if_false )
   {  //
      // result_size
      size_t result_size = std::max(left.size(), right.size());
      result_size        = std::max(result_size, if_true.size());
      result_size        = std::max(result_size, if_false.size());
      //
      // size_ok
      bool size_ok  = true;
      size_ok &= left.size()     == 1 || left.size()     == result_size;
      size_ok &= right.size()    == 1 || right.size()    == result_size;
      size_ok &= if_true.size()  == 1 || if_true.size()  == result_size;
      size_ok &= if_false.size() == 1 || if_false.size() == result_size;
      CPPAD_VALVECTOR_ASSERT_KNOWN(
         size_ok,
         "argument sizes do not agree in conditional expression"
      );
      //
      // result
      valvector result;
      result.resize(result_size);
      //
      for(size_t i = 0; i < result_size; ++i)
      {  switch( cop )
         {
            case CompareLt:
            if( left[i] < right[i] )
               result[i] = if_true[i];
            else
               result[i] = if_false[i];;
            break;

            case CompareLe:
            if( left[i] <= right[i] )
               result[i] = if_true[i];
            else
               result[i] = if_false[i];;
            break;

            case CompareEq:
            if( left[i] == right[i] )
               result[i] = if_true[i];
            else
               result[i] = if_false[i];;
            break;

            case CompareGe:
            if( left[i] >= right[i] )
               result[i] = if_true[i];
            else
               result[i] = if_false[i];;
            break;

            case CompareGt:
            if( left[i] > right[i] )
               result[i] = if_true[i];
            else
               result[i] = if_false[i];;
            break;

            default:
            assert(false);
            result[i] = if_true[i];
         }
      }
      // result
      return result;
   }
   CPPAD_COND_EXP_REL(valvector)
}
// ===========================================================================
/*
{xrst_begin valvector_base_require}
{xrst_spell
   geq
}

The valvector Implementation of CppAD Base Type Requirements
############################################################

Output Operator
***************
The :ref:`base_require@Output Operator` requirement is satisfied by
:ref:`valvector_output-name` .


Constructors
************
The :ref:`base_member@Constructors` requirements are satisfied by
:ref:`valvector_ctor-name` .

Unary Operators
***************
The :ref:`base_member@Unary Operators` requirements are satisfied by
:ref:`valvector_unary_op-name` .

Assignment Operators
********************
The :ref:`base_member@Assignment Operators` requirements are satisfied by
:ref:`valvector_assign-name` and :ref:`valvector_compound_op-name` .

Binary Operators
****************
The :ref:`base_member@Binary Operators` requirements are satisfied by
:ref:`valvector_binary_op-name` .

Bool Operators
**************
The :ref:`base_member@Bool Operators` requirements are satisfied by
:ref:`valvector_compare_op-name` .

Conditional Expressions
***********************
The :ref:`base_cond_exp-name` requirements are satisfied by
:ref:`valvector_condexp-name` .

Standard Math
*************
The :ref:`base_std_math-name` requirements are satisfied by
:ref:`valvector_unary_math-name` and :ref:`valvector_pow-name` .

azmul
*****
The :ref:`base_require@Absolute Zero, azmul` requirement is satisfied by
:ref:`valvector_azmul-name` .

Features Implemented Here
*************************

Integer
=======
The :ref:`base_require@Integer` requirement is satisfied by:
{xrst_code hpp} */
namespace CppAD {
   inline int Integer(const valvector& x) { return int( x[0] ); }
}
/* {xrst_code}

numeric_limits
==============
The :ref:`base_limits-name` requirement is satisfied by:
{xrst_code hpp} */
namespace CppAD {
   CPPAD_NUMERIC_LIMITS(valvector::scalar_type, valvector)
}
/* {xrst_code}

to_string
=========
The :ref:`base_to_string-name` requirement is satisfied by:
{xrst_code hpp} */
namespace CppAD {
   CPPAD_TO_STRING(valvector)
}
/* {xrst_code}

EqualOpSeq
==========
The :ref:`base_identical@EqualOpSeq` requirement is satisfied by:
{xrst_code hpp} */
namespace CppAD {
   inline bool EqualOpSeq(const valvector& left, const valvector& right)
   {  return left == right; }
}
/* {xrst_code}

Identical
=========
{xrst_code hpp} */
namespace CppAD {
   inline bool IdenticalCon(const valvector& x)  {  return true; }
   inline bool IdenticalZero(const valvector& x) {  return x == valvector(0); }
   inline bool IdenticalOne(const valvector& x)  {  return x == valvector(1); }
   inline bool IdenticalEqualCon(const valvector& x, const valvector& y)
   {  return x == y; }
}
/* {xrst_code}

Not Ordered
===========
The :ref:`base_ordered@Not Ordered` requirement is satisfied by:
{xrst_literal ,
   // BEGIN_NOT_AVAILABLE , // END_NOT_AVAILABLE
}
{xrst_code hpp} */
namespace CppAD {
   CPPAD_VALVECTOR_NOT_AVAILABLE(GreaterThanZero)
   CPPAD_VALVECTOR_NOT_AVAILABLE(GreaterThanOrZero)
   CPPAD_VALVECTOR_NOT_AVAILABLE(LessThanZero)
   CPPAD_VALVECTOR_NOT_AVAILABLE(LessThanOrZero)
   //
   // abs_geq
   inline bool abs_geq(const valvector& x, const valvector& y)
   {  CPPAD_VALVECTOR_ASSERT_KNOWN(
         x.size() == 1 || y.size() == 1 || x.size() == y.size() ,
         "size error in abs_geq functions"
      )
      size_t max_size = std::max( x.size(), y.size() );
      valvector::scalar_type abs_x(0), abs_y(0);
      for(size_t i = 0; i < max_size; ++i)
      {  abs_x += fabs( x[i] );
         abs_y += fabs( y[i] );
      }
      return abs_x >= abs_y;
   }
}
/* {xrst_code}

{xrst_toc_hidden
   example/valvector/base_require.cpp
}
Example
=======
The file :ref:`valvector_base_require.cpp-name` is an example and test of the
valvector :ref:`valvector_base_require@Features Implemented Here` .


{xrst_end valvector_base_require}
*/

# undef CPPAD_VALVECTOR_ASSERT_KNOWN
# undef CPPAD_VALVECTOR_UNARY_STD_MATH
# undef CPPAD_VALVECTOR_BINARY_ORDER_OP
# undef CPPAD_VALVECTOR_BINARY_NUMERIC_OP
# undef CPPAD_VALVECTOR_NOT_AVAILABLE


# endif
