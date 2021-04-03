# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell
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
    from_to_graph.cpp
    grad_det_minor.cpp
    link_adfun.cpp
    link_discrete.cpp
    link_atomic.cpp
    link_lib.cpp
    llvm.cpp
    optimize.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags(example_llvm "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_llvm EXCLUDE_FROM_ALL ${source_list})

# List of libraries to be linked into the specified target
IF( cppad_has_llvm )
    SET(cppad_link "cppad_link")
ELSE( cppad_has_llvm )
    SET(cppad_link "")
ENDIF( cppad_has_llvm )
TARGET_LINK_LIBRARIES(example_llvm
    discrete
    ${cppad_lib}
    ${cppad_link}
    ${colpack_libs}
)
#
# This library contains call backs from discrete functions
ADD_LIBRARY(discrete SHARED discrete.cpp)
#
# This library contains call backs from atomic functions
ADD_LIBRARY(atomic SHARED atomic.cpp)
#
# Add the check_example_llvm target
ADD_CUSTOM_TARGET(check_example_llvm
    example_llvm
    DEPENDS example_llvm atomic
)
MESSAGE(STATUS "make check_example_llvm: available")
#
# add to check check_example_depends in parent environment
add_to_list(check_example_depends check_example_llvm)
SET(check_example_depends "${check_example_depends}" PARENT_SCOPE)
