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
    add_op.cpp
    atom4_op.cpp
    atom_op.cpp
    azmul_op.cpp
    cexp_op.cpp
    comp_op.cpp
    discrete_op.cpp
    div_op.cpp
    from_json.cpp
    get_started.cpp
    json.cpp
    mul_op.cpp
    pow_op.cpp
    print_op.cpp
    sparse.cpp
    sub_op.cpp
    sum_op.cpp
    to_json.cpp
    unary_op.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags(example_json "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_json EXCLUDE_FROM_ALL ${source_list})

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_json
    ${cppad_lib}
    ${colpack_libs}
)
#
# check_example_json
add_check_executable(check_example json)
