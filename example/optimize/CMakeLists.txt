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
# Build the example/optimize directory tests
#
#
SET(source_list
	conditional_skip.cpp
	compare_op.cpp
	cumulative_sum.cpp
	forward_active.cpp
	nest_conditional.cpp
	print_for.cpp
	optimize.cpp
	reverse_active.cpp
)
set_compile_flags( example_optimize "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_optimize EXCLUDE_FROM_ALL ${source_list})

# Add the check_example_optimize target
ADD_CUSTOM_TARGET(check_example_optimize
	example_optimize
	DEPENDS example_optimize
)
MESSAGE(STATUS "make check_example_optimize: available")

# add to check check_example_depends in parent environment
add_to_list(check_example_depends check_example_optimize)
SET(check_example_depends "${check_example_depends}" PARENT_SCOPE)
