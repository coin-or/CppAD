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
# Build the multi_thread/* directory tests 
# Inherit environment from ../CMakeList.txt

# check for openmp
FIND_PACKAGE(OpenMP)
IF ( OPENMP_FOUND )
	# OpenMP_CXX_FLAGS - flags to add to the CXX compiler for OpenMP support
	ADD_SUBDIRECTORY(openmp)
ENDIF ( OPENMP_FOUND )

# check for pthreads
# find_library(<VAR> name1 [path1 path2 ...])
FIND_LIBRARY(pthread_lib_path pthread)
MESSAGE(STATUS "pthread library path = ${pthread_lib_path}")
IF ( pthread_lib_path )
	INCLUDE(CheckLibraryExists)
	# LIBRARY  - the name of the library you are looking for
	# FUNCTION - the name of the function
	# LOCATION - location where the library should be found
	# VARIABLE - variable to store the result
	# CHECK_LIBRARY_EXISTS (LIBRARY FUNCTION LOCATION VARIABLE)
	CHECK_LIBRARY_EXISTS(
		pthread pthread_barrier_wait ${pthread_lib_path} pthread_ok
	)   
	IF ( pthread_ok )
		ADD_SUBDIRECTORY(pthread)
	ENDIF ( pthread_ok )
ENDIF ( pthread_lib_path )
