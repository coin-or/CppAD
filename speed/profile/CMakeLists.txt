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
# Build the speed/profile directory tests
# Inherit environment from ../CMakeList.txt
# CMAKE_BUILD_TYPE determined by parent directory

# Adds flags to the compiler command line for sources in the current directory
# and below. This command can be used to add any flags, but it was originally
# intended to add preprocessor definitions.
ADD_DEFINITIONS("-DCPPAD_PROFILE_SPEED ${cppad_profile_flag}")

# Extra flags used by the linker when creating an executable.
SET(CMAKE_EXE_LINKER_FLAGS ${cppad_profile_flag})

# Local include directories to search (not in package_prefix/includdir)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../src )

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
ADD_EXECUTABLE(speed_profile EXCLUDE_FROM_ALL ../main.cpp
	../cppad/ode.cpp
	../cppad/det_lu.cpp
	../cppad/det_minor.cpp
	../cppad/mat_mul.cpp
	../cppad/poly.cpp
	../cppad/sparse_hessian.cpp
	../cppad/sparse_jacobian.cpp
	../src/link_det_lu.cpp
	../src/link_det_minor.cpp
	../src/link_mat_mul.cpp
	../src/link_ode.cpp
	../src/link_poly.cpp
	../src/link_sparse_hessian.cpp
	../src/link_sparse_jacobian.cpp
	../src/microsoft_timer.cpp
)

# Compiler flags for cppad source
add_cppad_cxx_flags( speed_profile )

# profile builds it own copy of src/speed library (see ADD_EXECUTABLE above)
# TARGET_LINK_LIBRARIES(speed_profile speed_src )
TARGET_LINK_LIBRARIES(speed_profile
	${cppad_lib}
	${colpack_libs}
)

# Add the check_speed_profile target
ADD_CUSTOM_TARGET(check_speed_profile
	speed_profile correct 54321
	DEPENDS speed_profile
)

# Sed script to make gprof output more readable, use:
#	./speed_profile speed 54321
#	gprof speed_profile gmon.out | sed -f 'gprof.sed' > gprof.out
#
# configure_file(InputFile OutputFile [COPYONLY] [ESCAPE_QUOTES] [@ONLY])
CONFIGURE_FILE(
	${CMAKE_CURRENT_SOURCE_DIR}/gprof.sed.in
	${CMAKE_CURRENT_BINARY_DIR}/gprof.sed
	COPYONLY
)
