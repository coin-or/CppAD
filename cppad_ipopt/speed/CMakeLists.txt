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
ADD_EXECUTABLE(speed_ipopt_nlp EXCLUDE_FROM_ALL 
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
add_cppad_cxx_flags( speed_ipopt_nlp )

# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(speed_ipopt_nlp cppad_ipopt ${ipopt_LIBRARIES})

# Add the check_speed_ipopt_nlp target
ADD_CUSTOM_TARGET(check_speed_ipopt_nlp
	speed_ipopt_nlp
	DEPENDS speed_ipopt_nlp cppad_ipopt
)
