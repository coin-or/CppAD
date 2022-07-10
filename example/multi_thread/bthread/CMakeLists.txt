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
# Build the example/multi_thread/bthread directory tests
# Inherit build type from ../CMakeList.txt

# Local include directories to search (not in package_prefix/include)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/.. )

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
SET(source_list ../thread_test.cpp
    ../team_example.cpp
    ../harmonic.cpp
    ../multi_atomic_two.cpp
    ../multi_atomic_three.cpp
    ../multi_chkpoint_one.cpp
    ../multi_chkpoint_two.cpp
    ../multi_newton.cpp
    a11c_bthread.cpp
    simple_ad_bthread.cpp
    team_bthread.cpp
)
set_compile_flags(
    example_multi_thread_bthread "${cppad_debug_which}" "${source_list}"
)
#
ADD_EXECUTABLE( example_multi_thread_bthread EXCLUDE_FROM_ALL ${source_list} )

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_multi_thread_bthread
    ${cppad_lib}
    ${colpack_libs}
    ${Boost_LIBRARIES}
)

# check_example_multi_thread_bthread
add_check_executable(check_example_multi_thread bthread simple_ad)
