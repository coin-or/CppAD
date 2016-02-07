# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Build the example/ipopt_solve directory tests

# Specifies build type for this directory. Possible values are
# empty, Debug, Release, RelWithDebInfo and MinSizeRel
SET(CMAKE_BUILD_TYPE DEBUG)

# Local link directories to search, as determined by pkg-config for ipopt
LINK_DIRECTORIES( ${ipopt_LIBRARY_DIRS} )

# Copy a file to another location and modify its contents.
# configure_file(InputFile OutputFile [COPYONLY] [ESCAPE_QUOTES] [@ONLY])
CONFIGURE_FILE(
	${CMAKE_CURRENT_SOURCE_DIR}/test_one.sh.in
	${CMAKE_CURRENT_SOURCE_DIR}/test_one.sh
)

#
ADD_EXECUTABLE(example_ipopt_solve EXCLUDE_FROM_ALL
	solve.cpp
	get_started.cpp
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
