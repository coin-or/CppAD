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
# Build the cppad_ipopt/speed directory tests
# Inherit environment from ../CMakeList.txt

# Specifies build type for this directory. Possible values are
# empty, Debug, Release, RelWithDebInfo and MinSizeRel
SET(CMAKE_BUILD_TYPE RELEASE)

# Local include directories to search (not in package_prefix/include)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../src )

# Local link directories to search (not in external packages)
# (cannot use ../src/cppad_ipopt library because it is linke with debugging)
LINK_DIRECTORIES( ${ipopt_LIBRARY_DIRS} )

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
# Include source for cppad_ipopt library so we build it without debugging
ADD_EXECUTABLE(cppad_ipopt_speed EXCLUDE_FROM_ALL
	speed.cpp
	../src/cppad_ipopt_nlp.cpp
	../src/fun_record.hpp
	../src/hes_fg_map.cpp
	../src/jac_g_map.cpp
	../src/sparse_map2vec.cpp
	../src/vec_fun_pattern.cpp
	ode_speed.cpp
)

# Add extra compiler flags
add_cppad_cxx_flags( cppad_ipopt_speed )

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(
	cppad_ipopt_speed
	cppad_ipopt
	${cppad_lib}
	${ipopt_LIBRARIES}
	${colpack_libs}
)

# Add the check_cppad_ipopt_speed target
ADD_CUSTOM_TARGET(check_cppad_ipopt_speed
	cppad_ipopt_speed
	DEPENDS cppad_ipopt_speed cppad_ipopt
)
