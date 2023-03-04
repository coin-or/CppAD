# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the cppad_ipopt/test directory tests
# Inherit build tyope from ../CMakeList.txt

# Local include directories to search (not in package_prefix/include)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../src )

# Local link directories to search (not in external packages)
LINK_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/../src ${ipopt_LIBRARY_DIRS} )

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
SET(source_list test_more.cpp
   k_gt_one.cpp
   multiple_solution.cpp
   retape_k1_l1.cpp
   retape_k1_l2.cpp
)
set_compile_flags( cppad_ipopt_test "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE( cppad_ipopt_test EXCLUDE_FROM_ALL ${source_list} )

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(cppad_ipopt_test
   cppad_ipopt
   ${cppad_lib}
   ${ipopt_LINK_LIBRARIES}
   ${colpack_libs}
)

# check_cppad_ipopt_test
add_check_executable(check_cppad_ipopt test)
