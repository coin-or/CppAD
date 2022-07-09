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
# Build the speed/cppad_jit directory tests
# Inherit build type from ../CMakeList.txt

# Set the install prefix for this package
SET(cppad_jit_prefix "/usr")

# Adds flags to the compiler command line for sources in the current directory
# and below. This command can be used to add any flags, but it was originally
# intended to add preprocessor definitions.
ADD_DEFINITIONS("-DCPPAD_CPPAD_JIT_SPEED")

# Local include directories to search (not in package_prefix/includdir)
INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${cppad_jit_prefix}/include
)

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
SET(source_list ../main.cpp
    det_lu.cpp
    det_minor.cpp
    mat_mul.cpp
    ode.cpp
    poly.cpp
    sparse_hessian.cpp
    sparse_jacobian.cpp
)
set_compile_flags( speed_cppad_jit "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE( speed_cppad_jit EXCLUDE_FROM_ALL ${source_list} )
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(speed_cppad_jit
    ${cppad_lib}
    ${colpack_libs}
)
# cppad_jit is an include file only library
TARGET_LINK_LIBRARIES(speed_cppad_jit speed_src )

# Add the check_speed_cppad_jit target
ADD_CUSTOM_TARGET(check_speed_cppad_jit
    ./speed_cppad_jit correct 54321
    DEPENDS speed_cppad_jit speed_src
)
MESSAGE(STATUS "make check_speed_cppad_jit: available")

# Change check depends in parent environment
add_to_list(check_speed_depends check_speed_cppad_jit)
SET(check_speed_depends "${check_speed_depends}" PARENT_SCOPE)
