# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
# Inherit build type from ../CMakeList.txt

# Local include directories to search (not in package_prefix/include)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../src )

# Local link directories to search (not in external packages)
LINK_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/../src ${ipopt_LIBRARY_DIRS} )

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
   example.cpp
   get_started.cpp
   ode_check.cpp
   ode_check.hpp
   ode_fast.hpp
   ode_fast_check.cpp
   ode_problem.hpp
   ode_run.hpp
   ode_simple.hpp
   ode_simple_check.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags( cppad_ipopt_example "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE( cppad_ipopt_example EXCLUDE_FROM_ALL ${source_list} )

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(cppad_ipopt_example
   cppad_ipopt
   ${cppad_lib}
   ${ipopt_LINK_LIBRARIES}
   ${colpack_libs}
)

#
# check_cppad_ipopt_example
add_check_executable(check_cppad_ipopt example)
