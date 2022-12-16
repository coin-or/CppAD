# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
   add_op.cpp
   atom4_op.cpp
   atom_op.cpp
   azmul_op.cpp
   cexp_op.cpp
   comp_op.cpp
   discrete_op.cpp
   div_op.cpp
   graph.cpp
   mul_op.cpp
   pow_op.cpp
   print_graph.cpp
   print_op.cpp
   sub_op.cpp
   sum_op.cpp
   switch_var_dyn.cpp
   unary_op.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags(example_graph "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_graph EXCLUDE_FROM_ALL ${source_list})

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_graph
   ${cppad_lib}
   ${colpack_libs}
)
#
# check_example_graph
add_check_executable(check_example graph)
