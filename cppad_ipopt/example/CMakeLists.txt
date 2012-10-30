# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Run the cppad_ipopt/example directory tests 
# Inherit environment from ../../CMakeList.txt

# assert ipopt_prefix is defined
IF ( NOT ipopt_prefix )
	MESSAGE(FATAL_ERROR 
		"cppad_ipopt/src/CMakeLists.txt: ipopt_prefix = ${ipopt_prefix}"
	)
ENDIF ( NOT ipopt_prefix )

# Specifies build type for this directory. Possible values are 
# empty, Debug, Release, RelWithDebInfo and MinSizeRel
SET(CMAKE_BUILD_TYPE DEBUG)

# Local include directories to search (not in external packages)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../src )

# Local link directories to search (not in external packages)
LINK_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR}/../src ${ipopt_LIBRARY_DIRS} )

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
ADD_EXECUTABLE(ipopt_example example.cpp
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

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(ipopt_example cppad_ipopt ${ipopt_LIBRARIES})

# add_test(testname Exename arg1 arg2 ... )
# ADD_TEST( ipopt_example ipopt_example )
