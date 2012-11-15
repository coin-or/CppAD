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
# Build the speed/sacado directory tests 
# Inherit environment from ../CMakeList.txt

# assert sacado_prefix is defined
IF ( NOT sacado_prefix )
	MESSAGE(FATAL_ERROR 
		"speed/sacado/CMakeLists.txt: sacado_prefix = ${sacado_prefix}"
	)
ENDIF ( NOT sacado_prefix )

# Specifies build type for this directory. Possible values are 
# empty, Debug, Release, RelWithDebInfo and MinSizeRel
SET(CMAKE_BUILD_TYPE RELEASE)

# Adds flags to the compiler command line for sources in the current directory 
# and below. This command can be used to add any flags, but it was originally 
# intended to add preprocessor definitions. 
ADD_DEFINITIONS("-DSPEED_SACADO -DRAD_AUTO_AD_Const")

# Local include directories to search (not in package_prefix/includdir)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../src )

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
ADD_EXECUTABLE(speed_sacado EXCLUDE_FROM_ALL ../main.cpp 
	det_lu.cpp
	det_minor.cpp
	mat_mul.cpp
	ode.cpp
	poly.cpp
	sparse_hessian.cpp
	sparse_jacobian.cpp
)

# Compiler flags for cppad source
add_cppad_cxx_flags( speed_sacado )

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(speed_sacado speed_src sacado )

# Add the check_speed_sacado target
ADD_CUSTOM_TARGET(check_speed_sacado 
	speed_sacado correct 54321
	DEPENDS speed_sacado speed_src
)
