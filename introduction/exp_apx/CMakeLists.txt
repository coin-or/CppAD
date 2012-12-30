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
# Build the introduction/exp_apx directory tests 

# Specifies build type for this directory. Possible values are 
# empty, Debug, Release, RelWithDebInfo and MinSizeRel
SET(CMAKE_BUILD_TYPE DEBUG)
#
ADD_EXECUTABLE(introduction_exp_apx EXCLUDE_FROM_ALL
	main.cpp
	exp_2.cpp
	exp_2.hpp
	exp_2_cppad.cpp
	exp_2_for1.cpp
	exp_2_for2.cpp
	exp_2_rev1.cpp
	exp_2_rev2.cpp
	exp_2_for0.cpp
	exp_eps.cpp 
	exp_eps.hpp
	exp_eps_cppad.cpp
	exp_eps_for1.cpp
	exp_eps_for2.cpp
	exp_eps_for0.cpp
	exp_eps_rev1.cpp
	exp_eps_rev2.cpp
)
# Compiler flags for cppad source
add_cppad_cxx_flags( introduction_exp_apx )

# Add the check_speed_example target
ADD_CUSTOM_TARGET(check_introduction_exp_apx 
	introduction_exp_apx 
	DEPENDS introduction_exp_apx 
)
