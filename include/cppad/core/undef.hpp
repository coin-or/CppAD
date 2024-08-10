# ifndef CPPAD_CORE_UNDEF_HPP
# define CPPAD_CORE_UNDEF_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
----------------------------------------------------------------------------
Preprecessor definitions that presist after cppad/cppad.hpp is included.
These are part of the user API (see omh/preprocessor.omh) with some exceptions
that are used by the CppAD examples and tests.

# undef CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL used by CPPAD_USER_ATOMIC
# undef CPPAD_ASSERT_KNOWN                   used by cppad_ipopt
# undef CPPAD_ASSERT_UNKNOWN                 used by cppad_ipopt
# undef CPPAD_HASH_TABLE_SIZE                used by test_more/optimize.cpp
# undef EIGEN_MATRIXBASE_PLUGIN              example use of Eigen with CppAD
# undef CPPAD_HAS_COLPACK                    used by speed/cppad/sparse_*.cpp
# undef CPPAD_C_COMPILER_GNU_FLAGS           used by dll examples.
# undef CPPAD_C_COMPILER_MSVC_FLAGS          used by dll examples.
# undef CPPAD_C_COMPILER_CMD                 used by dll examples.

// for conditional testing when implicit conversion is not present
# undef CPPAD_DEPRECATED
-----------------------------------------------------------------------------
*/
// Preprecessor definitions that do not presist. None of these are in the
// user API.
// BEGIN_SORT_THIS_LINE_PLUS_1
# undef CPPAD_ASSERT_AD_TYPE
# undef CPPAD_ASSERT_NARG_NRES
# undef CPPAD_AZMUL
# undef CPPAD_BOOSTVECTOR
# undef CPPAD_COMPILER_HAS_CONVERSION_WARN
# undef CPPAD_COND_EXP
# undef CPPAD_COND_EXP_BASE_REL
# undef CPPAD_COND_EXP_REL
# undef CPPAD_CPPADVECTOR
# undef CPPAD_DEBUG_AND_RELEASE
# undef CPPAD_EIGENVECTOR
# undef CPPAD_FOLD_AD_VALUED_BINARY_OPERATOR
# undef CPPAD_FOLD_ASSIGNMENT_OPERATOR
# undef CPPAD_FOLD_BOOL_VALUED_BINARY_OPERATOR
# undef CPPAD_HAS_ADOLC
# undef CPPAD_HAS_EIGEN
# undef CPPAD_HAS_GETTIMEOFDAY
# undef CPPAD_HAS_IPOPT
# undef CPPAD_HAS_MKSTEMP
# undef CPPAD_HAS_TMPNAM_S
# undef CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
# undef CPPAD_IS_SAME_TAPE_ADDR_TYPE_SIZE_T
# undef CPPAD_IS_SAME_UNSIGNED_INT_SIZE_T
# undef CPPAD_LIB_EXPORT
# undef CPPAD_MAX_NUM_CAPACITY
# undef CPPAD_MIN_DOUBLE_CAPACITY
# undef CPPAD_NDEBUG_NOEXCEPT
# undef CPPAD_NOEXCEPT
# undef CPPAD_PADDING_BLOCK_T
# undef CPPAD_STANDARD_MATH_UNARY_AD
# undef CPPAD_STDVECTOR
# undef CPPAD_TRACE_CAPACITY
# undef CPPAD_TRACE_THREAD
# undef CPPAD_TRACK_DEBUG
# undef CPPAD_USER_MACRO
# undef CPPAD_USER_MACRO_ONE
# undef CPPAD_USER_MACRO_TWO
# undef CPPAD_VEC_AD_COMP_ASSIGN
# undef CPPAD_VEC_ENUM_TYPE
// END_SORT_THIS_LINE_MINUS_1

# endif
