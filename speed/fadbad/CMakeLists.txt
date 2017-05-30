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
# Build the speed/fadbad directory tests
# Inherit build type from ../CMakeList.txt

# assert fadbad_prefix is defined
IF ( NOT fadbad_prefix )
	MESSAGE(FATAL_ERROR
		"speed/fadbad/CMakeLists.txt: fadbad_prefix = ${fadbad_prefix}"
	)
ENDIF ( NOT fadbad_prefix )

# Adds flags to the compiler command line for sources in the current directory
# and below. This command can be used to add any flags, but it was originally
# intended to add preprocessor definitions.
ADD_DEFINITIONS("-DCPPAD_FADBAD_SPEED")

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
set_compile_flags( speed_fadbad "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE( speed_fadbad EXCLUDE_FROM_ALL ${source_list} )

# fadbad is an include file only library
TARGET_LINK_LIBRARIES(speed_fadbad speed_src )

# Add the check_speed_fadbad target
ADD_CUSTOM_TARGET(check_speed_fadbad
	speed_fadbad correct 54321
	DEPENDS speed_fadbad speed_src
)
MESSAGE(STATUS "make check_speed_fadbad: available")

# Change check depends in parent environment
add_to_list(check_speed_depends check_speed_fadbad)
SET(check_speed_depends "${check_speed_depends}" PARENT_SCOPE)
