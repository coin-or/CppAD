# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Build the example directory tests
# Inherit environment from ../CMakeList.txt

# initialize check_depends
SET(check_example_depends "")

# abs_normal examples
ADD_SUBDIRECTORY(abs_normal)

# atomic examples
ADD_SUBDIRECTORY(atomic)

# deprecated examples
ADD_SUBDIRECTORY(deprecated)

# general examples
ADD_SUBDIRECTORY(general)

# get_started
ADD_SUBDIRECTORY(get_started)

# ipopt_solve examples
IF( cppad_has_ipopt)
	ADD_SUBDIRECTORY(ipopt_solve)
ENDIF( cppad_has_ipopt)

# multi_thread examples
ADD_SUBDIRECTORY(multi_thread)

# optimize examples
ADD_SUBDIRECTORY(optimize)

# print_for example
ADD_SUBDIRECTORY(print_for)

# sparse examples
ADD_SUBDIRECTORY(sparse)

# utility examples
ADD_SUBDIRECTORY(utility)

# Add the check_example target
ADD_CUSTOM_TARGET(check_example "" DEPENDS ${check_example_depends})
MESSAGE(STATUS "make check_example: available")

# Change check depends in parent environment
add_to_list(check_depends check_example)
SET(check_depends "${check_depends}" PARENT_SCOPE)
