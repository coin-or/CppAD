# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
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
