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
# Build the test_more directory tests

# initialize check_depends
SET(check_test_more_depends "")

# compare_c tests
ADD_SUBDIRECTORY(compare_c)

# debug_rel tests
ADD_SUBDIRECTORY(debug_rel)

# cppad_for_tmb tests
ADD_SUBDIRECTORY(cppad_for_tmb)

# general tests
ADD_SUBDIRECTORY(general)

# Add the check_example target
ADD_CUSTOM_TARGET(check_test_more "" DEPENDS ${check_test_more_depends})
MESSAGE(STATUS "make check_test_more: available")

# Change check depends in parent environment
add_to_list(check_depends check_test_more)
SET(check_depends "${check_depends}" PARENT_SCOPE)
