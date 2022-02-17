# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
# -----------------------------------------------------------------------------
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
    forward.cpp
    identical_zero.cpp
    mat_mul.cpp
    rev_depend.cpp
    reverse.cpp
    sparsity.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags(example_atomic_four_mat_mul
    "${cppad_debug_which}" "${source_list}"
)
#
ADD_EXECUTABLE(example_atomic_four_mat_mul EXCLUDE_FROM_ALL ${source_list})
#
# Add the check_example_atomic_four_mat_mul target
ADD_CUSTOM_TARGET(check_example_atomic_four_mat_mul
    example_atomic_four_mat_mul
    DEPENDS example_atomic_four_mat_mul
)
MESSAGE(STATUS "make check_example_atomic_four_mat_mul: available")
#
# add to check check_example_atomic_four_depends in parent environment
add_to_list(
    check_example_atomic_four_depends
    check_example_atomic_four_mat_mul
)
SET(check_example_atomic_four_depends
    "${check_example_atomic_four_depends}" PARENT_SCOPE
)
