// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin cppad_vector.cpp}

CppAD::vector Template Class: Example and Test
##############################################

Purpose
*******
This is an example and test of the features of the
:ref:`CppAD_vector-name` class that are not included in the
:ref:`SimpleVector-name` concept.

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end cppad_vector.cpp}
*/
// BEGIN C++

# include <cppad/utility/vector.hpp>
# include <cppad/utility/error_handler.hpp>
# include <cppad/utility/check_simple_vector.hpp>
# include <sstream> // sstream and string are used to test output operation
# include <string>
# include <algorithm>

namespace {
   void myhandler(
      bool known       ,
      int  line        ,
      const char *file ,
      const char *exp  ,
      const char *msg  )
   {  // error handler must not return, so throw an exception
      throw std::string( file );
   }
}

bool CppAD_vector(void)
{  bool ok = true;
   using CppAD::vector;     // so can use vector instead of CppAD::vector
   typedef double Scalar;   // change double to test other types

   // check Simple Vector specifications
   CppAD::CheckSimpleVector< Scalar, vector<Scalar> >();

   // check constructor with size_t, with int, and with value
   size_t two_s = 2;
   int    two_i = 2;
   Scalar value = 5.0;
   vector<Scalar> vec(2), other(two_s), another(two_i, value);
   ok &= another[0] == 5.0;
   ok &= another[1] == 5.0;

   // check resize with size_t and with int
   vec.resize(2);
   other.resize(two_s);
   another.resize(two_i);

   // assignment returns reference for use in other assignments
   another[0] = Scalar(1);
   another[1] = Scalar(2);
   vec = other = another;
   for(size_t i = 0; i < 2; ++i)
   {  ok &= vec[i] == other[i];
      ok &= vec[i] == another[i];
   }

   // operator ==
   ok &= vec == other;
   ok &= other == another;

   // initializer constructor
   vector<Scalar> yet_another = { 1.0, 3.0};
   ok &= yet_another.size() == 2;
   ok &= yet_another[0] == 1.0;
   ok &= yet_another[1] == 3.0;

   // operator <=, >=
   ok &= vec <= yet_another;
   ok &= yet_another >= vec;

   // test of output
   std::string        correct= "{ 1, 2 }";
   std::string        str;
   std::ostringstream buf;
   buf << vec;
   str = buf.str();
   ok &= (str == correct);

   // swap
   other[0] = vec[0] + 1;
   vec.swap(other);
   ok  &= vec[0] == other[0] + 1;

   // clear
   vec.clear();
   ok &= vec.size() == 0;
   ok &= vec.capacity() == 0;

   // push_back scalar and changes in capacity
   size_t n = 100;
   size_t old_capacity = vec.capacity();
   for(size_t i = 0; i < n; i++)
   {  vec.push_back( Scalar(n - i) );
      ok &= (i+1) == vec.size();
      ok &= i < vec.capacity();
      ok &= old_capacity == vec.capacity() || i == old_capacity;
      old_capacity = vec.capacity();
   }
   for(size_t i = 0; i < n; i++)
      ok &= ( vec[i] == Scalar(n - i) );

   // test of push_vector
   vec.push_vector(vec);
   ok &= (vec.size() == 2 * n);
   for(size_t i = 0; i < n; i++)
   {  ok &= vec[i]      == Scalar(n - i);
      ok &= vec[i + n]  == Scalar(n - i);
   }

   // resize perserves elements when new size less than capacity
   ok &= n < vec.capacity();
   vec.resize(n);
   for(size_t i = 0; i < n; i++)
      ok &= vec[i] == Scalar(n - i);

   // vector assignment OK no matter what target size was before assignment
   other[0] = vec[0] + 1;
   ok &= other.size() < vec.size();
   other    = vec;
   ok &= other.size() == vec.size();
   for(size_t i = 0; i < vec.size(); i++)
      ok &= other[i] == vec[i];

   // create a const vector equal to vec
   const vector<Scalar> cvec = vec;

   // sort of vec (will reverse order of elements for this case)
# ifndef _MSC_VER
   // 2DO: Determine why this test fails with Visual Studio 2019
   std::sort(vec.begin(), vec.end());
   for(size_t i = 0; i < n ; ++i)
      ok &= vec[i] == Scalar(i + 1);

   // use data pointer to sort using pointers instead of iterators
   std::sort(other.data(), other.data() + other.size());
   for(size_t i = 0; i < n ; ++i)
      ok &= other[i] == Scalar(i + 1);
# else
   for(size_t i = 0; i < n ; ++i)
      vec[i] = Scalar(i + 1);
# endif

   // test direct use of iterator and const_iterator
   typedef vector<Scalar>::iterator       iterator;
   typedef vector<Scalar>::const_iterator const_iterator;
   iterator        itr = vec.begin(); // increasing order
   const_iterator citr = cvec.end();  // decreasing order
   while( itr != vec.end() )
   {  --citr;
      ok &= *itr == *citr;
      ++itr;
   }
   // conversion from iterator to const_iterator
   citr = vec.begin();
   ok  &= *citr == vec[0];

   // test use of [] operator with const_itr
   for(size_t i = 0; i < n; ++i)
      ok &= citr[i] == vec[i];

   // test use of [] operator with iterator
   itr = vec.begin();
   for(size_t i = 0; i < n; ++i)
      itr[i] = Scalar(i + 1);

   // Replace the default CppAD error handler with myhandler (defined above).
   // This replacement is in effect until info drops out of scope.
   CppAD::ErrorHandler info(myhandler);

# ifndef NDEBUG
   // -----------------------------------------------------------------------
   // check that iterator access out of range generates an error
   ok  &= *itr == Scalar(1);  // this access OK
   bool detected_error = false;
   try
   {  vec.clear();
      // The iterator knows that the vector has changed and that
      // this access is no longer valid
      *itr;
   }
   catch(const std::string& file)
   {  // This location for the error is not part of user API and may change
      size_t pos     = file.find("/cppad_vector_itr.hpp");
      ok            &=  pos != std::string::npos;
      detected_error = true;
   }
   ok &= detected_error;
   // -----------------------------------------------------------------------
# endif

   return ok;
}

// END C++
