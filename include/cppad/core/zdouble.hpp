# ifndef CPPAD_CORE_ZDOUBLE_HPP
# define CPPAD_CORE_ZDOUBLE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin zdouble app}
zdouble: An AD Base Type With Absolute Zero
###########################################

Deprecated 2015-09-26
*********************
Use the function :ref:`azmul-name` instead.

Absolute Zero
*************
The ``zdouble`` class acts like the ``double`` type
with the added property that zero times any value is zero.
This includes zero time :ref:`nan-name` and zero times infinity.
In addition, zero divided by any value and any value times zero
are also zero.

Syntax
******

Constructor and Assignment
==========================

| ``zdouble z``
| ``zdouble z`` ( ``x`` )
| ``zdouble z`` ( ``i`` )
| *z1* *op* *x*

were *i* is a ``size_t`` or *long* or ``int`` ,
*x* is a ``double`` or ``zdouble`` ,
and *op* is ``=`` or ``+=`` or ``-=`` or ``*=``
or ``/=-`` .

Comparison Operators
====================

| *b* = *z* *op* *x*
| *b* = *x* *op* *z*

where *b* is a ``bool`` object,
*z* is a ``zdouble`` object,
*x* is a ``double`` or ``zdouble`` object, and
*op* is ``==`` , ``!=`` , ``<=`` , ``>=`` ,
``<`` or ``>`` .

Arithmetic Operators
====================

| *z2* = *z1* *op* *x*
| *z2* = *x* *op* *z1*

where *z1* , *z2* are ``zdouble`` objects,
*x* is a ``double`` or ``zdouble`` object, and
*op* is ``+`` , ``-`` , ``*`` or ``/`` .

Standard Math
=============

| *z2* = *fun* ( *z1* )
| *z3* = ``pow`` ( *z1* , *z2* )

where *z1* , *z2* , *z3* are ``zdouble`` objects and
*fun* is a :ref:`unary_standard_math-name` function.

Nan
===
There is a specialization of :ref:`nan-name` so that

   ``z2`` = *nan* ( ``z1`` )

returns 'not a number' when *z1* has type ``zdouble`` .
Note that this template function needs to be specialized because

   ``zdouble`` (0.0) == ``zdouble`` (0.0) / ``zdouble`` (0.0)

Motivation
**********

General
=======
Often during computing (and more so in parallel computing) alternative
values for an expression are computed and one of the alternatives
is chosen using some boolean variable.
This is often represented by

   *result* = *flag* * *value_if_true* + (1 ``-`` *flag* ) * *value_if_false*

where *flag* is one for true and zero for false.
This representation does not work for ``double`` when the value
being multiplied by zero is ``+inf`` , ``-inf`` , or ``nan`` .

CppAD
=====
In CppAD one can use
:ref:`conditional expressions<CondExp-name>` to achieve the representation

   *result* = *flag* * *value_if_true* + (1 ``-`` *flag* ) * *value_if_false*

This works fine except when there are
:ref:`multiple levels of AD<mul_level-name>` ; e.g.,
when using ``AD< AD<double> >`` .
In this case the corresponding AD function objects have type
:ref:`ADFun\< AD\<double> ><fun_construct-name>` .
When these AD function objects compute derivatives using
:ref:`reverse-name` mode, the conditional expressions are represented use
zeros to multiply the expression that is not used.
Using ``AD< AD<zdouble> >`` instead of ``AD< AD<double> >``
makes this representation work and fixes the problem.

Base Type Requirements
**********************
The type ``zdouble`` satisfies all of the CppAD
:ref:`base type requirements<base_require-name>` .

{xrst_end zdouble}
*/
# include <cppad/base_require.hpp>
# include <cppad/utility/nan.hpp>

/*!
\file zdouble.hpp
Define a class like double but with an absolute zero.
*/

