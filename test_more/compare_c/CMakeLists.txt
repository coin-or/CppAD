# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# $begin compare_c$$ $newlinech #$$
# $spell
#   det
#   cpp
# $$
#
# $section Compare Speed of C and C++$$
# $index compare, speed C and C++$$
# $index speed, compare C and C++$$
# $index C, compare speed with C++$$
# $index C++, compare speed with C$$
#
# $head Syntax$$
# $codei%test_more/compare_c/det_by_minor_c
# %$$
# $codei test_more/compare_c/det_by_minor_cpp$$
#
# $head Purpose$$
# Compares the speed of the exact same source code compiled
# using C versus C++.
#
# $childtable%
#   test_more/compare_c/det_by_minor.c
# %$$
#
# $end
# -----------------------------------------------------------------------------
#
# use cppad_debug_which to determine build type
IF( "${cppad_debug_which}" STREQUAL debug_all )
    SET(CMAKE_BUILD_TYPE DEBUG)
    SET(all_cxx_flags "${cppad_cxx_flags} ${CMAKE_CXX_FLAGSS_DEBUG}")
ELSEIF( "${cppad_debug_which}" STREQUAL debug_odd )
    SET(CMAKE_BUILD_TYPE DEBUG)
    SET(all_cxx_flags "${cppad_cxx_flags} ${CMAKE_CXX_FLAGSS_DEBUG}")
ELSE( "${cppad_debug_which}" STREQUAL debug_odd )
    SET(CMAKE_BUILD_TYPE RELEASE)
    SET(all_cxx_flags "${cppad_cxx_flags} ${CMAKE_CXX_FLAGSS_RELEASE}")
ENDIF( "${cppad_debug_which}" STREQUAL debug_all )
#
# initialize check_test_more_compare_c
SET(check_test_more_compare_c_depends "")
#
ADD_SUBDIRECTORY(c)
ADD_SUBDIRECTORY(cpp)
#
# check_test_more_compare_c target
ADD_CUSTOM_TARGET( check_test_more_compare_c
    DEPENDS ${check_test_more_compare_c_depends}
)
MESSAGE(STATUS "make check_test_more_compare_c: available")

# Add check_test_more_compare_c to check depends in parent environment
add_to_list(check_test_more_depends check_test_more_compare_c)
SET(check_test_more_depends "${check_test_more_depends}" PARENT_SCOPE)
