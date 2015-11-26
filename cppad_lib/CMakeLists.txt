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
# Build and install the cppad_lib shared library

#  add_library(<name> [STATIC | SHARED | MODULE] [EXCLUDE_FROM_ALL]
#	source1 source2 ... sourceN)
# )
ADD_LIBRARY( cppad_lib SHARED 
	cppad_colpack.cpp
)

# Add extra compiler flags
add_cppad_cxx_flags( cppad_lib )

# install(TARGETS myExe mySharedLib myStaticLib
#	RUNTIME DESTINATION bin
#	LIBRARY DESTINATION lib
#	ARCHIVE DESTINATION lib/static)
INSTALL(TARGETS cppad_lib DESTINATION ${cppad_abs_libdir})
