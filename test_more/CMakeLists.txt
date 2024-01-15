# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the test_more directory tests

# initialize check_depends
SET(check_test_more_depends "")

# deprecated tests
ADD_SUBDIRECTORY(deprecated)

# compare_c tests
# clange-cl.exe seems to be confused about C files and gives an erro message
# saying that C++ forbids macroizing the keywords bool, true, and false.
STRING(REGEX MATCH ".*[/\]clang-cl.exe" match "${CMAKE_C_COMPILER}" )
IF( NOT "${match}" STREQUAL "${CMAKE_C_COMPILER}" ) 
   ADD_SUBDIRECTORY(compare_c)
ENDIF( )
#
# debug_rel tests
IF( NOT "${CMAKE_GENERATOR}" STREQUAL "NMake Makefiles" )
   # Visual studio rejects mixing debug and release flags
   ADD_SUBDIRECTORY(debug_rel)
ENDIF( NOT "${CMAKE_GENERATOR}" STREQUAL "NMake Makefiles" )
#
# cppad_for_tmb tests
IF( OpenMP_CXX_FOUND )
   ADD_SUBDIRECTORY(cppad_for_tmb)
ENDIF( OpenMP_CXX_FOUND )

# general tests
ADD_SUBDIRECTORY(general)

# Add the check_example target
ADD_CUSTOM_TARGET(check_test_more "" DEPENDS ${check_test_more_depends})
MESSAGE(STATUS "make check_test_more: available")

# Change check depends in parent environment
add_to_list(check_depends check_test_more)
SET(check_depends "${check_depends}" PARENT_SCOPE)
