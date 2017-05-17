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
# Build the example/ipopt_solve directory tests

# Local link directories to search, as determined by pkg-config for ipopt
LINK_DIRECTORIES( ${ipopt_LIBRARY_DIRS} )

#
ADD_EXECUTABLE(example_ipopt_solve EXCLUDE_FROM_ALL
	get_started.cpp
	ipopt_solve.cpp
	ode_inverse.cpp
	retape.cpp
)

# Extra compiler flags
add_cppad_cxx_flags( example_ipopt_solve )

# libraries to be linked into the specified target,
# as determined by pkg-config for ipopt
TARGET_LINK_LIBRARIES(example_ipopt_solve
	${cppad_lib}
	${ipopt_LIBRARIES}
	${colpack_libs}
)

# Add the check_example_ipopt_solve target
ADD_CUSTOM_TARGET(check_example_ipopt_solve
	example_ipopt_solve
	DEPENDS example_ipopt_solve
)
MESSAGE(STATUS "make check_example_ipopt_solve: available")
