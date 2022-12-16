# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
#
# det_by_minor.cpp
CONFIGURE_FILE(
   ${CMAKE_CURRENT_SOURCE_DIR}/../det_by_minor.c
   ${CMAKE_CURRENT_BINARY_DIR}/det_by_minor.cpp
   COPYONLY
)
ADD_EXECUTABLE(test_more_compare_c_cpp EXCLUDE_FROM_ALL det_by_minor.cpp)
#
SET_TARGET_PROPERTIES(
   test_more_compare_c_cpp PROPERTIES COMPILE_FLAGS "${all_cxx_flags}"
)
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(test_more_compare_c_cpp
   ${cppad_lib}
   ${colpack_libs}
)
#
# check_test_more_compre_c_cpp
add_check_executable(check_test_more_compare_c cpp)