/*!
\def CPPAD_ZDOUBLE_NORMAL_ASSIGN_OPERATOR(op)
Define a compound assignment member operator that functions the same
as corresponding double operator.
*/
# define CPPAD_ZDOUBLE_NORMAL_ASSIGN_OPERATOR(op) \
   zdouble& operator op (const zdouble& z) \
   {  dbl_ op z.dbl_;                     \
      return *this;                       \
   }                                       \
   zdouble& operator op (const double& x)  \
   {  dbl_ op x;                          \
      return *this;                       \
   }

/*!
\def CPPAD_ZDOUBLE_UNARY_OPERATOR(op)
Define a unary compound assignment member operator.
*/
# define CPPAD_ZDOUBLE_UNARY_OPERATOR(op) \
   zdouble operator op (void) const      \
   {  return zdouble( op dbl_ ); }

/*!
# define CPPAD_ZDOUBLE_NORMAL_BINARY_OPERATOR(op)
Define a binary arithmetic member operator that functions the same
as corresponding double operator.
*/
# define CPPAD_ZDOUBLE_NORMAL_BINARY_OPERATOR(op) \
   zdouble operator op (const zdouble& z) const  \
   {  return zdouble( dbl_ op z.dbl_ ); }       \
   zdouble operator op (const double& x) const   \
   {  return zdouble( dbl_ op x ); }

/*!
\def CPPAD_ZDOUBLE_COMPARE_OPERATOR(op)
Define a comparison member operator.
*/
# define CPPAD_ZDOUBLE_COMPARE_OPERATOR(op)   \
   bool operator op (const zdouble& z) const \
   {  return dbl_ op z.dbl_; }             \
   bool operator op (const double& x) const \
   {  return dbl_ op x; }

/*!
\def CPPAD_ZDOUBLE_OTHER_BINARY_OPERATOR(op)
Define a binary arithmetic operator that is not a member because
the double operand is on the left.
*/
# define CPPAD_ZDOUBLE_OTHER_BINARY_OPERATOR(op) \
   inline zdouble operator op(const double& x, const zdouble& z) \
   {  return zdouble(x) op z; }

/*!
\def CPPAD_ZDOUBLE_OTHER_COMPARE_OPERATOR(op, op_switch)
Define a comparison operator that is not a member because
the double operand is on the left.
Convert it to the case where the double operand is on the right by
by using op_switch instead of op.
*/
# define CPPAD_ZDOUBLE_OTHER_COMPARE_OPERATOR(op, op_switch) \
   inline bool operator op(const double& x, const zdouble& z)    \
   {  return z op_switch x; }

/*!
\def CPPAD_ZDOUBLE_STD_MATH_FRIEND(fun)
Declare that a standard math function is a friend.
*/
# define CPPAD_ZDOUBLE_STD_MATH_FRIEND(fun) \
   friend zdouble fun(const zdouble& z);
/*!
\def CPPAD_ZDOUBLE_STD_MATH(fun)
Define a standard math function.
*/
# define CPPAD_ZDOUBLE_STD_MATH(fun)        \
   inline zdouble fun(const zdouble& z )    \
   {  return zdouble( std::fun(z.dbl_) ); }

