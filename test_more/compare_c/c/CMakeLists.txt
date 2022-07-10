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
ADD_EXECUTABLE(test_more_compare_c_c EXCLUDE_FROM_ALL ../det_by_minor.c)
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(test_more_compare_c_c
    ${cppad_lib}
    ${colpack_libs}
)
#
# check_test_more_compre_c_c
add_check_executable(check_test_more_compare_c c)
