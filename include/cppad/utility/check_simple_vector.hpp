# ifndef CPPAD_UTILITY_CHECK_SIMPLE_VECTOR_HPP
# define CPPAD_UTILITY_CHECK_SIMPLE_VECTOR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin CheckSimpleVector}
{xrst_spell
   preforms
}

Check Simple Vector Concept
###########################

Syntax
******

   # ``include <cppad/utility/check_simple_vector.hpp>``

``CheckSimpleVector`` < *Scalar* , *Vector* >()

``CheckSimpleVector`` < *Scalar* , *Vector* >( *x* , *y* )

Purpose
*******
Preforms compile and run time checks that the type specified
by *Vector* satisfies all the requirements for
a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
*Scalar* .
If a requirement is not satisfied,
a an error message makes it clear what condition is not satisfied.

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
The routine :ref:`thread_alloc::parallel_setup<ta_parallel_setup-name>`
must be called before it
can be used in :ref:`parallel<ta_in_parallel-name>` mode.

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

namespace CppAD {

# ifdef NDEBUG
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
   void CheckSimpleVector(const Scalar& x, const Scalar& y)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
      static size_t count;
      if( count > 0  )
         return;
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
      x = 0;
      y = 1;

      CheckSimpleVector<Scalar, Vector>(x, y);
   }

# endif

} // end namespace CppAD

# endif
