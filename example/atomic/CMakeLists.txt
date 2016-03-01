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
# Build the example/atomic directory tests

# Specifies build type for this directory. Possible values are
# empty, Debug, Release, RelWithDebInfo and MinSizeRel
SET(CMAKE_BUILD_TYPE DEBUG)

#
ADD_EXECUTABLE(example_atomic EXCLUDE_FROM_ALL
	atomic.cpp
	checkpoint.cpp
	for_sparse_hes.cpp
	for_sparse_jac.cpp
	get_started.cpp
	mat_mul.cpp
	norm_sq.cpp
	reciprocal.cpp
	rev_sparse_hes.cpp
	rev_sparse_jac.cpp
	sparsity.cpp
	tangent.cpp
)

# Extra compiler flags
add_cppad_cxx_flags( example_atomic )

# Add the check_example_atomic target
ADD_CUSTOM_TARGET(check_example_atomic
	example_atomic
	DEPENDS example_atomic
)
