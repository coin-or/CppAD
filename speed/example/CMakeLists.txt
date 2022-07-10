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
# Build the speed/cppad directory tests
# Inherit build type from ../CMakeList.txt

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
SET(source_list example.cpp
    det_by_lu.cpp
    det_by_minor.cpp
    det_of_minor.cpp
    elapsed_seconds.cpp
    mat_sum_sq.cpp
    ode_evaluate.cpp
    sparse_hes_fun.cpp
    sparse_jac_fun.cpp
    speed_test.cpp
    time_test.cpp
)
set_compile_flags( speed_example "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE( speed_example EXCLUDE_FROM_ALL ${source_list} )
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(speed_example
    ${cppad_lib}
    ${colpack_libs}
)
# check_speed_example
add_check_executable(check_speed example)
#
ADD_EXECUTABLE( speed_program EXCLUDE_FROM_ALL speed_program.cpp )
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(speed_program
    ${cppad_lib}
    ${colpack_libs}
)
# check_speed_program
add_check_executable(check_speed program)
