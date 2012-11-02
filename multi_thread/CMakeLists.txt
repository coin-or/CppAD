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

# Define the operation
# CHECK_LIBRARY_EXISTS (LIBRARY FUNCTION LOCATION VARIABLE)
#	LIBRARY  - the name of the library you are looking for
#	FUNCTION - the name of the function
#	LOCATION - location where the library should be found
#	VARIABLE - variable to store the result
INCLUDE(CheckLibraryExists)

# Define the operation
# CHECK_CXX_SOURCE_COMPILES(source variable)
# Checks whether the code given in source will compile and link.  You can set 
# CMAKE_REQUIRED_LIBRARIES, CMAKE_REQUIRED_FLAGS and CMAKE_REQUIRED_INCLUDES 
# accordingly if additional libraries or compiler flags are required. 
INCLUDE(CheckCXXSourceCompiles)

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
	CHECK_LIBRARY_EXISTS(
		pthread pthread_barrier_wait ${pthread_lib_path} pthread_ok
	)   
	IF ( pthread_ok )
		ADD_SUBDIRECTORY(pthread)
	ENDIF ( pthread_ok )
ENDIF ( pthread_lib_path )

# check for boost threads
FIND_PACKAGE(Boost)
IF ( Boost_FOUND )
	# Boost_INCLUDE_DIRS:         boost include directories
	MESSAGE(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")

	# boost_prefix:
	# Extract the Boost prefix (only works on unix systems)
	# string(REGEX REPLACE 
	#	<regular_expression> <replace_expression> 
	#	<output variable> <input> [<input>...])
	STRING(REGEX REPLACE 
		"(.*)/[^/]*" "\\1" 
		boost_prefix "${Boost_INCLUDE_DIRS}" 
	) 
	#
	# boost_thread_ok, boost_thread_mt_ok:
	#
	SET(CMAKE_REQUIRED_LIBRARIES "boost_thread")
	SET(CMAKE_REQUIRED_FLAGS "-L${boost_prefix}/${cmake_install_libdir}" )
	SET(CMAKE_REQUIRED_INCLUDES  "${Boost_INCLUDE_DIRS}")
	#
	# CHECK_CXX_SOURCE_COMPILES(source variable)
	SET(source "
# include <boost/thread.hpp>
main(void) 
{	boost::barrier wait(1);
	return 0;
}"	)
	CHECK_CXX_SOURCE_COMPILES("${source}" bthread_ok)
	#
	SET(CMAKE_REQUIRED_LIBRARIES "boost_thread-mt")
	CHECK_CXX_SOURCE_COMPILES("${source}" bthread_mt_ok)
ENDIF ( Boost_FOUND )

