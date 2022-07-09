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
# Build the example/git directory tests
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
    atomic.cpp
    compile.cpp
    dynamic.cpp
    get_started.cpp
    jit.cpp
    to_csrc.cpp
)
# END_SORT_THIS_LINE_MINUS_2
#
set_compile_flags( example_jit "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_jit EXCLUDE_FROM_ALL ${source_list})
#
# Add the check for this target
ADD_CUSTOM_TARGET(check_example_jit
    example_jit
    DEPENDS example_jit
)
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_jit
    ${cppad_lib}
    ${colpack_libs}
)
MESSAGE(STATUS "make check_example_jit: available")
#
# Change check depends in parent environment
add_to_list(check_example_depends check_example_jit)
SET(check_example_depends "${check_example_depends}" PARENT_SCOPE)
