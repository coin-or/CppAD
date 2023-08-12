# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-23 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the example/git directory tests
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
   atomic.cpp
   compare_change.cpp
   compile.cpp
   dynamic.cpp
   get_started.cpp
   jit.cpp
)
# END_SORT_THIS_LINE_MINUS_2
#
set_compile_flags( example_jit "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_jit EXCLUDE_FROM_ALL ${source_list})
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_jit
   ${cppad_lib}
   ${colpack_libs}
)
#
# check_example_jit
add_check_executable(check_example jit)
