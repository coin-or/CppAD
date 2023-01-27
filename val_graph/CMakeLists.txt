# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the val_graph directory tests
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list val_graph.cpp
   add_xam.cpp
   call_xam.cpp
   con_xam.cpp
   dead_xam.cpp
   fun2val_xam.cpp
   renumber_xam.cpp
   sub_xam.cpp
   test/fun2val_atom.cpp
   test/nan.cpp
   test/not_used.cpp
   test/opt_call.cpp
   val2fun_xam.cpp
)
# END_SORT_THIS_LINE_MINUS_2
#
set_compile_flags( val_graph "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(val_graph EXCLUDE_FROM_ALL ${source_list})
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(val_graph
   ${cppad_lib}
   ${colpack_libs}
)
#
# check_example_print_for
add_check_executable(check val_graph)
