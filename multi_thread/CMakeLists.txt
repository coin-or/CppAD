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

# initialize list as empty
SET(list_multi_thread "")

# Define the operation
# CHECK_LIBRARY_EXISTS (LIBRARY FUNCTION LOCATION VARIABLE)
#	LIBRARY  - the name of the library you are looking for
#	FUNCTION - the name of the function
#	LOCATION - location where the library should be found
#	VARIABLE - variable to store the result
INCLUDE(CheckLibraryExists)

# If openmp found, set OpenMP_CXX_FLAGS and process its subdirectory
#
FIND_PACKAGE(OpenMP)
IF ( OPENMP_FOUND )
	# OpenMP_CXX_FLAGS - flags to add to the CXX compiler for OpenMP support
	ADD_SUBDIRECTORY(openmp)
	add_to_list(list_multi_thread check_openmp)
ENDIF ( OPENMP_FOUND )

# If pthreads found, set pthread_lib and pthread_lib_path and process subdir
#
# find_library(<VAR> name1 [path1 path2 ...])
SET(pthread_lib "pthread")
FIND_LIBRARY(pthread_lib_path pthread)
MESSAGE(STATUS "pthread library path = ${pthread_lib_path}")
IF ( pthread_lib_path )
	CHECK_LIBRARY_EXISTS(
		pthread pthread_barrier_wait ${pthread_lib_path} pthread_ok
	)   
	IF ( pthread_ok )
		ADD_SUBDIRECTORY(pthread)
		add_to_list(list_multi_thread check_pthread)
	ENDIF ( pthread_ok )
ENDIF ( pthread_lib_path )

# If boost threads found, set 
# bthread_lib, bthread_lib_path and process its subdir
# 
# FIND_PACKAGE(Boost) was done by ../CMakeLists.txt
IF ( Boost_FOUND )
	#
	SET(CMAKE_REQUIRED_INCLUDES  "${boost_prefix}/${cmake_install_includdir}")
	#
	SET(bthread_lib               "boost_thread" )
	SET(CMAKE_REQUIRED_LIBRARIES "${bthread_lib}" )
	#
	SET(bthread_lib_path "${boost_prefix}/${cppad_install_libdir}" )
	SET(CMAKE_REQUIRED_FLAGS     "-L${bthread_lib_path}" )
	#
	# CHECK_CXX_SOURCE_RUNS(source variable)
	SET(source "
# include <boost/thread.hpp>
int main(void) 
{	boost::barrier wait(1);
	return 0;
}"	)
	CHECK_CXX_SOURCE_RUNS("${source}" bthread_ok)
	#
	IF ( NOT bthread_ok )
		SET(bthread_lib               "boost_thread-mt" )
		SET(CMAKE_REQUIRED_LIBRARIES "${bthread_lib}" )
		CHECK_CXX_SOURCE_RUNS("${source}" bthread_mt_ok)
	ENDIF ( NOT bthread_ok )
	IF ( bthread_ok OR bthread_mt_ok )
		ADD_SUBDIRECTORY(bthread)
		add_to_list(list_multi_thread check_bthread)
	ENDIF ( bthread_ok OR bthread_mt_ok )
ENDIF ( Boost_FOUND )

# check_multi_thread
ADD_CUSTOM_TARGET(check_multi_thread DEPENDS ${list_multi_thread})
