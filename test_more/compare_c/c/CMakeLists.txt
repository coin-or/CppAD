# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-25 Bradley M. Bell
# ----------------------------------------------------------------------------
#
# CMAKE_C_FLAGS
IF( ${cppad_link_flags_has_m32} )
   SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
ENDIF( )
#
ADD_EXECUTABLE(test_more_compare_c_c EXCLUDE_FROM_ALL ../det_by_minor.c)
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(test_more_compare_c_c
   ${cppad_lib}
   ${colpack_libs}
)
SET_TARGET_PROPERTIES(test_more_compare_c_c PROPERTIES
   LINK_OPTIONS "${cppad_link_flags}"
)
#
# check_test_more_compare_c_c
add_check_executable(check_test_more_compare_c c)
