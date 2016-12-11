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

# CMAKE_BUILD_TYPE determined by parent directory. Possible values are
# empty, Debug, Release, RelWithDebInfo and MinSizeRel
#
# eigen_sources and CPPAD_HAS_EIGEN
#
ADD_EXECUTABLE(example_optimize EXCLUDE_FROM_ALL
	conditional_exp.cpp
	compare_op.cpp
	cumulative_sum.cpp
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
