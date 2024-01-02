# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-22 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the example directory tests
# Inherit environment from ../CMakeList.txt

# initialize check_example_depends
SET(check_example_depends "")
#
# BEGIN_SORT_THIS_LINE_PLUS_1
ADD_SUBDIRECTORY(abs_normal)
ADD_SUBDIRECTORY(atomic_four)
ADD_SUBDIRECTORY(atomic_three)
ADD_SUBDIRECTORY(atomic_two)
ADD_SUBDIRECTORY(chkpoint_two)
ADD_SUBDIRECTORY(compare_change)
ADD_SUBDIRECTORY(general)
ADD_SUBDIRECTORY(get_started)
ADD_SUBDIRECTORY(graph)
ADD_SUBDIRECTORY(jit)
ADD_SUBDIRECTORY(json)
ADD_SUBDIRECTORY(multi_thread)
ADD_SUBDIRECTORY(optimize)
ADD_SUBDIRECTORY(print_for)
ADD_SUBDIRECTORY(sparse)
ADD_SUBDIRECTORY(utility)
# END_SORT_THIS_LINE_MINUS_1
#
# code_gen_fun examples
IF( cppad_has_cppadcg )
   ADD_SUBDIRECTORY(code_gen_fun)
ENDIF( cppad_has_cppadcg )
#
# ipopt_solve examples
IF( cppad_has_ipopt)
   ADD_SUBDIRECTORY(ipopt_solve)
ENDIF( cppad_has_ipopt)


# Add the check_example target
ADD_CUSTOM_TARGET(check_example "" DEPENDS ${check_example_depends})
MESSAGE(STATUS "make check_example: available")

# Change check depends in parent environment
add_to_list(check_depends check_example)
SET(check_depends "${check_depends}" PARENT_SCOPE)
