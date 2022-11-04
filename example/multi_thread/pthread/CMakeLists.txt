# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the example/multi_thread/pthread directory tests
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
   a11c_pthread.cpp
   simple_ad_pthread.cpp
   team_pthread.cpp
)
set_compile_flags( example_multi_thread_pthread "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE( example_multi_thread_pthread EXCLUDE_FROM_ALL ${source_list} )

# Paths in which the linker will search for libraries,
# only applies to targets created after it is called
LINK_DIRECTORIES( ${pthread_lib_path} )

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_multi_thread_pthread
   ${cppad_lib}
   ${colpack_libs}
   ${pthread_lib}
)

# check_example_multi_thread_pthread
add_check_executable(check_example_multi_thread pthread simple_ad)
