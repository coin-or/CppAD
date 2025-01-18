// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-25 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin utility.cpp}

CppAD Utilities Examples and Tests Driver
#########################################

Running These Tests
*******************
After executing the :ref:`cmake-name` command
form the :ref:`download@Distribution Directory`,
you can build and run these tests with the commands::

   cd build
   make check_example_utility

Note that your choice of :ref:`cmake@generator` may require using
an different version of make; e.g., ``ninja`` .

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end utility.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++

// for thread_alloc
# include <cppad/utility/thread_alloc.hpp>

// test runner
# include <cppad/utility/test_boolofvoid.hpp>

// BEGIN_SORT_THIS_LINE_PLUS_1
extern bool CheckNumericType(void);
extern bool CheckSimpleVector(void);
extern bool CppAD_vector(void);
extern bool ErrorHandler(void);
extern bool LuFactor(void);
extern bool LuInvert(void);
extern bool LuSolve(void);
extern bool Near_Equal(void);
extern bool OdeErrControl(void);
extern bool OdeErrMaxabs(void);
extern bool OdeGear(void);
extern bool OdeGearControl(void);
extern bool RombergMul(void);
extern bool RombergOne(void);
extern bool SimpleVector(void);
extern bool dll_lib(void);
extern bool index_sort(void);
extern bool nan(void);
extern bool poly(void);
extern bool pow_int(void);
extern bool rosen_34(void);
extern bool runge_45(void);
extern bool runge_45_1(void);
extern bool set_union(void);
extern bool sparse_rc(void);
extern bool sparse_rcv(void);
extern bool thread_alloc(void);
extern bool to_string(void);
extern bool vectorBool(void);
// END_SORT_THIS_LINE_MINUS_1

// main program that runs all the tests
int main(void)
{  std::string group = "example/utility";
   size_t      width = 20;
   CppAD::test_boolofvoid Run(group, width);

   // This line is used by test_one.sh

// BEGIN_SORT_THIS_LINE_PLUS_1
   Run( CheckNumericType,       "CheckNumericType" );
   Run( CheckSimpleVector,      "CheckSimpleVector" );
   Run( CppAD_vector,           "CppAD_vector" );
   Run( ErrorHandler,           "ErrorHandler" );
   Run( LuFactor,               "LuFactor" );
   Run( LuInvert,               "LuInvert" );
   Run( LuSolve,                "LuSolve" );
   Run( Near_Equal,             "Near_Equal" );
   Run( OdeErrControl,          "OdeErrControl" );
   Run( OdeErrMaxabs,           "OdeErrMaxabs" );
   Run( OdeGear,                "OdeGear" );
   Run( OdeGearControl,         "OdeGearControl" );
   Run( RombergMul,             "RombergMul" );
   Run( RombergOne,             "RombergOne" );
   Run( SimpleVector,           "SimpleVector" );
   Run( index_sort,             "index_sort" );
   Run( nan,                    "nan" );
   Run( poly,                   "poly" );
   Run( pow_int,                "pow_int" );
   Run( rosen_34,               "rosen_34" );
   Run( runge_45,               "runge_45" );
   Run( runge_45_1,             "runge_45_1" );
   Run( set_union,              "set_union" );
   Run( sparse_rc,              "sparse_rc" );
   Run( sparse_rcv,             "sparse_rcv" );
   Run( thread_alloc,           "thread_alloc" );
   Run( to_string,              "to_string" );
   Run( vectorBool,             "vectorBool" );
// END_SORT_THIS_LINE_MINUS_1
//
# if CPPAD_C_COMPILER_GNU_FLAGS || CPPAD_C_COMPILER_MSVC_FLAGS
# if CPPAD_USE_CPLUSPLUS_2017 && ! CPPAD_LINK_FLAGS_HAS_M32
   Run( dll_lib,                "dll_lib" );
# endif
# endif
   //
   // check for memory leak
   bool memory_ok = CppAD::thread_alloc::free_all();
   // print summary at end
   bool ok = Run.summary(memory_ok);
   //
   return static_cast<int>( ! ok );
}
// END C++
