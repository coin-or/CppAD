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
# Build the cppad_ipopt/src library
# Inherit environment from ../CMakeList.txt

# Specifies build type for this directory. Possible values are
# empty, Debug, Release, RelWithDebInfo and MinSizeRel
SET(CMAKE_BUILD_TYPE DEBUG)

#  add_library(<name> [STATIC | SHARED | MODULE] [EXCLUDE_FROM_ALL]
#	source1 source2 ... sourceN)
# )
ADD_LIBRARY(cppad_ipopt
	cppad_ipopt_nlp.cpp
	hes_fg_map.cpp
	jac_g_map.cpp
	sparse_map2vec.cpp
	vec_fun_pattern.cpp
)

# Add extra compiler flags
add_cppad_cxx_flags( cppad_ipopt )

# install(FILES files... DESTINATION <dir>
#	[PERMISSIONS permissions...]
#	[CONFIGURATIONS [Debug|Release|...]]
#	[COMPONENT <component>]
#	[RENAME <name>] [OPTIONAL])
INSTALL(FILES cppad_ipopt_nlp.hpp DESTINATION ${cppad_abs_includedir})

# install(TARGETS myExe mySharedLib myStaticLib
#	RUNTIME DESTINATION bin
#	LIBRARY DESTINATION lib
#	ARCHIVE DESTINATION lib/static)
INSTALL(TARGETS cppad_ipopt DESTINATION ${cppad_abs_libdir})
