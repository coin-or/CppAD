# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
   forward.cpp
   identical_zero.cpp
   mat_mul.cpp
   rev_depend.cpp
   reverse.cpp
   sparsity.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags(example_atomic_four_mat_mul
   "${cppad_debug_which}" "${source_list}"
)
#
ADD_EXECUTABLE(example_atomic_four_mat_mul EXCLUDE_FROM_ALL ${source_list})
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_atomic_four_mat_mul
   ${cppad_lib}
   ${colpack_libs}
)
#
# check_example_atomic_four_mat_mul
add_check_executable(check_example_atomic_four mat_mul)
