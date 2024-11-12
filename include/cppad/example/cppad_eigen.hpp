# ifndef CPPAD_EXAMPLE_CPPAD_EIGEN_HPP
# define CPPAD_EXAMPLE_CPPAD_EIGEN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin cppad_eigen.hpp}
Enable Use of Eigen Linear Algebra Package with CppAD
#####################################################

Syntax
******
| # ``include <cppad/example/cppad_eigen.hpp>``
{xrst_toc_hidden
   include/cppad/example/eigen_plugin.hpp
   example/general/eigen_array.cpp
   example/general/eigen_det.cpp
}

Purpose
*******
Enables the use of the :ref:`eigen-name`
linear algebra package with the type *AD* < ``Base`` > ; see
`custom scalar types <https://eigen.tuxfamily.org/dox/TopicCustomizing_CustomScalar.html>`_.

Example
*******
The files :ref:`eigen_array.cpp-name` and :ref:`eigen_det.cpp-name`
contain an example and test of this include file.
They return true if they succeed and false otherwise.

CppAD Declarations
******************
First declare some items that are defined by cppad.hpp:
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   // AD<Base>
   template <class Base> class AD;
   // numeric_limits<Float>
   template <class Float>  class numeric_limits;
}
/* {xrst_code}
{xrst_spell_on}

std Declarations
****************
Next declare some template specializations in std namespace:
{xrst_spell_off}
{xrst_code cpp} */
namespace std {
   template <class Base> bool isinf(const CppAD::AD<Base> &x);
   template <class Base> bool isfinite(const CppAD::AD<Base> &x);
   template <class Base> bool isnan(const CppAD::AD<Base> &x);
}
/* {xrst_code}
{xrst_spell_on}

Include Eigen/Core
******************
Next define the eigen plugin and then include Eigen/Core:
{xrst_spell_off}
{xrst_code cpp} */

