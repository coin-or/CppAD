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
# Specifies build type for this directory. Possible values are
# empty, Debug, Release, RelWithDebInfo and MinSizeRel
SET(CMAKE_BUILD_TYPE DEBUG)
#
ADD_EXECUTABLE(example_utility EXCLUDE_FROM_ALL
	check_numeric_type.cpp
	check_simple_vector.cpp
	cppad_vector.cpp
	error_handler.cpp
	index_sort.cpp
	lu_factor.cpp
	lu_invert.cpp
	lu_solve.cpp
	nan.cpp
	near_equal.cpp
	ode_err_control.cpp
	ode_err_maxabs.cpp
	ode_gear_control.cpp
	ode_gear.cpp
	romberg_mul.cpp
	romberg_one.cpp
	runge45_1.cpp
	set_union.cpp
	simple_vector.cpp
	sparse_rc.cpp
	thread_alloc.cpp
	to_string.cpp
	utility.cpp
	vector_bool.cpp
)

# Extra compiler flags
add_cppad_cxx_flags( example_utility )

# Add the check_example_utility target
ADD_CUSTOM_TARGET(check_example_utility
	example_utility
	DEPENDS example_utility
)
MESSAGE(STATUS "make check_example_utility: available")

# add to check check_example_depends in parent environment
add_to_list(check_example_depends check_example_utility)
SET(check_example_depends "${check_example_depends}" PARENT_SCOPE)
