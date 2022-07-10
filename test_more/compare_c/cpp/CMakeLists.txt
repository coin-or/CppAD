# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#       GNU General Public License, Version 2.0 or later.
# -----------------------------------------------------------------------------
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
# check_test_more_compre_c_cpp
add_check_executable(check_test_more_compare_c cpp)
