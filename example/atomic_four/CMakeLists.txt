# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
#
# initialize check_example_atomic_four_depends
SET(check_example_atomic_four_depends "")
#
ADD_SUBDIRECTORY(lin_ode)
ADD_SUBDIRECTORY(mat_mul)
ADD_SUBDIRECTORY(vector)
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
   atomic_four.cpp
   dynamic.cpp
   forward.cpp
   get_started.cpp
   norm_sq.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags(example_atomic_four "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_atomic_four EXCLUDE_FROM_ALL ${source_list})
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_atomic_four
   ${cppad_lib}
   ${colpack_libs}
)
#
# check_example_atomic_four
add_check_executable(check_example atomic_four)
