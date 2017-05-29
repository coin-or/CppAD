# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Build the example/deprecated tests
#
SET(source_list
	deprecated.cpp
	old_reciprocal.cpp
	old_usead_1.cpp
	old_usead_2.cpp
	old_tan.cpp
	old_mat_mul.cpp
	omp_alloc.cpp
	track_new_del.cpp
	zdouble.cpp
)
set_compile_flags( example_deprecated "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_deprecated EXCLUDE_FROM_ALL ${source_list})
#
# Add the check_example_deprecated target
ADD_CUSTOM_TARGET(check_example_deprecated
	example_deprecated
	DEPENDS
	example_deprecated
)
MESSAGE(STATUS "make check_example_deprecated: available")
#
# Change check depends in parent environment
add_to_list(check_example_depends check_example_deprecated)
SET(check_example_depends "${check_example_depends}" PARENT_SCOPE)
