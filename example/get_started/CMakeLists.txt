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
# Build the example/get_started directory tests
#
SET(source_list get_started.cpp)
#
set_compile_flags( example_get_started "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_get_started EXCLUDE_FROM_ALL ${source_list})
#
# Add the check for this target
ADD_CUSTOM_TARGET(check_example_get_started
	example_get_started
	DEPENDS example_get_started
)
MESSAGE(STATUS "make check_example_get_started: available")
#
# Change check depends in parent environment
add_to_list(check_example_depends check_example_get_started)
SET(check_example_depends "${check_example_depends}" PARENT_SCOPE)
