# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell
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
SET(source_list
    checkpoint.cpp
    get_started.cpp
    extended_ode.cpp
    mul_level.cpp
    ode.cpp
)
set_compile_flags( example_checkpoint "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_checkpoint EXCLUDE_FROM_ALL ${source_list})
#
# Add the check_example_checkpoint target
ADD_CUSTOM_TARGET(check_example_checkpoint
    example_checkpoint
    DEPENDS example_checkpoint
)
MESSAGE(STATUS "make check_example_checkpoint: available")
#
# add to check check_example_depends in parent environment
add_to_list(check_example_depends check_example_checkpoint)
SET(check_example_depends "${check_example_depends}" PARENT_SCOPE)
