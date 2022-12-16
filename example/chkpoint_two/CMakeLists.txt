# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
   base2ad.cpp
   chkpoint_two.cpp
   compare.cpp
   dynamic.cpp
   get_started.cpp
   ode.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags( example_chkpoint_two "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE(example_chkpoint_two EXCLUDE_FROM_ALL ${source_list})
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(example_chkpoint_two
   ${cppad_lib}
   ${colpack_libs}
)
#
# check_example_chkpoint_two
add_check_executable(check_example chkpoint_two)
