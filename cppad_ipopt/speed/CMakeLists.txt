# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the cppad_ipopt/speed directory tests
# Inherit build type from ../CMakeList.txt

# Local include directories to search (not in package_prefix/include)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../src )

# Local link directories to search (not in external packages)
# (cannot use ../src/cppad_ipopt library because it is linke with debugging)
LINK_DIRECTORIES( ${ipopt_LIBRARY_DIRS} )

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
# Include source for cppad_ipopt library so we build it without debugging
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
   ../src/cppad_ipopt_nlp.cpp
   ../src/fun_record.hpp
   ../src/hes_fg_map.cpp
   ../src/jac_g_map.cpp
   ../src/sparse_map2vec.cpp
   ../src/vec_fun_pattern.cpp
   ode_speed.cpp
   speed.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags( cppad_ipopt_example "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE( cppad_ipopt_speed EXCLUDE_FROM_ALL ${source_list} )

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(
   cppad_ipopt_speed
   cppad_ipopt
   ${cppad_lib}
   ${ipopt_LINK_LIBRARIES}
   ${colpack_libs}
)

# check_cppad_ipopt_speed
add_check_executable(check_cppad_ipopt speed)
