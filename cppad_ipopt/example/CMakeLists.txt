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
# Build the cppad_ipopt/example directory tests
# Inherit environment from ../CMakeList.txt

# Specifies build type for this directory. Possible values are
# empty, Debug, Release, RelWithDebInfo and MinSizeRel
SET(CMAKE_BUILD_TYPE DEBUG)

# Local include directories to search (not in package_prefix/include)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../src )

# Local link directories to search (not in external packages)
LINK_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/../src ${ipopt_LIBRARY_DIRS} )

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
ADD_EXECUTABLE(cppad_ipopt_example EXCLUDE_FROM_ALL
	example.cpp
	get_started.cpp
	ode_check.cpp
	ode_check.hpp
	ode_fast_check.cpp
	ode_fast.hpp
	ode_problem.hpp
	ode_run.hpp
	ode_simple_check.cpp
	ode_simple.hpp
)

# Extra compiler flags
add_cppad_cxx_flags( cppad_ipopt_example )

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(cppad_ipopt_example
	cppad_ipopt
	${cppad_lib}
	${ipopt_LIBRARIES}
	${colpack_libs}
)

# Add the check_cppad_ipopt_example target
ADD_CUSTOM_TARGET(check_cppad_ipopt_example
	cppad_ipopt_example
	DEPENDS cppad_ipopt_example cppad_ipopt
)