namespace CppAD { // CPPAD_BEGIN_NAMESPACDE


/*!
Class that is like double, except that it has an absolute zero.
*/
class zdouble {
   /*!
   For zdouble objects z1, z2, and std::ostream os,
   declare the following friends:
   \code
      os << z1
      Integer(z1)
      fabs(z1)
      pow(z1, z2)
      fabs_geq(z1, z2)
      fun(z1)
   \endcode
   where fun is any of the standard math unary functions.
   */
   friend std::ostream& operator << (std::ostream &os, const zdouble& z);
   friend int Integer(const zdouble& z);
   friend zdouble pow(const zdouble& x, const zdouble& y);
   friend bool abs_geq(const zdouble& x, const zdouble& y);
   //
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(acos)
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(asin)
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(atan)
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(cos)
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(cosh)
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(exp)
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(fabs)
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(log)
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(log10)
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(sin)
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(sinh)
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(sqrt)
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(tan)
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(tanh)
   //
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(asinh)
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(acosh)
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(atanh)
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(erf)
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(erfc)
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(expm1)
   CPPAD_ZDOUBLE_STD_MATH_FRIEND(log1p)
   //
private:
   /// The value for this object
   double dbl_;
public:
   /// Default constructor
   zdouble(void)
   : dbl_()
   { }
   /// Copy constructor
   zdouble(const zdouble& z)
   : dbl_(z.dbl_)
   { }
   /// Constructor from double
   zdouble(const double& dbl)
   : dbl_(dbl)
   { }
   /// Constructor from size_t
   zdouble(const size_t& i)
   : dbl_( double(i) )
   { }
   /// Constructor from long
   zdouble(const long& i)
   : dbl_( double(i) )
   { }
   /// Constructor from int
   zdouble(const int& i)
   : dbl_( double(i) )
   { }
   //
   /// Destructor
   ~zdouble(void)
   { }
   //
   /// Assignment from zdouble
   zdouble& operator=(const zdouble& z)
   {  dbl_ = z.dbl_;
      return *this;
   }
   /// Assignment from double
   zdouble& operator=(const double& dbl)
   {  dbl_ = dbl;
      return *this;
   }
   //
   /// Normal compound assignment
   CPPAD_ZDOUBLE_NORMAL_ASSIGN_OPERATOR(+=)
   /// Normal compound assignment
   CPPAD_ZDOUBLE_NORMAL_ASSIGN_OPERATOR(-=)
   /// Normal unary operator
   CPPAD_ZDOUBLE_UNARY_OPERATOR(+)
   /// Normal unary operator
   CPPAD_ZDOUBLE_UNARY_OPERATOR(-)
   /// Normal compare operator
   CPPAD_ZDOUBLE_COMPARE_OPERATOR(==)
   /// Normal compare operator
   CPPAD_ZDOUBLE_COMPARE_OPERATOR(!=)
   /// Normal compare operator
   CPPAD_ZDOUBLE_COMPARE_OPERATOR(<=)
   /// Normal compare operator
   CPPAD_ZDOUBLE_COMPARE_OPERATOR(>=)
   /// Normal compare operator
   CPPAD_ZDOUBLE_COMPARE_OPERATOR(<)
   /// Normal compare operator
   CPPAD_ZDOUBLE_COMPARE_OPERATOR(>)
   //
   /// Normal binary arithmetic operator
   CPPAD_ZDOUBLE_NORMAL_BINARY_OPERATOR(+)
   /// Normal binary arithmetic operator
   CPPAD_ZDOUBLE_NORMAL_BINARY_OPERATOR(-)
   //
   /// Binary arithmetic * with absolute zero
   zdouble operator * (const zdouble& z) const
   {  bool zero = (dbl_ == 0.0) || (z.dbl_ == 0.0);
      return zdouble( zero ? 0.0 : (dbl_ * z.dbl_) );
   }
   /// Binary arithmetic * with absolute zero
   zdouble operator * (const double& x) const
   {  bool zero = (dbl_ == 0.0) || (x == 0.0);
      return zdouble( zero ? 0.0 : (dbl_ * x) );
   }
   /// Binary arithmetic / with absolute zero
   zdouble operator / (const zdouble& z) const
   {  bool zero = (dbl_ == 0.0);
      return zdouble( zero ? 0.0 : (dbl_ / z.dbl_) );
   }
   /// Binary arithmetic / with absolute zero
   zdouble operator / (const double& x) const
   {  bool zero = (dbl_ == 0.0);
      return zdouble( zero ? 0.0 : (dbl_ / x) );
   }
   //
   /// Compute assignmnet *= with absolute zero
   zdouble& operator *= (const zdouble& z)
   {  bool zero = (dbl_ == 0.0) || (z.dbl_ == 0.0);
      zero ? (dbl_ = 0.0) : (dbl_ *= z.dbl_);
      return *this;
   }
   /// Compute assignmnet *= with absolute zero
   zdouble& operator *= (const double& x)
   {  bool zero = (dbl_ == 0.0) || (x == 0.0);
      zero ? (dbl_ = 0.0) : (dbl_ *= x);
      return *this;
   }
   //
   /// Compute assignmnet /= with absolute zero
   zdouble& operator /= (const zdouble& z)
   {  bool zero = (dbl_ == 0.0);
      zero ? (dbl_ = 0.0) : (dbl_ /= z.dbl_);
      return *this;
   }
   /// Compute assignmnet /= with absolute zero
   zdouble& operator /= (const double& x)
   {  bool zero = (dbl_ == 0.0);
      zero ? (dbl_ = 0.0) : (dbl_ /= x);
      return *this;
   }
};
// BEGIN nan
/// Must specialize CppAD::nan because zdouble 0/0 is not nan.
template <> inline
zdouble nan<zdouble>(const zdouble& zero)
{
   return zdouble( std::numeric_limits<double>::quiet_NaN() );
}
// END nan
//
/// Normal non-member compare operator
CPPAD_ZDOUBLE_OTHER_COMPARE_OPERATOR(==, ==)
/// Normal non-member compare operator
CPPAD_ZDOUBLE_OTHER_COMPARE_OPERATOR(!=, !=)
/// Normal non-member compare operator
CPPAD_ZDOUBLE_OTHER_COMPARE_OPERATOR(<=, >=)
/// Normal non-member compare operator
CPPAD_ZDOUBLE_OTHER_COMPARE_OPERATOR(>=, <=)
/// Normal non-member compare operator
CPPAD_ZDOUBLE_OTHER_COMPARE_OPERATOR(<,  >)
/// Normal non-member compare operator
CPPAD_ZDOUBLE_OTHER_COMPARE_OPERATOR(>,  <)
//
/// Normal binary arithmetic operator
CPPAD_ZDOUBLE_OTHER_BINARY_OPERATOR(+)
/// Normal binary arithmetic operator
CPPAD_ZDOUBLE_OTHER_BINARY_OPERATOR(-)
/// Binary arithmetic operator with absolute zero
CPPAD_ZDOUBLE_OTHER_BINARY_OPERATOR(*)
/// Binary arithmetic operator with absolute zero
CPPAD_ZDOUBLE_OTHER_BINARY_OPERATOR(/)
// -------------------------------------------------------------------------
// Base type requirements
// -------------------------------------------------------------------------

/// Base type requirement: CondExpOp
inline zdouble CondExpOp(
   enum CompareOp     cop          ,
   const zdouble&       left         ,
   const zdouble&       right        ,
   const zdouble&       exp_if_true  ,
   const zdouble&       exp_if_false )
{  return CondExpTemplate(cop, left, right, exp_if_true, exp_if_false);
}

/// Base type requirement: CondExpRel
CPPAD_COND_EXP_REL(zdouble)

/// Base type requirement: EqualOpSeq
inline bool EqualOpSeq(const zdouble& x, const zdouble& y)
{  return x == y; }

/// Base type requirement: Identical
inline bool IdenticalCon(const zdouble& x)
{  return true; }
inline bool IdenticalZero(const zdouble& x)
{  return (x == 0.0); }
inline bool IdenticalOne(const zdouble& x)
{  return (x == 1.); }
inline bool IdenticalEqualCon(const zdouble& x, const zdouble& y)
{  return (x == y); }

/// Base type requirement: output operator
inline std::ostream& operator << (std::ostream &os, const zdouble& z)
{  os << z.dbl_;
   return os;
}

/// Base type requirement: Integer
inline int Integer(const zdouble& x)
{  return static_cast<int>(x.dbl_); }

/// Base type requirement: azmul
inline zdouble azmul(const zdouble& x, const zdouble& y)
{  return x * y; }

/// Base type requirement: Ordered
inline bool GreaterThanZero(const zdouble& x)
{  return x > 0.0; }
inline bool GreaterThanOrZero(const zdouble& x)
{  return x >= 0.0; }
inline bool LessThanZero(const zdouble& x)
{  return x < 0.0; }
inline bool LessThanOrZero(const zdouble& x)
{  return x <= 0.0; }
inline bool abs_geq(const zdouble& x, const zdouble& y)
{  return std::fabs(x.dbl_) >= std::fabs(y.dbl_); }

/// Normal standard math function
CPPAD_ZDOUBLE_STD_MATH(acos)
/// Normal standard math function
CPPAD_ZDOUBLE_STD_MATH(asin)
/// Normal standard math function
CPPAD_ZDOUBLE_STD_MATH(atan)
/// Normal standard math function
CPPAD_ZDOUBLE_STD_MATH(cos)
/// Normal standard math function
CPPAD_ZDOUBLE_STD_MATH(cosh)
/// Normal standard math function
CPPAD_ZDOUBLE_STD_MATH(exp)
/// Normal standard math function
CPPAD_ZDOUBLE_STD_MATH(fabs)
/// Normal standard math function
CPPAD_ZDOUBLE_STD_MATH(log)
/// Normal standard math function
CPPAD_ZDOUBLE_STD_MATH(log10)
/// Normal standard math function
CPPAD_ZDOUBLE_STD_MATH(sin)
/// Normal standard math function
CPPAD_ZDOUBLE_STD_MATH(sinh)
/// Normal standard math function
CPPAD_ZDOUBLE_STD_MATH(sqrt)
/// Normal standard math function
CPPAD_ZDOUBLE_STD_MATH(tan)
/// Normal standard math function
CPPAD_ZDOUBLE_STD_MATH(tanh)
//
/// C++2011 standard math function
CPPAD_ZDOUBLE_STD_MATH(asinh)
/// C++2011 standard math function
CPPAD_ZDOUBLE_STD_MATH(acosh)
/// C++2011 standard math function
CPPAD_ZDOUBLE_STD_MATH(atanh)
/// C++2011 standard math function
CPPAD_ZDOUBLE_STD_MATH(erf)
/// C++2011 standard math function
CPPAD_ZDOUBLE_STD_MATH(erfc)
/// C++2011 standard math function
CPPAD_ZDOUBLE_STD_MATH(expm1)
/// C++2011 standard math function
CPPAD_ZDOUBLE_STD_MATH(log1p)

/// Base type requirement: abs
inline zdouble abs(const zdouble& x)
{  return fabs(x); }

/// Base type requirement: sign
inline zdouble sign(const zdouble& x)
{  if( x > 0.0 )
      return zdouble(1.);
   if( x == 0.0 )
      return zdouble(0.0);
   return zdouble(-1.);
}

/// Base type requirement: pow
inline zdouble pow(const zdouble& x, const zdouble& y)
{ return std::pow(x.dbl_, y.dbl_); }

/// Base type requirement: limits
CPPAD_NUMERIC_LIMITS(double, zdouble)

} // CPPAD_END_NAMESPACE

/// undef all macros defined in this file
# undef CPPAD_ZDOUBLE_NORMAL_ASSIGN_OPERATOR
# undef CPPAD_ZDOUBLE_UNARY_OPERATOR
# undef CPPAD_ZDOUBLE_NORMAL_BINARY_OPERATOR
# undef CPPAD_ZDOUBLE_COMPARE_OPERATOR
# undef CPPAD_ZDOUBLE_OTHER_BINARY_OPERATOR
# undef CPPAD_ZDOUBLE_OTHER_COMPARE_OPERATOR
# undef CPPAD_ZDOUBLE_STD_MATH_FRIEND
# undef CPPAD_ZDOUBLE_STD_MATH

# endif
