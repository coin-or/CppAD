# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
#
# BEGIN_SORT_THIS_LINE_PLUS_2
SET(source_list
   chkpoint_one.cpp
   extended_ode.cpp
   get_started.cpp
   mul_level.cpp
   ode.cpp
)
# END_SORT_THIS_LINE_MINUS_2

set_compile_flags(test_more_deprecated_chkpoint_one
   "${cppad_debug_which}" "${source_list}"
)
#
ADD_EXECUTABLE(test_more_deprecated_chkpoint_one
   EXCLUDE_FROM_ALL ${source_list}
)
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(test_more_deprecated_chkpoint_one
   ${cppad_lib}
   ${colpack_libs}
)
#
# check_test_more_deprecated_chkpoint_one
add_check_executable(check_test_more_deprecated chkpoint_one)
