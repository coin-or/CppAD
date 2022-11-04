# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the example/multi_thread/openmp directory tests
# Inherit build type from ../CMakeList.txt

# Local include directories to search (not in package_prefix/include)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/.. )

# Cannot find specifications for CMAKE_CXX_FLAGS in version 2.6 documentation
# so using ADD_DEFINITIONS instead.
ADD_DEFINITIONS( ${OpenMP_CXX_FLAGS} )

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
   a11c_openmp.cpp
   simple_ad_openmp.cpp
   team_openmp.cpp
)
set_compile_flags( example_multi_thread_openmp "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE( example_multi_thread_openmp EXCLUDE_FROM_ALL ${source_list} )
TARGET_LINK_LIBRARIES(
   example_multi_thread_openmp PRIVATE
   ${cppad_lib}
   ${colpack_libs}
   ${OpenMP_CXX_LIBRARIES}
)

# Extra flags used by linker for openmp support
SET(CMAKE_EXE_LINKER_FLAGS ${OpenMP_CXX_FLAGS} )

# check_example_multi_thread_openmp
add_check_executable(check_example_multi_thread openmp simple_ad)
