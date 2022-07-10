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
    add.cpp
    vector.cpp
    div.cpp
    hes_sparsity.cpp
    jac_sparsity.cpp
    mul.cpp
    neg.cpp
    rev_depend.cpp
    sub.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags(example_atomic_four_vector "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_atomic_four_vector EXCLUDE_FROM_ALL ${source_list})
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_atomic_four_vector
    ${cppad_lib}
    ${colpack_libs}
)
#
# check_example_atomic_four vector
add_check_executable(check_example_atomic_four vector)
