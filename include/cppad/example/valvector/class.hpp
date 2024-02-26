# ifndef CPPAD_EXAMPLE_VALVECTOR_CLASS_HPP
# define CPPAD_EXAMPLE_VALVECTOR_CLASS_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin_parent valvector}
{xrst_spell
   pytorch
   jax
   numpy
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

Operations
**********
{xrst_toc_table}


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
# define CPPAD_VALVECTOR_UNARY_NOT_AVAILABLE(type, fun) \
   inline type fun(const valvector& x) \
   {  CPPAD_VALVECTOR_ASSERT_KNOWN( false, #fun " is not available" ) \
      return type(); \
   }
// END_NOT_AVAILABLE
//
# define CPPAD_VALVECTOR_STD_MATH_MEMBER(fun) \
   valvector fun(void) const \
   {  valvector result; \
      result.resize( size() ); \
      for(size_t i = 0; i < size(); ++i) \
         result[i] = std::fun( (*this)[i] ); \
      return result; \
   }
//
# define CPPAD_VALVECTOR_MEMBER2FUNCTION(fun) \
   inline valvector fun(const valvector& x) \
   {  return x.fun(); \
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
// ============================================================================
// valvector
// ============================================================================
//
// valvector
// Forward declare
class valvector;
//
// CondExpOp
// Forward decalre
namespace CppAD {
   inline valvector CondExpOp(
      enum CompareOp         cop          ,
      const valvector&       left         ,
      const valvector&       right        ,
      const valvector&       if_true      ,
      const valvector&       if_false
   );
}
class valvector {
   //
   // friend
   friend inline valvector CppAD::CondExpOp(
      enum CppAD::CompareOp  cop          ,
      const valvector&       left         ,
      const valvector&       right        ,
      const valvector&       if_true      ,
      const valvector&       if_false
   );
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
   *s* has type `int``, ``long int``, ``double``, ``long_double``
   or ``size_t`` .

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

   {xrst_end valvector_ctor}
   ---------------------------------------------------------------------------
   */
   // default ctor
   valvector(void) : vec_(0), scalar_(0)
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
   If the size this valvector is one, *j* can have any value
   and the return is the single element in this valvector.
   Otherwise, *j* must be less than
   :ref:`size<valvector_size-name>` (for this valvector)
   and the return is the j-th element in this valvector.

   
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
   Compares this valvector with another valvector.

   Prototype
   *********
   {xrst_literal ,
      // BEGIN_EQUAL , // END_EQUAL
      // BEGIN_NOT_EQUAL , // END_NOT_EQUAL
   }

   {xrst_end valvector_compare_op}
   */
   //
   // BEGIN_EQUAL
   bool operator==(const valvector& other) const
   // END_EQUAL
   {  bool result = true;
      CPPAD_VALVECTOR_ASSERT_KNOWN(
         size() == 1 || other.size() == 1 || size() == other.size() ,
         "size error using == operator"
      )
      for(size_t i = 0; i < size(); ++i)
         result &= (*this)[i] == other[i];
      return result;
   }
   // BEGIN_NOT_EQUAL
   bool operator!=(const valvector& other) const
   // END_NOT_EQUAL
   {  return ! (*this == other); }
   /*
   {xrst_begin valvector_azmul}
   
   The valvector Absolute Zero Multiply Routine
   ############################################
   Return the element-by-element absolute zero product of this valvector
   times another; see :ref:`azmul-name` .

   Prototype
   *********
   {xrst_literal ,
      // BEGIN_AZMUL , END_AZMUL
   }

   {xrst_end valvector_azmul}
   */
   // BEGIN_AZMUL
   valvector azmul(const valvector& other) const
   // END_AZMUL
   {  CPPAD_VALVECTOR_ASSERT_KNOWN(
         size() == 1 || other.size() == 1 || size() == other.size() ,
         "size error using azmul function"
      )
      //
      // left multiply by the constant zero is a special case
      if( (*this) == valvector(0) )
         return *this;
      //
      scalar_type zero(0);
      valvector  result;
      result.vec_.resize( std::max( size(), other.size() ) );
      for(size_t i = 0; i < result.size(); ++i)
      {  if( (*this)[i] == zero )
            result[i] = zero;
          else
            result[i] = (*this)[i] * other[i];
      }
      return result;
   }
   //
   // pow
   valvector pow(const valvector& other) const
   {  CPPAD_VALVECTOR_ASSERT_KNOWN(
         size() == 1 || other.size() == 1 || size() == other.size() ,
         "size error using pow function"
      )
      valvector  result;
      result.vec_.resize( std::max( size(), other.size() ) );
      for(size_t i = 0; i < size(); ++i)
         result[i] = std::pow( (*this)[i] , other[i] );
      return result;
   }
   // =========================================================================
   // Member functions not in user API
   // =========================================================================
   //
   // Standard Math Fucntons
   CPPAD_VALVECTOR_STD_MATH_MEMBER(acos)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(acosh)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(asin)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(asinh)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(atan)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(atanh)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(cos)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(cosh)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(erf)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(erfc)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(exp)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(expm1)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(fabs)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(log)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(log1p)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(log10)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(sin)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(sinh)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(sqrt)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(tan)
   CPPAD_VALVECTOR_STD_MATH_MEMBER(tanh)
   //
   // sign
   valvector sign(void) const
   {  valvector result;
      result.resize( size() );
      scalar_type zero = scalar_type(0);
      for(size_t i = 0; i < size(); ++i)
      {  if( (*this)[i] < zero )
            result[i] = scalar_type(-1);
         if( (*this)[i] == zero )
            result[i] = zero;
         if( (*this)[i] > zero )
            result[i] = scalar_type(1);
      }
      return result;
   }
   // 
   // output
   std::ostream& output(std::ostream& os)  const
   {  os << "{ ";
      for(size_t i = 0; i < size(); ++i)
      {  os << (*this)[i];
         if( i + 1 < size() )
            os << ", ";
      }
      os << " }";
      return os;
   }
};
/*
{xrst_begin valvector_output}

Outputting a valvector
######################

Prototype
*********
{xrst_literal ,
   // BEGIN_OUTPUT , // END_OUTPUT
}

{xrst_end valvector_output}
*/
// BEGIN_OUTPUT
inline std::ostream& operator << (std::ostream& os, const valvector& v)
// END_OUTPUT
{  return v.output(os);
}
// ============================================================================
// CppAD namespace
// ============================================================================
namespace CppAD {
   //
   // numeric_limits
   CPPAD_NUMERIC_LIMITS(valvector::scalar_type, valvector)
   //
   // to_string_struct
   CPPAD_TO_STRING(valvector)
   //
   // -----------------------------------------------------------------------
   // Unary operators and functions
   //
   // a
   CPPAD_VALVECTOR_UNARY_NOT_AVAILABLE(bool, GreaterThanZero)
   CPPAD_VALVECTOR_UNARY_NOT_AVAILABLE(bool, GreaterThanOrZero)
   CPPAD_VALVECTOR_UNARY_NOT_AVAILABLE(bool, LessThanZero)
   CPPAD_VALVECTOR_UNARY_NOT_AVAILABLE(bool, LessThanOrZero)
   //
   // Unary function
   CPPAD_VALVECTOR_MEMBER2FUNCTION(acos)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(acosh)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(asin)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(asinh)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(atan)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(atanh)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(cos)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(cosh)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(erf)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(erfc)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(exp)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(expm1)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(fabs)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(log)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(log1p)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(log10)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(sin)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(sinh)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(sqrt)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(tan)
   CPPAD_VALVECTOR_MEMBER2FUNCTION(tanh)
   inline valvector abs(const valvector& x)
   {  return fabs(x); }
   CPPAD_VALVECTOR_MEMBER2FUNCTION(sign)
   // -----------------------------------------------------------------------
   // Binary opeators and functins
   //
   // Binary functions
   inline valvector pow(
      const valvector& left  ,
      const valvector& right )
   {  return left.pow(right); }
   //
   // abs_geq
   inline bool abs_geq(const valvector& x, const valvector& y)
   {  CPPAD_VALVECTOR_ASSERT_KNOWN( false, "abs_geq is not available" )
      return bool();
   }
   // ------------------------------------------------------------------------
   /*
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

The valvector Implementation of CppAD Base Type Requirements
############################################################

Not Available
*************
Some of the base type requirements cannot be satisfied for a valvector.
We use the following macro to define the corresponding functions:
{xrst_literal ,
   // BEGIN_NOT_AVAILABLE , // END_NOT_AVAILABLE
}



Output Operator
***************
The :ref:`base_require@Output Operator` requirement is satisfied by
:ref:`valvector_output-name` .

Integer
*******
The :ref:`base_require@Integer` requirement is satisfied by:
{xrst_code hpp} */
namespace CppAD {
   inline int Integer(const valvector& x) { return int( x[0] ); }
}
/* {xrst_code}

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

EqualOpSeq
**********
The :ref:`base_identical@EqualOpSeq` requirement is satisfied by:
{xrst_code hpp} */
namespace CppAD {
   inline bool EqualOpSeq(const valvector& left, const valvector& right)
   {  return left == right; }
}
/* {xrst_code}

azmul
*****
The :ref:`base_require@Absolute Zero, azmul` requirement is satisfied by:
{xrst_code hpp} */
namespace CppAD {
   inline valvector azmul( const valvector& left  , const valvector& right )
   {  return left.azmul(right); }
}
/* {xrst_code}

Identical
*********
{xrst_code hpp} */
namespace CppAD {
   inline bool IdenticalCon(const valvector& x)  {  return true; }
   inline bool IdenticalZero(const valvector& x) {  return x == valvector(0); }
   inline bool IdenticalOne(const valvector& x)  {  return x == valvector(1); }
   inline bool IdenticalEqualCon(const valvector& x, const valvector& y)
   {  return x == y; }
}
/* {xrst_code}


{xrst_end valvector_base_require}
*/

# undef CPPAD_VALVECTOR_ASSERT_KNOWN
# undef CPPAD_VALVECTOR_UNARY_NOT_AVAILABLE
# undef CPPAD_VALVECTOR_STD_MATH_MEMBER
# undef CPPAD_VALVECTOR_MEMBER2FUNCTION
# undef CPPAD_VALVECTOR_BINARY_NUMERIC_OP


# endif
