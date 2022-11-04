# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the example/print_for directory tests
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
   abs_eval.cpp
   abs_min_linear.cpp
   abs_min_quad.cpp
   abs_normal.cpp
   get_started.cpp
   lp_box.cpp
   min_nso_linear.cpp
   min_nso_quad.cpp
   qp_box.cpp
   qp_interior.cpp
   simplex_method.cpp
)
# END_SORT_THIS_LINE_MINUS_2

#
set_compile_flags( example_abs_normal "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_abs_normal EXCLUDE_FROM_ALL ${source_list})
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_abs_normal
   ${cppad_lib}
   ${colpack_libs}
)
#
# check_example_abs_normal
add_check_executable(check_example abs_normal)
