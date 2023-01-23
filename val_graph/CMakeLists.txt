# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the val_graph directory tests
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list val_graph.cpp
   add_xam.cpp
   con_xam.cpp
   dead_xam.cpp
   fun2val_xam.cpp
   map_xam.cpp
   renumber_xam.cpp
   sub_xam.cpp
   test_one.cpp
   test_two.cpp
)
# END_SORT_THIS_LINE_MINUS_2
#
set_compile_flags( val_graph "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(val_graph EXCLUDE_FROM_ALL ${source_list})
#
# check_example_print_for
add_check_executable(check val_graph)
