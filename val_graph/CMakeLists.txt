# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the val_graph directory tests
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list val_graph.cpp
   binary_xam.cpp
   call_xam.cpp
   cexp_xam.cpp
   comp_xam.cpp
   compress_xam.cpp
   con_xam.cpp
   csum_xam.cpp
   cumulative_xam.cpp
   dead_xam.cpp
   dis_xam.cpp
   fold_con_xam.cpp
   fun2val_xam.cpp
   pri_xam.cpp
   renumber_xam.cpp
   summation_xam.cpp
   test/ad_double.cpp
   test/fold.cpp
   test/fun2val.cpp
   test/nan.cpp
   test/opt_call.cpp
   test/optimize.cpp
   test/val2fun.cpp
   test/val_optimize.cpp
   unary_xam.cpp
   val2fun_xam.cpp
   vec_xam.cpp
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
