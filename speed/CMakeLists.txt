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
# Build the speed/* directory tests
# Inherit environment from ../CMakeList.txt

# CppAD developers use DEBUG for debugging speed tests.
# SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -DNDEBUG")

# Specify build type for this directory and all its subdirectories.
# Possible values are DEBUG, RELEASE, RelWithDebInfo and MinSizeRel
SET(CMAKE_BUILD_TYPE RELEASE)

# Initialize list of tests as empty
SET(check_depends "")

# The CMakeLists.txt file in the specified source directory is processed
# before the current input file continues beyond this command.
# add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
ADD_SUBDIRECTORY(src)
#
ADD_SUBDIRECTORY(cppad)
add_to_list(check_depends check_speed_cppad)
#
ADD_SUBDIRECTORY(double)
add_to_list(check_depends check_speed_double)
#
ADD_SUBDIRECTORY(example)
add_to_list(check_depends check_speed_example)
#
IF ( cppad_profile_flag )
	ADD_SUBDIRECTORY(profile)
	add_to_list(check_depends check_speed_profile)
ENDIF ( cppad_profile_flag )
#
IF ( adolc_prefix )
	ADD_SUBDIRECTORY(adolc)
	add_to_list(check_depends check_speed_adolc)
ENDIF( adolc_prefix )
IF ( fadbad_prefix )
	ADD_SUBDIRECTORY(fadbad)
	add_to_list(check_depends check_speed_fadbad)
ENDIF( fadbad_prefix )
IF ( sacado_prefix )
	ADD_SUBDIRECTORY(sacado)
	add_to_list(check_depends check_speed_sacado)
ENDIF( sacado_prefix )

# check_speed
ADD_CUSTOM_TARGET(check_speed DEPENDS ${check_depends} )
