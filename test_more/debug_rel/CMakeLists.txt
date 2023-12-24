# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the test_more/general tests

# set compiler flags for debug_rel.cpp and debug.cpp
IF( "${cppad_debug_which}" STREQUAL "debug_all" )
   SET(debug_flags    "${cppad_cxx_flags} ${CMAKE_CXX_FLAGS_DEBUG}")
   SET(release_flags  "${cppad_cxx_flags} ${CMAKE_CXX_FLAGS_DEBUG}")
ELSEIF( "${cppad_debug_which}" STREQUAL "debug_none" )
   SET(debug_flags    "${cppad_cxx_flags} ${CMAKE_CXX_FLAGS_RELEASE}")
   SET(release_flags  "${cppad_cxx_flags} ${CMAKE_CXX_FLAGS_RELEASE}")
ELSE( "${cppad_debug_which}" )
   SET(debug_flags    "${cppad_cxx_flags} ${CMAKE_CXX_FLAGS_DEBUG}")
   SET(release_flags  "${cppad_cxx_flags} ${CMAKE_CXX_FLAGS_RELEASE}")
ENDIF( "${cppad_debug_which}" STREQUAL "debug_all" )
#
SET_SOURCE_FILES_PROPERTIES(
   debug_rel.cpp debug.cpp PROPERTIES COMPILE_FLAGS "${debug_flags}"
)
#
SET_SOURCE_FILES_PROPERTIES(
   release.cpp PROPERTIES COMPILE_FLAGS "${release_flags}"
)

# now that we have the properties, add the executable
ADD_EXECUTABLE(test_more_debug_rel EXCLUDE_FROM_ALL
   debug_rel.cpp
   debug.cpp
   release.cpp
)
# debug_rel does not use cppad_lib

# check_test_more_debug_rel
add_check_executable(check_test_more debug_rel)
