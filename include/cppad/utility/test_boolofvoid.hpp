# ifndef CPPAD_UTILITY_TEST_BOOLOFVOID_HPP
# define CPPAD_UTILITY_TEST_BOOLOFVOID_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin test_boolofvoid}

Object that Runs a Group of Tests
#################################

Syntax
******
| # ``include <cppad/utility/test_boolofvoid.hpp>``
| ``test_boolofvoid`` *Run* ( *group* , *width* )
| *Run* ( *test* , *name* )
| *ok* = *Run* . ``summary`` ( *memory_ok* )

Purpose
*******
The object *Run* is used to run a group of tests functions
and report the results on standard output.

group
*****
The argument has prototype

   ``const std::string&`` *group*

It is the name for this group of tests.

width
*****
The argument has prototype

   ``size_t`` *width*

It is the number of columns used to display the name of each test.
It must be greater than the maximum number of characters in a test name.

test
****
The argument has prototype

   ``bool`` *test* ( ``void`` )

It is a function that returns true (when the test passes) and false
otherwise.

name
****
The argument has prototype

   ``const std::string&`` *name*

It is the name for the corresponding *test* .

memory_ok
*********
The argument has prototype

   ``bool`` *memory_ok*

It is false if a memory leak is detected (and true otherwise).

ok
**
This is true if all of the tests pass (including the memory leak test),
otherwise it is false.

Example
*******
See any of the main programs in the example directory; e.g.,
``example/ipopt_solve.cpp`` .

{xrst_end test_boolofvoid}
*/

# include <cppad/core/cppad_assert.hpp>
# include <string>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/// One class object is used to run a group of tests
class test_boolofvoid {
private:
   /// name for the group of test this object will run
   const std::string group_;
   /// number of characters used to display the name for each indiviual test
   /// (must be larger than the number of characters in name for each test)
   const size_t      width_;
   /// number of tests that have passed
   size_t            n_ok_;
   /// number of tests that have failed
   size_t            n_error_;

public:
   /// ctor
   test_boolofvoid(const std::string& group, size_t width) :
   group_(group) ,
   width_(width) ,
   n_ok_(0)      ,
   n_error_(0)
   {  std::cout << "Begin test group " << group_ << std::endl; }
   /// destructor
   ~test_boolofvoid(void)
   {  std::cout << "End test group " << group_ << std::endl; }
   /// run one test
   bool operator()(bool test(void), const std::string& name)
   {  CPPAD_ASSERT_KNOWN(
         name.size() < width_ ,
         "test_boolofvoid: name does not have less characters than width"
      );
      std::cout.width( int(width_) );
      std::cout.setf( std::ios_base::left );
      std::cout << name;
      //
      bool ok = test();
      if( ok )
      {  std::cout << "OK" << std::endl;
         n_ok_++;
      }
      else
      {  std::cout << "Error" << std::endl;
         n_error_++;
      }
      return ok;
   }
   /// nuber of tests that passed
   size_t n_ok(void) const
   {  return n_ok_; }
   /// nuber of tests that failed
   size_t n_error(void) const
   {  return n_error_; }
   /// summary
   bool summary(bool memory_ok )
   {
      std::cout.width( int(width_) );
      std::cout.setf( std::ios_base::left );
      std::cout << "memory_leak";
      //
      if( memory_ok  )
      {  std::cout << "OK" << std::endl;
         n_ok_++;
      }
      else
      {  std::cout << "Error" << std::endl;
         n_error_++;
      }
      if( n_error_ == 0 )
         std::cout << "All " << n_ok_ << " tests passed." << std::endl;
      else
         std::cout << n_error_ << " tests failed." << std::endl;
      //
      return n_error_ == 0;
   }
};

} // END_CPPAD_NAMESPACE

# endif
