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
# Build the example/print_for directory tests
#
SET(source_list
	abs_eval.cpp
	abs_min_linear.cpp
	abs_min_quad.cpp
	abs_normal.cpp
	get_started.cpp
	lp_box.cpp
	min_nso_linear.cpp
	min_nso_quad.cpp
	qp_box.cpp
	qp_interior.cpp
	simplex_method.cpp
)
#
set_compile_flags( example_abs_normal "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_abs_normal EXCLUDE_FROM_ALL ${source_list})

# check_example_abs_normal target
ADD_CUSTOM_TARGET(check_example_abs_normal
	example_abs_normal
	DEPENDS
	example_abs_normal
)
MESSAGE(STATUS "make check_example_abs_normal: available")

# Change check_example_depends in parent environment
add_to_list(check_example_depends check_example_abs_normal)
SET(check_example_depends "${check_example_depends}" PARENT_SCOPE)
