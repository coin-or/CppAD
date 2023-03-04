# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the example/ipopt_solve directory tests

# Local link directories to search, as determined by pkg-config for ipopt
LINK_DIRECTORIES( ${ipopt_LIBRARY_DIRS} )
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
   get_started.cpp
   ipopt_solve.cpp
   ode_inverse.cpp
   retape.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags( example_ipopt_solve "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_ipopt_solve EXCLUDE_FROM_ALL ${source_list})
#
# libraries to be linked into the specified target,
# as determined by pkg-config for ipopt
TARGET_LINK_LIBRARIES(example_ipopt_solve
   ${cppad_lib}
   ${ipopt_LINK_LIBRARIES}
   ${colpack_libs}
)

# check_example_ipopt_solve
add_check_executable(check_example ipopt_solve)
