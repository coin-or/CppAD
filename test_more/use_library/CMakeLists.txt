# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-26 Bradley M. Bell
# ----------------------------------------------------------------------------
# Test using a library that is separate from cppad_lib and has calls to
# thread_alloc in it.
#
# test_more_library
set_compile_flags(
    test_more_library "${cppad_debug_which}" library.cpp
)
IF( ${cppad_lib_static_01} )
    ADD_LIBRARY( test_more_library STATIC library.cpp )
ELSE( )
    ADD_LIBRARY( test_more_library SHARED library.cpp )
ENDIF( )
TARGET_LINK_LIBRARIES(test_more_library ${cppad_lib} )
#
# test_more_use_library
set_compile_flags(
    test_more_use_library "${cppad_debug_which}" use_library.cpp
)
ADD_EXECUTABLE( test_more_use_library EXCLUDE_FROM_ALL use_library.cpp )
TARGET_LINK_LIBRARIES(test_more_use_library PRIVATE
    test_more_library
    ${cppad_lib}
)
#
# check_test_more_use_library
add_check_executable(check_test_more use_library)