# define EIGEN_MATRIXBASE_PLUGIN <cppad/example/eigen_plugin.hpp>
# include <Eigen/Core>
/* {xrst_code}
{xrst_spell_on}

Eigen NumTraits
***************
Eigen needs the following definitions, in the Eigen namespace,
to work properly with ``AD`` < *Base* > scalars:
{xrst_spell_off}
{xrst_code cpp} */
namespace Eigen {
   template <class Base> struct NumTraits< CppAD::AD<Base> >
   {  // type that corresponds to the real part of an AD<Base> value
      typedef CppAD::AD<Base>   Real;
      // type for AD<Base> operations that result in non-integer values
      typedef CppAD::AD<Base>   NonInteger;
      //  type to use for numeric literals such as "2" or "0.5".
      typedef CppAD::AD<Base>   Literal;
      // type for nested value inside an AD<Base> expression tree
      typedef CppAD::AD<Base>   Nested;

      enum {
         // does not support complex Base types
         IsComplex             = 0 ,
         // does not support integer Base types
         IsInteger             = 0 ,
         // only support signed Base types
         IsSigned              = 1 ,
         // must initialize an AD<Base> object
         RequireInitialization = 1 ,
         // computational cost of the corresponding operations
         ReadCost              = 1 ,
         AddCost               = 2 ,
         MulCost               = 2
      };

      // machine epsilon with type of real part of x
      // (use assumption that Base is not complex)
      static CppAD::AD<Base> epsilon(void)
      {  return CppAD::numeric_limits< CppAD::AD<Base> >::epsilon(); }

      // relaxed version of machine epsilon for comparison of different
      // operations that should result in the same value
      static CppAD::AD<Base> dummy_precision(void)
      {  return 100. *
            CppAD::numeric_limits< CppAD::AD<Base> >::epsilon();
      }

      // minimum normalized positive value
      static CppAD::AD<Base> lowest(void)
      {  return CppAD::numeric_limits< CppAD::AD<Base> >::min(); }

      // maximum finite value
      static CppAD::AD<Base> highest(void)
      {  return CppAD::numeric_limits< CppAD::AD<Base> >::max(); }

      // number of decimal digits that can be represented without change.
      static int digits10(void)
      {  return CppAD::numeric_limits< CppAD::AD<Base> >::digits10; }

      // number of decimal digits necessary to uniquely represent values.
      static int max_digits10(void)
      {  return CppAD::numeric_limits< CppAD::AD<Base> >::max_digits10; }

      // not a number
      static CppAD::AD<Base> quiet_NaN(void)
      {  return CppAD::numeric_limits< CppAD::AD<Base> >::quiet_NaN(); }

      // positive infinite value
      static CppAD::AD<Base> infinity(void)
      {  return CppAD::numeric_limits< CppAD::AD<Base> >::infinity(); }
   };
}
/* {xrst_code}
{xrst_spell_on}

Eigen ScalarBinaryOpTraits
**************************
{xrst_spell_off}
{xrst_code cpp} */
namespace Eigen {
   // Inform Eigen that a binary operations between Base and AD<Base>
   // are allowed and thate the return type is AD<Base>
   template<typename Base, typename BinOp>
   struct ScalarBinaryOpTraits<CppAD::AD<Base>, Base, BinOp>{
      typedef CppAD::AD<Base> ReturnType;
   };
   template<typename Base, typename BinOp>
   struct ScalarBinaryOpTraits<Base, CppAD::AD<Base>, BinOp>
   {
      typedef CppAD::AD<Base> ReturnType;
   };
}
/* {xrst_code}
{xrst_spell_on}

CppAD Namespace
***************
Eigen needs the following definitions, in the CppAD namespace,
to work properly with ``AD`` < *Base* > scalars:
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
      // functions that return references
      template <class Base> const AD<Base>& conj(const AD<Base>& x)
      {  return x; }
      template <class Base> const AD<Base>& real(const AD<Base>& x)
      {  return x; }

      // functions that return values (note abs is defined by cppad.hpp)
      template <class Base> AD<Base> imag(const AD<Base>& x)
      {  return CppAD::AD<Base>(0.); }
      template <class Base> AD<Base> abs2(const AD<Base>& x)
      {  return x * x; }
}
/* {xrst_code}
{xrst_spell_on}

eigen_vector
************
The class ``CppAD::eigen_vector`` is a wrapper for Eigen column vectors
so that they are :ref:`simple vectors<SimpleVector-name>` .
To be specific, it converts ``Eigen::Index`` arguments and
return values to ``size_t`` .
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   template <class Scalar>
   class eigen_vector : public Eigen::Matrix<Scalar, Eigen::Dynamic, 1> {
   private:
      // base_class
      typedef Eigen::Matrix<Scalar, Eigen::Dynamic, 1> base_class;
   public:
      // constructor
      eigen_vector(size_t n) : base_class( Eigen::Index(n) )
      { }
      eigen_vector(void) : base_class()
      { }
      // operator[]
      Scalar& operator[](size_t i)
      {  return base_class::operator[]( Eigen::Index(i) ); }
      const Scalar& operator[](size_t i) const
      {  return base_class::operator[]( Eigen::Index(i) ); }
      // size
      size_t size(void) const
      {  return size_t( base_class::size() ); }
      // resize
      void resize(size_t n)
      {  base_class::resize( Eigen::Index(n) ); }
   };
}
/* {xrst_code}
{xrst_spell_on}

Include cppad.hpp
*****************
{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/cppad.hpp>
/* {xrst_code}
{xrst_spell_on}

std Definitions
***************
The definitions below use cppad.hpp.
Note that :ref:`Value-name` function can only be used with a
:ref:`constant parameter<glossary@Parameter@Constant>` argument.
{xrst_spell_off}
{xrst_code cpp} */
namespace std {
   template <class Base> bool isinf(const CppAD::AD<Base> &x)
   {  return isinf(CppAD::Value( CppAD::Var2Par(x) ) ); }

   template <class Base> bool isfinite(const CppAD::AD<Base> &x)
   {  return isfinite(CppAD::Value( CppAD::Var2Par(x) ) ); }

   template <class Base> bool isnan(const CppAD::AD<Base> &x)
   {  return CppAD::isnan(CppAD::Value( CppAD::Var2Par(x) ) ); }
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end cppad_eigen.hpp}
*/
# endif
