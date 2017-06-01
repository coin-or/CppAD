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
# Build the speed/cppad directory tests
# Inherit build type environment from ../CMakeList.txt

# Adds flags to the compiler command line for sources in the current directory
# and below. This command can be used to add any flags, but it was originally
# intended to add preprocessor definitions.
ADD_DEFINITIONS("-DCPPAD_CPPAD_SPEED")

# Local include directories to search (not in package_prefix/includdir)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../src )

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
SET(source_list ../main.cpp
	det_lu.cpp
	det_minor.cpp
	mat_mul.cpp
	ode.cpp
	poly.cpp
	sparse_hessian.cpp
	sparse_jacobian.cpp
)
# 2DO: fix problem with the test
#	./speed_cppad sparse_hessian 123 colpack
# when speed_cppad is compiled for debugging.
set_compile_flags( speed_cppad "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE( speed_cppad EXCLUDE_FROM_ALL ${source_list} )

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(speed_cppad
	speed_src
	${cppad_lib}
	${colpack_libs}
)

# Add the check_speed_cppad target
ADD_CUSTOM_TARGET(check_speed_cppad
	speed_cppad correct 54321
	DEPENDS speed_cppad speed_src
)
MESSAGE(STATUS "make check_speed_cppad: available")

# Change check depends in parent environment
add_to_list(check_speed_depends check_speed_cppad)
SET(check_speed_depends "${check_speed_depends}" PARENT_SCOPE)
