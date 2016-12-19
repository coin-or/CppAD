# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Build the example/optimize directory tests

# Specifies build type for this directory. Possible values are
# empty, Debug, Release, RelWithDebInfo and MinSizeRel
SET(CMAKE_BUILD_TYPE DEBUG)
#
ADD_EXECUTABLE(example_optimize EXCLUDE_FROM_ALL
	conditional_skip.cpp
	compare_op.cpp
	cumulative_sum.cpp
	forward_active.cpp
	nest_conditional.cpp
	print_for.cpp
	optimize.cpp
	reverse_active.cpp
)

# Extra compiler flags
add_cppad_cxx_flags( example_optimize )

# Add the check_example_optimize target
ADD_CUSTOM_TARGET(check_example_optimize
	example_optimize
	DEPENDS example_optimize
)
