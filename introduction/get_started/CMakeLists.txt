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
# Build the introduction/get_started directory tests 

# Specifies build type for this directory. Possible values are 
# empty, Debug, Release, RelWithDebInfo and MinSizeRel
SET(CMAKE_BUILD_TYPE DEBUG)
#
ADD_EXECUTABLE(introduction_get_started EXCLUDE_FROM_ALL
	get_started.cpp
)
# Compiler flags for cppad source
add_cppad_cxx_flags( introduction_get_started )

# Add the check_speed_example target
ADD_CUSTOM_TARGET(check_introduction_get_started 
	introduction_get_started 
	DEPENDS introduction_get_started 
)
