# ifndef CPPAD_UTILITY_CHECK_SIMPLE_VECTOR_HPP
# define CPPAD_UTILITY_CHECK_SIMPLE_VECTOR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin CheckSimpleVector}

Check Simple Vector Concept
###########################

Syntax
******
| # ``include <cppad/utility/check_simple_vector.hpp>``
| ``CheckSimpleVector`` < *Scalar* , *Vector* >()
| ``CheckSimpleVector`` < *Scalar* , *Vector* >( *x* , *y* )

Purpose
*******
Preforms compile and run time checks that the type specified
by *Vector* satisfies all the requirements for
a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
*Scalar* .
If a requirement is not satisfied,
a an error message makes it clear what condition is not satisfied.

Vector
******
is the vector type we are checking.

Scalar
******
is the type corresponding to the elements of an *Vector* .

x, y
****
If the arguments *x* and *y* are present,
they have prototype

| |tab| ``const`` *Scalar* & *x*
| |tab| ``const`` *Scalar* & *y*

In addition, the check

   *x* == *x*

will return the boolean value ``true`` , and

   *x* == *y*

will return ``false`` .

Restrictions
************
If the arguments *x* and *y* are not present,
the following extra assumption is made by ``CheckSimpleVector`` :
If *x* is a *Scalar* object

| |tab| *x*  = 0
| |tab| *y*  = 1

assigns values to the objects *x* and *y* .
In addition,
*x* == *x* would return the boolean value ``true`` and
*x* == *y* would return ``false`` .

Include
*******
The file ``cppad/utility/check_simple_vector.hpp``
is included by ``cppad/cppad.hpp``
but it can also be included separately with out the rest
if the CppAD include files.

Parallel Mode
*************
This routine must be called before entering parallel mode
because it has static variables that must be initialized.
If it's first call is not in  parallel mode, and NDEBUG is not defined,
you will get an assertion. Running in the debugger and going to the
stack frame where CheckSimpleVector is called may help you determine
what the value of *Scalar* and *Vector* need to be initialized.

Example
*******
{xrst_toc_hidden
   example/utility/check_simple_vector.cpp
}
The file :ref:`check_simple_vector.cpp-name`
contains an example and test of this function where *S*
is the same as *T* .
The comments in this example suggest a way to change the example
so *S* is not the same as *T* .

{xrst_end CheckSimpleVector}
---------------------------------------------------------------------------
*/

# include <cstddef>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/define.hpp>
# include <cppad/utility/thread_alloc.hpp>

// CPPAD_CHECK_SIMPLE_VECTOR
# ifndef NDEBUG
# define CPPAD_CHECK_SIMPLE_VECTOR 1
# elif CPPAD_DEBUG_AND_RELEASE
# define CPPAD_CHECK_SIMPLE_VECTOR 1
# else
# define CPPAD_CHECK_SIMPLE_VECTOR 0
# endif

namespace CppAD {

# if ! CPPAD_CHECK_SIMPLE_VECTOR
   template <class Scalar, class Vector>
   inline void CheckSimpleVector(const Scalar& x, const Scalar& y)
   { }
   template <class Scalar, class Vector>
   inline void CheckSimpleVector(void)
   { }
# else
   template <class S, class T>
   struct ok_if_S_same_as_T { };

   template <class T>
   struct ok_if_S_same_as_T<T,T> { T value; };

   template <class Scalar, class Vector>
   inline void CheckSimpleVector(const Scalar& x, const Scalar& y)
   {  //
      // count
      static size_t count = 0;
      if( count > 0  )
         return;
      CPPAD_ASSERT_KNOWN(
         ! CppAD::thread_alloc::in_parallel() ,
         "In parallel mode and CheckSimpleVector was not previously called\n"
         "with this Scalar and Vector type; see the heading\n"
         "Parallel Mode in the CheckSimpleVector documentation."
      );
      count++;

      // value_type must be type of elements of Vector
      typedef typename Vector::value_type value_type;

      // check that elements of Vector have type Scalar
      struct ok_if_S_same_as_T<Scalar, value_type> x_copy;
      x_copy.value = x;

      // check default constructor
      Vector d;

      // size member function
      CPPAD_ASSERT_KNOWN(
         d.size() == 0,
         "default construtor result does not have size zero"
      );

      // resize to same size as other vectors in test
      d.resize(1);

      // check sizing constructor
      Vector s(1);

      // check element assignment
      s[0] = y;
      CPPAD_ASSERT_KNOWN(
         s[0] == y,
         "element assignment failed"
      );

      // check copy constructor
      s[0] = x_copy.value;
      const Vector c(s);
      s[0] = y;
      CPPAD_ASSERT_KNOWN(
         c[0] == x,
         "copy constructor is shallow"
      );

      // vector assignment operator
      d[0] = x;
      s    = d;
      s[0] = y;
      CPPAD_ASSERT_KNOWN(
         d[0] == x,
         "assignment operator is shallow"
      );

      // element access, right side const
      // element assignment, left side not const
      d[0] = c[0];
      CPPAD_ASSERT_KNOWN(
         d[0] == x,
         "element assignment from const failed"
      );
   }
   template <class Scalar, class Vector>
   void CheckSimpleVector(void)
   {  Scalar x;
      Scalar y;

      // use assignment and not constructor
      x = Scalar(0);
      y = Scalar(1);

      CheckSimpleVector<Scalar, Vector>(x, y);
   }

# endif

} // end namespace CppAD

# undef CPPAD_CHECK_SIMPLE_VECTOR
# endif
