# ifndef CPPAD_EXAMPLE_GENERAL_BASE_ALLOC_HPP
# define CPPAD_EXAMPLE_GENERAL_BASE_ALLOC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin base_alloc.hpp}
{xrst_spell
   allocates
   apposed
   invocations
   isnan
}
Example AD<Base> Where Base Constructor Allocates Memory
########################################################

Purpose
*******
Demonstrate use of ``AD`` < *Base* >
where memory is allocated for each element of the type *Base* .
In addition, this is a complete example where all the
:ref:`required Base<base_require-name>` type
operations are defined (as apposed to other examples where
some of the operations for the Base type are already defined).

Include File
************
This file uses some of the definitions in :ref:`base_require-name`
and :ref:`thread_alloc-name` .
{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/base_require.hpp>
# include <cppad/utility/thread_alloc.hpp>
/* {xrst_code}
{xrst_spell_on}

Compound Assignment Macro
*************************
This macro is used for the
``base_alloc`` compound assignment operators; to be specific,
used with *op* equal to
``+=`` ,
``-=`` ,
``*=`` ,
``/=`` .
{xrst_spell_off}
{xrst_code cpp} */
# define BASE_ALLOC_ASSIGN_OPERATOR(op) \
   void operator op (const base_alloc& x) \
   {  *ptrdbl_ op *x.ptrdbl_; }
/* {xrst_code}
{xrst_spell_on}

Binary Operator Macro
*********************
This macro is used for the
``base_alloc`` binary operators (as member functions); to be specific,
used with *op* equal to
``+`` ,
``-`` ,
``*`` ,
``/`` .
{xrst_spell_off}
{xrst_code cpp} */
# define BASE_ALLOC_BINARY_OPERATOR(op) const \
   base_alloc operator op (const base_alloc& x) const \
   {  base_alloc result; \
      double   dbl = *ptrdbl_; \
      double x_dbl = *x.ptrdbl_; \
      *result.ptrdbl_ = dbl op x_dbl; \
      return result; \
   }
/* {xrst_code}
{xrst_spell_on}

Boolean Operator Macro
**********************
This macro can be used for the
``base_alloc`` binary operators that have a
``bool`` result; to be specific,
used with *op* equal to
``==`` ,
``!=`` ,
``<`` ,
``<=`` ,
``>=`` , and
``>`` ,
{xrst_spell_off}
{xrst_code cpp} */
# define BASE_ALLOC_BOOL_OPERATOR(op) const \
   bool operator op (const base_alloc& x) const \
   {  double   dbl = *ptrdbl_; \
      double x_dbl = *x.ptrdbl_; \
      return dbl op x_dbl; \
   }
/* {xrst_code}
{xrst_spell_on}

Class Definition
****************
The following example class
defines the necessary :ref:`base_member-name` functions.
It is made more complicated by storing a pointer to a ``double``
instead of the ``double`` value itself.
{xrst_spell_off}
{xrst_code cpp} */

class base_alloc {
public:
   double* ptrdbl_;

   base_alloc(void)
   {  size_t cap;
      void* v  = CppAD::thread_alloc::get_memory(sizeof(double), cap);
      ptrdbl_  = static_cast<double*>(v);
   }
   base_alloc(double dbl)
   {  size_t cap;
      void *v  = CppAD::thread_alloc::get_memory(sizeof(double), cap);
      ptrdbl_  = static_cast<double*>(v);
      *ptrdbl_ = dbl;
   }
   base_alloc(const base_alloc& x)
   {  size_t cap;
      void *v  = CppAD::thread_alloc::get_memory(sizeof(double), cap);
      ptrdbl_  = static_cast<double*>(v);
      *ptrdbl_ = *x.ptrdbl_;
   }
   ~base_alloc(void)
   {  void* v  = static_cast<void*>(ptrdbl_);
      CppAD::thread_alloc::return_memory(v);
   }
   base_alloc operator-(void) const
   {  base_alloc result;
      *result.ptrdbl_ = - *ptrdbl_;
      return result;
   }
   base_alloc operator+(void) const
   {  return *this; }
   void operator=(const base_alloc& x)
   {  *ptrdbl_ = *x.ptrdbl_; }
   BASE_ALLOC_ASSIGN_OPERATOR(+=)
   BASE_ALLOC_ASSIGN_OPERATOR(-=)
   BASE_ALLOC_ASSIGN_OPERATOR(*=)
   BASE_ALLOC_ASSIGN_OPERATOR(/=)
   BASE_ALLOC_BINARY_OPERATOR(+)
   BASE_ALLOC_BINARY_OPERATOR(-)
   BASE_ALLOC_BINARY_OPERATOR(*)
   BASE_ALLOC_BINARY_OPERATOR(/)
   BASE_ALLOC_BOOL_OPERATOR(==)
   BASE_ALLOC_BOOL_OPERATOR(!=)
   // The <= operator is not necessary for the base type requirements
   // (needed so we can use NearEqual with base_alloc arguments).
   BASE_ALLOC_BOOL_OPERATOR(<=)
};
/* {xrst_code}
{xrst_spell_on}

CondExpOp
*********
The type ``base_alloc`` does not use :ref:`CondExp-name` operations.
Hence its ``CondExpOp`` function is defined by
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline base_alloc CondExpOp(
      enum CompareOp     cop          ,
      const base_alloc&       left         ,
      const base_alloc&       right        ,
      const base_alloc&       exp_if_true  ,
      const base_alloc&       exp_if_false )
   {  // not used
      assert(false);

      // to void compiler error
      return base_alloc();
   }
}
/* {xrst_code}
{xrst_spell_on}

CondExpRel
**********
The :ref:`CPPAD_COND_EXP_REL<base_cond_exp@CondExpRel>` macro invocation
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   CPPAD_COND_EXP_REL(base_alloc)
}
/* {xrst_code}
{xrst_spell_on}
uses ``CondExpOp`` above to
define ``CondExp`` *Rel* for ``base_alloc`` arguments
and *Rel* equal to
``Lt`` , ``Le`` , ``Eq`` , ``Ge`` , and ``Gt`` .

EqualOpSeq
**********
The type ``base_alloc`` is simple (in this respect) and so we define
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline bool EqualOpSeq(const base_alloc& x, const base_alloc& y)
   {  return *x.ptrdbl_ == *y.ptrdbl_; }
}
/* {xrst_code}
{xrst_spell_on}

Identical
*********
The type ``base_alloc`` is simple (in this respect) and so we define
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline bool IdenticalCon(const base_alloc& x)
   {  return true; }
   inline bool IdenticalZero(const base_alloc& x)
   {  return (*x.ptrdbl_ == 0.0); }
   inline bool IdenticalOne(const base_alloc& x)
   {  return (*x.ptrdbl_ == 1.0); }
   inline bool IdenticalEqualCon(const base_alloc& x, const base_alloc& y)
   {  return (*x.ptrdbl_ == *y.ptrdbl_); }
}
/* {xrst_code}
{xrst_spell_on}

Output Operator
***************
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline std::ostream& operator << (std::ostream &os, const base_alloc& x)
   {  os << *x.ptrdbl_;
      return os;
   }
}
/* {xrst_code}
{xrst_spell_on}

Integer
*******
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline int Integer(const base_alloc& x)
   {  return static_cast<int>(*x.ptrdbl_); }
}
/* {xrst_code}
{xrst_spell_on}

azmul
*****
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   CPPAD_AZMUL( base_alloc )
}
/* {xrst_code}
{xrst_spell_on}

Ordered
*******
The ``base_alloc`` type supports ordered comparisons
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline bool GreaterThanZero(const base_alloc& x)
   {  return *x.ptrdbl_ > 0.0; }
   inline bool GreaterThanOrZero(const base_alloc& x)
   {  return *x.ptrdbl_ >= 0.0; }
   inline bool LessThanZero(const base_alloc& x)
   {  return *x.ptrdbl_ < 0.0; }
   inline bool LessThanOrZero(const base_alloc& x)
   {  return *x.ptrdbl_ <= 0.f; }
   inline bool abs_geq(const base_alloc& x, const base_alloc& y)
   {  return std::fabs(*x.ptrdbl_) >= std::fabs(*y.ptrdbl_); }
}
/* {xrst_code}
{xrst_spell_on}

Unary Standard Math
*******************
The macro
:ref:`base_std_math@CPPAD_STANDARD_MATH_UNARY`
would not work with the type ``base_alloc`` so we define
a special macro for this type:
{xrst_spell_off}
{xrst_code cpp} */
# define BASE_ALLOC_STD_MATH(fun) \
   inline base_alloc fun (const base_alloc& x) \
   { return   std::fun(*x.ptrdbl_); }
/* {xrst_code}
{xrst_spell_on}
The following invocations of the macro above define the
:ref:`base_std_math@Unary Standard Math` functions
(except for ``abs`` ):
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   BASE_ALLOC_STD_MATH(acos)
   BASE_ALLOC_STD_MATH(acosh)
   BASE_ALLOC_STD_MATH(asin)
   BASE_ALLOC_STD_MATH(asinh)
   BASE_ALLOC_STD_MATH(atan)
   BASE_ALLOC_STD_MATH(atanh)
   BASE_ALLOC_STD_MATH(cos)
   BASE_ALLOC_STD_MATH(cosh)
   BASE_ALLOC_STD_MATH(erf)
   BASE_ALLOC_STD_MATH(erfc)
   BASE_ALLOC_STD_MATH(exp)
   BASE_ALLOC_STD_MATH(expm1)
   BASE_ALLOC_STD_MATH(fabs)
   BASE_ALLOC_STD_MATH(log)
   BASE_ALLOC_STD_MATH(log1p)
   BASE_ALLOC_STD_MATH(log10)
   BASE_ALLOC_STD_MATH(sin)
   BASE_ALLOC_STD_MATH(sinh)
   BASE_ALLOC_STD_MATH(sqrt)
   BASE_ALLOC_STD_MATH(tan)
   BASE_ALLOC_STD_MATH(tanh)
}
/* {xrst_code}
{xrst_spell_on}
The absolute value function is special because it ``std`` name is
``fabs``
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline base_alloc abs(const base_alloc& x)
   {  return fabs(*x.ptrdbl_); }
}
/* {xrst_code}
{xrst_spell_on}
The isnan function is special because it returns a bool
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline bool isnan(const base_alloc& x)
   {  return *x.ptrdbl_ != *x.ptrdbl_; }
}
/* {xrst_code}
{xrst_spell_on}

sign
****
The following defines the ``CppAD::sign`` function that
is required to use ``AD<base_alloc>`` :
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline base_alloc sign(const base_alloc& x)
   {  if( *x.ptrdbl_ > 0.0 )
         return 1.0;
      if( *x.ptrdbl_ == 0.0 )
         return 0.0;
      return -1.0;
   }
}
/* {xrst_code}
{xrst_spell_on}

pow
***
The following defines a ``CppAD::pow`` function that
is required to use ``AD<base_alloc>`` :
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline base_alloc pow(const base_alloc& x, const base_alloc& y)
   { return std::pow(*x.ptrdbl_, *y.ptrdbl_); }
}
/* {xrst_code}
{xrst_spell_on}

numeric_limits
**************
The following defines the CppAD :ref:`numeric_limits-name`
for the type ``base_alloc`` :
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   CPPAD_NUMERIC_LIMITS(double, base_alloc)
}
/* {xrst_code}
{xrst_spell_on}

to_string
*********
The following defines the CppAD :ref:`to_string-name` function
for the type ``base_alloc`` :
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   CPPAD_TO_STRING(base_alloc)
}
/* {xrst_code}
{xrst_spell_on}

hash_code
*********
The :ref:`base_hash@Default` hashing function does
not work well for this case because two different pointers can
have the same value.
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline unsigned short hash_code(const base_alloc& x)
   {  unsigned short code = 0;
      if( *x.ptrdbl_ == 0.0 )
         return code;
      double log_x = log( std::fabs( *x.ptrdbl_ ) );
      // assume log( std::numeric_limits<double>::max() ) is near 700
      code = static_cast<unsigned short>(
         (CPPAD_HASH_TABLE_SIZE / 700 + 1) * log_x
      );
      code = code % CPPAD_HASH_TABLE_SIZE;
      return code;
   }
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end base_alloc.hpp}
*/
# endif
