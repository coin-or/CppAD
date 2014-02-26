# $Id$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-14 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Eclipse Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
# Build the speed/double directory tests 
# Inherit environment from ../CMakeList.txt
# CMAKE_BUILD_TYPE determined by parent directory

# Adds flags to the compiler command line for sources in the current directory 
# and below. This command can be used to add any flags, but it was originally 
# intended to add preprocessor definitions. 
ADD_DEFINITIONS("-DCPPAD_DOUBLE_SPEED")

# Local include directories to search (not in package_prefix/includdir)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../src )

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
ADD_EXECUTABLE(speed_double EXCLUDE_FROM_ALL ../main.cpp 
	det_lu.cpp
	det_minor.cpp
	mat_mul.cpp
	ode.cpp
	poly.cpp
	sparse_hessian.cpp
	sparse_jacobian.cpp
)

# Compiler flags for cppad source
add_cppad_cxx_flags( speed_double )

# double does not use any external library
TARGET_LINK_LIBRARIES(speed_double speed_src )


# Add the check_speed_double target
ADD_CUSTOM_TARGET(check_speed_double 
	speed_double correct 54321
	DEPENDS speed_double speed_src
)
