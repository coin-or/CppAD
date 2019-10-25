# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell
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
    atom_op.cpp
    cexp_op.cpp
    comp_op.cpp
    graph.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags(test_more_graph "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(test_more_graph EXCLUDE_FROM_ALL ${source_list})

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(test_more_graph
    ${cppad_lib}
    ${colpack_libs}
)
#
# Add the check_test_more_graph target
ADD_CUSTOM_TARGET(check_test_more_graph
    test_more_graph
    DEPENDS test_more_graph
)
MESSAGE(STATUS "make check_test_more_graph: available")
#
# add to check check_test_more_depends in parent environment
add_to_list(check_test_more_depends check_test_more_graph)
SET(check_test_more_depends "${check_test_more_depends}" PARENT_SCOPE)
